/**
 * @file scg_access.h
 * @brief Static inline function for the SCG module.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef SCG_ACCESS_H
#define SCG_ACCESS_H

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
 * @brief Get SCG ClockOut source select
 *
 * This function gets the SCG clockOut source
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @return ClockOut source.
 */
static inline uint32_t
scg_get_clockout_source_sel(const SCG_t *p_base)
{
    return (p_base->CLKOUTCNFG & SCG_CLKOUTCNFG_CLKOUTSEL_MASK) >> SCG_CLKOUTCNFG_CLKOUTSEL_SHIFT;
}

/**
 * @brief Gets SCG current system clock source
 *
 * This function gets the current system clock source.
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @return Current system clock source.
 */
static inline uint32_t
scg_get_system_clk_source(const SCG_t *p_base)
{
    return ((p_base->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT);
}

/**
 * @brief Gets SCG Current Core Clock Divide Ratio
 *
 * This function gets the Current Core Clock Divide Ratio.
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @return Current Core Clock Divide Ratio.
 */
static inline uint32_t
scg_get_core_clk_divider_ratio(const SCG_t *p_base)
{
    return ((p_base->CSR & SCG_CSR_DIVCORE_MASK) >> SCG_CSR_DIVCORE_SHIFT);
}

/**
 * @brief Gets SCG Current Slow Clock Divide Ratio
 *
 * This function gets the Current Slow Clock Divide Ratio.
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @return Current Slow Clock Divide Ratio.
 */
static inline uint32_t
scg_get_slow_clk_divider_ratio(const SCG_t *p_base)
{
    return ((p_base->CSR & SCG_CSR_DIVSLOW_MASK) >> SCG_CSR_DIVSLOW_SHIFT);
}

/**
 * @brief Sets SCG run system clock
 *
 * This function sets the RUN clock control (system clock source, core and slow dividers).
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @param[in] p_base System clock configuration address.
 */
static inline void
scg_set_run_clk_control(SCG_t   *p_base,
                        uint32_t source,
                        uint32_t core_divider,
                        uint32_t slow_divider)
{
    uint32_t reg_val =
        (uint32_t)(((source << SCG_RCCR_SCS_SHIFT) & SCG_RCCR_SCS_MASK) |
                   ((core_divider << SCG_RCCR_DIVCORE_SHIFT) & SCG_RCCR_DIVCORE_MASK) |
                   ((slow_divider << SCG_RCCR_DIVSLOW_SHIFT) & SCG_RCCR_DIVSLOW_MASK));
    p_base->RCCR = reg_val;
}

/**
 * @brief Sets SCG vlpr system clock
 *
 * This function sets the VLPR clock control (system clock source, core and slow dividers).
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @param[in] p_base System clock configuration address.
 */
static inline void
scg_set_vlpr_clk_control(SCG_t   *p_base,
                         uint32_t source,
                         uint32_t core_divider,
                         uint32_t slow_divider)
{
    uint32_t reg_val =
        (uint32_t)(((source << SCG_VCCR_SCS_SHIFT) & SCG_VCCR_SCS_MASK) |
                   ((core_divider << SCG_VCCR_DIVCORE_SHIFT) & SCG_VCCR_DIVCORE_MASK) |
                   ((slow_divider << SCG_VCCR_DIVSLOW_SHIFT) & SCG_VCCR_DIVSLOW_MASK));

    p_base->VCCR = reg_val;
}

/**
 * @brief Gets the first asynchronous divider for FIRC.
 *
 * This function gets the first asynchronous divider for FIRC.
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @return FIRC first asynchronous divider
 */
static inline uint32_t
scg_get_firc_first_async_divider(const SCG_t *p_base)
{
    return (p_base->FIRCDIV & SCG_FIRCDIV_FIRCDIV1_MASK) >> SCG_FIRCDIV_FIRCDIV1_SHIFT;
}

/**
 * @brief Gets the second asynchronous divider for FIRC.
 *
 * This function gets the second asynchronous divider for FIRC.
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @return FIRC second asynchronous divider
 */
static inline uint32_t
scg_get_firc_second_async_divider(const SCG_t *p_base)
{
    return (p_base->FIRCDIV & SCG_FIRCDIV_FIRCDIV2_MASK) >> SCG_FIRCDIV_FIRCDIV2_SHIFT;
}

/**
 * @brief Sets SCG asynchronous dividers for FIRC.
 *
 * This function sets SCG asynchronous dividers for FIRC.
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @param[in] div1 Asynchronous first divider
 * @param[in] div2 Asynchronous second divider
 */
static inline void
scg_set_firc_async_config(SCG_t *p_base, uint32_t div1, uint32_t div2)
{
    uint32_t reg_val =
        (uint32_t)(((div1 << SCG_FIRCDIV_FIRCDIV1_SHIFT) & SCG_FIRCDIV_FIRCDIV1_MASK) |
                   ((div2 << SCG_FIRCDIV_FIRCDIV2_SHIFT) & SCG_FIRCDIV_FIRCDIV2_MASK));
    p_base->FIRCDIV = reg_val;
}

/**
 * @brief Gets the first asynchronous divider for SIRC.
 *
 * This function gets the first asynchronous divider for SIRC.
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @return SIRC first asynchronous divider
 */
static inline uint32_t
scg_get_sirc_first_async_divider(const SCG_t *p_base)
{
    return (p_base->SIRCDIV & SCG_SIRCDIV_SIRCDIV1_MASK) >> SCG_SIRCDIV_SIRCDIV1_SHIFT;
}

/**
 * @brief Gets the second asynchronous divider for SIRC.
 *
 * This function gets the second asynchronous divider for SIRC.
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @return SIRC second asynchronous divider
 */
static inline uint32_t
scg_get_sirc_second_async_divider(const SCG_t *p_base)
{
    return (p_base->SIRCDIV & SCG_SIRCDIV_SIRCDIV2_MASK) >> SCG_SIRCDIV_SIRCDIV2_SHIFT;
}

/**
 * @brief Sets SCG asynchronous dividers for SIRC.
 *
 * This function sets SCG asynchronous dividers for SIRC.
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @param[in] div1 Asynchronous first divider
 * @param[in] div2 Asynchronous second divider
 */
static inline void
scg_set_sirc_async_config(SCG_t *p_base, uint32_t div1, uint32_t div2)
{
    uint32_t reg_val =
        (uint32_t)(((div1 << SCG_SIRCDIV_SIRCDIV1_SHIFT) & SCG_SIRCDIV_SIRCDIV1_MASK) |
                   ((div2 << SCG_SIRCDIV_SIRCDIV2_SHIFT) & SCG_SIRCDIV_SIRCDIV2_MASK));
    p_base->SIRCDIV = reg_val;
}

/**
 * @brief Gets the first asynchronous divider for SOSC.
 *
 * This function gets the first asynchronous divider for SOSC.
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @return SOSC first asynchronous divider
 */
static inline uint32_t
scg_get_sosc_first_async_divider(const SCG_t *p_base)
{
    return (p_base->SOSCDIV & SCG_SOSCDIV_SOSCDIV1_MASK) >> SCG_SOSCDIV_SOSCDIV1_SHIFT;
}

/**
 * @brief Gets the second asynchronous divider for SOSC.
 *
 * This function gets the second asynchronous divider for SOSC.
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @return SOSC second asynchronous divider
 */
static inline uint32_t
scg_get_sosc_second_async_divide(const SCG_t *p_base)
{
    return (p_base->SOSCDIV & SCG_SOSCDIV_SOSCDIV2_MASK) >> SCG_SOSCDIV_SOSCDIV2_SHIFT;
}

/**
 * @brief Sets SCG asynchronous dividers for SOSC.
 *
 * This function sets SCG asynchronous dividers for SOSC.
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @param[in] div1 Asynchronous first divider
 * @param[in] div2 Asynchronous second divider
 */
static inline void
scg_set_sosc_async_config(SCG_t *p_base, uint32_t div1, uint32_t div2)
{
    uint32_t reg_val =
        (uint32_t)(((div1 << SCG_SOSCDIV_SOSCDIV1_SHIFT) & SCG_SOSCDIV_SOSCDIV1_MASK) |
                   ((div2 << SCG_SOSCDIV_SOSCDIV2_SHIFT) & SCG_SOSCDIV_SOSCDIV2_MASK));
    p_base->SOSCDIV = reg_val;
}

/**
 * @brief Set SCG ClockOut source select
 *
 * This function sets the SCG ClockOut source
 *
 * @param[in] p_base Register base address for the SCG instance.
 * @param[in] source used for ClockOut
 */
static inline void
scg_set_clockout_source_sel(SCG_t *p_base, uint32_t source)
{
    uint32_t reg_val;

    reg_val = p_base->CLKOUTCNFG;
    reg_val &= ~(SCG_CLKOUTCNFG_CLKOUTSEL_MASK);
    reg_val |= SCG_CLKOUTCNFG_CLKOUTSEL(source);
    p_base->CLKOUTCNFG = reg_val;
}

/**
 * @brief  Get core clock divide ratio value.
 *
 * @param[in] p_base  Base address for current SCG instance.
 */
static inline uint32_t
scg_get_vccr_divcore(const SCG_t *const p_base)
{
    uint32_t reg_val = p_base->VCCR;
    reg_val          = (reg_val & SCG_VCCR_DIVCORE_MASK) >> SCG_VCCR_DIVCORE_SHIFT;
    return reg_val;
}

/**
 * @brief Get slow clock divide ratio value.
 *
 * @param[in] p_base  Base address for current SCG instance.
 */
static inline uint32_t
scg_get_vccr_divslow(const SCG_t *const p_base)
{
    uint32_t reg_val = p_base->VCCR;
    reg_val          = (reg_val & SCG_VCCR_DIVSLOW_MASK) >> SCG_VCCR_DIVSLOW_SHIFT;
    return reg_val;
}

#ifdef __cplusplus
}
#endif

#endif /* SCG_ACCESS_H */

/*** end of file ***/
