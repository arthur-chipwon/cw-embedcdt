/**
 * @file smc_driver.h
 * @brief Header file for the SMC module.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef SMC_DRIVER_H
#define SMC_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include "status.h"
#include "device_registers.h"
#include "power_manager.h"
#include "smc_access.h"

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Configures the power mode.
 *
 * This function configures the power mode control for both run, stop, and
 * stop sub mode if needed. Also it configures the power options for a specific
 * power mode. An application should follow the proper procedure to configure and
 * switch power modes between  different run and stop modes. For proper procedures
 * and supported power modes, see an appropriate chip reference manual.
 * See the smc_power_mode_config_t for required parameters to configure the power
 * mode and the supported options.
 * Other options may need to be individually configured through the HAL driver.
 * See the HAL driver header file for details.
 *
 * @param p_base  Base address for current SMC instance.
 * @param p_power_mode_config Power mode configuration structure smc_power_mode_config_t
 * @return errorCode SMC error code
 */
status_t smc_set_power_mode(SMC_t *const                         p_base,
                            const smc_power_mode_config_t *const p_power_mode_config);

/**
 * @brief Configures all power mode protection settings.
 *
 * This register can only write once after the power reset.
 * If the user has only a single option to set,
 * either use this function or use the individual set function.
 *
 * @param[in] p_base  Base address for current SMC instance.
 * @param[in] p_protect_config Configurations for the supported power mode protect settings
 *                      - See smc_power_mode_protection_config_t for details.
 */
void smc_set_protection_mode(SMC_t *const                                    p_base,
                             const smc_power_mode_protection_config_t *const p_protect_config);

#ifdef __cplusplus
}
#endif

#endif /* SMC_DRIVER_H */

/*** end of file ***/
