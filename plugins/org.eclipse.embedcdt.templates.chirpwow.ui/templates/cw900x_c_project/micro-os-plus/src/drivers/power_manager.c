/**
 * @file power_manager.c
 * @brief This file provides access to the power manager.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <clock_gating.h>
#include <stddef.h>
#include "power_manager.h"
#include "scg_access.h"
#include "smc_access.h"
#include "smc_driver.h"

/*******************************************************************************
 * Global variables
 ******************************************************************************/
/** @brief Power manager internal structure. */
power_manager_state_t g_power_manager_state;

/*******************************************************************************
 * Static variables
 ******************************************************************************/
/* Save system clock configure */
static sys_clk_config_t s_sys_clk_config;
/* Confirm change clock when switch very low power run mode */
static bool sb_change_clk_vlp = false;
#if FEATURE_HAS_SPLL_CLK
/* Confirm clock source SPLL config enabled or disable */
static bool sb_enable_spll = false;
#endif /* #if FEATURE_HAS_SPLL_CLK */

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
static status_t power_switch_to_sleeping_power_mode(
    const power_manager_user_config_t *const p_config_ptr);

static status_t power_switch_to_running_power_mode(
    const power_manager_user_config_t *const p_config_ptr);

static status_t power_switch_vlpr_clk(const sys_clk_config_t *const p_sys_clock);

static status_t power_update_Init_clk(const sys_clk_config_t *const p_sys_clk);

/**
 * @brief Gets the default power_manager configuration structure.
 *
 * This function gets the power_manager configuration structure of the default power mode.
 *
 * @param[out] p_default_config : Pointer to power mode configuration structure of
 *                              the default power mode.
 */
static void power_do_get_default_config(power_manager_user_config_t *const p_default_config);

/**
 * @brief power_callbacks_management.
 *
 * Internal function used by power_set_mode function for callback management.
 *
 * @param[in] p_notify_struct : callback notification structure
 * @param[in] p_current_static_callback : index to array of statically registered call-backs
 * @param[in] policy : transaction policy
 */
static status_t power_callbacks_management(power_manager_notify_struct_t *p_notify_struct,
                                           uint8_t                       *p_current_static_callback,
                                           power_manager_policy_t         policy);

/*******************************************************************************
 * Global functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : power_smc_set_protection_mode
 * Description   : Configure all power mode protection settings
 * This register can only write once after power reset. So either use this function
 * or use the individual set function if you only have single option to set.
 *
 *END**************************************************************************/
void
power_smc_set_protection_mode(SMC_t *const                                    baseAddr,
                              const smc_power_mode_protection_config_t *const p_protect_config)
{
    /* Initialize the setting */
    uint32_t reg_val = (uint32_t)baseAddr->PMPROT;

    reg_val &= (uint32_t)(~(SMC_PMPROT_AVLP_MASK));

    /* Check configurations for each mode and combine the setting together */
    if (p_protect_config->b_vlp_prot)
    {
        reg_val |= SMC_PMPROT_AVLP(1);
    }

    /* Write once into PMPROT register*/
    baseAddr->PMPROT = reg_val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_do_init
 * Description   : It is expected that prior to the power_init() call the
 * write-once protection register was configured appropriately allowing entry to
 * all required low power modes.
 *
 *END**************************************************************************/
status_t
power_do_init(void)
{
    uint8_t                            idx = 0U;
    smc_power_mode_protection_config_t power_mode_prot_config;
    power_mode_prot_config.b_vlp_prot = false;

    for (idx = 0; idx < g_power_manager_state.configs_number; idx++)
    {
        const power_manager_user_config_t *const config = (*g_power_manager_state.pp_configs)[idx];

        if ((config->power_mode == POWER_MANAGER_VLPR) ||
            (config->power_mode == POWER_MANAGER_VLPS))
        {
            power_mode_prot_config.b_vlp_prot = true; /* Very low power mode is allowed. */
        }
    }

    /* Very low power modes are not protected. */
    power_smc_set_protection_mode(SMC, &power_mode_prot_config);
    /* Get all clock source were enabled. This one was used for update initialize clock when CPU
    came back RUN mode from very low power mode */

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_do_deinit
 * Description   : This function performs the actual implementation-specific de-initialization.
 *
 *END**************************************************************************/
status_t
power_do_deinit(void)
{
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_do_set_mode
 * Description   : This function performs the actual implementation-specific logic to switch
 * to one of the defined power modes.
 *
 *END**************************************************************************/
status_t
power_do_set_mode(const power_manager_user_config_t *const p_config_ptr)
{
    status_t return_code; /* Function return */

    /* Check whether the power mode is a sleeping or a running power mode */
    if (p_config_ptr->power_mode <= POWER_MANAGER_VLPR)
    {
        /* Switch to a running power mode */
        return_code = power_switch_to_running_power_mode(p_config_ptr);
    }
    else
    {
        /* Switch to a sleeping power mode */
        return_code = power_switch_to_sleeping_power_mode(p_config_ptr);
    }

    return return_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_get_current_mode
 * Description   : Returns currently running power mode.
 *
 * Implements    : power_get_current_mode_activity
 *END**************************************************************************/
power_manager_modes_t
power_get_current_mode(void)
{
    power_manager_modes_t ret_val;

    switch (smc_get_power_mode_status(SMC))
    {
        /* Run mode */
        case STAT_RUN:
        {
            ret_val = POWER_MANAGER_RUN;
        }
        break;
        /* Very low power run mode */
        case STAT_VLPR:
        {
            ret_val = POWER_MANAGER_VLPR;
        }
        break;
        /* This should never happen - core has to be in some run mode to execute code */
        default:
        {
            ret_val = POWER_MANAGER_MAX;
        }
        break;
    }

    return ret_val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_init
 * Description   : Initializes the Power manager for operation.
 * This function initializes the Power manager and its run-time state structure.
 * Reference to an array of Power mode configuration structures has to be passed
 * as parameter along with parameter specifying its size. At least one power mode
 * configuration is required. Optionally, reference to array of predefined
 * call-backs can be passed with its size parameter.
 * For details about call-backs refer to the power_manager_callback_user_config_t.
 * As Power manager stores only references to array of these structures they have
 * to exist while Power manager is used.
 *
 * Implements    : power_init_activity
 *END**************************************************************************/
status_t
power_init(power_manager_user_config_t *(*pp_power_configs_ptr)[],
           uint8_t configs_number,
           power_manager_callback_user_config_t *(*pp_callbacks_ptr)[],
           uint8_t callbacks_number)
{
    DEV_ASSERT(pp_power_configs_ptr != NULL); /* Reference to the power configurations is valid. */
    DEV_ASSERT(configs_number != 0U);         /* Power configuration index is valid. */
    DEV_ASSERT(g_power_manager_state.pp_configs ==
               NULL); /* Driver is not initialized, reference to configuration is not valid. */
    DEV_ASSERT(g_power_manager_state.configs_number ==
               0U); /* Driver is not initialized, number of configurations is zero. */

    /* Store references to user-defined power mode configurations */
    g_power_manager_state.pp_configs = (power_manager_user_config_t * (*)[]) pp_power_configs_ptr;
    g_power_manager_state.configs_number = configs_number;
    g_power_manager_state.current_config = 0U;

    /* Store references to user-defined callback configurations and increment call-back handle
     * counter */
    if (pp_callbacks_ptr != NULL)
    {
        g_power_manager_state.pp_static_callbacks =
            (power_manager_callback_user_config_t * (*)[]) pp_callbacks_ptr;
        g_power_manager_state.static_callbacks_number = callbacks_number;
        /* Default value of handle of last call-back that returned error */
        g_power_manager_state.err_callback_index = callbacks_number;
    }
    else
    {
        g_power_manager_state.pp_static_callbacks     = NULL;
        g_power_manager_state.static_callbacks_number = 0U;
        g_power_manager_state.err_callback_index      = 0U;
    }

    return power_do_init();
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_deinit
 * Description   : De-initializes the Power manager.
 *
 * Implements    : power_deinit_activity
 *END**************************************************************************/
status_t
power_deinit(void)
{
    g_power_manager_state.pp_configs              = NULL;
    g_power_manager_state.configs_number          = 0U;
    g_power_manager_state.pp_static_callbacks     = NULL;
    g_power_manager_state.static_callbacks_number = 0U;

    return power_do_deinit();
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_get_last_mode
 * Description   : This function returns power mode set as the last one.
 *
 * This function returns index of power mode which was set using power_set_mode()
 * as the last one. If the power mode was entered although some of the registered
 * call-back denied the mode change or if any of the call-backs invoked after
 * the entering/restoring run mode failed then the return code of this function
 * has STATUS_ERROR value.
 *
 * Implements    : power_get_last_mode_activity
 *END**************************************************************************/
status_t
power_get_last_mode(uint8_t *p_power_mode_index_ptr)
{
    status_t return_code; /* Function return */

    /* Pass index of user-defined configuration structure of currently running power mode */
    *p_power_mode_index_ptr = g_power_manager_state.current_config;

    /* Return whether all call-backs executed without error */
    if (g_power_manager_state.err_callback_index == g_power_manager_state.static_callbacks_number)
    {
        return_code = STATUS_SUCCESS;
    }
    else
    {
        return_code = STATUS_ERROR;
    }

    return return_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_get_last_mode_config
 * Description   : This function returns user configuration structure of power
 *          mode set as the last one.
 *
 * This function returns reference to configuration structure which was set using
 * power_set_mode() as the last one. If the current power mode was entered although some
 * of the registered call-back denied the mode change or if any of the call-backs
 * invoked after the entering/restoring run mode failed then the return code of
 * this function has STATUS_ERROR value.
 *
 * Implements    : power_get_last_mode_config_activity
 *END**************************************************************************/
status_t
power_get_last_mode_config(power_manager_user_config_t **pp_power_mode_ptr)
{
    status_t return_code; /* Function return */
    /* Pass reference to user-defined configuration structure of currently running power mode */
    *pp_power_mode_ptr = (*g_power_manager_state.pp_configs)[g_power_manager_state.current_config];

    /* Return whether all call-backs executed without error */
    if (g_power_manager_state.err_callback_index == g_power_manager_state.static_callbacks_number)
    {
        return_code = STATUS_SUCCESS;
    }
    else
    {
        return_code = STATUS_ERROR;
    }

    return return_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_get_error_callback_index
 * Description   : Returns the last failed notification callback.
 *
 * This function returns index of the last call-back that failed during the power mode switch while
 * the last power_set_mode() was called. If the last power_set_mode() call ended successfully
 * value equal to callbacks number is returned. Returned value represents index in the array of
 * static call-backs.
 *
 * Implements    : power_get_error_callback_index_activity
 *END**************************************************************************/
uint8_t
power_get_error_callback_index(void)
{
    return g_power_manager_state.err_callback_index;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_get_error_callback
 * Description   : Get the callback which returns error in last mode switch.
 *
 * Implements    : power_get_error_callback_activity
 *END**************************************************************************/
power_manager_callback_user_config_t *
power_get_error_callback(void)
{
    /* If all callbacks return success. */
    return (g_power_manager_state.err_callback_index >=
            g_power_manager_state.static_callbacks_number)
               ? NULL
               : (*g_power_manager_state
                       .pp_static_callbacks)[g_power_manager_state.err_callback_index];
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_set_mode
 * Description   : Configures the power mode.
 *
 * Implements    : power_set_mode_activity
 *END**************************************************************************/
status_t
power_set_mode(uint8_t power_mode_index, power_manager_policy_t policy)
{
    power_manager_user_config_t
        *p_config_ptr;    /* Local pointer to the requested user-defined power mode configuration */
    status_t return_code; /* Function return */
    status_t error_code;
    bool     b_successful_switch;          /* Power mode switch is successful or not */
    uint8_t  current_static_callback = 0U; /* Index to array of statically registered call-backs */
    power_manager_notify_struct_t notify_struct; /* Callback notification structure */

    /* Driver is already initialized. */
    DEV_ASSERT(g_power_manager_state.pp_configs != NULL);
    DEV_ASSERT(g_power_manager_state.configs_number != 0U);

    /* Power mode index is valid. */
    DEV_ASSERT(power_mode_index < g_power_manager_state.configs_number);

    /* Initialization of local pointer to the requested user-defined power mode configuration */
    p_config_ptr = (*g_power_manager_state.pp_configs)[power_mode_index];

    /* Reference to the requested user-defined power mode configuration is valid. */
    DEV_ASSERT(p_config_ptr != NULL);

    /* Default value of handle of last call-back that returned error */
    g_power_manager_state.err_callback_index = g_power_manager_state.static_callbacks_number;

    /* Set the transaction policy in the notification structure */
    notify_struct.policy = policy;

    /* Set the target power mode configuration in the notification structure */
    notify_struct.target_power_config_index = power_mode_index;
    notify_struct.p_target_power_config     = p_config_ptr;

    /* Notify those which asked to be called before the power mode change */
    notify_struct.pwr_notify_type = POWER_MANAGER_NOTIFY_BEFORE;
    return_code = power_callbacks_management(&notify_struct, &current_static_callback, policy);

    /* Power mode switch */
    /* In case that any call-back returned error code and  policy doesn't force
     * the mode switch go to after switch call-backs */
    if ((policy == POWER_MANAGER_POLICY_FORCIBLE) || (return_code == STATUS_SUCCESS))
    {
        return_code         = power_do_set_mode(p_config_ptr);
        b_successful_switch = (STATUS_SUCCESS == return_code);
    }
    else
    { /* Unsuccessful switch */
        b_successful_switch = false;
    }

    if (b_successful_switch)
    { /* End of successful switch */
        /* Update current configuration index */
        g_power_manager_state.current_config = power_mode_index;

        /* Notify those which asked to be called after the power mode change */
        notify_struct.pwr_notify_type = POWER_MANAGER_NOTIFY_AFTER;
        return_code                   = power_callbacks_management(
            &notify_struct, &current_static_callback, POWER_MANAGER_POLICY_FORCIBLE);
    }
    else
    { /* End of unsuccessful switch */
        /* Notify those which have been called before the power mode change */
        notify_struct.pwr_notify_type = POWER_MANAGER_NOTIFY_RECOVER;
        error_code                    = power_callbacks_management(
            &notify_struct, &current_static_callback, POWER_MANAGER_POLICY_FORCIBLE);
        (void)(error_code);
    }

    return return_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_get_default_config
 * Description   : Initializes the power_manager configuration structure.
 * This function returns a pointer of the power_manager configuration structure.
 * All structure members have default value when CPU is default power mode.
 *
 * Implements    : power_get_default_config_activity
 *END**************************************************************************/
void
power_get_default_config(power_manager_user_config_t *const p_config)
{
    power_do_get_default_config(p_config);
}

/*******************************************************************************
 * Static functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : power_switch_to_running_power_mode
 * Description   : Internal function used by power_set_mode function to switch to
 *                  a running powermode
 * - p_config_ptr : pointer to the requested user-defined power mode configuration.
 * System clock source must be SIRC or SOSC in Run mode before transition VLPR mode.
 * Update initialization or default clock source in run mode when came back
 * from very low power run mode.
 *
 *END**************************************************************************/
static status_t
power_switch_to_running_power_mode(const power_manager_user_config_t *const p_config_ptr)
{
    smc_power_mode_config_t mode_config; /* SMC hardware layer configuration structure */
    power_mode_stat_t       current_mode = smc_get_power_mode_status(SMC);
    status_t                return_code  = STATUS_SUCCESS;

    /* Configure the running mode */
    switch (p_config_ptr->power_mode)
    {
        /* Run mode */
        case POWER_MANAGER_RUN:
        {
            if (current_mode != STAT_RUN)
            {
                mode_config.power_mode_name = POWER_MANAGER_RUN;
                /* Switch the mode */
                return_code = smc_set_power_mode(SMC, &mode_config);
            }
            if ((return_code == STATUS_SUCCESS) && sb_change_clk_vlp)
            {
                /* Enable all clock source */

                /* Update initialize clock configuration */
                return_code = power_update_Init_clk(&s_sys_clk_config);
                if (return_code == STATUS_SUCCESS)
                {
                    sb_change_clk_vlp = false;
                }
            }
        }
        break;
        /* Very low power run mode */
        case POWER_MANAGER_VLPR:
        {
            if (current_mode != STAT_VLPR)
            {
                /* Very low power run mode can be entered only from Run mode */
                if (smc_get_power_mode_status(SMC) != STAT_RUN)
                {
                    mode_config.power_mode_name = POWER_MANAGER_RUN;
                    /* Switch the mode */
                    return_code = smc_set_power_mode(SMC, &mode_config);
                }
                if (STATUS_SUCCESS == return_code)
                {
                    if (!sb_change_clk_vlp)
                    {
                        clock_get_system_clock_source(&s_sys_clk_config);
                    }
                    return_code = power_switch_vlpr_clk(&s_sys_clk_config);
                    if (STATUS_SUCCESS == return_code)
                    {
                        sb_change_clk_vlp           = true;
                        mode_config.power_mode_name = POWER_MANAGER_VLPR;
                        /* Disable all clock source except SIRC */

                        /* Switch the mode */
                        return_code = smc_set_power_mode(SMC, &mode_config);
                    }
                }
            }
            else
            {
                return_code = STATUS_SUCCESS;
            }
        }
        break;
        /* Wait mode */
        default:
        { /* invalid power mode */
            return_code                 = STATUS_UNSUPPORTED;
            mode_config.power_mode_name = POWER_MANAGER_MAX;
        }
        break;
    }

    return return_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_switch_to_sleeping_power_mode
 * Description   : Internal function used by power_set_mode function to switch to
 *                  a sleeping power mode
 * - p_config_ptr : pointer to the requested user-defined power mode configuration
 *
 *END**************************************************************************/
static status_t
power_switch_to_sleeping_power_mode(const power_manager_user_config_t *const p_config_ptr)
{
    smc_power_mode_config_t mode_config; /* SMC hardware layer configuration structure */
    status_t                return_code   = STATUS_SUCCESS;                 /* return value */
    power_mode_stat_t       pwr_mode_stat = smc_get_power_mode_status(SMC); /* power mode stat */

    /* Configure the hardware layer */
    switch (p_config_ptr->power_mode)
    {
        /* Stop modes */
        case POWER_MANAGER_STOP1:
        /* fall-through */
        case POWER_MANAGER_STOP2:
        { /* Stop1 and Stop2 mode can be entered only from Run mode */
            if (pwr_mode_stat != STAT_RUN)
            {
                mode_config.power_mode_name = POWER_MANAGER_RUN;
                /* Switch the mode */
                return_code = smc_set_power_mode(SMC, &mode_config);
            }
            mode_config.power_mode_name = p_config_ptr->power_mode;
            /* Set the stop option value */
            if (POWER_MANAGER_STOP1 == p_config_ptr->power_mode)
            {
                mode_config.stop_option_value = SMC_STOP1;
            }
            else
            {
                mode_config.stop_option_value = SMC_STOP2;
            }
        }
        break;
        /* Very low power stop mode */
        case POWER_MANAGER_VLPS:
        { /* Very low power stop mode can be entered only from Run mode or Very low power run
           * mode*/
            if ((pwr_mode_stat != STAT_RUN) && (pwr_mode_stat != STAT_VLPR))
            {
                mode_config.power_mode_name = POWER_MANAGER_RUN;
                return_code                 = smc_set_power_mode(SMC, &mode_config);
            }

            if (return_code == STATUS_SUCCESS)
            {
                if (power_get_current_mode() == POWER_MANAGER_RUN)
                {
                    /* Get current source clock */
                    if (!sb_change_clk_vlp)
                    {
                        clock_get_system_clock_source(&s_sys_clk_config);
                    }
                    return_code = power_switch_vlpr_clk(&s_sys_clk_config);
                    if (STATUS_SUCCESS == return_code)
                    {
                        sb_change_clk_vlp           = true;
                        mode_config.power_mode_name = POWER_MANAGER_VLPS;
                        /* Disable all clock source except SIRC */
                    }
                }
            }
            mode_config.power_mode_name = POWER_MANAGER_VLPS;
        }
        break;
        default:
        { /* invalid power mode */
            return_code                 = STATUS_UNSUPPORTED;
            mode_config.power_mode_name = POWER_MANAGER_MAX;
        }
        break;
    }

    if (STATUS_SUCCESS == return_code)
    {
        /* Configure ARM core what to do after interrupt invoked in (deep) sleep state */
        if (p_config_ptr->b_sleep_on_exit_value)
        {
            /* Go back to (deep) sleep state on ISR exit */
            SCB->SCR |= SCB_SCR_SLEEPONEXIT_MASK;
        }
        else
        {
            /* Do not re-enter (deep) sleep state on ISR exit */
            SCB->SCR &= ~(SCB_SCR_SLEEPONEXIT_MASK);
        }

        /* Switch the mode */
        if (smc_set_power_mode(SMC, &mode_config) != STATUS_SUCCESS)
        {
            return_code = STATUS_MCU_TRANSITION_FAILED;
        }
    }

    return return_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_switch_vlpr_clk
 * Description   : This function will change system clock in run mode before MCU
 *                  enter very low power run mode.
 *
 *END**************************************************************************/
static status_t
power_switch_vlpr_clk(const sys_clk_config_t *const p_sys_clock)
{
    status_t         ret_code = STATUS_SUCCESS;
    sys_clk_config_t sys_clk_vlpr_config;
    clock_names_t    current_system_clock_source = p_sys_clock->src;

    if (current_system_clock_source != SIRC_CLK)
    {
        /* Set SIRC the system clock source */
        sys_clk_vlpr_config.src = SIRC_CLK;
        sys_clk_vlpr_config.dividers[0U] =
            (uint16_t)(scg_get_vccr_divcore(SCG) + 1U); /* Core clock divider, do not divide */
        sys_clk_vlpr_config.dividers[1U] =
            (uint16_t)(scg_get_vccr_divslow(SCG) + 1U); /* Bus clock divider, do not divide */

        ret_code = clock_set_system_clock(NULL, &sys_clk_vlpr_config);
    }

    return ret_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_update_Init_clk
 * Description   : This function will  update initialization or default clock
 *                  source of run mode when MCU come back run mode.
 *
 *END**************************************************************************/
static status_t
power_update_Init_clk(const sys_clk_config_t *const p_sys_clk)
{
    status_t ret_code = STATUS_SUCCESS;

    ret_code = clock_set_system_clock(NULL, p_sys_clk);

    return ret_code;
}

static void
power_do_get_default_config(power_manager_user_config_t *const p_default_config)
{
    p_default_config->power_mode            = POWER_MANAGER_RUN; /**< Power manager mode  */
    p_default_config->b_sleep_on_exit_value = false;             /**< Sleep on exit value */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : power_callbacks_management
 * Description   : Internal function used by power_set_mode function for callback management
 *
 *END***************************************************************************/
static status_t
power_callbacks_management(power_manager_notify_struct_t *p_notify_struct,
                           uint8_t                       *p_current_static_callback,
                           power_manager_policy_t         policy)
{
    uint8_t  callbacks_number;             /* The total number of callbacks */
    status_t return_code = STATUS_SUCCESS; /* Function return */
    status_t error_code  = STATUS_SUCCESS; /* Error code to be returned(error case) */
    status_t callback_status_code;         /* Status code returned by callback function */
    power_manager_callback_type_t callback_type_filter; /* Callback types to be excluded */

    switch (p_notify_struct->pwr_notify_type)
    {
        /* notify before */
        case POWER_MANAGER_NOTIFY_BEFORE:
        {
            callbacks_number     = g_power_manager_state.static_callbacks_number;
            callback_type_filter = POWER_MANAGER_CALLBACK_AFTER;
            error_code           = STATUS_MCU_NOTIFY_BEFORE_ERROR;
        }
        break;
        /* notify after */
        case POWER_MANAGER_NOTIFY_AFTER:
        {
            callbacks_number     = g_power_manager_state.static_callbacks_number;
            callback_type_filter = POWER_MANAGER_CALLBACK_BEFORE;
            error_code           = STATUS_MCU_NOTIFY_AFTER_ERROR;
        }
        break;
        /* notify recover */
        case POWER_MANAGER_NOTIFY_RECOVER:
        {
            callbacks_number     = g_power_manager_state.static_callbacks_number;
            callback_type_filter = POWER_MANAGER_CALLBACK_AFTER;
            error_code           = STATUS_MCU_NOTIFY_BEFORE_ERROR;
        }
        break;
        default:
        { /* invalid value for pwr_notify_type */
            DEV_ASSERT(false);
            callbacks_number     = 0U;
            callback_type_filter = POWER_MANAGER_CALLBACK_BEFORE;
        }
        break;
    }

    /* From all statically registered call-backs... */
    for ((*p_current_static_callback) = 0U; (*p_current_static_callback) < callbacks_number;
         (*p_current_static_callback)++)
    {
        /* Pointer to callback configuration */
        const power_manager_callback_user_config_t *const p_callback_config =
            ((*g_power_manager_state.pp_static_callbacks)[*p_current_static_callback]);

        /* Check pointer to static callback configuration */
        if ((p_callback_config != NULL) &&
            (callback_type_filter != p_callback_config->callback_type))
        {
            /* In case that call-back returned error code mark it, store the call-back handle and
             * eventually cancel the mode switch */
            callback_status_code = p_callback_config->callback_function(
                p_notify_struct, p_callback_config->p_callback_data);
            if (STATUS_SUCCESS != callback_status_code)
            {
                return_code                              = error_code;
                g_power_manager_state.err_callback_index = *p_current_static_callback;
                /* If not forcing power mode switch, call all already notified call-backs to revert
                 * their state as the mode change is canceled */
                if (policy != POWER_MANAGER_POLICY_FORCIBLE)
                {
                    break;
                }
            }
        }
    }

    return return_code;
}

/*** end of file ***/
