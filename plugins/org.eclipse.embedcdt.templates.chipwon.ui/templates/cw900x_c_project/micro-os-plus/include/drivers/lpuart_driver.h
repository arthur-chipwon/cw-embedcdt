/**
 * @file lpuart_driver.h
 * @brief Header file for the lpuart driver.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef LPUART_DRIVER_H
#define LPUART_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stddef.h>
#include "device_registers.h"
#include "lpuart_access.h"
#include "interrupt_manager.h"
#include "status.h"
#include "osif_driver.h"
#include "edma_driver.h"
#include "callbacks.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/** @brief Device instance number. */
#define INST_LPUART0 0u

/** @brief Device instance number. */
#define INST_LPUART1 1u

#define LPUART_SHIFT        (16U)
#define LPUART_BAUD_REG_ID  (1U)
#define LPUART_STAT_REG_ID  (2U)
#define LPUART_CTRL_REG_ID  (3U)
#define LPUART_DATA_REG_ID  (4U)
#define LPUART_MATCH_REG_ID (5U)
#define LPUART_MODIR_REG_ID (6U)
#define LPUART_FIFO_REG_ID  (7U)

/**
 * @brief LPUART status flags.
 *
 * This provides constants for the LPUART status flags for use in the UART functions.
 */
typedef enum
{
    LPUART_TX_COMPLETE =
        (((uint32_t)LPUART_STAT_REG_ID << (uint32_t)LPUART_SHIFT) | (uint32_t)LPUART_STAT_TC_SHIFT),
    LPUART_RX_OVERRUN =
        (((uint32_t)LPUART_STAT_REG_ID << (uint32_t)LPUART_SHIFT) | (uint32_t)LPUART_STAT_OR_SHIFT),
    LPUART_NOISE_DETECT =
        (((uint32_t)LPUART_STAT_REG_ID << (uint32_t)LPUART_SHIFT) | (uint32_t)LPUART_STAT_NF_SHIFT),
    LPUART_FRAME_ERR =
        (((uint32_t)LPUART_STAT_REG_ID << (uint32_t)LPUART_SHIFT) | (uint32_t)LPUART_STAT_FE_SHIFT),
    LPUART_PARITY_ERR =
        (((uint32_t)LPUART_STAT_REG_ID << (uint32_t)LPUART_SHIFT) | (uint32_t)LPUART_STAT_PF_SHIFT),
    LPUART_TX_DATA_REG_EMPTY = (((uint32_t)LPUART_STAT_REG_ID << (uint32_t)LPUART_SHIFT) |
                                (uint32_t)LPUART_STAT_TDRE_SHIFT),
    LPUART_RX_DATA_REG_FULL  = (((uint32_t)LPUART_STAT_REG_ID << (uint32_t)LPUART_SHIFT) |
                               (uint32_t)LPUART_STAT_RDRF_SHIFT),
    LPUART_IDLE_LINE_DETECT  = (((uint32_t)LPUART_STAT_REG_ID << (uint32_t)LPUART_SHIFT) |
                               (uint32_t)LPUART_STAT_IDLE_SHIFT),
    LPUART_MATCH_ADDR_ONE    = (((uint32_t)LPUART_STAT_REG_ID << (uint32_t)LPUART_SHIFT) |
                             (uint32_t)LPUART_STAT_MA1F_SHIFT),
    LPUART_MATCH_ADDR_TWO    = (((uint32_t)LPUART_STAT_REG_ID << (uint32_t)LPUART_SHIFT) |
                             (uint32_t)LPUART_STAT_MA2F_SHIFT),
    LPUART_FIFO_TX_OF        = (((uint32_t)LPUART_FIFO_REG_ID << (uint32_t)LPUART_SHIFT) |
                         (uint32_t)LPUART_FIFO_TXOF_SHIFT),
    LPUART_FIFO_RX_UF        = (((uint32_t)LPUART_FIFO_REG_ID << (uint32_t)LPUART_SHIFT) |
                         (uint32_t)LPUART_FIFO_RXUF_SHIFT),
    LPUART_ALL_STATUS        = 0xC01FC000u /**< Used for clearing all w1c status flags */
} lpuart_status_flag_t;

/** @brief LPUART interrupt configuration structure, default settings are 0(disabled) */
typedef enum
{
    LPUART_INT_TX_DATA_REG_EMPTY = (((uint32_t)LPUART_CTRL_REG_ID << (uint32_t)LPUART_SHIFT) |
                                    (uint32_t)LPUART_CTRL_TIE_SHIFT),
    LPUART_INT_TX_COMPLETE       = (((uint32_t)LPUART_CTRL_REG_ID << (uint32_t)LPUART_SHIFT) |
                              (uint32_t)LPUART_CTRL_TCIE_SHIFT),
    LPUART_INT_RX_DATA_REG_FULL  = (((uint32_t)LPUART_CTRL_REG_ID << (uint32_t)LPUART_SHIFT) |
                                   (uint32_t)LPUART_CTRL_RIE_SHIFT),
    LPUART_INT_IDLE_LINE         = (((uint32_t)LPUART_CTRL_REG_ID << (uint32_t)LPUART_SHIFT) |
                            (uint32_t)LPUART_CTRL_ILIE_SHIFT),
    LPUART_INT_RX_OVERRUN        = (((uint32_t)LPUART_CTRL_REG_ID << (uint32_t)LPUART_SHIFT) |
                             (uint32_t)LPUART_CTRL_ORIE_SHIFT),
    LPUART_INT_NOISE_ERR_FLAG    = (((uint32_t)LPUART_CTRL_REG_ID << (uint32_t)LPUART_SHIFT) |
                                 (uint32_t)LPUART_CTRL_NEIE_SHIFT),
    LPUART_INT_FRAME_ERR_FLAG    = (((uint32_t)LPUART_CTRL_REG_ID << (uint32_t)LPUART_SHIFT) |
                                 (uint32_t)LPUART_CTRL_FEIE_SHIFT),
    LPUART_INT_PARITY_ERR_FLAG   = (((uint32_t)LPUART_CTRL_REG_ID << (uint32_t)LPUART_SHIFT) |
                                  (uint32_t)LPUART_CTRL_PEIE_SHIFT),
    LPUART_INT_MATCH_ADDR_ONE    = (((uint32_t)LPUART_CTRL_REG_ID << (uint32_t)LPUART_SHIFT) |
                                 (uint32_t)LPUART_CTRL_MA1IE_SHIFT),
    LPUART_INT_MATCH_ADDR_TWO    = (((uint32_t)LPUART_CTRL_REG_ID << (uint32_t)LPUART_SHIFT) |
                                 (uint32_t)LPUART_CTRL_MA2IE_SHIFT),
    LPUART_INT_FIFO_TXOF         = (((uint32_t)LPUART_FIFO_REG_ID << (uint32_t)LPUART_SHIFT) |
                            (uint32_t)LPUART_FIFO_TXOFE_SHIFT),
    LPUART_INT_FIFO_RXUF         = (((uint32_t)LPUART_FIFO_REG_ID << (uint32_t)LPUART_SHIFT) |
                            (uint32_t)LPUART_FIFO_RXUFE_SHIFT)
} lpuart_interrupt_t;

typedef enum
{
    LPUART_USING_DMA = 0,   /**< use DMA to perform UART transfer */
    LPUART_USING_INTERRUPTS /**< use interrupts to perform UART transfer */
} lpuart_transfer_type_t;

/**
 * @brief LPUART number of bits in a character
 *
 * Implements : lpuart_bit_count_per_char_t_Class
 */
typedef enum
{
    LPUART_8_BITS_PER_CHAR  = 0x0U, /**< 8-bit data characters */
    LPUART_9_BITS_PER_CHAR  = 0x1U, /**< 9-bit data characters */
    LPUART_10_BITS_PER_CHAR = 0x2U  /**< 10-bit data characters */
} lpuart_bit_count_per_char_t;

/**
 * @brief LPUART parity mode
 *
 * Implements : lpuart_parity_mode_t_Class
 */
typedef enum
{
    LPUART_PARITY_DISABLED = 0x0U, /**< parity disabled */
    LPUART_PARITY_EVEN     = 0x2U, /**< type even, bit setting: PE|PT = 10 */
    LPUART_PARITY_ODD      = 0x3U  /**< type odd,  bit setting: PE|PT = 11 */
} lpuart_parity_mode_t;

/*******************************************************************************
 * Exported variables
 ******************************************************************************/
/** @brief Table of base pointers for LPUART instances. */
extern LPUART_t *const gp_lpuart_base[LPUART_INSTANCE_COUNT];

/** @brief Table to save LPUART IRQ enumeration numbers defined in the CMSIS
 * header file. */
extern const IRQn_t g_lpuart_rx_tx_irq_id[LPUART_INSTANCE_COUNT];

/** @brief Table to save LPUART clock names as defined in clock manager. */
extern const clock_names_t g_lpuart_clk_names[LPUART_INSTANCE_COUNT];

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Initializes the LPUART controller.
 *
 * This function Initializes the LPUART controller to known state.
 *
 * @param[in] p_base LPUART base pointer.
 */
void lpuart_init_set(LPUART_t *p_base);

/**
 * @brief Configures the number of bits per character in the LPUART controller.
 *
 * This function configures the number of bits per character in the LPUART
 * controller. In some LPUART instances, the user should disable the
 * transmitter/receiver before calling this function. Generally, this may be
 * applied to all LPUARTs to ensure safe operation.
 *
 * @param[in] p_base LPUART base pointer.
 * @param[in] bit_count_per_char  Number of bits per char (8, 9, or 10, depending on the LPUART
 * instance)
 * @param[in] b_parity  Specifies whether parity bit is enabled
 */
void lpuart_set_bit_count_per_char(LPUART_t                   *p_base,
                                   lpuart_bit_count_per_char_t bit_count_per_char,
                                   bool                        b_parity);

/**
 * @brief Configures parity mode in the LPUART controller.
 *
 * This function configures parity mode in the LPUART controller.
 * In some LPUART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all LPUARTs to ensure safe operation.
 *
 * @param[in] p_base LPUART base pointer.
 * @param[in] parity_mode_type  Parity mode (enabled, disable, odd, even - see parity_mode_t struct)
 */
void lpuart_set_parity_mode(LPUART_t *p_base, lpuart_parity_mode_t parity_mode_type);

/**
 * @brief Configures the LPUART module interrupts.
 *
 * This function configures the LPUART module interrupts to enable/disable
 * various interrupt sources.
 *
 * @param[in]   p_base LPUART module base pointer.
 * @param[in]   int_src LPUART interrupt configuration data.
 * @param[in]   b_enable   true: enable, false: disable.
 */
void lpuart_set_int_mode(LPUART_t *p_base, lpuart_interrupt_t int_src, bool b_enable);

/**
 * @brief Returns LPUART module interrupts state.
 *
 * This function returns whether a certain LPUART module interrupt is enabled or disabled.
 *
 * @param[in]   p_base LPUART module base pointer.
 * @param[in]   int_src LPUART interrupt configuration data.
 * @return  true: enable, false: disable.
 */
bool lpuart_get_int_mode(const LPUART_t *p_base, lpuart_interrupt_t int_src);

/**
 * @brief Sends the LPUART 9-bit character.
 *
 * This functions sends a 9-bit character.
 *
 * @param[in] p_base LPUART Instance
 * @param[in] data data to send (9-bit)
 */
void lpuart_put_char9(LPUART_t *p_base, uint16_t data);

/**
 * @brief Sends the LPUART 10-bit character (Note: Feature available on select LPUART instances).
 *
 * This functions sends a 10-bit character.
 *
 * @param[in] p_base LPUART Instance
 * @param[in] data data to send (10-bit)
 */
void lpuart_put_char10(LPUART_t *p_base, uint16_t data);

/**
 * @brief Gets the LPUART 9-bit character.
 *
 * This functions receives a 9-bit character.
 *
 * @param[in] p_base LPUART base pointer
 * @param[out] p_read_data Data read from receive (9-bit)
 */
void lpuart_get_char9(const LPUART_t *p_base, uint16_t *p_read_data);

/**
 * @brief Gets the LPUART 10-bit character.
 *
 * This functions receives a 10-bit character.
 *
 * @param[in] p_base LPUART base pointer
 * @param[out] p_read_data Data read from receive (10-bit)
 */
void lpuart_get_char10(const LPUART_t *p_base, uint16_t *p_read_data);

/**
 * @brief Gets the LUART status bitmap state.
 *
 * This function returns the state of bitmap of the LPUART status at the moment of intrrupt event.
 *
 * @param[in] p_base Module base pointer of type LPUART_t.
 * @return State of bitmap of the status.
 */
uint32_t lpuart_get_status_bm(LPUART_t *p_base);

/**
 * @brief Clears the LPUART status bitmap state.
 *
 * @param[in] p_base Module base pointer of type LPUART_t.
 * @param[in] status_bm bitmap for w1c interrupt event.
 */
void lpuart_clear_status_bm(LPUART_t *p_base, uint32_t status_bm);

/**
 * @brief  Gets the PUART status flag
 *
 * This function returns the state of a status flag.
 *
 * @param[in] p_base LPUART base pointer
 * @param[in] status_flag  The status flag to query
 * @return Whether the current status flag is set(true) or not(false).
 */
bool lpuart_get_status_flag(const LPUART_t *p_base, lpuart_status_flag_t status_flag);

/**
 * @brief LPUART clears an individual status flag.
 *
 * This function clears an individual status flag (see lpuart_status_flag_t for
 * list of status bits).
 *
 * @param[in] p_base LPUART base pointer
 * @param[in] status_flag  Desired LPUART status flag to clear
 * @return STATUS_SUCCESS if successful or STATUS_ERROR if an error occured
 */
status_t lpuart_clear_status_flag(LPUART_t *p_base, lpuart_status_flag_t status_flag);

/**
 * @brief Enable or disable the LPUART error interrupts.
 *
 * This function configures the error interrupts (parity, noise, overrun, framing).
 *
 * @param[in] p_base LPUART base pointer
 * @param[in] b_enable true - enable, false - disable error interrupts
 */
void lpuart_set_error_interrupts(LPUART_t *p_base, bool b_enable);

#ifdef __cplusplus
}
#endif

#endif /* LPUART_DRIVER_H */

/*** end of file ***/
