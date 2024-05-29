/**
 * @file callbacks.h
 * @brief The header defines callback types.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef CALLBACKS_H
#define CALLBACKS_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/

/**
 * @brief Define the enum of the events which can trigger I2C slave callback
 *
 * This enum should include the events for all platforms
 */
typedef enum
{
    I2C_SLAVE_EVENT_RX_FULL  = 0x00U,
    I2C_SLAVE_EVENT_TX_EMPTY = 0x01U,
    I2C_SLAVE_EVENT_TX_REQ   = 0x02U,
    I2C_SLAVE_EVENT_RX_REQ   = 0x03U,
    I2C_SLAVE_EVENT_STOP     = 0X04U,
} i2c_slave_event_t;

/**
 * @brief Define the enum of the events which can trigger I2C master callback
 *
 * This enum should include the events for all platforms
 */
typedef enum
{
    I2C_MASTER_EVENT_END_TRANSFER = 0x4U,
} i2c_master_event_t;

/**
 *
 * This enum should include the events for all platforms
 */
typedef enum
{
    SPI_EVENT_END_TRANSFER = 0
} spi_event_t;

/**
 * @brief Define the enum of the events which can trigger UART callback
 *
 * This enum should include the events for all platforms
 *
 * Implements : uart_event_t_Class
 */
typedef enum
{
    UART_EVENT_RX_FULL      = 0x00U, /**< Rx buffer is full */
    UART_EVENT_TX_EMPTY     = 0x01U, /**< Tx buffer is empty */
    UART_EVENT_END_TRANSFER = 0x02U, /**< The current transfer is ending */
    UART_EVENT_ERROR        = 0x03U, /**< An error occured during transfer */
} uart_event_t;

/**
 * @brief Define the enum of the events which can trigger the output compare callback
 */
typedef enum
{
    OC_EVENT_GENERATION_OUTPUT_COMPLETE = 0x00U /**< Generation output signal is completed */
} oc_event_t;

/**
 * @brief Define the enum of the events which can trigger the input capture callback
 */
typedef enum
{
    IC_EVENT_MEASUREMENT_COMPLETE = 0x00U /**< Capture input signal is completed */
} ic_event_t;

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/**
 * @brief Callback for all peripherals which supports I2C features for slave mode
 */
typedef void (*i2c_slave_callback_t)(i2c_slave_event_t event, void *p_user_data);

/**
 * @brief Callback for all peripherals which supports I2C features for master mode
 */
typedef void (*i2c_master_callback_t)(i2c_master_event_t event, void *p_user_data);

/**
 * @brief Callback for all peripherals which supports SPI features */
typedef void (*spi_callback_t)(void *p_driver_state, spi_event_t event, void *p_user_data);

/**
 * @brief Callback for all peripherals which support UART features
 * Implements : uart_callback_t_Class
 */
typedef void (*uart_callback_t)(void *p_driver_state, uart_event_t event, void *p_user_data);

/**
 * @brief Callback for all peripherals which support TIMING features
 */
typedef void (*timer_callback_t)(void *p_user_data);

/**
 * @brief Callback for security modules
 * Implements : security_callback_t_Class
 */
typedef void (*security_callback_t)(uint32_t completed_cmd, void *p_callback_param);

/**
 * @brief Callback for all peripherals which support OC feature
 */
typedef void (*oc_callback_t)(oc_event_t event, void *p_user_data);

/**
 * @brief Callback for all peripherals which support IC feature
 */
typedef void (*ic_callback_t)(ic_event_t event, void *p_user_data);

#ifdef __cplusplus
}
#endif

#endif /* CALLBACKS_H */

/*** end of file ***/
