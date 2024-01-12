/**
 * @file wdg_access.h
 * @brief WDG driver header file.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef WDG_ACCESS_H
#define WDG_ACCESS_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "device_registers.h"

/*******************************************************************************
 * Static functions
 ******************************************************************************/
/**
 * @brief Verifies if the WDOG is enabled.
 *
 * This function verifies the state of the WDOG.
 *
 * @param[in] base WDOG base pointer.
 * @return the state of the WDOG.
 */
static inline bool
wdog_is_enabled(const WDOG_t *base)
{
    return ((base->CS & WDOG_CS_EN_MASK) >> WDOG_CS_EN_SHIFT) != 0U;
}

/**
 * @brief Refreshes the WDOG counter
 *
 * @param[in] base WDOG base pointer.
 */
static inline void
wdog_trigger_reg(WDOG_t *const base)
{
    if ((base->CS & WDOG_CS_CMD32EN_MASK) != 0U)
    {
        base->CNT = FEATURE_WDOG_TRIGGER_VALUE;
    }
    else
    {
        base->CNT = FEATURE_WDOG_TRIGGER16_FIRST_VALUE;
        (void)base->CNT;
        base->CNT = FEATURE_WDOG_TRIGGER16_SECOND_VALUE;
    }
}

/**
 * @brief Verifies if the WDOG updates are allowed.
 *
 * This function verifies if software is allowed to reconfigure the WDOG without
 * a reset.
 *
 * @param[in] base WDOG base pointer.
 * @return the state of the WDOG updates:
 *         - false: updates are not allowed
 *         - true: updates are allowed
 */
static inline bool
wdog_is_update_enabled(const WDOG_t *base)
{
    return ((base->CS & WDOG_CS_UPDATE_MASK) >> WDOG_CS_UPDATE_SHIFT) != 0U;
}

#ifdef __cplusplus
}
#endif

#endif /* WDG_ACCESS_H */
