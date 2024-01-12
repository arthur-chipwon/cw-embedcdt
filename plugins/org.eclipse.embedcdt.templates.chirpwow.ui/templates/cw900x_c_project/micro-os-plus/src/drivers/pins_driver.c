/**
 * @file pins_driver.c
 * @brief Pins configure and set operations.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "pins_driver.h"

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
static void pins_init_set(const pin_settings_config_t *p_config);

/*******************************************************************************
 * Global functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : pins_chip_init
 * Description   : This function initializes these pins as pull-down
 *
 * Implements    : pins_chip_init_activity
 *END**************************************************************************/
void
pins_chip_init(void)
{
	uint32_t idx;
	uint32_t pin = 0x0000FFEF;  //skip swd pin

	for(idx=0; idx<16; idx++)
	{
	    if((pin >> idx) & 0x01){
		    PORTA->PCR[idx] = 0x00000002;
		    PORTC->PCR[idx] = 0x00000002;
        }
        PORTB->PCR[idx] = 0x00000002;
		PORTD->PCR[idx] = 0x00000002;
		PORTE->PCR[idx] = 0x00000002;
	}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_init
 * Description   : This function configures the pins with the options provided
 * in the given structure.
 *
 * Implements    : pins_init_activity
 *END**************************************************************************/
status_t
pins_init(uint32_t pin_count, const pin_settings_config_t config[])
{
    uint32_t idx;
    for (idx = 0U; idx < pin_count; idx++)
    {
        pins_init_set(&config[idx]);
    }
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_set_pull_sel
 * Description   : This function configures the internal resistor.
 *
 * Implements    : pins_set_pull_sel_activity
 *END**************************************************************************/
void
pins_set_pull_sel(PORT_t *const p_base, uint32_t pin, port_pull_config_t pull_config)
{
    pins_set_pull_sel_reg(p_base, pin, pull_config);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_set_mux_mode_sel
 * Description   : This function configures the pin muxing.
 *
 * Implements    : pins_set_mux_mode_sel_activity
 *END**************************************************************************/
void
pins_set_mux_mode_sel(PORT_t *const p_base, uint32_t pin, port_mux_t mux)
{
    DEV_ASSERT(pin < PORT_PCR_COUNT);
    uint32_t   reg_val = p_base->PCR[pin];
    port_mux_t muxing  = mux;
    reg_val &= ~(PORT_PCR_MUX_MASK);
    reg_val |= PORT_PCR_MUX(muxing);
    p_base->PCR[pin] = reg_val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_set_pin_int_sel
 * Description   : This function configures the port pin interrupt/DMA request.
 *
 * Implements    : pins_set_pin_int_sel_activity
 *END**************************************************************************/
void
pins_set_pin_int_sel(PORT_t *const p_base, uint32_t pin, port_interrupt_config_t int_config)
{
    pins_set_pin_int_sel_reg(p_base, pin, int_config);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_get_pin_int_sel
 * Description   : This function gets the current port pin interrupt/DMA request
 * configuration.
 *
 * Implements    : pins_get_pin_int_sel_activity
 *END**************************************************************************/
port_interrupt_config_t
pins_get_pin_int_sel(const PORT_t *const p_base, uint32_t pin)
{
    return pins_get_pin_int_sel_reg(p_base, pin);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_clear_pin_int_flag_cmd
 * Description   : This function clears the individual pin-interrupt status
 * flag.
 *
 * Implements    : pins_clear_pin_int_flag_cmd_activity
 *END**************************************************************************/
void
pins_clear_pin_int_flag_cmd(PORT_t *const p_base, uint32_t pin)
{
    pins_clear_pin_int_flag_cmd_reg(p_base, pin);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_enable_digital_filter
 * Description   : This function enables digital filter feature for digital pin
 * muxing.
 *
 * Implements    : pins_enable_digital_filter_activity
 *END**************************************************************************/
void
pins_enable_digital_filter(PORT_t *const p_base, uint32_t pin)
{
    pins_enable_digital_filter_reg(p_base, pin);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_disable_digital_filter
 * Description   : This function disables digital filter feature for digital
 * pin muxing.
 *
 * Implements    : pins_disable_digital_filter_activity
 *END**************************************************************************/
void
pins_disable_digital_filter(PORT_t *const p_base, uint32_t pin)
{
    pins_disable_digital_filter_reg(p_base, pin);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_config_digital_filter
 * Description   : This function configures digital filter for port with
 * given configuration.
 *
 * Implements    : pins_config_digital_filter_activity
 *END**************************************************************************/
void
pins_config_digital_filter(PORT_t *const p_base, const port_digital_filter_config_t *const config)
{
    pins_config_digital_filter_reg(p_base, config);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_get_port_int_flag
 * Description   : This function reads the entire port interrupt status flag.
 *
 * Implements    : pins_get_port_int_flag_activity
 *END**************************************************************************/
uint32_t
pins_get_port_int_flag(const PORT_t *const p_base)
{
    return pins_get_port_int_flag_reg(p_base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_clear_port_int_flag_cmd
 * Description   : This function clears the entire port interrupt status flag.
 *
 * Implements    : pins_clear_port_int_flag_cmd_activity
 *END**************************************************************************/
void
pins_clear_port_int_flag_cmd(PORT_t *const p_base)
{
    pins_clear_port_int_flag_cmd_reg(p_base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_get_pins_direction
 * Description   : This function returns the current pins directions for a port.
 * Pins corresponding to bits with value of '1' are configured as output and
 * pins corresponding to bits with value of '0' are configured as input.
 *
 * Implements    : pins_get_pins_direction_activity
 *END**************************************************************************/
pins_channel_type_t
pins_get_pins_direction(const GPIO_t *const p_base)
{
    return pins_gpio_get_pins_direction(p_base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_set_pin_direction
 * Description   : This function configures the direction for the given pin,
 * with the given value('1' for pin to be configured as output and '0' for pin
 * to be configured as input).
 *
 * Implements    : pins_set_pin_direction_activity
 *END**************************************************************************/
void
pins_set_pin_direction(GPIO_t *const p_base, uint32_t pin, pins_level_type_t direction)
{
    pins_gpio_set_pin_direction(p_base, pin, direction);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_set_pins_direction
 * Description   : This function sets the direction configuration for all pins
 * in a port. Pins corresponding to bits with value of '1' will be configured as
 * output and pins corresponding to bits with value of '0' will be configured as
 * input.
 *
 * Implements    : pins_set_pins_direction_activity
 *END**************************************************************************/
void
pins_set_pins_direction(GPIO_t *const p_base, pins_channel_type_t pins)
{
    pins_gpio_set_pins_direction(p_base, pins);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_set_port_input_disable
 * Description   : This function sets the pins input state for a port.
 * Pins corresponding to bits with value of '1' will not be configured
 * as input and pins corresponding to bits with value of '0' will be configured
 * as input.
 *
 * Implements    : pins_set_port_input_disable_activity
 *END**************************************************************************/
void
pins_set_port_input_disable(GPIO_t *const p_base, pins_channel_type_t pins)
{
    pins_gpio_set_port_input_disable(p_base, pins);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_get_port_input_disable
 * Description   : This function returns the current pins input state for a
 * port. Pins corresponding to bits with value of '1' are not configured as
 * input and pins corresponding to bits with value of '0' are configured as
 * input.
 *
 * Implements    : pins_get_port_input_disable_activity
 *END**************************************************************************/
pins_channel_type_t
pins_get_port_input_disable(const GPIO_t *const p_base)
{
    return pins_gpio_get_port_input_disable(p_base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_set_global_pin_control
 * Description   : This function quickly configures multiple pins within the one
 * port for the same peripheral function with the same pin configuration.
 * Supports up to 16 pins with the lower or upper half of pin registers at the
 * same port.
 *
 * Implements    : pins_set_global_pin_control_activity
 *END**************************************************************************/
void
pins_set_global_pin_control(PORT_t *const p_base, uint16_t pins, uint16_t value)
{
    uint16_t mask = 0;
    /* keep only available fields */
    mask |= PORT_PCR_PS_MASK;
    mask |= PORT_PCR_PE_MASK;
    mask |= PORT_PCR_DSE_MASK;
    mask |= PORT_PCR_MUX_MASK;
    mask |= PORT_PCR_LK_MASK;
    mask &= value;

    p_base->GPCLR = (((uint32_t)pins) << PORT_GPCLR_GPWE_SHIFT) | (uint32_t)mask;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_set_global_int_control
 * Description   : This function quickly configures multiple pins within the one
 * port for the same peripheral function with the same interrupt configuration.
 * Supports up to 16 pins with the lower or upper half of pin registers at the
 * same port.
 *
 * Implements    : pins_set_global_int_control_activity
 *END**************************************************************************/
void
pins_set_global_int_control(PORT_t *const p_base, uint16_t pins, uint16_t value)
{
    uint32_t mask;
    mask = (((uint32_t)value) << PORT_GICLR_GIWD_SHIFT) & PORT_PCR_IRQC_MASK;

    p_base->GICLR = ((uint32_t)pins) | mask;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_set_pin_output
 * Description   : This function writes the given pin from a port, with the
 * given value * ('0' represents LOW, '1' represents HIGH).
 *
 * Implements    : pins_set_pin_output_activity
 *END**************************************************************************/
void
pins_set_pin_output(GPIO_t *const p_base, uint32_t pin, pins_level_type_t value)
{
    pins_gpio_write_pin(p_base, pin, value);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_set_pins_output
 * Description   : This function writes all pins configured as output with the
 * values given in the parameter pins. '0' represents LOW, '1' represents HIGH.
 *
 * Implements    : pins_set_pins_output_activity
 *END**************************************************************************/
void
pins_set_pins_output(GPIO_t *const p_base, pins_channel_type_t pins)
{
    pins_gpio_write_pins(p_base, pins);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_get_pins_output
 * Description   : This function returns the current output that is written to a
 * port. Only pins that are configured as output will have meaningful values.
 *
 * Implements    : pins_get_pins_output_activity
 *END**************************************************************************/
pins_channel_type_t
pins_get_pins_output(const GPIO_t *const p_base)
{
    return pins_gpio_get_pins_output(p_base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_set_pins
 * Description   : This function configures output pins listed in parameter pins
 * (bits that are '1') to have a value of 'set' (HIGH). Pins corresponding to
 * '0'will be unaffected.
 *
 * Implements    : pins_set_pins_activity
 *END**************************************************************************/
void
pins_set_pins(GPIO_t *const p_base, pins_channel_type_t pins)
{
    pins_gpio_set_pins(p_base, pins);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_clear_pins
 * Description   : This function configures output pins listed in parameter pins
 * (bits that are '1') to have a 'cleared' value (LOW). Pins corresponding to
 * '0'will be unaffected.
 *
 * Implements    : pins_clear_pins_activity
 *END**************************************************************************/
void
pins_clear_pins(GPIO_t *const p_base, pins_channel_type_t pins)
{
    pins_gpio_clear_pins(p_base, pins);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_toggle_pins
 * Description   : This function toggles output pins listed in parameter pins
 * (bits that are '1'). Pins corresponding to '0' will be unaffected.
 *
 * Implements    : pins_toggle_pins_activity
 *END**************************************************************************/
void
pins_toggle_pins(GPIO_t *const p_base, pins_channel_type_t pins)
{
    pins_gpio_toggle_pins(p_base, pins);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pins_read_pins
 * Description   : This function returns the current input values from a port.
 * Only pins configured as input will have meaningful values.
 *
 * Implements    : pins_read_pins_activity
 *END**************************************************************************/
pins_channel_type_t
pins_read_pins(const GPIO_t *const p_base)
{
    return pins_gpio_read_pins(p_base);
}

/*******************************************************************************
 * Static functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : PINS_Init
 * Description   : This function configures the pins with the options provided
 * in the provided structure.
 *
 *END**************************************************************************/
static void
pins_init_set(const pin_settings_config_t *p_config)
{
    DEV_ASSERT(p_config->p_base != NULL);
    DEV_ASSERT((PORT_MUX_AS_GPIO != p_config->mux) || (p_config->p_gpio_base != NULL));
    DEV_ASSERT(p_config->pin_port_idx < PORT_PCR_COUNT);
    uint32_t   reg_val = p_config->p_base->PCR[p_config->pin_port_idx];
    uint32_t   directions;
    uint32_t   digital_filters;
    port_mux_t muxing;

    switch (p_config->pull_config)
    {
        case PORT_INTERNAL_PULL_NOT_ENABLED:
        {
            reg_val &= ~(PORT_PCR_PE_MASK);
        }
        break;
        case PORT_INTERNAL_PULL_DOWN_ENABLED:
        {
            reg_val &= ~(PORT_PCR_PS_MASK);
            reg_val |= PORT_PCR_PE(1U);
        }
        break;
        case PORT_INTERNAL_PULL_UP_ENABLED:
        {
            reg_val |= PORT_PCR_PE(1U);
            reg_val |= PORT_PCR_PS(1U);
        }
        break;
        default:
        {
            /* invalid command */
            DEV_ASSERT(false);
        }
        break;
    }

    reg_val &= ~(PORT_PCR_DSE_MASK);
    reg_val |= PORT_PCR_DSE(p_config->drive_select);

    reg_val &= ~(PORT_PCR_MUX_MASK);
    muxing = p_config->mux;
    reg_val |= PORT_PCR_MUX(muxing);

    reg_val &= ~(PORT_PCR_LK_MASK);
    reg_val |= PORT_PCR_LK(p_config->b_pin_lock);

    reg_val &= ~(PORT_PCR_IRQC_MASK);
    reg_val |= PORT_PCR_IRQC(p_config->int_config);
    if (p_config->b_clear_int_flag)
    {
        reg_val &= ~(PORT_PCR_ISF_MASK);
        reg_val |= PORT_PCR_ISF(1U);
    }

    p_config->p_base->PCR[p_config->pin_port_idx] = reg_val;

    /* Read current digital filter of port */
    digital_filters = (uint32_t)(p_config->p_base->DFER);
    digital_filters &= ~(1UL << (p_config->pin_port_idx));
    digital_filters |= (((uint32_t)(p_config->b_digital_filter)) << (p_config->pin_port_idx));
    /* Write to digital filter enable register */
    p_config->p_base->DFER = digital_filters;

    /* If p_gpio_base address not null setup the direction of pin */
    if (PORT_MUX_AS_GPIO == p_config->mux)
    {
        /* Read current direction */
        directions = (uint32_t)(p_config->p_gpio_base->PDDR);
        switch (p_config->direction)
        {
            case GPIO_INPUT_DIRECTION:
            {
                directions &= ~(1UL << p_config->pin_port_idx);
            }
            break;
            case GPIO_OUTPUT_DIRECTION:
            {
                directions |= (1UL << p_config->pin_port_idx);
            }
            break;
            case GPIO_UNSPECIFIED_DIRECTION:
            /* pass-through */
            default:
            { /* nothing to configure */
                DEV_ASSERT(false);
            }
            break;
        }

        /* Configure initial value for output */
        if (p_config->direction == GPIO_OUTPUT_DIRECTION)
        {
            pins_gpio_write_pin(
                p_config->p_gpio_base, p_config->pin_port_idx, p_config->init_value);
        }

        /* Configure direction */
        p_config->p_gpio_base->PDDR = GPIO_PDDR_PDD(directions);
    }
}

/*** end of file ***/
