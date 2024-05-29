/**
 * @file interrupt_manager.c
 * @brief This file provides access to the interrupt manager.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "interrupt_manager.h"
#include "startup.h"

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/*******************************************************************************
 * Static variables
 ******************************************************************************/
/**
 * @brief Counter to manage the nested callings of global disable/enable interrupt.
 */
static int32_t s_interrupt_disable_count = 0;

/*******************************************************************************
 * Global functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : int_enable_irq
 * Description   : Enables an interrupt for a given IRQ number.
 * It calls the system NVIC API to access the interrupt control register for interrupt routing.
 * The input IRQ number does not include the core interrupt, only the peripheral interrupt,
 * from 0 to a maximum supported IRQ.
 *
 * Implements    : int_enable_irq_activity
 *END**************************************************************************/
void
int_enable_irq(IRQn_t irq_number)
{
#if (defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT))
    /* Check IRQ number - dev_irq_number is used to avoid compiler warning */
    int32_t dev_irq_number = (int32_t)irq_number;
    DEV_ASSERT(0 <= (int32_t)irq_number);
    DEV_ASSERT(dev_irq_number <= (int32_t)FEATURE_INTERRUPT_IRQ_MAX);
#endif /*(defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT)) */
    /* Enable interrupt */
    NVIC->ISER[(uint32_t)(irq_number) >> 5U] =
        (uint32_t)(1UL << ((uint32_t)(irq_number) & (uint32_t)0x1FU));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : int_disable_irq
 * Description   : Disable individual interrupt for a specified IRQ
 * It calls the system NVIC API to access the interrupt control register for interrupt routing.
 *
 * Implements    : int_disable_irq_activity
 *
 *END**************************************************************************/
void
int_disable_irq(IRQn_t irq_number)
{
#if (defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT))
    /* Check IRQ number - dev_irq_number is used to avoid compiler warning */
    int32_t dev_irq_number = (int32_t)irq_number;
    DEV_ASSERT(0 <= (int32_t)irq_number);
    DEV_ASSERT(dev_irq_number <= (int32_t)FEATURE_INTERRUPT_IRQ_MAX);
#endif /*(defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT)) */
    /* Disable interrupt */
    NVIC->ICER[((uint32_t)(irq_number) >> 5U)] =
        (uint32_t)(1UL << ((uint32_t)(irq_number) & (uint32_t)0x1FU));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : int_enable_irq_global
 * Description   : Enable system interrupt
 * This function will enable the global interrupt by calling the core API
 *
 * Implements    : int_enable_irq_global_activity
 *
 *END**************************************************************************/
void
int_enable_irq_global(void)
{
    /* Check and update */
    if (s_interrupt_disable_count > 0)
    {
        s_interrupt_disable_count--;

        if (s_interrupt_disable_count <= 0)
        {
            /* Enable the global interrupt*/
            ENABLE_INTERRUPTS();
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : int_disable_irq_global
 * Description   : Disable system interrupt
 * This function will disable the global interrupt by calling the core API
 *
 * Implements    : int_disable_irq_global_activity
 *
 *END**************************************************************************/
void
int_disable_irq_global(void)
{
    /* Disable the global interrupt */
    DISABLE_INTERRUPTS();

    /* Update counter*/
    s_interrupt_disable_count++;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : int_set_priority
 * Description   : Set the priority of an interrupt
 * This function will set the priority of an interrupt.
 * Note: The priority cannot be set for every core interrupt.
 *
 * Implements    : int_set_priority_activity
 *
 *END**************************************************************************/
void
int_set_priority(IRQn_t irq_number, uint8_t priority)
{

#if (defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT))

    /* Check IRQ number and priority - dev_irq_number is used to avoid compiler
     * warning */
    int32_t dev_irq_number = (int32_t)irq_number;
    DEV_ASSERT(FEATURE_INTERRUPT_IRQ_MIN <= irq_number);
    DEV_ASSERT(dev_irq_number <= (int32_t)FEATURE_INTERRUPT_IRQ_MAX);
    DEV_ASSERT(priority < (uint8_t)(1U << FEATURE_NVIC_PRIO_BITS));

#endif /*(defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT)) */

    uint8_t shift = (uint8_t)(8U - FEATURE_NVIC_PRIO_BITS);

    if ((int32_t)irq_number < 0)
    {
        uint32_t int_vector_id = ((uint32_t)(irq_number)&0xFU);
        uint32_t reg_id        = int_vector_id / 4U;

        /* Compute pointer to SHPR register - avoid MISRA violation. */
        DEV_ASSERT((int32_t)reg_id != 1);

        volatile uint32_t *p_shpr_reg_ptr =
            ((reg_id == 2U) ? (volatile uint32_t *)&SCB->SHPR2 : (volatile uint32_t *)&SCB->SHPR3);
        uint8_t pri_byte_shift = (uint8_t)(((uint8_t)(int_vector_id)&0x3U) << 3U);

        /* Clear the old value from the register */
        *p_shpr_reg_ptr &= ~(0xFFUL << pri_byte_shift);

        /* Set Priority for Cortex-M0P System Interrupts */
        *p_shpr_reg_ptr |= ((uint32_t)(((((uint32_t)priority) << shift)) & 0xFFUL))
                           << pri_byte_shift;
    }
    else
    {
        /* Set Priority for device specific Interrupts */

        uint32_t ipr_vector_id  = (uint32_t)(irq_number) >> 2U;
        uint8_t  pri_byte_shift = (uint8_t)((((uint8_t)irq_number) & 0x3U) << 3U);

        /* Clear the old value from the register */
        NVIC->IPR[ipr_vector_id] &= ~(0xFFUL << pri_byte_shift);

        NVIC->IPR[ipr_vector_id] |= ((uint32_t)(((((uint32_t)priority) << shift)) & 0xFFUL))
                                    << pri_byte_shift;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : int_get_priority
 * Description   : Get the priority of an interrupt
 * This function will get the priority of an interrupt.
 * Note: The priority cannot be obtained for every core interrupt.
 *
 * Implements    : int_get_priority_activity
 *
 *END**************************************************************************/
uint8_t
int_get_priority(IRQn_t irq_number)
{

#if (defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT))

    /* Check IRQ number - dev_irq_number is used to avoid compiler warning */
    int32_t dev_irq_number = (int32_t)irq_number;
    DEV_ASSERT(FEATURE_INTERRUPT_IRQ_MIN <= irq_number);
    DEV_ASSERT(dev_irq_number <= (int32_t)FEATURE_INTERRUPT_IRQ_MAX);

#endif /*(defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT)) */

    uint8_t priority = 0U;
    uint8_t shift    = (uint8_t)(8U - FEATURE_NVIC_PRIO_BITS);

    if ((int32_t)irq_number < 0)
    {
        uint32_t int_vector_id = ((uint32_t)(irq_number)&0xFU);
        uint32_t reg_id        = int_vector_id / 4U;

        /* Compute pointer to SHPR register - avoid MISRA violation. */

        DEV_ASSERT((int32_t)reg_id != 1);

        volatile const uint32_t *p_shpr_reg_ptr =
            ((reg_id == 2U) ? (volatile uint32_t *)&SCB->SHPR2 : (volatile uint32_t *)&SCB->SHPR3);
        uint8_t pri_byte_shift = (uint8_t)(((uint8_t)(int_vector_id)&0x3U) << 3U);

        priority = ((uint8_t)(*p_shpr_reg_ptr >> pri_byte_shift)) >> shift;
    }
    else
    {
        /* Get Priority for device specific Interrupts  */
        uint32_t ipr_vector_id  = (uint32_t)(irq_number) >> 2U;
        uint8_t  pri_byte_shift = (uint8_t)((((uint8_t)(irq_number)) & 0x3U) << 3U);
        priority                = ((uint8_t)(NVIC->IPR[ipr_vector_id] >> pri_byte_shift)) >> shift;
    }

    return priority;
}

#if FEATURE_INTERRUPT_HAS_PENDING_STATE
/*FUNCTION**********************************************************************
 *
 * Function Name : int_clear_pending
 * Description   : Clear Pending Interrupt
 * This function clears the pending bit of a peripheral interrupt
 * or a directed interrupt to this CPU (if available).
 * Implements    : int_clear_pending_activity
 *
 *END**************************************************************************/
void
int_clear_pending(IRQn_t irq_number)
{
#if (defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT))
    /* Check IRQ number - dev_irq_number is used to avoid compiler warning */
    int32_t dev_irq_number = (int32_t)irq_number;
    DEV_ASSERT(0 <= (int32_t)irq_number);
    DEV_ASSERT(dev_irq_number <= (int32_t)FEATURE_INTERRUPT_IRQ_MAX);
#endif /*(defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT)) */

    /* Clear Pending Interrupt */
    NVIC->ICPR[(uint32_t)(irq_number) >> 5U] =
        (uint32_t)(1UL << ((uint32_t)(irq_number) & (uint32_t)0x1FU));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : int_set_pending
 * Description   : Set Pending Interrupt
 * This function configures the pending bit of a peripheral interrupt.
 *
 * Implements    : int_set_pending_activity
 *
 *END**************************************************************************/
void
int_set_pending(IRQn_t irq_number)
{
#if (defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT))
    /* Check IRQ number - dev_irq_number is used to avoid compiler warning */
    int32_t dev_irq_number = (int32_t)irq_number;
    DEV_ASSERT(0 <= (int32_t)irq_number);
    DEV_ASSERT(dev_irq_number <= (int32_t)FEATURE_INTERRUPT_IRQ_MAX);
#endif /*(defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT)) */
    /* Set Pending Interrupt */
    NVIC->ISPR[(uint32_t)(irq_number) >> 5U] =
        (uint32_t)(1UL << ((uint32_t)(irq_number) & (uint32_t)0x1FU));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : int_get_pending
 * Description   : Get Pending Interrupt
 * This function gets the pending bit of a peripheral interrupt or a directed
 * interrupt to this CPU (if available).
 *
 * Implements    : int_get_pending_activity
 *
 *END**************************************************************************/
uint32_t
int_get_pending(IRQn_t irq_number)
{
#if (defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT))
    /* Check IRQ number - dev_irq_number is used to avoid compiler warning */
    int32_t dev_irq_number = (int32_t)irq_number;
    DEV_ASSERT(0 <= (int32_t)irq_number);
    DEV_ASSERT(dev_irq_number <= (int32_t)FEATURE_INTERRUPT_IRQ_MAX);
#endif /*(defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT)) */

    /* Get Pending Interrupt */
    return ((uint32_t)(((NVIC->ISPR[(((uint32_t)irq_number) >> 5UL)] &
                         (1UL << (((uint32_t)irq_number) & 0x1FUL))) != 0UL)
                           ? 1UL
                           : 0UL));
}
#endif /* FEATURE_INTERRUPT_HAS_PENDING_STATE */

/*** end of file ***/
