/**
 * @file pcc_access.h
 * @brief Static inline function for the PCC module.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef PCC_ACCESS_H
#define PCC_ACCESS_H

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
 * Exported variables
 ******************************************************************************/
extern const uint16_t g_clock_name_mappings[CLOCK_NAME_COUNT];

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/
/**
 * @brief Sets PCC control register
 *
 * @param[in] p_base        pcc base pointer
 * @param[in] b_clock_gate  control register can be written or not
 * @param[in] clock_name    clock for module
 * @param[in] clock_source  SIRC,FIRC,SOSC..
 * @param[in] divider       divider
 * @param[in] multiplier    multiplier
 */
static inline void
pcc_set_peripheral_clk_control(PCC_t        *p_base,
                               clock_names_t clock_name,
                               bool          b_clock_gate,
                               uint32_t      clock_source,
                               uint32_t      divider,
                               uint32_t      multiplier)
{
    /* Configure the peripheral clock source, the fractional clock divider and the clock gate */
    uint32_t reg_val = PCC_PCCn_PCS(clock_source) | PCC_PCCn_FRAC(multiplier) |
                       PCC_PCCn_PCD(divider) | PCC_PCCn_CGC(b_clock_gate ? 1UL : 0UL);

    p_base->PCCn[g_clock_name_mappings[clock_name]] = reg_val;
}

/**
 * @brief Enables/disables the clock for a given peripheral.
 * For example, to enable the LPUART0 clock, use like this:
 * @code
 *  pcc_set_clk_mode(PCC, LPUART0_CLK, true);
 * @endcode
 *
 * @param[in] p_base        pcc base pointer
 * @param[in] clock_name   is the name of the peripheral clock must be one of
 * the following values
 *          DMAMUX0_CLK, ..., LPTMR0_CLK
 * @param[in] b_is_clock_enabled  is the value of the command that enables/disables the clock
 */
static inline void
pcc_set_clk_mode(PCC_t *const p_base, const clock_names_t clock_name, const bool b_is_clock_enabled)
{
    if (b_is_clock_enabled)
    {
        p_base->PCCn[g_clock_name_mappings[clock_name]] |= PCC_PCCn_CGC(1UL);
    }
    else
    {
        p_base->PCCn[g_clock_name_mappings[clock_name]] &= (uint32_t)(~(PCC_PCCn_CGC_MASK));
    }
}

/**
 * @brief Gets the clock gate control mode.
 *
 * @param[in] p_base        pcc base pointer
 * @param[in] clock_name   is the name of the peripheral clock must be one of
 * the following values
 *          DMAMUX0_CLK, ..., LPTMR0_CLK
 * @return  the clock gate control mode
 *        - false : Clock is disabled
 *        - true : Clock is enabled
 */
static inline bool
pcc_get_clk_mode(const PCC_t *const p_base, const clock_names_t clock_name)
{
    uint32_t reg_val = (uint32_t)p_base->PCCn[g_clock_name_mappings[clock_name]];
    reg_val          = (reg_val & PCC_PCCn_CGC_MASK) >> PCC_PCCn_CGC_SHIFT;
    return (reg_val == 0U) ? false : true;
}

/**
 * @brief Gets the selection of a clock source for a specific peripheral
 *
 * @param[in] p_base        pcc base pointer
 * @param[in] clock_name   is the name of the peripheral clock must be one of
 * the following values
 *          DMAMUX0_CLK, ..., LPTMR0_CLK
 * @return  the clock source
 */
static inline uint32_t
pcc_get_clk_source_sel(const PCC_t *const p_base, const clock_names_t clock_name)
{
    return ((p_base->PCCn[g_clock_name_mappings[clock_name]] & PCC_PCCn_PCS_MASK) >>
            PCC_PCCn_PCS_SHIFT);
}

/**
 * @brief Gets the selection of the fractional value for a specific
 * peripheral
 *
 * @param[in] p_base        pcc base pointer
 * @param[in] clock_name   is the name of the peripheral clock must be one of
 * the following values
 *          DMAMUX0_CLK, ..., LPTMR0_CLK
 * @return  the fractional value
 *        - PCC_MULTPCCnLY_BY_ONE : Fractional value is zero
 *        - PCC_MULTPCCnLY_BY_TWO : Fractional value is one
 */
static inline uint32_t
pcc_get_frac_value_sel(const PCC_t *const p_base, const clock_names_t clock_name)
{
    return ((p_base->PCCn[g_clock_name_mappings[clock_name]] & PCC_PCCn_FRAC_MASK) >>
            PCC_PCCn_FRAC_SHIFT);
}

/**
 * @brief Gets the selection of the divider value for a specific peripheral
 *
 * @param[in] p_base        pcc base pointer
 * @param[in] clock_name   is the name of the peripheral clock must be one of
 * the following values
 *          DMAMUX0_CLK, ..., LPTMR0_CLK
 * @return  the divider value
 *        - PCC_DIVIDE_BY_ONE   : Divide by 1
 *        - PCC_DIVIDE_BY_TWO   : Divide by 2
 *        - PCC_DIVIDE_BY_THREE : Divide by 3
 *        - PCC_DIVIDE_BY_FOUR  : Divide by 4
 *        - PCC_DIVIDE_BY_FIVE  : Divide by 5
 *        - PCC_DIVIDE_BY_SIX   : Divide by 6
 *        - PCC_DIVIDE_BY_SEVEN : Divide by 7
 *        - PCC_DIVIDE_BY_EIGTH : Divide by 8
 */
static inline uint32_t
pcc_get_divider_sel(const PCC_t *const p_base, const clock_names_t clock_name)
{
    return ((p_base->PCCn[g_clock_name_mappings[clock_name]] & PCC_PCCn_PCD_MASK) >>
            PCC_PCCn_PCD_SHIFT);
}

#ifdef __cplusplus
}
#endif

#endif /* PCC_ACCESS_H */

/*** end of file ***/
