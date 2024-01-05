/**
 * @file startup.c
 * @brief During startup, tw9001 performs necessary initialization on the RAM.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include "startup.h"

/*******************************************************************************
 * Static variables
 ******************************************************************************/

/*******************************************************************************
 * Global functions
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : init_data_bss
 * Description   : Make necessary initializations for RAM.
 * - Copy the vector table from ROM to RAM.
 * - Copy initialized data from ROM to RAM.
 * - Copy code that should reside in RAM from ROM
 * - Clear the zero-initialized data section.
 *
 * Tool Chains:
 *   __GNUC__           : GNU Compiler Collection
 *
 * Implements    : init_data_bss_Activity
 *END**************************************************************************/
void
init_data_bss(void)
{
    /* For ARMC we are using the library method of initializing DATA, Custom
     * Section and Code RAM sections so the below variables are not needed */

    /* Declare pointers for various data sections. These pointers
     * are initialized using values pulled in from the linker file */
    uint8_t       *data_ram;
    uint8_t       *code_ram;
    uint8_t       *bss_start;
    uint8_t       *custom_ram;
    const uint8_t *data_rom, *data_rom_end;
    const uint8_t *code_rom, *code_rom_end;
    const uint8_t *bss_end;
    const uint8_t *custom_rom, *custom_rom_end;

    /* Get section information from linker files */
    extern uint32_t __DATA_ROM[];
    extern uint32_t __DATA_RAM[];
    extern uint32_t __DATA_END[];

    extern uint32_t __CODE_RAM[];
    extern uint32_t __CODE_ROM[];
    extern uint32_t __CODE_END[];

    extern uint32_t __BSS_START[];
    extern uint32_t __BSS_END[];

    extern uint32_t __CUSTOM_ROM[];
    extern uint32_t __CUSTOM_END[];

    /* Data */
    data_ram     = (uint8_t *)__DATA_RAM;
    data_rom     = (uint8_t *)__DATA_ROM;
    data_rom_end = (uint8_t *)__DATA_END;
    /* CODE RAM */
    code_ram     = (uint8_t *)__CODE_RAM;
    code_rom     = (uint8_t *)__CODE_ROM;
    code_rom_end = (uint8_t *)__CODE_END;
    /* BSS */
    bss_start = (uint8_t *)__BSS_START;
    bss_end   = (uint8_t *)__BSS_END;

    /* Custom section */
    custom_ram     = CUSTOMSECTION_SECTION_START;
    custom_rom     = (uint8_t *)__CUSTOM_ROM;
    custom_rom_end = (uint8_t *)__CUSTOM_END;

    /* Copy initialized data from ROM to RAM */
    while (data_rom_end != data_rom)
    {
        *data_ram = *data_rom;
        data_ram++;
        data_rom++;
    }

    /* Copy functions from ROM to RAM */
    while (code_rom_end != code_rom)
    {
        *code_ram = *code_rom;
        code_ram++;
        code_rom++;
    }

    /* Clear the zero-initialized data section */
    while (bss_end != bss_start)
    {
        *bss_start = 0;
        bss_start++;
    }

    /* Copy customsection rom to ram */
    while (custom_rom_end != custom_rom)
    {
        *custom_ram = *custom_rom;
        custom_rom++;
        custom_ram++;
    }
}

/*** end of file ***/
