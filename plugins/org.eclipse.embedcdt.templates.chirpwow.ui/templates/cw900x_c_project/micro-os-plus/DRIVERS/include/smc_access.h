/**
 * @file smc_access.h
 * @brief Static inline function for the SMC module.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef SMC_ACCESS_H
#define SMC_ACCESS_H

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

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/
/**
 * @brief Gets the current running power mode.
 *
 * This function  returns the current running power mode.
 *
 * @param[in] p_base  Base address for current SMC instance.
 * @return stat  Current power mode stat
 */
static inline uint32_t
smc_get_current_running_mode(const SMC_t *const p_base)
{
    return (p_base->PMSTAT & SMC_PMSTAT_PMSTAT_MASK) >> SMC_PMSTAT_PMSTAT_SHIFT;
}

/**
 * @brief Configures the the RUN mode control setting.
 *
 * This function sets the run mode settings, for example, normal run mode,
 * very lower power run mode, etc. See the smc_run_mode_t for supported run mode.
 *
 * @param[in] p_base  Base address for current SMC instance.
 * @param[in] run_mode   Run mode setting defined in smc_run_mode_t
 */
static inline void
smc_set_run_mode_control(SMC_t *const p_base, const smc_run_mode_t run_mode)
{
    uint32_t reg_val = p_base->PMCTRL;
    reg_val &= ~(SMC_PMCTRL_RUNM_MASK);
    reg_val |= SMC_PMCTRL_RUNM(run_mode);
    p_base->PMCTRL = reg_val;
}

/**
 * @brief Configures  the STOP mode control setting.
 *
 * This function sets the stop mode settings, for example, normal stop mode,
 * very lower power stop mode, etc. See the smc_stop_mode_t for supported stop
 * mode on the chip family and the reference manual for details about the stop mode.
 *
 * @param[in] p_base  Base address for current SMC instance.
 * @param[in] stop_mode  Stop mode defined in smc_stop_mode_t
 */
static inline void
smc_set_stop_mode_control(SMC_t *const p_base, const smc_stop_mode_t stop_mode)
{
    uint32_t reg_val = p_base->PMCTRL;
    reg_val &= ~(SMC_PMCTRL_STOPM_MASK);
    reg_val |= SMC_PMCTRL_STOPM(stop_mode);
    p_base->PMCTRL = reg_val;
}

/**
 * @brief Configures the STOPO (Stop Option).
 *
 * It controls the type of the stop operation when STOPM=STOP. When entering Stop mode
 * from RUN mode, the PMC, SCG and flash remain fully powered, allowing the device to
 * wakeup almost instantaneously at the expense of higher power consumption. In STOP2,
 * only system clocks are gated allowing peripherals running on bus clock to
 * remain fully functional. In STOP1, both system and bus clocks are gated.
 *
 * @param[in] p_base  Base address for current SMC instance.
 * @param[in] option STOPO option setting defined in smc_stop_option_t
 */
static inline void
smc_set_stop_option(SMC_t *const p_base, const smc_stop_option_t option)
{
    uint32_t reg_val = p_base->STOPCTRL;
    reg_val &= ~(SMC_STOPCTRL_STOPO_MASK);
    reg_val |= SMC_STOPCTRL_STOPO(option);
    p_base->STOPCTRL = reg_val;
}

/**
 * @brief Gets the current power mode stat.
 *
 * This function returns the current power mode stat. Once application
 * switches the power mode, it should always check the stat to check whether it
 * runs into the specified mode or not. An application should check
 * this mode before switching to a different mode. The system requires that
 * only certain modes can switch to other specific modes. See the reference manual
 * for details and the power_mode_stat for information about the power stat.
 *
 * @param[in] p_base  Base address for current SMC instance.
 * @return stat  Current power mode stat
 */
static inline power_mode_stat_t
smc_get_power_mode_status(const SMC_t *const p_base)
{
    power_mode_stat_t ret_value;
    uint32_t          reg_val = p_base->PMSTAT;
    reg_val                   = (reg_val & SMC_PMSTAT_PMSTAT_MASK) >> SMC_PMSTAT_PMSTAT_SHIFT;

    switch (reg_val)
    {
        case 1UL:
        {
            ret_value = STAT_RUN;
        }
        break;
        case 2UL:
        {
            ret_value = STAT_STOP;
        }
        break;
        case 4UL:
        {
            ret_value = STAT_VLPR;
        }
        break;
        case 16UL:
        {
            ret_value = STAT_VLPS;
        }
        break;
        case 255UL:
        default:
        {
            ret_value = STAT_INVALID;
        }
        break;
    }

    return ret_value;
}

#ifdef __cplusplus
}
#endif

#endif /* SMC_ACCESS_H */

/*** end of file ***/
