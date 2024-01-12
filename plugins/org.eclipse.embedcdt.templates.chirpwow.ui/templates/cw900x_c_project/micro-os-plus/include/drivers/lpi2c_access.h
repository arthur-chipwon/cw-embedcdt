/**
 * @file lpi2c_access.h
 * @brief The module provides for requiring LPI2C hardwre registers access.
 * Export functions for wide scope use.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef LPI2C_ACCESS_H
#define LPI2C_ACCESS_H

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

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/*!
 * LPI2C master interrupts
 */
#define LPI2C_MASTER_DATA_MATCH_INT       0x4000U /*!< Data Match Interrupt       */
#define LPI2C_MASTER_PIN_LOW_TIMEOUT_INT  0x2000U /*!< Pin Low Timeout Interrupt  */
#define LPI2C_MASTER_FIFO_ERROR_INT       0x1000U /*!< FIFO Error Interrupt       */
#define LPI2C_MASTER_ARBITRATION_LOST_INT 0x800U  /*!< Arbitration Lost Interrupt */
#define LPI2C_MASTER_NACK_DETECT_INT      0x400U  /*!< NACK Detect Interrupt      */
#define LPI2C_MASTER_STOP_DETECT_INT      0x200U  /*!< STOP Detect Interrupt      */
#define LPI2C_MASTER_END_PACKET_INT       0x100U  /*!< End Packet Interrupt       */
#define LPI2C_MASTER_RECEIVE_DATA_INT     0x2U    /*!< Receive Data Interrupt     */
#define LPI2C_MASTER_TRANSMIT_DATA_INT    0x1U    /*!< Transmit Data Interrupt    */

/*!
 * LPI2C slave interrupts
 */
#define LPI2C_SLAVE_SMBUS_ALERT_RESPONSE_INT 0x8000U /*!< SMBus Alert Response Interrupt */
#define LPI2C_SLAVE_GENERAL_CALL_INT         0x4000U /*!< General Call Interrupt         */
#define LPI2C_SLAVE_ADDRESS_MATCH_1_INT      0x2000U /*!< Address Match 1 Interrupt      */
#define LPI2C_SLAVE_ADDRESS_MATCH_0_INT      0x1000U /*!< Address Match 0 Interrupt      */
#define LPI2C_SLAVE_FIFO_ERROR_INT           0x800U  /*!< FIFO Error Interrupt           */
#define LPI2C_SLAVE_BIT_ERROR_INT            0x400U  /*!< Bit Error Interrupt            */
#define LPI2C_SLAVE_STOP_DETECT_INT          0x200U  /*!< STOP Detect Interrupt          */
#define LPI2C_SLAVE_REPEATED_START_INT       0x100U  /*!< Repeated Start Interrupt       */
#define LPI2C_SLAVE_TRANSMIT_ACK_INT         0x8U    /*!< Transmit ACK Interrupt         */
#define LPI2C_SLAVE_ADDRESS_VALID_INT        0x4U    /*!< Address Valid Interrupt        */
#define LPI2C_SLAVE_RECEIVE_DATA_INT         0x2U    /*!< Receive Data Interrupt         */
#define LPI2C_SLAVE_TRANSMIT_DATA_INT        0x1U    /*!< Transmit Data Interrupt        */

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/*! @brief LPI2C master status flags.
 */
typedef enum
{
    LPI2C_MSR_TDF_FLAG      = LPI2C_MSR_TDF_SHIFT,  /**< MSR TDF flag */
    LPI2C_MSR_RDF_FLAG      = LPI2C_MSR_RDF_SHIFT,  /**< MSR RDF flag */
    LPI2C_MSR_EPF_FLAG      = LPI2C_MSR_EPF_SHIFT,  /**< MSR EPF flag */
    LPI2C_MSR_SDF_FLAG      = LPI2C_MSR_SDF_SHIFT,  /**< MSR SDF flag */
    LPI2C_MSR_NDF_FLAG      = LPI2C_MSR_NDF_SHIFT,  /**< MSR NDF flag */
    LPI2C_MSR_ALF_FLAG      = LPI2C_MSR_ALF_SHIFT,  /**< MSR ALF flag */
    LPI2C_MSR_FEF_FLAG      = LPI2C_MSR_FEF_SHIFT,  /**< MSR FEF flag */
    LPI2C_MSR_PLTF_FLAG     = LPI2C_MSR_PLTF_SHIFT, /**< MSR PLTF flag */
    LPI2C_MSR_DMF_FLAG      = LPI2C_MSR_DMF_SHIFT,  /**< MSR DMF flag */
    LPI2C_MASTER_ALL_STATUS = 0x00007F00u           /**< Used for clearing all w1c status flags */
} lpi2c_master_status_flag_t;

/*! @brief LPI2C master status flags.
 */
typedef enum
{
    LPI2C_SSR_TDF_FLAG     = LPI2C_SSR_TDF_SHIFT,  /**< MSR TDF flag */
    LPI2C_SSR_RDF_FLAG     = LPI2C_SSR_RDF_SHIFT,  /**< MSR RDF flag */
    LPI2C_SSR_AVF_FLAG     = LPI2C_SSR_AVF_SHIFT,  /**< MSR AVF flag */
    LPI2C_SSR_TAF_FLAG     = LPI2C_SSR_TAF_SHIFT,  /**< MSR TAF flag */
    LPI2C_SSR_RSF_FLAG     = LPI2C_SSR_RSF_SHIFT,  /**< SSR RSF flag */
    LPI2C_SSR_SDF_FLAG     = LPI2C_SSR_SDF_SHIFT,  /**< SSR SDF flag */
    LPI2C_SSR_BEF_FLAG     = LPI2C_SSR_BEF_SHIFT,  /**< SSR BEF flag */
    LPI2C_SSR_FEF_FLAG     = LPI2C_SSR_FEF_SHIFT,  /**< SSR FEF flag */
    LPI2C_SSR_AM0F_FLAG    = LPI2C_SSR_AM0F_SHIFT, /**< SSR AM0F flag */
    LPI2C_SSR_AM1F_FLAG    = LPI2C_SSR_AM1F_SHIFT, /**< SSR AM1F flag */
    LPI2C_SSR_GCF_FLAG     = LPI2C_SSR_GCF_SHIFT,  /**< SSR GCF flag */
    LPI2C_SSR_SARF_FLAG    = LPI2C_SSR_SARF_SHIFT, /**< SSR SARF flag */
    LPI2C_SLAVE_ALL_STATUS = 0x00000F00u           /**< Used for clearing all w1c status flags */
} lpi2c_slave_status_flag_t;

/*! @brief Pin configuration selection
 */
typedef enum
{
    LPI2C_CFG_2PIN_OPEN_DRAIN       = 0U, /*!< 2-pin open drain mode */
    LPI2C_CFG_2PIN_OUTPUT_ONLY      = 1U, /*!< 2-pin output only mode (ultra-fast mode) */
    LPI2C_CFG_2PIN_PUSH_PULL        = 2U, /*!< 2-pin push-pull mode */
    LPI2C_CFG_4PIN_PUSH_PULL        = 3U, /*!< 4-pin push-pull mode */
    LPI2C_CFG_2PIN_OPEN_DRAIN_SLAVE = 4U, /*!< 2-pin open drain mode with separate LPI2C slave */
    LPI2C_CFG_2PIN_OUTPUT_ONLY_SLAVE =
        5U, /*!< 2-pin output only mode (ultra-fast mode) with separate LPI2C slave */
    LPI2C_CFG_2PIN_PUSH_PULL_SLAVE    = 6U, /*!< 2-pin push-pull mode with separate LPI2C slave */
    LPI2C_CFG_4PIN_PUSH_PULL_INVERTED = 7U, /*!< 4-pin push-pull mode (inverted outputs) */
} lpi2c_pin_config_t;

/*! @brief Master NACK reaction configuration
 */
typedef enum
{
    LPI2C_NACK_RECEIVE = 0U, /*!< Receive ACK and NACK normally */
    LPI2C_NACK_IGNORE  = 1U, /*!< Treat a received NACK as if it was an ACK */
} lpi2c_nack_config_t;

/*! @brief LPI2C master prescaler options
 */
typedef enum
{
    LPI2C_MASTER_PRESC_DIV_1   = 0U, /*!< Divide by 1   */
    LPI2C_MASTER_PRESC_DIV_2   = 1U, /*!< Divide by 2   */
    LPI2C_MASTER_PRESC_DIV_4   = 2U, /*!< Divide by 4   */
    LPI2C_MASTER_PRESC_DIV_8   = 3U, /*!< Divide by 8   */
    LPI2C_MASTER_PRESC_DIV_16  = 4U, /*!< Divide by 16  */
    LPI2C_MASTER_PRESC_DIV_32  = 5U, /*!< Divide by 32  */
    LPI2C_MASTER_PRESC_DIV_64  = 6U, /*!< Divide by 64  */
    LPI2C_MASTER_PRESC_DIV_128 = 7U, /*!< Divide by 128 */
} lpi2c_master_prescaler_t;

/*! @brief Slave address configuration
 */
typedef enum
{
    LPI2C_SLAVE_ADDR_MATCH_0_7BIT  = 0U, /*!< Address match 0 (7-bit) */
    LPI2C_SLAVE_ADDR_MATCH_0_10BIT = 1U, /*!< Address match 0 (10-bit) */
    LPI2C_SLAVE_ADDR_MATCH_0_7BIT_OR_1_7BIT =
        2U,                              /*!< Address match 0 (7-bit) or Address match 1 (7-bit) */
    LPI2C_SLAVE_ADDR_MATCH_0_10BIT_OR_1_10BIT =
        3U, /*!< Address match 0 (10-bit) or Address match 1 (10-bit) */
    LPI2C_SLAVE_ADDR_MATCH_0_7BIT_OR_1_10BIT =
        4U, /*!< Address match 0 (7-bit) or Address match 1 (10-bit) */
    LPI2C_SLAVE_ADDR_MATCH_0_10BIT_OR_1_7BIT =
        5U, /*!< Address match 0 (10-bit) or Address match 1 (7-bit) */
    LPI2C_SLAVE_ADDR_MATCH_RANGE_7BIT =
        6U, /*!< From Address match 0 (7-bit) to Address match 1 (7-bit) */
    LPI2C_SLAVE_ADDR_MATCH_RANGE_10BIT =
        7U, /*!< From Address match 0 (10-bit) to Address match 1 (10-bit) */
} lpi2c_slave_addr_config_t;

/*! @brief Slave NACK reaction configuration
 */
typedef enum
{
    LPI2C_SLAVE_NACK_END_TRANSFER      = 0U, /*!< Slave will end transfer when NACK detected */
    LPI2C_SLAVE_NACK_CONTINUE_TRANSFER = 1U, /*!< Slave will not end transfer when NACK detected */
} lpi2c_slave_nack_config_t;

/*! @brief Slave ACK transmission options
 */
typedef enum
{
    LPI2C_SLAVE_TRANSMIT_ACK  = 0U, /*!< Transmit ACK for received word  */
    LPI2C_SLAVE_TRANSMIT_NACK = 1U, /*!< Transmit NACK for received word */
} lpi2c_slave_nack_transmit_t;

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

/*******************************************************************************
 * Exported variables
 ******************************************************************************/

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/
/*!
 * @brief Get the size of the Master Receive FIFO
 *
 * This function returns the size of the Master Receive FIFO, always a power of 2.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  Master Receive FIFO Size
 */
static inline uint16_t
lpi2c_get_master_rx_fifo_size(const LPI2C_t *p_base)
{
    uint32_t tmp = p_base->PARAM;
    tmp          = (tmp & LPI2C_PARAM_MRXFIFO_MASK) >> LPI2C_PARAM_MRXFIFO_SHIFT;
    tmp          = 1UL << tmp; /* RX FIFO size = 2^MRXFIFO */
    return (uint16_t)tmp;
}

/*!
 * @brief Get the size of the Master Transmit FIFO
 *
 * This function returns the size of the Master Transmit FIFO, always a power of 2.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  Master Transmit FIFO Size
 */
static inline uint16_t
lpi2c_get_master_tx_fifo_size(const LPI2C_t *p_base)
{
    uint32_t tmp = p_base->PARAM;
    tmp          = (tmp & LPI2C_PARAM_MTXFIFO_MASK) >> LPI2C_PARAM_MTXFIFO_SHIFT;
    tmp          = 1UL << tmp; /* TX FIFO size = 2^MTXFIFO */
    return (uint16_t)tmp;
}

/*!
 * @brief Reset the master receive FIFO
 *
 * This function empties the receive FIFO of the LPI2C master.
 *
 * @param[in] p_base  base address of the LPI2C module
 */
static inline void
lpi2c_reset_master_rx_fifo_cmd(LPI2C_t *p_base)
{
    uint32_t reg_val = (uint32_t)p_base->MCR;
    reg_val &= (uint32_t)(~(LPI2C_MCR_RRF_MASK));
    reg_val |= LPI2C_MCR_RRF(1u);
    p_base->MCR = (uint32_t)reg_val;
}

/*!
 * @brief Reset the master transmit FIFO
 *
 * This function empties the transmit FIFO of the LPI2C master.
 *
 * @param[in] p_base  base address of the LPI2C module
 */
static inline void
lpi2c_reset_master_tx_fifo_cmd(LPI2C_t *p_base)
{
    uint32_t reg_val = (uint32_t)p_base->MCR;
    reg_val &= (uint32_t)(~(LPI2C_MCR_RTF_MASK));
    reg_val |= LPI2C_MCR_RTF(1u);
    p_base->MCR = (uint32_t)reg_val;
}

/*!
 * @brief Set/clear the master reset command
 *
 * Calling this function with enable parameter set to true resets all internal
 * master logic and registers, except the Master Control Register. The reset state
 * persists until this function is called with enable parameter set to false.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] b_enable  specifies the reset state of the LPI2C master logic
 */
static inline void
lpi2c_set_master_sw_reset(LPI2C_t *p_base, bool b_enable)
{
    uint32_t reg_val = (uint32_t)p_base->MCR;
    reg_val &= (uint32_t)(~(LPI2C_MCR_RST_MASK));
    reg_val |= LPI2C_MCR_RST(b_enable);
    p_base->MCR = (uint32_t)reg_val;
}

/*!
 * @brief Enable or disable the LPI2C master
 *
 * This function enables or disables the LPI2C module in master mode. If the module
 * is enabled, the transmit FIFO  is not empty and the I2C bus is idle, then
 * the LPI2C master will immediately initiate a transfer on the I2C bus.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] b_enable  specifies whether to enable or disable the LPI2C master
 */
static inline void
lpi2c_set_master_enable(LPI2C_t *p_base, bool b_enable)
{
    uint32_t reg_val = (uint32_t)p_base->MCR;
    reg_val &= (uint32_t)(~(LPI2C_MCR_MEN_MASK));
    reg_val |= LPI2C_MCR_MEN(b_enable);
    p_base->MCR = (uint32_t)reg_val;
}

/*!
 * @brief Indicate the availability of receive data
 *
 * This function returns true when the number of words in the receive FIFO is greater
 * than the receive FIFO watermark. See function LPI2C_MasterSetRxFIFOWatermark()
 * for configuring the receive FIFO watermark.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  receive data ready/not ready
 */
static inline bool
lpi2c_get_master_rcv_data_rdy_event(const LPI2C_t *p_base)
{
    uint32_t reg_val = (uint32_t)p_base->MSR;
    reg_val          = (reg_val & LPI2C_MSR_RDF_MASK) >> LPI2C_MSR_RDF_SHIFT;
    return (bool)reg_val;
}

/*!
 * @brief Indicate if the LPI2C master requests more data
 *
 * This function returns true when the number of words in the transmit FIFO is equal
 * or less than the transmit FIFO watermark. See function lpi2c_set_master_tx_watermark()
 * for configuring the transmit FIFO watermark.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  transmit data requested/not requested
 */
static inline bool
lpi2c_get_master_xmt_data_req_event(const LPI2C_t *p_base)
{
    uint32_t reg_val = (uint32_t)p_base->MSR;
    reg_val          = (reg_val & LPI2C_MSR_TDF_MASK) >> LPI2C_MSR_TDF_SHIFT;
    return (bool)reg_val;
}

/*!
 * @brief Check the occurrence of a FIFO error event
 *
 * This function returns true if the LPI2C master detects an attempt to send or
 * receive data without first generating a (repeated) START condition. This can
 * occur if the transmit FIFO underflows when the AUTOSTOP bit is set. When this
 * flag is set, the LPI2C master will send a STOP condition (if busy) and will
 * not initiate a new START condition until this flag has been cleared.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  indication of a FIFO error event
 */
static inline bool
lpi2c_get_master_fifo_error_event(const LPI2C_t *p_base)
{
    uint32_t reg_val = (uint32_t)p_base->MSR;
    reg_val          = (reg_val & LPI2C_MSR_FEF_MASK) >> LPI2C_MSR_FEF_SHIFT;
    return (bool)reg_val;
}

/*!
 * @brief Check the occurrence of an arbitration lost event
 *
 * This function returns true if the LPI2C master detects an arbitration lost
 * condition, as defined by the I2C standard. When this flag sets, the LPI2C
 * master will release the bus (go idle) and will not initiate a new START
 * condition until this flag has been cleared.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  indication of an arbitration lost event
 */
static inline bool
lpi2c_get_master_arbitr_lost_event(const LPI2C_t *p_base)
{
    uint32_t reg_val = (uint32_t)p_base->MSR;
    reg_val          = (reg_val & LPI2C_MSR_ALF_MASK) >> LPI2C_MSR_ALF_SHIFT;
    return (bool)reg_val;
}

/*!
 * @brief Check the occurrence of an unexpected NACK event
 *
 * This function returns true if the LPI2C master detects a NACK when
 * transmitting an address or data. If a NACK is expected for a given address
 * (as configured by the command word) then the flag will set if a NACK is not
 * generated. When set, the master will transmit a STOP condition and will not
 * initiate a new START condition until this flag has been cleared.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  indication of an unexpected NACK event
 */
static inline bool
lpi2c_get_master_nack_det_event(const LPI2C_t *p_base)
{
    uint32_t reg_val = (uint32_t)p_base->MSR;
    reg_val          = (reg_val & LPI2C_MSR_NDF_MASK) >> LPI2C_MSR_NDF_SHIFT;
    return (bool)reg_val;
}

/*!
 * @brief Clear the FIFO error event flag
 *
 * This function clears the FIFO error event. This event must be cleared before
 * the LPI2C master can initiate a START condition.
 *
 * @param[in] p_base  base address of the LPI2C module
 */
static inline void
lpi2c_clear_master_fifo_error_event(LPI2C_t *p_base)
{
    p_base->MSR = ((uint32_t)1U << LPI2C_MSR_FEF_SHIFT);
}

/*!
 * @brief Clear the arbitration lost event flag
 *
 * This function clears the arbitration lost event. This event must be cleared
 * before the LPI2C master can initiate a START condition.
 *
 * @param[in] p_base  base address of the LPI2C module
 */
static inline void
lpi2c_clear_master_arbitr_lost_event(LPI2C_t *p_base)
{
    p_base->MSR = ((uint32_t)1U << LPI2C_MSR_ALF_SHIFT);
}

/*!
 * @brief Clear the unexpected NACK event flag
 *
 * This function clears the unexpected NACK event. This event must be cleared
 * before the LPI2C master can initiate a START condition.
 *
 * @param[in] p_base  base address of the LPI2C module
 */
static inline void
lpi2c_clear_master_nack_det_event(LPI2C_t *p_base)
{
    p_base->MSR = ((uint32_t)1U << LPI2C_MSR_NDF_SHIFT);
}

/*!
 * @brief Enable or disable specified LPI2C master interrupts
 *
 * This function can enable or disable one or more master interrupt sources
 * specified by the interrupts parameter.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] interrupts  interrupts to be enabled or disabled;
 *  must be a bitwise or between one or more of the following constants:
 *  - LPI2C_MASTER_DATA_MATCH_INT          - Data Match Interrupt
 *  - LPI2C_MASTER_PIN_LOW_TIMEOUT_INT     - Pin Low Timeout Interrupt
 *  - LPI2C_MASTER_FIFO_ERROR_INT          - FIFO Error Interrupt
 *  - LPI2C_MASTER_ARBITRATION_LOST_INT    - Arbitration Lost Interrupt
 *  - LPI2C_MASTER_NACK_DETECT_INT         - NACK Detect Interrupt
 *  - LPI2C_MASTER_STOP_DETECT_INT         - STOP Detect Interrupt
 *  - LPI2C_MASTER_END_PACKET_INT          - End Packet Interrupt
 *  - LPI2C_MASTER_RECEIVE_DATA_INT        - Receive Data Interrupt
 *  - LPI2C_MASTER_TRANSMIT_DATA_INT       - Transmit Data Interrupt
 * @param[in] b_enable  specifies whether to enable or disable specified interrupts
 */
static inline void
lpi2c_set_master_int(LPI2C_t *p_base, uint32_t interrupts, bool b_enable)
{
    uint32_t tmp = p_base->MIER;

    if (b_enable == true)
    {
        tmp |= interrupts;
    }
    else
    {
        tmp &= ~interrupts;
    }
    p_base->MIER = tmp;
}

#if defined(REFER_TO_REF_DESIGN)
#else  /* REFER_TO_REF_DESIGN */
/*!
 * @brief Gets the LPI2C master status busy.
 *
 * This function returns the busy state of the LPI2C master.
 *
 * @param[in] p_base Module base pointer of type LPI2C_t.
 * @return bool true is core busy.
 */
static inline bool
lpi2c_get_master_status_busy(LPI2C_t *p_base)
{
    return (p_base->MSR & LPI2C_MSR_BBF_MASK) ? true : false;
}
#endif /* REFER_TO_REF_DESIGN */

/*!
 * @brief Gets the LPI2C master status bitmap state.
 *
 * This function returns the state of bitmap of the LPI2C master status at the moment of intrrupt
 * event.
 *
 * @param[in] p_base Module base pointer of type LPI2C_t.
 * @return State of bitmap of the status.
 */
static inline uint32_t
lpi2c_get_master_status_bm(LPI2C_t *p_base)
{
    return p_base->MSR;
}

/*!
 * @brief Clears the LPI2C master status bitmap state.
 *
 * @param[in] p_base Module base pointer of type LPI2C_t.
 * @param[in] status_bm bitmap for w1c interrupt event.
 */
static inline void
lpi2c_clear_master_status_bm(LPI2C_t *p_base, uint32_t status_bm)
{
    p_base->MSR = status_bm;
}

/*!
 * @brief Enable/disable receive data DMA requests
 *
 * This function enables or disables generation of Rx DMA requests when data
 * can be read from the receive FIFO, as configured by the receive FIFO watermark.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] b_enable  specifies whether to enable or disable DMA requests
 */
static inline void
lpi2c_set_master_rx_dma(LPI2C_t *p_base, bool b_enable)
{
    uint32_t reg_val = (uint32_t)p_base->MDER;
    reg_val &= (uint32_t)(~(LPI2C_MDER_RDDE_MASK));
    reg_val |= LPI2C_MDER_RDDE(b_enable);
    p_base->MDER = (uint32_t)reg_val;
}

/*!
 * @brief Enable/disable transmit data DMA requests
 *
 * This function enables or disables generation of Tx DMA requests when data
 * can be written to the transmit FIFO, as configured by the transmit FIFO watermark.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] b_enable  specifies whether to enable or disable DMA requests
 */
static inline void
lpi2c_set_master_tx_dma(LPI2C_t *p_base, bool b_enable)
{
    uint32_t reg_val = (uint32_t)p_base->MDER;
    reg_val &= (uint32_t)(~(LPI2C_MDER_TDDE_MASK));
    reg_val |= LPI2C_MDER_TDDE(b_enable);
    p_base->MDER = (uint32_t)reg_val;
}

/*!
 * @brief Set the pin mode of the module
 *
 * This function sets the pin mode of the module. See type lpi2c_pin_config_t for
 * a description of available modes.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] configuration  pin mode of the module
 */
static inline void
lpi2c_set_master_pin_config(LPI2C_t *p_base, lpi2c_pin_config_t configuration)
{
    uint32_t tmp = p_base->MCFGR1;
    tmp &= ~(LPI2C_MCFGR1_PINCFG_MASK);
    tmp |= LPI2C_MCFGR1_PINCFG(configuration);
    p_base->MCFGR1 = tmp;
}

/*!
 * @brief Configure the reaction of the module on NACK reception
 *
 * This function configures how the LPI2C master reacts when receiving a NACK. NACK responses can
 * be treated normally or ignored. In Ultra-Fast mode it is necessary to configure the module to
 * ignore NACK responses.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] configuration  set reaction of the module on NACK reception
 */
static inline void
lpi2c_set_master_nack_config(LPI2C_t *p_base, lpi2c_nack_config_t configuration)
{
    uint32_t reg_val = (uint32_t)p_base->MCFGR1;
    reg_val &= (uint32_t)(~(LPI2C_MCFGR1_IGNACK_MASK));
    reg_val |= LPI2C_MCFGR1_IGNACK(configuration);
    p_base->MCFGR1 = (uint32_t)reg_val;
}

/*!
 * @brief Configure the LPI2C master prescaler
 *
 * This function configures the clock prescaler used for all LPI2C master logic,
 * except the digital glitch filters.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] prescaler  LPI2C master prescaler
 */
static inline void
lpi2c_set_master_prescaler(LPI2C_t *p_base, lpi2c_master_prescaler_t prescaler)
{
    uint32_t tmp = p_base->MCFGR1;
    tmp &= ~(LPI2C_MCFGR1_PRESCALE_MASK);
    tmp |= LPI2C_MCFGR1_PRESCALE(prescaler);
    p_base->MCFGR1 = tmp;
}

/*!
 * @brief Return the LPI2C master prescaler
 *
 * This function returns the currently configured clock prescaler.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  LPI2C master prescaler
 */
static inline lpi2c_master_prescaler_t
lpi2c_get_master_prescaler(const LPI2C_t *p_base)
{
    uint32_t tmp = p_base->MCFGR1;
    tmp          = (tmp & LPI2C_MCFGR1_PRESCALE_MASK) >> LPI2C_MCFGR1_PRESCALE_SHIFT;
    return (lpi2c_master_prescaler_t)tmp;
}

/*!
 * @brief Set the minimum clock high period
 *
 * This function configures the minimum number of cycles (minus one) that the
 * SCL clock is driven high by the master. The SCL high time is extended by the
 * time it takes to detect a rising edge on the external SCL pin. Ignoring any
 * additional board delay due to external loading, this is equal to
 * (2 + FILTSCL) / 2^PRESCALE cycles.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] value  minimum clock high period
 */
static inline void
lpi2c_set_master_clk_high_period(LPI2C_t *p_base, uint8_t value)
{
    uint32_t tmp = p_base->MCCR0;
    tmp &= ~(LPI2C_MCCR0_CLKHI_MASK);
    tmp |= LPI2C_MCCR0_CLKHI(value);
    p_base->MCCR0 = tmp;
}

/*!
 * @brief Return the configured minimum clock high period
 *
 * This function returns the currently configured value for clock high period.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  minimum clock high period
 */
static inline uint8_t
lpi2c_get_master_clk_high_period(const LPI2C_t *p_base)
{
    uint32_t tmp = p_base->MCCR0;
    tmp          = (tmp & LPI2C_MCCR0_CLKHI_MASK) >> LPI2C_MCCR0_CLKHI_SHIFT;
    return (uint8_t)tmp;
}

/*!
 * @brief Set the minimum clock low period
 *
 * This function configures the minimum number of cycles (minus one) that the
 * SCL clock is driven low by the master. This value is also used for the
 * minimum bus free time between a STOP and a START condition.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] value  minimum clock low period
 */
static inline void
lpi2c_set_master_clk_low_period(LPI2C_t *p_base, uint8_t value)
{
    uint32_t tmp = p_base->MCCR0;
    tmp &= ~(LPI2C_MCCR0_CLKLO_MASK);
    tmp |= LPI2C_MCCR0_CLKLO(value);
    p_base->MCCR0 = tmp;
}

/*!
 * @brief Return the configured minimum clock low period
 *
 * This function returns the currently configured value for clock low period.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  minimum clock low period
 */
static inline uint8_t
lpi2c_get_master_clk_low_period(const LPI2C_t *p_base)
{
    uint32_t tmp = p_base->MCCR0;
    tmp          = (tmp & LPI2C_MCCR0_CLKLO_MASK) >> LPI2C_MCCR0_CLKLO_SHIFT;
    return (uint8_t)tmp;
}

/*!
 * @brief Set the data hold time for SDA
 *
 * This function sets the minimum number of cycles (minus one) that is used as the
 * data hold time for SDA. Must be configured less than the minimum SCL low period.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] value  value of the data hold time for SDA
 */
static inline void
lpi2c_set_master_data_valid_delay(LPI2C_t *p_base, uint8_t value)
{
    uint32_t tmp = p_base->MCCR0;
    tmp &= ~(LPI2C_MCCR0_DATAVD_MASK);
    tmp |= LPI2C_MCCR0_DATAVD(value);
    p_base->MCCR0 = tmp;
}

/*!
 * @brief Set the setup and hold delay for a START / STOP condition
 *
 * This function configures the Minimum number of cycles (minus one) that is used
 * by the master as the setup and hold time for a (repeated) START condition and setup
 * time for a STOP condition. The setup time is extended by the time it takes to detect
 * a rising edge on the external SCL pin. Ignoring any additional board delay due to
 * external loading, this is equal to (2 + FILTSCL) / 2^PRESCALE cycles.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] value  setup and hold time for a START / STOP condition
 */
static inline void
lpi2c_set_master_setup_hold_delay(LPI2C_t *p_base, uint8_t value)
{
    uint32_t tmp = p_base->MCCR0;
    tmp &= ~(LPI2C_MCCR0_SETHOLD_MASK);
    tmp |= LPI2C_MCCR0_SETHOLD(value);
    p_base->MCCR0 = tmp;
}

#if (LPI2C_HAS_HIGH_SPEED_MODE)
/*!
 * @brief Set the minimum clock high period in high-speed mode
 *
 * This function configures the minimum number of cycles (minus one) that the
 * SCL clock is driven high by the master. The SCL high time is extended by the
 * time it takes to detect a rising edge on the external SCL pin. Ignoring any
 * additional board delay due to external loading, this is equal to
 * (2 + FILTSCL) / 2^PRESCALE cycles.
 * This setting only has effect during High-Speed mode transfers.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] value  minimum clock high period
 */
static inline void
lpi2c_set_master_clk_high_period_hs(LPI2C_t *p_base, uint8_t value)
{
    uint32_t tmp = p_base->MCCR1;
    tmp &= ~(LPI2C_MCCR1_CLKHI_MASK);
    tmp |= LPI2C_MCCR1_CLKHI(value);
    p_base->MCCR1 = tmp;
}
#endif

#if (LPI2C_HAS_HIGH_SPEED_MODE)
/*!
 * @brief Return the configured minimum clock high period in high-speed mode
 *
 * This function returns the currently configured value for clock high period
 * in high-speed mode.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  minimum clock high period
 */
static inline uint8_t
lpi2c_get_master_clk_high_period_hs(const LPI2C_t *p_base)
{
    uint32_t tmp = p_base->MCCR1;
    tmp          = (tmp & LPI2C_MCCR1_CLKHI_MASK) >> LPI2C_MCCR1_CLKHI_SHIFT;
    return (uint8_t)tmp;
}
#endif

#if (LPI2C_HAS_HIGH_SPEED_MODE)
/*!
 * @brief Set the minimum clock low period in high-speed mode
 *
 * This function configures the minimum number of cycles (minus one) that the
 * SCL clock is driven low by the master. This value is also used for the
 * minimum bus free time between a STOP and a START condition.
 * This setting only has effect during High-Speed mode transfers.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] value  minimum clock low period
 */
static inline void
lpi2c_set_master_clk_low_period_hs(LPI2C_t *p_base, uint8_t value)
{
    uint32_t tmp = p_base->MCCR1;
    tmp &= ~(LPI2C_MCCR1_CLKLO_MASK);
    tmp |= LPI2C_MCCR1_CLKLO(value);
    p_base->MCCR1 = tmp;
}
#endif

#if (LPI2C_HAS_HIGH_SPEED_MODE)
/*!
 * @brief Return the configured minimum clock low period in high-speed mode
 *
 * This function returns the currently configured value for clock low period
 * in high-speed mode.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  minimum clock low period
 */
static inline uint8_t
lpi2c_get_master_clk_low_period_hs(const LPI2C_t *p_base)
{
    uint32_t tmp = p_base->MCCR1;
    tmp          = (tmp & LPI2C_MCCR1_CLKLO_MASK) >> LPI2C_MCCR1_CLKLO_SHIFT;
    return (uint8_t)tmp;
}
#endif

#if (LPI2C_HAS_HIGH_SPEED_MODE)
/*!
 * @brief Set the data hold time for SDA in high-speed mode
 *
 * This function sets the minimum number of cycles (minus one) that is used as the
 * data hold time for SDA in High-Speed mode. Must be configured less than the
 * minimum SCL low period.
 * This setting only has effect during High-Speed mode transfers.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] value  value of the data hold time for SDA
 */
static inline void
lpi2c_set_master_data_valid_delay_hs(LPI2C_t *p_base, uint8_t value)
{
    uint32_t tmp = p_base->MCCR1;
    tmp &= ~(LPI2C_MCCR1_DATAVD_MASK);
    tmp |= LPI2C_MCCR1_DATAVD(value);
    p_base->MCCR1 = tmp;
}
#endif

#if (LPI2C_HAS_HIGH_SPEED_MODE)
/*!
 * @brief Set the setup and hold time for a START / STOP condition in high-speed mode
 *
 * This function configures the Minimum number of cycles (minus one) that is used
 * by the master as the setup and hold time for a (repeated) START condition and setup
 * time for a STOP condition. The setup time is extended by the time it takes to detect
 * a rising edge on the external SCL pin. Ignoring any additional board delay due to
 * external loading, this is equal to (2 + FILTSCL) / 2^PRESCALE cycles.
 * This setting only has effect during High-Speed mode transfers.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] value  setup and hold time for a START / STOP condition
 */
static inline void
lpi2c_set_master_setup_hold_delay_hs(LPI2C_t *p_base, uint8_t value)
{
    uint32_t tmp = p_base->MCCR1;
    tmp &= ~(LPI2C_MCCR1_SETHOLD_MASK);
    tmp |= LPI2C_MCCR1_SETHOLD(value);
    p_base->MCCR1 = tmp;
}
#endif

/*!
 * @brief Set the receive FIFO watermark
 *
 * This function configures the receive FIFO watermark. Whenever the number of words in the receive
 * FIFO is greater than the receive FIFO watermark, a receive data ready event is generated.
 * Writing a value equal or greater than the FIFO size will be truncated.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] value  number of words in the receive FIFO that will cause the receive data flag to be
 * set
 */
static inline void
lpi2c_set_master_rx_watermark(LPI2C_t *p_base, uint16_t value)
{
    uint32_t tmp = p_base->MFCR;
    tmp &= ~(LPI2C_MFCR_RXWATER_MASK);
    tmp |= LPI2C_MFCR_RXWATER(value);
    p_base->MFCR = tmp;
}

/*!
 * @brief Return the configured receive FIFO watermark
 *
 * This function returns the currently configured value for receive FIFO watermark
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  number of words in the receive FIFO that will cause the receive data flag to be set
 */
static inline uint16_t
lpi2c_get_master_rx_watermark(const LPI2C_t *p_base)
{
    uint32_t tmp = p_base->MFCR;
    tmp          = (tmp & LPI2C_MFCR_RXWATER_MASK) >> LPI2C_MFCR_RXWATER_SHIFT;
    return (uint16_t)tmp;
}

/*!
 * @brief Set the transmit FIFO watermark
 *
 * This function configures the transmit FIFO watermark. Whenever the number of words in the
 * transmit FIFO is greater than the transmit FIFO watermark, a transmit data request event is
 * generated. Writing a value equal or greater than the FIFO size will be truncated.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] value  number of words in the transmit FIFO that will cause the transmit data flag to
 * be set
 */
static inline void
lpi2c_set_master_tx_watermark(LPI2C_t *p_base, uint16_t value)
{
    uint32_t tmp = p_base->MFCR;
    tmp &= ~(LPI2C_MFCR_TXWATER_MASK);
    tmp |= LPI2C_MFCR_TXWATER(value);
    p_base->MFCR = tmp;
}

/*!
 * @brief Return the number of words in the receive FIFO
 *
 * This function returns the number of words currently available in the receive FIFO.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  the number of words in the receive FIFO
 */
static inline uint16_t
lpi2c_get_master_rx_fifo_count(const LPI2C_t *p_base)
{
    uint32_t tmp = p_base->MFSR;
    tmp          = (tmp & LPI2C_MFSR_RXCOUNT_MASK) >> LPI2C_MFSR_RXCOUNT_SHIFT;
    return (uint16_t)tmp;
}

/*!
 * @brief Return the number of words in the transmit FIFO
 *
 * This function returns the number of words currently available in the transmit FIFO.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  the number of words in the transmit FIFO
 */
static inline uint16_t
lpi2c_get_master_tx_fifo_count(const LPI2C_t *p_base)
{
    uint32_t tmp = p_base->MFSR;
    tmp          = (tmp & LPI2C_MFSR_TXCOUNT_MASK) >> LPI2C_MFSR_TXCOUNT_SHIFT;
    return (uint16_t)tmp;
}

/*!
 * @brief Provide commands and data for the LPI2C master
 *
 * This function stores commands and data in the transmit FIFO and increments the FIFO
 * write pointer.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] cmd  command for the LPI2C master
 * @param[in] data  data for the LPI2C master
 */
static inline void
lpi2c_set_master_queue_cmd_xmt(LPI2C_t *p_base, lpi2c_master_command_t cmd, uint8_t data)
{
    p_base->MTDR = ((uint32_t)cmd << 8U) + (uint32_t)data;
}

/*!
 * @brief Return the received data
 *
 * This function returns data received by the I2C master that has not been discarded
 * due to data match settings or active command, and increments the FIFO read pointer.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  data received by the LPI2C master
 */
static inline uint8_t
lpi2c_get_master_rcvd_data(const LPI2C_t *p_base)
{
    uint32_t tmp = p_base->MRDR;
    tmp          = (tmp & LPI2C_MRDR_DATA_MASK) >> LPI2C_MRDR_DATA_SHIFT;
    return (uint8_t)tmp;
}

/*!
 * @brief Set the timeout for bus idle for Master
 *
 * This function sets time out for bus idle for Master.If both SCL and SDA are high for longer than
 * Timeout cycles, then the I2C bus is assumed to be idle and the master can generate a START
 * condition
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] timeout   bus idle timeout period. Zero means no bus idle timeout
 */
static inline void
lpi2c_set_master_bus_idle_timeout(LPI2C_t *p_base, uint16_t timeout)
{
    uint32_t tmp = p_base->MCFGR2;
    tmp &= ~(LPI2C_MCFGR2_BUSIDLE_MASK);
    tmp |= LPI2C_MCFGR2_BUSIDLE(timeout);
    p_base->MCFGR2 = tmp;
}

/*!
 * @brief Set/clear the slave reset command
 *
 * Calling this function with enable parameter set to true will perform a software
 * reset of the LPI2C slave.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] b_enable  specifies the reset state of the LPI2C slave logic
 */
static inline void
lpi2c_set_slave_sw_reset(LPI2C_t *p_base, bool b_enable)
{
    uint32_t reg_val = (uint32_t)p_base->SCR;
    reg_val &= (uint32_t)(~(LPI2C_SCR_RST_MASK));
    reg_val |= LPI2C_SCR_RST(b_enable);
    p_base->SCR = (uint32_t)reg_val;
}

/*!
 * @brief Enable or disable the LPI2C slave
 *
 * This function enables or disables the LPI2C module in slave mode.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] b_enable  specifies whether to enable or disable the LPI2C slave
 */
static inline void
lpi2c_set_slave_enable(LPI2C_t *p_base, bool b_enable)
{
    uint32_t reg_val = (uint32_t)p_base->SCR;
    reg_val &= (uint32_t)(~(LPI2C_SCR_SEN_MASK));
    reg_val |= LPI2C_SCR_SEN(b_enable);
    p_base->SCR = (uint32_t)reg_val;
}

#if (LPI2C_HAS_ULTRA_FAST_MODE)
/*!
 * @brief Check the detection of a FIFO overflow or underflow
 *
 * This function checks for the occurrence of a slave FIFO overflow or underflow.
 * This event can only occur if clock stretching is disabled.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  indication of a FIFO overflow or underflow
 */
static inline bool
lpi2c_get_slave_fifo_error_event(const LPI2C_t *p_base)
{
    uint32_t reg_val = (uint32_t)p_base->SSR;
    reg_val          = (reg_val & LPI2C_SSR_FEF_MASK) >> LPI2C_SSR_FEF_SHIFT;
    return (bool)reg_val;
}
#endif

/*!
 * @brief Check the detection of a bit error
 *
 * This function checks for the occurrence of a bit error event. This event occurs
 * if the LPI2C slave transmits a logic one and detects a logic zero on the I2C bus. The
 * slave will ignore the rest of the transfer until the next (repeated) START condition.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  indication of a bit error
 */
static inline bool
lpi2c_get_slave_bit_error_event(const LPI2C_t *p_base)
{
    uint32_t reg_val = (uint32_t)p_base->SSR;
    reg_val          = (reg_val & LPI2C_SSR_BEF_MASK) >> LPI2C_SSR_BEF_SHIFT;
    return (bool)reg_val;
}

/*!
 * @brief Check the detection of a STOP condition
 *
 * This function checks for the detection of a STOP condition, after the LPI2C slave
 * matched the last address byte.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  indication of a STOP condition
 */
static inline bool
lpi2c_get_slave_stop_det_event(const LPI2C_t *p_base)
{
    uint32_t reg_val = (uint32_t)p_base->SSR;
    reg_val          = (reg_val & LPI2C_SSR_SDF_MASK) >> LPI2C_SSR_SDF_SHIFT;
    return (bool)reg_val;
}

/*!
 * @brief Check the detection of a repeated START condition
 *
 * This function checks for the detection of a repeated START condition, after
 * the LPI2C slave matched the last address byte. This event does not occur
 * when the slave first detects a START condition.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  indication of a repeated START condition
 */
static inline bool
lpi2c_get_slave_rept_start_event(const LPI2C_t *p_base)
{
    uint32_t reg_val = (uint32_t)p_base->SSR;
    reg_val          = (reg_val & LPI2C_SSR_RSF_MASK) >> LPI2C_SSR_RSF_SHIFT;
    return (bool)reg_val;
}

/*!
 * @brief Check the validity of the Address Status Register
 *
 * This function checks for the detection of a valid address. The event is
 * cleared by reading the address - see function lpi2c_get_slave_rcvd_addr().
 * It can also be cleared by reading the data register, when data register has
 * been configured to allow address reads.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  indication of the validity of the Address Status Register
 */
static inline bool
lpi2c_get_slave_addr_valid_event(const LPI2C_t *p_base)
{
    uint32_t reg_val = (uint32_t)p_base->SSR;
    reg_val          = (reg_val & LPI2C_SSR_AVF_MASK) >> LPI2C_SSR_AVF_SHIFT;
    return (bool)reg_val;
}

/*!
 * @brief Check the availability of receive data
 *
 * This function checks for the availability of data received by the I2C slave.
 * The event is cleared by reading the received data - see function
 * lpi2c_get_slave_rcvd_data(). The event is not cleared by calling
 * lpi2c_get_slave_rcvd_data() if the data register is configured to allow address
 * reads and an address valid event is active.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  indication of receive data availability
 */
static inline bool
lpi2c_get_slave_rcv_data_event(const LPI2C_t *p_base)
{
    uint32_t reg_val = (uint32_t)p_base->SSR;
    reg_val          = (reg_val & LPI2C_SSR_RDF_MASK) >> LPI2C_SSR_RDF_SHIFT;
    return (bool)reg_val;
}

/*!
 * @brief Check if transmit data is requested
 *
 * This function checks if the LPI2C slave requests data to transmit. The
 * event is cleared by providing transmit data - see function
 * lpi2c_set_slave_xmt_data(). The event can also be automatically cleared
 * if the LPI2C module detects a NACK or a repeated START or STOP condition
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  indication of a transmit data request
 */
static inline bool
lpi2c_get_slave_xmt_data_event(const LPI2C_t *p_base)
{
    uint32_t reg_val = (uint32_t)p_base->SSR;
    reg_val          = (reg_val & LPI2C_SSR_TDF_MASK) >> LPI2C_SSR_TDF_SHIFT;
    return (bool)reg_val;
}

#if (LPI2C_HAS_ULTRA_FAST_MODE)
/*!
 * @brief Clear the FIFO overflow or underflow flag
 *
 * This function clears the FIFO overflow or underflow event.
 *
 * @param[in] p_base  base address of the LPI2C module
 */
static inline void
lpi2c_clear_slave_fifo_error_event(LPI2C_t *p_base)
{
    p_base->SSR = ((uint32_t)1U << LPI2C_SSR_FEF_SHIFT);
}
#endif

/*!
 * @brief Clear bit error flag
 *
 * This function clears the bit error event.
 *
 * @param[in] p_base  base address of the LPI2C module
 */
static inline void
lpi2c_clear_slave_bit_error_event(LPI2C_t *p_base)
{
    p_base->SSR = ((uint32_t)1U << LPI2C_SSR_BEF_SHIFT);
}

/*!
 * @brief Clear the STOP detect flag
 *
 * This function clears the STOP detect event.
 *
 * @param[in] p_base  base address of the LPI2C module
 */
static inline void
lpi2c_clear_slave_stop_det_event(LPI2C_t *p_base)
{
    p_base->SSR = ((uint32_t)1U << LPI2C_SSR_SDF_SHIFT);
}

/*!
 * @brief Clear the repeated START detect flag
 *
 * This function clears the repeated START detect event.
 *
 * @param[in] p_base  base address of the LPI2C module
 */
static inline void
lpi2c_clear_slave_rept_start_event(LPI2C_t *p_base)
{
    p_base->SSR = ((uint32_t)1U << LPI2C_SSR_RSF_SHIFT);
}

/*!
 * @brief Enable or disable specified LPI2C slave interrupts
 *
 * This function can enable or disable one or more slave interrupt sources
 * specified by the interrupts parameter.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] interrupts  interrupts to be enabled or disabled;
 *  must be a bitwise or between one or more of the following constants:
 *  - LPI2C_SLAVE_SMBUS_ALERT_RESPONSE  - SMBus Alert Response Interrupt
 *  - LPI2C_SLAVE_GENERAL_CALL          - General Call Interrupt
 *  - LPI2C_SLAVE_ADDRESS_MATCH_1       - Address Match 1 Interrupt
 *  - LPI2C_SLAVE_ADDRESS_MATCH_0       - Address Match 0 Interrupt
 *  - LPI2C_SLAVE_FIFO_ERROR            - FIFO Error Interrupt
 *  - LPI2C_SLAVE_BIT_ERROR             - Bit Error Interrupt
 *  - LPI2C_SLAVE_STOP_DETECT           - STOP Detect Interrupt
 *  - LPI2C_SLAVE_REPEATED_START        - Repeated Start Interrupt
 *  - LPI2C_SLAVE_TRANSMIT_ACK          - Transmit ACK Interrupt
 *  - LPI2C_SLAVE_ADDRESS_VALID         - Address Valid Interrupt
 *  - LPI2C_SLAVE_RECEIVE_DATA          - Receive Data Interrupt
 *  - LPI2C_SLAVE_TRANSMIT_DATA         - Transmit Data Interrupt
 * @param[in] b_enable  specifies whether to enable or disable specified interrupts
 */
static inline void
lpi2c_set_slave_int(LPI2C_t *p_base, uint32_t interrupts, bool b_enable)
{
    uint32_t tmp = p_base->SIER;

    if (b_enable == true)
    {
        tmp |= interrupts;
    }
    else
    {
        tmp &= ~interrupts;
    }
    p_base->SIER = tmp;
}

/*!
 * @brief Return the state of the specified LPI2C slave interrupt
 *
 * This function returns the enabled/disabled state of the slave interrupt
 * source specified by the interrupt parameter.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] interrupts  interrupt for which the check is made;
 *  must be one of the following constants:
 *  - LPI2C_SLAVE_SMBUS_ALERT_RESPONSE  - SMBus Alert Response Interrupt
 *  - LPI2C_SLAVE_GENERAL_CALL          - General Call Interrupt
 *  - LPI2C_SLAVE_ADDRESS_MATCH_1       - Address Match 1 Interrupt
 *  - LPI2C_SLAVE_ADDRESS_MATCH_0       - Address Match 0 Interrupt
 *  - LPI2C_SLAVE_FIFO_ERROR            - FIFO Error Interrupt
 *  - LPI2C_SLAVE_BIT_ERROR             - Bit Error Interrupt
 *  - LPI2C_SLAVE_STOP_DETECT           - STOP Detect Interrupt
 *  - LPI2C_SLAVE_REPEATED_START        - Repeated Start Interrupt
 *  - LPI2C_SLAVE_TRANSMIT_ACK          - Transmit ACK Interrupt
 *  - LPI2C_SLAVE_ADDRESS_VALID         - Address Valid Interrupt
 *  - LPI2C_SLAVE_RECEIVE_DATA          - Receive Data Interrupt
 *  - LPI2C_SLAVE_TRANSMIT_DATA         - Transmit Data Interrupt
 * @return  enable/disable state of specified interrupt
 */
static inline bool
lpi2c_get_slave_int(const LPI2C_t *p_base, uint32_t interrupts)
{
    uint32_t tmp           = p_base->SIER;
    bool     hasInterrupts = false;

    if ((tmp & interrupts) != (uint32_t)0U)
    {
        hasInterrupts = true;
    }

    return hasInterrupts;
}

/*!
 * @brief Gets the LPI2C slave status bitmap state.
 *
 * This function returns the state of bitmap of the LPI2C slave status at the moment of intrrupt
 * event.
 *
 * @param[in] p_base Module base pointer of type LPI2C_t.
 * @return State of bitmap of the status.
 */
static inline uint32_t
lpi2c_get_slave_status_bm(LPI2C_t *p_base)
{
    return p_base->SSR;
}

/*!
 * @brief Clears the LPI2C slave status bitmap state.
 *
 * @param[in] p_base Module base pointer of type LPI2C_t.
 * @param[in] status_bm bitmap for w1c interrupt event.
 */
static inline void
lpi2c_clear_slave_status_bm(LPI2C_t *p_base, uint32_t status_bm)
{
    p_base->SSR = status_bm;
}

/*!
 * @brief Enable/disable slave receive data DMA requests
 *
 * This function enables or disables generation of Rx DMA requests when received
 * data is available.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] b_enable  specifies whether to enable or disable receive data DMA requests
 */
static inline void
lpi2c_set_slave_rx_dma(LPI2C_t *p_base, bool b_enable)
{
    uint32_t reg_val = (uint32_t)p_base->SDER;
    reg_val &= (uint32_t)(~(LPI2C_SDER_RDDE_MASK));
    reg_val |= LPI2C_SDER_RDDE(b_enable);
    p_base->SDER = (uint32_t)reg_val;
}

/*!
 * @brief Enable/disable slave transmit data DMA requests
 *
 * This function enables or disables generation of Tx DMA requests when the module
 * requires more data to transmit.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] b_enable  specifies whether to enable or disable transmit data DMA requests
 */
static inline void
lpi2c_set_slave_tx_dma(LPI2C_t *p_base, bool b_enable)
{
    uint32_t reg_val = (uint32_t)p_base->SDER;
    reg_val &= (uint32_t)(~(LPI2C_SDER_TDDE_MASK));
    reg_val |= LPI2C_SDER_TDDE(b_enable);
    p_base->SDER = (uint32_t)reg_val;
}

/*!
 * @brief Control address match configuration
 *
 * This function configures the condition that will cause an address match to
 * occur. See type lpi2c_slave_addr_config_t for a description of available options.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] configuration  configures the condition that will cause an address to match
 */
static inline void
lpi2c_set_slave_addr_config(LPI2C_t *p_base, lpi2c_slave_addr_config_t configuration)
{
    uint32_t tmp = p_base->SCFGR1;
    tmp &= ~(LPI2C_SCFGR1_ADDRCFG_MASK);
    tmp |= LPI2C_SCFGR1_ADDRCFG(configuration);
    p_base->SCFGR1 = tmp;
}

/*!
 * @brief Control detection of the High-speed Mode master code
 *
 * This function enables or disables the detection of the High-speed Mode
 * master code of slave address 0000_1XX, but does not cause an address match
 * on this code. When set and any Hs-mode master code is detected, the slave
 * filter and ACK stalls are disabled until the next STOP condition is detected.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] b_enable  enable/disable the detection of the High-speed Mode master code
 */
static inline void
lpi2c_set_slave_hs_mode_det(LPI2C_t *p_base, bool b_enable)
{
    uint32_t reg_val = (uint32_t)p_base->SCFGR1;
    reg_val &= (uint32_t)(~(LPI2C_SCFGR1_HSMEN_MASK));
    reg_val |= LPI2C_SCFGR1_HSMEN(b_enable);
    p_base->SCFGR1 = (uint32_t)reg_val;
}

/*!
 * @brief Control slave behaviour when NACK is detected
 *
 * This function controls the option to ignore received NACKs. When enabled, the
 * LPI2C slave will continue transfers after a NACK is detected. This option is needed
 * for Ultra-Fast mode.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] nack_config  slave behaviour when NACK is detected
 */
static inline void
lpi2c_set_slave_ignore_nack(LPI2C_t *p_base, lpi2c_slave_nack_config_t nack_config)
{
    uint32_t reg_val = (uint32_t)p_base->SCFGR1;
    reg_val &= (uint32_t)(~(LPI2C_SCFGR1_IGNACK_MASK));
    reg_val |= LPI2C_SCFGR1_IGNACK(nack_config);
    p_base->SCFGR1 = (uint32_t)reg_val;
}

/*!
 * @brief Enable or disable clock stretching for the sending of the ACK bit
 *
 * This function enables or disables SCL clock stretching during slave-transmit address
 * byte(s) and slave-receiver address and data byte(s) to allow software to write the
 * Transmit ACK Register before the ACK or NACK is transmitted. Clock stretching occurs
 * when transmitting the 9th bit and is therefore not compatible with high speed mode.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] b_enable  enable or disable clock stretching
 */
static inline void
lpi2c_set_slave_ack_stall(LPI2C_t *p_base, bool b_enable)
{
    uint32_t reg_val = (uint32_t)p_base->SCFGR1;
    reg_val &= (uint32_t)(~(LPI2C_SCFGR1_ACKSTALL_MASK));
    reg_val |= LPI2C_SCFGR1_ACKSTALL(b_enable);
    p_base->SCFGR1 = (uint32_t)reg_val;
}

/*!
 * @brief Enable or disable clock stretching for data transmission
 *
 * This function enables or disables SCL clock stretching when the transmit data
 * flag is set during a slave-transmit transfer. Clock stretching occurs following
 * the 9th bit and is therefore compatible with high speed mode.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] b_enable  enable or disable clock stretching
 */
static inline void
lpi2c_set_slave_tx_data_stall(LPI2C_t *p_base, bool b_enable)
{
    uint32_t reg_val = (uint32_t)p_base->SCFGR1;
    reg_val &= (uint32_t)(~(LPI2C_SCFGR1_TXDSTALL_MASK));
    reg_val |= LPI2C_SCFGR1_TXDSTALL(b_enable);
    p_base->SCFGR1 = (uint32_t)reg_val;
}

/*!
 * @brief Enable or disable clock stretching for data reception
 *
 * This function enables or disables SCL clock stretching when receive data flag
 * is set during a slave-receive transfer. Clock stretching occurs following the 9th
 * bit and is therefore compatible with high speed mode.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] b_enable  enable or disable clock stretching
 */
static inline void
lpi2c_set_slave_rx_stall(LPI2C_t *p_base, bool b_enable)
{
    uint32_t reg_val = (uint32_t)p_base->SCFGR1;
    reg_val &= (uint32_t)(~(LPI2C_SCFGR1_RXSTALL_MASK));
    reg_val |= LPI2C_SCFGR1_RXSTALL(b_enable);
    p_base->SCFGR1 = (uint32_t)reg_val;
}

/*!
 * @brief Enable or disable clock stretching for valid address reception
 *
 * This function enables or disables SCL clock stretching when the address valid
 * flag is asserted. Clock stretching only occurs following the 9th bit and is
 * therefore compatible with high speed mode.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] b_enable  enable or disable clock stretching
 */
static inline void
lpi2c_set_slave_addr_stall(LPI2C_t *p_base, bool b_enable)
{
    uint32_t reg_val = (uint32_t)p_base->SCFGR1;
    reg_val &= (uint32_t)(~(LPI2C_SCFGR1_ADRSTALL_MASK));
    reg_val |= LPI2C_SCFGR1_ADRSTALL(b_enable);
    p_base->SCFGR1 = (uint32_t)reg_val;
}

/*!
 * @brief Configure the ADDR0 address for slave address match
 *
 * This function configures the ADDR0 value which is used to validate the received
 * slave address. In 10-bit mode, the first address byte is compared to
 * { 11110, ADDR0[10:9] } and the second address byte is compared to ADDR0[8:1].
 * In 7-bit mode, the address is compared to ADDR0[7:1]
 * The formula used for address validation is configured with function
 * lpi2c_set_slave_addr_config().
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] addr  ADDR0 address for slave address match
 */
static inline void
lpi2c_set_slave_match_addr0(LPI2C_t *p_base, uint16_t addr)
{
    uint32_t tmp = p_base->SAMR;
    tmp &= ~(LPI2C_SAMR_ADDR0_MASK);
    tmp |= LPI2C_SAMR_ADDR0(addr);
    p_base->SAMR = tmp;
}

/*!
 * @brief Return the received slave address
 *
 * This function returns the received slave address. Reading the address clears
 * the address valid event. The address can be 7-bit or 10-bit (10-bit addresses
 * are prefixed by 11110) and includes the R/W bit in the least significant position.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  received address
 */
static inline uint16_t
lpi2c_get_slave_rcvd_addr(const LPI2C_t *p_base)
{
    uint32_t tmp = p_base->SASR;
    tmp          = (tmp & LPI2C_SASR_RADDR_MASK) >> LPI2C_SASR_RADDR_SHIFT;
    return (uint16_t)tmp;
}

/*!
 * @brief Configure the ACK/NACK transmission after a received byte
 *
 * This function can be used to instruct the LPI2C slave whether to send an ACK or
 * a NACK after receiving a byte. When ACK stall is enabled this function must be
 * called after each matching address and after each received data byte. It can also
 * be called when LPI2C Slave is disabled or idle to configure the default ACK/NACK.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] nack  specifies whether to transmit ACK or NACK
 */
static inline void
lpi2c_set_slave_xmt_nack(LPI2C_t *p_base, lpi2c_slave_nack_transmit_t nack)
{
    uint32_t reg_val = (uint32_t)p_base->STAR;
    reg_val &= (uint32_t)(~(LPI2C_STAR_TXNACK_MASK));
    reg_val |= LPI2C_STAR_TXNACK(nack);
    p_base->STAR = (uint32_t)reg_val;
}

/*!
 * @brief Provide data for the LPI2C slave transmitter
 *
 * This function provides one byte of data for the LPI2C slave to transmit.
 * Calling this function clears the transmit data event.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @param[in] data  data for the LPI2C slave transmitter
 */
static inline void
lpi2c_set_slave_xmt_data(LPI2C_t *p_base, uint8_t data)
{
    p_base->STDR = (uint32_t)data;
}

/*!
 * @brief Return the data received by the LPI2C slave receiver
 *
 * This function returns the data received by the I2C slave.
 * Calling this function clears the receive data event.
 *
 * @param[in] p_base  base address of the LPI2C module
 * @return  data received by the LPI2C slave receiver
 */
static inline uint8_t
lpi2c_get_slave_rcvd_data(const LPI2C_t *p_base)
{
    uint32_t tmp = p_base->SRDR;
    tmp          = (tmp & LPI2C_SRDR_DATA_MASK) >> LPI2C_SRDR_DATA_SHIFT;
    return (uint8_t)tmp;
}

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* LPI2C_ACCESS_H */

/*** end of file ***/
