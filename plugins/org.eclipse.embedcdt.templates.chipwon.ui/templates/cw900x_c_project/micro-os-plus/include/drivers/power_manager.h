/**
 * @file power_manager.h
 * @brief Header file for the power manager driver.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "device_registers.h"
#include "status.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/**
 * @brief Callback-specific data.
 *
 * Pointer to data of this type is passed during callback registration. The pointer
 * is part of the power_manager_callback_user_config_t structure and is passed to
 * the callback during power mode change notifications.
 *
 * Implements    : power_manager_callback_data_t_Class
 */
typedef void power_manager_callback_data_t;

/**
 * @brief Power modes enumeration.
 *
 * Defines power modes. Used in the power mode configuration structure
 * (power_manager_user_config_t). From ARM core perspective, Power modes can be
 * generally divided into run modes (Run and Very low power run), deep sleep
 * modes (all Stop modes).
 *
 * List of all supported power modes: \n
 *      \li POWER_MANAGER_RUN - Run mode.
 *      \li POWER_MANAGER_VLPR - Very low power run mode.
 *      \li POWER_MANAGER_PSTOP2 - Partial stop 2 mode.
 *      \li POWER_MANAGER_PSTOP1 - Stop 1 mode.
 *      \li POWER_MANAGER_PSTOP2 - Stop 2 mode.
 *      \li POWER_MANAGER_VLPS - Very low power stop mode.
 *
 * Implements    : power_manager_modes_t_Class
 */
typedef enum
{
    POWER_MANAGER_RUN,   /**< Run mode. */
    POWER_MANAGER_VLPR,  /**< Very low power run mode.  */
    POWER_MANAGER_STOP1, /**< Stop 1 mode. */
    POWER_MANAGER_STOP2, /**< Stop 2 mode. */
    POWER_MANAGER_VLPS,  /**< Very low power stop mode.  */
    POWER_MANAGER_MAX
} power_manager_modes_t;

/**
 * @brief Power mode user configuration structure.
 *
 * List of power mode configuration structure members depends on power options
 * available for the specific chip. Complete list contains:
 * power_mode - See power_manager_modes_t list of modes.
 * b_sleep_on_exit_value - When set to true, ARM core returns to sleep or deep sleep state
 *      after interrupt service finishes. When set to false, core stays woken-up.
 *
 * Implements    : power_manager_user_config_t_Class
 */
typedef struct
{
    power_manager_modes_t power_mode;            /**< power mode. */
    bool                  b_sleep_on_exit_value; /**< ARM core returns to sleep state. */
} power_manager_user_config_t;

/**
 * @brief Power Modes in PMSTAT
 *
 */
typedef enum
{
    STAT_RUN     = 0x01, /**< 0000_0001 - Current power mode is RUN*/
    STAT_STOP    = 0x02, /**< 0000_0010 - Current power mode is STOP*/
    STAT_VLPR    = 0x04, /**< 0000_0100 - Current power mode is VLPR*/
    STAT_VLPS    = 0x10, /**< 0001_0000 - Current power mode is VLPS*/
    STAT_INVALID = 0xFF  /**< 1111_1111 - Non-existing power mode*/
} power_mode_stat_t;

/**
 * @brief Run mode definition
 *
 */
typedef enum
{
    SMC_RUN, /**< normal RUN mode*/
    SMC_RESERVED_RUN,
    SMC_VLPR /**< Very-Low-Power RUN mode*/
} smc_run_mode_t;

/**
 * @brief Stop mode definition
 *
 */
typedef enum
{
    SMC_STOP           = 0U, /**< Normal STOP mode*/
    SMC_RESERVED_STOP1 = 1U, /**< Reserved*/
    SMC_VLPS           = 2U  /**< Very-Low-Power STOP mode*/
} smc_stop_mode_t;

/**
 * @brief STOP option
 *
 */
typedef enum
{
    SMC_STOP_RESERVED = 0x00, /**< Reserved stop mode */
    SMC_STOP1         = 0x01, /**< Stop with both system and bus clocks disabled */
    SMC_STOP2         = 0x02  /**< Stop with system clock disabled and bus clock enabled */
} smc_stop_option_t;

/**
 * @brief Power mode protection configuration
 *
 */
typedef struct
{
    bool b_vlp_prot; /**< VLP protect*/
} smc_power_mode_protection_config_t;

/**
 * @brief Power mode control configuration used for calling the SMC_SYS_SetPowerMode API
 *
 */
typedef struct
{
    power_manager_modes_t power_mode_name;   /**< Power mode(enum), see power_manager_modes_t */
    smc_stop_option_t     stop_option_value; /**< STOPO option(enum), see smc_stop_option_t */
} smc_power_mode_config_t;

/**
 * @brief Power manager policies.
 *
 * Defines whether the mode switch initiated by the power_set_mode() is agreed
 * upon (depending on the result of notification callbacks), or forced.
 *
 * Implements    : power_manager_policy_t_Class
 */
typedef enum
{
    POWER_MANAGER_POLICY_AGREEMENT, /**< Power mode is changed if the callbacks return success. */
    POWER_MANAGER_POLICY_FORCIBLE /**< Power mode is changed regardless of the result of callbacks*/
} power_manager_policy_t;

/**
 * @brief The PM notification type. Used to notify registered callbacks.
 * Callback notifications can be invoked in following situations:
 *  - before a power mode change (Callback return value can affect power_set_mode() execution.
 *  - after a successful change of the power mode.
 *  - after an unsuccessful attempt to switch power mode, in order to recover to a working state.
 *
 * Implements    : power_manager_notify_t_Class
 */
typedef enum
{
    POWER_MANAGER_NOTIFY_RECOVER = 0x00U, /**< Recover to previous work state. */
    POWER_MANAGER_NOTIFY_BEFORE  = 0x01U, /**< The system will change the power setting. */
    POWER_MANAGER_NOTIFY_AFTER   = 0x02U  /**< The system has changed to a new power setting. */
} power_manager_notify_t;

/**
 * @brief The callback type indicates when a callback will be invoked.
 *
 * Used in the callback configuration structures (power_manager_callback_user_config_t)
 * to specify when the registered callback will be called during power mode change
 * initiated by power_set_mode().
 *
 * Implements    : power_manager_callback_type_t_Class
 */
typedef enum
{
    POWER_MANAGER_CALLBACK_BEFORE       = 0x01U, /**< Before callback. */
    POWER_MANAGER_CALLBACK_AFTER        = 0x02U, /**< After callback. */
    POWER_MANAGER_CALLBACK_BEFORE_AFTER = 0x03U  /**< Before-After callback. */
} power_manager_callback_type_t;

/**
 * @brief Power notification structure passed to registered callback function
 *
 * Implements    : power_manager_notify_struct_t_Class
 */
typedef struct
{
    power_manager_user_config_t
                          *p_target_power_config;     /**< Pointer to target power configuration */
    uint8_t                target_power_config_index; /**< Target power configuration index. */
    power_manager_policy_t policy;                    /**< Power mode transition policy. */
    power_manager_notify_t pwr_notify_type;           /**< Power mode notification type. */
} power_manager_notify_struct_t;

/**
 * @brief Callback prototype.
 *
 * Declaration of callback. It is common for all registered callbacks.
 * Function pointer of this type is part of power_manager_callback_user_config_t callback
 * configuration structure.

 * @param p_notify Notification structure.
 * @param p_data_ptr Callback data. Pointer to the data passed during callback registration.
 * Intended to pass any driver or application data such as internal state information.
 * @return An error code or STATUS_SUCCESS.
 *
 * Implements    : power_manager_callback_t_Class
 */
typedef status_t (*power_manager_callback_t)(power_manager_notify_struct_t *p_notify,
                                             power_manager_callback_data_t *p_data_ptr);

/**
 * @brief callback configuration structure
 *
 * This structure holds configuration of callbacks passed to the Power manager
 * during its initialization.
 * Structures of this type are expected to be statically allocated.
 * This structure contains following application-defined data:
 *          callback - pointer to the callback function
 *          callback_type - specifies when the callback is called
 *          p_callback_data - pointer to the data passed to the callback
 *
 * Implements    : power_manager_callback_user_config_t_Class
 */
typedef struct
{
    power_manager_callback_t       callback_function;
    power_manager_callback_type_t  callback_type;
    power_manager_callback_data_t *p_callback_data;
} power_manager_callback_user_config_t;

/**
 * @brief Power manager internal state structure.
 *
 * Power manager internal structure. Contains data necessary for Power manager proper
 * functionality. Stores references to registered power mode configurations,
 * callbacks, and other internal data.
 * This structure is statically allocated and initialized by power_init().
 *
 * Implements    : power_manager_state_t_Class
 */
typedef struct
{
    power_manager_user_config_t *(*pp_configs)[]; /**< Pointer to power configure table.*/
    uint8_t configs_number;                       /**< Number of power configurations */
    power_manager_callback_user_config_t *(
        *pp_static_callbacks)[];                  /**< Pointer to callback table. */
    uint8_t static_callbacks_number;              /**< Max. number of callback configurations */
    uint8_t err_callback_index;                   /**< Index of callback returns error. */
    uint8_t current_config;                       /**< Index of current configuration.  */
} power_manager_state_t;

/*******************************************************************************
 * Exported variables
 ******************************************************************************/
/** @brief Power manager internal structure. */
extern power_manager_state_t g_power_manager_state;

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief This function implementation-specific configuration of power modes.
 *
 * This function performs the actual implementation-specific initialization
 * based on the provided power mode configurations.
 * In addition, This function get all clock source were enabled.
 * This one was used for update init clock when CPU jump from very low power mode
 * to run mode.
 *
 * @return Operation status
 *        - STATUS_SUCCESS: Operation was successful.
 *        - STATUS_ERROR: Operation failed.
 */
status_t power_do_init(void);

/**
 * @brief This function implementation-specific de-initialization of power manager.
 *
 * This function performs the actual implementation-specific de-initialization.
 *
 * @return Operation status
 *        - STATUS_SUCCESS: Operation was successful.
 *        - STATUS_ERROR: Operation failed.
 */
status_t power_do_deinit(void);

/**
 * @brief This function configures the power mode.
 *
 * This function performs the actual implementation-specific logic to switch to
 * one of the defined power modes.
 *
 * @param p_config_ptr: Pointer to user configuration structure
 * @return Operation status
 *        - STATUS_SUCCESS: Operation was successful.
 *        - STATUS_MCU_TRANSITION_FAILED: Operation failed.
 */
status_t power_do_set_mode(const power_manager_user_config_t *const p_config_ptr);

/**
 * @brief Power manager initialization for operation.
 *
 * This function initializes the Power manager and its run-time state structure.
 * Pointer to an array of Power mode configuration structures needs to be passed
 * as a parameter along with a parameter specifying its size. At least one power mode
 * configuration is required. Optionally, pointer to the array of predefined
 * callbacks can be passed with its corresponding size parameter.
 * For details about callbacks, refer to the power_manager_callback_user_config_t.
 * As Power manager stores only pointers to arrays of these structures,
 * they need to exist and be valid for the entire life cycle of Power manager.
 *
 * @param[in] pp_power_configs_ptr A pointer to an array of pointers to all power
 *                          configurations which will be handled by Power manager.
 * @param[in] configs_number Number of power configurations. Size of powerConfigsPtr array.
 * @param[in] pp_callbacks_ptr A pointer to an array of pointers to callback configurations.
 *                          If there are no callbacks to register during Power manager
 *                          initialization, use NULL value.
 * @param[in] callbacksNumber Number of registered callbacks. Size of callbacksPtr array.
 * @return An error code or STATUS_SUCCESS.
 */
status_t power_init(power_manager_user_config_t *(*pp_power_configs_ptr)[],
                    uint8_t configs_number,
                    power_manager_callback_user_config_t *(*pp_callbacks_ptr)[],
                    uint8_t callbacksNumber);

/**
 * @brief This function deinitializes the Power manager.
 *
 * @return An error code or STATUS_SUCCESS.
 */
status_t power_deinit(void);

/**
 * @brief This function configures the power mode.
 *
 * This function switches to one of the defined power modes. Requested mode number is passed
 * as an input parameter. This function notifies all registered callback functions before
 * the mode change (using POWER_MANAGER_CALLBACK_BEFORE set as callback type parameter),
 * sets specific power options defined in the power mode configuration and enters
 * the specified mode. In case of run modes (for example, Run, Very low power run),
 * this function also invokes all registered callbacks after the mode change (using
 * POWER_MANAGER_CALLBACK_AFTER). In case of sleep or deep sleep modes, if the requested mode
 * is not exited through a reset, these notifications are sent after the core wakes up.
 * Callbacks are invoked in the following order: All registered callbacks are notified ordered
 * by index in the callbacks array (see callbacksPtr parameter of power_init().
 * The same order is used for before and after switch notifications. The notifications
 * before the power mode switch can be used to obtain confirmation about the change from
 * registered callbacks. If any registered callback denies the power mode change,
 * further execution of this function depends on mode change policy: the mode change
 * is either forced(POWER_MANAGER_POLICY_FORCIBLE) or aborted(POWER_MANAGER_POLICY_AGREEMENT).
 * When mode change is forced, the results of the before switch notifications are ignored.
 * If agreement is requested, in case any callback returns an error code then further
 * before switch notifications are cancelled and all already notified callbacks
 * are re-invoked with POWER_MANAGER_CALLBACK_AFTER set as callback type parameter.
 * The index of the callback which returned error code during pre-switch notifications
 * is stored and can be obtained by using power_get_error_callback().
 * Any error codes during callbacks re-invocation (recover phase) are ignored.
 * power_set_mode() returns an error code denoting the phase in which a callback failed.
 * It is possible to enter any mode supported by the processor. Refer to the chip
 * reference manual for the list of available power modes.
 *
 * @param[in] power_mode_index Requested power mode represented as an index into array of
 *                  user-defined power mode configurations passed to the power_init().
 * @param[in] policy Transaction policy
 * @return An error code or STATUS_SUCCESS.
 */
status_t power_set_mode(uint8_t power_mode_index, power_manager_policy_t policy);

/**
 * @brief This function returns the last successfully set power mode.
 *
 * This function returns index of power mode which was last set using power_set_mode().
 * If the power mode was entered even though some of the registered callbacks denied
 * the mode change, or if any of the callbacks invoked after the entering/restoring
 * run mode failed, then the return code of this function has STATUS_ERROR value.
 *
 * @param[out] p_power_mode_index_ptr Power mode which has been set represented as an index
 *                      into array of power mode configurations passed to the power_init().
 * @return An error code or STATUS_SUCCESS.
 */
status_t power_get_last_mode(uint8_t *p_power_mode_index_ptr);

/**
 * @brief This function returns the user configuration structure of the last
 * successfully set power mode.
 *
 * This function returns a pointer to configuration structure which was last set using
 * power_set_mode(). If the current power mode was entered even though some of the registered
 * callbacks denied the mode change, or if any of the callbacks invoked after the entering/restoring
 * run mode failed, then the return code of this function has STATUS_ERROR value.
 *
 * @param[out] pp_power_mode_ptr Pointer to power mode configuration structure of
 *                          the last set power mode.
 * @return An error code or STATUS_SUCCESS.
 */
status_t power_get_last_mode_config(power_manager_user_config_t **pp_power_mode_ptr);

/**
 * @brief This function returns currently running power mode.
 *
 * This function reads hardware settings and returns currently running power mode.
 *
 * @return Currently used run power mode.
 */
power_manager_modes_t power_get_current_mode(void);

/**
 * @brief This function returns the last failed notification callback.
 *
 * This function returns the index of the last callback that failed during the power mode switch
 * when power_set_mode() was called. The returned value represents the index in the array of
 * registered callbacks. If the last power_set_mode() call ended successfully, a value equal to
 * the number of registered callbacks is returned.
 *
 * @return Callback index of last failed callback or value equal to callbacks count.
 */
uint8_t power_get_error_callback_index(void);

/**
 * @brief This function returns the callback configuration structure for
 * the last failed notification.
 *
 * This function returns a pointer to configuration structure of the last callback that
 * failed during the power mode switch when power_set_mode() was called. If the last
 * power_set_mode() call ended successfully, a NULL value is returned.
 *
 * @return Pointer to the callback configuration which returns error.
 */
power_manager_callback_user_config_t *power_get_error_callback(void);

/**
 * @brief This function returns the default power_manager configuration structure.
 *
 * This function returns a pointer of the power_manager configuration structure.
 * All structure members have default value when CPU is default power mode.
 *
 */
void power_get_default_config(power_manager_user_config_t *const p_config);

#ifdef __cplusplus
}
#endif

#endif /* POWER_MANAGER_H */

/*** end of file ***/
