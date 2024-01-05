/**
 * @file lptmr_driver.h
 * @brief LPTMR driver header file.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef LPTMR_DRIVER_H
#define LPTMR_DRIVER_H

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
#include <clock_gating.h>

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/**
 * @brief Pulse Counter Input selection
 * Implements : lptmr_pinselect_t_Class
 */
typedef enum
{
    LPTMR_PINSELECT_TRGMUX = 0x00u,
    LPTMR_PINSELECT_ALT1   = 0x01u,
    LPTMR_PINSELECT_ALT2   = 0x02u,
    LPTMR_PINSELECT_ALT3   = 0x03u
} lptmr_pinselect_t;

/**
 * @brief Pulse Counter input polarity
 * Implements : lptmr_pinpolarity_t_Class
 */
typedef enum
{
    LPTMR_PINPOLARITY_RISING  = 0u,
    LPTMR_PINPOLARITY_FALLING = 1u /*!< Count pulse on falling edge */
} lptmr_pinpolarity_t;

/**
 * @brief Work Mode
 * Implements : lptmr_workmode_t_Class
 */
typedef enum
{
    LPTMR_WORKMODE_TIMER        = 0u, /*!< Timer */
    LPTMR_WORKMODE_PULSECOUNTER = 1u  /*!< Pulse counter */
} lptmr_workmode_t;

/**
 * @brief Prescaler Selection
 *
 * LPTMR_PRESCALE_2                 - prescaler:2, filter:invalid
 * LPTMR_PRESCALE_4_GLITCHFILTER_2  - prescaler:4, filter:2clocks
 * LPTMR_PRESCALE_8_GLITCHFILTER_4  - prescaler:8, filter:4clocks
 * LPTMR_PRESCALE_16_GLITCHFILTER_8 - prescaler:16, filter:8clocks
 * LPTMR_PRESCALE_32_GLITCHFILTER_16 - prescaler:32, filter:16clocks
 * LPTMR_PRESCALE_64_GLITCHFILTER_32 - prescaler:64, filter:32clocks
 * LPTMR_PRESCALE_128_GLITCHFILTER_64 - prescaler:128, filter:64clocks
 * LPTMR_PRESCALE_256_GLITCHFILTER_128 - prescaler:256, filter:128clocks
 * LPTMR_PRESCALE_512_GLITCHFILTER_256 - prescaler:512, filter:256clocks
 * LPTMR_PRESCALE_1024_GLITCHFILTER_512 - prescaler:1024, filter:512clocks
 * LPTMR_PRESCALE_2048_GLITCHFILTER_1024 - prescaler:2048, filter:1024clocks
 * LPTMR_PRESCALE_4096_GLITCHFILTER_2048 - prescaler:4096, filter:2048clocks
 * LPTMR_PRESCALE_8192_GLITCHFILTER_4096 - prescaler:8192, filter:4096clocks
 * LPTMR_PRESCALE_16384_GLITCHFILTER_8192 - prescaler:16384, filter:8192clocks
 * LPTMR_PRESCALE_32768_GLITCHFILTER_16384 - prescaler:32768, filter:16384clocks
 * LPTMR_PRESCALE_65536_GLITCHFILTER_32768 - prescaler:65536, filter:32768clocks
 *
 */
typedef enum
{
    LPTMR_PRESCALE_2                        = 0x00u,
    LPTMR_PRESCALE_4_GLITCHFILTER_2         = 0x01u,
    LPTMR_PRESCALE_8_GLITCHFILTER_4         = 0x02u,
    LPTMR_PRESCALE_16_GLITCHFILTER_8        = 0x03u,
    LPTMR_PRESCALE_32_GLITCHFILTER_16       = 0x04u,
    LPTMR_PRESCALE_64_GLITCHFILTER_32       = 0x05u,
    LPTMR_PRESCALE_128_GLITCHFILTER_64      = 0x06u,
    LPTMR_PRESCALE_256_GLITCHFILTER_128     = 0x07u,
    LPTMR_PRESCALE_512_GLITCHFILTER_256     = 0x08u,
    LPTMR_PRESCALE_1024_GLITCHFILTER_512    = 0x09u,
    LPTMR_PRESCALE_2048_GLITCHFILTER_1024   = 0x0Au,
    LPTMR_PRESCALE_4096_GLITCHFILTER_2048   = 0x0Bu,
    LPTMR_PRESCALE_8192_GLITCHFILTER_4096   = 0x0Cu,
    LPTMR_PRESCALE_16384_GLITCHFILTER_8192  = 0x0Du,
    LPTMR_PRESCALE_32768_GLITCHFILTER_16384 = 0x0Eu,
    LPTMR_PRESCALE_65536_GLITCHFILTER_32768 = 0x0Fu
} lptmr_prescaler_t;

/**
 *  @brief Clock Source selection
 *  Implements : lptmr_clocksource_t_Class
 */
typedef enum
{
    LPTMR_CLOCKSOURCE_SIRCDIV2 = 0x00u, /*!< SIRC clock */
    LPTMR_CLOCKSOURCE_1KHZ_LPO = 0x01u, /*!< 1kHz LPO clock */
    LPTMR_CLOCKSOURCE_PCC      = 0x03u  /*!< PCC configured clock */
} lptmr_clocksource_t;

/**
 * @brief Defines the configuration structure for LPTMR.
 *
 * General parameters :
 *      b_dma_request       - Enable/Disable DMA requests
 *      b_interrupt_enable  - Enable/Disable Interrupt
 *      b_free_run          - Enable/Disable Free Running Mode
 *      work_mode         - Time/Pulse Counter Mode
 * Counter parameters :
 *      clock_select      - Clock selection for Timer/Glitch filter
 *      prescaler        - Prescaler Selection
 *      compare_value     - Compare value
 * Pulse Counter specific parameters :
 *      pin_select        - Pin selection for Pulse-Counter
 *      pin_polarity      - Pin Polarity for Pulse-Counter
 *
 */
typedef struct
{
    /* General parameters */
    bool             b_dma_request;
    bool             b_interrupt_enable;
    bool             b_free_run;
    lptmr_workmode_t work_mode;
    /* Counter parameters */
    lptmr_clocksource_t clock_select;
    lptmr_prescaler_t   prescaler;
    uint32_t            compare_value;
    /* Pulse Counter specific parameters */
    lptmr_pinselect_t   pin_select;
    lptmr_pinpolarity_t pin_polarity;
} lptmr_config_t;

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Initialize LPTMR configuration
 *
 * @param[in] instance - LPTMR instance number
 * @param[in] p_config - Pointer to the input configuration structure
 * @param[in] b_start_counter - Starting the counter immediately
 *
 */
void lptmr_init(const uint32_t              instance,
                const lptmr_config_t *const p_config,
                const bool                  b_start_counter);

/**
 * @brief Initialize the LPTMR instance to reset values.
 *
 * @param[in] instance - LPTMR instance number
 */
void lptmr_set_reset_value(const uint32_t instance);

/**
 * @brief Configure a LPTMR instance.
 *
 * @param[in] instance - LPTMR instance number
 * @param[in] p_config - Pointer to the input configuration structure
 */
void lptmr_set_config(const uint32_t instance, const lptmr_config_t *const p_config);

/**
 * @brief Get the current configuration of a LPTMR instance.
 *
 * @param[in] instance - LPTMR instance number
 * @param[out] p_config  - Pointer to the output configuration structure
 */
void lptmr_get_config(const uint32_t instance, lptmr_config_t *const p_config);

/**
 * @brief Enable the LPTMR / Start the counter
 *
 * @param[in] instance - LPTMR instance number
 */
void lptmr_start_counter(const uint32_t instance);

/**
 * @brief Disable the LPTMR / Stop the counter
 *
 * @param[in] instance - LPTMR instance number
 */
void lptmr_stop_counter(const uint32_t instance);

/**
 * @brief Get the current Counter Value
 *
 * This function returns the Counter Value.
 *
 * @param[in] instance - LPTMR instance number
 * @return The Counter Value
 */
uint16_t lptmr_get_counter_value(const uint32_t instance);

/**
 * @brief Clear the Compare Flag
 *
 * This function clears the Compare Flag/Interrupt Pending state.
 *
 * @param[in] base - LPTMR base pointer
 */
void lptmr_clear_compare_flag(const uint32_t instance);

#ifdef __cplusplus
}
#endif

#endif /* LPTMR_DRIVER_H */
