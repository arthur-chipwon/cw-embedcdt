/**
 * @file lpi2c_driver.h
 * @brief The module provides for requiring LPI2C hardwre registers access.
 * Export functions for wide scope use.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef LPI2C_DRIVER_H
#define LPI2C_DRIVER_H

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
#include "lpi2c_access.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/** @brief Device instance number. */
#define INST_LPI2C0 0u

/** @brief Device instance number. */
#define INST_LPI2C1 1u

/*******************************************************************************
 * Typedefs
 ******************************************************************************/

/*******************************************************************************
 * Exported variables
 ******************************************************************************/
/** @brief Table of base pointers for LPI2C instances. */
extern LPI2C_t *const gp_lpi2c_base[LPI2C_INSTANCE_COUNT];

/** @brief Table to save LPI2C IRQ enumeration numbers defined in the CMSIS
 * header file. */
extern const IRQn_t g_lpi2c_irq_id[LPI2C_INSTANCE_COUNT];

/** @brief Table to save LPI2C clock names as defined in clock manager. */
extern const clock_names_t g_lpi2c_clk_names[LPI2C_INSTANCE_COUNT];

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/*!
 * @brief Initializes the LPI2C module to a known state.
 *
 * This function initializes all the registers of the LPI2C module to
 * their reset value.
 *
 * @param[in] p_base  base address of the LPI2C module
 */
void lpi2c_init(LPI2C_t *p_base);

#ifdef __cplusplus
}
#endif

#endif /* LPI2C_DRIVER_H */

/*** end of file ***/
