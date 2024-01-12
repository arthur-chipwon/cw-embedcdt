/**
 * @file lpuart_access.h
 * @brief Static inline function for the lpuart driver.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef LPUART_ACCESS_H
#define LPUART_ACCESS_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include "device_registers.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/**
 * @brief LPUART number of stop bits
 *
 * Implements : lpuart_stop_bit_count_t_Class
 */
typedef enum
{
    LPUART_ONE_STOP_BIT = 0x0U, /**< one stop bit */
    LPUART_TWO_STOP_BIT = 0x1U  /**< two stop bits */
} lpuart_stop_bit_count_t;

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/
/**
 * @brief Enable/Disable the LPUART transmitter.
 *
 * This function enables or disables the LPUART transmitter, based on the parameter received.
 *
 * @param[in] p_base LPUART base pointer.
 * @param[in] b_enable Enable(true) or disable(false) transmitter.
 */
static inline void
lpuart_set_transmitter_cmd(LPUART_t *p_base, bool b_enable)
{
    p_base->CTRL =
        (p_base->CTRL & ~LPUART_CTRL_TE_MASK) | ((b_enable ? 1UL : 0UL) << LPUART_CTRL_TE_SHIFT);
    /* Wait for the register write operation to complete */
    while ((bool)((p_base->CTRL & LPUART_CTRL_TE_MASK) != 0U) != b_enable)
    {
    }
}

/**
 * @brief Enable/Disable the LPUART receiver.
 *
 * This function enables or disables the LPUART receiver, based on the parameter received.
 *
 * @param[in] p_base LPUART base pointer
 * @param[in] b_enable Enable(true) or disable(false) receiver.
 */
static inline void
lpuart_set_receiver_cmd(LPUART_t *p_base, bool b_enable)
{
    p_base->CTRL =
        (p_base->CTRL & ~LPUART_CTRL_RE_MASK) | ((b_enable ? 1UL : 0UL) << LPUART_CTRL_RE_SHIFT);
    /* Wait for the register write operation to complete */
    while ((bool)((p_base->CTRL & LPUART_CTRL_RE_MASK) != 0U) != b_enable)
    {
    }
}

/**
 * @brief Sets the LPUART baud rate modulo divisor.
 *
 * This function sets the LPUART baud rate modulo divisor.
 *
 * @param[in] p_base LPUART base pointer.
 * @param[in] baud_rate_divisor The baud rate modulo division "SBR"
 */
static inline void
lpuart_set_baud_rate_divisor(LPUART_t *p_base, uint32_t baud_rate_divisor)
{
    DEV_ASSERT((baud_rate_divisor <= 0x1FFFU) && (baud_rate_divisor >= 1U));
    uint32_t reg_val;

    reg_val = p_base->BAUD;
    reg_val &= ~(LPUART_BAUD_SBR_MASK);
    reg_val |= LPUART_BAUD_SBR(baud_rate_divisor);
    p_base->BAUD = reg_val;
}

/**
 * @brief Gets the LPUART baud rate modulo divisor.
 *
 * This function gets the LPUART baud rate modulo divisor.
 *
 * @param[in] p_base LPUART base pointer.
 * @return The baud rate modulo division "SBR"
 */
static inline uint16_t
lpuart_get_baud_rate_divisor(const LPUART_t *p_base)
{
    return ((uint16_t)((p_base->BAUD & LPUART_BAUD_SBR_MASK) >> LPUART_BAUD_SBR_SHIFT));
}

/**
 * @brief Sets the LPUART baud rate oversampling ratio
 *
 * This function sets the LPUART baud rate oversampling ratio.
 * (Note: Feature available on select LPUART instances used together with baud rate programming)
 * The oversampling ratio should be set between 4x (00011) and 32x (11111). Writing an invalid
 * oversampling ratio results in an error and is set to a default 16x (01111) oversampling ratio.
 * Disable the transmitter/receiver before calling this function.
 *
 * @param[in] p_base LPUART base pointer.
 * @param[in] over_sampling_ratio The oversampling ratio "OSR"
 */
static inline void
lpuart_set_oversampling_ratio(LPUART_t *p_base, uint32_t over_sampling_ratio)
{
    DEV_ASSERT(over_sampling_ratio <= 0x1FU);
    uint32_t reg_val;

    reg_val = p_base->BAUD;
    reg_val &= ~(LPUART_BAUD_OSR_MASK);
    reg_val |= LPUART_BAUD_OSR(over_sampling_ratio);
    p_base->BAUD = reg_val;
}

/**
 * @brief Gets the LPUART baud rate oversampling ratio
 *
 * This function gets the LPUART baud rate oversampling ratio.
 * (Note: Feature available on select LPUART instances used together with baud rate programming)
 *
 * @param[in] p_base LPUART base pointer.
 * @return The oversampling ratio "OSR"
 */
static inline uint8_t
lpuart_get_oversampling_ratio(const LPUART_t *p_base)
{
    return ((uint8_t)((p_base->BAUD & LPUART_BAUD_OSR_MASK) >> LPUART_BAUD_OSR_SHIFT));
}

/**
 * @brief Configures the LPUART baud rate both edge sampling
 *
 * This function configures the LPUART baud rate both edge sampling.
 * (Note: Feature available on select LPUART instances used with baud rate programming)
 * When enabled, the received data is sampled on both edges of the baud rate clock.
 * This must be set when the oversampling ratio is between 4x and 7x.
 * This function should only be called when the receiver is disabled.
 *
 * @param[in] p_base LPUART base pointer.
 * @param[in] enable   Enable (1) or Disable (0) Both Edge Sampling
 */
static inline void
lpuart_enable_both_edge_sampling(LPUART_t *p_base)
{
    p_base->BAUD |= LPUART_BAUD_BOTHEDGE_MASK;
}

/**
 * @brief Configures the number of stop bits in the LPUART controller.
 *
 * This function configures the number of stop bits in the LPUART controller.
 * In some LPUART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all LPUARTs to ensure safe operation.
 *
 * @param[in] p_base LPUART base pointer.
 * @param[in] stop_bit_count Number of stop bits (1 or 2 - see lpuart_stop_bit_count_t struct)
 */
static inline void
lpuart_set_stop_bit_count(LPUART_t *p_base, lpuart_stop_bit_count_t stop_bit_count)
{
    p_base->BAUD = (p_base->BAUD & ~LPUART_BAUD_SBNS_MASK) |
                   ((uint32_t)stop_bit_count << LPUART_BAUD_SBNS_SHIFT);
}

/**
 * @brief Configures DMA requests.
 *
 * This function configures DMA requests for LPUART Transmitter.
 *
 * @param[in] p_base LPUART base pointer
 * @param[in] b_enable Transmit DMA request configuration (enable:1 /disable: 0)
 */
static inline void
lpuart_set_tx_dma_cmd(LPUART_t *p_base, bool b_enable)
{
    p_base->BAUD = (p_base->BAUD & ~LPUART_BAUD_TDMAE_MASK) |
                   ((b_enable ? 1UL : 0UL) << LPUART_BAUD_TDMAE_SHIFT);
}

/**
 * @brief Configures DMA requests.
 *
 * This function configures DMA requests for LPUART Receiver.
 *
 * @param[in] p_base LPUART base pointer
 * @param[in] b_enable Receive DMA request configuration (enable: 1/disable: 0)
 */
static inline void
lpuart_set_rx_dma_cmd(LPUART_t *p_base, bool b_enable)
{
    p_base->BAUD = (p_base->BAUD & ~LPUART_BAUD_RDMAE_MASK) |
                   ((b_enable ? 1UL : 0UL) << LPUART_BAUD_RDMAE_SHIFT);
}

/**
 * @brief Sends the LPUART 8-bit character.
 *
 * This functions sends an 8-bit character.
 *
 * @param[in] p_base LPUART Instance
 * @param[in] data data to send (8-bit)
 */
static inline void
lpuart_put_char(LPUART_t *p_base, uint8_t data)
{
    volatile uint8_t *dataRegBytes = (volatile uint8_t *)(&(p_base->DATA));
    dataRegBytes[0]                = data;
}

/**
 * @brief Gets the LPUART 8-bit character.
 *
 * This functions receives an 8-bit character.
 *
 * @param[in] p_base LPUART base pointer
 * @param[in] p_read_data Data read from receive (8-bit)
 */
static inline void
lpuart_get_char(const LPUART_t *p_base, uint8_t *p_read_data)
{
    DEV_ASSERT(p_read_data != NULL);
    *p_read_data = (uint8_t)p_base->DATA;
}

/**
 * @brief  Clears the error flags treated by the driver
 *
 * This function clears the error flags treated by the driver.
 * *
 * @param[in] p_base LPUART base pointer
 */
static inline void
lpuart_clear_error_flags(LPUART_t *p_base)
{
    uint32_t mask =
        LPUART_STAT_OR_MASK | LPUART_STAT_NF_MASK | LPUART_STAT_FE_MASK | LPUART_STAT_PF_MASK;

    p_base->STAT = (p_base->STAT & (~FEATURE_LPUART_STAT_REG_FLAGS_MASK)) | mask;
}

#ifdef __cplusplus
}
#endif

#endif /* LPUART_ACCESS_H */

/*** end of file ***/
