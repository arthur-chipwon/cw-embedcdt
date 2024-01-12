/**
 * @file wdg_driver.c
 * @brief WDG configure and set operations.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "status.h"
#include "wdg_driver.h"
#include "wdg_access.h"
/*******************************************************************************
 * Private macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/** The dummy read is used in order to make sure that any write to the
 * WDOG registers will be started only after the write of the unlock value was
 * completed.
 */
#define WDOG_UNLOCK32(base)                      \
    {                                            \
        (base)->CNT = FEATURE_WDOG_UNLOCK_VALUE; \
        (void)(base)->CNT;                       \
    }

#define WDOG_UNLOCK16(base)                               \
    {                                                     \
        (base)->CNT = FEATURE_WDOG_UNLOCK16_FIRST_VALUE;  \
        (void)(base)->CNT;                                \
        (base)->CNT = FEATURE_WDOG_UNLOCK16_SECOND_VALUE; \
        (void)(base)->CNT;                                \
    }

#define WDOG_UNLOCK(base)                              \
    {                                                  \
        if (((base)->CS & WDOG_CS_CMD32EN_MASK) != 0U) \
        {                                              \
            WDOG_UNLOCK32(base);                       \
        }                                              \
        else                                           \
        {                                              \
            WDOG_UNLOCK16(base);                       \
        }                                              \
    }

/*! @brief Table of base addresses for WDOG instances. */
static WDOG_t *const s_wdogBase[] = WDOG_BASE_PTRS;

/*******************************************************************************
 * Global functions
 ******************************************************************************/
status_t
wdog_config(WDOG_t *const base, const wdog_user_config_t *wdogUserConfig)
{
    status_t status = STATUS_SUCCESS;
    uint32_t cs     = base->CS;
    bool     tmp1   = wdog_is_enabled(base);
    bool     tmp2   = wdog_is_update_enabled(base);

    if ((tmp1 == false) && (tmp2 == true))
    {
        /* Clear the bits used for configuration */
        cs &= ~(WDOG_CS_WIN_MASK | WDOG_CS_PRES_MASK | WDOG_CS_CLK_MASK | WDOG_CS_INT_MASK |
                WDOG_CS_UPDATE_MASK | WDOG_CS_STOP_MASK);
        /* Construct CS register new value */
        cs |= WDOG_CS_WIN(wdogUserConfig->winEnable ? 1UL : 0UL);
        cs |= WDOG_CS_PRES(wdogUserConfig->prescalerEnable ? 1UL : 0UL);
        cs |= WDOG_CS_CLK(wdogUserConfig->clkSource);
        cs |= WDOG_CS_INT(wdogUserConfig->intEnable ? 1UL : 0UL);
        cs |= WDOG_CS_UPDATE(wdogUserConfig->updateEnable ? 1UL : 0UL);
        /* Reset interrupt flags */
        cs |= WDOG_CS_FLG_MASK;
        /* Enable WDOG in 32-bit mode */
        cs |= WDOG_CS_EN_MASK | WDOG_CS_CMD32EN_MASK;

        WDOG_UNLOCK(base);

        base->CS    = cs;
        base->TOVAL = wdogUserConfig->timeoutValue;
        if (wdogUserConfig->winEnable)
        {
            base->WIN = wdogUserConfig->windowValue;
        }

        if (wdogUserConfig->intEnable)
        {
            NVIC->ISER[(uint32_t)(WDOG_IRQn) >> 5U] =
                (uint32_t)(1UL << ((uint32_t)(WDOG_IRQn) & (uint32_t)0x1FU));
        }
    }
    else
    {
        status = STATUS_ERROR;
    }

    return status;
}

void
wdog_trigger(uint32_t instance)
{
    DEV_ASSERT(instance < WDOG_INSTANCE_COUNT);
    WDOG_t *base = s_wdogBase[instance];

    wdog_trigger_reg(base);

    for (int i = 0; i < 3; i++)
    {
        NOP();
    }
}

uint16_t
wdog_get_counter(uint32_t instance)
{
    DEV_ASSERT(instance < WDOG_INSTANCE_COUNT);
    const WDOG_t *base = s_wdogBase[instance];

    return (uint16_t)base->CNT;
}
