/**
 * @file hsspi_driver.h
 * @brief The module provides for requiring HSSPI hardwre registers access.
 * Export functions for wide scope use.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef HSSPI_DRIVER_H
#define HSSPI_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "device_registers.h"
#include "hsspi_access.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/** @brief Device instance number. */
#define INST_HSSPI0 0u

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
typedef enum
{
    LOC0    = 0x100u,
    LOC1    = 0x200u,
    LOC2    = 0x400u,
    LOC3    = 0x800u,
    LOC4    = 0x1000u,
    LOC_ALL = (LOC0 | LOC1 | LOC2 | LOC3 | LOC4)
} tpm_loc_num_sel_t;
/*******************************************************************************
 * Exported variables
 ******************************************************************************/
/** @brief Table of base pointers for HSSPI instances. */
extern HSSPI_t *const gp_hsspi_base[HSSPI_INSTANCE_COUNT];

/** @brief Table to save HSSPI IRQ enumeration numbers defined in the CMSIS
 * header file. */
extern const IRQn_t g_hsspi_irq_id[HSSPI_INSTANCE_COUNT];

/** @brief Table to save HSSPI clock names as defined in clock manager. */
extern const clock_names_t g_hsspi_clk_names[HSSPI_INSTANCE_COUNT];

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Initializes the HSSPI module to a known state.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t.
 */
void hsspi_init(HSSPI_t *p_base);

/**
 * @brief Reset HSSPI module.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t.
 * @param[in] b_flag
 *            - true : Reset signal assertion.
 *            - false: Reset signal de-assertion.
 */
void hsspi_reset_signal_assert(HSSPI_t *p_base, bool b_flag);

/**
 * @brief Get status of HSSPI.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @return All of HSSPI status.
 */
uint32_t hsspi_get_status_bm(HSSPI_t *p_base);

/**
 * @brief Clear status of HSSPI.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] status_bm - All W1C (writer 1 clear)
 */
void hsspi_clear_status_bm(HSSPI_t *p_base, uint32_t status_bm);

/**
 * @brief Enable RPMC.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] Enable RPMC
 *            - true : Enable
 *            - false: Disable
 */
void hsspi_set_rpmc_enable(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Set RPMC response status.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] status - Set RPMC response status
 */
void hsspi_fillup_response_status(HSSPI_t *p_base, uint8_t status);

/**
 * @brief Set RPMC read/write command value.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] read_opcode - Set RPMC read command value
 * @param[in] write_opcode - Set RPMC write command value
 */
void hsspi_fillup_rw_opcode(HSSPI_t *p_base, uint8_t read_opcode, uint8_t write_opcode);

/**
 * @brief Set OE bit count.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] value - Set OE bit count
 */
void hsspi_set_oe_bitcount(HSSPI_t *p_base, uint8_t value);

/**
 * @brief Driving MISO at SPI clock rising edge.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable -
 *            - true : Enable
 *            - false: Disable
 */
void hsspi_set_clk_skew_cancellation_on_off(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get received packet length.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return Packet length.
 */
uint16_t hsspi_get_rx_len(HSSPI_t *p_base);

/**
 * @brief Set_TX sram update completed.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 */
void hsspi_set_tx_sram_update_completed(HSSPI_t *p_base);

/**
 * @brief Set TPM Locality header.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] head - TPM Locality header
 */
void hsspi_tpm_loc_head(HSSPI_t *p_base, uint8_t head);

/**
 * @brief Select one of TPM Locality number (number0 ~ number4)
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] loc_sel - TPM Locality number
 */
void hsspi_tpm_loc_num_sel(HSSPI_t *p_base, tpm_loc_num_sel_t loc_sel);

/**
 * @brief Select TPM Locality number0
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true : selected
 */
void hsspi_tpm_loc_num0_sel(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Select TPM Locality number1
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true : selected
 */
void hsspi_tpm_loc_num1_sel(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Select TPM Locality number2
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true : selected
 */
void hsspi_tpm_loc_num2_sel(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Select TPM Locality number3
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true : selected
 */
void hsspi_tpm_loc_num3_sel(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Select TPM Locality number4
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true : selected
 */
void hsspi_tpm_loc_num4_sel(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Bypass TPM locality check
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true : bypass locality check
 */
void hsspi_tpm_byp_locchk(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Bypass TPM locality number check
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true : bypass locality number check
 */
void hsspi_tpm_byp_locnum(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get all status of HSSPI.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @return All of HSSPI status.
 */
uint32_t hsspi_tpm_get_locality_all_reg(HSSPI_t *p_base);

/**
 * @brief Get TPM_ACCESS_x register bit0-31.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @return TPM_ACCESS_x register value.
 */
uint32_t hsspi_get_tpm_access_reg(HSSPI_t *p_base);

/**
 * @brief Set TPM_ACCESS_x register bit0-31.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] reg_value - TPM_ACCESS_x register value
 */
void hsspi_set_tpm_access_reg(HSSPI_t *p_base, uint32_t reg_value);

/**
 * @brief Get TPM_ACCESS_x register bit0(tpmEstablishment) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit0 (tpmEstablishment) setting.
 */
bool hsspi_get_tpm_access_establishment(HSSPI_t *p_base);

/**
 * @brief Set TPM_ACCESS_x register bit0 tpmEstablishment.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_access_establishment(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_ACCESS_x register bit1 (requestUse) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit1 (requestUse) setting.
 */
bool hsspi_get_tpm_access_requestUse(HSSPI_t *p_base);

/**
 * @brief Set TPM_ACCESS_x register bit1 (requestUse).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_access_requestUse(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_ACCESS_x register bit2 (pendingRequest) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit2 (pendingRequest) setting.
 */
bool hsspi_get_tpm_access_pendingRequest(HSSPI_t *p_base);

/**
 * @brief Set TPM_ACCESS_x register bit2 (pendingRequest).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_access_pendingRequest(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_ACCESS_x register bit3 (Seize) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit3 (Seize) setting.
 */
bool hsspi_get_tpm_access_seize(HSSPI_t *p_base);

/**
 * @brief Set TPM_ACCESS_x register bit3 (Seize).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_access_seize(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_ACCESS_x register bit4 (beenSeized) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit4 (beenSeized) setting.
 */
bool hsspi_get_tpm_access_beenseize(HSSPI_t *p_base);

/**
 * @brief Set TPM_ACCESS_x register bit4 (beenSeized).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_access_beenseize(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_ACCESS_x register bit5 (activeLocality) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit5 (activeLocality) setting.
 */
bool hsspi_get_tpm_access_activeLocality(HSSPI_t *p_base);

/**
 * @brief Set TPM_ACCESS_x register bit5 (activeLocality).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_access_activeLocality(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_ACCESS_x register bit7 (tpmRegValidSts) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit7 (tpmRegValidSts) setting.
 */
bool hsspi_get_tpm_access_tpmRegValidSts(HSSPI_t *p_base);

/**
 * @brief Set TPM_ACCESS_x register bit7 (tpmRegValidSts).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_access_tpmRegValidSts(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_INT_ENABLE_x register bit0-31.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @return TPM_INT_ENABLE_x register value.
 */
uint32_t hsspi_get_tpm_inten_reg(HSSPI_t *p_base);

/**
 * @brief Set TPM_INT_ENABLE_x register bit0-31.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] reg_value - TPM_INT_ENABLE_x register value
 */
void hsspi_set_tpm_inten_reg(HSSPI_t *p_base, uint32_t reg_value);

/**
 * @brief Get TPM_INT_ENABLE_x register bit0 (dataAvailIntEnable) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit0 (dataAvailIntEnable) setting.
 */
bool hsspi_get_tpm_inten_dataavailint(HSSPI_t *p_base);

/**
 * @brief Set TPM_INT_ENABLE_x register bit0 (dataAvailIntEnable).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_inten_dataavailint(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_INT_ENABLE_x register bit1 (stsValidIntEnable) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit1 (stsValidIntEnable) setting.
 */
bool hsspi_get_tpm_inten_stsvalidint(HSSPI_t *p_base);

/**
 * @brief Set TPM_INT_ENABLE_x register bit1 (stsValidIntEnable).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_inten_stsvalidint(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_INT_ENABLE_x register bit2 (localityChangeIntEnable) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit2 (localityChangeIntEnable) setting.
 */
bool hsspi_get_tpm_inten_localitychangeint(HSSPI_t *p_base);

/**
 * @brief Set TPM_INT_ENABLE_x register bit2 (localityChangeIntEnable).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_inten_localitychangeint(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_INT_ENABLE_x register bit3-bit4 (typePolarity) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit3-bit4 (typePolarity) setting.
 */
tpm_inten_typpol_t hsspi_get_tpm_inten_typepolarity(HSSPI_t *p_base);

/**
 * @brief Set TPM_INT_ENABLE_x register bit2 (localityChangeIntEnable).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] polarity -
 *            - 00 : High level
 *            - 01 : Low level
 *            - 10 : Low level
 *            - 11 : Falling edge
 */
void hsspi_set_tpm_inten_typepolarity(HSSPI_t *p_base, tpm_inten_typpol_t polarity);

/**
 * @brief Get TPM_INT_ENABLE_x register bit7 (commandReadyEnable) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit7 (commandReadyEnable) setting.
 */
bool hsspi_get_tpm_inten_commandready(HSSPI_t *p_base);

/**
 * @brief Set TPM_INT_ENABLE_x register bit7 (commandReadyEnable).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_inten_commandready(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_INT_ENABLE_x register bit31 (globalIntEnable) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit31 (globalIntEnable) setting.
 */
bool hsspi_get_tpm_inten_globalint(HSSPI_t *p_base);

/**
 * @brief Set TPM_INT_ENABLE_x register bit31 (globalIntEnable).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_inten_globalint(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_INT_VECTOR_x register bit0-3 (sirqVec) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit0-3 (sirqVec) setting.
 */
uint8_t hsspi_get_tpm_intvector_sirqvec(HSSPI_t *p_base);

/**
 * @brief Set TPM_INT_VECTOR_x register bit0-3 (sirqVec).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] vector - SIRQ Value.
 */
void hsspi_set_tpm_intvector_sirqvec(HSSPI_t *p_base, uint8_t vector);

/**
 * @brief Get TPM_INT_STATUS_x register bit0-31.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @return TPM_INT_STATUS_x register value.
 */
uint32_t hsspi_get_tpm_intstatus_reg(HSSPI_t *p_base);

/**
 * @brief Set TPM_INT_STATUS_x register bit0-31.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] reg_value - TPM_INT_STATUS_x register value
 */
void hsspi_set_tpm_intstatus_reg(HSSPI_t *p_base, uint32_t reg_value);

/**
 * @brief Get TPM_INT_STATUS_x register bit0(dataAvailIntOccured) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit0(dataAvailIntOccured) setting.
 */
bool hsspi_get_tpm_intstatus_dataAvailInt(HSSPI_t *p_base);

/**
 * @brief Set TPM_INT_STATUS_x register bit0(dataAvailIntOccured).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_clear - true: enable, false: disable.
 */
void hsspi_set_tpm_intstatus_dataAvailInt(HSSPI_t *p_base, bool b_clear);

/**
 * @brief Get TPM_INT_STATUS_x register bit1(stsValidIntOccurred) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit1(stsValidIntOccurred) setting.
 */
bool hsspi_get_tpm_intstatus_stsValidInt(HSSPI_t *p_base);

/**
 * @brief Set TPM_INT_STATUS_x register bit1(stsValidIntOccurred).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_clear - true: enable, false: disable.
 */
void hsspi_set_tpm_intstatus_stsValidInt(HSSPI_t *p_base, bool b_clear);

/**
 * @brief Get TPM_INT_STATUS_x register bit2(localityChangeIntOccured) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit2(localityChangeIntOccured) setting.
 */
bool hsspi_get_tpm_intstatus_localityChangeInt(HSSPI_t *p_base);

/**
 * @brief Set TPM_INT_STATUS_x register bit2(localityChangeIntOccured).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_clear - true: enable, false: disable.
 */
void hsspi_set_tpm_intstatus_localityChangeInt(HSSPI_t *p_base, bool b_clear);

/**
 * @brief Get TPM_INT_STATUS_x register bit7(commandReadyIntOccured) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit7(commandReadyIntOccured) setting.
 */
bool hsspi_get_tpm_intstatus_commandReadyInt(HSSPI_t *p_base);

/**
 * @brief Set TPM_INT_STATUS_x register bit7(commandReadyIntOccured).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_clear - true: enable, false: disable.
 */
void hsspi_set_tpm_intstatus_commandReadyInt(HSSPI_t *p_base, bool b_clear);

/**
 * @brief Get TPM_INTF_CAPABILITY_x register bit0-31.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @return TPM_INTF_CAPABILITY_x register value.
 */
uint32_t hsspi_get_tpm_intfcap_reg(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTF_CAPABILITY_x register bit0-31.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] reg_value - TPM_INTF_CAPABILITY_x register value
 */
void hsspi_set_tpm_intfcap_reg(HSSPI_t *p_base, uint32_t reg_value);

/**
 * @brief Get TPM_INTF_CAPABILITY_x register bit0(dataAvailIntSupport) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit0(dataAvailIntSupport) setting.
 */
bool hsspi_get_tpm_intfcap_dataAvailInt(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTF_CAPABILITY_x register bit0(dataAvailIntSupport).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_support - true: enable, false: disable.
 */
void hsspi_set_tpm_intfcap_dataAvailInt(HSSPI_t *p_base, bool b_support);

/**
 * @brief Get TPM_INTF_CAPABILITY_x register bit1(stsValidIntSupport) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit1(stsValidIntSupport) setting.
 */
bool hsspi_get_tpm_intfcap_stsValidInt(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTF_CAPABILITY_x register bit1(stsValidIntSupport).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_support - true: enable, false: disable.
 */
void hsspi_set_tpm_intfcap_stsValidInt(HSSPI_t *p_base, bool b_support);

/**
 * @brief Get TPM_INTF_CAPABILITY_x register bit2(LocalityChangeIntSupport) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit2(LocalityChangeIntSupport) setting.
 */
bool hsspi_get_tpm_intfcap_localityChangeInt(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTF_CAPABILITY_x register bit2(LocalityChangeIntSupport).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_support - true: enable, false: disable.
 */
void hsspi_set_tpm_intfcap_localityChangeInt(HSSPI_t *p_base, bool b_support);

/**
 * @brief Get TPM_INTF_CAPABILITY_x register bit3(InterruptLevelHigh) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit3(InterruptLevelHigh) setting.
 */
bool hsspi_get_tpm_intfcap_InterruptLevelHigh(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTF_CAPABILITY_x register bit3(InterruptLevelHigh).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_support - true: enable, false: disable.
 */
void hsspi_set_tpm_intfcap_InterruptLevelHigh(HSSPI_t *p_base, bool b_support);

/**
 * @brief Get TPM_INTF_CAPABILITY_x register bit4(nterruptLevelLow) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit4(nterruptLevelLow) setting.
 */
bool hsspi_get_tpm_intfcap_InterruptLevelLow(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTF_CAPABILITY_x register bit4(nterruptLevelLow).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_support - true: enable, false: disable.
 */
void hsspi_set_tpm_intfcap_InterruptLevelLow(HSSPI_t *p_base, bool b_support);

/**
 * @brief Get TPM_INTF_CAPABILITY_x register bit5(InterruptEdgeRising) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit5(InterruptEdgeRising) setting.
 */
bool hsspi_get_tpm_intfcap_InterruptEdgeRising(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTF_CAPABILITY_x register bit5(InterruptEdgeRising).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_support - true: enable, false: disable.
 */
void hsspi_set_tpm_intfcap_InterruptEdgeRising(HSSPI_t *p_base, bool b_support);

/**
 * @brief Get TPM_INTF_CAPABILITY_x register bit6(InterruptEdgeFalling) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit6(InterruptEdgeFalling) setting.
 */
bool hsspi_get_tpm_intfcap_InterruptEdgeFalling(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTF_CAPABILITY_x register bit6(InterruptEdgeFalling).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_support - true: enable, false: disable.
 */
void hsspi_set_tpm_intfcap_InterruptEdgeFalling(HSSPI_t *p_base, bool b_support);

/**
 * @brief Get TPM_INTF_CAPABILITY_x register bit7(CommandReadyIntSupport) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit7(CommandReadyIntSupport) setting.
 */
bool hsspi_get_tpm_intfcap_CommandReadyInt(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTF_CAPABILITY_x register bit7(CommandReadyIntSupport).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_support - true: enable, false: disable.
 */
void hsspi_set_tpm_intfcap_CommandReadyInt(HSSPI_t *p_base, bool b_support);

/**
 * @brief Get TPM_INTF_CAPABILITY_x register bit8(BurstCountStatic) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit8(BurstCountStatic) setting.
 */
bool hsspi_get_tpm_intfcap_BurstCountStatic(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTF_CAPABILITY_x register bit8(BurstCountStatic).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_support - true: enable, false: disable.
 */
void hsspi_set_tpm_intfcap_BurstCountStatic(HSSPI_t *p_base, bool b_support);

/**
 * @brief Get TPM_INTF_CAPABILITY_x register bit9-10(DataTransferSizeSupport)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit9-10(DataTransferSizeSupport) setting.
 */
tpm_intfcap_transfsize_t hsspi_get_tpm_intfcap_DataTransferSize(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTF_CAPABILITY_x register bit9-10(DataTransferSizeSupport).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] size -
 *            - 00 : legacy transfer
 *            - 01 : 8-byte maximum
 *            - 10 : 32-byte maximum
 *            - 11 : 64-byte maximum
 */
void hsspi_set_tpm_intfcap_DataTransferSize(HSSPI_t *p_base, tpm_intfcap_transfsize_t size);

/**
 * @brief Get TPM_INTF_CAPABILITY_x register bit28-30(InterfaceVersion)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit28-30(InterfaceVersion) setting.
 */
tpm_intfcap_interface_t hsspi_get_tpm_intfcap_InterfaceVersion(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTF_CAPABILITY_x register bit28-30(InterfaceVersion).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] itf -
 *            - 000 : Interface 1.21 or earlier
 *            - 001 : Reserved
 *            - 010 : Interface 1.3
 *            - 011 : Interface 1.3 for TPM 2.0
 *            - 100-111: Reserved
 */
void hsspi_set_tpm_intfcap_InterfaceVersion(HSSPI_t *p_base, tpm_intfcap_interface_t itf);

/**
 * @brief Get TPM_STS_x register bit0-31.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return TPM_STS_x register value
 */
uint32_t hsspi_get_tpm_sts_reg(HSSPI_t *p_base);

/**
 * @brief Set TPM_STS_x register bit0-31.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] reg_value - TPM_STS_x register value
 */
void hsspi_set_tpm_sts_reg(HSSPI_t *p_base, uint32_t reg_value);

/**
 * @brief Get TPM_STS_x register bit1(responseRetry)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit1(responseRetry) setting.
 */
bool hsspi_get_tpm_sts_responseretry(HSSPI_t *p_base);

/**
 * @brief Set TPM_STS_x register bit1(responseRetry).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_sts_responseretry(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_STS_x register bit2(selfTestDone)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit2(selfTestDone) setting.
 */
bool hsspi_get_tpm_sts_selftestdone(HSSPI_t *p_base);

/**
 * @brief Set TPM_STS_x register bit2(selfTestDone).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_done - true: enable, false: disable.
 */
void hsspi_set_tpm_sts_selftestdone(HSSPI_t *p_base, bool b_done);

/**
 * @brief Get TPM_STS_x register bit3(Expect)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit3(Expect) setting.
 */
bool hsspi_get_tpm_sts_expect(HSSPI_t *p_base);

/**
 * @brief Set TPM_STS_x register bit3(Expect).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_sts_expect(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_STS_x register bit4(dataAvail)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit4(dataAvail) setting.
 */
bool hsspi_get_tpm_sts_dataavail(HSSPI_t *p_base);

/**
 * @brief Set TPM_STS_x register bit4(dataAvail).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_sts_dataavail(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_STS_x register bit5(tpmGo)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit5(tpmGo) setting.
 */
bool hsspi_get_tpm_sts_tpmgo(HSSPI_t *p_base);

/**
 * @brief Set TPM_STS_x register bit5(tpmGo)setting.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_sts_tpmgo(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_STS_x register bit6(commandReady)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit6(commandReady) setting.
 */
bool hsspi_get_tpm_sts_commandready(HSSPI_t *p_base);

/**
 * @brief Set TPM_STS_x register bit6(commandReady)setting.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_ready - true: enable, false: disable.
 */
void hsspi_set_tpm_sts_commandready(HSSPI_t *p_base, bool b_ready);

/**
 * @brief Get TPM_STS_x register bit7(stsValid)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit7(stsValid) setting.
 */
bool hsspi_get_tpm_sts_stsvalid(HSSPI_t *p_base);

/**
 * @brief Set TPM_STS_x register bit7(stsValid)setting.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_valid - true: enable, false: disable.
 */
void hsspi_set_tpm_sts_stsvalid(HSSPI_t *p_base, bool b_valid);

/**
 * @brief Get TPM_STS_x register bit8-23(burstCount)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit8-23(burstCount) setting.
 */
uint16_t hsspi_get_tpm_sts_burstcount(HSSPI_t *p_base);

/**
 * @brief Set TPM_STS_x register bit8-23(burstCount)setting.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] count - The number of bytes.
 */
void hsspi_set_tpm_sts_burstcount(HSSPI_t *p_base, uint16_t count);

/**
 * @brief Get TPM_STS_x register bit24(commandCancel)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit24(commandCancel) setting.
 */
bool hsspi_get_tpm_sts_commandcancel(HSSPI_t *p_base);

/**
 * @brief Set TPM_STS_x register bit24(commandCancel)setting.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_cancel - true: enable, false: disable.
 */
void hsspi_set_tpm_sts_commandcancel(HSSPI_t *p_base, bool b_cancel);

/**
 * @brief Get TPM_STS_x register bit25(resetEstablishmentBit)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit25(resetEstablishmentBit) setting.
 */
bool hsspi_get_tpm_sts_resetestablishment(HSSPI_t *p_base);

/**
 * @brief Set TPM_STS_x register bit25(resetEstablishmentBit)setting.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_reset - true: enable, false: disable.
 */
void hsspi_set_tpm_sts_resetestablishment(HSSPI_t *p_base, bool b_reset);

/**
 * @brief Get TPM_STS_x register bit26-27(tpmFamily)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit26-27(tpmFamily) setting.
 */
tpm_sts_tpmfamily_t hsspi_get_tpm_sts_tpmfamily(HSSPI_t *p_base);

/**
 * @brief Set TPM_STS_x register bit26-27(tpmFamily).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] select -
 *            - 00 : TPM 1.2 Family
 *            - 01 : TPM 2.0 Family
 *            - 10 : Reserved
 *            - 11 : Reserved
 */
void hsspi_set_tpm_sts_tpmfamily(HSSPI_t *p_base, tpm_sts_tpmfamily_t select);

/**
 * @brief Get TPM_DID_VID_x register bit0-31 setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit0-31 register value.
 */
uint32_t hsspi_get_tpm_did_vid_reg(HSSPI_t *p_base);

/**
 * @brief Set TPM_DID_VID_x register bit0-31.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] reg_value - TPM_DID_VID_x register value
 */
void hsspi_set_tpm_did_vid_reg(HSSPI_t *p_base, uint32_t reg_value);

/**
 * @brief Get TPM_DID_VID_x register bit0-15(VID)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit0-15(VID) setting.
 */
uint16_t hsspi_get_tpm_vid(HSSPI_t *p_base);

/**
 * @brief Set TPM_STS_x register bit26-27(tpmFamily).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] vid - Vendor ID
 */
void hsspi_set_tpm_vid(HSSPI_t *p_base, uint16_t vid);

/**
 * @brief Get TPM_DID_VID_x register bit16-31(DID)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit16-31(DID) setting.
 */
uint16_t hsspi_get_tpm_did(HSSPI_t *p_base);

/**
 * @brief Set TPM_STS_x register bit16-31(DID).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] did - Device ID
 */
void hsspi_set_tpm_did(HSSPI_t *p_base, uint16_t did);

/**
 * @brief Get TPM_RID_x register bit0-31 setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit0-31 register value.
 */
uint32_t hsspi_get_tpm_rid_reg(HSSPI_t *p_base);

/**
 * @brief Set TPM_RID_x register bit0-31.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] reg_value - TPM_RID_x register value
 */
void hsspi_set_tpm_rid_reg(HSSPI_t *p_base, uint32_t reg_value);

/**
 * @brief Get TPM_RID_x register bit0-7(RID)setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit0-7(RID) setting.
 */
uint8_t hsspi_get_tpm_rid(HSSPI_t *p_base);

/**
 * @brief Set TPM_RID_x register bit0-7(RID).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] rid - Revision ID
 */
void hsspi_set_tpm_rid(HSSPI_t *p_base, uint8_t rid);

/**
 * @brief Get TPM_HASH_END register setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return HASH END.
 */
uint8_t hsspi_get_tpm_hash_end(HSSPI_t *p_base);

/**
 * @brief Set TPM_HASH_END register.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] parm - HASH END
 */
void hsspi_set_tpm_hash_end(HSSPI_t *p_base, uint8_t parm);

/**
 * @brief Get TPM_HASH_START register setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return HASH START.
 */
uint8_t hsspi_get_tpm_hash_start(HSSPI_t *p_base);

/**
 * @brief Set TPM_HASH_START register.
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] parm - HASH START
 */
void hsspi_set_tpm_hash_start(HSSPI_t *p_base, uint8_t parm);

/**
 * @brief Get TPM_INTERFACE_ID_x register bit0-3(InterfaceType) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return Interface type.
 */
tpm_intfid_type_t hsspi_get_tpm_interfaceid_type(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTERFACE_ID_x register bit0-3(InterfaceType).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] select -
 *            - 0000 : FIFO interface(PTP for TPM 2.0)
 *            - 0001 : CRB interface
 *            - 1111 : FIFO interface(TIS1.3)
 */
void hsspi_set_tpm_interfaceid_type(HSSPI_t *p_base, tpm_intfid_type_t select);

/**
 * @brief Get TPM_INTERFACE_ID_x register bit4-7(InterfaceVersion) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return Interface Version.
 */
tpm_intfid_ver_t hsspi_get_tpm_interfaceid_ver(HSSPI_t *p_base); // TPM_INTERFACE_ID 0x228

/**
 * @brief Set TPM_INTERFACE_ID_x register bit4-7(InterfaceVersion).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] select -
 *            - 0000 : FIFO interface(PTP for TPM 2.0)
 *            - 0001 : CRB interface
 */
void hsspi_set_tpm_interfaceid_ver(HSSPI_t *p_base, tpm_intfid_ver_t select);

/**
 * @brief Get TPM_INTERFACE_ID_x register bit8(CapLocality) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return Locality support.
 */
tpm_intfid_caploc_t hsspi_get_tpm_interfaceid_caplocality(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTERFACE_ID_x register bit8(CapLocality).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] select -
 *            - 0 : Locality 0 only
 *            - 1 : Locality 0~4
 */
void hsspi_set_tpm_interfaceid_caplocality(HSSPI_t *p_base, tpm_intfid_caploc_t select);

/**
 * @brief Get TPM_INTERFACE_ID_x register bit13(CapTIS) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return TIS interface support.
 */
bool hsspi_get_tpm_interfaceid_captis(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTERFACE_ID_x register bit13(CapTIS).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_interfaceid_captis(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_INTERFACE_ID_x register bit14(CapCRB) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return CRB interface support.
 */
bool hsspi_get_tpm_interfaceid_capcrb(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTERFACE_ID_x register bit14(CapCRB).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_interfaceid_capcrb(HSSPI_t *p_base, bool b_enable);

/**
 * @brief Get TPM_INTERFACE_ID_x register bit17-18(InterfaceSelector) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit17-18(InterfaceSelector) setting.
 */
tpm_intfid_intfsel_t hsspi_get_tpm_interfaceid_intfsel(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTERFACE_ID_x register bit17-18(InterfaceSelector).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] select -
 *            - 0 : changes interface to TIS
 *            - 1 : changes interface to CRB
 */
void hsspi_set_tpm_interfaceid_intfsel(HSSPI_t *p_base, tpm_intfid_intfsel_t select);

/**
 * @brief Get TPM_INTERFACE_ID_x register bit19(IntfSelLock) setting.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return bit19(IntfSelLock) setting.
 */
bool hsspi_get_tpm_interfaceid_intfsel_lock(HSSPI_t *p_base);

/**
 * @brief Set TPM_INTERFACE_ID_x register bit19(IntfSelLock).
 *
 * @param[in] p_base - Module base pointer of type HSSPI_t
 * @param[in] b_enable - true: enable, false: disable.
 */
void hsspi_set_tpm_interfaceid_intfsel_lock(HSSPI_t *p_base, bool select);

#ifdef __cplusplus
}
#endif

#endif /* HSSPI_DRIVER_H */

/*** end of file ***/
