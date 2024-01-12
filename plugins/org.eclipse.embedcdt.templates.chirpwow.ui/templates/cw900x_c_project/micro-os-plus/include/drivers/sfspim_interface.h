/**
 * @file sfspim_interface.h
 * @brief An implementation of helper functions for serial flash SPI master interface.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef SFSPIM_INTERFACE_H
#define SFSPIM_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "lpspi_driver.h"
#include "interrupt_manager.h"
#include "osif_driver.h"
#include "callbacks.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/**
 * @brief Due to TCR is relative to internal hardcore states, so there is asynchronous timing
 * difference between firmware layer and RTL layer. A method offers an workaround solution for
 * aoviding double register write in runtime scheduling.
 */
// #define _SPI_TCR_NONCOMMON_REG_

/** @brief Interface instance number. */
#define INST_SFSPIM INST_LPSPI0

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
typedef enum
{
    SFSPIM_USING_DMA = 0,    /*!< The driver will use DMA to perform SPI transfer */
    SFSPIM_USING_INTERRUPTS, /*!< The driver will use interrupts to perform SPI transfer */
} sfspim_xfer_type_t;

typedef enum
{
    SFSPIM_TRANSFER_OK = 0U, /*!< Transfer OK */
    SFSPIM_TRANSMIT_FAIL,    /*!< Error during transmission */
    SFSPIM_RECEIVE_FAIL      /*!< Error during reception */
} sfspim_xfer_status_t;

typedef enum
{
    SFSPIM_KICKOFF = 0U,           /*!<  */
    SFSPIM_CMD_PHASE,              /*!<  */
    SFSPIM_ADDR_PHASE,             /*!<  */
    SFSPIM_DUMMY_PHASE,            /*!<  */
    SFSPIM_DATA_PHASE,             /*!<  */
    SFSPIM_POSTPONE_EVENT,         /*!<	*/
    SFSPIM_PAUSE_TRANSMISSION,     /*!<  */
    SFSPIM_RESTART_TX_COUNTING,    /*!<	*/
    SFSPIM_AWAIT_TX_COMPLETED,     /*!<	*/
    SFSPIM_TRANSIT_TO_DUMMY_PHASE, /*!< */
    SFSPIM_TRANSIT_TO_DATA_PHASE,  /*!< */
    SFSPIM_TRANSIT_TO_STOP_PHASE,  /*!< */
    SFSPIM_STOP_PHASE,             /*!<  */
} sfspim_sched_sm_t;

/*!
 * @brief Data structure containing information about a device on the SPI bus.
 *
 * The user must populate these members to set up the LPSPI master and
 * properly communicate with the SPI device.
 * Implements : lpspi_master_config_t_Class
 */
typedef struct
{
    uint32_t                baud_rate;         /*!< Baud rate in bits per second*/
    lpspi_which_pcs_t       which_pcs;         /*!< Selects which PCS to use */
    lpspi_signal_polarity_t pcs_polarity;      /*!< PCS polarity */
    bool                    b_pcs_continuous;  /*!< Keeps PCS asserted until transfer complete */
    bool                    b_alt_pcs;         /*!< An alternative internal PCS drive  */
    bool                    b_indep_data_wire; /*!< Independent two wires DI/DO  */
    bool                    b_full_duplex;     /*!< Full-duplex mode */
    lpspi_transfer_width_t  xfer_width; /*!< Transfer width, single, 2-bit, or 4-bit transfer. */
    uint16_t                bits_per_frame; /*!< Number of bits/frame, minimum is 8-bits */
    lpspi_clock_phase_t     clk_phase;      /*!< Selects which phase of clock to capture data */
    lpspi_sck_polarity_t    clk_polarity;   /*!< Selects clock polarity */
    bool                    b_lsb_first;    /*!< Option to transmit LSB first */
    sfspim_xfer_type_t      xfer_type;      /*!< Type of LPSPI transfer */
    uint8_t rx_dma_channel;  /*!< Channel number for DMA rx channel. If DMA mode isn't used this
                              field  will be ignored. */
    uint8_t tx_dma_channel;  /*!< Channel number for DMA tx channel. If DMA mode isn't used this
                              field  will be ignored. */
    spi_callback_t callback; /*!< Select the callback to transfer complete */
    void          *p_callback_param; /*!< Select additional callback parameters if it's necessary */
} sfspim_config_t;

/*!
 * @brief Runtime state structure for the LPSPI master driver.
 *
 * This structure holds data that is used by the LPSPI peripheral driver to
 * communicate between the transfer function and the interrupt handler. The
 * interrupt handler also uses this information to keep track of its progress.
 * The user must pass  the memory for this run-time state structure. The
 * LPSPI master driver populates the members.
 * Implements : lpspi_state_t_Class
 */
typedef struct
{
    LPSPI_t             *p_base;
    const IRQn_t        *p_irq_id;
    const clock_names_t *p_clk_name;
    uint16_t             bits_per_frame; /*!< Number of bits per frame: 8- to 4096-bits; needed for
                 TCR programming */
    uint16_t                bytes_per_frame; /*!< Number of bytes per frame: 1- to 512-bytes */
    lpspi_signal_polarity_t pcs_polarity;    /*!< PCS polarity */
    bool b_pcs_continuous;                   /*!< Option to keep chip select asserted until transfer
                       complete; needed for TCR programming */
    bool              b_alt_pcs;             /*!< An alternative internal PCS drive  */
    bool              b_keep_continuing;     /*!< Keep continuous */
    bool              b_blocking;            /*!< Save the transfer type */
    bool              b_indep_data_wire;     /*!< Independent two wires DI/DO  */
    bool              b_full_duplex;         /*!< Full-duplex mode */
    uint32_t          src_clk_freq;          /*!< Module source clock */
    volatile bool     b_xfer_in_progress;    /*!< True if there is an active transfer */
    const uint8_t    *p_tx_buffer;    /*!< The buffer from which transmitted bytes are taken */
    uint8_t          *p_rx_buffer;    /*!< The buffer into which received bytes are placed */
    volatile uint16_t tx_remain_cntr; /*!< Number of bytes remaining to send  */
    volatile uint16_t rx_remain_cntr; /*!< Number of bytes remaining to receive */
    volatile uint16_t
        tx_frame_cntr; /*!< Number of bytes from current frame which were already sent */
    volatile uint16_t
        rx_frame_cntr; /*!< Number of bytes from current frame which were already received */
    volatile bool        b_lsb;     /*!< True if first bit is LSB and false if first bit is MSB */
    uint8_t              fifo_size; /*!< RX/TX fifo size */
    uint8_t              rx_dma_channel;     /*!< Channel number for DMA rx channel */
    uint8_t              tx_dma_channel;     /*!< Channel number for DMA tx channel */
    sfspim_xfer_type_t   xfer_type;          /*!< Type of LPSPI transfer */
    semaphore_t          sema_blocking_xfer; /*!< The semaphore used for blocking transfers */
    sfspim_xfer_status_t xfer_status;        /*!< The status of the current */
    spi_callback_t       callback;           /*!< Select the callback to transfer complete */
    void *p_callback_param; /*!< Select additional callback parameters if it's necessary */
    uint32_t
          dummy; /*!< This field is used for the cases when TX is NULL and LPSPI is in DMA mode */
    isr_t pfunc_primary_isr;
    lpspi_tx_cmd_config_t      tx_cmd_cfg;
    volatile uint32_t          reg_tcr_persist;
    volatile bool              b_access_read;
    volatile bool              b_tx_frobidden;
    volatile sfspim_sched_sm_t state_machine;
    volatile uint8_t           sig_cmd_len;  /**< payload transfer length of significiant command */
    volatile uint8_t           sig_addr_len; /**< payload transfer length of significiant address */
    volatile uint8_t           dummy_len;    /**< payload transfer length for dummy transmission */
    volatile uint16_t          data_len;     /**< payload transfer length for data transmission */
    volatile uint16_t          tx_prog_cntr; /**< real time progress counter for transmitter */
    volatile uint16_t          rx_prog_cntr; /**< reai time progress counter for receiver */
    volatile uint16_t          tx_mute_cnt;  /**< mute byte count on transmitting */
    volatile uint16_t          rx_mute_cnt;  /**< mute byte count on receiving */
    uint8_t                    cmd_buf[4];   /*!<  */
    uint8_t                    addr_buf[4];  /*!<  */
    uint8_t                    dummy_symbol; /*!<  */
    uint8_t                   *p_data_buf_ptr; /*!<  */
} sfspim_state_t;

typedef struct _sfspim_cmd_wrapper_t
{
    bool     b_cmd_single_wire; /*!<  */
    bool     b_access_read;     /*!<  */
    bool     b_high_z_at_dummy; /*!<  */
    uint8_t  sig_cmd_len;       /**< payload transfer length of significiant command */
    uint8_t  sig_addr_len;      /**< payload transfer length of significiant address */
    uint8_t  dummy_len;         /*!<  */
    uint16_t data_len;          /*!<  */
    uint32_t sig_cmd;           /*!<  */
    uint32_t sig_addr;          /*!<  */
    uint8_t  dummy_symbol;      /*!<  */
    uint8_t *p_data_buf_ptr;    /*!<  */
} sfspim_cmd_wrapper_t;

/*******************************************************************************
 * Exported variables
 ******************************************************************************/
/** Define state structure for current serial flash SPI master instance */
extern sfspim_state_t g_sfspim_state;

/** SFSPIM LPSPI0 master configurations */
extern const sfspim_config_t g_sfspim_init_config;

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
status_t sfspim_init(uint32_t               instance,
                     sfspim_state_t        *p_state_ptr,
                     const sfspim_config_t *p_init_config);
status_t sfspim_deinit(void);
status_t sfspim_cmd_wrap_passthrough(sfspim_cmd_wrapper_t *p_cmd_wrap, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* SFSPIM_INTERFACE_H */

/*** end of file ***/
