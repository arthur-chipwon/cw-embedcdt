/**
 * @file osif_driver.c
 * @brief This file provides access to the osif driver.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stddef.h>
#include "osif_driver.h"
#include "interrupt_manager.h"
#include "clock_gating.h"

/*******************************************************************************
 * Private macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/** @brief Converts milliseconds to ticks - in this case, one tick = one millisecond */
#define MSEC_TO_TICK(msec) (msec)

/*******************************************************************************
 * Static variables
 ******************************************************************************/
/* Only include headers for configurations that need them. */
#if defined(CUSTOM_SYSICK_IRQ)
#else
static volatile uint32_t s_osif_tick_cnt = 0u;
#endif

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
#if defined(CUSTOM_SYSICK_IRQ)
#else
static inline void     osif_tick(void);
static inline uint32_t osif_get_current_tick_count(void);
static inline void     osif_update_tick_config(void);
static inline void     osif_disable_irq_global(void);
static inline void     osif_enable_irq_global(void);
#endif

/*******************************************************************************
 * Global functions
 ******************************************************************************/
#if defined(CUSTOM_SYSICK_IRQ)
static inline uint32_t
osif_get_current_tick_count(void)
{
    return 0u;
}

static inline void
osif_update_tick_config(void)
{
    /* do not update tick */
}

#define osif_disable_irq_global() (void)0;
#define osif_enable_irq_global()  (void)0;

#else
void
SysTick_Handler(void)
{
    osif_tick();
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : osif_time_delay
 * Description   : This function blocks execution for a number of milliseconds.
 *
 * Implements : osif_time_delay_baremetal_activity
 *END**************************************************************************/
void
osif_time_delay(const uint32_t delay)
{
    osif_update_tick_config();
    uint32_t start       = osif_get_current_tick_count();
    uint32_t crt_ticks   = osif_get_current_tick_count();
    uint32_t delta       = crt_ticks - start;
    uint32_t delay_ticks = MSEC_TO_TICK(delay);
    while (delta < delay_ticks)
    {
        crt_ticks = osif_get_current_tick_count();
        delta     = crt_ticks - start;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : osif_get_milliseconds
 * Description   : This function returns the number of miliseconds elapsed since
 *      starting the internal timer. To initialize the internal timer(Systick)
 *      in bare-metal, call either osif_time_delay or osif_sema_wait functions.
 *      Calling osif_time_delay(0) will initialize the timer without any side-effects (no delay).
 *
 * Implements    : osif_get_milliseconds_baremetal_activity
 *END**************************************************************************/
uint32_t
osif_get_milliseconds(void)
{
    /*
     * Please make sure the timer is initialized before calling this function.
     * For example, calling osif_time_delay(0) ensures that the timer is
     * initialized without any other side-effects. If osif_time_delay or
     * osif_sema_wait functions have been called, the timer is already
     * initialized.
     */
    return osif_get_current_tick_count(); /* This assumes that 1 tick = 1
                                             millisecond */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : osif_mutex_lock
 * Description   : This function locks a mutex (mock operation in baremetal case).
 *
 * Implements    : osif_mutex_lock_baremetal_activity
 *END**************************************************************************/
status_t
osif_mutex_lock(const mutex_t *const p_mutex, const uint32_t timeout)
{
    (void)p_mutex;
    (void)timeout;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : osif_mutex_unlock
 * Description   : This function unlocks a mutex (mock operation in baremetal case).
 *
 * Implements    : osif_mutex_unlock_baremetal_activity
 *END**************************************************************************/
status_t
osif_mutex_unlock(const mutex_t *const p_mutex)
{
    (void)p_mutex;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : osif_mutex_create
 * Description   : This function creates a mutex (mock operation in baremetal case).
 *
 * Implements    : osif_mutex_create_baremetal_activity
 *END**************************************************************************/
status_t
osif_mutex_create(mutex_t *const p_mutex)
{
    (void)p_mutex;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : osif_mutex_destroy
 * Description   : This function destroys a mutex (mock operation in baremetal case).
 *
 * Implements    : osif_mutex_destroy_baremetal_activity
 *END**************************************************************************/
status_t
osif_mutex_destroy(const mutex_t *const p_mutex)
{
    (void)p_mutex;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : osif_sema_wait
 * Description   : This function performs the 'wait' (decrement) operation on a
 * semaphore. When timeout value is 0, it's the equivalent of TryWait - try to
 * decrement but return immediately if it fails (counter is 0).
 *
 * Implements    : osif_sema_wait_baremetal_activity
 *END**************************************************************************/
status_t
osif_sema_wait(semaphore_t *const p_sem, const uint32_t timeout)
{
    DEV_ASSERT(p_sem != NULL);

    status_t osif_ret_code = STATUS_SUCCESS;

    osif_update_tick_config();
    if ((timeout == 0u))
    {
        /* when the timeout is 0 the wait operation is the equivalent of
           try_wait, meaning that if the semaphore is 0 return immediately with
           an error code
        */
        if (*p_sem == 0u)
        {
            osif_ret_code = STATUS_TIMEOUT;
        }
    }
    else
    {
        /* timeout is not 0 */
        uint32_t timeout_ticks;
        if (timeout == OSIF_WAIT_FOREVER)
        {
            timeout_ticks = OSIF_WAIT_FOREVER;
        }
        else
        {
            /* Convert timeout from milliseconds to ticks. */
            timeout_ticks = MSEC_TO_TICK(timeout);
        }
        uint32_t start = osif_get_current_tick_count();
        uint32_t end   = (uint32_t)(start + timeout_ticks);
        uint32_t max   = end - start;
        while (*p_sem == 0u)
        {
            uint32_t crt_ticks = osif_get_current_tick_count();
            uint32_t delta     = crt_ticks - start;
            if ((timeout_ticks != OSIF_WAIT_FOREVER) && (delta > max))
            {
                /* Timeout occured, stop waiting and return fail code */
                osif_ret_code = STATUS_TIMEOUT;
                break;
            }
        }
    }

    if (osif_ret_code == STATUS_SUCCESS)
    {
        osif_disable_irq_global();
        --(*p_sem);
        osif_enable_irq_global();
    }

    return osif_ret_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : osif_sema_post
 * Description   : This function performs the 'post' (increment) operation on a semaphore.
 *
 * Implements    : osif_sema_post_baremetal_activity
 *END**************************************************************************/
status_t
osif_sema_post(semaphore_t *const p_sem)
{
    DEV_ASSERT(p_sem != NULL);

    status_t osif_ret_code = STATUS_SUCCESS;
    osif_disable_irq_global();
    if (*p_sem != 255u)
    {
        ++(*p_sem);
    }
    else
    {
        osif_ret_code = STATUS_ERROR;
    }

    osif_enable_irq_global();

    return osif_ret_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : osif_sema_create
 * Description   : This function creates (initializes) a semaphore.
 *
 * Implements    : osif_sema_create_baremetal_activity
 *END**************************************************************************/
status_t
osif_sema_create(semaphore_t *const p_sem, const uint8_t init_value)
{
    DEV_ASSERT(p_sem != NULL);
    osif_disable_irq_global();
    *p_sem = init_value;
    osif_enable_irq_global();

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : osif_sema_destroy
 * Description   : This function destroys a semaphore object (mock operation in
 *                  baremetal case).
 *
 * Implements    : osif_sema_destroy_baremetal_activity
 *END**************************************************************************/
status_t
osif_sema_destroy(const semaphore_t *const p_sem)
{
    DEV_ASSERT(p_sem != NULL);

    (void)p_sem;

    return STATUS_SUCCESS;
}

/*******************************************************************************
 * Static functions
 ******************************************************************************/
#if defined(CUSTOM_SYSICK_IRQ)
#else
static inline void
osif_tick(void)
{
    s_osif_tick_cnt++;
}

static inline uint32_t
osif_get_current_tick_count(void)
{
    return s_osif_tick_cnt;
}

static inline void
osif_update_tick_config(void)
{
    uint32_t    core_freq  = 0u;
    static bool first_init = true;
    /* Get the correct name of the core clock */
    clock_names_t core_clk   = CORE_CLK;
    status_t      clk_status = clock_get_freq(core_clk, &core_freq);
    DEV_ASSERT(clk_status == STATUS_SUCCESS);
    DEV_ASSERT(core_freq > 0u);
    (void)clk_status;

    /* For Cortex-M0 devices the systick counter is initialized with an
     undefined value, so make sure to initialize it to 0 before starting */
    SysTick->CSR = SysTick_CSR_ENABLE(0u);
    SysTick->RVR = SysTick_RVR_RELOAD(core_freq / 1000u);
    if (first_init)
    {
        /* only initialize CVR on the first entry, to not cause time drift */
        SysTick->CVR = SysTick_CVR_CURRENT(0U);
        first_init   = false;
    }
    SysTick->CSR = SysTick_CSR_ENABLE(1u) | SysTick_CSR_TICKINT(1u) | SysTick_CSR_CLKSOURCE(1u);
}

static inline void
osif_disable_irq_global(void)
{
    int_disable_irq_global();
}

static inline void
osif_enable_irq_global(void)
{
    int_enable_irq_global();
}
#endif

/*** end of file ***/
