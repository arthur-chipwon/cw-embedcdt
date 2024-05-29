/**
 * @file pins_access.h
 * @brief Static inline function for the PORT/GPIO module.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef PINS_ACCESS_H
#define PINS_ACCESS_H

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
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/**
 * @brief Type of a GPIO channel representation
 * Implements : pins_channel_type_t_Class
 */
typedef uint32_t pins_channel_type_t;

/**
 * @brief Type of a port levels representation.
 * Implements : pins_level_type_t_Class
 */
typedef uint8_t pins_level_type_t;

/**
 * @brief Internal resistor pull feature selection
 * Implements : port_pull_config_t_Class
 */
typedef enum
{
    PORT_INTERNAL_PULL_NOT_ENABLED  = 0U, /**< internal is not enabled. */
    PORT_INTERNAL_PULL_DOWN_ENABLED = 1U, /**< internal pull-down.      */
    PORT_INTERNAL_PULL_UP_ENABLED   = 2U  /**< internal pull-up         */
} port_pull_config_t;

/**
 * @brief Configures the interrupt generation condition.
 * Implements : port_interrupt_config_t_Class
 */
typedef enum
{
    PORT_DMA_INT_DISABLED = 0x0U, /**< Interrupt/DMA request is disabled. */
    PORT_DMA_RISING_EDGE  = 0x1U, /**< DMA request on rising edge. */
    PORT_DMA_FALLING_EDGE = 0x2U, /**< DMA request on falling edge. */
    PORT_DMA_EITHER_EDGE  = 0x3U, /**< DMA request on either edge. */
    PORT_INT_LOGIC_ZERO   = 0x8U, /**< Interrupt when logic 0. */
    PORT_INT_RISING_EDGE  = 0x9U, /**< Interrupt on rising edge. */
    PORT_INT_FALLING_EDGE = 0xAU, /**< Interrupt on falling edge. */
    PORT_INT_EITHER_EDGE  = 0xBU, /**< Interrupt on either edge. */
    PORT_INT_LOGIC_ONE    = 0xCU, /**< Interrupt when logic 1. */
} port_interrupt_config_t;

/**
 * @brief Clock source for the digital input filters
 * Implements : port_digital_filter_clock_t_Class
 */
typedef enum
{
    PORT_DIGITAL_FILTER_BUS_CLOCK = 0U, /**< BUS clock. */
    PORT_DIGITAL_FILTER_LPO_CLOCK = 1U  /**< LPO clock. */
} port_digital_filter_clock_t;

/**
 * @brief The digital filter configuration
 * Implements : port_digital_filter_config_t_Class
 */
typedef struct
{
    port_digital_filter_clock_t clock; /**< The digital filter clock for port */
    uint8_t                     width; /**< The digital filter width value */
} port_digital_filter_config_t;

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/
/**
 * @brief Get the pins directions configuration for a port
 *
 * This function returns the current pins directions for a port. Pins
 * corresponding to bits with value of '1' are configured as output and
 * pins corresponding to bits with value of '0' are configured as input.
 *
 * @param p_base  GPIO base pointer (PTA, PTB, PTC, etc.)
 * @return GPIO directions. Each bit represents one pin (LSB is pin 0, MSB is
 * pin 31). For each bit:
 *        - 0: corresponding pin is set to input
 *        - 1: corresponding pin is set to output
 */
static inline pins_channel_type_t
pins_gpio_get_pins_direction(const GPIO_t *const p_base)
{
    return (pins_channel_type_t)p_base->PDDR;
}

/**
 * @brief Configure the direction for a certain pin from a port
 *
 * This function configures the direction for the given pin, with the
 * given value('1' for pin to be configured as output and '0' for pin to
 * be configured as input)
 *
 * @param p_base  GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param pin the pin number for which to configure the direction
 * @param direction the pin direction:
 *        - 0: corresponding pin is set to input
 *        - 1: corresponding pin is set to output
 */
static inline void
pins_gpio_set_pin_direction(GPIO_t *const       p_base,
                            pins_channel_type_t pin,
                            pins_level_type_t   direction)
{
    pins_channel_type_t pin_direction = (pins_channel_type_t)p_base->PDDR;
    pin_direction &= (pins_channel_type_t)(~((pins_channel_type_t)1U << pin));
    pin_direction |= (pins_channel_type_t)((pins_channel_type_t)direction << pin);
    p_base->PDDR = GPIO_PDDR_PDD(pin_direction);
}

/**
 * @brief Set the pins directions configuration for a port
 *
 * This function sets the direction configuration for all pins in a port.
 * Pins corresponding to bits with value of '1' will be configured as output and
 * pins corresponding to bits with value of '0' will be configured as input.
 *
 * @param p_base  GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param pins pin mask where each bit represents one pin (LSB is pin 0, MSB is pin 31).
 *      For each bit:
 *        - 0: corresponding pin is set to input
 *        - 1: corresponding pin is set to output
 */
static inline void
pins_gpio_set_pins_direction(GPIO_t *const p_base, pins_channel_type_t pins)
{
    p_base->PDDR = GPIO_PDDR_PDD(pins);
}

/**
 * @brief Set the pins input disable state for a port
 *
 * This function sets the pins input state for a port.
 * Pins corresponding to bits with value of '1' will not be configured as input and
 * pins corresponding to bits with value of '0' will be configured as input.
 *
 * @param p_base  GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param pins pin mask where each bit represents one pin (LSB is pin 0, MSB is pin 31).
 *      For each bit:
 *        - 0: corresponding pin is set to input
 *        - 1: corresponding pin is not set to input
 */
static inline void
pins_gpio_set_port_input_disable(GPIO_t *const p_base, pins_channel_type_t pins)
{
    p_base->PIDR = GPIO_PIDR_PID(pins);
}

/**
 * @brief Get the pins input disable state for a port
 *
 * This function returns the current pins input state for a port.
 * Pins corresponding to bits with value of '1' are not configured as input and
 * pins corresponding to bits with value of '0' are configured as input.
 *
 * @param p_base  GPIO base pointer (PTA, PTB, PTC, etc.)
 * @return GPIO input state. Each bit represents one pin (LSB is pin 0, MSB is pin 31).
 *      For each bit:
 *        - 0: corresponding pin is set to input
 *        - 1: corresponding pin is not set to input
 */
static inline pins_channel_type_t
pins_gpio_get_port_input_disable(const GPIO_t *const p_base)
{
    return (pins_channel_type_t)p_base->PIDR;
}

/**
 * @brief Write a pin of a port with a given value
 *
 * This function writes the given pin from a port, with the given value
 * ('0' represents LOW, '1' represents HIGH).
 *
 * @param p_base  GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param pin pin number to be written
 * @param value pin value to be written
 *        - 0: corresponding pin is set to LOW
 *        - 1: corresponding pin is set to HIGH
 */
static inline void
pins_gpio_write_pin(GPIO_t *const p_base, pins_channel_type_t pin, pins_level_type_t value)
{
    pins_channel_type_t pin_value = (pins_channel_type_t)p_base->PDOR;
    pin_value &= (pins_channel_type_t)(~((pins_channel_type_t)1U << pin));
    pin_value |= (pins_channel_type_t)((pins_channel_type_t)value << pin);
    p_base->PDOR = GPIO_PDOR_PDO(pin_value);
}

/**
 * @brief Write all pins of a port
 *
 * This function writes all pins configured as output with the values given in
 * the parameter pins. ('0' represents LOW, '1' represents HIGH).
 *
 * @param p_base  GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param pins pin mask to be written
 *        - 0: corresponding pin is set to LOW
 *        - 1: corresponding pin is set to HIGH
 */
static inline void
pins_gpio_write_pins(GPIO_t *const p_base, pins_channel_type_t pins)
{
    p_base->PDOR = GPIO_PDOR_PDO(pins);
}

/**
 * @brief Get the current output from a port
 *
 * This function returns the current output that is written to a port.
 * Only pins that are configured as output will have meaningful values.
 *
 * @param p_base  GPIO base pointer (PTA, PTB, PTC, etc.)
 * @return GPIO outputs. Each bit represents one pin (LSB is pin 0, MSB is pin 31).
 *      For each bit:
 *        - 0: corresponding pin is set to LOW
 *        - 1: corresponding pin is set to HIGH
 */
static inline pins_channel_type_t
pins_gpio_get_pins_output(const GPIO_t *const p_base)
{
    pins_channel_type_t ret_value = 0U;
    ret_value                     = (pins_channel_type_t)(p_base->PDOR);
    return ret_value;
}

/**
 * @brief Write pins with 'Set' value
 *
 * This function configures output pins listed in parameter pins (bits that are
 * '1') to have a value of 'set' (HIGH). Pins corresponding to '0' will be
 * unaffected.
 *
 * @param p_base  GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param pins pin mask of bits to be set. Each bit represents one pin (LSB is
 * pin 0, MSB is pin 31). For each bit:
 *        - 0: corresponding pin is unaffected
 *        - 1: corresponding pin is set to HIGH
 */
static inline void
pins_gpio_set_pins(GPIO_t *const p_base, pins_channel_type_t pins)
{
    p_base->PSOR = GPIO_PSOR_PTSO(pins);
}

/**
 * @brief Write pins to 'Clear' value
 *
 * This function configures output pins listed in parameter pins (bits that are
 * '1') to have a 'cleared' value (LOW). Pins corresponding to '0' will be
 * unaffected.
 *
 * @param p_base  GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param pins pin mask of bits to be cleared. Each bit represents one pin (LSB
 * is pin 0, MSB is pin 31). For each bit:
 *        - 0: corresponding pin is unaffected
 *        - 1: corresponding pin is cleared(set to LOW)
 */
static inline void
pins_gpio_clear_pins(GPIO_t *const p_base, pins_channel_type_t pins)
{
    p_base->PCOR = GPIO_PCOR_PTCO(pins);
}

/**
 * @brief Toggle pins value
 *
 * This function toggles output pins listed in parameter pins (bits that are
 * '1'). Pins corresponding to '0' will be unaffected.
 *
 * @param p_base  GPIO base pointer (PTA, PTB, PTC, etc.)
 * @param pins pin mask of bits to be toggled.  Each bit represents one pin (LSB
 * is pin 0, MSB is pin 31). For each bit:
 *        - 0: corresponding pin is unaffected
 *        - 1: corresponding pin is toggled
 */
static inline void
pins_gpio_toggle_pins(GPIO_t *const p_base, pins_channel_type_t pins)
{
    p_base->PTOR = GPIO_PTOR_PTTO(pins);
}

/**
 * @brief Read input pins
 *
 * This function returns the current input values from a port. Only pins
 * configured as input will have meaningful values.
 *
 * @param p_base  GPIO base pointer (PTA, PTB, PTC, etc.)
 * @return GPIO inputs. Each bit represents one pin (LSB is pin 0, MSB is pin
 * 31). For each bit:
 *        - 0: corresponding pin is read as LOW
 *        - 1: corresponding pin is read as HIGH
 */
static inline pins_channel_type_t
pins_gpio_read_pins(const GPIO_t *const p_base)
{
    pins_channel_type_t ret_value = 0U;
    ret_value                     = (pins_channel_type_t)(p_base->PDIR);
    return ret_value;
}

/**
 * @brief Configures the internal resistor.
 *
 * Pull configuration is valid in all digital pin muxing modes.
 *
 * @param[in] p_base      port base pointer.
 * @param[in] pin         port pin number
 * @param[in] pull_config internal resistor pull feature selection
 *        - PORT_PULL_NOT_ENABLED: internal pull-down or pull-up resistor is not enabled.
 *        - PORT_PULL_DOWN_ENABLED: internal pull-down resistor is enabled.
 *        - PORT_PULL_UP_ENABLED: internal pull-up resistor is enabled.
 */
static inline void
pins_set_pull_sel_reg(PORT_t *const p_base, uint32_t pin, port_pull_config_t pull_config)
{
    DEV_ASSERT(pin < PORT_PCR_COUNT);
    switch (pull_config)
    {
        case PORT_INTERNAL_PULL_NOT_ENABLED:
        {
            p_base->PCR[pin] &= ~(PORT_PCR_PE_MASK);
        }
        break;
        case PORT_INTERNAL_PULL_DOWN_ENABLED:
        {
            uint32_t reg_val = p_base->PCR[pin];
            reg_val &= ~(PORT_PCR_PS_MASK);
            reg_val |= PORT_PCR_PE(1U);
            p_base->PCR[pin] = reg_val;
        }
        break;
        case PORT_INTERNAL_PULL_UP_ENABLED:
        {
            uint32_t reg_val = p_base->PCR[pin];
            reg_val |= PORT_PCR_PE(1U);
            reg_val |= PORT_PCR_PS(1U);
            p_base->PCR[pin] = reg_val;
        }
        break;
        default:
            /* invalid command */
            DEV_ASSERT(false);
            break;
    }
}

/**
 * @brief Configures the port pin interrupt/DMA request.
 *
 * @param[in] p_base     port base pointer.
 * @param[in] pin        port pin number
 * @param[in] int_config  interrupt configuration
 *        - PORT_INT_DISABLED     : Interrupt/DMA request disabled.
 *        - PORT_DMA_RISING_EDGE  : DMA request on rising edge.
 *        - PORT_DMA_FALLING_EDGE : DMA request on falling edge.
 *        - PORT_DMA_EITHER_EDGE  : DMA request on either edge.
 *        - PORT_FLAG_RISING_EDGE : Flag sets on rising edge only.
 *        - PORT_FLAG_FALLING_EDGE: Flag sets on falling edge only.
 *        - PORT_FLAG_EITHER_EDGE : Flag sets on either edge only.
 *        - PORT_INT_LOGIC_ZERO   : Interrupt when logic zero.
 *        - PORT_INT_RISING_EDGE  : Interrupt on rising edge.
 *        - PORT_INT_FALLING_EDGE : Interrupt on falling edge.
 *        - PORT_INT_EITHER_EDGE  : Interrupt on either edge.
 *        - PORT_INT_LOGIC_ONE    : Interrupt when logic one.
 *        - PORT_HIGH_TRIGGER_OUT : Enable active high trigger output, flag is disabled.
 *        - PORT_LOW_TRIGGER_OUT  : Enable active low trigger output, flag is disabled.
 */
static inline void
pins_set_pin_int_sel_reg(PORT_t *const p_base, uint32_t pin, port_interrupt_config_t int_config)
{
    DEV_ASSERT(pin < PORT_PCR_COUNT);
    uint32_t reg_val = p_base->PCR[pin];
    reg_val &= ~(PORT_PCR_IRQC_MASK);
    reg_val |= PORT_PCR_IRQC(int_config);
    p_base->PCR[pin] = reg_val;
}

/**
 * @brief Gets the current port pin interrupt/DMA request configuration.
 *
 * @param[in] p_base  port base pointer
 * @param[in] pin   port pin number
 * @return  interrupt configuration
 *        - PORT_INT_DISABLED     : Interrupt/DMA request disabled.
 *        - PORT_DMA_RISING_EDGE  : DMA request on rising edge.
 *        - PORT_DMA_FALLING_EDGE : DMA request on falling edge.
 *        - PORT_DMA_EITHER_EDGE  : DMA request on either edge.
 *        - PORT_FLAG_RISING_EDGE : Flag sets on rising edge only.
 *        - PORT_FLAG_FALLING_EDGE: Flag sets on falling edge only.
 *        - PORT_FLAG_EITHER_EDGE : Flag sets on either edge only.
 *        - PORT_INT_LOGIC_ZERO   : Interrupt when logic zero.
 *        - PORT_INT_RISING_EDGE  : Interrupt on rising edge.
 *        - PORT_INT_FALLING_EDGE : Interrupt on falling edge.
 *        - PORT_INT_EITHER_EDGE  : Interrupt on either edge.
 *        - PORT_INT_LOGIC_ONE    : Interrupt when logic one.
 *        - PORT_HIGH_TRIGGER_OUT : Enable active high trigger output, flag is disabled.
 *        - PORT_LOW_TRIGGER_OUT  : Enable active low trigger output, flag is disabled.
 */
static inline port_interrupt_config_t
pins_get_pin_int_sel_reg(const PORT_t *const p_base, uint32_t pin)
{
    DEV_ASSERT(pin < PORT_PCR_COUNT);
    uint32_t reg_val = p_base->PCR[pin];
    reg_val          = (reg_val & PORT_PCR_IRQC_MASK) >> PORT_PCR_IRQC_SHIFT;

    return (port_interrupt_config_t)reg_val;
}

/**
 * @brief Clears the individual pin-interrupt status flag.
 *
 * @param[in] p_base  port base pointer
 * @param[in] pin   port pin number
 */
static inline void
pins_clear_pin_int_flag_cmd_reg(PORT_t *const p_base, uint32_t pin)
{
    DEV_ASSERT(pin < PORT_PCR_COUNT);
    uint32_t reg_val = p_base->PCR[pin];
    reg_val &= ~(PORT_PCR_ISF_MASK);
    reg_val |= PORT_PCR_ISF(1U);
    p_base->PCR[pin] = reg_val;
}

/**
 * @brief Enables digital filter for digital pin muxing
 *
 * @param[in] p_base  port base pointer
 * @param[in] pin   port pin number
 */
static inline void
pins_enable_digital_filter_reg(PORT_t *const p_base, uint32_t pin)
{
    DEV_ASSERT(pin < PORT_PCR_COUNT);
    p_base->DFER |= (uint32_t)1U << pin;
}

/**
 * @brief Disables digital filter for digital pin muxing
 *
 * @param[in] p_base  port base pointer
 * @param[in] pin   port pin number
 */
static inline void
pins_disable_digital_filter_reg(PORT_t *const p_base, uint32_t pin)
{
    DEV_ASSERT(pin < PORT_PCR_COUNT);
    p_base->DFER &= ~((uint32_t)1U << pin);
}

/**
 * @brief Configures digital filter clock for port with given configuration
 *
 * @param[in] p_base   port base pointer
 * @param[in] p_config configuration struct
 */
static inline void
pins_config_digital_filter_reg(PORT_t *const                             p_base,
                               const port_digital_filter_config_t *const p_config)
{
    DEV_ASSERT(p_config->width <= PORT_DFWR_FILT_MASK);
    p_base->DFCR = PORT_DFCR_CS(p_config->clock);
    p_base->DFWR = PORT_DFWR_FILT(p_config->width);
}

/**
 * @brief Reads the entire port interrupt status flag.
 *
 * @param[in] p_base  port base pointer
 * @return all 32 pin interrupt status flags. For specific bit:
 *         - 0: interrupt is not detected.
 *         - 1: interrupt is detected.
 */
static inline uint32_t
pins_get_port_int_flag_reg(const PORT_t *const p_base)
{
    uint32_t reg_val = p_base->ISFR;

    return reg_val;
}

/**
 * @brief Clears the entire port interrupt status flag.
 *
 * @param[in] p_base  port base pointer
 */
static inline void
pins_clear_port_int_flag_cmd_reg(PORT_t *const p_base)
{
    p_base->ISFR = PORT_ISFR_ISF_MASK;
}

#ifdef __cplusplus
}
#endif

#endif /* PINS_ACCESS_H */

/*** end of file ***/
