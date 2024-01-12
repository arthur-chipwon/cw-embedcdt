/**
 * @file lptmr_access.h
 * @brief LPTMR driver header file.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef LPTMR_ACCESS_H
#define LPTMR_ACCESS_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "device_registers.h"

/*******************************************************************************
 * Static functions
 ******************************************************************************/
/**
 * @brief Get the DMA Request Enable Flag
 *
 * This function checks whether a DMA Request feature of the LPTMR is enabled.
 * The DMA Request is issued when a Compare Match is asserted. If enabled, the
 * Compare Match/Interrupt Pending flag is cleared when the DMA controller is
 * done.
 *
 * @param[in] p_base - LPTMR base pointer
 * @return DMA Request enable
 *      - true: enable DMA Request
 *      - false: disable DMA Request
 */
static inline bool
lptmr_get_dma_request(const LPTMR_t *const p_base)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CSR;

    tmp = (tmp & LPTMR_CSR_TDRE_MASK) >> LPTMR_CSR_TDRE_SHIFT;

    return ((tmp == 1u) ? true : false);
}

/**
 * @brief Configure the DMA Request Enable Flag state
 *
 * This function configures the DMA Request feature of the LPTMR. If enabled,
 * a DMA Request is issued when the Compare Match event occurs. If enabled, the
 * Compare Match/Interrupt Pending flag is cleared when the DMA controller is
 * done.
 *
 * @param[in] p_base   - LPTMR base pointer
 * @param[in] b_enable - The new state of the DMA Request Enable Flag
 *      - true: enable DMA Request
 *      - false: disable DMA Request
 */
static inline void
lptmr_set_dma_request(LPTMR_t *const p_base, bool b_enable)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CSR;
    tmp &= ~(LPTMR_CSR_TDRE_MASK | LPTMR_CSR_TCF_MASK);
    tmp |= LPTMR_CSR_TDRE(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->CSR = tmp;
}

/**
 * @brief Get the Interrupt Enable state
 *
 * This function returns the Interrupt Enable state for the LPTMR. If enabled,
 * an interrupt is generated when a Compare Match event occurs.
 *
 * @param[in] p_base - LPTMR base pointer
 * @return Interrupt Enable state
 *      - true: Interrupt enabled
 *      - false: Interrupt disabled
 */
static inline bool
lptmr_get_interrupt_enable(const LPTMR_t *const p_base)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CSR;
    tmp          = (tmp & LPTMR_CSR_TIE_MASK) >> LPTMR_CSR_TIE_SHIFT;

    return ((tmp == 1u) ? true : false);
}

/**
 * @brief Configure the Interrupt Enable state
 *
 * This function configures the Interrupt Enable state for the LPTMR. If
 * enabled, an interrupt is generated when a Compare Match event occurs.
 *
 * @param[in] p_base   - LPTMR base pointer
 * @param[in] b_enable - The new state for the interrupt
 *          - true: enable Interrupt
 *          - false: disable Interrupt
 */
static inline void
lptmr_set_interrupt(LPTMR_t *const p_base, bool b_enable)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CSR;
    tmp &= ~(LPTMR_CSR_TIE_MASK | LPTMR_CSR_TCF_MASK);
    tmp |= LPTMR_CSR_TIE(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->CSR = tmp;
}

/**
 * @brief Get the Pin select for Counter Mode
 *
 * This function returns the configured Input Pin for Pulse Counter Mode.
 *
 * @param[in] p_base - LPTMR base pointer
 * @return Input pin selection
 *          - LPTMR_PINSELECT_TRGMUX: count pulses from TRGMUX output
 *          - LPTMR_PINSELECT_ALT1: count pulses from pin alt 1
 *          - LPTMR_PINSELECT_ALT2: count pulses from pin alt 2
 *          - LPTMR_PINSELECT_ALT3: count pulses from pin alt 3
 */
static inline lptmr_pinselect_t
lptmr_get_pin_select(const LPTMR_t *const p_base)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CSR;
    tmp          = (tmp & LPTMR_CSR_TPS_MASK) >> LPTMR_CSR_TPS_SHIFT;
    return (lptmr_pinselect_t)(tmp);
}

/**
 * @brief Configure the Pin selection for Pulse Counter Mode
 *
 * This function configures the input Pin selection for Pulse Counter Mode.
 * This feature can be configured only when the LPTMR is disabled.
 *
 * @param[in] p_base   - LPTMR base pointer
 * @param[in] pinsel - Pin selection
 *          - LPTMR_PINSELECT_TRGMUX: count pulses from TRGMUX output
 *          - LPTMR_PINSELECT_ALT1: count pulses from pin alt 1
 *          - LPTMR_PINSELECT_ALT2: count pulses from pin alt 2
 *          - LPTMR_PINSELECT_ALT3: count pulses from pin alt 3
 */
static inline void
lptmr_set_pin_select(LPTMR_t *const p_base, const lptmr_pinselect_t pinsel)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CSR;
    tmp &= ~(LPTMR_CSR_TPS_MASK | LPTMR_CSR_TCF_MASK);
    tmp |= LPTMR_CSR_TPS(pinsel);
    p_base->CSR = tmp;
}

/**
 * @brief Get Pin Polarity for Pulse Counter Mode
 *
 * This function returns the configured pin polarity that triggers an increment
 * in Pulse Counter Mode.
 *
 * @param[in] p_base - LPTMR base pointer
 * @return The pin polarity for Pulse Counter Mode
 *          - LPTMR_PINPOLARITY_RISING: count pulse on Rising Edge
 *          - LPTMR_PINPOLARITY_FALLING: count pulse on Falling Edge
 */
static inline lptmr_pinpolarity_t
lptmr_get_pin_polarity(const LPTMR_t *const p_base)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CSR;
    tmp          = (tmp & LPTMR_CSR_TPP_MASK) >> LPTMR_CSR_TPP_SHIFT;

    return (lptmr_pinpolarity_t)((tmp == 0u) ? LPTMR_PINPOLARITY_RISING
                                             : LPTMR_PINPOLARITY_FALLING);
}

/**
 * @brief Configure Pin Polarity for Pulse Counter Mode
 *
 * This function configures the pin polarity that triggers an increment in Pulse
 * Counter Mode. This feature can be configured only when the LPTMR is disabled.
 *
 * @param[in] p_base - LPTMR base pointer
 * @param[in] pol  - The pin polarity to count in Pulse Counter Mode
 *          - LPTMR_PINPOLARITY_RISING: count pulse on Rising Edge
 *          - LPTMR_PINPOLARITY_FALLING: count pulse on Falling Edge
 */
static inline void
lptmr_set_pin_polarity(LPTMR_t *const p_base, const lptmr_pinpolarity_t pol)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CSR;
    tmp &= ~(LPTMR_CSR_TPP_MASK | LPTMR_CSR_TCF_MASK);
    tmp |= LPTMR_CSR_TPP(pol);
    p_base->CSR = tmp;
}

/**
 * @brief Get the Free Running state
 *
 * This function checks whether the Free Running feature of the LPTMR is enabled
 * or disabled.
 *
 * @param[in] p_base - LPTMR base pointer
 * @return Free running mode state
 *          - true: Free Running Mode enabled. Reset counter on 16-bit overflow
 *          - false: Free Running Mode disabled. Reset counter on Compare Match.
 */
static inline bool
lptmr_get_free_running(const LPTMR_t *const p_base)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CSR;
    tmp          = (tmp & LPTMR_CSR_TFC_MASK) >> LPTMR_CSR_TFC_SHIFT;

    return ((tmp == 1u) ? true : false);
}

/**
 * @brief Configure the Free Running state
 *
 * This function configures the Free Running feature of the LPTMR. This feature
 * can be configured only when the LPTMR is disabled.
 *
 * @param[in] p_base   - LPTMR base pointer
 * @param[in] b_enable - The new Free Running state
 *          - true: Free Running Mode enabled. Reset counter on 16-bit overflow
 *          - false: Free Running Mode disabled. Reset counter on Compare Match.
 */
static inline void
lptmr_set_free_running(LPTMR_t *const p_base, const bool b_enable)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CSR;
    tmp &= ~(LPTMR_CSR_TFC_MASK | LPTMR_CSR_TCF_MASK);
    tmp |= LPTMR_CSR_TFC(b_enable ? (uint32_t)1u : (uint32_t)0u);
    p_base->CSR = tmp;
}

/**
 * @brief Get current Work Mode
 *
 * This function returns the currently configured Work Mode for the LPTMR.
 *
 *
 * @param[in] p_base - LPTMR base pointer
 * @return Work Mode
 *          - LPTMR_WORKMODE_TIMER: LPTMR is in Timer Mode
 *          - LPTMR_WORKMODE_PULSECOUNTER: LPTMR is in Pulse Counter Mode
 */
static inline lptmr_workmode_t
lptmr_get_work_mode(const LPTMR_t *const p_base)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CSR;
    tmp          = (tmp & LPTMR_CSR_TMS_MASK) >> LPTMR_CSR_TMS_SHIFT;

    return (lptmr_workmode_t)((tmp == 0u) ? LPTMR_WORKMODE_TIMER : LPTMR_WORKMODE_PULSECOUNTER);
}

/**
 * @brief Configure the Work Mode
 *
 * This function configures the LPTMR to either Timer Mode or Pulse Counter
 * Mode. This feature can be configured only when the LPTMR is disabled.
 *
 * @param[in] p_base - LPTMR base pointer
 * @param[in] mode - New Work Mode
 *          - LPTMR_WORKMODE_TIMER: LPTMR set to Timer Mode
 *          - LPTMR_WORKMODE_PULSECOUNTER: LPTMR set to Pulse Counter Mode
 */
static inline void
lptmr_set_work_mode(LPTMR_t *const p_base, const lptmr_workmode_t mode)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CSR;
    tmp &= ~(LPTMR_CSR_TMS_MASK | LPTMR_CSR_TCF_MASK);
    tmp |= LPTMR_CSR_TMS(mode);
    p_base->CSR = tmp;
}

/**
 * @brief Enable the LPTMR
 *
 * Enable the LPTMR. Starts the timer/counter.
 *
 *
 * @param[in] p_base - LPTMR base pointer
 */
static inline void
lptmr_enable(LPTMR_t *const p_base)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CSR;
    tmp &= ~(LPTMR_CSR_TEN_MASK | LPTMR_CSR_TCF_MASK);
    tmp |= LPTMR_CSR_TEN(1u);
    p_base->CSR = tmp;
}

/**
 * @brief Disable the LPTMR
 *
 * Disable the LPTMR. Stop the Counter/Timer and allow reconfiguration.
 *
 * @param[in] p_base - LPTMR base pointer
 */
static inline void
lptmr_disable(LPTMR_t *const p_base)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CSR;
    tmp &= ~(LPTMR_CSR_TEN_MASK | LPTMR_CSR_TCF_MASK);
    tmp |= LPTMR_CSR_TEN(0u);
    p_base->CSR = tmp;
}

/**
 * @brief Get Prescaler/Glitch Filter divider value
 *
 * This function returns the currently configured Prescaler/Glitch Filter
 divider
 * value.
 *
 * @param[in] p_base - LPTMR base pointer
 * @return The Prescaler/Glitch filter value
 *- LPTMR_PRESCALE_2:
 *      Timer mode: prescaler 2, Glitch filter mode: invalid
 *- LPTMR_PRESCALE_4_GLITCHFILTER_2:
        Timer mode: prescaler 4, Glitch filter mode: 2 clocks
 *- LPTMR_PRESCALE_8_GLITCHFILTER_4:
        Timer mode: prescaler 8, Glitch filter mode: 4 clocks
 *- LPTMR_PRESCALE_16_GLITCHFILTER_8:
        Timer mode: prescaler 16, Glitch filter mode: 8 clocks
 *- LPTMR_PRESCALE_32_GLITCHFILTER_16:
        Timer mode: prescaler 32, Glitch filter mode: 16 clocks
 *- LPTMR_PRESCALE_64_GLITCHFILTER_32:
        Timer mode: prescaler 64, Glitch filter mode: 32 clocks
 *- LPTMR_PRESCALE_128_GLITCHFILTER_64:
        Timer mode: prescaler 128, Glitch filter mode: 64 clocks
 *- LPTMR_PRESCALE_256_GLITCHFILTER_128:
        Timer mode: prescaler 256, Glitch filter mode: 128 clocks
 *- LPTMR_PRESCALE_512_GLITCHFILTER_256:
        Timer mode: prescaler 512, Glitch filter mode: 256 clocks
 *- LPTMR_PRESCALE_1024_GLITCHFILTER_512:
        Timer mode: prescaler 1024, Glitch filter mode: 512 clocks
 *- LPTMR_PRESCALE_2048_GLITCHFILTER_1024:
        Timer mode: prescaler 2048, Glitch filter mode: 1024 clocks
 *- LPTMR_PRESCALE_4096_GLITCHFILTER_2048:
        Timer mode: prescaler 4096, Glitch filter mode: 2048 clocks
 *- LPTMR_PRESCALE_8192_GLITCHFILTER_4096:
        Timer mode: prescaler 8192, Glitch filter mode: 4096 clocks
 *- LPTMR_PRESCALE_16384_GLITCHFILTER_8192:
        Timer mode: prescaler 16384, Glitch filter mode: 8192 clocks
 *- LPTMR_PRESCALE_32768_GLITCHFILTER_16384:
        Timer mode: prescaler 32768, Glitch filter mode: 16384 clocks
 *- LPTMR_PRESCALE_65536_GLITCHFILTER_32768:
        Timer mode: prescaler 65536, Glitch filter mode: 32768 clocks
 */
static inline lptmr_prescaler_t
lptmr_get_prescaler(const LPTMR_t *const p_base)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->PSR;
    tmp          = (tmp & LPTMR_PSR_PRESCALE_MASK) >> LPTMR_PSR_PRESCALE_SHIFT;
    return (lptmr_prescaler_t)(tmp);
}

/**
 * @brief Configure the Prescaler/Glitch Filter divider value
 *
 * This function configures the value for the Prescaler/Glitch Filter. This
 * feature can be configured only when the LPTMR is disabled.
 *
 * @param[in] p_base  - LPTMR base pointer
 * @param[in] presc - The new Prescaler value
 * - LPTMR_PRESCALE_2:
 *      Timer mode: prescaler 2, Glitch filter mode: invalid
 * - LPTMR_PRESCALE_4_GLITCHFILTER_2:
 *      Timer mode: prescaler 4, Glitch filter mode: 2 clocks
 * - LPTMR_PRESCALE_8_GLITCHFILTER_4:
 *      Timer mode: prescaler 8, Glitch filter mode: 4 clocks
 * - LPTMR_PRESCALE_16_GLITCHFILTER_8:
 *      Timer mode: prescaler 16, Glitch filter mode: 8 clocks
 * - LPTMR_PRESCALE_32_GLITCHFILTER_16:
 *       Timer mode: prescaler 32, Glitch filter mode: 16 clocks
 * - LPTMR_PRESCALE_64_GLITCHFILTER_32:
 *      Timer mode: prescaler 64, Glitch filter mode: 32 clocks
 * - LPTMR_PRESCALE_128_GLITCHFILTER_64:
 *      Timer mode: prescaler 128, Glitch filter mode: 64 clocks
 * - LPTMR_PRESCALE_256_GLITCHFILTER_128:
 *      Timer mode: prescaler 256, Glitch filter mode: 128 clocks
 * - LPTMR_PRESCALE_512_GLITCHFILTER_256:
 *      Timer mode: prescaler 512, Glitch filter mode: 256 clocks
 * - LPTMR_PRESCALE_1024_GLITCHFILTER_512:
 *      Timer mode: prescaler 1024, Glitch filter mode: 512 clocks
 * - LPTMR_PRESCALE_2048_GLITCHFILTER_1024:
 *       Timer mode: prescaler 2048, Glitch filter mode: 1024 clocks
 * - LPTMR_PRESCALE_4096_GLITCHFILTER_2048:
 *      Timer mode: prescaler 4096, Glitch filter mode: 2048 clocks
 * - LPTMR_PRESCALE_8192_GLITCHFILTER_4096:
 *      Timer mode: prescaler 8192, Glitch filter mode: 4096 clocks
 * - LPTMR_PRESCALE_16384_GLITCHFILTER_8192:
 *      Timer mode: prescaler 16384, Glitch filter mode: 8192 clocks
 * - LPTMR_PRESCALE_32768_GLITCHFILTER_16384:
 *       Timer mode: prescaler 32768, Glitch filter mode: 16384 clocks
 * - LPTMR_PRESCALE_65536_GLITCHFILTER_32768:
 *      Timer mode: prescaler 65536, Glitch filter mode: 32768 clocks
 */
static inline void
lptmr_set_prescaler(LPTMR_t *const p_base, const lptmr_prescaler_t presc)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->PSR;
    tmp &= ~(LPTMR_PSR_PRESCALE_MASK);
    tmp |= LPTMR_PSR_PRESCALE(presc);
    p_base->PSR = tmp;
}

/**
 * @brief Get the LPTMR input Clock selection
 *
 * This function returns the current configured input Clock for the LPTMR.
 *
 * @param[in] p_base - LPTMR base pointer
 * @return The Clock source
 *          - LPTMR_CLOCKSOURCE_SIRCDIV2: clock from SIRC DIV2
 *          - LPTMR_CLOCKSOURCE_1KHZ_LPO: clock from 1kHz LPO
 *          - LPTMR_CLOCKSOURCE_PCC: clock from PCC
 */
static inline lptmr_clocksource_t
lptmr_get_clock_select(const LPTMR_t *const p_base)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->PSR;
    tmp          = (tmp & LPTMR_PSR_PCS_MASK) >> LPTMR_PSR_PCS_SHIFT;
    return (lptmr_clocksource_t)(tmp);
}

/**
 * @brief Configure the LPTMR input Clock selection
 *
 * This function configures a clock source for the LPTMR. This feature can be
 * configured only when the LPTMR is disabled.
 *
 * @param[in] p_base - LPTMR base pointer
 * @param[in] clocksel - New Clock Source
 *          - LPTMR_CLOCKSOURCE_SIRCDIV2: clock from SIRC DIV2
 *          - LPTMR_CLOCKSOURCE_1KHZ_LPO: clock from 1kHz LPO
 *          - LPTMR_CLOCKSOURCE_PCC: clock from PCC
 */
static inline void
lptmr_set_clock_select(LPTMR_t *const p_base, const lptmr_clocksource_t clocksel)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->PSR;
    tmp &= ~(LPTMR_PSR_PCS_MASK);
    tmp |= LPTMR_PSR_PCS(clocksel);
    p_base->PSR = tmp;
}

/**
 * @brief Get the Compare Value
 *
 * This function returns the current Compare Value.
 *
 * @param[in] p_base - LPTMR base pointer
 * @return The Compare Value
 */
static inline uint16_t
lptmr_get_compare_value(const LPTMR_t *const p_base)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CMR;
    tmp          = (tmp & LPTMR_CMR_COMPARE_MASK) >> LPTMR_CMR_COMPARE_SHIFT;
    return (uint16_t)(tmp);
}

/**
 * @brief Configure the Compare Value
 *
 * This function configures the Compare Value. If set to 0, the Compare Match
 * event and the hardware trigger assert and remain asserted until the timer is
 * disabled.
 *
 * @param[in] p_base - LPTMR base pointer
 * @param[in] compval - The new Compare Value
 */
static inline void
lptmr_set_compare_value(LPTMR_t *const p_base, const uint16_t compval)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t tmp = p_base->CMR;
    tmp &= ~(LPTMR_CMR_COMPARE_MASK);
    tmp |= LPTMR_CMR_COMPARE(compval);
    p_base->CMR = tmp;
}

/**
 * @brief Clear the Compare Flag
 *
 * This function clears the Compare Flag/Interrupt Pending state.
 *
 * @param[in] base - LPTMR base pointer
 */
static inline void
lptmr_clear_compare_flag_reg(LPTMR_t *const base)
{
    DEV_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    tmp |= (LPTMR_CSR_TCF_MASK);
    base->CSR = tmp;
}

#ifdef __cplusplus
}
#endif

#endif /* LPTMR_ACCESS_H */
