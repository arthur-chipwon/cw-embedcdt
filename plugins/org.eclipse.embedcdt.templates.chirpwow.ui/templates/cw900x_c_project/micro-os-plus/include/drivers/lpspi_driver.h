/**
 * @file lpspi_driver.h
 * @brief The module provides for requiring LPSPI hardwre registers access.
 * Export functions for wide scope use.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef LPSPI_DRIVER_H
#define LPSPI_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "device_registers.h"
#include "lpspi_access.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/** @brief Device instance number. */
#define INST_LPSPI0 0u

/** @brief Device instance number. */
#define INST_LPSPI1 1u

/** @brief Device instance number. */
#define INST_LPSPI2 2u

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/*! @brief LPSPI master or slave configuration.
 */
typedef enum
{
    LPSPI_MASTER = 1U, /*!< LPSPI peripheral operates in master mode. */
    LPSPI_SLAVE  = 0U  /*!< LPSPI peripheral operates in slave mode. */
} lpspi_master_slave_mode_t;

/*! @brief LPSPI pin (SDO and SDI) configuration.
 */
typedef enum
{
    LPSPI_SDI_IN_SDO_OUT = 0U, /*!< LPSPI SDI input, SDO output. */
    LPSPI_SDI_IN_OUT     = 1U, /*!< SDI is used for both input and output data. */
    LPSPI_SDO_IN_OUT     = 2U, /*!< SDO is used for both input and output data. */
    LPSPI_SDI_OUT_SDO_IN = 3U  /*!< LPSPI SDO input, SDI output. */
} lpspi_pin_config_t;

/*! @brief LPSPI data output configuration.
 */
typedef enum
{
    LPSPI_DATA_OUT_RETAINED = 0U, /*!< Data out retains last value when chip select de-asserted */
    LPSPI_DATA_OUT_TRISTATE = 1U  /*!< Data out is tri-stated when chip select de-asserted */
} lpspi_data_out_config_t;

/*! @brief LPSPI transfer width configuration.
 */
typedef enum
{
    LPSPI_SINGLE_BIT_XFER = 0U, /*!< 1-bit shift at a time, data out on SDO, in
                                   on SDI (normal mode) */
    LPSPI_TWO_BIT_XFER  = 1U,   /*!< 2-bits shift out on SDO/SDI and in on SDO/SDI */
    LPSPI_FOUR_BIT_XFER = 2U /*!< 4-bits shift out on SDO/SDI/PCS[3:2] and in on SDO/SDI/PCS[3:2] */
} lpspi_transfer_width_t;

/*! @brief LPSPI Peripheral Chip Select (PCS) configuration (which PCS to
 * configure). Implements : lpspi_which_pcs_t_Class
 */
typedef enum
{
    LPSPI_PCS0 = 0U, /*!< PCS[0] */
    LPSPI_PCS1 = 1U, /*!< PCS[1] */
    LPSPI_PCS2 = 2U, /*!< PCS[2] */
    LPSPI_PCS3 = 3U  /*!< PCS[3] */
} lpspi_which_pcs_t;

/*! @brief LPSPI Signal (PCS and Host Request) Polarity configuration.
 * Implements : lpspi_signal_polarity_t_Class
 */
typedef enum
{
    LPSPI_ACTIVE_HIGH = 1U, /*!< Signal is Active High (idles low). */
    LPSPI_ACTIVE_LOW  = 0U  /*!< Signal is Active Low (idles high). */
} lpspi_signal_polarity_t;

/*! @brief LPSPI clock phase configuration.
 * Implements : lpspi_clock_phase_t_Class
 */
typedef enum
{
    LPSPI_CLOCK_PHASE_1ST_EDGE = 0U, /*!< Data captured on SCK 1st edge, changed on 2nd. */
    LPSPI_CLOCK_PHASE_2ND_EDGE = 1U  /*!< Data changed on SCK 1st edge, captured on 2nd. */
} lpspi_clock_phase_t;

/*! @brief LPSPI Clock Signal (SCK) Polarity configuration.
 * Implements : lpspi_sck_polarity_t_Class
 */
typedef enum
{
    LPSPI_SCK_ACTIVE_HIGH = 0U, /*!< Signal is Active High (idles low). */
    LPSPI_SCK_ACTIVE_LOW  = 1U  /*!< Signal is Active Low (idles high). */
} lpspi_sck_polarity_t;

/*! @brief LPSPI Transmit Command Register configuration structure.
 *
 * This structure contains the Transmit Command Register (TCR) settings. Any
 * writes to the TCR will cause the entire TCR contents to be pushed to the TX
 * FIFO. Therefore any updates to the TCR should include updates to all of the
 * register bit fields to form a 32-bit write to the TCR.
 */
typedef struct
{
    uint32_t               frame_size; /*!< Number of bits/frame, minimum is 8-bits. */
    lpspi_transfer_width_t width;      /*!< Transfer width, single, 2-bit, or 4-bit transfer. */
    bool                   b_tx_mask; /*!< Option to mask the transmit data (won't load to FIFO). */
    bool                   b_rx_mask; /*!< Option to mask the receive data (won't store in FIFO). */
    bool                 b_cont_cmd;  /*!< Master option to change cmd word within cont transfer. */
    bool                 b_cont_xfer; /*!< Master option for continuous transfer. */
    bool                 b_byte_swap; /*!< Option to invoke the byte swap option in the FIFOs. */
    bool                 b_lsb_first; /*!< Option to transmit LSB first. */
    lpspi_which_pcs_t    which_pcs;   /*!< Selects which PCS to use. */
    uint32_t             pre_div;   /*!< Selects the baud rate prescaler divider TCR bit setting. */
    lpspi_clock_phase_t  clk_phase; /*!< Selects which phase of clock to capture data. */
    lpspi_sck_polarity_t clk_polarity; /*!< Selects clock polarity. */
} lpspi_tx_cmd_config_t;

/*! @brief LPSPI initialization configuration structure.
 *
 * This structure contains parameters for the user to fill in to configure the
 LPSPI.
 * The user passes this structure into the LPSPI init function to configure it
 to
 * their desired parameters.
 * Example user code for:
    - 60MHz assumed, check ref manual for exact value
    - baudrate 500KHz
    - master mode
    - PCS is active low
   @code
    lpspi_init_config_t lpspiCfg;
    lpspiCfg.src_clk_freq = 60000000;
    lpspiCfg.baud_rate = 500000;
    lpspiCfg.lpspi_mode = LPSPI_MASTER;
    lpspiCfg.pcs_polarity = LPSPI_ACTIVE_LOW;
   @endcode
 */
typedef struct
{
    uint32_t                  src_clk_freq; /*!< LPSPI module clock */
    uint32_t                  baud_rate;    /*!< LPSPI baudrate */
    lpspi_master_slave_mode_t lpspi_mode;   /*!< LPSPI master/slave mode */
    lpspi_signal_polarity_t   pcs_polarity; /*!< LPSPI PCS polarity */
} lpspi_init_config_t;

/*******************************************************************************
 * Exported variables
 ******************************************************************************/
/** @brief Table of base pointers for SPI instances. */
extern LPSPI_t *const gp_lpspi_base[LPSPI_INSTANCE_COUNT];

/** @brief Table to save LPSPI IRQ enumeration numbers defined in the CMSIS
 * header file. */
extern const IRQn_t g_lpspi_irq_id[LPSPI_INSTANCE_COUNT];

/** @brief Table to save LPSPI clock names as defined in clock manager. */
extern const clock_names_t g_lpspi_clk_names[LPSPI_INSTANCE_COUNT];

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/*!
 * @brief Resets the LPSPI internal logic and registers to their default
 * settings.
 *
 * This function first performs a software reset of the LPSPI module which
 * resets the internal LPSPI logic and most registers, then proceeds to manually
 * reset all of the LPSPI registers to their default setting to ensuring these
 * registers at programmed to their default value which includes disabling the
 * module.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 */
void lpspi_init(LPSPI_t *p_base);

/*!
 * @brief Disables the LPSPI module.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @return This function returns STATUS_BUSY if it is detected that the Module
 * Busy Flag (MBF) is set, otherwise, if success, it returns STATUS_SUCCESS.
 */
status_t lpspi_disable(LPSPI_t *p_base);

/*!
 * @brief Configures the LPSPI for master or slave.
 *
 * Note that the LPSPI module must first be disabled before configuring this.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] mode Mode setting (master or slave) of type lpspi_master_slave_mode_t
 * @return This function returns the error condition STATUS_ERROR if the module
 * is not disabled else it returns STATUS_SUCCESS.
 */
status_t lpspi_set_master_slave_mode(LPSPI_t *p_base, lpspi_master_slave_mode_t mode);

/*!
 * @brief Flushes the LPSPI FIFOs.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] b_flush_tx_fifo Flushes (true) the Tx FIFO, else do not flush (false) the
 * Tx FIFO
 * @param[in] b_flush_rx_fifo Flushes (true) the Rx FIFO, else do not flush (false) the
 * Rx FIFO
 */
void lpspi_set_flush_fifo_cmd(LPSPI_t *p_base, bool b_flush_tx_fifo, bool b_flush_rx_fifo);

/*!
 * @brief Clears the LPSPI status flag.
 *
 * This function clears the state of one of the LPSPI status flags as requested
 * by the user. Note, the flag must be w1c capable, if not the function returns
 * an error. w1c capable flags are: LPSPI_WORD_COMPLETE LPSPI_FRAME_COMPLETE
 *   LPSPI_TRANSFER_COMPLETE
 *   LPSPI_TRANSMIT_ERROR
 *   LPSPI_RECEIVE_ERROR
 *   LPSPI_DATA_MATCH
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] status_flag The status flag, of type lpspi_status_flag_t
 * @return STATUS_SUCCESS or LPSPI_STATUS_INVALID_PARAMETER
 */
status_t lpspi_clear_status_flag(LPSPI_t *p_base, lpspi_status_flag_t status_flag);

/*!
 * @name SPI Bus Configuration
 * @{
 */

/*!
 * @brief Configures the desired LPSPI PCS polarity.
 *
 * This function allows the user to configure the polarity of a particular PCS
 * signal. Note that the LPSPI module must first be disabled before configuring
 * this.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] which_pcs Select which PCS to program, of type lpspi_which_pcs_t
 * @param[in] pcs_polarity Set PCS as active high or low, of type
 * lpspi_signal_polarity_t
 * @return This function returns the error condition STATUS_ERROR if the module
 * is not disabled else it returns STATUS_SUCCESS.
 */
status_t lpspi_set_pcs_polarity_mode(LPSPI_t                *p_base,
                                     lpspi_which_pcs_t       which_pcs,
                                     lpspi_signal_polarity_t pcs_polarity);

/*!
 * @brief Configures the LPSPI SDO/SDI pin configuration mode.
 *
 * This function configures the pin mode of the LPSPI.
 * For the SDI and SDO pins, the user can configure these pins as follows:
 *  SDI is used for input data and SDO for output data.
 *  SDO is used for input data and SDO for output data.
 *  SDI is used for input data and SDI for output data.
 *  SDO is used for input data and SDI for output data.
 *
 * The user has the option to configure the output data as:
 *  Output data retains last value when chip select is de-asserted (default
 * setting). Output data is tristated when chip select is de-asserted.
 *
 * Finally, the user has the option to configure the PCS[3:2] pins as:
 *  Enabled for PCS operation (default setting).
 *  Disabled - this is need if the user wishes to configure the LPSPI mode for
 * 4-bit transfers where these pins will be used as I/O data pins.
 *
 * Note that the LPSPI module must first be disabled before configuring this.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] pin_cfg Select configuration for the SDO/SDI pins (see
 * lpspi_pin_config_t)
 * @param[in] data_out_cfg Select data output config after chip select de-assertion
 * @param[in] b_pcs_no3_no2_enable Enable or disable PCS[3:2]
 * @return This function returns the error condition STATUS_ERROR if the module
 * is not disabled else it returns STATUS_SUCCESS.
 */
status_t lpspi_set_pin_config_mode(LPSPI_t                *p_base,
                                   lpspi_pin_config_t      pin_cfg,
                                   lpspi_data_out_config_t data_out_cfg,
                                   bool                    b_pcs_no3_no2_enable);

/*!
 * @brief Sets the LPSPI baud rate in bits per second.
 *
 * This function takes in the desired bits_per_sec (baud rate) and calculates the
 * nearest possible baud rate without exceeding the desired baud rate, and
 * returns the calculated baud rate in bits-per-second. It requires that the
 * caller also provide the frequency of the module source clock (in Hertz). Also
 * note that the baud rate does not take into affect until the Transmit Control
 * Register (TCR) is programmed with the PRESCALE value. Hence, this function
 * returns the PRESCALE p_tcr_prescale_value parameter for later programming in the
 * TCR.  It is up to the higher level peripheral driver to alert the user of an
 * out of range baud rate input.
 *
 * Note that the LPSPI module must first be disabled before configuring this.
 * Note that the LPSPI module must be configure for master mode before
 * configuring this.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] bits_per_sec The desired baud rate in bits per second
 * @param[in] src_clk_freq Module source input clock in Hertz
 * @param[out] p_tcr_prescale_value The TCR PRESCALE value, needed by user to program the
 * TCR
 * @return  The actual calculated baud rate. This function may also return a "0"
 * if the LPSPI is not configued for master mode or if the LPSPI module is not
 * disabled.
 */
uint32_t lpspi_set_baud_rate(LPSPI_t  *p_base,
                             uint32_t  bits_per_sec,
                             uint32_t  src_clk_freq,
                             uint32_t *p_tcr_prescale_value);

/*!
 * @brief Configures the baud rate divisor manually (only the
 * LPSPI_CCR[SCKDIV]).
 *
 * This function allows the caller to manually set the baud rate divisor in the
 * event that this divider is known and the caller does not wish to call the
 * lpspi_set_baud_rate function. Note that this only affects the
 * LPSPI_CCR[SCKDIV]). The Transmit Control Register (TCR) is programmed
 * separately with the PRESCALE value. The valid range is 0x00 to 0xFF, if the
 * user inputs outside of this range, an error is returned.
 *
 * Note that the LPSPI module must first be disabled before configuring this.
 * Note that the LPSPI module must be configure for master mode before
 * configuring this.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] divisor Desired baud rate divisor setting (0x00 to 0xFF)
 * @return STATUS_SUCCESS or LPSPI_STATUS_OUT_OF_RANGE if divisor > 0xFF
 */
status_t lpspi_set_baud_rate_divisor(LPSPI_t *p_base, uint32_t divisor);

/*!
 * @brief Sets the PCS flag to the value of the which_pcs parameter.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] which_pcs Desired chip
 */
void lpspi_set_pcs(LPSPI_t *p_base, lpspi_which_pcs_t which_pcs);

/**@}*/

/*!
 * @name Data transfer
 * @{
 */

/*!
 * @brief Sets the Transmit Command Register (TCR) parameters.
 *
 * The Transmit Command Register (TCR) contains multiple parameters that affect
 * the transmission of data, such as clock phase and polarity, which PCS to use,
 * whether or not the PCS remains asserted at the completion of a frame, etc.
 * Any writes to this register results in an immediate push of the entire
 * register and its contents to the TX FIFO.  Hence, writes to this register
 * should include all of the desired parameters written to the register at once.
 * Hence, the user should fill in the members of the lpspi_tx_cmd_config_t data
 * structure and pass this to the function.
 *
 * @param[in] p_base Module base pointer of type LPSPI_t.
 * @param[in] tx_cmd_cfg Structure that contains the Transmit Command Register
 * (TCR) settings of type lpspi_tx_cmd_config_t
 */
void lpspi_set_tx_cmd_reg_tcr(LPSPI_t *p_base, const lpspi_tx_cmd_config_t *tx_cmd_cfg);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* LPSPI_DRIVER_H */

/*** end of file ***/
