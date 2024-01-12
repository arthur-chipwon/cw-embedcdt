/**
 * @file hsspi_driver.c
 * @brief The module provides for requiring HSSPI hardwre registers access.
 * Export functions for wide scope use.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "hsspi_driver.h"

/*******************************************************************************
 * Private macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/

/*******************************************************************************
 * Private typedefs
 ******************************************************************************/

/*******************************************************************************
 * Global variables
 ******************************************************************************/
/** @brief Table of base pointers for HSSPI instances. */
HSSPI_t *const gp_hsspi_base[HSSPI_INSTANCE_COUNT] = HSSPI_BASE_PTRS;

/** @brief Table to save HSSPI IRQ enumeration numbers defined in the CMSIS
 * header file. */
const IRQn_t g_hsspi_irq_id[HSSPI_INSTANCE_COUNT] = HSSPI_IRQS;

/** @brief Table to save HSSPI clock names as defined in clock manager. */
const clock_names_t g_hsspi_clk_names[HSSPI_INSTANCE_COUNT] = HSSPI_CLOCK_NAMES;

/*******************************************************************************
 * Static variables
 ******************************************************************************/

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/

/*******************************************************************************
 * Global functions
 ******************************************************************************/
void
hsspi_init(HSSPI_t *p_base)
{
    hsspi_reset_signal_assert(p_base, true);
    hsspi_reset_signal_assert(p_base, false);
}

void
hsspi_reset_signal_assert(HSSPI_t *p_base, bool b_flag)
{
    p_base->RST = HSSPI_RST_ACT(b_flag);
}

uint32_t
hsspi_get_status_bm(HSSPI_t *p_base)
{
    return p_base->SR;
}

void
hsspi_clear_status_bm(HSSPI_t *p_base, uint32_t status_bm)
{
    p_base->SR = status_bm;
}

void
hsspi_set_rpmc_enable(HSSPI_t *p_base, bool b_enable)
{
    if (true == b_enable)
    {
        p_base->FUN |= (uint32_t)1u << HSSPI_FUN_RPEN_SHIFT;
    }
    else
    {
        p_base->FUN &= ~((uint32_t)1u << HSSPI_FUN_RPEN_SHIFT);
    }
}

void
hsspi_fillup_response_status(HSSPI_t *p_base, uint8_t status)
{
    p_base->RSPSR = (uint32_t)status; // status & HSSPI_RSPSR_STS_MASK;
}

void
hsspi_fillup_rw_opcode(HSSPI_t *p_base, uint8_t read_opcode, uint8_t write_opcode)
{
    p_base->OCR = (read_opcode << HSSPI_OCR_RD_SHIFT) | (write_opcode << HSSPI_OCR_WR_SHIFT);
}

void
hsspi_set_oe_bitcount(HSSPI_t *p_base, uint8_t value)
{
    p_base->SOR &= ~HSSPI_SOR_BC_MASK;
    p_base->SOR |= (uint32_t)value & HSSPI_SOR_BC_MASK;
}

void
hsspi_set_clk_skew_cancellation_on_off(HSSPI_t *p_base, bool b_enable)
{
    if (true == b_enable)
    {
        p_base->SOR |= (uint32_t)1u << HSSPI_SOR_CTS_SHIFT;
    }
    else
    {
        p_base->SOR &= ~((uint32_t)1u << HSSPI_SOR_CTS_SHIFT);
    }
}

uint16_t
hsspi_get_rx_len(HSSPI_t *p_base)
{
    return (uint16_t)p_base->PL;
}

void
hsspi_set_tx_sram_update_completed(HSSPI_t *p_base)
{
    p_base->INTMSK |= HSSPI_INTMSK_UPD_MASK;
}

void
hsspi_set_tpm_fun_enable(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->FUN;
    tmp &= ~(HSSPI_FUN_TPMEN_MASK);
    tmp |= HSSPI_FUN_TPMEN(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->FUN = tmp;
}

void
hsspi_set_tpm_wait_enable(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->FUN;
    tmp &= ~(HSSPI_FUN_TPMWAIT_MASK);
    tmp |= HSSPI_FUN_TPMWAIT(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->FUN = tmp;
}

void
hsspi_set_tpm_fifowait_enable(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->FUN;
    tmp &= ~(HSSPI_FUN_TPMFIFOWAIT_MASK);
    tmp |= HSSPI_FUN_TPMFIFOWAIT(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->FUN = tmp;
}

/**
 * @brief TPM locality header.
 *
 * @param[in] p_base  - HSSPI base pointer
 * @param[in] head - TPM locality header.
 */
void
hsspi_tpm_loc_head(HSSPI_t *p_base, uint8_t head)
{
    uint32_t tmp = p_base->LOC;
    tmp &= ~(HSSPI_TPM_LOCHEAD_MASK);
    tmp |= HSSPI_TPM_LOCHEAD(head);
    p_base->LOC = tmp;
}

void
hsspi_tpm_loc_num_sel(HSSPI_t *p_base, tpm_loc_num_sel_t loc_sel)
{
    uint32_t tmp = p_base->LOC;
    tmp &= ~(HSSPI_TPM_LOCNUM0_MASK | HSSPI_TPM_LOCNUM1_MASK | HSSPI_TPM_LOCNUM2_MASK |
             HSSPI_TPM_LOCNUM3_MASK | HSSPI_TPM_LOCNUM4_MASK | HSSPI_TPM_BYPASSLOCCHK_MASK |
             HSSPI_TPM_BYPASSLOCNUM_MASK);
    tmp |= loc_sel;
    p_base->LOC = tmp;
}

/**
 * @brief TPM locality number0 selection.
 *
 * @param[in] p_base  - HSSPI base pointer
 * @param[in] b_enable - TPM locality number0 enable.
 */
void
hsspi_tpm_loc_num0_sel(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->LOC;
    tmp &= ~(HSSPI_TPM_LOCNUM0_MASK);
    tmp |= HSSPI_TPM_LOCNUM0(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->LOC = tmp;
}

/**
 * @brief TPM locality number1 selection.
 *
 * @param[in] p_base  - HSSPI base pointer
 * @param[in] b_enable - TPM locality number1 enable.
 */
void
hsspi_tpm_loc_num1_sel(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->LOC;
    tmp &= ~(HSSPI_TPM_LOCNUM1_MASK);
    tmp |= HSSPI_TPM_LOCNUM1(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->LOC = tmp;
}

/**
 * @brief TPM locality number2 selection.
 *
 * @param[in] p_base  - HSSPI base pointer
 * @param[in] b_enable - TPM locality number2 enable.
 */
void
hsspi_tpm_loc_num2_sel(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->LOC;
    tmp &= ~(HSSPI_TPM_LOCNUM2_MASK);
    tmp |= HSSPI_TPM_LOCNUM2(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->LOC = tmp;
}

/**
 * @brief TPM locality number3 selection.
 *
 * @param[in] p_base  - HSSPI base pointer
 * @param[in] b_enable - TPM locality number3 enable.
 */
void
hsspi_tpm_loc_num3_sel(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->LOC;
    tmp &= ~(HSSPI_TPM_LOCNUM3_MASK);
    tmp |= HSSPI_TPM_LOCNUM3(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->LOC = tmp;
}

/**
 * @brief TPM locality number4 selection.
 *
 * @param[in] p_base  - HSSPI base pointer
 * @param[in] b_enable - TPM locality number4 enable.
 */
void
hsspi_tpm_loc_num4_sel(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->LOC;
    tmp &= ~(HSSPI_TPM_LOCNUM4_MASK);
    tmp |= HSSPI_TPM_LOCNUM4(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->LOC = tmp;
}

/**
 * @brief TPM locality bypass check.
 *
 * @param[in] p_base  - HSSPI base pointer
 * @param[in] b_enable - TPM locality bypass check enable.
 */
void
hsspi_tpm_byp_locchk(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->LOC;
    tmp &= ~(HSSPI_TPM_BYPASSLOCCHK_MASK);
    tmp |= HSSPI_TPM_BYPASSLOCCHK(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->LOC = tmp;
}

/**
 * @brief TPM locality number bypass check.
 *
 * @param[in] p_base  - HSSPI base pointer
 * @param[in] enable - TPM locality number bypass check enable.
 */
void
hsspi_tpm_byp_locnum(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->LOC;
    tmp &= ~(HSSPI_TPM_BYPASSLOCNUM_MASK);
    tmp |= HSSPI_TPM_BYPASSLOCNUM(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->LOC = tmp;
}

/**
 * @brief Get TPM locality register.
 *
 * @param[in] p_base  - HSSPI base pointer
 */
uint32_t
hsspi_tpm_get_locality_all_reg(HSSPI_t *p_base)
{
    return p_base->LOC;
}

uint32_t
hsspi_get_tpm_access_reg(HSSPI_t *p_base)
{
    return (uint32_t)(p_base->ACCESS);
}

void
hsspi_set_tpm_access_reg(HSSPI_t *p_base, uint32_t reg_value)
{
    p_base->ACCESS = reg_value;
}

bool
hsspi_get_tpm_access_establishment(HSSPI_t *p_base)
{
    return ((p_base->ACCESS & HSSPI_TPM_ESTB_MASK) >> HSSPI_TPM_ESTB_SHIFT) != 0U;
}

void
hsspi_set_tpm_access_establishment(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->ACCESS;
    tmp &= ~(HSSPI_TPM_ESTB_MASK);
    tmp |= HSSPI_TPM_ESTB(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->ACCESS = tmp;
}

bool
hsspi_get_tpm_access_requestUse(HSSPI_t *p_base)
{
    return ((p_base->ACCESS & HSSPI_TPM_REQ_MASK) >> HSSPI_TPM_REQ_SHIFT) != 0U;
}

void
hsspi_set_tpm_access_requestUse(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->ACCESS;
    tmp &= ~(HSSPI_TPM_REQ_MASK);
    tmp |= HSSPI_TPM_REQ(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->ACCESS = tmp;
}

bool
hsspi_get_tpm_access_pendingRequest(HSSPI_t *p_base)
{
    return ((p_base->ACCESS & HSSPI_TPM_PENDREQ_MASK) >> HSSPI_TPM_PENDREQ_SHIFT) != 0U;
}

void
hsspi_set_tpm_access_pendingRequest(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->ACCESS;
    tmp &= ~(HSSPI_TPM_PENDREQ_MASK);
    tmp |= HSSPI_TPM_PENDREQ(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->ACCESS = tmp;
}

bool
hsspi_get_tpm_access_seize(HSSPI_t *p_base)
{
    return ((p_base->ACCESS & HSSPI_TPM_SEIZE_MASK) >> HSSPI_TPM_SEIZE_SHIFT) != 0U;
}

void
hsspi_set_tpm_access_seize(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->ACCESS;
    tmp &= ~(HSSPI_TPM_SEIZE_MASK);
    tmp |= HSSPI_TPM_SEIZE(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->ACCESS = tmp;
}

bool
hsspi_get_tpm_access_beenseize(HSSPI_t *p_base)
{
    return ((p_base->ACCESS & HSSPI_TPM_BSEIZE_MASK) >> HSSPI_TPM_BSEIZE_SHIFT) != 0U;
}

void
hsspi_set_tpm_access_beenseize(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->ACCESS;
    tmp &= ~(HSSPI_TPM_BSEIZE_MASK);
    tmp |= HSSPI_TPM_BSEIZE(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->ACCESS = tmp;
}

bool
hsspi_get_tpm_access_activeLocality(HSSPI_t *p_base)
{
    return ((p_base->ACCESS & HSSPI_TPM_ACTLOC_MASK) >> HSSPI_TPM_ACTLOC_SHIFT) != 0U;
}

void
hsspi_set_tpm_access_activeLocality(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->ACCESS;
    tmp &= ~(HSSPI_TPM_ACTLOC_MASK);
    tmp |= HSSPI_TPM_ACTLOC(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->ACCESS = tmp;
}

bool
hsspi_get_tpm_access_tpmRegValidSts(HSSPI_t *p_base)
{
    return ((p_base->ACCESS & HSSPI_TPM_REGVALID_MASK) >> HSSPI_TPM_REGVALID_SHIFT) != 0U;
}

void
hsspi_set_tpm_access_tpmRegValidSts(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->ACCESS;
    tmp &= ~(HSSPI_TPM_REGVALID_MASK);
    tmp |= HSSPI_TPM_REGVALID(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->ACCESS = tmp;
}

uint32_t
hsspi_get_tpm_inten_reg(HSSPI_t *p_base)
{
    return (uint32_t)(p_base->INTEN);
}

void
hsspi_set_tpm_inten_reg(HSSPI_t *p_base, uint32_t reg_value)
{
    p_base->INTEN = reg_value;
}

bool
hsspi_get_tpm_inten_dataavailint(HSSPI_t *p_base)
{
    return ((p_base->INTEN & HSSPI_TPM_AVAINTEN_MASK) >> HSSPI_TPM_AVAINTEN_SHIFT) != 0U;
}

void
hsspi_set_tpm_inten_dataavailint(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->INTEN;
    tmp &= ~(HSSPI_TPM_AVAINTEN_MASK);
    tmp |= HSSPI_TPM_AVAINTEN(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTEN = tmp;
}

bool
hsspi_get_tpm_inten_stsvalidint(HSSPI_t *p_base)
{
    return ((p_base->INTEN & HSSPI_TPM_STSINTEN_MASK) >> HSSPI_TPM_STSINTEN_SHIFT) != 0U;
}

void
hsspi_set_tpm_inten_stsvalidint(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->INTEN;
    tmp &= ~(HSSPI_TPM_STSINTEN_MASK);
    tmp |= HSSPI_TPM_STSINTEN(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTEN = tmp;
}

bool
hsspi_get_tpm_inten_localitychangeint(HSSPI_t *p_base)
{
    return ((p_base->INTEN & HSSPI_TPM_LOCCHGINTEN_MASK) >> HSSPI_TPM_LOCCHGINTEN_SHIFT) != 0U;
}

void
hsspi_set_tpm_inten_localitychangeint(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->INTEN;
    tmp &= ~(HSSPI_TPM_LOCCHGINTEN_MASK);
    tmp |= HSSPI_TPM_LOCCHGINTEN(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTEN = tmp;
}

tpm_inten_typpol_t
hsspi_get_tpm_inten_typepolarity(HSSPI_t *p_base)
{
    uint32_t tmp = p_base->INTEN;
    tmp          = (tmp & HSSPI_TPM_TYPPOL_MASK) >> HSSPI_TPM_TYPPOL_SHIFT;
    return (tpm_inten_typpol_t)(tmp);
}

void
hsspi_set_tpm_inten_typepolarity(HSSPI_t *p_base, tpm_inten_typpol_t polarity)
{
    uint32_t tmp = p_base->INTEN;
    tmp &= ~(HSSPI_TPM_TYPPOL_MASK);
    tmp |= HSSPI_TPM_TYPPOL(polarity);
    p_base->INTEN = tmp;
}

bool
hsspi_get_tpm_inten_commandready(HSSPI_t *p_base)
{
    return ((p_base->INTEN & HSSPI_TPM_CMDRDYEN_MASK) >> HSSPI_TPM_CMDRDYEN_SHIFT) != 0U;
}

void
hsspi_set_tpm_inten_commandready(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->INTEN;
    tmp &= ~(HSSPI_TPM_CMDRDYEN_MASK);
    tmp |= HSSPI_TPM_CMDRDYEN(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTEN = tmp;
}

bool
hsspi_get_tpm_inten_globalint(HSSPI_t *p_base)
{
    return ((p_base->INTEN & HSSPI_TPM_GLBINTEN_MASK) >> HSSPI_TPM_GLBINTEN_SHIFT) != 0U;
}

void
hsspi_set_tpm_inten_globalint(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->INTEN;
    tmp &= ~(HSSPI_TPM_GLBINTEN_MASK);
    tmp |= HSSPI_TPM_GLBINTEN(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTEN = tmp;
}

uint8_t
hsspi_get_tpm_intvector_sirqvec(HSSPI_t *p_base)
{
    return (uint8_t)((((p_base)->VECTOR) & HSSPI_TPM_SIRQVEC_MASK) >> HSSPI_TPM_SIRQVEC_SHIFT);
}

void
hsspi_set_tpm_intvector_sirqvec(HSSPI_t *p_base, uint8_t vector)
{
    uint32_t tmp = p_base->VECTOR;
    tmp &= ~(HSSPI_TPM_SIRQVEC_MASK);
    tmp |= HSSPI_TPM_SIRQVEC(vector);
    p_base->VECTOR = tmp;
}

uint32_t
hsspi_get_tpm_intstatus_reg(HSSPI_t *p_base)
{
    return (uint32_t)(p_base->INTSTS);
}

void
hsspi_set_tpm_intstatus_reg(HSSPI_t *p_base, uint32_t reg_value)
{
    p_base->INTSTS = reg_value;
}

bool
hsspi_get_tpm_intstatus_dataAvailInt(HSSPI_t *p_base)
{
    return ((p_base->INTSTS & HSSPI_TPM_AVAINTOC_MASK) >> HSSPI_TPM_AVAINTOC_SHIFT) != 0U;
}

void
hsspi_set_tpm_intstatus_dataAvailInt(HSSPI_t *p_base, bool b_clear)
{
    uint32_t tmp = p_base->INTSTS;
    tmp &= ~(HSSPI_TPM_AVAINTOC_MASK);
    tmp |= HSSPI_TPM_AVAINTOC(b_clear ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTSTS = tmp;
}

bool
hsspi_get_tpm_intstatus_stsValidInt(HSSPI_t *p_base)
{
    return ((p_base->INTSTS & HSSPI_TPM_STSINTOC_MASK) >> HSSPI_TPM_STSINTOC_SHIFT) != 0U;
}

void
hsspi_set_tpm_intstatus_stsValidInt(HSSPI_t *p_base, bool b_clear)
{
    uint32_t tmp = p_base->INTSTS;
    tmp &= ~(HSSPI_TPM_STSINTOC_MASK);
    tmp |= HSSPI_TPM_STSINTOC(b_clear ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTSTS = tmp;
}

bool
hsspi_get_tpm_intstatus_localityChangeInt(HSSPI_t *p_base)
{
    return ((p_base->INTSTS & HSSPI_TPM_LOCCHGINTOC_MASK) >> HSSPI_TPM_LOCCHGINTOC_SHIFT) != 0U;
}

void
hsspi_set_tpm_intstatus_localityChangeInt(HSSPI_t *p_base, bool b_clear)
{
    uint32_t tmp = p_base->INTSTS;
    tmp &= ~(HSSPI_TPM_LOCCHGINTOC_MASK);
    tmp |= HSSPI_TPM_LOCCHGINTOC(b_clear ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTSTS = tmp;
}

bool
hsspi_get_tpm_intstatus_commandReadyInt(HSSPI_t *p_base)
{
    return ((p_base->INTSTS & HSSPI_TPM_CMDRDYOC_MASK) >> HSSPI_TPM_CMDRDYOC_SHIFT) != 0U;
}

void
hsspi_set_tpm_intstatus_commandReadyInt(HSSPI_t *p_base, bool b_clear)
{
    uint32_t tmp = p_base->INTSTS;
    tmp &= ~(HSSPI_TPM_CMDRDYOC_MASK);
    tmp |= HSSPI_TPM_CMDRDYOC(b_clear ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTSTS = tmp;
}

uint32_t
hsspi_get_tpm_intfcap_reg(HSSPI_t *p_base)
{
    return (uint32_t)(p_base->INTFCAP);
}

void
hsspi_set_tpm_intfcap_reg(HSSPI_t *p_base, uint32_t reg_value)
{
    p_base->INTFCAP = reg_value;
}

bool
hsspi_get_tpm_intfcap_dataAvailInt(HSSPI_t *p_base)
{
    return ((p_base->INTFCAP & HSSPI_TPM_AVAINTSP_MASK) >> HSSPI_TPM_AVAINTSP_SHIFT) != 0U;
}

void
hsspi_set_tpm_intfcap_dataAvailInt(HSSPI_t *p_base, bool b_support)
{
    uint32_t tmp = p_base->INTFCAP;
    tmp &= ~(HSSPI_TPM_AVAINTSP_MASK);
    tmp |= HSSPI_TPM_AVAINTSP(b_support ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTFCAP = tmp;
}

bool
hsspi_get_tpm_intfcap_stsValidInt(HSSPI_t *p_base)
{
    return ((p_base->INTFCAP & HSSPI_TPM_STSINTSP_MASK) >> HSSPI_TPM_STSINTSP_SHIFT) != 0U;
}

void
hsspi_set_tpm_intfcap_stsValidInt(HSSPI_t *p_base, bool b_support)
{
    uint32_t tmp = p_base->INTFCAP;
    tmp &= ~(HSSPI_TPM_STSINTSP_MASK);
    tmp |= HSSPI_TPM_STSINTSP(b_support ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTFCAP = tmp;
}

bool
hsspi_get_tpm_intfcap_localityChangeInt(HSSPI_t *p_base)
{
    return ((p_base->INTFCAP & HSSPI_TPM_LOCCHGINTSP_MASK) >> HSSPI_TPM_LOCCHGINTSP_SHIFT) != 0U;
}

void
hsspi_set_tpm_intfcap_localityChangeInt(HSSPI_t *p_base, bool b_support)
{
    uint32_t tmp = p_base->INTFCAP;
    tmp &= ~(HSSPI_TPM_LOCCHGINTSP_MASK);
    tmp |= HSSPI_TPM_LOCCHGINTSP(b_support ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTFCAP = tmp;
}

bool
hsspi_get_tpm_intfcap_InterruptLevelHigh(HSSPI_t *p_base)
{
    return ((p_base->INTFCAP & HSSPI_TPM_HIGHLVINT_MASK) >> HSSPI_TPM_HIGHLVINT_SHIFT) != 0U;
}

void
hsspi_set_tpm_intfcap_InterruptLevelHigh(HSSPI_t *p_base, bool b_support)
{
    uint32_t tmp = p_base->INTFCAP;
    tmp &= ~(HSSPI_TPM_HIGHLVINT_MASK);
    tmp |= HSSPI_TPM_HIGHLVINT(b_support ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTFCAP = tmp;
}

bool
hsspi_get_tpm_intfcap_InterruptLevelLow(HSSPI_t *p_base)
{
    return ((p_base->INTFCAP & HSSPI_TPM_LOWLVINT_MASK) >> HSSPI_TPM_LOWLVINT_SHIFT) != 0U;
}

void
hsspi_set_tpm_intfcap_InterruptLevelLow(HSSPI_t *p_base, bool b_support)
{
    uint32_t tmp = p_base->INTFCAP;
    tmp &= ~(HSSPI_TPM_LOWLVINT_MASK);
    tmp |= HSSPI_TPM_LOWLVINT(b_support ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTFCAP = tmp;
}

bool
hsspi_get_tpm_intfcap_InterruptEdgeRising(HSSPI_t *p_base)
{
    return ((p_base->INTFCAP & HSSPI_TPM_EDGRISEINT_MASK) >> HSSPI_TPM_EDGRISEINT_SHIFT) != 0U;
}

void
hsspi_set_tpm_intfcap_InterruptEdgeRising(HSSPI_t *p_base, bool b_support)
{
    uint32_t tmp = p_base->INTFCAP;
    tmp &= ~(HSSPI_TPM_EDGRISEINT_MASK);
    tmp |= HSSPI_TPM_EDGRISEINT(b_support ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTFCAP = tmp;
}

bool
hsspi_get_tpm_intfcap_InterruptEdgeFalling(HSSPI_t *p_base)
{
    return ((p_base->INTFCAP & HSSPI_TPM_EDGFALLINT_MASK) >> HSSPI_TPM_EDGFALLINT_SHIFT) != 0U;
}

void
hsspi_set_tpm_intfcap_InterruptEdgeFalling(HSSPI_t *p_base, bool b_support)
{
    uint32_t tmp = p_base->INTFCAP;
    tmp &= ~(HSSPI_TPM_EDGFALLINT_MASK);
    tmp |= HSSPI_TPM_EDGFALLINT(b_support ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTFCAP = tmp;
}

bool
hsspi_get_tpm_intfcap_CommandReadyInt(HSSPI_t *p_base)
{
    return ((p_base->INTFCAP & HSSPI_TPM_CMDRDYSP_MASK) >> HSSPI_TPM_CMDRDYSP_SHIFT) != 0U;
}

void
hsspi_set_tpm_intfcap_CommandReadyInt(HSSPI_t *p_base, bool b_support)
{
    uint32_t tmp = p_base->INTFCAP;
    tmp &= ~(HSSPI_TPM_CMDRDYSP_MASK);
    tmp |= HSSPI_TPM_CMDRDYSP(b_support ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTFCAP = tmp;
}

bool
hsspi_get_tpm_intfcap_BurstCountStatic(HSSPI_t *p_base)
{
    return ((p_base->INTFCAP & HSSPI_TPM_BSTCNTSP_MASK) >> HSSPI_TPM_BSTCNTSP_SHIFT) != 0U;
}

void
hsspi_set_tpm_intfcap_BurstCountStatic(HSSPI_t *p_base, bool b_support)
{
    uint32_t tmp = p_base->INTFCAP;
    tmp &= ~(HSSPI_TPM_BSTCNTSP_MASK);
    tmp |= HSSPI_TPM_BSTCNTSP(b_support ? (uint32_t)1u : (uint32_t)0u);
    p_base->INTFCAP = tmp;
}

tpm_intfcap_transfsize_t
hsspi_get_tpm_intfcap_DataTransferSize(HSSPI_t *p_base)
{
    uint32_t tmp = p_base->INTFCAP;
    tmp          = (tmp & HSSPI_TPM_TRFSZSP_MASK) >> HSSPI_TPM_TRFSZSP_SHIFT;
    return (tpm_intfcap_transfsize_t)(tmp);
}

void
hsspi_set_tpm_intfcap_DataTransferSize(HSSPI_t *p_base, tpm_intfcap_transfsize_t size)
{
    uint32_t tmp = p_base->INTFCAP;
    tmp &= ~(HSSPI_TPM_TRFSZSP_MASK);
    tmp |= HSSPI_TPM_TRFSZSP(size);
    p_base->INTFCAP = tmp;
}

tpm_intfcap_interface_t
hsspi_get_tpm_intfcap_InterfaceVersion(HSSPI_t *p_base)
{
    uint32_t tmp = p_base->INTFCAP;
    tmp          = (tmp & HSSPI_TPM_IFVERSP_MASK) >> HSSPI_TPM_IFVERSP_SHIFT;
    return (tpm_intfcap_interface_t)(tmp);
}

void
hsspi_set_tpm_intfcap_InterfaceVersion(HSSPI_t *p_base, tpm_intfcap_interface_t itf)
{
    uint32_t tmp = p_base->INTFCAP;
    tmp &= ~(HSSPI_TPM_IFVERSP_MASK);
    tmp |= HSSPI_TPM_IFVERSP(itf);
    p_base->INTFCAP = tmp;
}

uint32_t
hsspi_get_tpm_sts_reg(HSSPI_t *p_base)
{
    return (uint32_t)(p_base->STS);
}

void
hsspi_set_tpm_sts_reg(HSSPI_t *p_base, uint32_t reg_value)
{
    p_base->STS = reg_value;
}

bool
hsspi_get_tpm_sts_responseretry(HSSPI_t *p_base)
{
    return ((p_base->STS & HSSPI_TPM_RESEND_MASK) >> HSSPI_TPM_RESEND_SHIFT) != 0U;
}

void
hsspi_set_tpm_sts_responseretry(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->STS;
    tmp &= ~(HSSPI_TPM_RESEND_MASK);
    tmp |= HSSPI_TPM_RESEND(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->STS = tmp;
}

bool
hsspi_get_tpm_sts_selftestdone(HSSPI_t *p_base)
{
    return ((p_base->STS & HSSPI_TPM_SELFTEST_MASK) >> HSSPI_TPM_SELFTEST_SHIFT) != 0U;
}

void
hsspi_set_tpm_sts_selftestdone(HSSPI_t *p_base, bool b_done)
{
    uint32_t tmp = p_base->STS;
    tmp &= ~(HSSPI_TPM_SELFTEST_MASK);
    tmp |= HSSPI_TPM_SELFTEST(b_done ? (uint32_t)1u : (uint32_t)0u);
    p_base->STS = tmp;
}

bool
hsspi_get_tpm_sts_expect(HSSPI_t *p_base)
{
    return ((p_base->STS & HSSPI_TPM_EXPECT_MASK) >> HSSPI_TPM_EXPECT_SHIFT) != 0U;
}

void
hsspi_set_tpm_sts_expect(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->STS;
    tmp &= ~(HSSPI_TPM_EXPECT_MASK);
    tmp |= HSSPI_TPM_EXPECT(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->STS = tmp;
}

bool
hsspi_get_tpm_sts_dataavail(HSSPI_t *p_base)
{
    return ((p_base->STS & HSSPI_TPM_AVA_MASK) >> HSSPI_TPM_AVA_SHIFT) != 0U;
}

void
hsspi_set_tpm_sts_dataavail(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->STS;
    tmp &= ~(HSSPI_TPM_AVA_MASK);
    tmp |= HSSPI_TPM_AVA(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->STS = tmp;
}

bool
hsspi_get_tpm_sts_tpmgo(HSSPI_t *p_base)
{
    return ((p_base->STS & HSSPI_TPM_TPMGO_MASK) >> HSSPI_TPM_TPMGO_SHIFT) != 0U;
}

void
hsspi_set_tpm_sts_tpmgo(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->STS;
    tmp &= ~(HSSPI_TPM_TPMGO_MASK);
    tmp |= HSSPI_TPM_TPMGO(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->STS = tmp;
}

bool
hsspi_get_tpm_sts_commandready(HSSPI_t *p_base)
{
    return ((p_base->STS & HSSPI_TPM_CMDRDY_MASK) >> HSSPI_TPM_CMDRDY_SHIFT) != 0U;
}

void
hsspi_set_tpm_sts_commandready(HSSPI_t *p_base, bool b_ready)
{
    uint32_t tmp = p_base->STS;
    tmp &= ~(HSSPI_TPM_CMDRDY_MASK);
    tmp |= HSSPI_TPM_CMDRDY(b_ready ? (uint32_t)1u : (uint32_t)0u);
    p_base->STS = tmp;
}

bool
hsspi_get_tpm_sts_stsvalid(HSSPI_t *p_base)
{
    return ((p_base->STS & HSSPI_TPM_STSVLD_MASK) >> HSSPI_TPM_STSVLD_SHIFT) != 0U;
}

void
hsspi_set_tpm_sts_stsvalid(HSSPI_t *p_base, bool b_valid)
{
    uint32_t tmp = p_base->STS;
    tmp &= ~(HSSPI_TPM_STSVLD_MASK);
    tmp |= HSSPI_TPM_STSVLD(b_valid ? (uint32_t)1u : (uint32_t)0u);
    p_base->STS = tmp;
}

uint16_t
hsspi_get_tpm_sts_burstcount(HSSPI_t *p_base)
{
    return (uint16_t)((((p_base)->STS) & HSSPI_TPM_BSTCNT_MASK) >> HSSPI_TPM_BSTCNT_SHIFT);
}

void
hsspi_set_tpm_sts_burstcount(HSSPI_t *p_base, uint16_t count)
{
    uint32_t tmp = p_base->STS;
    tmp &= ~(HSSPI_TPM_BSTCNT_MASK);
    tmp |= HSSPI_TPM_BSTCNT(count);
    p_base->STS = tmp;
}

bool
hsspi_get_tpm_sts_commandcancel(HSSPI_t *p_base)
{
    return ((p_base->STS & HSSPI_TPM_CMDCAN_MASK) >> HSSPI_TPM_CMDCAN_SHIFT) != 0U;
}

void
hsspi_set_tpm_sts_commandcancel(HSSPI_t *p_base, bool b_cancel)
{
    uint32_t tmp = p_base->STS;
    tmp &= ~(HSSPI_TPM_CMDCAN_MASK);
    tmp |= HSSPI_TPM_CMDCAN(b_cancel ? (uint32_t)1u : (uint32_t)0u);
    p_base->STS = tmp;
}

bool
hsspi_get_tpm_sts_resetestablishment(HSSPI_t *p_base)
{
    return ((p_base->STS & HSSPI_TPM_RSTESTB_MASK) >> HSSPI_TPM_RSTESTB_SHIFT) != 0U;
}

void
hsspi_set_tpm_sts_resetestablishment(HSSPI_t *p_base, bool b_reset)
{
    uint32_t tmp = p_base->STS;
    tmp &= ~(HSSPI_TPM_RSTESTB_MASK);
    tmp |= HSSPI_TPM_RSTESTB(b_reset ? (uint32_t)1u : (uint32_t)0u);
    p_base->STS = tmp;
}

tpm_sts_tpmfamily_t
hsspi_get_tpm_sts_tpmfamily(HSSPI_t *p_base)
{
    uint32_t tmp = p_base->STS;
    tmp          = (tmp & HSSPI_TPM_FAMILY_MASK) >> HSSPI_TPM_FAMILY_SHIFT;
    return (tpm_sts_tpmfamily_t)(tmp);
}

void
hsspi_set_tpm_sts_tpmfamily(HSSPI_t *p_base, tpm_sts_tpmfamily_t select)
{
    uint32_t tmp = p_base->STS;
    tmp &= ~(HSSPI_TPM_FAMILY_MASK);
    tmp |= HSSPI_TPM_FAMILY(select);
    p_base->STS = tmp;
}

uint32_t
hsspi_get_tpm_did_vid_reg(HSSPI_t *p_base)
{
    return (uint32_t)(p_base->DIDVID);
}

void
hsspi_set_tpm_did_vid_reg(HSSPI_t *p_base, uint32_t reg_value)
{
    p_base->DIDVID = reg_value;
}

uint16_t
hsspi_get_tpm_vid(HSSPI_t *p_base)
{
    return (uint16_t)((((p_base)->DIDVID) & HSSPI_TPM_VID_MASK) >> HSSPI_TPM_VID_SHIFT);
}

void
hsspi_set_tpm_vid(HSSPI_t *p_base, uint16_t vid)
{
    uint32_t tmp = p_base->DIDVID;
    tmp &= ~(HSSPI_TPM_VID_MASK);
    tmp |= HSSPI_TPM_VID(vid);
    p_base->DIDVID = tmp;
}

uint16_t
hsspi_get_tpm_did(HSSPI_t *p_base)
{
    return (uint16_t)((((p_base)->DIDVID) & HSSPI_TPM_DID_MASK) >> HSSPI_TPM_DID_SHIFT);
}

void
hsspi_set_tpm_did(HSSPI_t *p_base, uint16_t did)
{
    uint32_t tmp = p_base->DIDVID;
    tmp &= ~(HSSPI_TPM_DID_MASK);
    tmp |= HSSPI_TPM_DID(did);
    p_base->DIDVID = tmp;
}

uint32_t
hsspi_get_tpm_rid_reg(HSSPI_t *p_base)
{
    return (uint32_t)(p_base->RID);
}

void
hsspi_set_tpm_rid_reg(HSSPI_t *p_base, uint32_t reg_value)
{
    p_base->RID = reg_value;
}

uint8_t
hsspi_get_tpm_rid(HSSPI_t *p_base)
{
    return (uint16_t)((((p_base)->RID) & HSSPI_TPM_RID_MASK) >> HSSPI_TPM_RID_SHIFT);
}

void
hsspi_set_tpm_rid(HSSPI_t *p_base, uint8_t rid)
{
    uint32_t tmp = p_base->RID;
    tmp &= ~(HSSPI_TPM_RID_MASK);
    tmp |= HSSPI_TPM_RID(rid);
    p_base->RID = tmp;
}

uint8_t
hsspi_get_tpm_hash_end(HSSPI_t *p_base)
{
    return (uint16_t)((((p_base)->HASHEND) & HSSPI_TPM_HASHEND_MASK) >> HSSPI_TPM_HASHEND_SHIFT);
}

void
hsspi_set_tpm_hash_end(HSSPI_t *p_base, uint8_t parm)
{
    uint32_t tmp = p_base->HASHEND;
    tmp &= ~(HSSPI_TPM_HASHEND_MASK);
    tmp |= HSSPI_TPM_HASHEND(parm);
    p_base->HASHEND = tmp;
}

uint8_t
hsspi_get_tpm_hash_start(HSSPI_t *p_base)
{
    return (uint16_t)((((p_base)->HASHSTART) & HSSPI_TPM_HASHSTRT_MASK) >>
                      HSSPI_TPM_HASHSTRT_SHIFT);
}

void
hsspi_set_tpm_hash_start(HSSPI_t *p_base, uint8_t parm)
{
    uint32_t tmp = p_base->HASHSTART;
    tmp &= ~(HSSPI_TPM_HASHSTRT_MASK);
    tmp |= HSSPI_TPM_HASHSTRT(parm);
    p_base->HASHSTART = tmp;
}

tpm_intfid_type_t
hsspi_get_tpm_interfaceid_type(HSSPI_t *p_base)
{
    uint32_t tmp = p_base->INFID;
    tmp          = (tmp & HSSPI_TPM_IFTYP_MASK) >> HSSPI_TPM_IFTYP_SHIFT;
    return (tpm_intfid_type_t)(tmp);
}

void
hsspi_set_tpm_interfaceid_type(HSSPI_t *p_base, tpm_intfid_type_t select)
{
    uint32_t tmp = p_base->INFID;
    tmp &= ~(HSSPI_TPM_IFTYP_MASK);
    tmp |= HSSPI_TPM_IFTYP(select);
    p_base->INFID = tmp;
}

tpm_intfid_ver_t
hsspi_get_tpm_interfaceid_ver(HSSPI_t *p_base)
{
    uint32_t tmp = p_base->INFID;
    tmp          = (tmp & HSSPI_TPM_IFVER_MASK) >> HSSPI_TPM_IFVER_SHIFT;
    return (tpm_intfid_ver_t)(tmp);
}

void
hsspi_set_tpm_interfaceid_ver(HSSPI_t *p_base, tpm_intfid_ver_t select)
{
    uint32_t tmp = p_base->INFID;
    tmp &= ~(HSSPI_TPM_IFVER_MASK);
    tmp |= HSSPI_TPM_IFVER(select);
    p_base->INFID = tmp;
}

tpm_intfid_caploc_t
hsspi_get_tpm_interfaceid_caplocality(HSSPI_t *p_base)
{
    uint32_t tmp = p_base->INFID;
    tmp          = (tmp & HSSPI_TPM_CAPLOC_MASK) >> HSSPI_TPM_CAPLOC_SHIFT;
    return (tpm_intfid_caploc_t)(tmp);
}

void
hsspi_set_tpm_interfaceid_caplocality(HSSPI_t *p_base, tpm_intfid_caploc_t select)
{
    uint32_t tmp = p_base->INFID;
    tmp &= ~(HSSPI_TPM_CAPLOC_MASK);
    tmp |= HSSPI_TPM_CAPLOC(select);
    p_base->INFID = tmp;
}

bool
hsspi_get_tpm_interfaceid_captis(HSSPI_t *p_base)
{
    return ((p_base->INFID & HSSPI_TPM_CAPTIS_MASK) >> HSSPI_TPM_CAPTIS_SHIFT) != 0U;
}

void
hsspi_set_tpm_interfaceid_captis(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->INFID;
    tmp &= ~(HSSPI_TPM_CAPTIS_MASK);
    tmp |= HSSPI_TPM_CAPTIS(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->INFID = tmp;
}

bool
hsspi_get_tpm_interfaceid_capcrb(HSSPI_t *p_base)
{
    return ((p_base->INFID & HSSPI_TPM_CAPCRB_MASK) >> HSSPI_TPM_CAPCRB_SHIFT) != 0U;
}

void
hsspi_set_tpm_interfaceid_capcrb(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->INFID;
    tmp &= ~(HSSPI_TPM_CAPCRB_MASK);
    tmp |= HSSPI_TPM_CAPCRB(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->INFID = tmp;
}

tpm_intfid_intfsel_t
hsspi_get_tpm_interfaceid_intfsel(HSSPI_t *p_base)
{
    uint32_t tmp = p_base->INFID;
    tmp          = (tmp & HSSPI_TPM_IFSEL_MASK) >> HSSPI_TPM_IFSEL_SHIFT;
    return (tpm_intfid_intfsel_t)(tmp);
}

void
hsspi_set_tpm_interfaceid_intfsel(HSSPI_t *p_base, tpm_intfid_intfsel_t select)
{
    uint32_t tmp = p_base->INFID;
    tmp &= ~(HSSPI_TPM_IFSEL_MASK);
    tmp |= HSSPI_TPM_IFSEL(select);
    p_base->INFID = tmp;
}

bool
hsspi_get_tpm_interfaceid_intfsel_lock(HSSPI_t *p_base)
{
    return ((p_base->INFID & HSSPI_TPM_IFSELLK_MASK) >> HSSPI_TPM_IFSELLK_SHIFT) != 0U;
}

void
hsspi_set_tpm_interfaceid_intfsel_lock(HSSPI_t *p_base, bool b_enable)
{
    uint32_t tmp = p_base->INFID;
    tmp &= ~(HSSPI_TPM_IFSELLK_MASK);
    tmp |= HSSPI_TPM_IFSELLK(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->INFID = tmp;
}

/*******************************************************************************
 * Static functions
 ******************************************************************************/

/*** end of file ***/
