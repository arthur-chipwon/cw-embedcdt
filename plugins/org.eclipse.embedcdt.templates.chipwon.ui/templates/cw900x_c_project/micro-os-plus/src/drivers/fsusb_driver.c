/**
 * @file fsusb_driver.c
 * @brief The module provides for requiring FSUSB hardwre registers access.
 * Export functions for wide scope use.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsusb_driver.h"

/*******************************************************************************
 * Private macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/

/*******************************************************************************
 * Private typedefs
 ******************************************************************************/

/*******************************************************************************
 * Global variables
 ******************************************************************************/
/** @brief Table of base pointers for FSUSB instances. */
FSUSB_t *const gp_fsusb_base[FSUSB_INSTANCE_COUNT] = FSUSB_BASE_PTRS;

/** @brief Table to save FSUSB IRQ enumeration numbers defined in the CMSIS
 * header file. */
const IRQn_t g_fsusb_irq_id[FSUSB_INSTANCE_COUNT] = FSUSB_IRQS;

/** @brief Table to save FSUSB clock names as defined in clock manager. */
const clock_names_t g_fsusb_clk_names[FSUSB_INSTANCE_COUNT] = FSUSB_CLOCK_NAMES;

/*******************************************************************************
 * Static variables
 ******************************************************************************/

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/

/*******************************************************************************
 * Global functions
 ******************************************************************************/
void
fsusb_init(FSUSB_t *p_base)
{
    fsusb_set_ep0_stall(p_base, false);
    fsusb_set_ep1_stall(p_base, false);
    fsusb_set_ep2_stall(p_base, false);
    fsusb_set_ep3_stall(p_base, false);
    fsusb_set_ep4_stall(p_base, false);
    fsusb_set_ep5_stall(p_base, false);
    fsusb_set_ep6_stall(p_base, false);
    fsusb_set_ep0_toggle(p_base, false);
    fsusb_set_ep1_toggle(p_base, false);
    fsusb_set_ep2_toggle(p_base, false);
    fsusb_set_ep3_toggle(p_base, false);
    fsusb_set_ep4_toggle(p_base, false);
    fsusb_set_ep5_toggle(p_base, false);
    fsusb_set_ep6_toggle(p_base, false);
    fsusb_set_sram0_cs_enable_bit(p_base);
    fsusb_set_sram1_cs_enable_bit(p_base);
    fsusb_set_sram3_cs_enable_bit(p_base);
    fsusb_set_rdy_for_bus_connect(p_base);
    fsusb_set_pwr_on_suspend_on_off(p_base, false); /* clear the feature of power on suspend and
                                               perhaps remain it off tile bus power on signal */
    fsusb_set_pwr_on_suspend_on_off(p_base, true);  /* set the feature of power on suspend */
    fsusb_set_dp_dm_line_in(p_base);
    fsusb_clear_ep0_latched_st(p_base);
    fsusb_clear_status_bm(p_base, (uint32_t)FSUSB_ALL_STATUS);
    fsusb_set_usb_dev_addr(p_base, 0);
    fsusb_clear_intrpt_enable_bit(p_base, FSUSB_INT_ALL_BM);
}

uint32_t
fsusb_get_status_bm(FSUSB_t *p_base)
{
    return p_base->UISR;
}

void
fsusb_clear_status_bm(FSUSB_t *p_base, uint32_t status_bm)
{
    p_base->UISR = status_bm;
}

void
fsusb_set_intrpt_enable_bit(FSUSB_t *p_base, fsusb_int_bm_t bitmap)
{
    p_base->INTMSK |= bitmap;
}

void
fsusb_clear_intrpt_enable_bit(FSUSB_t *p_base, fsusb_int_bm_t bitmap)
{
    p_base->INTMSK &= ~bitmap;
}

void
fsusb_set_pwr_on_suspend_on_off(FSUSB_t *p_base, bool b_flag)
{
    p_base->POSR = b_flag ? FSUSB_POSR_ON_MASK : (FSUSB_POSR_ON_MASK & (~FSUSB_POSR_ON_MASK));
}

bool
fsusb_get_ep_sts_nak(FSUSB_t *p_base, fsusb_ep_sts_bm_t bitmap)
{
    return (p_base->EPX & bitmap) ? true : false;
}

void
fsusb_set_sram0_cs_enable_bit(FSUSB_t *p_base)
{
    p_base->SRCSR |= FSUSB_SRCSR_CS0EN_MASK;
}

void
fsusb_clear_sram0_cs_enable_bit(FSUSB_t *p_base)
{
    p_base->SRCSR &= ~FSUSB_SRCSR_CS0EN_MASK;
}

void
fsusb_set_sram1_cs_enable_bit(FSUSB_t *p_base)
{
    p_base->SRCSR |= FSUSB_SRCSR_CS1EN_MASK;
}

void
fsusb_clear_sram1_cs_enable_bit(FSUSB_t *p_base)
{
    p_base->SRCSR &= ~FSUSB_SRCSR_CS1EN_MASK;
}

void
fsusb_set_sram3_cs_enable_bit(FSUSB_t *p_base)
{
    p_base->SRCSR |= FSUSB_SRCSR_CS3EN_MASK;
}

void
fsusb_clear_sram3_cs_enable_bit(FSUSB_t *p_base)
{
    p_base->SRCSR &= ~FSUSB_SRCSR_CS3EN_MASK;
}

bool
fsusb_get_bus_line_st(FSUSB_t *p_base)
{
    return (FSUSB_STS_BL_MASK == (p_base->STS & FSUSB_STS_BL_MASK)) ? true : false;
}

bool
fsusb_get_bus_connect_rdy(FSUSB_t *p_base)
{
    return ((p_base->PHY & FSUSB_PHY_CONN_MASK)) ? true : false;
}

void
fsusb_set_rdy_for_bus_connect(FSUSB_t *p_base)
{
    p_base->PHY |= FSUSB_PHY_CONN_MASK;
}

void
fsusb_set_dp_dm_line_in(FSUSB_t *p_base)
{
    p_base->PHY |= FSUSB_PHY_DPDMLI_MASK;
}

void
fsusb_set_ep0_toggle(FSUSB_t *p_base, bool b_flag)
{
    p_base->EP0TCR =
        b_flag ? (FSUSB_EP0TCR_CLR_MASK & (~FSUSB_EP0TCR_CLR_MASK)) : FSUSB_EP0TCR_CLR_MASK;
}

void
fsusb_set_ep0_stall(FSUSB_t *p_base, bool b_flag)
{
    p_base->EP0STR =
        b_flag ? FSUSB_EP0STR_STL_MASK : (FSUSB_EP0STR_STL_MASK & (~FSUSB_EP0STR_STL_MASK));
}

bool
fsusb_get_ep0_stall_st(FSUSB_t *p_base)
{
    return (p_base->EP0STR & FSUSB_EP0STR_STL_MASK) ? true : false;
}

void
fsusb_set_ep0_zero_send(FSUSB_t *p_base)
{
    DEV_ASSERT(0u == (p_base->EP0TZR & FSUSB_EP0TZR_ZERO_MASK));
    p_base->EP0TZR = FSUSB_EP0TZR_ZERO_MASK;
}

bool
fsusb_get_ep0_zero_sent_rdy(FSUSB_t *p_base)
{
    return (p_base->EP0TZR & FSUSB_EP0TZR_ZERO_MASK) ? false : true;
}

void
fsusb_set_ep0_pkt_send(FSUSB_t *p_base, uint16_t byte_count)
{
    DEV_ASSERT(0u != byte_count);
    DEV_ASSERT(64u >= byte_count);
    DEV_ASSERT(0u == (p_base->EP0SR & FSUSB_EP0SR_TXBSY_MASK));
    p_base->EP0TPR = FSUSB_EP0TPR_LEN(byte_count);
}

bool
fsusb_get_ep0_pkt_sent_rdy(FSUSB_t *p_base)
{
    return (p_base->EP0SR & FSUSB_EP0SR_TXBSY_MASK) ? false : true;
}

void
fsusb_clear_ep0_latched_st(FSUSB_t *p_base)
{
    p_base->EP0CR &= ~FSUSB_EP0CR_LAT_MASK;
}

uint8_t
fsusb_get_ep0_rx_pkt_len(FSUSB_t *p_base)
{
    return (uint8_t)p_base->EP0RPR;
}

void
fsusb_set_usb_dev_addr(FSUSB_t *p_base, uint8_t addr_num)
{
    DEV_ASSERT(128u > addr_num);
    p_base->UADR = (addr_num & FSUSB_UADR_MASK);
}

uint8_t
fsusb_get_usb_dev_addr(FSUSB_t *p_base)
{
    return (uint8_t)(p_base->UADR & FSUSB_UADR_MASK);
}

uint8_t
fsusb_get_usb_dev_req_data_byte(FSUSB_t *p_base, uint8_t byte_order)
{
    volatile uint32_t *p_reg_addr = &(p_base->SD0) + byte_order;
    return (uint8_t)(*p_reg_addr);
}

void
fsusb_set_ep1_toggle(FSUSB_t *p_base, bool b_flag)
{
    p_base->EP1TCR =
        b_flag ? (FSUSB_EP1TCR_CLR_MASK & (~FSUSB_EP1TCR_CLR_MASK)) : FSUSB_EP1TCR_CLR_MASK;
}

void
fsusb_set_ep1_stall(FSUSB_t *p_base, bool b_flag)
{
    p_base->EP1STR =
        b_flag ? FSUSB_EP1STR_STL_MASK : (FSUSB_EP1STR_STL_MASK & (~FSUSB_EP1STR_STL_MASK));
}

bool
fsusb_get_ep1_stall_st(FSUSB_t *p_base)
{
    return (p_base->EP1STR & FSUSB_EP1STR_STL_MASK) ? true : false;
}

void
fsusb_set_ep1_pkt_send(FSUSB_t *p_base, uint16_t byte_count)
{
    DEV_ASSERT(0u != byte_count);
    DEV_ASSERT(64u >= byte_count);
    DEV_ASSERT(0u == (p_base->EP1SR & FSUSB_EP1SR_TXBSY_MASK));
    p_base->EP1TPR = FSUSB_EP1TPR_LEN(byte_count);
}

bool
fsusb_get_ep1_pkt_sent_rdy(FSUSB_t *p_base)
{
    return (p_base->EP1SR & FSUSB_EP1SR_TXBSY_MASK) ? false : true;
}

void
fsusb_set_ep2_toggle(FSUSB_t *p_base, bool b_flag)
{
    p_base->EP2TCR =
        b_flag ? (FSUSB_EP2TCR_CLR_MASK & (~FSUSB_EP2TCR_CLR_MASK)) : FSUSB_EP2TCR_CLR_MASK;
}

void
fsusb_set_ep2_stall(FSUSB_t *p_base, bool b_flag)
{
    p_base->EP2STR =
        b_flag ? FSUSB_EP2STR_STL_MASK : (FSUSB_EP2STR_STL_MASK & (~FSUSB_EP2STR_STL_MASK));
}

bool
fsusb_get_ep2_stall_st(FSUSB_t *p_base)
{
    return (p_base->EP2STR & FSUSB_EP2STR_STL_MASK) ? true : false;
}

void
fsusb_set_ep2_pkt_send(FSUSB_t *p_base, uint16_t byte_count)
{
    DEV_ASSERT(0u != byte_count);
    DEV_ASSERT(64u >= byte_count);
    DEV_ASSERT(0u == (p_base->EP2SR & FSUSB_EP2SR_TXBSY_MASK));
    p_base->EP2TPR = FSUSB_EP2TPR_LEN(byte_count);
}

bool
fsusb_get_ep2_pkt_sent_rdy(FSUSB_t *p_base)
{
    return (p_base->EP2SR & FSUSB_EP2SR_TXBSY_MASK) ? false : true;
}

void
fsusb_set_ep3_toggle(FSUSB_t *p_base, bool b_flag)
{
    p_base->EP3TCR =
        b_flag ? (FSUSB_EP3TCR_CLR_MASK & (~FSUSB_EP3TCR_CLR_MASK)) : FSUSB_EP3TCR_CLR_MASK;
}

void
fsusb_set_ep3_stall(FSUSB_t *p_base, bool b_flag)
{
    p_base->EP3STR =
        b_flag ? FSUSB_EP3STR_STL_MASK : (FSUSB_EP3STR_STL_MASK & (~FSUSB_EP3STR_STL_MASK));
}

bool
fsusb_get_ep3_stall_st(FSUSB_t *p_base)
{
    return (p_base->EP3STR & FSUSB_EP3STR_STL_MASK) ? true : false;
}

void
fsusb_set_ep3_pkt_send(FSUSB_t *p_base, uint16_t byte_count)
{
    DEV_ASSERT(0u != byte_count);
    DEV_ASSERT(64u >= byte_count);
    DEV_ASSERT(0u == (p_base->EP3SR & FSUSB_EP3SR_TXBSY_MASK));
    p_base->EP3TPR = FSUSB_EP3TPR_LEN(byte_count);
}

bool
fsusb_get_ep3_pkt_sent_rdy(FSUSB_t *p_base)
{
    return (p_base->EP3SR & FSUSB_EP3SR_TXBSY_MASK) ? false : true;
}

void
fsusb_set_ep4_toggle(FSUSB_t *p_base, bool b_flag)
{
    p_base->EP4TCR =
        b_flag ? (FSUSB_EP4TCR_CLR_MASK & (~FSUSB_EP4TCR_CLR_MASK)) : FSUSB_EP4TCR_CLR_MASK;
}

void
fsusb_set_ep4_stall(FSUSB_t *p_base, bool b_flag)
{
    p_base->EP4STR =
        b_flag ? FSUSB_EP4STR_STL_MASK : (FSUSB_EP4STR_STL_MASK & (~FSUSB_EP4STR_STL_MASK));
}

bool
fsusb_get_ep4_stall_st(FSUSB_t *p_base)
{
    return (p_base->EP4STR & FSUSB_EP4STR_STL_MASK) ? true : false;
}

void
fsusb_clear_ep4_latched_st(FSUSB_t *p_base)
{
    p_base->EP4CR &= ~FSUSB_EP4CR_LAT_MASK;
}

bool
fsusb_get_ep4_pkt_fetched_rdy(FSUSB_t *p_base)
{
    return (p_base->EP4CR & FSUSB_EP4CR_LAT_MASK) ? true : false;
}

uint8_t
fsusb_get_ep4_rx_pkt_len(FSUSB_t *p_base)
{
    return (uint8_t)p_base->EP4RPR;
}

void
fsusb_set_ep5_toggle(FSUSB_t *p_base, bool b_flag)
{
    uint32_t reg_val = p_base->EP56TCR;
    uint32_t info    = (uint32_t)(!b_flag) << FSUSB_EP5TCR_CLR_SHIFT;

    if ((reg_val & FSUSB_EP5TCR_CLR_MASK) ^ info)
    {
        p_base->EP56TCR = reg_val ^ FSUSB_EP5TCR_CLR_MASK;
    }
}

void
fsusb_set_ep5_stall(FSUSB_t *p_base, bool b_flag)
{
    uint32_t reg_val = p_base->EP56STR;
    uint32_t info    = (uint32_t)b_flag << FSUSB_EP5STR_STL_SHIFT;

    if ((reg_val & FSUSB_EP5STR_STL_MASK) ^ info)
    {
        p_base->EP56STR = reg_val ^ FSUSB_EP5STR_STL_MASK;
    }
}

bool
fsusb_get_ep5_stall_st(FSUSB_t *p_base)
{
    return (p_base->EP56STR & FSUSB_EP5STR_STL_MASK) ? true : false;
}

void
fsusb_set_ep5_pkt_send(FSUSB_t *p_base, uint16_t byte_count)
{
    uint32_t reg_val = p_base->EP56PR;

    DEV_ASSERT(0u != byte_count);
    DEV_ASSERT(64u >= byte_count);
    DEV_ASSERT(0u == (p_base->EP56CSR & FSUSB_EP5SR_TXBSY_MASK));
    reg_val &= (~FSUSB_EP5PR_LEN_MASK);
    p_base->EP56PR = reg_val | FSUSB_EP5PR_LEN(byte_count);
}

bool
fsusb_get_ep5_pkt_sent_rdy(FSUSB_t *p_base)
{
    return (p_base->EP56CSR & FSUSB_EP5SR_TXBSY_MASK) ? false : true;
}

void
fsusb_set_ep6_toggle(FSUSB_t *p_base, bool b_flag)
{
    uint32_t reg_val = p_base->EP56TCR;
    uint32_t info    = (uint32_t)(!b_flag) << FSUSB_EP6TCR_CLR_SHIFT;

    if ((reg_val & FSUSB_EP6TCR_CLR_MASK) ^ info)
    {
        p_base->EP56TCR = reg_val ^ FSUSB_EP6TCR_CLR_MASK;
    }
}

void
fsusb_set_ep6_stall(FSUSB_t *p_base, bool b_flag)
{
    uint32_t reg_val = p_base->EP56STR;
    uint32_t info    = (uint32_t)b_flag << FSUSB_EP6STR_STL_SHIFT;

    if ((reg_val & FSUSB_EP6STR_STL_MASK) ^ info)
    {
        p_base->EP56STR = reg_val ^ FSUSB_EP6STR_STL_MASK;
    }
}

bool
fsusb_get_ep6_stall_st(FSUSB_t *p_base)
{
    return (p_base->EP56STR & FSUSB_EP6STR_STL_MASK) ? true : false;
}

void
fsusb_clear_ep6_latched_st(FSUSB_t *p_base)
{
    p_base->EP56CSR = FSUSB_EP6CR_LAT_MASK;
}

bool
fsusb_get_ep6_pkt_fetched_rdy(FSUSB_t *p_base)
{
    return (p_base->EP56CSR & FSUSB_EP6CR_LAT_MASK) ? true : false;
}

uint8_t
fsusb_get_ep6_rx_pkt_len(FSUSB_t *p_base)
{
    return (uint8_t)((p_base->EP56PR & FSUSB_EP6PR_LEN_MASK) >> FSUSB_EP6PR_LEN_SHIFT);
}

uint16_t
fsusb_get_rc_trim_deviation_val(FSUSB_t *p_base)
{
    return (uint8_t)p_base->TRDIF;
}

/*******************************************************************************
 * Static functions
 ******************************************************************************/

/*** end of file ***/
