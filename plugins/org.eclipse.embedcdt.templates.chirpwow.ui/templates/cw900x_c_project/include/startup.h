/**
 * @file startup.h
 * @brief define symbols that specific start and end addres of some basic sections. 
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef STARTUP_H
#define STARTUP_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include "device_registers.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/**
 * @brief define symbols that specific start and end addres of some basic
 * sections.
 */
#define INTERRUPTS_SECTION_START    (uint8_t *)&__interrupts_start__
#define INTERRUPTS_SECTION_END      (uint8_t *)&__interrupts_end__
#define BSS_SECTION_START           (uint8_t *)&__bss_start__
#define BSS_SECTION_END             (uint8_t *)&__bss_end__
#define DATA_SECTION_START          (uint8_t *)&__data_start__
#define DATA_SECTION_END            (uint8_t *)&__data_end__
#define CUSTOMSECTION_SECTION_START (uint8_t *)&__customSection_start__
#define CUSTOMSECTION_SECTION_END   (uint8_t *)&__customSection_end__
#define CODE_RAM_SECTION_START      (uint8_t *)&__code_ram_start__
#define CODE_RAM_SECTION_END        (uint8_t *)&__code_ram_end__

/*******************************************************************************
 * Exported variables
 ******************************************************************************/
extern uint32_t __interrupts_start__;   /**< interrupt start address */
extern uint32_t __interrupts_end__;     /**< interrupt end address */
extern uint32_t __bss_start__;          /**< bss start address */
extern uint32_t __bss_end__;            /**< bss end address */
extern uint32_t __data_start__;         /**< data start address */
extern uint32_t __data_end__;           /**< data end address */
extern uint32_t __customSection_start__;/**< custom section start address */
extern uint32_t __customSection_end__;  /**< custom section end address */
extern uint32_t __code_ram_start__;     /**< ram start address */
extern uint32_t __code_ram_end__;       /**< ram end address */

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Make necessary initializations for RAM.
 *
 * - Copy initialized data from ROM to RAM.
 * - Clear the zero-initialized data section.
 * - Copy the vector table from ROM to RAM. This could be an option.  
 */
void init_data_bss(void);

#ifdef __cplusplus
}
#endif

#endif /* STARTUP_H */

/*** end of file ***/
