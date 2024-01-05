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

#include "timer.h"
#include "cortexm/exception-handlers.h"

// ----------------------------------------------------------------------------

#if defined(USE_HAL_DRIVER)
void HAL_IncTick(void);
#endif

// Forward declarations.

void
timer_tick (void);

// ----------------------------------------------------------------------------

volatile timer_ticks_t timer_delayCount;

// ----------------------------------------------------------------------------

/* Interrupt Priorities are WORD accessible only under Armv6-M                  */
/* The following MACROS handle generation of the register offset and byte masks */
#define _BIT_SHIFT(IRQn)         (  ((((uint32_t)(int32_t)(IRQn))         )      &  0x03UL) * 8UL)
#define _SHP_IDX(IRQn)           ( (((((uint32_t)(int32_t)(IRQn)) & 0x0FUL)-8UL) >>    2UL)      )
#define _IP_IDX(IRQn)            (   (((uint32_t)(int32_t)(IRQn))                >>    2UL)      )

void
timer_start (void)
{
  // Use SysTick as reference for the delay loops.
  // SysTick_Config (g_system_core_clock / TIMER_FREQUENCY_HZ);
  uint32_t ticks = (g_system_core_clock / TIMER_FREQUENCY_HZ);;
  if ((ticks - 1UL) > SysTick_RVR_RELOAD_MASK)
  {
    return;                                                         /* Reload value impossible */
  }

  SysTick->RVR   = (uint32_t)(ticks - 1UL);                         /* set reload register */
  // NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  {
	  int32_t IRQn = (int32_t)SysTick_IRQn;
	  uint32_t priority = (1UL << __NVIC_PRIO_BITS) - 1UL;
	  if (IRQn >= 0)
	  {
	    NVIC->IPR[_IP_IDX(IRQn)]  = ((uint32_t)(NVIC->IPR[_IP_IDX(IRQn)]  & ~(0xFFUL << _BIT_SHIFT(IRQn))) |
	      (((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL) << _BIT_SHIFT(IRQn)));
	  }
	  else
	  {
		if (_SHP_IDX(IRQn) == 0)
		{
	      SCB->SHPR2 = ((uint32_t)(SCB->SHPR2 & ~(0xFFUL << _BIT_SHIFT(IRQn))) |
	        (((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL) << _BIT_SHIFT(IRQn)));
		}
		else
		{
		  SCB->SHPR3 = ((uint32_t)(SCB->SHPR3 & ~(0xFFUL << _BIT_SHIFT(IRQn))) |
		    (((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL) << _BIT_SHIFT(IRQn)));

		}
	  }

  }
  SysTick->CVR   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CSR   = SysTick_CSR_CLKSOURCE_MASK |
				   SysTick_CSR_TICKINT_MASK   |
				   SysTick_CSR_ENABLE_MASK;                         /* Enable SysTick IRQ and SysTick Timer */
  return;                                                           /* Function successful */
}

void
timer_sleep (timer_ticks_t ticks)
{
  timer_delayCount = ticks;

  // Busy wait until the SysTick decrements the counter to zero.
  while (timer_delayCount != 0u)
    ;
}

void
timer_tick (void)
{
  // Decrement to zero the counter used by the delay routine.
  if (timer_delayCount != 0u)
    {
      --timer_delayCount;
    }
}

// ----- SysTick_Handler() ----------------------------------------------------

void
SysTick_Handler (void)
{
#if defined(USE_HAL_DRIVER)
  HAL_IncTick();
#endif
  timer_tick ();
}

// ----------------------------------------------------------------------------
