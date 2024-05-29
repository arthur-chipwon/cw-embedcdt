/**
 * @file lpi2c_interface.h
 * @brief An application interface with respect to both master and slave
 * for I2C bus.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef LPI2C_INTERFACE_H
#define LPI2C_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "lpi2c_driver.h"
#include "status.h"
#include "osif_driver.h"
#include "callbacks.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/**
 * @brief Manual ACK/NACK mode. 'ACKSTALL' is for manual ACK/NACK response. At the mean while, when
 * received DeviceID/Address b'[7:1] + Read/Write b'[0], the SCL is stretching till ACK or NACK
 * response has been asserted by firmware manual claim.
 */
// #define _I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_

/* Size of the master command queue. Worst case: 5 commands in High-Speed receive with 10-bit
   address: START + master code, REP START + addr_1 + tx, addr_2, REP START + addr_1 + rx, receive
   command */
#define LPI2C_MASTER_CMD_QUEUE_SIZE 5U

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/*! @brief I2C operating modes
 * Implements : lpi2c_mode_t_Class
 */
typedef enum
{
    LPI2C_STANDARD_MODE =
        0x0U, /*!< Standard-mode (Sm), bidirectional data transfers up to 100 kbit/s */
    LPI2C_FAST_MODE = 0x1U, /*!< Fast-mode (Fm), bidirectional data transfers up to 400 kbit/s */
#if (LPI2C_HAS_FAST_PLUS_MODE)
    LPI2C_FASTPLUS_MODE =
        0x2U, /*!< Fast-mode Plus (Fm+), bidirectional data transfers up to 1 Mbit/s */
#endif
#if (LPI2C_HAS_HIGH_SPEED_MODE)
    LPI2C_HIGHSPEED_MODE =
        0x3U, /*!< High-speed Mode (Hs-mode), bidirectional data transfers up to 3.4 Mbit/s */
#endif
#if (LPI2C_HAS_ULTRA_FAST_MODE)
    LPI2C_ULTRAFAST_MODE =
        0x4U /*!< Ultra Fast Mode (UFm), unidirectional data transfers up to 5 Mbit/s */
#endif
} lpi2c_mode_t;

/*! @brief Type of LPI2C transfer (based on interrupts or DMA).
 * Implements : lpi2c_transfer_type_t_Class
 */
typedef enum
{
    LPI2CIF_USING_DMA        = 0, /*!< The driver will use DMA to perform I2C transfer */
    LPI2CIF_USING_INTERRUPTS = 1, /*!< The driver will use interrupts to perform I2C transfer */
} lpi2cif_xfer_type_t;

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Defines the example structure
 *
 * This structure is used as an example.
 */

/*!
 * @brief Master configuration structure
 *
 * This structure is used to provide configuration parameters for the LPI2C master at initialization
 * time. Implements : lpi2c_master_user_config_t_Class
 */
typedef struct
{
    uint16_t     dev_id_addr;         /*!< Slave address, 7-bit or 10-bit */
    bool         b_10bit_dev_id_addr; /*!< Selects 7-bit or 10-bit slave address */
    lpi2c_mode_t operating_mode;      /*!< I2C Operating mode */
    uint32_t     baud_rate; /*!< The baud rate in hertz to use with current slave device */
#if (LPI2C_HAS_HIGH_SPEED_MODE)
    uint32_t baud_rate_hs;  /*!< Baud rate for High-speed mode. Unused in other operating modes */
    uint8_t  master_code;   /*!< Master code for High-speed mode. Valid range: 0-7. Unused in other
                              operating modes */
#endif
    lpi2cif_xfer_type_t xfer_type; /*!< Type of LPI2C transfer */
    uint8_t dma_channel; /*!< Channel number for DMA channel. If DMA mode isn't used this field will
                           be ignored. */
    i2c_master_callback_t
        callback;        /*!< Master callback function. Note that this function will be
                                      called from the interrupt service routine at the end of a transfer,
                                      so its execution time should be as small as possible. It can be
                                      NULL if you want to check manually the status of the transfer. */
    void *p_callback_param; /*!< Parameter for the master callback function */
} lpi2cif_master_config_t;

/*!
 * @brief Slave configuration structure
 *
 * This structure is used to provide configuration parameters for the LPI2C slave at initialization
 * time. Implements : lpi2c_slave_user_config_t_Class
 */
typedef struct
{
    uint16_t     dev_id_addr;           /*!< Slave address, 7-bit or 10-bit */
    bool         b_10bit_dev_id_addr;   /*!< Selects 7-bit or 10-bit slave address */
    lpi2c_mode_t operating_mode;        /*!< I2C Operating mode */
    bool         b_listening_on_demand; /*!< Slave mode (always listening or on demand only) */
    lpi2cif_xfer_type_t xfer_type;      /*!< Type of LPI2C transfer */
    uint8_t dma_channel; /*!< Channel number for DMA rx channel. If DMA mode isn't used this field
                           will be ignored. */
    i2c_slave_callback_t callback; /*!< Slave callback function. Note that this function will
                                           be called from the interrupt service routine, so its
                                            execution time should be as small as possible. It can be
                                            NULL if the slave is not in listening mode
                                            (b_listening_on_demand = false) */
    void *p_callback_param;        /*!< Parameter for the slave callback function */
} lpi2cif_slave_config_t;

/*!
 * @brief Baud rate structure
 *
 * This structure is used for setting or getting the baud rate.
 * Implements : lpi2c_baud_rate_params_t_Class
 */
typedef struct
{
    uint32_t baud_rate;
#if (LPI2C_HAS_HIGH_SPEED_MODE)
    uint32_t baud_rate_hs;
#endif
} lpi2cif_mode_baud_rate_t;

#if 0
/*! @cond DRIVER_INTERNAL_USE_ONLY */
/* LPI2C master commands */
typedef enum
{
    LPI2C_MASTER_COMMAND_TRANSMIT        = 0U, /*!< Transmit DATA[7:0] */
    LPI2C_MASTER_COMMAND_RECEIVE         = 1U, /*!< Receive (DATA[7:0] + 1) bytes */
    LPI2C_MASTER_COMMAND_STOP            = 2U, /*!< Generate STOP condition */
    LPI2C_MASTER_COMMAND_RECEIVE_DISCARD = 3U, /*!< Receive and discard (DATA[7:0] + 1) bytes */
    LPI2C_MASTER_COMMAND_START = 4U, /*!< Generate START and transmit address in DATA[7:0] */
    LPI2C_MASTER_COMMAND_START_NACK =
        5U, /*!< Generate START and transmit address in DATA[7:0], expect a NACK to be returned */
    LPI2C_MASTER_COMMAND_START_HS =
        6U, /*!< Generate START and transmit address in DATA[7:0] in high speed mode */
    LPI2C_MASTER_COMMAND_START_NACK_HS = 7U, /*!< Generate START and transmit address in DATA[7:0]
                                                in high speed mode, expect a NACK to be returned */
} lpi2c_master_command_t;
/*! @endcond */
#endif

/*! @cond DRIVER_INTERNAL_USE_ONLY */
/* Master software command queue */
typedef struct
{
    lpi2c_master_command_t cmd[LPI2C_MASTER_CMD_QUEUE_SIZE];
    uint8_t                data[LPI2C_MASTER_CMD_QUEUE_SIZE];
    uint8_t                write_idx;
    uint8_t                read_idx;
} lpi2cif_cmd_queue_t;
/*! @endcond */

/*!
 * @brief Master internal context structure
 *
 * This structure is used by the master-mode driver for its internal logic. It must
 * be provided by the application through the lpi2cif_master_init() function, then
 * it cannot be freed until the driver is de-initialized using lpi2cif_master_deinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct
{
    /*! @cond DRIVER_INTERNAL_USE_ONLY */
    lpi2cif_cmd_queue_t
                   cmd_queue;   /* Software queue for commands, when LPI2C FIFO is not big enough */
    uint8_t       *p_rx_buffer; /* Pointer to receive data buffer */
    uint32_t       rx_cntr;     /* Size of receive data buffer */
    const uint8_t *p_tx_buffer; /* Pointer to transmit data buffer */
    uint32_t       tx_cntr;     /* Size of transmit data buffer */
    volatile status_t status;   /* Status of last driver operation */
    lpi2c_mode_t      operating_mode; /* I2C Operating mode */
    uint16_t          dev_id_addr;    /* Slave address */
    volatile bool     b_i2c_idle;     /* Idle/busy state of the driver */
#if (LPI2C_HAS_HIGH_SPEED_MODE)
    uint8_t  master_code;             /* Master code for High-speed mode */
    bool     b_hs_in_progress;        /* High-speed communication is in progress */
    uint32_t baud_rate_hs; /*!< Baud rate for High-speed mode. Unused in other operating modes */
#endif
    bool b_end_stop; /* Specifies if STOP condition must be generated after current transfer */
    bool b_10bit_dev_id_addr;                 /* Selects 7-bit or 10-bit slave address */
    semaphore_t           sema_blocking_xfer; /* Semaphore used by blocking functions */
    bool                  blocking;           /* Specifies if the current transfer is blocking */
    lpi2cif_xfer_type_t   xfer_type;          /* Type of LPI2C transfer */
    uint8_t               dma_channel;        /* Channel number for DMA rx channel */
    i2c_master_callback_t callback;           /* Master callback function */
    void                 *p_callback_param;   /* Parameter for the master callback function */
    bool                  b_abort_xfer;       /* Specifies if master has aborted transfer */
    uint32_t              baud_rate;          /* Baud rate in Hz*/
    /*! @endcond */
} lpi2cif_master_state_t;

/*!
 * @brief Slave internal context structure
 *
 * This structure is used by the slave-mode driver for its internal logic. It must
 * be provided by the application through the lpi2cif_slave_init() function, then
 * it cannot be freed until the driver is de-initialized using lpi2cif_slave_deinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct
{
    /*! @cond DRIVER_INTERNAL_USE_ONLY */
    status_t            status;                /* The I2C slave status */
    volatile bool       b_xfer_in_progress;    /* Slave is busy because of an ongoing transfer */
    uint32_t            tx_cntr;               /* Size of the TX buffer*/
    uint32_t            rx_cntr;               /* Size of the RX buffer*/
    const uint8_t      *p_tx_buffer;           /* Pointer to Tx Buffer*/
    uint8_t            *p_rx_buffer;           /* Pointer to Rx Buffer*/
    lpi2c_mode_t        operating_mode;        /* I2C Operating mode */
    bool                b_listening_on_demand; /* Slave mode (always listening or on demand only) */
    bool                b_10bit_dev_id_addr;   /* Specifies if 10-bit or 7-bit address */
    uint8_t             num_rept_start;        /* Specifies the number of repeated starts */
    bool                b_tx_underrun_warn;    /* Possible slave tx underrun */
    semaphore_t         sema_blocking_xfer;    /* Semaphore used by blocking functions */
    bool                blocking;              /* Specifies if the current transfer is blocking */
    lpi2cif_xfer_type_t xfer_type;             /* Type of LPI2C transfer */
    uint8_t             dma_channel;
    /* Channel number for DMA channel */       /* Channel number for DMA tx channel */
    i2c_slave_callback_t callback;             /* Slave callback function */
    void                *p_callback_param;     /* Parameter for the slave callback function */
    /*! @endcond */
} lpi2cif_slave_state_t;

/*!
 * @brief LPI2C internal context structure
 *
 * This structure is used by the LPI2C driver for its internal logic.
 */
typedef struct
{
    /** @cond Internal use only. */
    LPI2C_t             *p_base;
    const IRQn_t        *p_irq_id;
    const clock_names_t *p_clk_name;
    bool b_is_master; /**< A master is configured, if flag is true, else hardware is used to be
                         slave. */
    union
    {
        lpi2cif_master_state_t master;
        lpi2cif_slave_state_t  slave;
    };
    /*! @endcond */
} lpi2cif_state_t;

/*******************************************************************************
 * Exported variables
 ******************************************************************************/
/** Define state structure for current LPI2CIF LPI2C0 instance */
extern lpi2cif_state_t g_lpi2cif_inst0_state;

/** LPI2CIF LPI2C0 Master Configurations */
extern const lpi2cif_master_config_t g_lpi2cif_inst0_master_config;

/** LPI2CIF LPI2C0 Slave Configurations */
extern const lpi2cif_slave_config_t g_lpi2cif_inst0_slave_config;

/** Define state structure for current LPI2CIF LPI2C1 instance */
extern lpi2cif_state_t g_lpi2cif_inst1_state;

/** LPI2CIF LPI2C1 Master Configurations */
extern const lpi2cif_master_config_t g_lpi2cif_inst1_master_config;

/** LPI2CIF LPI2C1 Slave Configurations */
extern const lpi2cif_slave_config_t g_lpi2cif_inst1_slave_config;

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/*!
 * @brief Initialize the LPI2C master mode driver
 *
 * This function initializes the LPI2C driver in master mode.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[in] p_state_ptr    Pointer to the LPI2C master driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using lpi2cif_master_deinit().
 * @param[in] p_init_config    Pointer to the LPI2C master user configuration structure. The
 * function reads configuration data from this structure and initializes the driver accordingly. The
 * application may free this structure after the function returns.
 * @return    Error or success status returned by API
 */
status_t lpi2cif_master_init(uint32_t                       instance,
                             lpi2cif_state_t               *p_state_ptr,
                             const lpi2cif_master_config_t *p_init_config);

/*!
 * @brief De-initialize the LPI2C master mode driver
 *
 * This function de-initializes the LPI2C driver in master mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @return    Error or success status returned by API
 */
status_t lpi2cif_master_deinit(uint32_t instance);

/*!
 * @brief Set bus idle timeout for LPI2C
 *
 * This function sets time out for bus idle for Master.If both SCL and SDA are high for longer than
 * Timeout cycles, then the I2C bus is assumed to be idle and the master can generate a START
 * condition
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[in] timeout   bus idle timeout period in clock cycle. Zero means no bus idle timeout
 */
void lpi2cif_master_cfg_bus_idle_timeout(uint32_t instance, uint16_t timeout);

/*!
 * @brief Get the currently configured baud rate
 *
 * This function returns the currently configured baud rate.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[in] p_mode_baud_rate  structure that contains the current baud rate in hertz
 *                  and the baud rate in hertz for High-speed mode (unused
 *                  in other modes, can be NULL)
 */
void lpi2cif_master_get_baud_rate(uint32_t instance, lpi2cif_mode_baud_rate_t *p_mode_baud_rate);

/*!
 * @brief Set the baud rate for any subsequent I2C communication
 *
 * This function sets the baud rate (SCL frequency) for the I2C master. It can also
 * change the operating mode. If the operating mode is High-Speed, a second baud rate
 * must be provided for high-speed communication.
 * Note that due to module limitation not any baud rate can be achieved. The driver
 * will set a baud rate as close as possible to the requested baud rate, but there may
 * still be substantial differences, for example if requesting a high baud rate while
 * using a low-frequency protocol clock for the LPI2C module. The application should
 * call lpi2cif_master_get_baud_rate() after lpi2cif_master_set_baud_rate() to check
 * what baud rate was actually set.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[in] operating_mode  I2C operating mode
 * @param[in] p_mode_baud_rate  structure that contains the baud rate in hertz to use by current
 * slave device and also the baud rate in hertz for High-speed mode (unused in other modes)
 *
 * @return    Error or success status returned by API
 */
status_t lpi2cif_master_set_baud_rate(uint32_t                        instance,
                                      const lpi2c_mode_t              operating_mode,
                                      const lpi2cif_mode_baud_rate_t *p_mode_baud_rate);

/*!
 * @brief Set the slave address for any subsequent I2C communication
 *
 * This function sets the slave address which will be used for any future
 * transfer initiated by the LPI2C master.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[in] address   slave address, 7-bit or 10-bit
 * @param[in] b_10bit_dev_id_addr   specifies if provided address is 10-bit
 */
void lpi2cif_master_set_dev_id_addr(uint32_t       instance,
                                    const uint16_t address,
                                    const bool     b_10bit_dev_id_addr);

/*!
 * @brief Perform a non-blocking send transaction on the I2C bus
 *
 * This function starts the transmission of a block of data to the currently
 * configured slave address and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine.
 * Use LPI2C_DRV_MasterGetSendStatus() to check the progress of the transmission.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[in] p_tx_buffer    pointer to the data to be transferred
 * @param[in] xfer_byte_count    length in bytes of the data to be transferred
 * @param[in] b_end_stop    specifies whether or not to generate stop condition after the
 * transmission
 * @return    Error or success status returned by API
 */
status_t lpi2cif_master_send_data(uint32_t       instance,
                                  const uint8_t *p_tx_buffer,
                                  uint32_t       xfer_byte_count,
                                  bool           b_end_stop);

/*!
 * @brief Perform a blocking send transaction on the I2C bus
 *
 * This function sends a block of data to the currently configured slave address, and
 * only returns when the transmission is complete.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[in] p_tx_buffer    pointer to the data to be transferred
 * @param[in] xfer_byte_count    length in bytes of the data to be transferred
 * @param[in] b_end_stop    specifies whether or not to generate stop condition after the
 * transmission
 * @param[in] timeout   timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t lpi2cif_master_send_data_blocking(uint32_t       instance,
                                           const uint8_t *p_tx_buffer,
                                           uint32_t       xfer_byte_count,
                                           bool           b_end_stop,
                                           uint32_t       timeout);

/*!
 * @brief Perform a blocking receive transaction on the I2C bus
 *
 * This function receives a block of data from the currently configured slave address,
 * and only returns when the transmission is complete.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[out] p_rx_buffer    pointer to the buffer where to store received data
 * @param[in] xfer_byte_count    length in bytes of the data to be transferred
 * @param[in] b_end_stop    specifies whether or not to generate stop condition after the reception
 * @param[in] timeout   timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t lpi2cif_master_rcv_data_blocking(uint32_t instance,
                                          uint8_t *p_rx_buffer,
                                          uint32_t xfer_byte_count,
                                          bool     b_end_stop,
                                          uint32_t timeout);

/*!
 * @brief Return the current status of the I2C master transfer
 *
 * This function can be called during a non-blocking transmission to check the
 * status of the transfer.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[out] p_remain_byte_count   the number of remaining bytes in the active I2C transfer
 * @return    Error or success status returned by API
 */
status_t lpi2cif_master_get_xfer_status(uint32_t instance, uint32_t *p_remain_byte_count);

/*!
 * @brief Abort a non-blocking I2C Master transmission or reception
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @return    Error or success status returned by API
 */
status_t lpi2cif_master_abort_xfer_data(uint32_t instance);

/*!
 * @brief Perform a non-blocking receive transaction on the I2C bus
 *
 * This function starts the reception of a block of data from the currently
 * configured slave address and returns immediately.
 * The rest of the reception is handled by the interrupt service routine.
 * Use LPI2C_DRV_MasterGetReceiveStatus() to check the progress of the reception.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[out] p_rx_buffer    pointer to the buffer where to store received data
 * @param[in] xfer_byte_count    length in bytes of the data to be transferred
 * @param[in] b_end_stop    specifies whether or not to generate stop condition after the reception
 * @return    Error or success status returned by API
 */
status_t lpi2cif_master_rcv_data(uint32_t instance,
                                 uint8_t *p_rx_buffer,
                                 uint32_t xfer_byte_count,
                                 bool     b_end_stop);

/*!
 * @brief Gets the default configuration structure for master
 *
 * The default configuration structure is:
 *
 * @param[in] p_config Pointer to configuration structure
 */
void lpi2cif_master_get_default_config(lpi2cif_master_config_t *p_config);

/*!
 * @brief Initialize the I2C slave mode driver
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[in] p_state_ptr     Pointer to the LPI2C slave driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using lpi2cif_slave_deinit().
 * @param[in] p_init_config    Pointer to the LPI2C slave user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @return    Error or success status returned by API
 */
status_t lpi2cif_slave_init(uint32_t                      instance,
                            lpi2cif_state_t              *p_state_ptr,
                            const lpi2cif_slave_config_t *p_init_config);

/*!
 * @brief De-initialize the I2C slave mode driver
 *
 * This function de-initializes the LPI2C driver in slave mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @return    Error or success status returned by API
 */
status_t lpi2cif_slave_deinit(uint32_t instance);

/*!
 * @brief Provide a buffer for transmitting data
 *
 * This function provides a buffer from which the LPI2C slave-mode driver can
 * transmit data. It can be called for example from the user callback provided at
 * initialization time, when the driver reports events LPI2C_SLAVE_EVENT_TX_REQ or
 * LPI2C_SLAVE_EVENT_TX_EMPTY.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[in] p_tx_buffer    pointer to the data to be transferred
 * @param[in] buf_size    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t lpi2cif_slave_set_tx_buffer(uint32_t       instance,
                                     const uint8_t *p_tx_buffer,
                                     uint32_t       buf_size);

/*!
 * @brief Provide a buffer for receiving data.
 *
 * This function provides a buffer in which the LPI2C slave-mode driver can
 * store received data. It can be called for example from the user callback provided at
 * initialization time, when the driver reports events LPI2C_SLAVE_EVENT_RX_REQ or
 * LPI2C_SLAVE_EVENT_RX_FULL.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[in] p_rx_buffer    pointer to the data to be transferred
 * @param[in] buf_size    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t lpi2cif_slave_set_rx_buffer(uint32_t instance, uint8_t *p_rx_buffer, uint32_t buf_size);

#if defined(REFER_TO_REF_DESIGN)
#else  /* REFER_TO_REF_DESIGN */
/*!
 * @brief Release connection of buffer for transmitting data and receiving data.
 *
 * This function retrieves unassigned buffer pointer to state variables for slave
 * listen mechanism.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[in] p_tx_buffer    pointer to the data to be transferred
 * @param[in] tx_buf_size    length in bytes of the data to be transferred
 * @param[in] p_rx_buffer    pointer to the data to be transferred
 * @param[in] rx_buf_size    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t lpi2cif_slave_release_buffer(uint32_t       instance,
                                      const uint8_t *p_tx_buffer,
                                      uint32_t       tx_buf_size,
                                      uint8_t       *p_rx_buffer,
                                      uint32_t       rx_buf_size);
#endif /* REFER_TO_REF_DESIGN */

/*!
 * @brief Perform a non-blocking send transaction on the I2C bus
 *
 * Performs a non-blocking send transaction on the I2C bus when the slave is
 * not in listening mode (initialized with b_listening_on_demand = false). It starts
 * the transmission and returns immediately. The rest of the transmission is
 * handled by the interrupt service routine.
 * Use LPI2C_DRV_SlaveGetTransmitStatus() to check the progress of the transmission.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[in] p_tx_buffer    pointer to the data to be transferred
 * @param[in] xfer_byte_count    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t lpi2cif_slave_send_data(uint32_t       instance,
                                 const uint8_t *p_tx_buffer,
                                 uint32_t       xfer_byte_count);

/*!
 * @brief Perform a blocking send transaction on the I2C bus
 *
 * Performs a blocking send transaction on the I2C bus when the slave is
 * not in listening mode (initialized with b_listening_on_demand = false). It sets
 * up the transmission and then waits for the transfer to complete before
 * returning.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[in] p_tx_buffer    pointer to the data to be transferred
 * @param[in] xfer_byte_count    length in bytes of the data to be transferred
 * @param[in] timeout   timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t lpi2cif_slave_send_data_blocking(uint32_t       instance,
                                          const uint8_t *p_tx_buffer,
                                          uint32_t       xfer_byte_count,
                                          uint32_t       timeout);

/*!
 * @brief Perform a non-blocking receive transaction on the I2C bus
 *
 * Performs a non-blocking receive transaction on the I2C bus when the slave is
 * not in listening mode (initialized with b_listening_on_demand = false). It starts
 * the reception and returns immediately. The rest of the reception is
 * handled by the interrupt service routine.
 * Use LPI2C_DRV_SlaveGetReceiveStatus() to check the progress of the reception.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[out] p_rx_buffer    pointer to the buffer where to store received data
 * @param[in] xfer_byte_count    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t lpi2cif_slave_rcv_data(uint32_t instance, uint8_t *p_rx_buffer, uint32_t xfer_byte_count);

/*!
 * @brief Perform a blocking receive transaction on the I2C bus
 *
 * Performs a blocking receive transaction on the I2C bus when the slave is
 * not in listening mode (initialized with b_listening_on_demand = false). It sets
 * up the reception and then waits for the transfer to complete before
 * returning.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[out] p_rx_buffer    pointer to the buffer where to store received data
 * @param[in] xfer_byte_count    length in bytes of the data to be transferred
 * @param[in] timeout   timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t lpi2cif_slave_rcv_data_blocking(uint32_t instance,
                                         uint8_t *p_rx_buffer,
                                         uint32_t xfer_byte_count,
                                         uint32_t timeout);

/*!
 * @brief Return the current status of the I2C slave transfer
 *
 * This function can be called during a non-blocking transmission to check the
 * status of the transfer.
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @param[out] p_remain_byte_count   the number of remaining bytes in the active I2C transfer
 * @return    Error or success status returned by API
 */
status_t lpi2cif_slave_get_xfer_status(uint32_t instance, uint32_t *p_remain_byte_count);

/*!
 * @brief Abort a non-blocking I2C Master transmission or reception
 *
 * @param[in] instance  LPI2C peripheral instance number
 * @return    Error or success status returned by API
 */
status_t lpi2cif_slave_abort_xfer_data(uint32_t instance);

/*!
 * @brief Gets the default configuration structure for slave
 *
 * The default configuration structure is:
 *
 * @param[in] p_config Pointer to configuration structure
 */
void lpi2cif_slave_get_default_config(lpi2cif_slave_config_t *p_config);

#ifdef __cplusplus
}
#endif

#endif /* LPI2C_INTERFACE_H */

/*** end of file ***/
