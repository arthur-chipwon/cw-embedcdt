/**
 * @file trgmux_driver.h
 * @brief Header file for the trgmux driver.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef TRGMUX_DRIVER_H
#define TRGMUX_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "status.h"
#include "device_registers.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/**
 * @brief Enumeration for trigger source module of TRGMUX
 *
 * Describes all possible inputs (trigger sources) of the TRGMUX IP
 * This enumeration depends on the supported instances in device
 *
 * Implements : trgmux_trigger_source_t_Class
 */
typedef enum trgmux_trigger_source_e trgmux_trigger_source_t;

/**
 * @brief Enumeration for target module of TRGMUX
 *
 * Describes all possible outputs (target modules) of the TRGMUX IP
 * This enumeration depends on the supported instances in device
 *
 * Implements : trgmux_target_module_t_Class
 */
typedef enum trgmux_target_module_e trgmux_target_module_t;

/**
 * @brief Configuration structure for pairing source triggers with target modules.
 *
 * Use an instance of this structure to define a TRGMUX link between a trigger source
 * and a target module. This structure is used by the user configuration structure.
 *
 * Implements : trgmux_inout_mapping_config_t_Class
 */
typedef struct
{
    trgmux_trigger_source_t trigger_source; /**< selects one of the TRGMUX trigger sources */
    trgmux_target_module_t  target_module;  /**< selects one of the TRGMUX target modules  */
    bool b_lock_target_module_reg;          /**< if true, the LOCK bit of the target module
                                          register will be set by trgmux_init(),
                                          after the current mapping is configured */
} trgmux_inout_mapping_config_t;

/**
 * @brief User configuration structure for the TRGMUX driver.
 *
 * Use an instance of this structure with the trgmux_init() function.
 * This enables configuration of TRGMUX with the user defined mappings
 * between inputs (source triggers) and outputs (target modules), via a
 * single function call.
 *
 * Implements : trgmux_user_config_t_Class
 */
typedef struct
{
    uint8_t num_in_out_mapping_configs; /**< number of in-out mappings defined
                                       in TRGMUX configuration */
    const trgmux_inout_mapping_config_t *p_in_out_mapping_config; /**< pointer to array of in-out
                                                                mapping structures */
} trgmux_user_config_t;

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Initialize a TRGMUX instance for operation.
 *
 * This function first resets the source triggers of all TRGMUX target
 * modules to their default values, then configures the TRGMUX with all the
 * user defined in-out mappings. If at least one of the target modules is
 * locked, the function will not change any of the TRGMUX target modules and
 * return error code. This example shows how to set up the
 * trgmux_user_config_t parameters and how to call the trgmux_init()
 * function with the required parameters:
 *  @code
 *   trgmux_user_config_t             trgmuxConfig;
 *   trgmux_inout_mapping_config_t    trgmuxInoutMappingConfig[] =
 *   {
 *       {TRGMUX_TRIG_SOURCE_TRGMUX_IN9,     TRGMUX_TARGET_MODULE_DMA_CH0, false},
 *       {TRGMUX_TRIG_SOURCE_FTM1_EXT_TRIG,  TRGMUX_TARGET_MODULE_TRGMUX_OUT4, true}
 *   };
 *
 *   trgmuxConfig.num_in_out_mapping_configs = 2;
 *   trgmuxConfig.p_in_out_mapping_config     = trgmuxInoutMappingConfig;
 *   trgmux_init(instance, &trgmuxConfig);
 *   @endcode
 *
 * @param[in] instance          The TRGMUX instance number.
 * @param[in] p_trgmux_user_config  Pointer to the user configuration structure.
 * @return                      Execution status: \n
 *   STATUS_SUCCESS \n
 *   STATUS_ERROR    - if at least one of the target module register is locked.
 */
status_t trgmux_init(const uint32_t                    instance,
                     const trgmux_user_config_t *const p_trgmux_user_config);

/**
 * @brief Reset to default values the source triggers corresponding to all
 * target modules, if none of the target modules is locked.
 *
 * @param[in] instance          The TRGMUX instance number.
 * @return                      Execution status: \n
 *   STATUS_SUCCESS \n
 *   STATUS_ERROR    - if at least one of the target module register is locked.
 */
status_t trgmux_deinit(const uint32_t instance);

/**
 * @brief Configure a source trigger for a selected target module.
 *
 * This function configures a TRGMUX link between a source trigger and a
 * target module, if the requested target module is not locked.
 *
 * @param[in] instance          The TRGMUX instance number.
 * @param[in] trigger_source     One of the values in the trgmux_trigger_source_t enumeration
 * @param[in] target_module      One of the values in the trgmux_target_module_t enumeration
 * @return                      Execution status: \n
 *   STATUS_SUCCESS \n
 *   STATUS_ERROR    - if requested target module is locked
 */
status_t trgmux_set_trig_source_for_target_module(const uint32_t                instance,
                                                  const trgmux_trigger_source_t trigger_source,
                                                  const trgmux_target_module_t  target_module);

/**
 * @brief Get the source trigger configured for a target module.
 *
 * This function returns the TRGMUX source trigger linked to a selected target module.
 *
 * @param[in] instance      The TRGMUX instance number.
 * @param[in] target_module  One of the values in the trgmux_target_module_t enumeration.
 * @return                  Enum value corresponding to the trigger source configured
 *                           for the selected target module.
 */
trgmux_trigger_source_t trgmux_get_trig_source_for_target_module(
    const uint32_t instance, const trgmux_target_module_t target_module);

/**
 * @brief Locks the TRGMUX register of a target module.
 *
 * This function sets the LK bit of the TRGMUX register corresponding to
 * the selected target module. Please note that some TRGMUX registers can
 * contain up to 4 SEL bitfields, meaning that these registers can be used
 * to configure up to 4 target modules independently. Because the LK bit is
 * only one per register, the configuration of all target modules referred
 * from that register will be locked.
 *
 * @param[in] instance          The TRGMUX instance number.
 * @param[in] target_module      One of the values in the trgmux_target_module_t enumeration
 */
void trgmux_set_lock_for_target_module(const uint32_t               instance,
                                       const trgmux_target_module_t target_module);

/**
 * @brief Get the Lock bit status of the TRGMUX register of a target module.
 *
 * This function gets the value of the LK bit from the TRGMUX register
 * corresponding to the selected target module.
 *
 * @param[in] instance          The TRGMUX instance number.
 * @param[in] target_module      One of the values in the trgmux_target_module_t enumeration
 * @return                      true - if the selected target_module register is locked \n
 *                              false - if the selected target_module register is not locked
 */
bool trgmux_get_lock_for_target_module(const uint32_t               instance,
                                       const trgmux_target_module_t target_module);

#ifdef __cplusplus
}
#endif

#endif /* TRGMUX_DRIVER_H */

/*** end of file ***/
