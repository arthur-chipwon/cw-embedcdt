/**
 * @file lpspi_interface.h
 * @brief An application interface for SPI bus.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef LPSPI_INTERFACE_H
#define LPSPI_INTERFACE_H

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
#include "osif_driver.h"
#include "callbacks.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/*! @brief Type of LPSPI transfer (based on interrupts or DMA).
 * Implements : lpspi_transfer_type_Class
 */
typedef enum
{
    LPSPIIF_USING_DMA = 0,    /*!< The driver will use DMA to perform SPI transfer */
    LPSPIIF_USING_INTERRUPTS, /*!< The driver will use interrupts to perform SPI
                               transfer */
} lpspiif_xfer_type_t;

/*! @brief Type of error reported by LPSPI
 */
typedef enum
{
    LPSPIIF_TRANSFER_OK = 0U, /*!< Transfer OK */
    LPSPIIF_TRANSMIT_FAIL,    /*!< Error during transmission */
    LPSPIIF_RECEIVE_FAIL      /*!< Error during reception */
} lpspiif_xfer_status_t;

/*!
 * @brief Data structure containing information about a device on the SPI bus.
 *
 * The user must populate these members to set up the LPSPI master and
 * properly communicate with the SPI device.
 * Implements : lpspi_master_config_t_Class
 */
typedef struct
{
    uint32_t                bits_per_sec;     /*!< Baud rate in bits per second*/
    lpspi_which_pcs_t       which_pcs;        /*!< Selects which PCS to use */
    lpspi_signal_polarity_t pcs_polarity;     /*!< PCS polarity */
    bool                    b_pcs_continuous; /*!< Keeps PCS asserted until transfer complete */
    uint16_t                bits_per_frame;   /*!< Number of bits/frame, minimum is 8-bits */
    lpspi_clock_phase_t     clk_phase;        /*!< Selects which phase of clock to capture data */
    lpspi_sck_polarity_t    clk_polarity;     /*!< Selects clock polarity */
    bool                    b_lsb_first;      /*!< Option to transmit LSB first */
    bool                    b_byte_swap;      /*!< Option to transmit byte ordering */
    lpspiif_xfer_type_t     xfer_type;        /*!< Type of LPSPI transfer */
    uint8_t                 rx_dma_channel;   /*!< Channel number for DMA rx channel. If DMA mode
                                               isn't used this field will be ignored. */
    uint8_t tx_dma_channel;                   /*!< Channel number for DMA tx channel. If DMA mode
                                               isn't used this field will be ignored. */
    spi_callback_t callback;                  /*!< Select the callback to transfer complete */
    void          *p_callback_param;          /*!< Select additional callback parameters if it's
                                              necessary */
} lpspiif_master_config_t;

/*!
 *  @brief User configuration structure for the SPI slave driver.
 * Implements : lpspi_slave_config_t_Class
 */
typedef struct
{
    lpspi_signal_polarity_t pcs_polarity;   /*!< PCS polarity */
    uint16_t                bits_per_frame; /*!< Number of bits/frame, minimum is 8-bits */
    lpspi_clock_phase_t     clk_phase;      /*!< Selects which phase of clock to capture data */
    lpspi_which_pcs_t       which_pcs;
    lpspi_sck_polarity_t    clk_polarity;   /*!< Selects clock polarity */
    bool                    b_lsb_first;    /*!< Option to transmit LSB first */
    bool                    b_byte_swap;    /*!< Option to transmit byte ordering */
    lpspiif_xfer_type_t     xfer_type;      /*!< Type of LPSPI transfer */
    uint8_t                 rx_dma_channel; /*!< Channel number for DMA rx channel. If DMA mode
                                             isn't used this field will be ignored. */
    uint8_t tx_dma_channel;                 /*!< Channel number for DMA tx channel. If DMA mode
                                             isn't used this field will be ignored. */
    spi_callback_t callback;                /*!< Select the callback to transfer complete */
    void          *p_callback_param;        /*!< Select additional callback parameters if it's
                                            necessary */
} lpspiif_slave_config_t;

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
    uint16_t             bits_per_frame;  /*!< Number of bits per frame: 8- to 4096-bits;
                                           needed for    TCR programming */
    uint16_t bytes_per_frame;             /*!< Number of bytes per frame: 1- to 512-bytes */
    bool     b_pcs_continuous;            /*!< Option to keep chip select asserted until
                                            transfer     complete; needed for TCR programming */
    bool              b_blocking;         /*!< Save the transfer type */
    uint32_t          src_clk_freq;       /*!< Module source clock */
    volatile bool     b_xfer_in_progress; /*!< True if there is an active transfer */
    const uint8_t    *p_tx_buffer;        /*!< The buffer from which transmitted bytes are taken */
    uint8_t          *p_rx_buffer;        /*!< The buffer into which received bytes are placed */
    volatile uint16_t tx_remain_cntr;     /*!< Number of bytes remaining to send  */
    volatile uint16_t rx_remain_cntr;     /*!< Number of bytes remaining to receive */
    volatile uint16_t tx_frame_cntr;      /*!< Number of bytes from current frame which
                                          were already sent */
    volatile uint16_t rx_frame_cntr;      /*!< Number of bytes from current frame which
                                          were already received */
    volatile bool         b_lsb;     /*!< True if first bit is LSB and false if first bit is MSB */
    uint8_t               fifo_size; /*!< RX/TX fifo size */
    uint8_t               rx_dma_channel;     /*!< Channel number for DMA rx channel */
    uint8_t               tx_dma_channel;     /*!< Channel number for DMA tx channel */
    lpspiif_xfer_type_t   xfer_type;          /*!< Type of LPSPI transfer */
    semaphore_t           sema_blocking_xfer; /*!< The semaphore used for blocking transfers */
    lpspiif_xfer_status_t xfer_status;        /*!< The status of the current */
    spi_callback_t        callback;           /*!< Select the callback to transfer complete */
    void                 *p_callback_param;   /*!< Select additional callback parameters if it's
                                              necessary */
    uint32_t dummy; /*!< This field is used for the cases when TX is NULL and
                       LPSPI is in DMA mode */
} lpspiif_state_t;

/*******************************************************************************
 * Exported variables
 ******************************************************************************/
/** Define state structure for current LPSPIIF LPSPI0 instance */
extern lpspiif_state_t g_lpspiif_inst0_state;

/** LPSPIIF LPSPI0 Master Configurations */
extern const lpspiif_master_config_t g_lpspiif_inst0_master_config;

/** LPSPIIF LPSPI0 Slave Configurations */
extern const lpspiif_slave_config_t g_lpspiif_inst0_slave_config;

/** Define state structure for current LPSPIIF LPSPI1 instance */
extern lpspiif_state_t g_lpspiif_inst1_state;

/** LPSPIIF LPSPI1 Master Configurations */
extern const lpspiif_master_config_t g_lpspiif_inst1_master_config;

/** LPSPIIF LPSPI1 Slave Configurations */
extern const lpspiif_slave_config_t g_lpspiif_inst1_slave_config;

/** Define state structure for current LPSPIIF LPSPI2 instance */
extern lpspiif_state_t g_lpspiif_inst2_state;

/** LPSPIIF LPSPI2 Master Configurations */
extern const lpspiif_master_config_t g_lpspiif_inst2_master_config;

/** LPSPIIF LPSPI2 Slave Configurations */
extern const lpspiif_slave_config_t g_lpspiif_inst2_slave_config;

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : lpspiif_master_init
 * Description   : Initializes a LPSPI instance for interrupt driven master mode
 *operation.
 *
 * This function uses an interrupt-driven method for transferring data.
 * In this function, the term "p_init_config" is used to indicate the SPI device for
 *which the LPSPI master is communicating. This function initializes the
 *run-time state structure to track the ongoing transfers, un-gates the clock to
 *the LPSPI module, resets the LPSPI module, configures the IRQ state structure,
 *enables the module-level interrupt to the core, and enables the LPSPI module.
 * Implements : LPSPI_DRV_MasterInit_Activity
 *
 *END**************************************************************************/
status_t lpspiif_master_init(uint32_t                       instance,
                             lpspiif_state_t               *p_state_ptr,
                             const lpspiif_master_config_t *p_init_config);

/*FUNCTION**********************************************************************
 *
 * Function Name : lpspiif_master_deinit
 * Description   : Shuts down a LPSPI instance.
 *
 * This function resets the LPSPI peripheral, gates its clock, and disables the
 *interrupt to the core.  It first checks to see if a transfer is in progress
 *and if so returns an error status. Implements :
 *LPSPI_DRV_MasterDeinit_Activity
 *
 *END**************************************************************************/
status_t lpspiif_master_deinit(uint32_t instance);

/*FUNCTION**********************************************************************
 *
 * Function Name : lpspiif_master_xfer_blocking
 * Description   : Performs an interrupt driven blocking SPI master mode
 *transfer.
 *
 * This function simultaneously sends and receives data on the SPI bus, as SPI
 *is naturally a full-duplex bus. The function does not return until the
 *transfer is complete. This function allows the user to optionally pass in a
 *SPI configuration structure which allows the user to change the SPI bus
 *attributes in conjunction with initiating a SPI transfer. The difference
 *between passing in the SPI configuration structure here as opposed to the
 * configure bus function is that the configure bus function returns the
 *calculated baud rate where this function does not. The user can also call the
 *configure bus function prior to the transfer in which case the user would
 *simply pass in a NULL to the transfer function's device structure parameter.
 * Implements : LPSPI_DRV_MasterTransferBlocking_Activity
 *
 *END**************************************************************************/
status_t lpspiif_master_xfer_blocking(uint32_t       instance,
                                      const uint8_t *p_send_buffer,
                                      uint8_t       *p_rcv_buffer,
                                      uint16_t       xfer_byte_count,
                                      uint32_t       timeout);

/*FUNCTION**********************************************************************
 *
 * Function Name : lpspiif_master_xfer
 * Description   : Performs an interrupt driven non-blocking SPI master mode
 *transfer.
 *
 * This function simultaneously sends and receives data on the SPI bus, as SPI
 *is naturally a full-duplex bus. The function returns immediately after
 *initiating the transfer. The user needs to check whether the transfer is
 *complete using the lpspiif_master_get_xfer_status function. This function
 *allows the user to optionally pass in a SPI configuration structure which
 * allows the user to change the SPI bus attributes in conjunction with
 *initiating a SPI transfer. The difference between passing in the SPI
 *configuration structure here as opposed to the configure bus function is that
 *the configure bus function returns the calculated baud rate where this
 *function does not. The user can also call the configure bus function prior to
 *the transfer in which case the user would simply pass in a NULL to the
 *transfer function's device structure parameter. Implements :
 *LPSPI_DRV_MasterTransfer_Activity
 *
 *END**************************************************************************/
status_t lpspiif_master_xfer(uint32_t       instance,
                             const uint8_t *p_send_buffer,
                             uint8_t       *p_rcv_buffer,
                             uint16_t       xfer_byte_count);

/*FUNCTION**********************************************************************
 *
 * Function Name : lpspiif_master_get_xfer_status
 * Description   : Returns whether the previous interrupt driven transfer is
 *completed.
 *
 * When performing an a-sync (non-blocking) transfer, the user can call this
 *function to ascertain the state of the current transfer: in progress (or busy)
 *or complete (success). In addition, if the transfer is still in progress, the
 *user can get the number of words that should be receive. Implements :
 *LPSPI_DRV_MasterGetTransferStatus_Activity
 *
 *END**************************************************************************/
status_t lpspiif_master_get_xfer_status(uint32_t instance, uint32_t *p_remain_byte_count);

/*
 * Implements : LPSPI_DRV_SlaveInit_Activity
 */
status_t lpspiif_slave_init(uint32_t                      instance,
                            lpspiif_state_t              *p_state_ptr,
                            const lpspiif_slave_config_t *p_init_config);

/*
 * Implements : LPSPI_DRV_SlaveDeinit_Activity
 */
status_t lpspiif_slave_deinit(uint32_t instance);

/*
 * Implements : LPSPI_DRV_SlaveTransferBlocking_Activity
 */
status_t lpspiif_slave_xfer_blocking(uint32_t       instance,
                                     const uint8_t *p_send_buffer,
                                     uint8_t       *p_rcv_buffer,
                                     uint16_t       xfer_byte_count,
                                     uint32_t       timeout);

/*
 * Implements : LPSPI_DRV_SlaveTransfer_Activity
 */
status_t lpspiif_slave_xfer(uint32_t       instance,
                            const uint8_t *p_send_buffer,
                            uint8_t       *p_rcv_buffer,
                            uint16_t       xfer_byte_count);

/*
 * Implements : LPSPI_DRV_SlaveGetTransferStatus_Activity
 */
status_t lpspiif_slave_get_xfer_status(uint32_t instance, uint32_t *p_remain_byte_count);

/*
 * Implements : lpspiif_slave_break_xfer_off
 */
status_t lpspiif_slave_break_xfer_off(uint32_t instance);

#ifdef __cplusplus
}
#endif

#endif /* LPSPI_INTERFACE_H */

/*** end of file ***/
