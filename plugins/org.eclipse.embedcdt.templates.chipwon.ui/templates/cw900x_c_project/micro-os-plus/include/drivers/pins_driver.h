/**
 * @file pins_driver.h
 * @brief Header file for the pins driver.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef PINS_DRIVER_H
#define PINS_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stddef.h>
#include "device_registers.h"
#include "pins_access.h"
#include "status.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/

/**
 * @brief Configures the port data direction
 * Implements : port_data_direction_t_Class
 */
typedef enum
{
    GPIO_INPUT_DIRECTION       = 0x0U, /**< Input direction.       */
    GPIO_OUTPUT_DIRECTION      = 0x1U, /**< Output direction.      */
    GPIO_UNSPECIFIED_DIRECTION = 0x2U  /**< Unspecified direction. */
} port_data_direction_t;

/**
 * @brief Configures the drive strength.
 * Implements : port_drive_strength_t_Class
 */
typedef enum
{
    PORT_LOW_DRIVE_STRENGTH  = 0U, /**< low drive strength is configured. */
    PORT_HIGH_DRIVE_STRENGTH = 1U  /**< high drive strength is configured.*/
} port_drive_strength_t;

/**
 * @brief Configures the Pin mux selection
 * Implements : port_mux_t_Class
 */
typedef enum
{
    PORT_PIN_DISABLED = 0U, /**< corresponding pin is disabled, but is used as an analog pin */
    PORT_MUX_AS_GPIO  = 1U, /**< corresponding pin is configured as GPIO */
    PORT_MUX_ALT2     = 2U, /**< chip-specific */
    PORT_MUX_ALT3     = 3U, /**< chip-specific */
    PORT_MUX_ALT4     = 4U, /**< chip-specific */
    PORT_MUX_ALT5     = 5U, /**< chip-specific */
    PORT_MUX_ALT6     = 6U, /**< chip-specific */
    PORT_MUX_ALT7     = 7U, /**< chip-specific */
} port_mux_t;

/**
 * @brief Defines the converter configuration
 *
 * This structure is used to configure the pins
 * Implements : pin_settings_config_t_Class
 */
typedef struct
{
    PORT_t                 *p_base;           /**< Port base pointer.           */
    uint32_t                pin_port_idx;     /**< Port pin number.             */
    port_pull_config_t      pull_config;      /**< Internal resistor pull feature. */
    port_drive_strength_t   drive_select;     /**< Configures the drive strength. */
    port_mux_t              mux;              /**< Pin (C55: Out) mux selection. */
    bool                    b_pin_lock;       /**< Lock pin control register. */
    port_interrupt_config_t int_config;       /**< Interrupt generation condition. */
    bool                    b_clear_int_flag; /**< Clears the interrupt status flag.   */
    bool                    b_digital_filter; /**< Enables digital filter.             */
    GPIO_t                 *p_gpio_base;      /**< GPIO base pointer.                  */
    port_data_direction_t   direction;        /**< Configures the port data direction. */
    pins_level_type_t       init_value;       /**< Initial value */
} pin_settings_config_t;

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * This function initializes these pins as pull-down
 */
void pins_chip_init(void);

/**
 * @brief Initializes the pins with the given configuration structure
 *
 * This function configures the pins with the options provided in the
 * provided structure.
 *
 * @param[in] pin_count The number of configured pins in structure
 * @param[in] config The configuration structure
 * @return The status of the operation
 */
status_t pins_init(uint32_t pin_count, const pin_settings_config_t config[]);

/**
 * @brief Configures the internal resistor.
 *
 * This function configures the internal resistor.
 *
 * @param[in] p_base Port base pointer (PORTA, PORTB, PORTC, etc.)
 * @param[in] pin Port pin number
 * @param[in] pull_config The pull configuration
 */
void pins_set_pull_sel(PORT_t *const p_base, uint32_t pin, port_pull_config_t pull_config);

/**
 * @brief Configures the pin muxing.
 *
 * This function configures the pin muxing.
 *
 * @param[in] p_base Port base pointer (PORTA, PORTB, PORTC, etc.)
 * @param[in] pin Port pin number
 * @param[in] mux Pin muxing slot selection
 */
void pins_set_mux_mode_sel(PORT_t *const p_base, uint32_t pin, port_mux_t mux);

/**
 * @brief Configures the port pin interrupt/DMA request.
 *
 * This function configures the port pin interrupt/DMA request.
 *
 * @param[in] p_base Port base pointer (PORTA, PORTB, PORTC, etc.)
 * @param[in] pin Port pin number
 * @param[in] int_config  Interrupt configuration
 */
void pins_set_pin_int_sel(PORT_t *const p_base, uint32_t pin, port_interrupt_config_t int_config);

/**
 * @brief Gets the current port pin interrupt/DMA request configuration.
 *
 * This function gets the current port pin interrupt/DMA request
 * configuration.
 *
 * @param[in] p_base Port base pointer (PORTA, PORTB, PORTC, etc.)
 * @param[in] pin Port pin number
 * @return Interrupt configuration
 */
port_interrupt_config_t pins_get_pin_int_sel(const PORT_t *const p_base, uint32_t pin);

/**
 * @brief Clears the individual pin-interrupt status flag.
 *
 * This function clears the individual pin-interrupt status flag.
 *
 * @param[in] p_base Port base pointer (PORTA, PORTB, PORTC, etc.)
 * @param[in] pin Port pin number
 */
void pins_clear_pin_int_flag_cmd(PORT_t *const p_base, uint32_t pin);

/**
 * @brief Enables digital filter for digital pin muxing
 *
 * This function enables digital filter feature for digital pin muxing
 *
 * @param[in] p_base Port base pointer (PORTA, PORTB, PORTC, etc.)
 * @param[in] pin Port pin number
 */
void pins_enable_digital_filter(PORT_t *const p_base, uint32_t pin);

/**
 * @brief Disables digital filter for digital pin muxing
 *
 * This function disables digital filter feature for digital pin muxing
 *
 * @param[in] p_base Port base pointer (PORTA, PORTB, PORTC, etc.)
 * @param[in] pin Port pin number
 */
void pins_disable_digital_filter(PORT_t *const p_base, uint32_t pin);

/**
 * @brief Configures digital filter for port with given configuration
 *
 * This function configures digital filter for port with given configuration
 *
 * Note: Updating the filter configuration must be done only after all
 * filters are disabled.
 *
 * @param[in] p_base Port base pointer (PORTA, PORTB, PORTC, etc.)
 * @param[in] config the digital filter configuration struct
 */
void pins_config_digital_filter(PORT_t *const                             p_base,
                                const port_digital_filter_config_t *const config);

/**
 * @brief Reads the entire port interrupt status flag
 *
 * This function reads the entire port interrupt status flag.
 *
 * @param[in] p_base Port base pointer (PORTA, PORTB, PORTC, etc.)
 * @return All 32 pin interrupt status flags
 */
uint32_t pins_get_port_int_flag(const PORT_t *const p_base);

/**
 * @brief Clears the entire port interrupt status flag.
 *
 * This function clears the entire port interrupt status flag.
 *
 * @param[in] p_base Port base pointer (PORTA, PORTB, PORTC, etc.)
 */
void pins_clear_port_int_flag_cmd(PORT_t *const p_base);

/**
 * @brief Quickly configures multiple pins with the same pin configuration.
 *
 * This function quickly configures multiple pins within the one port for
 * the same peripheral function with the same pin configuration. Supports up
 * to 16 pins with the lower or upper half of pin registers at the same port.
 *
 * Note: The set global interrupt control function
 * (pins_set_global_int_control) cannot be configured if you ever used this
 * function at the same port
 *
 * @param[in] p_base Port base pointer (PORTA, PORTB, PORTC, etc.)
 * @param[in] pins Pin mask where each bit represents one pin. For each bit:
 *        - 0: pins corresponding to bits with value of '1' is updated with the value input
 *        - 1: pins corresponding to bits with value of '0' is not updated with the value input
 * @param[in] value the config value will be updated for the pins are set to '1'
 */
void pins_set_global_pin_control(PORT_t *const p_base, uint16_t pins, uint16_t value);

/**
 * @brief Quickly configures multiple pins with the same interrupt configuration.
 *
 * This function quickly configures multiple pins within the one port for
 * the same peripheral function with the same interrupt configuration.
 * Supports up to 16 pins with the lower or upper half of pin registers at
 * the same port.
 *
 * Note: The set global pin control function (pins_set_global_pin_control)
 * cannot be configured if you ever used this function at the same port
 *
 * @param[in] p_base Port base pointer (PORTA, PORTB, PORTC, etc.)
 * @param[in] pins Pin mask where each bit represents one pin. For each bit:
 *        - 0: pins corresponding to bits with value of '1' is updated with the value input
 *        - 1: pins corresponding to bits with value of '0' is not updated with the value input
 * @param[in] value the config value will be updated for the pins are set to '1'
 */
void pins_set_global_int_control(PORT_t *const p_base, uint16_t pins, uint16_t value);

/**
 * @brief Get the pins directions configuration for a port
 *
 * This function returns the current pins directions for a port. Pins
 * corresponding to bits with value of '1' are configured as output and
 * pins corresponding to bits with value of '0' are configured as input.
 *
 * @param[in] p_base GPIO base pointer (PTA, PTB, PTC, etc.)
 * @return GPIO directions. Each bit represents one pin (LSB is pin 0, MSB
 * is pin 31). For each bit:
 *        - 0: corresponding pin is set to input
 *        - 1: corresponding pin is set to output
 */
pins_channel_type_t pins_get_pins_direction(const GPIO_t *const p_base);

/**
 * @brief Configure the direction for a certain pin from a port
 *
 * This function configures the direction for the given pin, with the
 * given value('1' for pin to be configured as output and '0' for pin to
 * be configured as input)
 *
 * Note: With some platforms when you want to set a pin as output only and
 * disable input completely, it is required to call
 * pins_set_port_input_disable if platform has this feature.
 *
 * @param[in] p_base GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param[in] pin The pin number for which to configure the direction
 * @param[in] direction The pin direction:
 *        - 0: corresponding pin is set to input
 *        - 1: corresponding pin is set to output
 */
void pins_set_pin_direction(GPIO_t *const p_base, uint32_t pin, pins_level_type_t direction);

/**
 * @brief Set the pins directions configuration for a port
 *
 * This function sets the direction configuration for all pins
 * in a port. Pins corresponding to bits with value of '1' will be
 * configured as output and pins corresponding to bits with value of '0'
 * will be configured as input.
 *
 * Note: With some platforms when you want to set a pin as output only and
 * disable input completely, it is required to call
 * pins_set_port_input_disable if platform has this feature.
 *
 * @param[in] p_base GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param[in] pins Pin mask where each bit represents one pin (LSB
 * is pin 0, MSB is pin 31). For each bit:
 *        - 0: corresponding pin is set to input
 *        - 1: corresponding pin is set to output
 */
void pins_set_pins_direction(GPIO_t *const p_base, pins_channel_type_t pins);

/**
 * @brief Set the pins input disable state for a port
 *
 * This function sets the pins input state for a port.
 * Pins corresponding to bits with value of '1' will not be configured
 * as input and pins corresponding to bits with value of '0' will be
 * configured as input.
 *
 * @param[in] p_base GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param[in] pins Pin mask where each bit represents one pin (LSB is pin 0,
 * MSB is pin 31). For each bit:
 *        - 0: corresponding pin is set to input
 *        - 1: corresponding pin is not set to input
 */
void pins_set_port_input_disable(GPIO_t *const p_base, pins_channel_type_t pins);

/**
 * @brief Get the pins input disable state for a port
 *
 * This function returns the current pins input state for a port. Pins
 * corresponding to bits with value of '1' are not configured as input and
 * pins corresponding to bits with value of '0' are configured as input.
 *
 * @param[in] p_base GPIO base pointer (PTA, PTB, PTC, etc.)
 * @return GPIO input state. Each bit represents one pin (LSB is pin 0, MSB
 * is pin 31). For each bit:
 *        - 0: corresponding pin is set to input
 *        - 1: corresponding pin is not set to input
 */
pins_channel_type_t pins_get_port_input_disable(const GPIO_t *const p_base);

/**
 * @brief Write a pin of a port with a given value
 *
 * This function writes the given pin from a port, with the given value
 * ('0' represents LOW, '1' represents HIGH).
 *
 * @param[in] p_base GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param[in] pin Pin number to be written
 * @param[in] value Pin value to be written
 *        - 0: corresponding pin is set to LOW
 *        - 1: corresponding pin is set to HIGH
 */
void pins_set_pin_output(GPIO_t *const p_base, uint32_t pin, pins_level_type_t value);

/**
 * @brief Write all pins of a port
 *
 * This function writes all pins configured as output with the values given
 * in the parameter pins. '0' represents LOW, '1' represents HIGH.
 *
 * @param[in] p_base GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param[in] pins Pin mask to be written
 *        - 0: corresponding pin is set to LOW
 *        - 1: corresponding pin is set to HIGH
 */
void pins_set_pins_output(GPIO_t *const p_base, pins_channel_type_t pins);

/**
 * @brief Get the current output from a port
 *
 * This function returns the current output that is written to a port. Only
 * pins that are configured as output will have meaningful values.
 *
 * @param[in] p_base GPIO base pointer (PTA, PTB, PTC, etc.)
 * @return GPIO outputs. Each bit represents one pin (LSB is pin 0, MSB is
 * pin 31). For each bit:
 *        - 0: corresponding pin is set to LOW
 *        - 1: corresponding pin is set to HIGH
 */
pins_channel_type_t pins_get_pins_output(const GPIO_t *const p_base);

/**
 * @brief Write pins with 'Set' value
 *
 * This function configures output pins listed in parameter pins (bits that
 * are '1') to have a value of 'set' (HIGH). Pins corresponding to '0' will
 * be unaffected.
 *
 * @param[in] p_base GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param[in] pins Pin mask of bits to be set. Each bit represents one pin
 * (LSB is pin 0, MSB is pin 31). For each bit:
 *        - 0: corresponding pin is unaffected
 *        - 1: corresponding pin is set to HIGH
 */
void pins_set_pins(GPIO_t *const p_base, pins_channel_type_t pins);

/**
 * @brief Write pins to 'Clear' value
 *
 * This function configures output pins listed in parameter pins (bits that
 * are '1') to have a 'cleared' value (LOW). Pins corresponding to '0' will
 * be unaffected.
 *
 * @param[in] p_base GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param[in] pins Pin mask of bits to be cleared. Each bit represents one
 * pin (LSB is pin 0, MSB is pin 31). For each bit:
 *        - 0: corresponding pin is unaffected
 *        - 1: corresponding pin is cleared(set to LOW)
 */
void pins_clear_pins(GPIO_t *const p_base, pins_channel_type_t pins);

/**
 * @brief Toggle pins value
 *
 * This function toggles output pins listed in parameter pins (bits that are
 * '1'). Pins corresponding to '0' will be unaffected.
 *
 * @param[in] p_base GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param[in] pins Pin mask of bits to be toggled.  Each bit represents one
 * pin (LSB is pin 0, MSB is pin 31). For each bit:
 *        - 0: corresponding pin is unaffected
 *        - 1: corresponding pin is toggled
 */
void pins_toggle_pins(GPIO_t *const p_base, pins_channel_type_t pins);

/**
 * @brief Read input pins
 *
 * This function returns the current input values from a port. Only pins
 * configured as input will have meaningful values.
 *
 * @param[in] p_base GPIO base pointer (PTA, PTB, PTC, etc.)
 * @return GPIO inputs. Each bit represents one pin (LSB is pin 0, MSB is pin 31).
 *      For each bit:
 *        - 0: corresponding pin is read as LOW
 *        - 1: corresponding pin is read as HIGH
 */
pins_channel_type_t pins_read_pins(const GPIO_t *const p_base);

#ifdef __cplusplus
}
#endif

#endif /* PINS_DRIVER_H */

/*** end of file ***/
