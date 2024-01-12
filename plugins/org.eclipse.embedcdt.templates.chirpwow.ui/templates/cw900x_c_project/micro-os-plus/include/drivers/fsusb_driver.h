/**
 * @file fsusb_driver.h
 * @brief The module provides for requiring FSUSB hardwre registers access.
 * Export functions for wide scope use.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef FSUSB_DRIVER_H
#define FSUSB_DRIVER_H

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
/** @brief Device instance number. */
#define INST_FSUSB 0u

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
typedef enum
{
    FSUSB_RESET_FLAG        = FSUSB_UISR_RST_SHIFT,
    FSUSB_CTRL_ARRIVAL_FLAG = FSUSB_UISR_SETUP_SHIFT,
    /* FSUSB_EP0_RX_RDY_FLAG = FSUSB_UISR_EP0RXRDY_SHIFT, */
    FSUSB_EP0_TX_DONE_FLAG = FSUSB_UISR_EP0TXDONE_SHIFT,
    FSUSB_EP1_TX_DONE_FLAG = FSUSB_UISR_EP1TXDONE_SHIFT,
    FSUSB_EP2_TX_DONE_FLAG = FSUSB_UISR_EP2TXDONE_SHIFT,
    FSUSB_EP3_TX_DONE_FLAG = FSUSB_UISR_EP3TXDONE_SHIFT,
    FSUSB_EP4_RX_RDY_FLAG  = FSUSB_UISR_EP4RXRDY_SHIFT,
    FSUSB_EP5_TX_DONE_FLAG = FSUSB_UISR_EP5TXDONE_SHIFT,
    FSUSB_EP6_RX_RDY_FLAG  = FSUSB_UISR_EP6RXRDY_SHIFT,
    FSUSB_ALL_STATUS =
        (FSUSB_UISR_RST_MASK |
         FSUSB_UISR_SETUP_MASK
         /* | FSUSB_UISR_EP0RXRDY_MASK */
         | FSUSB_UISR_EP0TXDONE_MASK | FSUSB_UISR_EP1TXDONE_MASK | FSUSB_UISR_EP2TXDONE_MASK |
         FSUSB_UISR_EP3TXDONE_MASK | FSUSB_UISR_EP4RXRDY_MASK | FSUSB_UISR_EP5TXDONE_MASK |
         FSUSB_UISR_EP6RXRDY_MASK),
} fsusb_status_flag_t;

typedef enum
{
    FSUSB_INT_RESET_BM = FSUSB_INTMSK_RST_MASK,
    FSUSB_INT_SETUP_BM = FSUSB_INTMSK_SETUP_MASK,
    /* FSUSB_INT_EP0RX_BM = FSUSB_INTMSK_EP0RX_MASK, */
    FSUSB_INT_EP0TX_BM = FSUSB_INTMSK_EP0TX_MASK,
    FSUSB_INT_EP1TX_BM = FSUSB_INTMSK_EP1TX_MASK,
    FSUSB_INT_EP2TX_BM = FSUSB_INTMSK_EP2TX_MASK,
    FSUSB_INT_EP3TX_BM = FSUSB_INTMSK_EP3TX_MASK,
    FSUSB_INT_EP4RX_BM = FSUSB_INTMSK_EP4RX_MASK,
    FSUSB_INT_EP5TX_BM = FSUSB_INTMSK_EP5TX_MASK,
    FSUSB_INT_EP6RX_BM = FSUSB_INTMSK_EP6RX_MASK,
    FSUSB_INT_ALL_BM =
        (FSUSB_INTMSK_RST_MASK |
         FSUSB_INTMSK_SETUP_MASK
         /* | FSUSB_INTMSK_EP0RX_MASK */
         | FSUSB_INTMSK_EP0TX_MASK | FSUSB_INTMSK_EP1TX_MASK | FSUSB_INTMSK_EP2TX_MASK |
         FSUSB_INTMSK_EP3TX_MASK | FSUSB_INTMSK_EP4RX_MASK | FSUSB_INTMSK_EP5TX_MASK |
         FSUSB_INTMSK_EP6RX_MASK),
} fsusb_int_bm_t;

typedef enum
{
    FSUSB_EP0_IN_BM  = FSUSB_EPX_EP0IN_MASK,
    FSUSB_EP0_OUT_BM = FSUSB_EPX_EP0OUT_MASK,
    FSUSB_EP1_IN_BM  = FSUSB_EPX_EP1IN_MASK,
    FSUSB_EP2_IN_BM  = FSUSB_EPX_EP2IN_MASK,
    FSUSB_EP3_IN_BM  = FSUSB_EPX_EP3IN_MASK,
    FSUSB_EP4_OUT_BM = FSUSB_EPX_EP4OUT_MASK,
    FSUSB_EP5_IN_BM  = FSUSB_EPX_EP5IN_MASK,
    FSUSB_EP6_OUT_BM = FSUSB_EPX_EP6OUT_MASK,
} fsusb_ep_sts_bm_t;

/*******************************************************************************
 * Exported variables
 ******************************************************************************/
/** @brief Table of base pointers for FSUSB instances. */
extern FSUSB_t *const gp_fsusb_base[FSUSB_INSTANCE_COUNT];

/** @brief Table to save FSUSB IRQ enumeration numbers defined in the CMSIS
 * header file. */
extern const IRQn_t g_fsusb_irq_id[FSUSB_INSTANCE_COUNT];

/** @brief Table to save FSUSB clock names as defined in clock manager. */
extern const clock_names_t g_fsusb_clk_names[FSUSB_INSTANCE_COUNT];

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/
/**
 * @brief To get the start address of SRAM0 buffer.
 *
 * @return uint32_t The address of SRAM0 buffer.
 */
static inline uint32_t
fsusb_get_sram0_buffer_address(void)
{
    return (FSUSB_BASE + FSUSB_SRAM0_MAP_BIAS);
}

/**
 * @brief To get the size of SRAM0 ubffer.
 *
 * @return uint32_t The size of SRAM0 buffer.
 */
static inline uint32_t
fsusb_get_sram0_buffer_size(void)
{
    return FSUSB_SRAM0_BUF_SIZE;
}

/**
 * @brief To get the start address of SRAM1 buffer.
 *
 * @return uint32_t The address of SRAM1 buffer.
 */
static inline uint32_t
fsusb_get_sram1_buffer_address(void)
{
    return (FSUSB_BASE + FSUSB_SRAM1_MAP_BIAS);
}

/**
 * @brief To get the size of SRAM1 ubffer.
 *
 * @return uint32_t The size of SRAM1 buffer.
 */
static inline uint32_t
fsusb_get_sram1_buffer_size(void)
{
    return FSUSB_SRAM1_BUF_SIZE;
}

/**
 * @brief To get the start address of SRAM3 buffer.
 *
 * @return uint32_t The address of SRAM3 buffer.
 */
static inline uint32_t
fsusb_get_sram3_buffer_address(void)
{
    return (FSUSB_BASE + FSUSB_SRAM3_MAP_BIAS);
}

/**
 * @brief To get the size of SRAM3 ubffer.
 *
 * @return uint32_t The size of SRAM3 buffer.
 */
static inline uint32_t
fsusb_get_sram3_buffer_size(void)
{
    return FSUSB_SRAM3_BUF_SIZE;
}

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Initializes the FSUSB module to a known state.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t.
 */
void fsusb_init(FSUSB_t *p_base);

/**
 * @brief Get status of FSUSB.
 *
 * @param[in] p_base - Module base pointer of type FSUSB_t
 * @return All of FSUSB status.
 */
uint32_t fsusb_get_status_bm(FSUSB_t *p_base);

/**
 * @brief Clear status of FSUSB.
 *
 * @param[in] p_base - Module base pointer of type FSUSB_t
 * @param[in] status_bm - All W1C (writer 1 clear)
 */
void fsusb_clear_status_bm(FSUSB_t *p_base, uint32_t status_bm);

/**
 * @brief Enable the FSUSB interrupts.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] bitmap Bit field of type fsusb_int_bm_t to the proper gate of interrupt events.
 */
void fsusb_set_intrpt_enable_bit(FSUSB_t *p_base, fsusb_int_bm_t bitmap);

/**
 * @brief Disable the FSUSB interrupts.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] bitmap Bit field of type fsusb_int_bm_t to the proper gate of interrupt events.
 */
void fsusb_clear_intrpt_enable_bit(FSUSB_t *p_base, fsusb_int_bm_t bitmap);

/**
 * @brief Turn power on suspend on/off.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] b_flag
 *            - true : On.
 *            - false: Off.
 */
void fsusb_set_pwr_on_suspend_on_off(FSUSB_t *p_base, bool b_flag);

/**
 * @brief Get end-points status.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] bitmap Bitmap for each end-point.
 * @return bool Return true if hardware is auto response NAK.
 */
bool fsusb_get_ep_sts_nak(FSUSB_t *p_base, fsusb_ep_sts_bm_t bitmap);

/**
 * @brief Set SRAM0 CS active. SRAM0 is only for end-point 0.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 */
void fsusb_set_sram0_cs_enable_bit(FSUSB_t *p_base);

/**
 * @brief Set SRAM0 CS deactive. SRAM0 is only for end-point 0.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 */
void fsusb_clear_sram0_cs_enable_bit(FSUSB_t *p_base);

/**
 * @brief Set SRAM1 CS active. SRAM1 is respectively used for end-point 5 and 6.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 */
void fsusb_set_sram1_cs_enable_bit(FSUSB_t *p_base);

/**
 * @brief Set SRAM1 CS deactive. SRAM1 is respectively used for end-point 5 and 6.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 */
void fsusb_clear_sram1_cs_enable_bit(FSUSB_t *p_base);

/**
 * @brief Set SRAM3 CS active. SRAM3 is respectively used for end-point 1, 2, 3 and 4.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 */
void fsusb_set_sram3_cs_enable_bit(FSUSB_t *p_base);

/**
 * @brief Set SRAM3 CS deactive. SRAM3 is respectively used for end-point 1, 2, 3 and 4.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 */
void fsusb_clear_sram3_cs_enable_bit(FSUSB_t *p_base);

/**
 * @brief Get bus line state.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return bool Return true if USB bus is powered.
 */
bool fsusb_get_bus_line_st(FSUSB_t *p_base);

/**
 * @brief Get bus connetion status.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return bool Return true if USB protocol handshake is initially steady.
 */
bool fsusb_get_bus_connect_rdy(FSUSB_t *p_base);

/**
 * @brief Set USB engine is ready to dectect bus state.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 */
void fsusb_set_rdy_for_bus_connect(FSUSB_t *p_base);

/**
 * @brief Set USB physical amplifer is ready to work.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 */
void fsusb_set_dp_dm_line_in(FSUSB_t *p_base);

/**
 * @brief Set end-point 0 handshake is working with toggle mode.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] b_flag
 *            - true : On.
 *            - false: Off.
 */
void fsusb_set_ep0_toggle(FSUSB_t *p_base, bool b_flag);

/**
 * @brief Set end-point 0 stall.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] b_flag
 *            - true : Stall.
 *            - false: Normal.
 */
void fsusb_set_ep0_stall(FSUSB_t *p_base, bool b_flag);

/**
 * @brief Get end-point 0 pipe state.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return bool Return true if end-point 0 is stall.
 */
bool fsusb_get_ep0_stall_st(FSUSB_t *p_base);

/**
 * @brief Set end-point 0 sends ACK status with zero byte payload.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 */
void fsusb_set_ep0_zero_send(FSUSB_t *p_base);

/**
 * @brief Get end-point 0 state is ready after sent zero byte payload.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return bool Return true if end-point 0 zero byte payload is done.
 */
bool fsusb_get_ep0_zero_sent_rdy(FSUSB_t *p_base);

/**
 * @brief Set end-point 0 sends data payload.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] byte_count Size of payload.
 */
void fsusb_set_ep0_pkt_send(FSUSB_t *p_base, uint16_t byte_count);

/**
 * @brief Get end-point 0 state is ready after sent data payload.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return bool Return true if end-point 0 sent payload is done.
 */
bool fsusb_get_ep0_pkt_sent_rdy(FSUSB_t *p_base);

/**
 * @brief Clear end-point 0 payload latched state after received completed packet then
 * automatically start to receive next completed payload.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 */
void fsusb_clear_ep0_latched_st(FSUSB_t *p_base);

/**
 * @brief Get end-point 0 received packet length.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return uint8_t Return received packet length.
 */
uint8_t fsusb_get_ep0_rx_pkt_len(FSUSB_t *p_base);

/**
 * @brief Set USB device address in hardware engine for listening to host bus instruction.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] addr_num Be assgined address by host that issued Set Address via control setup packet.
 */
void fsusb_set_usb_dev_addr(FSUSB_t *p_base, uint8_t addr_num);

/**
 * @brief Get USB device address in hardware engine for listening to host bus instruction.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * return uint8_t Current address number in hardware engine.
 */
uint8_t fsusb_get_usb_dev_addr(FSUSB_t *p_base);

/**
 * @brief Get the received data length of USB setup packet via end-point 0.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * return uint8_t Return the number of byte of received request.
 */
uint8_t fsusb_get_usb_dev_req_data_byte(FSUSB_t *p_base, uint8_t byte_order);

/**
 * @brief Set end-point 1 handshake is working with toggle mode.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] b_flag
 *            - true : On.
 *            - false: Off.
 */
void fsusb_set_ep1_toggle(FSUSB_t *p_base, bool b_flag);

/**
 * @brief Set end-point 1 stall.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] b_flag
 *            - true : Stall.
 *            - false: Normal.
 */
void fsusb_set_ep1_stall(FSUSB_t *p_base, bool b_flag);

/**
 * @brief Get end-point 1 pipe state.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return bool Return true if end-point 1 is stall.
 */
bool fsusb_get_ep1_stall_st(FSUSB_t *p_base);

/**
 * @brief Set end-point 1 sends data payload.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] byte_count Size of payload.
 */
void fsusb_set_ep1_pkt_send(FSUSB_t *p_base, uint16_t byte_count);

/**
 * @brief Get end-point 1 state is ready after sent data payload.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return bool Return true if end-point 1 sent payload is done.
 */
bool fsusb_get_ep1_pkt_sent_rdy(FSUSB_t *p_base);

/**
 * @brief Set end-point 2 handshake is working with toggle mode.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] b_flag
 *            - true : On.
 *            - false: Off.
 */
void fsusb_set_ep2_toggle(FSUSB_t *p_base, bool b_flag);

/**
 * @brief Set end-point 2 stall.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] b_flag
 *            - true : Stall.
 *            - false: Normal.
 */
void fsusb_set_ep2_stall(FSUSB_t *p_base, bool b_flag);

/**
 * @brief Get end-point 2 pipe state.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return bool Return true if end-point 1 is stall.
 */
bool fsusb_get_ep2_stall_st(FSUSB_t *p_base);

/**
 * @brief Set end-point 2 sends data payload.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] byte_count Size of payload.
 */
void fsusb_set_ep2_pkt_send(FSUSB_t *p_base, uint16_t byte_count);

/**
 * @brief Get end-point 2 state is ready after sent data payload.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return bool Return true if end-point 2 sent payload is done.
 */
bool fsusb_get_ep2_pkt_sent_rdy(FSUSB_t *p_base);

/**
 * @brief Set end-point 3 handshake is working with toggle mode.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] b_flag
 *            - true : On.
 *            - false: Off.
 */
void fsusb_set_ep3_toggle(FSUSB_t *p_base, bool b_flag);

/**
 * @brief Set end-point 3 stall.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] b_flag
 *            - true : Stall.
 *            - false: Normal.
 */
void fsusb_set_ep3_stall(FSUSB_t *p_base, bool b_flag);

/**
 * @brief Get end-point 3 pipe state.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return bool Return true if end-point 1 is stall.
 */
bool fsusb_get_ep3_stall_st(FSUSB_t *p_base);

/**
 * @brief Set end-point 3 sends data payload.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] byte_count Size of payload.
 */
void fsusb_set_ep3_pkt_send(FSUSB_t *p_base, uint16_t byte_count);

/**
 * @brief Get end-point 3 state is ready after sent data payload.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return bool Return true if end-point 3 sent payload is done.
 */
bool fsusb_get_ep3_pkt_sent_rdy(FSUSB_t *p_base);

/**
 * @brief Set end-point 4 handshake is working with toggle mode.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] b_flag
 *            - true : On.
 *            - false: Off.
 */
void fsusb_set_ep4_toggle(FSUSB_t *p_base, bool b_flag);

/**
 * @brief Set end-point 4 stall.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] b_flag
 *            - true : Stall.
 *            - false: Normal.
 */
void fsusb_set_ep4_stall(FSUSB_t *p_base, bool b_flag);

/**
 * @brief Get end-point 4 pipe state.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return bool Return true if end-point 4 is stall.
 */
bool fsusb_get_ep4_stall_st(FSUSB_t *p_base);

/**
 * @brief Clear end-point 4 payload latched state after received completed packet then
 * automatically start to receive next completed payload.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 */
void fsusb_clear_ep4_latched_st(FSUSB_t *p_base);

/**
 * @brief Get the state of end-point 4 received buffer is fetched completedly.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @bool bool Return true if completion.
 */
bool fsusb_get_ep4_pkt_fetched_rdy(FSUSB_t *p_base);

/**
 * @brief Get end-point 4 received packet length.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return uint8_t Return received packet length.
 */
uint8_t fsusb_get_ep4_rx_pkt_len(FSUSB_t *p_base);

/**
 * @brief Set end-point 5 handshake is working with toggle mode.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] b_flag
 *            - true : On.
 *            - false: Off.
 */
void fsusb_set_ep5_toggle(FSUSB_t *p_base, bool b_flag);

/**
 * @brief Set end-point 5 stall.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] b_flag
 *            - true : Stall.
 *            - false: Normal.
 */
void fsusb_set_ep5_stall(FSUSB_t *p_base, bool b_flag);

/**
 * @brief Get end-point 5 pipe state.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return bool Return true if end-point 5 is stall.
 */
bool fsusb_get_ep5_stall_st(FSUSB_t *p_base);

/**
 * @brief Set end-point 5 sends data payload.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] byte_count Size of payload.
 */
void fsusb_set_ep5_pkt_send(FSUSB_t *p_base, uint16_t byte_count);

/**
 * @brief Get end-point 5 state is ready after sent data payload.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return bool Return true if end-point 3 sent payload is done.
 */
bool fsusb_get_ep5_pkt_sent_rdy(FSUSB_t *p_base);

/**
 * @brief Set end-point 6 handshake is working with toggle mode.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] b_flag
 *            - true : On.
 *            - false: Off.
 */
void fsusb_set_ep6_toggle(FSUSB_t *p_base, bool b_flag);

/**
 * @brief Set end-point 6 stall.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @param[in] b_flag
 *            - true : Stall.
 *            - false: Normal.
 */
void fsusb_set_ep6_stall(FSUSB_t *p_base, bool b_flag);

/**
 * @brief Get end-point 6 pipe state.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return bool Return true if end-point 6 is stall.
 */
bool fsusb_get_ep6_stall_st(FSUSB_t *p_base);

/**
 * @brief Clear end-point 6 payload latched state after received completed packet then
 * automatically start to receive next completed payload.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 */
void fsusb_clear_ep6_latched_st(FSUSB_t *p_base);

/**
 * @brief Get the state of end-point 6 received buffer is fetched completedly.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @bool bool Return true if completion.
 */
bool fsusb_get_ep6_pkt_fetched_rdy(FSUSB_t *p_base);

/**
 * @brief Get end-point 6 received packet length.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return uint8_t Return received packet length.
 */
uint8_t fsusb_get_ep6_rx_pkt_len(FSUSB_t *p_base);

/**
 * @brief Get RC trim deviation value.
 *
 * @param[in] p_base Module base pointer of type FSUSB_t
 * @return uint16_t Return the deviation value.
 */
uint16_t fsusb_get_rc_trim_deviation_val(FSUSB_t *p_base);

#ifdef __cplusplus
}
#endif

#endif /* FSUSB_DRIVER_H */

/*** end of file ***/
