/**
 * @file smc_driver.c
 * @brief This file provides access to the smc module.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "smc_driver.h"

/*******************************************************************************
 * Private macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/** Timeout used for waiting to set new mode */
#define SMC_TIMEOUT 1000U

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
static bool smc_wait_for_stat_change(const SMC_t *const      p_base,
                                     const power_mode_stat_t mode,
                                     const uint32_t          timeout);

/*******************************************************************************
 * Global functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : smc_set_power_mode
 * Description   : Configure the power mode
 * This function will configure the power mode control for any run, stop and
 * stop submode if needed. It will also configure the power options for specific
 * power mode. Application should follow the proper procedure to configure and
 * switch power mode between the different run and stop mode. Refer to reference
 * manual for the proper procedure and supported power mode that can be configured
 * and switch between each other. Refer to smc_power_mode_config_t for required
 * parameters to configure the power mode and the supported options.
 *
 *END**************************************************************************/
status_t
smc_set_power_mode(SMC_t *const p_base, const smc_power_mode_config_t *const p_power_mode_config)
{
    status_t              ret_code;
    smc_stop_mode_t       stop_mode;
    power_manager_modes_t power_mode_name = p_power_mode_config->power_mode_name;
    /* Branch based on power mode name*/
    switch (power_mode_name)
    {
        case POWER_MANAGER_RUN:
        {
            /* Set to RUN mode. */
            smc_set_run_mode_control(p_base, SMC_RUN);
            /* Wait for stat change */
            if (!smc_wait_for_stat_change(p_base, STAT_RUN, SMC_TIMEOUT))
            {
                /* Timeout for power mode change expired. */
                ret_code = STATUS_MCU_TRANSITION_FAILED;
            }
            else
            {
                ret_code = STATUS_SUCCESS;
            }
        }
        break;
        case POWER_MANAGER_VLPR:
        {
            /* Set power mode to VLPR*/
            smc_set_run_mode_control(p_base, SMC_VLPR);
            /* Wait for stat change */
            if (!smc_wait_for_stat_change(p_base, STAT_VLPR, SMC_TIMEOUT))
            {
                /* Timeout for power mode change expired. */
                ret_code = STATUS_MCU_TRANSITION_FAILED;
            }
            else
            {
                ret_code = STATUS_SUCCESS;
            }
        }
        break;
        case POWER_MANAGER_STOP1:
        /* Fall-through */
        case POWER_MANAGER_STOP2:
        /* Fall-through */
        case POWER_MANAGER_VLPS:
        {
            if ((power_mode_name == POWER_MANAGER_STOP1) ||
                (power_mode_name == POWER_MANAGER_STOP2))
            {
                stop_mode = SMC_STOP;
                smc_set_stop_option(p_base, p_power_mode_config->stop_option_value);
            }
            else
            {
                stop_mode = SMC_VLPS;
            }

            /* Set power mode to specified STOP mode*/
            smc_set_stop_mode_control(p_base, stop_mode);

            /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP)*/
            SCB->SCR |= SCB_SCR_SLEEPDEEP_MASK;

            /* Cpu is going into deep sleep state */
            STANDBY();

            ret_code = STATUS_SUCCESS;
        }
        break;
        default:
        {
            ret_code = STATUS_UNSUPPORTED;
        }
        break;
    }

    return ret_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : smc_set_protection_mode
 * Description   : Configure all power mode protection settings
 * This register can only write once after power reset. So either use this function
 * or use the individual set function if you only have single option to set.
 *
 *END**************************************************************************/
void
smc_set_protection_mode(SMC_t *const                                    p_base,
                        const smc_power_mode_protection_config_t *const p_protect_config)
{
    /* Initialize the setting */
    uint32_t reg_val = (uint32_t)p_base->PMPROT;

    reg_val &= (uint32_t)(~(SMC_PMPROT_AVLP_MASK));

    /* Check configurations for each mode and combine the setting together */
    if (p_protect_config->b_vlp_prot)
    {
        reg_val |= SMC_PMPROT_AVLP(1);
    }

    /* Write once into PMPROT register*/
    p_base->PMPROT = reg_val;
}

/*******************************************************************************
 * Static functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 * Function Name : smc_wait_for_stat_change
 * Description   : Internal function used by smc_set_power_mode function
 * to wait until the state is changed or timeout expires
 *
 * return power mode status change
 *                - true: power mode has been changed successfully
 *                - false: timeout expired, power mode has not been changed
 *END**************************************************************************/
static bool
smc_wait_for_stat_change(const SMC_t *const      p_base,
                         const power_mode_stat_t mode,
                         const uint32_t          timeout)
{
    uint32_t idx;
    bool     ret_value;

    for (idx = 0U; idx < timeout; idx++)
    {
        if (mode == smc_get_power_mode_status(p_base))
        {
            /* Power mode has been changed successfully */
            break;
        }
    }

    /* If i greater or equal to timeout, then timeout expired(the power mode has not been changed)*/
    ret_value = (idx < timeout);

    return ret_value;
}

/*** end of file ***/
