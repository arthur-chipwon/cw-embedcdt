/**
 * @file startup_$(CMSIS_name).c
 * @brief The file is generated from CMSIS-Core(M) Device Startup File.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */
/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "cortexm/exception-handlers.h"

/*******************************************************************************
 * Private macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/

/*******************************************************************************
 * Private typedefs
 ******************************************************************************/
typedef void (*const handler_pft)(void);

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/*******************************************************************************
 * Static variables
 ******************************************************************************/

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
void __attribute__((weak)) Default_Handler(void);

// Forward declaration of the specific IRQ handlers. These are aliased
// to the Default_Handler, which is a 'forever' loop. When the application
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions
//
// TODO: Rename this and add the actual routines here.
void __attribute__((weak, alias("Default_Handler"))) DeviceInterrupt_Handler(void);

/*******************************************************************************
 * Global functions
 ******************************************************************************/
extern unsigned int _estack;

// The vector table.
// This relies on the linker script to place at correct location in memory.
__attribute__((section(".isr_vector"), used)) handler_pft __isr_vectors[] = {
    //
    (handler_pft)&_estack, /*     Initial Stack Pointer */
    Reset_Handler,         /*     Reset Handler */
    NMI_Handler,           /* -14 NMI Handler */
    HardFault_Handler,     /* -13 Hard Fault Handler */
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
    MemManage_Handler,     /* -12 MPU Fault Handler */
    BusFault_Handler,      /* -11 Bus Fault Handler */
    UsageFault_Handler,    /* -10 Usage Fault Handler */
#else
    0,                     /*     Reserved */
    0,                     /*     Reserved */
    0,                     /*     Reserved */
#endif
    0,                     /*     Reserved */
    0,                     /*     Reserved */
    0,                     /*     Reserved */
    0,                     /*     Reserved */
    SVC_Handler,           /*  -5 SVCall Handler */
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
    DebugMon_Handler,      /*  -4 Debug Monitor Handler */
#else
    0,                     /*     Reserved */
#endif
    0,                     /*     Reserved */
    PendSV_Handler,        /*  -2 PendSV Handler */
    SysTick_Handler,       /*  -1 SysTick Handler */

    // ----------------------------------------------------------------------
    // $(CMSIS_name) vectors
    DeviceInterrupt_Handler, /* first Device Interrupt */
                             // TODO: rename and add more vectors here
};

/*******************************************************************************
 * Static functions
 ******************************************************************************/
// Processor ends up here if an unexpected interrupt occurs or a specific
// handler is not present in the application code.
void __attribute__((section(".after_vectors"))) Default_Handler(void)
{
    for (;;)
    {
        ;
    }
}

/*** end of file ***/
