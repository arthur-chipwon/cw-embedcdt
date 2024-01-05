/**
 * @file lptmr_driver.c
 * @brief LPTMR configure and set operations.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lptmr_driver.h"
#include "lptmr_access.h"
/*******************************************************************************
 * Static variables
 ******************************************************************************/
/* Table of base addresses for LPTMR instances */
static LPTMR_t *const g_lptmrBase[LPTMR_INSTANCE_COUNT] = LPTMR_BASE_PTRS;

/*******************************************************************************
 * Global functions
 ******************************************************************************/
void
lptmr_init(const uint32_t              instance,
           const lptmr_config_t *const p_config,
           const bool                  b_start_counter)
{
    DEV_ASSERT(instance < LPTMR_INSTANCE_COUNT);
    DEV_ASSERT(p_config != NULL);

    LPTMR_t *const p_base = g_lptmrBase[instance];

    lptmr_set_config(instance, p_config);

    if (p_config->b_interrupt_enable == true)
    {
        NVIC->ISER[(uint32_t)(LPTMR0_IRQn) >> 5U] =
            (uint32_t)(1UL << ((uint32_t)(LPTMR0_IRQn) & (uint32_t)0x1FU));
    }

    /* Start the counter if requested */
    if (b_start_counter)
    {
        lptmr_enable(p_base);
    }
}

void
lptmr_set_reset_value(const uint32_t instance)
{
    DEV_ASSERT(instance < LPTMR_INSTANCE_COUNT);

    LPTMR_t *const p_base = g_lptmrBase[instance];
    uint32_t tmp = p_base->CSR;
    tmp &= ~(LPTMR_CSR_TEN_MASK | LPTMR_CSR_TCF_MASK);
    tmp |= LPTMR_CSR_TEN(0u);
    p_base->CSR = tmp;

    p_base->CSR = 0UL;

    p_base->PSR = 0UL;

    p_base->CMR = 0UL;
}

void
lptmr_set_config(const uint32_t instance, const lptmr_config_t *const p_config)
{
    DEV_ASSERT(instance < LPTMR_INSTANCE_COUNT);
    DEV_ASSERT(p_config != NULL);

    LPTMR_t *const    p_base           = g_lptmrBase[instance];
    uint32_t          config_cmp_value = p_config->compare_value;
    lptmr_workmode_t  config_work_mode = p_config->work_mode;
    uint16_t          cmp_value_ticks  = 0U;
    lptmr_prescaler_t presc_val        = p_config->prescaler;

    if (config_work_mode == LPTMR_WORKMODE_TIMER)
    {
        DEV_ASSERT(config_cmp_value <= LPTMR_CMR_COMPARE_MASK);
        cmp_value_ticks = (uint16_t)(config_cmp_value & LPTMR_CMR_COMPARE_MASK);
    }
    else
    {
        /* If config_work_mode is not LPTMR_WORKMODE_TIMER, then it must be
         * LPTMR_WORKMODE_PULSECOUNTER. */
        DEV_ASSERT(config_work_mode == LPTMR_WORKMODE_PULSECOUNTER);

        DEV_ASSERT(config_cmp_value <= LPTMR_CMR_COMPARE_MASK);

        cmp_value_ticks = (uint16_t)(config_cmp_value & LPTMR_CMR_COMPARE_MASK);
    }

    /* Initialize and write configuration parameters. */
    lptmr_set_reset_value(instance);

    lptmr_set_dma_request(p_base, p_config->b_dma_request);
    lptmr_set_interrupt(p_base, p_config->b_interrupt_enable);
    lptmr_set_free_running(p_base, p_config->b_free_run);
    lptmr_set_work_mode(p_base, config_work_mode);
    lptmr_set_prescaler(p_base, presc_val);
    lptmr_set_clock_select(p_base, p_config->clock_select);
    lptmr_set_compare_value(p_base, cmp_value_ticks);
    lptmr_set_pin_select(p_base, p_config->pin_select);
    lptmr_set_pin_polarity(p_base, p_config->pin_polarity);
}

void
lptmr_get_config(const uint32_t instance, lptmr_config_t *const p_config)
{
    DEV_ASSERT(instance < LPTMR_INSTANCE_COUNT);
    DEV_ASSERT(p_config != NULL);

    const LPTMR_t *const p_base = g_lptmrBase[instance];

    /* Read current configuration */
    p_config->b_dma_request      = lptmr_get_dma_request(p_base);
    p_config->b_interrupt_enable = lptmr_get_interrupt_enable(p_base);
    p_config->b_free_run         = lptmr_get_free_running(p_base);
    p_config->work_mode          = lptmr_get_work_mode(p_base);
    p_config->prescaler          = lptmr_get_prescaler(p_base);
    p_config->clock_select       = lptmr_get_clock_select(p_base);
    p_config->compare_value      = lptmr_get_compare_value(p_base);
    p_config->pin_select         = lptmr_get_pin_select(p_base);
    p_config->pin_polarity       = lptmr_get_pin_polarity(p_base);
}

void
lptmr_start_counter(const uint32_t instance)
{
    DEV_ASSERT(instance < LPTMR_INSTANCE_COUNT);

    LPTMR_t *const p_base = g_lptmrBase[instance];

    lptmr_enable(p_base);
}

void
lptmr_stop_counter(const uint32_t instance)
{
    DEV_ASSERT(instance < LPTMR_INSTANCE_COUNT);

    LPTMR_t *const p_base = g_lptmrBase[instance];

    lptmr_disable(p_base);
}

uint16_t
lptmr_get_counter_value(const uint32_t instance)
{
    DEV_ASSERT(instance < LPTMR_INSTANCE_COUNT);
    LPTMR_t *const p_base = g_lptmrBase[instance];

    /* Write dummy value before reading register */
    p_base->CNR  = LPTMR_CNR_COUNTER(0u);
    uint16_t cnr = (uint16_t)p_base->CNR;
    return cnr;
}

void
lptmr_clear_compare_flag(const uint32_t instance)
{
    DEV_ASSERT(instance < LPTMR_INSTANCE_COUNT);

    LPTMR_t *const base = g_lptmrBase[instance];

    lptmr_clear_compare_flag_reg(base);
}
/*** end of file ***/
