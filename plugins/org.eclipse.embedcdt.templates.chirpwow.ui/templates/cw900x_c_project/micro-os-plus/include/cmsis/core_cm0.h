/**
 * @file core_cm0.h
 * @brief CMSIS Cortex-M0 Core Peripheral Access Layer Header File.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef CORE_CM0_H
#define CORE_CM0_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/** \brief  BKPT_ASM
 *
 *   Macro to be used to trigger an debug interrupt
 */
#define BKPT_ASM __asm("BKPT #0\n\t")

/** \brief  Enable interrupts
 */
#define ENABLE_INTERRUPTS() __asm volatile("cpsie i" : : : "memory");

/** \brief  Disable interrupts
 */
#define DISABLE_INTERRUPTS() __asm volatile("cpsid i" : : : "memory");

/** \brief  Enter low-power standby state
 *    WFI (Wait For Interrupt) makes the processor suspend execution (Clock is
 * stopped) until an IRQ interrupts.
 */
#define STANDBY() __asm volatile("wfi")

/** \brief  No-op
 */
#define NOP() __asm volatile("nop")

/** \brief  Reverse byte order in a word.
 * ARM Documentation Cortex-M0 Devices Generic User Guide
 * Accordingly to 3.5.7. REV, REV16, and REVSH
 * Restriction "This function requires low registers R0-R7 register"
 */
#define REV_BYTES_32(a, b) __asm volatile("rev %0, %1" : "=l"(b) : "l"(a))

/** \brief  Reverse byte order in each halfword independently.
 * ARM Documentation Cortex-M0 Devices Generic User Guide
 * Accordingly to 3.5.7. REV, REV16, and REVSH
 * Restriction "This function requires low registers R0-R7 register"
 */
#define REV_BYTES_16(a, b) __asm volatile("rev16 %0, %1" : "=l"(b) : "l"(a))

/** \brief  Places a function in RAM. */
#define START_FUNCTION_DECLARATION_RAMSECTION
/** \brief  Places a function in RAM. */
#define END_FUNCTION_DECLARATION_RAMSECTION __attribute__((section(".code_ram")));

#define START_FUNCTION_DEFINITION_RAMSECTION    /**< Start function definition ramsection */
#define END_FUNCTION_DEFINITION_RAMSECTION      /**< End function definition ramsection */
#define DISABLE_CHECK_RAMSECTION_FUNCTION_CALL  /**< Disable check ramsection function code */
#define ENABLE_CHECK_RAMSECTION_FUNCTION_CALL   /**< Enable check ramsection function code */

/** \brief  Get Core ID
 *
 *   GET_CORE_ID returns the processor identification number for cm0
 */
#define GET_CORE_ID() 0U

/** \brief  Data alignment.
 */
#define ALIGNED(x) __attribute__((aligned(x)))

/** \brief  Endianness.
 */
#define CORE_LITTLE_ENDIAN

#ifdef __cplusplus
}
#endif

#endif /* CORE_CM0_H */

/*** end of file ***/
