/**
 * @file lpspi_access.h
 * @brief The module provides for requiring LPSPI hardwre registers access.
 * Export functions for wide scope use.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef LPSPI_ACCESS_H
#define LPSPI_ACCESS_H

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
#include "status.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/*! @brief Prescaler values for LPSPI clock source.
 */
typedef enum
{
    LPSPI_DIV_1   = 0u,
    LPSPI_DIV_2   = 1u,
    LPSPI_DIV_4   = 2u,
    LPSPI_DIV_8   = 3u,
    LPSPI_DIV_16  = 4u,
    LPSPI_DIV_32  = 5u,
    LPSPI_DIV_64  = 6u,
    LPSPI_DIV_128 = 7u,
} lpspi_prescaler_t;

/*! @brief LPSPI status flags.
 */
typedef enum
{
    LPSPI_TX_DATA_FLAG      = LPSPI_SR_TDF_SHIFT, /**< TX data flag */
    LPSPI_RX_DATA_FLAG      = LPSPI_SR_RDF_SHIFT, /**< RX data flag */
    LPSPI_WORD_COMPLETE     = LPSPI_SR_WCF_SHIFT, /**< Word Complete flag */
    LPSPI_FRAME_COMPLETE    = LPSPI_SR_FCF_SHIFT, /**< Frame Complete flag */
    LPSPI_TRANSFER_COMPLETE = LPSPI_SR_TCF_SHIFT, /**< Transfer Complete flag */
    LPSPI_TRANSMIT_ERROR    = LPSPI_SR_TEF_SHIFT, /**< Transmit Error flag (FIFO underrun) */
    LPSPI_RECEIVE_ERROR     = LPSPI_SR_REF_SHIFT, /**< Receive Error flag (FIFO overrun) */
    LPSPI_DATA_MATCH        = LPSPI_SR_DMF_SHIFT, /**< Data Match flag */
    LPSPI_MODULE_BUSY       = LPSPI_SR_MBF_SHIFT, /**< Module Busy flag */
    LPSPI_ALL_STATUS        = 0x00003F00u         /**< Used for clearing all w1c status flags */
} lpspi_status_flag_t;

/*! @brief LPSPI delay type selection
 */
typedef enum
{
    LPSPI_SCK_TO_PCS       = LPSPI_CCR_SCKPCS_SHIFT, /**< SCK to PCS Delay */
    LPSPI_PCS_TO_SCK       = LPSPI_CCR_PCSSCK_SHIFT, /**< PCS to SCK Delay */
    LPSPI_BETWEEN_TRANSFER = LPSPI_CCR_DBT_SHIFT     /**< Delay between transfers */
} lpspi_delay_type_t;

/*******************************************************************************
 * Exported variables
 ******************************************************************************/

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/
/*!
 * @brief Enables the LPSPI module.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 */
static inline void
lpspi_enable(LPSPI_t *p_base)
{
    (p_base->CR) |= (uint32_t)1U << LPSPI_CR_MEN_SHIFT;
}

/*!
 * @brief Returns whether the LPSPI module is in master mode.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @return Returns true if LPSPI in master mode or false if in slave mode.
 */
static inline bool
lpspi_is_master(const LPSPI_t *p_base)
{
    return (bool)((p_base->CFGR1 >> LPSPI_CFGR1_MASTER_SHIFT) & 1U);
}

/*!
 * @brief Gets FIFO sizes of the LPSPI module.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[out] fifo_size The FIFO size passed back to the user
 */
static inline void
lpspi_get_fifo_sizes(const LPSPI_t *p_base, uint8_t *fifo_size)
{
    if (fifo_size != NULL)
    {
        *fifo_size = (uint8_t)(1U << ((p_base->PARAM & LPSPI_PARAM_TXFIFO_MASK) >>
                                      LPSPI_PARAM_TXFIFO_SHIFT));
    }
}

/*!
 * @brief Sets the RX FIFO watermark values.
 *
 * This function allows the user to set the RX FIFO watermarks.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] watermark The RX FIFO watermark value
 */
static inline void
lpspi_set_rx_watermarks(LPSPI_t *p_base, uint32_t watermark)
{
    uint32_t reg_val = p_base->FCR;
    reg_val &= ~(LPSPI_FCR_RXWATER_MASK);
    reg_val |= (watermark << LPSPI_FCR_RXWATER_SHIFT);
    p_base->FCR = reg_val;
}

/*!
 * @brief Sets the TX FIFO watermark values.
 *
 * This function allows the user to set the TX FIFO watermarks.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] watermark The TX FIFO watermark value
 */
static inline void
lpspi_set_tx_watermarks(LPSPI_t *p_base, uint32_t watermark)
{
    uint32_t reg_val = p_base->FCR;
    reg_val &= ~(LPSPI_FCR_TXWATER_MASK);
    reg_val |= (watermark << LPSPI_FCR_TXWATER_SHIFT);
    p_base->FCR = reg_val;
}

/*!
 * @name Status flags and Interrupt configuration
 * @{
 */

/*!
 * @brief Gets the LPSPI status bitmap state.
 *
 * This function returns the state of bitmap of the LPSPI status at the moment of intrrupt event.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @return State of bitmap of the status.
 */
static inline uint32_t
lpspi_get_status_bm(LPSPI_t *p_base)
{
    return p_base->SR;
}

/*!
 * @brief Clears the LPSPI status bitmap state.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] status_bm bitmap for w1c interrupt event.
 */
static inline void
lpspi_clear_status_bm(LPSPI_t *p_base, uint32_t status_bm)
{
    p_base->SR = status_bm;
}

/*!
 * @brief Gets the LPSPI status flag state.
 *
 * This function returns the state of one of the LPSPI status flags as requested
 * by the user.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] status_flag The status flag, of type lpspi_status_flag_t
 * @return State of the status flag: asserted (true) or not-asserted (false)
 */
static inline bool
lpspi_get_status_flag(const LPSPI_t *p_base, lpspi_status_flag_t status_flag)
{
    return (bool)(((p_base->SR) >> (uint8_t)status_flag) & 1U);
}

/*!
 * @brief Configures the LPSPI interrupts.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] interrupt_src The interrupt source, of type lpspi_status_flag_t
 * @param[in] b_enable Enable (true) or disable (false) the interrupt source
 */
static inline void
lpspi_set_int_mode(LPSPI_t *p_base, lpspi_status_flag_t interrupt_src, bool b_enable)
{
    if (b_enable == true)
    {
        p_base->IER |= (uint32_t)1U << (uint8_t)interrupt_src;
    }
    else
    {
        p_base->IER &= ~((uint32_t)1U << (uint8_t)interrupt_src);
    }
}

/*!
 * @brief Returns if the LPSPI interrupt request is enabled or disabled.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] interrupt_src The interrupt source, of type lpspi_status_flag_t
 * @return Returns if the interrupt source is enabled (true) or disabled (false)
 */
static inline bool
lpspi_get_int_mode(const LPSPI_t *p_base, lpspi_status_flag_t interrupt_src)
{
    return (bool)(((p_base->IER) >> (uint8_t)interrupt_src) & 1U);
}

/**@}*/

/*!
 * @name DMA configuration
 * @{
 */

/*!
 * @brief Sets the LPSPI Transmit Data DMA configuration (enable or disable).
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] b_enable Enable (true) or disable (false) the TX DMA request
 */
static inline void
lpspi_set_tx_dma_cmd(LPSPI_t *p_base, bool b_enable)
{
    p_base->DER =
        (p_base->DER & (~LPSPI_DER_TDDE_MASK)) | ((uint32_t)b_enable << LPSPI_DER_TDDE_SHIFT);
}

/*!
 * @brief Sets the LPSPI Receive Data DMA configuration (enable or disable).
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] b_enable Enable (true) or disable (false) the RX DMA request
 */
static inline void
lpspi_set_rx_dma_cmd(LPSPI_t *p_base, bool b_enable)
{
    (p_base->DER) =
        (p_base->DER & (~LPSPI_DER_RDDE_MASK)) | ((uint32_t)b_enable << LPSPI_DER_RDDE_SHIFT);
}

/*!
 * @brief Manually configures a specific LPSPI delay parameter (module must be
 * disabled to change the delay values).
 *
 * This function configures the:
 * SCK to PCS delay, or
 * PCS to SCK delay, or
 * Between transfer delay.
 *
 * These delay names are available in type lpspi_delay_type_t.
 *
 * The user passes which delay they want to configure along with the delay
 * value. This allows the user to directly set the delay values if they have
 * pre-calculated them or if they simply wish to manually increment the value.
 *
 * Note that the LPSPI module must first be disabled before configuring this.
 * Note that the LPSPI module must be configure for master mode before
 * configuring this.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] which_delay The desired delay to configure, must be of type
 * lpspi_delay_type_t
 * @param[in] delay The 8-bit delay value 0x00 to 0xFF (255). The delay is equal to:
 *             -delay + 1 cycles of the LPSPI baud rate clock (SCK to PCS and
 * PCS to SCK) -delay + 2 cycles of the LPSPI baud rate clock (Between transfer
 * delay)
 * @return Either STATUS_SUCCESS, LPSPI_STATUS_OUT_OF_RANGE, or STATUS_ERROR if
 *         LPSPI is not disabled or if is not set for master mode.
 */
static inline status_t
lpspi_set_delay(LPSPI_t *p_base, lpspi_delay_type_t which_delay, uint32_t delay)
{
    uint32_t reg_val = 0;

    reg_val = p_base->CCR & ~(0xFFUL << (uint32_t)which_delay);
    reg_val |= delay << (uint32_t)which_delay;
    p_base->CCR = reg_val;
    return STATUS_SUCCESS;
}
/**@}*/

/*!
 * @brief Writes data into the TX data buffer.
 *
 * This function writes data passed in by the user to the Transmit Data Register
 * (TDR). The user can pass up to 32-bits of data to load into the TDR. If the
 * frame size exceeds 32-bits, the user will have to manage sending the data one
 * 32-bit word at a time. Any writes to the TDR will result in an immediate push
 * to the TX FIFO. This function can be used for either master or slave mode.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] data The data word to be sent
 */
static inline void
lpspi_write_data(LPSPI_t *p_base, uint32_t data)
{
    p_base->TDR = data;
}

/*!
 * @brief Reads data from the data buffer.
 *
 * This function reads the data from the Receive Data Register (RDR).
 * This function can be used for either master or slave mode.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @return The data read from the data buffer
 */
static inline uint32_t
lpspi_read_data(const LPSPI_t *p_base)
{
    return (uint32_t)p_base->RDR;
}

/*!
 * @brief Reads TX COUNT form the FIFO Status Register.
 *
 * This function reads the TX COUNT field  from the FIFO Status Register (FSR).
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @return The data read from the FIFO Status Register
 */
static inline uint32_t
lpspi_read_tx_count(const LPSPI_t *p_base)
{
    return (uint32_t)(((uint32_t)(p_base->FSR & LPSPI_FSR_TXCOUNT_MASK)) >>
                      LPSPI_FSR_TXCOUNT_SHIFT);
}

/*!
 * @brief Reads RX COUNT form the FIFO Status Register.
 *
 * This function reads the RX COUNT field  from the FIFO Status Register (FSR).
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @return The data read from the FIFO Status Register
 */
static inline uint32_t
lpspi_read_rx_count(const LPSPI_t *p_base)
{
    return (uint32_t)((((uint32_t)p_base->FSR & (uint32_t)LPSPI_FSR_RXCOUNT_MASK)) >>
                      (uint32_t)LPSPI_FSR_RXCOUNT_SHIFT);
}

/*!
 * @brief Clear RXMSK bit form TCR Register.
 *
 * This function clears the RXMSK bit from the Transmit Command Register (TCR).
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 */
static inline void
lpspi_clear_rx_mask_bit(LPSPI_t *p_base)
{
    (p_base->TCR) = ((p_base->TCR) & (~LPSPI_TCR_RXMSK_MASK));
}

/*!
 * @brief Set RXMSK bit form TCR Register.
 *
 * This function set the RXMSK bit from the Transmit Command Register (TCR).
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 */
static inline void
lpspi_set_rx_mask_bit(LPSPI_t *p_base)
{
    (p_base->TCR) = ((p_base->TCR) | (LPSPI_TCR_RXMSK_MASK));
}

/*!
 * @brief Clear TXMSK bit form TCR Register.
 *
 * This function clears the TXMSK bit from the Transmit Command Register (TCR).
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 */
static inline void
lpspi_clear_tx_mask_bit(LPSPI_t *p_base)
{
    (p_base->TCR) = ((p_base->TCR) & (~LPSPI_TCR_TXMSK_MASK));
}

/*!
 * @brief Set TXMSK bit form TCR Register.
 *
 * This function set the TXMSK bit from the Transmit Command Register (TCR).
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 */
static inline void
lpspi_set_tx_mask_bit(LPSPI_t *p_base)
{
    (p_base->TCR) = ((p_base->TCR) | (LPSPI_TCR_TXMSK_MASK));
}

/*!
 * @brief Clear CONTC bit form TCR Register.
 *
 * This function clears the CONTC bit from the Transmit Command Register
 * (TCR).
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 */
static inline void
lpspi_clear_cont_cmd_bit(LPSPI_t *p_base)
{
    (p_base->TCR) = ((p_base->TCR) & (~LPSPI_TCR_CONTC_MASK));
}

/*!
 * @brief Set CONTC bit form TCR Register.
 *
 * This function set the CONTC bit from the Transmit Command Register (TCR).
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 */
static inline void
lpspi_set_cont_cmd_bit(LPSPI_t *p_base)
{
    (p_base->TCR) = ((p_base->TCR) | (LPSPI_TCR_CONTC_MASK));
}

/*!
 * @brief Configures the clock prescaler used for all LPSPI master logic.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] prescaler Prescaler value for master logic.
 */
static inline void
lpspi_set_clock_prescaler(LPSPI_t *p_base, lpspi_prescaler_t prescaler)
{
    uint32_t reg_val = p_base->TCR;
    reg_val &= ~(LPSPI_TCR_PRESCALE_MASK);
    reg_val |= ((uint32_t)prescaler << LPSPI_TCR_PRESCALE_SHIFT);
    p_base->TCR = reg_val;
}

/*!
 * @brief Get the clock prescaler used for all LPSPI master logic.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @return Prescaler value for master logic.
 */
static inline lpspi_prescaler_t
lpspi_get_clock_prescaler(const LPSPI_t *p_base)
{
    return (lpspi_prescaler_t)(((uint32_t)((p_base->TCR) & LPSPI_TCR_PRESCALE_MASK)) >>
                               LPSPI_TCR_PRESCALE_SHIFT);
}

/*!
 * @brief Configures if the sample point for master devices is delayed.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] b_sampling_point_delayed Configure if the sampling point is delayed for
 * master devices
 */
static inline void
lpspi_set_sampling_point(LPSPI_t *p_base, bool b_sampling_point_delayed)
{
    uint32_t reg_val = p_base->CFGR1;
    reg_val &= ~(LPSPI_CFGR1_SAMPLE_MASK);
    reg_val |= ((uint32_t)b_sampling_point_delayed << LPSPI_CFGR1_SAMPLE_SHIFT);
    p_base->CFGR1 = reg_val;
}

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* LPSPI_ACCESS_H */

/*** end of file ***/
