/**
 * @file uart_interface.h
 * @brief An application interface for UART bus.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef UART_INTERFACE_H
#define UART_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "lpuart_driver.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/** @brief Interface instance number. */
#define INST_UARTIF INST_LPUART0

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/**
 * @brief LPUART configuration structure
 *
 * Implements : lpuart_user_config_t_Class
 */
typedef struct
{
    uint32_t             baud_rate;   /*!< LPUART baud rate */
    lpuart_parity_mode_t parity_mode; /*!< parity mode, disabled (default), even, odd */
    lpuart_stop_bit_count_t
        stop_bit_count; /*!< number of stop bits, 1 stop bit (default) or 2 stop bits */
    lpuart_bit_count_per_char_t bit_count_per_char; /*!< number of bits in a character (8-default, 9
     or 10); for 9/10 bits chars, users must provide appropriate buffers to the send/receive
     functions (bits 8/9 in subsequent bytes); for DMA transmission only 8-bit char is supported. */
    lpuart_transfer_type_t transfer_type;  /*!< Type of LPUART transfer (interrupt/dma based) */
    uint8_t                rx_dma_channel; /*!< Channel number for DMA rx channel.
                    If DMA mode isn't used this field will be ignored. */
    uint8_t tx_dma_channel;                /*!< Channel number for DMA tx channel.
                    If DMA mode isn't used this field will be ignored. */
} uartif_config_t;

/**
 * @brief Runtime state of the LPUART driver.
 *
 * Note that the caller provides memory for the driver state structures during
 * initialization because the driver does not statically allocate memory.
 *
 * Implements : lpuart_state_t_Class
 */
typedef struct
{
    LPUART_t            *p_base;
    const IRQn_t        *p_tx_rx_irq_id;
    const clock_names_t *p_clk_name;
    volatile bool        b_tx_int_mute;
    volatile bool        b_rx_int_mute;
    const uint8_t       *p_tx_ring_buf; /*!< The buffer of data being sent.*/
    uint32_t
        tx_size_alloc; /*!< The allocated number of bytes to be used to transmitter ring buffer. */
    uint8_t *volatile p_tx_front_ptr;
    uint8_t *volatile p_tx_rear_ptr;
    uint8_t *p_tx_top_ptr;
    uint8_t *p_tx_bottom_ptr;
    uint8_t *p_rx_ring_buf; /*!< The buffer of received data.*/
    uint32_t
        rx_size_alloc; /*!< The allocated number of bytes to be used to receiver ring buffer. */
    uint8_t *volatile p_rx_front_ptr;
    uint8_t *volatile p_rx_rear_ptr;
    uint8_t                    *p_rx_top_ptr;
    uint8_t                    *p_rx_bottom_ptr;
    lpuart_bit_count_per_char_t bit_count_per_char; /*!< number of bits in a char (8/9/10) */
    lpuart_transfer_type_t      transfer_type; /*!< Type of LPUART transfer (interrupt/dma based) */
} uartif_state_t;

/*******************************************************************************
 * Exported variables
 ******************************************************************************/
/**
 * @brief A specified UART interface runtime state variable.
 */
extern uartif_state_t g_uartif_state;

/* External declaration of LPUART configuration structure */
extern const uartif_config_t g_uartif_init_config;

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes(excluding global inline function prototypes,
 * C99 compliance)
 ******************************************************************************/
status_t uartif_init(uint32_t               instance,
                     uartif_state_t        *p_state_ptr,
                     const uartif_config_t *p_user_config);
status_t uartif_deinit(void);
void     uartif_install_tx_ring_buffer(uint8_t *p_buf, uint32_t buf_size);
void     uartif_install_rx_ring_buffer(uint8_t *p_buf, uint32_t buf_size);
void     uartif_print_format(const char *p_fmt, ...);
void     uartif_print_buffer(const uint8_t *p_buf, uint32_t buf_size);
void     uartif_print_await_done(void);
bool     uartif_acquire_new_arrival_char(uint32_t buf_size, uint32_t *p_size_acq, uint8_t *p_buf);
void     uartif_tx_put_char(uint8_t letter);

#ifdef __cplusplus
}
#endif

#endif /* UART_INTERFACE_H */

/*** end of file ***/
