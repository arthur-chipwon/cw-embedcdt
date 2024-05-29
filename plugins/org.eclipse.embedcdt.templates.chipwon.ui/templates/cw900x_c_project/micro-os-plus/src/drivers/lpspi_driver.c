/**
 * @file lpspi_driver.c
 * @brief The module provides for requiring LPSPI hardwre registers access.
 * Export functions for wide scope use.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lpspi_driver.h"

/*******************************************************************************
 * Private macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/

/*******************************************************************************
 * Private typedefs
 ******************************************************************************/

/*******************************************************************************
 * Global variables
 ******************************************************************************/
/** @brief Table of base pointers for LPSPI instances. */
LPSPI_t *const gp_lpspi_base[LPSPI_INSTANCE_COUNT] = LPSPI_BASE_PTRS;

/** @brief Table to save LPSPI IRQ enumeration numbers defined in the CMSIS
 * header file. */
const IRQn_t g_lpspi_irq_id[LPSPI_INSTANCE_COUNT] = LPSPI_IRQS;

/** @brief Table to save LPSPI clock names as defined in clock manager. */
const clock_names_t g_lpspi_clk_names[LPSPI_INSTANCE_COUNT] = LPSPI_CLOCK_NAMES;

/*******************************************************************************
 * Static variables
 ******************************************************************************/
/* Defines constant value arrays for the baud rate pre-scalar values.*/
static const uint32_t s_baudrate_prescaler[] = {1, 2, 4, 8, 16, 32, 64, 128};

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : lpspi_abs_dif
 * Description   : This is a helper function which implements absolute
 *difference between two numbers.
 *
 *END**************************************************************************/
static uint32_t lpspi_abs_dif(uint32_t a, uint32_t b);

/*******************************************************************************
 * Global functions
 ******************************************************************************/
void
lpspi_init(LPSPI_t *p_base)
{
    /* Software reset the internal logic */
    p_base->CR = LPSPI_CR_RST_MASK;
    /* Now bring the LPSPI module out of reset and clear CR register */
    p_base->CR = (uint32_t)0x0;
}

status_t
lpspi_disable(LPSPI_t *p_base)
{
    uint32_t reg_val = p_base->SR;
    reg_val          = (reg_val & LPSPI_SR_MBF_MASK) >> LPSPI_SR_MBF_SHIFT;

    if (reg_val == (uint32_t)1)
    {
        return STATUS_BUSY;
    }
    else
    {
        p_base->CR = p_base->CR & (~(LPSPI_CR_MEN_MASK));
        return STATUS_SUCCESS;
    }
}

status_t
lpspi_set_master_slave_mode(LPSPI_t *p_base, lpspi_master_slave_mode_t mode)
{
    p_base->CFGR1 =
        (p_base->CFGR1 & (~LPSPI_CFGR1_MASTER_MASK)) | ((uint32_t)mode << LPSPI_CFGR1_MASTER_SHIFT);
    return STATUS_SUCCESS;
}

void
lpspi_set_flush_fifo_cmd(LPSPI_t *p_base, bool b_flush_tx_fifo, bool b_flush_rx_fifo)
{
    uint32_t reg_val = 0;

    reg_val = ((uint32_t)b_flush_tx_fifo << LPSPI_CR_RTF_SHIFT) |
              ((uint32_t)b_flush_rx_fifo << LPSPI_CR_RRF_SHIFT);

    p_base->CR |= reg_val;
}

status_t
lpspi_clear_status_flag(LPSPI_t *p_base, lpspi_status_flag_t status_flag)
{
    if (status_flag == LPSPI_ALL_STATUS)
    {
        p_base->SR |= (uint32_t)LPSPI_ALL_STATUS;
    }
    else
    {
        p_base->SR |= ((uint32_t)1U << (uint32_t)status_flag);
    }
    return STATUS_SUCCESS;
}

status_t
lpspi_set_pcs_polarity_mode(LPSPI_t                *p_base,
                            lpspi_which_pcs_t       which_pcs,
                            lpspi_signal_polarity_t pcs_polarity)
{
    uint32_t reg_val = 0;

    /* Clear the PCS polarity bit */
    reg_val =
        (p_base->CFGR1) & (~((uint32_t)1U << (LPSPI_CFGR1_PCSPOL_SHIFT + (uint32_t)which_pcs)));

    /* Configure the PCS polarity bit according to the pcs_polarity setting */
    reg_val |= (uint32_t)pcs_polarity << (LPSPI_CFGR1_PCSPOL_SHIFT + (uint32_t)which_pcs);

    p_base->CFGR1 = reg_val;

    return STATUS_SUCCESS;
}

status_t
lpspi_set_pin_config_mode(LPSPI_t                *p_base,
                          lpspi_pin_config_t      pin_cfg,
                          lpspi_data_out_config_t data_out_cfg,
                          bool                    b_pcs_no3_no2_enable)
{
    uint32_t reg_val = 0;

    reg_val = p_base->CFGR1 &
              ~(LPSPI_CFGR1_PINCFG_MASK | LPSPI_CFGR1_OUTCFG_MASK | LPSPI_CFGR1_PCSCFG_MASK);

    reg_val |= ((uint32_t)(pin_cfg) << LPSPI_CFGR1_PINCFG_SHIFT) |
               ((uint32_t)(data_out_cfg) << LPSPI_CFGR1_OUTCFG_SHIFT) |
               ((uint32_t)(!b_pcs_no3_no2_enable) << LPSPI_CFGR1_PCSCFG_SHIFT); /* enable = 0 */

    p_base->CFGR1 = reg_val;

    return STATUS_SUCCESS;
}

uint32_t
lpspi_set_baud_rate(LPSPI_t  *p_base,
                    uint32_t  bits_per_sec,
                    uint32_t  src_clk_freq,
                    uint32_t *p_tcr_prescale_value)
{

    uint32_t bes_freq      = 0xFFFFFFFFU;
    uint32_t best_scaler    = 0U;
    uint32_t best_prescaler = 0U;
    uint32_t freq1         = 0U;
    uint32_t freq2         = 0U;
    uint8_t  scaler        = 0U;
    uint8_t  prescaler     = 0U;
    uint32_t low, high;
    uint32_t tmp_best_freq   = 0U;
    uint32_t tmp_best_scaler = 0U;

    for (prescaler = 0; prescaler < 8U; prescaler++)
    {
        low  = 0U;
        high = 256U;

        /* Implement golden section search algorithm */
        do
        {
            scaler = (uint8_t)((low + high) / 2U);
            freq1  = src_clk_freq / (s_baudrate_prescaler[prescaler] * (scaler + (uint32_t)2U));

            if (lpspi_abs_dif(bits_per_sec, bes_freq) > lpspi_abs_dif(bits_per_sec, freq1))
            {
                bes_freq = freq1;
            }
            if (freq1 < bits_per_sec)
            {
                high = scaler;
            }
            else
            {
                low = scaler;
            }
        } while ((high - low) > 1U);

        /* Evaluate last 2 scaler values */
        freq1 = src_clk_freq / (s_baudrate_prescaler[prescaler] * (low + (uint32_t)2U));
        freq2 = src_clk_freq / (s_baudrate_prescaler[prescaler] * (high + (uint32_t)2U));

        if (lpspi_abs_dif(bits_per_sec, freq1) > lpspi_abs_dif(bits_per_sec, freq2))
        {
            tmp_best_freq   = freq2;
            tmp_best_scaler = high;
        }
        else
        {
            tmp_best_freq   = freq1;
            tmp_best_scaler = low;
        }

        if (lpspi_abs_dif(bits_per_sec, bes_freq) >= lpspi_abs_dif(bits_per_sec, tmp_best_freq))
        {
            bes_freq      = tmp_best_freq;
            best_scaler    = tmp_best_scaler;
            best_prescaler = prescaler;
        }

        /* If current frequency is equal to target frequency  stop the search */
        if (bes_freq == bits_per_sec)
        {
            break;
        }
    }

    /* Add default values for delay between transfers, delay between sck to pcs
     * and between pcs to sck. */
    (void)lpspi_set_delay(p_base, LPSPI_SCK_TO_PCS, best_scaler >> 2U);
    (void)lpspi_set_delay(p_base, LPSPI_PCS_TO_SCK, best_scaler >> 2U);
    (void)lpspi_set_delay(p_base, LPSPI_BETWEEN_TRANSFER, best_scaler >> 2U);

    /* Write the best baud rate scalar to the CCR.
     * Note, no need to check for error since we've already checked to make sure
     * the module is disabled and in master mode. Also, there is a limit on the
     * maximum divider so we will not exceed this.
     */
    (void)lpspi_set_baud_rate_divisor(p_base, best_scaler);

    /* return the best prescaler value for user to use later */
    *p_tcr_prescale_value = best_prescaler;

    /* return the actual calculated baud rate */
    return bes_freq;
}

status_t
lpspi_set_baud_rate_divisor(LPSPI_t *p_base, uint32_t divisor)
{
    uint32_t reg_val;

    reg_val = p_base->CCR;
    reg_val &= ~(LPSPI_CCR_SCKDIV_MASK);
    reg_val |= LPSPI_CCR_SCKDIV(divisor);
    p_base->CCR = reg_val;

    return STATUS_SUCCESS;
}

void
lpspi_set_pcs(LPSPI_t *p_base, lpspi_which_pcs_t which_pcs)
{
    uint32_t reg_val;

    reg_val = p_base->TCR;
    reg_val &= (uint32_t)(~(LPSPI_TCR_PCS_MASK));
    reg_val |= (uint32_t)((uint32_t)which_pcs << LPSPI_TCR_PCS_SHIFT);
    p_base->TCR = reg_val;
}

void
lpspi_set_tx_cmd_reg_tcr(LPSPI_t *p_base, const lpspi_tx_cmd_config_t *tx_cmd_cfg)
{
    p_base->TCR = (((uint32_t)tx_cmd_cfg->clk_polarity << LPSPI_TCR_CPOL_SHIFT) |
                   ((uint32_t)tx_cmd_cfg->clk_phase << LPSPI_TCR_CPHA_SHIFT) |
                   ((uint32_t)tx_cmd_cfg->pre_div << LPSPI_TCR_PRESCALE_SHIFT) |
                   ((uint32_t)tx_cmd_cfg->which_pcs << LPSPI_TCR_PCS_SHIFT) |
                   ((uint32_t)tx_cmd_cfg->b_lsb_first << LPSPI_TCR_LSBF_SHIFT) |
                   ((uint32_t)tx_cmd_cfg->b_byte_swap << LPSPI_TCR_BYSW_SHIFT) |
                   ((uint32_t)tx_cmd_cfg->b_cont_xfer << LPSPI_TCR_CONT_SHIFT) |
                   ((uint32_t)tx_cmd_cfg->b_cont_cmd << LPSPI_TCR_CONTC_SHIFT) |
                   ((uint32_t)tx_cmd_cfg->b_rx_mask << LPSPI_TCR_RXMSK_SHIFT) |
                   ((uint32_t)tx_cmd_cfg->b_tx_mask << LPSPI_TCR_TXMSK_SHIFT) |
                   ((uint32_t)tx_cmd_cfg->width << LPSPI_TCR_WIDTH_SHIFT) |
                   ((uint32_t)(tx_cmd_cfg->frame_size - 1UL) << LPSPI_TCR_FRAMESZ_SHIFT));
}

/*******************************************************************************
 * Static functions
 ******************************************************************************/
static uint32_t
lpspi_abs_dif(uint32_t a, uint32_t b)
{
    if (a > b)
    {
        return (a - b);
    }
    else
    {
        return (b - a);
    }
}

/*** end of file ***/
