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
extern unsigned int _estack;

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
/* Exceptions */
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMA0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void LPTMR0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void PORT_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FTM0_Ch0_7_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FTM0_Fault_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FTM0_Ovf_Reload_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FTM1_Ch0_7_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FTM1_Fault_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FTM1_Ovf_Reload_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FTFC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void HSSPI_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void LPI2C1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void WDOG_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void RCM_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void LPI2C0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void PUF_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void LPSPI0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void LPSPI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FSUSB_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void LPSPI2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void LPUART1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void LPUART0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

/*******************************************************************************
 * Global functions
 ******************************************************************************/

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
    DMA0_IRQHandler,            /*   0 DMA channel 0 transfer complete */
    DMA1_IRQHandler,            /*   1 DMA channel 1 transfer complete */
    DMA2_IRQHandler,            /*   2 DMA channel 2 transfer complete */
    DMA3_IRQHandler,            /*   3 DMA channel 3 transfer complete */
    0,                          /*   4 Reserved */
    0,                          /*   5 Reserved */
    0,                          /*   6 Reserved */
    0,                          /*   7 Reserved */
    LPTMR0_IRQHandler,          /*   8 LPTIMER interrupt request */
    PORT_IRQHandler,            /*   9 Port A, B, C, D and E pin detect interrupt */
    0,                          /*  10 Reserved */
    0,                          /*  11 Reserved */
    FTM0_Ch0_7_IRQHandler,      /*  12 FTM0 Channel 0 to 7 interrupt */
    FTM0_Fault_IRQHandler,      /*  13 FTM0 Fault interrupt */
    FTM0_Ovf_Reload_IRQHandler, /*  14 FTM0 Counter overflow and Reload interrupt */
    FTM1_Ch0_7_IRQHandler,      /*  15 FTM1 Channel 0 to 7 interrupt */
    FTM1_Fault_IRQHandler,      /*  16 FTM1 Fault interrupt */
    FTM1_Ovf_Reload_IRQHandler, /*  17 FTM1 Counter overflow and Reload interrupt */
    FTFC_IRQHandler,            /*  18 FTFC Read collision and Double bit fault detect */
    HSSPI_IRQHandler,           /*  19 HSSPI Interrupt */
    LPI2C1_IRQHandler,          /*  20 LPI2C1 Interrupt */
    0,                          /*  21 Reserved */
    WDOG_IRQHandler,            /*  22 WDOG interrupt request out before wdg reset out */
    RCM_IRQHandler,             /*  23 RCM Asynchronous Interrupt */
    LPI2C0_IRQHandler,          /*  24 LPI2C0 Interrupt */
    PUF_IRQHandler,             /*  25 PUF Interrupt */
    LPSPI0_IRQHandler,          /*  26 LPSPI0 Interrupt */
    LPSPI1_IRQHandler,          /*  27 LPSPI1 Interrupt */
    FSUSB_IRQHandler,           /*  28 FSUSB Interrupt */
    LPSPI2_IRQHandler,          /*  29 LPSPI2 Interrupt */
    LPUART1_IRQHandler,         /*  30 LPUART1 Transmit / Receive  Interrupt */
    LPUART0_IRQHandler          /*  31 LPUART0 Transmit / Receive Interrupt */
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
