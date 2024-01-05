/**
 * @file osif_driver.h
 * @brief Header file for the osif driver.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef OSIF_H
#define OSIF_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include "status.h"
#include "device_registers.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
#define OSIF_WAIT_FOREVER 0xFFFFFFFFu   /**< Time to wait for OSIF */

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/* Bare-metal implementation */
/** @brief Type for a mutex. */
typedef uint8_t mutex_t;
/** @brief Type for a semaphore. */
typedef volatile uint8_t semaphore_t;

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Delays execution for a number of milliseconds.
 *
 * @param[in] delay Time delay in milliseconds.
 */
void osif_time_delay(const uint32_t delay);

/**
 * @brief Returns the number of miliseconds elapsed since starting the internal
 * timer or starting the scheduler.
 *
 * @return the number of miliseconds elapsed
 */
uint32_t osif_get_milliseconds(void);

/**
 * @brief Waits for a mutex and locks it.
 *
 * @param[in] p_mutex reference to the mutex object
 * @param[in] timeout time-out value in milliseconds
 * @return  One of the possible status codes:
 *              - STATUS_SUCCESS: mutex lock operation success
 *              - STATUS_ERROR: mutex already owned by current thread
 *              - STATUS_TIMEOUT: mutex lock operation timed out
 *
 */
status_t osif_mutex_lock(const mutex_t *const p_mutex, const uint32_t timeout);

/**
 * @brief Unlocks a previously locked mutex.
 *
 * @param[in] p_mutex reference to the mutex object
 * @return  One of the possible status codes:
 *              - STATUS_SUCCESS: mutex unlock operation success
 *              - STATUS_ERROR: mutex unlock failed
 */
status_t osif_mutex_unlock(const mutex_t *const p_mutex);

/**
 * @brief Create an unlocked mutex.
 *
 * @param[in] p_mutex reference to the mutex object
 * @return  One of the possible status codes:
 *              - STATUS_SUCCESS: mutex created
 *              - STATUS_ERROR: mutex could not be created
 */
status_t osif_mutex_create(mutex_t *const p_mutex);

/**
 * @brief Destroys a previously created mutex.
 *
 * @param[in] p_mutex reference to the mutex object
 * @return  One of the possible status codes:
 *              - STATUS_SUCCESS: mutex destroyed
 */
status_t osif_mutex_destroy(const mutex_t *const p_mutex);

/**
 * @brief Decrement a semaphore with timeout.
 *
 * @param[in] p_sem reference to the semaphore object
 * @param[in] timeout time-out value in milliseconds
 * @return  One of the possible status codes:
 *              - STATUS_SUCCESS: semaphore wait operation success
 *              - STATUS_TIMEOUT: semaphore wait timed out
 */
status_t osif_sema_wait(semaphore_t *const p_sem, const uint32_t timeout);

/**
 * @brief Increment a semaphore
 *
 * @param[in] p_sem reference to the semaphore object
 * @return  One of the possible status codes:
 *              - STATUS_SUCCESS: semaphore post operation success
 *              - STATUS_ERROR: semaphore could not be incremented
 */
status_t osif_sema_post(semaphore_t *const p_sem);

/**
 * @brief Creates a semaphore with a given value.
 *
 * @param[in] p_sem reference to the semaphore object
 * @param[in] init_value initial value of the semaphore
 * @return  One of the possible status codes:
 *              - STATUS_SUCCESS: semaphore created
 *              - STATUS_ERROR: semaphore could not be created
 */
status_t osif_sema_create(semaphore_t *const p_sem, const uint8_t init_value);

/**
 * @brief Destroys a previously created semaphore.
 *
 * @param[in] p_sem reference to the semaphore object
 * @return  One of the possible status codes:
 *              - STATUS_SUCCESS: semaphore destroyed
 */
status_t osif_sema_destroy(const semaphore_t *const p_sem);

#ifdef __cplusplus
}
#endif

#endif /* OSIF_H */

/*** end of file ***/
