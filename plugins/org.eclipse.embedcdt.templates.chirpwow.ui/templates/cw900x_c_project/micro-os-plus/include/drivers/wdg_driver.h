/**
 * @file wdg_driver.h
 * @brief WDG driver header file.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef WDG_DRIVER_H
#define WDG_DRIVER_H

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
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/

/**
 * @brief Clock sources for the WDOG.
 * Implements : wdog_clk_source_t_Class
 */
typedef enum
{
    WDOG_LPO_CLOCK = 0x01U, /*!< LPO clock */
} wdog_clk_source_t;

/**
 * @brief WDOG user configuration structure
 *
 * clkSource     -  The clock source of the WDOG
 * updateEnable	 -	The modes in which the WDOG is functional
 * intEnable	 -  If true, further updates of the WDOG are enabled
 * winEnable	 -  If true, an interrupt request is generated before reset
 * windowValue	 -  If true, window mode is enabled
 * timeoutValue  -  The timeout value
 * prescalerEnable - a fixed 256 prescaling of the counter clock is enabled
 *
 */
typedef struct
{
    wdog_clk_source_t clkSource;
    bool              updateEnable;
    bool              intEnable;
    bool              winEnable;
    uint16_t          windowValue;
    uint16_t          timeoutValue;
    bool              prescalerEnable;
} wdog_user_config_t;

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Configures all WDOG registers.
 *
 * @param[in] base - WDOG base pointer
 * @param[in] wdogUserConfig - Pointer to the input configuration structure
 * @return Operation status
 */
status_t wdog_config(WDOG_t *const base, const wdog_user_config_t *wdogUserConfig);

/**
 * @brief Refreshes the WDOG counter.
 *
 * @param[in] instance  WDOG instance number
 */
void wdog_trigger(uint32_t instance);

/**
 * @brief Get the value of the WDOG counter.
 *
 * @param[in] instance  WDOG instance number
 */
uint16_t wdog_get_counter(uint32_t instance);

#ifdef __cplusplus
}
#endif

#endif /* WDG_DRIVER_H */
