/**
 * @file interrupt_manager.h
 * @brief Header file for the interrupt manager driver.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef INTERRUPT_MANAGER_H
#define INTERRUPT_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "device_registers.h"

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/** @brief Interrupt handler type */
typedef void (*isr_t)(void);

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/** @brief Default ISR. */
void DefaultISR(void);

/**
 * @brief Enables an interrupt for a given IRQ number.
 *
 * This function  enables the individual interrupt for a specified IRQ number.
 *
 * @param irq_number IRQ number
 */
void int_enable_irq(IRQn_t irq_number);

/**
 * @brief Disables an interrupt for a given IRQ number.
 *
 * This function disables the individual interrupt for a specified IRQ number.
 *
 * @param irq_number IRQ number
 */
void int_disable_irq(IRQn_t irq_number);

/**
 * @brief Enables system interrupt.
 *
 * This function enables the global interrupt by calling the core API.
 *
 */
void int_enable_irq_global(void);

/**
 * @brief Disable system interrupt.
 *
 * This function disables the global interrupt by calling the core API.
 *
 */
void int_disable_irq_global(void);

/**
 * @brief  Set Interrupt Priority
 *
 *   The function sets the priority of an interrupt.
 *
 *   @param  irq_number  Interrupt number.
 *   @param  priority  Priority to set.
 */
void int_set_priority(IRQn_t irq_number, uint8_t priority);

/**
 * @brief  Get Interrupt Priority
 *
 *   The function gets the priority of an interrupt.
 *
 *   @param  irq_number  Interrupt number.
 *   @return priority   Priority of the interrupt.
 */
uint8_t int_get_priority(IRQn_t irq_number);

#if FEATURE_INTERRUPT_HAS_PENDING_STATE
/**
 * @brief Clear Pending Interrupt
 *
 * The function clears the pending bit of a peripheral interrupt
 * or a directed interrupt to this CPU (if available).
 *
 * @param irq_number IRQ number
 */
void int_clear_pending(IRQn_t irq_number);

/**
 * @brief Set Pending Interrupt
 *
 * The function configures the pending bit of a peripheral interrupt.
 *
 * @param irq_number IRQ number
 */
void int_set_pending(IRQn_t irq_number);

/**
 * @brief Get Pending Interrupt
 *
 * The function gets the pending bit of a peripheral interrupt
 * or a directed interrupt to this CPU (if available).
 *
 * @param irq_number IRQ number
 * @return pending  Pending status 0/1
 */
uint32_t int_get_pending(IRQn_t irq_number);

#endif /* FEATURE_INTERRUPT_HAS_PENDING_STATE */

#ifdef __cplusplus
}
#endif

#endif /* INTERRUPT_MANAGER_H */

/*** end of file ***/
