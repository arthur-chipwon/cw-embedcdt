/**
 * @file status.h
 * @brief Header file for status return codes.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef STATUS_H
#define STATUS_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/** @brief Status return codes.
 * Common error codes will be a unified enumeration (C enum) that will
 * contain all error codes (common and specific). There will be separate
 * "error values spaces" (or slots), each of 256 positions, allocated per
 * functionality.
 */
typedef enum
{
    /* Generic error codes */
    STATUS_SUCCESS     = 0x000U, /**< Success status */
    STATUS_ERROR       = 0x001U, /**< Failure status */
    STATUS_BUSY        = 0x002U, /**< Busy status */
    STATUS_TIMEOUT     = 0x003U, /**< Timeout status */
    STATUS_UNSUPPORTED = 0x004U, /**< Unsupported status */
    /* MCU specific error codes */
    STATUS_MCU_GATED_OFF           = 0x100U, /**< Module is gated off */
    STATUS_MCU_TRANSITION_FAILED   = 0x101U, /**< Transition Error. */
    STATUS_MCU_INVALID_STATE       = 0x102U, /**< Unsupported in current state. */
    STATUS_MCU_NOTIFY_BEFORE_ERROR = 0x103U, /**< Notification Error. */
    STATUS_MCU_NOTIFY_AFTER_ERROR  = 0x104U, /**< Notification Error. */
    /* I2C specific error codes */
    STATUS_I2C_RECEIVED_NACK    = 0x200U, /**< NACK signal received  */
    STATUS_I2C_TX_UNDERRUN      = 0x201U, /**< TX underrun error */
    STATUS_I2C_RX_OVERRUN       = 0x202U, /**< RX overrun error */
    STATUS_I2C_ARBITRATION_LOST = 0x203U, /**< Arbitration lost */
    STATUS_I2C_ABORTED          = 0x204U, /**< A transfer was aborted */
    STATUS_I2C_BUS_BUSY         = 0x205U, /**< Bus Busy */
    /* SPI specific error codes */
    STATUS_SPI_TX_UNDERRUN = 0x500U, /**< TX underrun error */
    STATUS_SPI_RX_OVERRUN  = 0x501U, /**< RX overrun error */
    STATUS_SPI_ABORTED     = 0x502U, /**< A transfer was aborted */
    /* UART specific error codes */
    STATUS_UART_TX_UNDERRUN   = 0x600U, /**< TX underrun error */
    STATUS_UART_RX_OVERRUN    = 0x601U, /**< RX overrun error */
    STATUS_UART_ABORTED       = 0x602U, /**< A transfer was aborted */
    STATUS_UART_FRAMING_ERROR = 0x603U, /**< Framing error */
    STATUS_UART_PARITY_ERROR  = 0x604U, /**< Parity error */
    STATUS_UART_NOISE_ERROR   = 0x605U, /**< Noise error */
    /* SBC specific error codes */
    SBC_NVN_ERROR = 0x801U,    /**< Unsuccessful attempt writing to non volatile memory
                                    (0x73 and 0x74). Set device to factory settings. */
    SBC_COMM_ERROR   = 0x802U, /**< Data transfer was aborted */
    SBC_CMD_ERROR    = 0x804U, /**< Wrong command. */
    SBC_ERR_NA       = 0x808U, /**< Feature/device not available */
    SBC_MTPNV_LOCKED = 0x810U, /**< Unable to write MTPNV cells, NVMPS = 0 */
    /* FLASH specific error codes */
    STATUS_FLASH_ERROR_ENABLE   = 0x901U, /**< It's impossible to enable an operation */
    STATUS_FLASH_ERROR_NO_BLOCK = 0x902U, /**< No blocks have been enabled */
    STATUS_FLASH_INPROGRESS     = 0x903U, /**< InProgress status */
} status_t;

#ifdef __cplusplus
}
#endif

#endif /* STATUS_H */

/*** end of file ***/
