/**
 * @file lpuart_driver.c
 * @brief This file provides access to the Lpuart module.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stddef.h>
#include <stdbool.h>
#include "lpuart_driver.h"

/*******************************************************************************
 * Global variables
 ******************************************************************************/
/** @brief Table of base pointers for LPUART instances. */
LPUART_t *const gp_lpuart_base[LPUART_INSTANCE_COUNT] = LPUART_BASE_PTRS;

/** @brief Table to save LPUART IRQ enumeration numbers defined in the CMSIS
 * header file. */
const IRQn_t g_lpuart_rx_tx_irq_id[LPUART_INSTANCE_COUNT] = LPUART_RX_TX_IRQS;

/** @brief Table to save LPUART clock names as defined in clock manager. */
const clock_names_t g_lpuart_clk_names[LPUART_INSTANCE_COUNT] = LPUART_CLOCK_NAMES;

/*******************************************************************************
 * Static variables
 ******************************************************************************/

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/

/*******************************************************************************
 * Global functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : lpuart_init_set
 * Description   : Initializes the LPUART controller to known state, using
 *                 register reset values defined in the reference manual.
 *END**************************************************************************/
void
lpuart_init_set(LPUART_t *p_base)
{
    /* Set the default oversampling ratio (16) and baud-rate divider (4) */
    p_base->BAUD = ((uint32_t)((FEATURE_LPUART_DEFAULT_OSR << LPUART_BAUD_OSR_SHIFT) |
                               (FEATURE_LPUART_DEFAULT_SBR << LPUART_BAUD_SBR_SHIFT)));
    /* Clear the error/interrupt flags */
    p_base->STAT = FEATURE_LPUART_STAT_REG_FLAGS_MASK;
    /* Reset all features/interrupts by default */
    p_base->CTRL = 0x00000000;
    /* Reset match addresses */
    p_base->MATCH = 0x00000000;
    /* Reset FIFO feature */
    p_base->FIFO = FEATURE_LPUART_FIFO_RESET_MASK;
    /* Reset FIFO Watermark values */
    p_base->WATER = 0x00000000;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpuart_set_bit_count_per_char
 * Description   : Configures the number of bits per char in LPUART controller.
 * In some LPUART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all LPUARTs to ensure safe operation.
 *END**************************************************************************/
void
lpuart_set_bit_count_per_char(LPUART_t                   *p_base,
                              lpuart_bit_count_per_char_t bit_count_per_char,
                              bool                        b_parity)
{
    uint32_t tmp_bit_count_per_char = (uint32_t)bit_count_per_char;
    if (b_parity)
    {
        tmp_bit_count_per_char += 1U;
    }

    if (tmp_bit_count_per_char == (uint32_t)LPUART_10_BITS_PER_CHAR)
    {
        p_base->BAUD =
            (p_base->BAUD & ~LPUART_BAUD_M10_MASK) | ((uint32_t)1U << LPUART_BAUD_M10_SHIFT);
    }
    else
    {
        /* config 8-bit (M=0) or 9-bits (M=1) */
        p_base->CTRL =
            (p_base->CTRL & ~LPUART_CTRL_M_MASK) | (tmp_bit_count_per_char << LPUART_CTRL_M_SHIFT);
        /* clear M10 to make sure not 10-bit mode */
        p_base->BAUD &= ~LPUART_BAUD_M10_MASK;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpuart_set_parity_mode
 * Description   : Configures parity mode in the LPUART controller.
 * In some LPUART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all LPUARTs to ensure safe operation.
 *END**************************************************************************/
void
lpuart_set_parity_mode(LPUART_t *p_base, lpuart_parity_mode_t parity_mode_type)
{
    p_base->CTRL = (p_base->CTRL & ~LPUART_CTRL_PE_MASK) |
                   (((uint32_t)parity_mode_type >> 1U) << LPUART_CTRL_PE_SHIFT);
    p_base->CTRL = (p_base->CTRL & ~LPUART_CTRL_PT_MASK) |
                   (((uint32_t)parity_mode_type & 1U) << LPUART_CTRL_PT_SHIFT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpuart_set_int_mode
 * Description   : Configures the LPUART module interrupts to enable/disable
 * various interrupt sources.
 *END**************************************************************************/
void
lpuart_set_int_mode(LPUART_t *p_base, lpuart_interrupt_t int_src, bool b_enable)
{
    uint32_t reg            = (uint32_t)(int_src) >> LPUART_SHIFT;
    uint32_t int_reg_offset = (uint16_t)(int_src);

    switch (reg)
    {
        case LPUART_BAUD_REG_ID:
        {
            p_base->BAUD = (p_base->BAUD & ~(1UL << int_reg_offset)) |
                           ((b_enable ? 1U : 0U) << int_reg_offset);
        }
        break;
        case LPUART_CTRL_REG_ID:
        {
            p_base->CTRL = (p_base->CTRL & ~(1UL << int_reg_offset)) |
                           ((b_enable ? 1U : 0U) << int_reg_offset);
        }
        break;
        case LPUART_FIFO_REG_ID:
        {
            p_base->FIFO =
                (p_base->FIFO & (~FEATURE_LPUART_FIFO_REG_FLAGS_MASK & ~(1UL << int_reg_offset))) |
                ((b_enable ? 1U : 0U) << int_reg_offset);
        }
        break;
        default:
        { /* Invalid parameter: return */
        }
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpuart_get_int_mode
 * Description   : Returns whether LPUART module interrupt is enabled/disabled.
 *END**************************************************************************/
bool
lpuart_get_int_mode(const LPUART_t *p_base, lpuart_interrupt_t int_src)
{
    uint32_t reg       = (uint32_t)(int_src) >> LPUART_SHIFT;
    bool     b_ret_val = false;

    switch (reg)
    {
        case LPUART_BAUD_REG_ID:
        {
            b_ret_val = (((p_base->BAUD >> (uint16_t)(int_src)) & 1U) > 0U);
        }
        break;
        case LPUART_CTRL_REG_ID:
        {
            b_ret_val = (((p_base->CTRL >> (uint16_t)(int_src)) & 1U) > 0U);
        }
        break;
        case LPUART_FIFO_REG_ID:
        {
            b_ret_val = (((p_base->FIFO >> (uint16_t)(int_src)) & 1U) > 0U);
        }
        break;
        default:
        { /* Invalid parameter: return */
        }
        break;
    }
    return b_ret_val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpuart_put_char9
 * Description   : Sends the LPUART 9-bit character.
 *END**************************************************************************/
void
lpuart_put_char9(LPUART_t *p_base, uint16_t data)
{
    uint8_t           ninth_data_bit;
    volatile uint8_t *p_data_reg_bytes = (volatile uint8_t *)(&(p_base->DATA));

    ninth_data_bit = (uint8_t)((data >> 8U) & 0x1U);

    /* write to ninth data bit T8(where T[0:7]=8-bits, T8=9th bit) */
    p_base->CTRL = (p_base->CTRL & ~LPUART_CTRL_R9T8_MASK) |
                   ((uint32_t)(ninth_data_bit) << LPUART_CTRL_R9T8_SHIFT);

    /* write 8-bits to the data register*/
    p_data_reg_bytes[0] = (uint8_t)data;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpuart_put_char10
 * Description   : Sends the LPUART 10-bit character.
 *END**************************************************************************/
void
lpuart_put_char10(LPUART_t *p_base, uint16_t data)
{
    uint8_t           ninth_data_bit, tenth_data_bit;
    uint32_t          reg_val;
    volatile uint8_t *p_data_reg_bytes = (volatile uint8_t *)(&(p_base->DATA));

    ninth_data_bit = (uint8_t)((data >> 8U) & 0x1U);
    tenth_data_bit = (uint8_t)((data >> 9U) & 0x1U);

    /* write to ninth/tenth data bit (T[0:7]=8-bits, T8=9th bit, T9=10th bit) */
    reg_val = p_base->CTRL;
    reg_val =
        (reg_val & ~LPUART_CTRL_R9T8_MASK) | ((uint32_t)ninth_data_bit << LPUART_CTRL_R9T8_SHIFT);
    reg_val =
        (reg_val & ~LPUART_CTRL_R8T9_MASK) | ((uint32_t)tenth_data_bit << LPUART_CTRL_R8T9_SHIFT);
    p_base->CTRL = reg_val;

    /* write to 8-bits to the data register */
    p_data_reg_bytes[0] = (uint8_t)data;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpuart_get_char9
 * Description   : Gets the LPUART 9-bit character.
 *END**************************************************************************/
void
lpuart_get_char9(const LPUART_t *p_base, uint16_t *p_read_data)
{
    DEV_ASSERT(p_read_data != NULL);

    /* get ninth bit from lpuart data register */
    *p_read_data = (uint16_t)(((p_base->CTRL >> LPUART_CTRL_R8T9_SHIFT) & 1U) << 8);

    /* get 8-bit data from the lpuart data register */
    *p_read_data |= (uint8_t)p_base->DATA;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpuart_get_char10
 * Description   : Gets the LPUART 10-bit character
 *END**************************************************************************/
void
lpuart_get_char10(const LPUART_t *p_base, uint16_t *p_read_data)
{
    DEV_ASSERT(p_read_data != NULL);

    /* read tenth data bit */
    *p_read_data = (uint16_t)(((p_base->CTRL >> LPUART_CTRL_R9T8_SHIFT) & 1U) << 9);
    /* read ninth data bit */
    *p_read_data |= (uint16_t)(((p_base->CTRL >> LPUART_CTRL_R8T9_SHIFT) & 1U) << 8);

    /* get 8-bit data */
    *p_read_data |= (uint8_t)p_base->DATA;
}

/**
 * @brief Gets the LPUART status bitmap state.
 *
 * This function returns the state of bitmap of the LPUART status at the moment of intrrupt event.
 *
 * @param[in] p_base Module base pointer of type LPUART_t.
 * @return State of bitmap of the status.
 */
uint32_t
lpuart_get_status_bm(LPUART_t *p_base)
{
    return p_base->STAT;
}

/**
 * @brief Clears the LPUART status bitmap state.
 *
 * @param[in] p_base Module base pointer of type LPUART_t.
 * @param[in] status_bm bitmap for w1c interrupt event.
 */
void
lpuart_clear_status_bm(LPUART_t *p_base, uint32_t status_bm)
{
    p_base->STAT = status_bm;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpuart_get_status_flag
 * Description   : LPUART get status flag by passing flag enum.
 *END**************************************************************************/
bool
lpuart_get_status_flag(const LPUART_t *p_base, lpuart_status_flag_t status_flag)
{
    uint32_t reg     = (uint32_t)(status_flag) >> LPUART_SHIFT;
    bool     ret_val = false;

    switch (reg)
    {
        case LPUART_STAT_REG_ID:
        {
            ret_val = (((p_base->STAT >> (uint16_t)(status_flag)) & 1U) > 0U);
        }
        break;
        case LPUART_DATA_REG_ID:
        {
            ret_val = (((p_base->DATA >> (uint16_t)(status_flag)) & 1U) > 0U);
        }
        break;
        case LPUART_FIFO_REG_ID:
        {
            ret_val = (((p_base->FIFO >> (uint16_t)(status_flag)) & 1U) > 0U);
        }
        break;
        default:
        { /* Invalid parameter: return */
        }
        break;
    }

    return ret_val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpuart_clear_status_flag
 * Description   : LPUART clears an individual status flag
 * (see lpuart_status_flag_t for list of status bits).
 *END**************************************************************************/
status_t
lpuart_clear_status_flag(LPUART_t *p_base, lpuart_status_flag_t status_flag)
{
    status_t return_code = STATUS_SUCCESS;

    switch (status_flag)
    {
        /* These flags are cleared automatically by other lpuart operations
         * and cannot be manually cleared, return error code */
        case LPUART_TX_DATA_REG_EMPTY:
        case LPUART_TX_COMPLETE:
        case LPUART_RX_DATA_REG_FULL:
        {
            return_code = STATUS_ERROR;
        }
        break;
        case LPUART_IDLE_LINE_DETECT:
        {
            p_base->STAT =
                (p_base->STAT & (~FEATURE_LPUART_STAT_REG_FLAGS_MASK)) | LPUART_STAT_IDLE_MASK;
        }
        break;
        case LPUART_RX_OVERRUN:
        {
            p_base->STAT =
                (p_base->STAT & (~FEATURE_LPUART_STAT_REG_FLAGS_MASK)) | LPUART_STAT_OR_MASK;
        }
        break;
        case LPUART_NOISE_DETECT:
        {
            p_base->STAT =
                (p_base->STAT & (~FEATURE_LPUART_STAT_REG_FLAGS_MASK)) | LPUART_STAT_NF_MASK;
        }
        break;
        case LPUART_FRAME_ERR:
        {
            p_base->STAT =
                (p_base->STAT & (~FEATURE_LPUART_STAT_REG_FLAGS_MASK)) | LPUART_STAT_FE_MASK;
        }
        break;
        case LPUART_PARITY_ERR:
        {
            p_base->STAT =
                (p_base->STAT & (~FEATURE_LPUART_STAT_REG_FLAGS_MASK)) | LPUART_STAT_PF_MASK;
        }
        break;
        case LPUART_MATCH_ADDR_ONE:
        {
            p_base->STAT =
                (p_base->STAT & (~FEATURE_LPUART_STAT_REG_FLAGS_MASK)) | LPUART_STAT_MA1F_MASK;
        }
        break;
        case LPUART_MATCH_ADDR_TWO:
        {
            p_base->STAT =
                (p_base->STAT & (~FEATURE_LPUART_STAT_REG_FLAGS_MASK)) | LPUART_STAT_MA2F_MASK;
        }
        break;
        case LPUART_FIFO_TX_OF:
        {
            p_base->FIFO =
                (p_base->FIFO & (~FEATURE_LPUART_FIFO_REG_FLAGS_MASK)) | LPUART_FIFO_TXOF_MASK;
        }
        break;
        case LPUART_FIFO_RX_UF:
        {
            p_base->FIFO =
                (p_base->FIFO & (~FEATURE_LPUART_FIFO_REG_FLAGS_MASK)) | LPUART_FIFO_RXUF_MASK;
        }
        break;
        default:
        {
            return_code = STATUS_ERROR;
        }
        break;
    }

    return (return_code);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpuart_set_error_interrupts
 * Description   : Enable or disable the LPUART error interrupts.
 *END**************************************************************************/
void
lpuart_set_error_interrupts(LPUART_t *p_base, bool b_enable)
{
    /* Configure the error interrupts */
    lpuart_set_int_mode(p_base, LPUART_INT_RX_OVERRUN, b_enable);
    lpuart_set_int_mode(p_base, LPUART_INT_PARITY_ERR_FLAG, b_enable);
    lpuart_set_int_mode(p_base, LPUART_INT_NOISE_ERR_FLAG, b_enable);
    lpuart_set_int_mode(p_base, LPUART_INT_FRAME_ERR_FLAG, b_enable);
}

/*******************************************************************************
 * Static functions
 ******************************************************************************/

/*** end of file ***/
