/**
 * @file lpi2c_driver.c
 * @brief The module provides for requiring LPI2C hardwre registers access.
 * Export functions for wide scope use.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lpi2c_driver.h"

/*******************************************************************************
 * Private macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/

/*******************************************************************************
 * Private typedefs
 ******************************************************************************/

/*******************************************************************************
 * Global variables
 ******************************************************************************/
/** @brief Table of base pointers for LPI2C instances. */
LPI2C_t *const gp_lpi2c_base[LPI2C_INSTANCE_COUNT] = LPI2C_BASE_PTRS;

/** @brief Table to save LPI2C IRQ enumeration numbers defined in the CMSIS
 * header file. */
const IRQn_t g_lpi2c_irq_id[LPI2C_INSTANCE_COUNT] = LPI2C_IRQS;

/** @brief Table to save LPI2C clock names as defined in clock manager. */
const clock_names_t g_lpi2c_clk_names[LPI2C_INSTANCE_COUNT] = LPI2C_CLOCK_NAMES;

/*******************************************************************************
 * Static variables
 ******************************************************************************/

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/

/*******************************************************************************
 * Global functions
 ******************************************************************************/
void
lpi2c_init(LPI2C_t *p_base)
{
    /*
     * The MCR[RST] will reset all master logic and registers to their default state,
     * except for the MCR itself. The SCR[RST] will reset all slave logic and registers
     * to their default state, except for the SCR itself. So we must reset the control
     * registers manually;
     */
    lpi2c_set_master_sw_reset(p_base, true);
    lpi2c_set_slave_sw_reset(p_base, true);

    p_base->MCR = 0x0U;
    p_base->SCR = 0x0U;
}

/*******************************************************************************
 * Static functions
 ******************************************************************************/

/*** end of file ***/
