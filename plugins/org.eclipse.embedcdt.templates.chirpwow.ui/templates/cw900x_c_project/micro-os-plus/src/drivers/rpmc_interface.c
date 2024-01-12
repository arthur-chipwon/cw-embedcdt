/**
 * @file rpmc_interface.c
 * @brief An implementation of runtime functions for RPMC client service.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <clock_gating.h>
#include <stdio.h>
#include <string.h>
#include "rpmc_interface.h"
#include "interrupt_manager.h"

/*******************************************************************************
 * Private macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/

/*******************************************************************************
 * Private typedefs
 ******************************************************************************/
typedef struct _rpmc_state_t
{
    HSSPI_t                        *p_base;
    const IRQn_t                   *p_irq_id;
    const clock_names_t            *p_clk_name;
    uint8_t                        *p_tx_sram_ptr;
    uint8_t                        *p_rx_sram_ptr;
    uint32_t                        tx_buf_size;
    uint8_t                        *p_rx_buffer;
    uint32_t                        rx_buf_size;
    volatile bool                   b_new_txn_arrival;
    volatile uint8_t                txn_cmd_type;
    volatile rpmc_support_cmd_idx_t txn_cmd_idx;
    volatile uint16_t               txn_pkt_len;
} rpmc_state_t;

/*******************************************************************************
 * Global variables
 ******************************************************************************/
rpmc_internal_register_t g_rpmc_intl_reg = {
    .state_at_boot =
        {
#ifndef _RPMC_ROOT_KEY_EXIST_
            RPMC_UNINITINALIZED_STATE,
            RPMC_UNINITINALIZED_STATE,
#else  /* _RPMC_ROOT_KEY_EXIST_ */
            RPMC_INITINALIZED_STATE,
            RPMC_INITINALIZED_STATE,
#endif /* _RPMC_ROOT_KEY_EXIST_ */
        },
    .root_key =
        {
#ifndef _RPMC_ROOT_KEY_EXIST_
            {
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
            },
            {
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
            },
#else  /* _RPMC_ROOT_KEY_EXIST_ */
            {
                0x00u, 0x01u, 0x02u, 0x03u, 0x04u, 0x05u, 0x06u, 0x07u, 0x08u, 0x09u, 0x0Au,
                0x0Bu, 0x0Cu, 0x0Du, 0x0Eu, 0x0Fu, 0x10u, 0x11u, 0x12u, 0x13u, 0x14u, 0x15u,
                0x16u, 0x17u, 0x18u, 0x19u, 0x1Au, 0x1Bu, 0x1Cu, 0x1Du, 0x1Eu, 0x1Fu,
            },
            {
                0x00u, 0x01u, 0x02u, 0x03u, 0x04u, 0x05u, 0x06u, 0x07u, 0x08u, 0x09u, 0x0Au,
                0x0Bu, 0x0Cu, 0x0Du, 0x0Eu, 0x0Fu, 0x10u, 0x11u, 0x12u, 0x13u, 0x14u, 0x15u,
                0x16u, 0x17u, 0x18u, 0x19u, 0x1Au, 0x1Bu, 0x1Cu, 0x1Du, 0x1Eu, 0x1Fu,
            },
#endif /* _RPMC_ROOT_KEY_EXIST_ */
        },
    .hmac_key =
        {
            {
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
            },
            {
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
            },
        },
    .monotonic_counter = {
#ifndef _RPMC_ROOT_KEY_EXIST_
        0x00000000u,
        0x00000000u,
#else  /* _RPMC_ROOT_KEY_EXIST_ */
        0x00000008u,
        0x00000007u,
#endif /* _RPMC_ROOT_KEY_EXIST_ */
    }};

/*******************************************************************************
 * Static variables
 ******************************************************************************/
/** Pointer to rpmc runtime state structure. */
static rpmc_state_t *sp_rpmc_state_ptr = NULL;

rpmc_state_t g_rpmc_state = {0u};

static rpmc_txn_buf_t s_rpmc_txn_buffer;
// static rpmc_resp_buf_t s_rpmc_resp_buffer;

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/

/*******************************************************************************
 * Global functions
 ******************************************************************************/
/**
 * @brief This function is the implementation of HSSPI handler named in startup
 * code.
 */
void
HSSPI_IRQHandler(void)
{
    DEV_ASSERT(NULL != sp_rpmc_state_ptr);
    rpmc_state_t *p_state_ptr = sp_rpmc_state_ptr;
    HSSPI_t      *p_base      = p_state_ptr->p_base;
    bool          b_exit      = false;
    uint8_t       cmd_type;
    uint32_t      status_bm;

    status_bm = hsspi_get_status_bm(p_base);
    hsspi_clear_status_bm(p_base, status_bm & (uint32_t)HSSPI_ALL_STATUS);
    if (status_bm & (1u << HSSPI_RX_CMD_FLAG))
    {
        status_bm &= ~(1u << HSSPI_RX_CMD_FLAG);
        p_state_ptr->b_new_txn_arrival = true;
        cmd_type                       = *(p_state_ptr->p_rx_sram_ptr);
        p_state_ptr->txn_pkt_len       = hsspi_get_rx_len(p_base);
        switch (cmd_type)
        {
            case RPMC_CT_WRITE_ROOT_KEY_REG:
            {
                if (p_state_ptr->txn_pkt_len != sizeof(rpmc_write_root_key_t) + 1u)
                {
                    b_exit = true;
                    break;
                }
                memcpy(p_state_ptr->p_rx_buffer,
                       p_state_ptr->p_rx_sram_ptr + 1u,
                       sizeof(rpmc_write_root_key_t));
                p_state_ptr->txn_cmd_type = cmd_type;
                p_state_ptr->txn_cmd_idx  = RPMC_WRITE_ROOT_KEY_REG;
            }
            break;
            case RPMC_CT_UPDATE_HMAC_KEY_REG:
            {
                if (p_state_ptr->txn_pkt_len != sizeof(rpmc_update_hmac_key_t) + 1u)
                {
                    b_exit = true;
                    break;
                }
                memcpy(p_state_ptr->p_rx_buffer,
                       p_state_ptr->p_rx_sram_ptr + 1u,
                       sizeof(rpmc_update_hmac_key_t));
                p_state_ptr->txn_cmd_type = cmd_type;
                p_state_ptr->txn_cmd_idx  = RPMC_UPDATE_HMAC_KEY_REG;
            }
            break;
            case RPMC_CT_INCR_MONOTONIC_CNTR:
            {
                if (p_state_ptr->txn_pkt_len != sizeof(rpmc_incr_monotonic_cntr_t) + 1u)
                {
                    b_exit = true;
                    break;
                }
                memcpy(p_state_ptr->p_rx_buffer,
                       p_state_ptr->p_rx_sram_ptr + 1u,
                       sizeof(rpmc_incr_monotonic_cntr_t));
                p_state_ptr->txn_cmd_type = cmd_type;
                p_state_ptr->txn_cmd_idx  = RPMC_INCR_MONOTONIC_CNTR;
            }
            break;
            case RPMC_CT_REQ_MONOTONIC_CNTR:
            {
                if (p_state_ptr->txn_pkt_len != sizeof(rpmc_req_monotonic_cntr_t) + 1u)
                {
                    b_exit = true;
                    break;
                }
                memcpy(p_state_ptr->p_rx_buffer,
                       p_state_ptr->p_rx_sram_ptr + 1u,
                       sizeof(rpmc_req_monotonic_cntr_t));
                p_state_ptr->txn_cmd_type = cmd_type;
                p_state_ptr->txn_cmd_idx  = RPMC_REQ_MONOTONIC_CNTR;
            }
            break;
            case RPMC_CT_RESERVED:
            default:
            {
                b_exit = true;
            }
            break;
        }
        if (!b_exit)
        {
            hsspi_fillup_response_status(p_base, RPMC_STS_BUSY);
        }
        else
        {
            hsspi_fillup_response_status(p_base, RPMC_STS_CNTR_OUT_OF_RANGE);
            p_state_ptr->b_new_txn_arrival = false;
            p_state_ptr->txn_cmd_type      = RPMC_CT_UNSPECIFIED;
        }
    }
    if (status_bm & (1u << HSSPI_TX_COMPLETION_FLAG))
    {
        status_bm &= ~(1u << HSSPI_TX_COMPLETION_FLAG);
        hsspi_fillup_response_status(p_base, RPMC_STS_CLEAR);
        p_state_ptr->b_new_txn_arrival = true;
        p_state_ptr->txn_cmd_type      = RPMC_CT_UNSPECIFIED;
        p_state_ptr->txn_cmd_idx       = RPMC_READ_STATUS_DATA;
    }
}

void
rpmc_init(void)
{
    DEV_ASSERT(NULL == sp_rpmc_state_ptr);
    rpmc_state_t *p_state_ptr = &g_rpmc_state;
    HSSPI_t      *p_base      = NULL;
    uint32_t      source_clk_freq;

    /** Clear the state struct for this instance. */
    memset((void *)p_state_ptr, 0x00u, sizeof(rpmc_state_t));
    /** Save the runtime state structure pointer. */
    sp_rpmc_state_ptr   = p_state_ptr;
    p_state_ptr->p_base = p_base = gp_hsspi_base[INST_HSSPI0];
    p_state_ptr->p_irq_id        = &g_hsspi_irq_id[INST_HSSPI0];
    p_state_ptr->p_clk_name      = &g_hsspi_clk_names[INST_HSSPI0];

    /* Get the HSSPI clock as configured in the clock manager */
    (void)clock_get_freq(*(p_state_ptr->p_clk_name), &source_clk_freq);
    DEV_ASSERT(0u < source_clk_freq);

    hsspi_init(p_base);
    p_state_ptr->p_rx_sram_ptr     = (uint8_t *)hsspi_get_req_buffer_address();
    p_state_ptr->p_rx_buffer       = (uint8_t *)&s_rpmc_txn_buffer;
    p_state_ptr->rx_buf_size       = hsspi_get_req_buffer_size();
    p_state_ptr->p_tx_sram_ptr     = (uint8_t *)hsspi_get_resp_buffer_address();
    p_state_ptr->tx_buf_size       = hsspi_get_resp_buffer_size();
    p_state_ptr->b_new_txn_arrival = false;
    hsspi_fillup_response_status(p_base, RPMC_STS_CLEAR);
    hsspi_fillup_rw_opcode(p_base, RPMC_READ_OPCODE, RPMC_WRITE_OPCODE);
    hsspi_set_oe_bitcount(p_base, 0x06u); /**< For pairing with master baud rate == 16670000. */
#if 0
    hsspi_set_clk_skew_cancellation_on_off(
        p_base, true); /**< For pairing with master baud rate == 16670000. */
#endif
    p_base->FUN = 0x00000000u; /**< Todo. Temporary statement. Arthur20230427 */
    hsspi_set_rpmc_enable(p_base, true);
    /** Enable the event alert gate of the HSSPI module. */
    hsspi_set_intrpt_enable_bit(p_base, HSSPI_INT_RX_CMD_BM | HSSPI_INT_RX_COMPLETION_BM);
    /** Enable the vector interrupt. */
    int_enable_irq(*(p_state_ptr->p_irq_id));
}

void
rpmc_deinit(void)
{
    DEV_ASSERT(NULL != sp_rpmc_state_ptr);
    rpmc_state_t *p_state_ptr = sp_rpmc_state_ptr;
    HSSPI_t      *p_base      = p_state_ptr->p_base;

    hsspi_set_rpmc_enable(p_base, false);
    /** Disable the event alert gate of the HSSPI module. */
    hsspi_clear_intrpt_enable_bit(p_base, HSSPI_INT_ALL_BM);
    /** Disable the vector interrupt. */
    int_disable_irq(*(p_state_ptr->p_irq_id));
    /** Clear the state pointer. */
    sp_rpmc_state_ptr = NULL;
    hsspi_init(p_base);
}

bool
rpmc_check_transaction_new_arrival(void)
{
    DEV_ASSERT(NULL != sp_rpmc_state_ptr);
    rpmc_state_t *p_state_ptr = sp_rpmc_state_ptr;
    return p_state_ptr->b_new_txn_arrival;
}

rpmc_support_cmd_idx_t
rpmc_get_transaction_info(void)
{
    DEV_ASSERT(NULL != sp_rpmc_state_ptr);
    rpmc_state_t *p_state_ptr = sp_rpmc_state_ptr;
    DEV_ASSERT(true == p_state_ptr->b_new_txn_arrival);
    p_state_ptr->b_new_txn_arrival = false;
    return p_state_ptr->txn_cmd_idx;
}

void
rpmc_update_status_on_demand(uint8_t status)
{
    DEV_ASSERT(NULL != sp_rpmc_state_ptr);
    rpmc_state_t *p_state_ptr = sp_rpmc_state_ptr;
    HSSPI_t      *p_base      = p_state_ptr->p_base;
    hsspi_fillup_response_status(p_base, status);
}

rpmc_txn_buf_t *
rpmc_get_transaction_buf_ptr(void)
{
    DEV_ASSERT(NULL != sp_rpmc_state_ptr);
    rpmc_state_t *p_state_ptr = sp_rpmc_state_ptr;
    return (rpmc_txn_buf_t *)(p_state_ptr->p_rx_buffer);
}

rpmc_resp_buf_t *
rpmc_get_response_buf_ptr(void)
{
    DEV_ASSERT(NULL != sp_rpmc_state_ptr);
    rpmc_state_t *p_state_ptr = sp_rpmc_state_ptr;
    return (rpmc_resp_buf_t *)(p_state_ptr->p_tx_sram_ptr);
}

void
rpmc_complete_response_buf(void)
{
    DEV_ASSERT(NULL != sp_rpmc_state_ptr);
    rpmc_state_t *p_state_ptr = sp_rpmc_state_ptr;
    HSSPI_t      *p_base      = p_state_ptr->p_base;
    hsspi_set_tx_sram_update_completed(p_base);
}

/*******************************************************************************
 * Static functions
 ******************************************************************************/

/*** end of file ***/
