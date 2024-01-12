/**
 * @file sim_access.h
 * @brief Static inline function for the SIM module.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef SIM_ACCESS_H
#define SIM_ACCESS_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include "device_registers.h"

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/
/**
 * @brief Get the clock selection of RTCCLKSEL.
 *
 * This function gets the clock selection of RTCCLKSEL.
 *
 * @param[in] p_base Base address for current SIM instance.
 * @return Current selection.
 */
static inline uint32_t
sim_get_rtc_clk_src(const SIM_t *p_base)
{
    return ((p_base->LPOCLKS & SIM_LPOCLKS_RTCCLKSEL_MASK) >> SIM_LPOCLKS_RTCCLKSEL_SHIFT);
}

/**
 * @brief Set the clock selection of LPOCLKSEL.
 *
 * This function sets the clock selection of LPOCLKSEL.
 *
 * @param[in] p_base Base address for current SIM instance.
 * @param[in] setting The value to set.
 * @note This function ignores initialize member
 */
static inline void
sim_set_lpo_clks(SIM_t   *p_base,
                 bool     b_enable_lpo1k,
                 bool     b_enable_lpo32k,
                 uint32_t source_lpo_clk,
                 uint32_t source_rtc_clk)
{
    uint32_t reg_val = p_base->LPOCLKS;

    reg_val &= ~(SIM_LPOCLKS_LPO1KCLKEN_MASK | SIM_LPOCLKS_LPO32KCLKEN_MASK |
                 SIM_LPOCLKS_LPOCLKSEL_MASK | SIM_LPOCLKS_RTCCLKSEL_MASK);

    reg_val |= SIM_LPOCLKS_LPO1KCLKEN(b_enable_lpo1k ? 1UL : 0UL);
    reg_val |= SIM_LPOCLKS_LPO32KCLKEN(b_enable_lpo32k ? 1UL : 0UL);
    reg_val |= SIM_LPOCLKS_LPOCLKSEL(source_lpo_clk);
    reg_val |= SIM_LPOCLKS_RTCCLKSEL(source_rtc_clk);

    /* Write value to register. */
    p_base->LPOCLKS = reg_val;
}

/*FUNCTION**********************************************************************
 * Function Name : sim_get_ftm0_ext_clk_pin_mode
 * Description   : Get FlexTimer 0 external clock pin select setting
 * This function will get FlexTimer 0 external clock pin select setting.
 *END**************************************************************************/
static inline uint32_t
sim_get_ftm0_ext_clk_pin_mode(const SIM_t *p_base)
{
    return (p_base->FTMOPT0 & SIM_FTMOPT0_FTM0CLKSEL_MASK) >> SIM_FTMOPT0_FTM0CLKSEL_SHIFT;
}

/*FUNCTION**********************************************************************
 * Function Name : sim_get_ftm1_ext_clk_pin_mode
 * Description   : Get FlexTimer 1 external clock pin select setting
 * This function will get FlexTimer 1 external clock pin select setting.
 *END**************************************************************************/
static inline uint32_t
sim_get_ftm1_ext_clk_pin_mode(const SIM_t *p_base)
{
    return (p_base->FTMOPT0 & SIM_FTMOPT0_FTM1CLKSEL_MASK) >> SIM_FTMOPT0_FTM1CLKSEL_SHIFT;
}

/*FUNCTION**********************************************************************
 * Function Name : sim_get_ftm2_ext_clk_pin_mode
 * Description   : Get FlexTimer 2 external clock pin select setting
 * This function will get FlexTimer 2 external clock pin select setting.
 *END**************************************************************************/
static inline uint32_t
sim_get_ftm2_ext_clk_pin_mode(const SIM_t *p_base)
{
    return (p_base->FTMOPT0 & SIM_FTMOPT0_FTM2CLKSEL_MASK) >> SIM_FTMOPT0_FTM2CLKSEL_SHIFT;
}

/*FUNCTION**********************************************************************
 * Function Name : sim_get_ftm3_ext_clk_pin_mode
 * Description   : Get FlexTimer 3 external clock pin select setting
 * This function will get FlexTimer 3 external clock pin select setting.
 *END**************************************************************************/
static inline uint32_t
sim_get_ftm3_ext_clk_pin_mode(const SIM_t *p_base)
{
    return (p_base->FTMOPT0 & SIM_FTMOPT0_FTM3CLKSEL_MASK) >> SIM_FTMOPT0_FTM3CLKSEL_SHIFT;
}

/*FUNCTION*********************************************************************
 * Function Name : sim_get_lpo32k_status
 * Description   : Get SIM LPO 32KHz clock gating status (LPO_32K_CLOCK).
 *END*************************************************************************/
static inline bool
sim_get_lpo32k_status(const SIM_t *p_base)
{
    return (((p_base->LPOCLKS & SIM_LPOCLKS_LPO32KCLKEN_MASK) >> SIM_LPOCLKS_LPO32KCLKEN_SHIFT) !=
            0U)
               ? true
               : false;
}

/*FUNCTION*********************************************************************
 * Function Name : sim_get_lpo1k_status
 * Description   : Get SIM LPO 1KHz clock gating status (LPO_1K_CLOCK).
 *END*************************************************************************/
static inline bool
sim_get_lpo1k_status(const SIM_t *p_base)
{
    return (((p_base->LPOCLKS & SIM_LPOCLKS_LPO1KCLKEN_MASK) >> SIM_LPOCLKS_LPO1KCLKEN_SHIFT) != 0U)
               ? true
               : false;
}

/**
 * @brief Gets the LPO selector value
 *
 * This function gets the LPO selector value
 *
 * @param[in] p_base Base address for current SIM instance.
 * @return clockout status
 */
static inline uint32_t
sim_get_lpo_clk_selector_value(const SIM_t *p_base)
{
    return ((p_base->LPOCLKS & SIM_LPOCLKS_LPOCLKSEL_MASK) >> SIM_LPOCLKS_LPOCLKSEL_SHIFT);
}

/**
 * @brief Gets the clock out status
 *
 * This function gets the clock out status (enabled or disabled)
 *
 * @param[in] p_base Base address for current SIM instance.
 * @return clockout status
 */
static inline bool
sim_get_clockout_status(const SIM_t *p_base)
{
    return (((p_base->CHIPCTL & SIM_CHIPCTL_CLKOUTEN_MASK) >> SIM_CHIPCTL_CLKOUTEN_SHIFT) == 0U)
               ? false
               : true;
}

/**
 * @brief Gets the clock out divider value
 *
 * This function gets the clock out divider value
 *
 * @param[in] p_base Base address for current SIM instance.
 * @return clockout divider
 */
static inline uint32_t
sim_get_clockout_divider_value(const SIM_t *p_base)
{
    return ((p_base->CHIPCTL & SIM_CHIPCTL_CLKOUTDIV_MASK) >> SIM_CHIPCTL_CLKOUTDIV_SHIFT);
}

/**
 * @brief Gets the clock out source
 *
 * This function gets the clock out source
 *
 * @param[in] p_base Base address for current SIM instance.
 * @return clockout source
 */
static inline uint32_t
sim_get_clockout_selector_value(const SIM_t *p_base)
{
    return ((p_base->CHIPCTL & SIM_CHIPCTL_CLKOUTSEL_MASK) >> SIM_CHIPCTL_CLKOUTSEL_SHIFT);
}

/**
 * @brief Sets ext pin clock source for FTM
 *
 * @param[in] p_base        sim base pointer
 * @param[in] instance    ftm instance
 * @param[in] source      clock source
 */
static inline void
sim_set_ext_pin_source_ftm(SIM_t *p_base, uint8_t instance, uint32_t source)
{
    uint32_t reg_val = p_base->FTMOPT0;

    switch (instance)
    {
        case 0U:
        {
            reg_val &= ~SIM_FTMOPT0_FTM0CLKSEL_MASK;
            reg_val |= SIM_FTMOPT0_FTM0CLKSEL(source);
        }
        break;
        case 1U:
        {
            reg_val &= ~SIM_FTMOPT0_FTM1CLKSEL_MASK;
            reg_val |= SIM_FTMOPT0_FTM1CLKSEL(source);
        }
        break;
#if FTM_INSTANCE_COUNT > 2U
        case 2U:
        {
            reg_val &= ~SIM_FTMOPT0_FTM2CLKSEL_MASK;
            reg_val |= SIM_FTMOPT0_FTM2CLKSEL(source);
        }
        break;
#endif
#if FTM_INSTANCE_COUNT > 3U
        case 3U:
        {
            reg_val &= ~SIM_FTMOPT0_FTM3CLKSEL_MASK;
            reg_val |= SIM_FTMOPT0_FTM3CLKSEL(source);
        }
        break;
#endif
        default:
        { /* Do nothing */
        }
        break;
    }

    p_base->FTMOPT0 = reg_val;
}

/**
 * @brief Sets clockout
 *
 * @param[in] p_base        sim base pointer
 * @param[in] source      clock source
 * @param[in] divider     clock divider
 */
static inline void
sim_set_clockout(SIM_t *p_base, bool b_enable, uint32_t source, uint32_t divider)
{
    uint32_t reg_val;

    /* CLKOUTEN should be first cleared and then execute sequence */
    p_base->CHIPCTL &= ~SIM_CHIPCTL_CLKOUTEN_MASK;

    reg_val = p_base->CHIPCTL;
    reg_val &=
        ~(SIM_CHIPCTL_CLKOUTEN_MASK | SIM_CHIPCTL_CLKOUTDIV_MASK | SIM_CHIPCTL_CLKOUTSEL_MASK);

    reg_val |= SIM_CHIPCTL_CLKOUTEN(b_enable ? 1UL : 0UL);
    reg_val |= SIM_CHIPCTL_CLKOUTSEL(source);
    reg_val |= SIM_CHIPCTL_CLKOUTDIV(divider);

    p_base->CHIPCTL = reg_val;
}

#ifdef __cplusplus
}
#endif

#endif /* SIM_ACCESS_H */

/*** end of file ***/
