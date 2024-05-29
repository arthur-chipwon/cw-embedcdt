/**
 * @file hsspi_access.h
 * @brief The module provides for requiring HSSPI hardwre registers access.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef HSSPI_ACCESS_H
#define HSSPI_ACCESS_H

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

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
typedef enum
{
    HSSPI_RX_CMD_FLAG         = HSSPI_SR_RWCC_SHIFT,
    HSSPI_RX_TPM_CMD_FLAG     = HSSPI_SR_RWCC_SHIFT,
    HSSPI_RX_TPM_WAIT_ST_FLAG = HSSPI_SR_RRCW_SHIFT,
    HSSPI_TX_COMPLETION_FLAG  = HSSPI_SR_RRCC_SHIFT,
    HSSPI_ALL_STATUS          = (HSSPI_SR_RWCC_MASK | HSSPI_SR_RRCW_MASK |
                        HSSPI_SR_RRCC_MASK), /**< All W1C (writer 1 clear) bit mask */
} hsspi_status_flag_t;

typedef enum
{
    HSSPI_FUN_RPEN_BM        = HSSPI_FUN_RPEN_MASK,
    HSSPI_FUN_TPMEN_BM       = HSSPI_FUN_TPMEN_MASK,
    HSSPI_FUN_TPMWAIT_BM     = HSSPI_FUN_TPMWAIT_MASK,
    HSSPI_FUN_TPMFIFOWAIT_BM = HSSPI_FUN_TPMFIFOWAIT_MASK,
    HSSPI_FUN_ALL_BM = (HSSPI_FUN_RPEN_MASK | HSSPI_FUN_TPMEN_MASK | HSSPI_FUN_TPMWAIT_MASK |
                        HSSPI_FUN_TPMFIFOWAIT_MASK),
} hsspi_func_bm_t;

typedef enum
{
    HSSPI_INT_RX_CMD_BM         = HSSPI_INTMSK_RWCC_MASK,
    HSSPI_INT_RX_TPM_WAIT_ST_BM = HSSPI_INTMSK_RRCW_MASK,
    HSSPI_INT_RX_COMPLETION_BM  = HSSPI_INTMSK_RRCC_MASK,
    HSSPI_INT_ALL_BM = (HSSPI_INTMSK_RWCC_MASK | HSSPI_INTMSK_RRCW_MASK | HSSPI_INTMSK_RRCC_MASK),
} hsspi_int_bm_t;

typedef enum
{
    HIGH_LEVEL   = 0x00u,
    LOW_LEVEL    = 0x01u,
    RISING_EDGE  = 0x02u,
    FALLING_EDGE = 0x03u
} tpm_inten_typpol_t;

typedef enum
{
    SIZE_LEGACY = 0x00u,
    MAX_8BYTE   = 0x01u,
    MAX_32BYTE  = 0x02u,
    MAX_64BYTE  = 0x03u
} tpm_intfcap_transfsize_t;

typedef enum
{
    IF_1_21     = 0x00u,
    IF_1_3      = 0x02u,
    IF_1_3_TPM2 = 0x03u,
} tpm_intfcap_interface_t;

typedef enum
{
    TPM_1_2 = 0x00u,
    TPM_2_0 = 0x01u,
} tpm_sts_tpmfamily_t;

typedef enum
{
    FIFO_PTP_TPM2_0 = 0x00u,
    CRB             = 0x01u,
    FIFO_TIS1_3     = 0xffu,
} tpm_intfid_type_t;

typedef enum
{
    FIFO_TPM2_0 = 0x00u,
    CRB_VER0    = 0x01u,
} tpm_intfid_ver_t;

typedef enum
{
    SUPPORT_LOC0      = 0x00u,
    SUPPORT_LOC0_LOC4 = 0x01u,
} tpm_intfid_caploc_t;

typedef enum
{
    SEL_TIS = 0x00u,
    SEL_CRB = 0x01u,
} tpm_intfid_intfsel_t;

/*******************************************************************************
 * Exported variables
 ******************************************************************************/

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/
/**
 * @brief Enable the HSSPI function.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @param[in] bitmap Bit field of type hsspi_int_bm_t.
 */
static inline void
hsspi_set_function_enable_bit(HSSPI_t *p_base, hsspi_func_bm_t bitmap) // 0x14
{
    p_base->FUN = bitmap;
}

/**
 * @brief Enable the HSSPI interrupts.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @param[in] bitmap Bit field of type hsspi_int_bm_t to the proper gate of interrupt events.
 */
static inline void
hsspi_set_intrpt_enable_bit(HSSPI_t *p_base, hsspi_int_bm_t bitmap)
{
    p_base->INTMSK |= bitmap;
}

/**
 * @brief Disable the HSSPI interrupts.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @param[in] bitmap Bit field of type hsspi_int_bm_t to the proper gate of interrupt events.
 */
static inline void
hsspi_clear_intrpt_enable_bit(HSSPI_t *p_base, hsspi_int_bm_t bitmap)
{
    p_base->INTMSK &= ~bitmap;
}

/**
 * @brief To get the start address of incoming request buffer.
 *
 * @return uint32_t The address of incoming request buffer.
 */
static inline uint32_t
hsspi_get_req_buffer_address(void)
{
    return (HSSPI_BASE + 0x80u);
}

/**
 * @brief To get the size of incoming request ubffer.
 *
 * @return uint32_t The size of incoming request buffer.
 */
static inline uint32_t
hsspi_get_req_buffer_size(void)
{
    return HSSPI_RX_BUF_SIZE;
}

/**
 * @brief To get the start address of outgoing response buffer.
 *
 * @return uint32_t The address of outgoing response buffer.
 */
static inline uint32_t
hsspi_get_resp_buffer_address(void)
{
    return (HSSPI_BASE + 0x100u);
}

/**
 * @brief To get the size of outgoing response ubffer.
 *
 * @return uint32_t The size of outgoing response buffer.
 */
static inline uint32_t
hsspi_get_resp_buffer_size(void)
{
    return HSSPI_TX_BUF_SIZE;
}

/**
 * @brief To get the received RPMC/TPM write command flag.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return State of received RPMC/TPM write command.
 *         - true : Received.
 *         - false: No event occurred.
 */
static inline bool
hsspi_get_rx_wrcmd_flag(HSSPI_t *p_base)
{
    return ((p_base->SR & HSSPI_SR_RWCC_MASK) >> HSSPI_SR_RWCC_SHIFT) != 0U;
}

/**
 * @brief Clear the received RPMC/TPM write command flag
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 */
static inline void
hsspi_clear_rx_wrcmd_flag(HSSPI_t *p_base)
{
    uint32_t tmp = p_base->SR;
    tmp          = HSSPI_SR_RWCC_MASK;
    p_base->SR   = tmp;
}

/**
 * @brief To get the received TPM read command at wait status flag.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return State of received TPM read command at wait status.
 *         - true : Received.
 *         - false: No event occurred.
 */
static inline bool
hsspi_get_rx_rdcmd_wait_flag(HSSPI_t *p_base)
{
    return ((p_base->SR & HSSPI_SR_RRCW_MASK) >> HSSPI_SR_RRCW_SHIFT) != 0U;
}

/*!
 * @brief Clear the received TPM read command at wait status flag
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 */
static inline void
hsspi_clear_rx_rdcmd_wait_flag(HSSPI_t *p_base)
{
    uint32_t tmp = p_base->SR;
    tmp          = HSSPI_SR_RRCW_MASK;
    p_base->SR   = tmp;
}

/**
 * @brief To get the received TPM read command at completion status flag.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return State of received TPM read command at completion status.
 *         - true : Received.
 *         - false: No event occurred.
 */
static inline bool
hsspi_get_rx_rdcmd_ok_flag(HSSPI_t *p_base)
{
    return ((p_base->SR & HSSPI_SR_RRCC_MASK) >> HSSPI_SR_RRCC_SHIFT) != 0U;
}

/*!
 * @brief Clear the received TPM read command at completion status flag.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 */
static inline void
hsspi_clear_rx_rdcmd_ok_flag(HSSPI_t *p_base)
{
    uint32_t tmp = p_base->SR;
    tmp          = HSSPI_SR_RRCC_MASK;
    p_base->SR   = tmp;
}

/**
 * @brief To get the received TPM FIFO write command flag.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return State of received TPM FIFO write command.
 *         - true : Received.
 *         - false: No event occurred.
 */
static inline bool
hsspi_get_rx_fifo_wrcmd_flag(HSSPI_t *p_base)
{
    return ((p_base->SR & HSSPI_SR_RTFWCC_MASK) >> HSSPI_SR_RTFWCC_SHIFT) != 0U;
}

/**
 * @brief Clear the received TPM read command at completion status flag.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 */
static inline void
hsspi_clear_rx_fifo_wrcmd_flag(HSSPI_t *p_base)
{
    uint32_t tmp = p_base->SR;
    tmp          = HSSPI_SR_RTFWCC_MASK;
    p_base->SR   = tmp;
}

/**
 * @brief HSSPI Busy status.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return State of HSSPI Busy status.
 *         - true : Busy.
 *         - false: No event occurred.
 */
static inline bool
hsspi_busy_flag(HSSPI_t *p_base)
{
    return ((p_base->SR & HSSPI_SR_BSY_MASK) >> HSSPI_SR_BSY_SHIFT) != 0U;
}

/**
 * @brief Indicate TPM last FIFO command address is 0x80.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return Command address.
 *         - true : address is 0x80.
 *         - false: address is 0x24.
 */
static inline bool
hsspi_tpm_last_fifo_0x80(HSSPI_t *p_base)
{
    return ((p_base->SR & HSSPI_SR_LASTADDR80_MASK) >> HSSPI_SR_LASTADDR80_SHIFT) != 0U;
}

/**
 * @brief Indicate current received RPMC/TPM R/W packet length.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return received R/W packet length.
 */
static inline uint8_t
hsspi_rx_packetlen(HSSPI_t *p_base)
{
    return (uint8_t)((((p_base)->PL) & HSSPI_PL_RX_MASK) >> HSSPI_PL_RX_SHIFT);
}

/**
 * @brief TPM TX data length.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return TX data length.
 */
static inline uint8_t
hsspi_tpm_tx_len(HSSPI_t *p_base)
{
    return (uint8_t)((((p_base)->RDFIFOCNT) & HSSPI_TPM_TXLEN_MASK) >> HSSPI_TPM_TXLEN_SHIFT);
}

/**
 * @brief TPM clear read FIFO byte count.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @param[in] clear read FIFO byte count
 *            - true : Clear
 *            - false: No action
 */
static inline void
hsspi_tpm_clear_rdfifo_cnt(HSSPI_t *p_base, bool b_clear)
{
    uint32_t tmp = p_base->RDFIFOCNT;
    tmp &= ~(HSSPI_TPM_RDFIFOCLR_MASK);
    tmp |= HSSPI_TPM_RDFIFOCLR(b_clear ? (uint32_t)1u : (uint32_t)0u);
    p_base->RDFIFOCNT = tmp;
}

/**
 * @brief TPM read FIFO byte count.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return Read FIFO byte count.
 */
static inline uint8_t
hsspi_tpm_rdfifo_cnt(HSSPI_t *p_base)
{
    return (uint8_t)((((p_base)->RDFIFOCNT) & HSSPI_TPM_RDFIFOCNT_MASK) >>
                     HSSPI_TPM_RDFIFOCNT_SHIFT);
}

/**
 * @brief TPM last transfer byte length.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return last transfer byte length.
 */
static inline uint8_t
hsspi_tpm_last_len_rec(HSSPI_t *p_base)
{
    return (uint8_t)((((p_base)->LAST) & HSSPI_TPM_LASTLEN_MASK) >> HSSPI_TPM_LASTLEN_SHIFT);
}

/**
 * @brief Indicate last TPM transfer is read command.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return
 *         - true : Read.
 *         - false: Write.
 */
static inline bool
hsspi_tpm_last_cmd_is_read(HSSPI_t *p_base)
{
    return ((p_base->LAST & HSSPI_TPM_LASTREAD_MASK) >> HSSPI_TPM_LASTREAD_SHIFT) != 0U;
}

/**
 * @brief TPM write FIFO byte count.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return write FIFO byte count.
 */
static inline uint8_t
hsspi_tpm_wrfifo_cnt(HSSPI_t *p_base)
{
    return (uint8_t)((((p_base)->WRFIFOCNT) & HSSPI_TPM_WRFIFOCNT_MASK) >>
                     HSSPI_TPM_WRFIFOCNT_SHIFT);
}

/**
 * @brief TPM last transfer address record.
 *
 * @param[in] p_base Module base pointer of type HSSPI_t
 * @return Last transfer address.
 */
static inline uint32_t
hsspi_tpm_last_addr_rec(HSSPI_t *p_base)
{
    return (uint32_t)((((p_base)->ADDRREC) & HSSPI_TPM_LASTADDR_MASK) >> HSSPI_TPM_LASTADDR_SHIFT);
}

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* HSSPI_ACCESS_H */

/*** end of file ***/
