/**
 * @file sfspim_interface.c
 * @brief An implementation of helper functions for serial flash SPI master interface.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string.h>
#include "sfspim_interface.h"
#include "clock_gating.h"

/*******************************************************************************
 * Private macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/

/*******************************************************************************
 * Private typedefs
 ******************************************************************************/

/*******************************************************************************
 * Global variables
 ******************************************************************************/
sfspim_state_t g_sfspim_state = {0};

#warning "Serial flash SPI master baud rate is currently configured as 200000."
const sfspim_config_t g_sfspim_init_config = {.baud_rate         = 200000UL,
                                              .which_pcs         = LPSPI_PCS0,
                                              .pcs_polarity      = LPSPI_ACTIVE_LOW,
                                              .b_pcs_continuous  = true,
                                              .b_alt_pcs         = false,
                                              .b_indep_data_wire = true,
                                              .b_full_duplex     = true,
                                              .xfer_width        = LPSPI_SINGLE_BIT_XFER,
                                              .bits_per_frame    = 8U,
                                              .clk_phase         = LPSPI_CLOCK_PHASE_1ST_EDGE,
                                              .clk_polarity      = LPSPI_SCK_ACTIVE_HIGH,
                                              .b_lsb_first       = false,
                                              .xfer_type         = SFSPIM_USING_INTERRUPTS,
                                              .rx_dma_channel    = 0U,
                                              .tx_dma_channel    = 0U,
                                              .callback          = NULL,
                                              .p_callback_param  = NULL};

/*******************************************************************************
 * Static variables
 ******************************************************************************/
/** Pointer to sfspim runtime state structure. */
static sfspim_state_t *sp_sfspim_state_ptr = NULL;

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
static void     sfspim_full_duplex_isr(void);
static void     sfspim_full_duplex_fillup_tx_buffer(void);
static void     sfspim_full_duplex_read_rx_buffer(void);
static void     sfspim_disable_teie_intrpts(void);
static void     sfspim_layer_convert(sfspim_cmd_wrapper_t *p_cmd_wrap);
static void     sfspim_start_xfer(bool b_one_wire_bidirection);
static void     sfspim_complete_xfer(void);
static status_t sfspim_abort_xfer(void);
static status_t sfspim_configure_bus(const sfspim_config_t *p_init_config,
                                     uint32_t              *p_calc_baud_rate);

/*******************************************************************************
 * Global functions
 ******************************************************************************/
#if (INST_SFSPIM == INST_LPSPI0)
/**
 * @brief This function is the implementation of LPSPI0 handler named in startup
 * code.
 */
void
LPSPI0_IRQHandler(void)
{
    sfspim_full_duplex_isr();
}
#endif /* (INST_SFSPIM == INST_LPSPI0) */

#if (INST_SFSPIM == INST_LPSPI1)
/**
 * @brief This function is the implementation of LPSPI1 handler named in startup
 * code.
 */
void
LPSPI1_IRQHandler(void)
{
    sfspim_full_duplex_isr();
}
#endif /* (INST_SFSPIM == INST_LPSPI0) */

#if (INST_SFSPIM == INST_LPSPI2)
/**
 * @brief This function is the implementation of LPSPI2 handler named in startup
 * code.
 */
void
LPSPI2_IRQHandler(void)
{
    sfspim_full_duplex_isr();
}
#endif /* (INST_SFSPIM == INST_LPSPI2) */

status_t
sfspim_init(uint32_t instance, sfspim_state_t *p_state_ptr, const sfspim_config_t *p_init_config)
{
    DEV_ASSERT(instance < LPSPI_INSTANCE_COUNT);
    DEV_ASSERT(p_state_ptr != NULL);
    DEV_ASSERT(p_init_config != NULL);
    LPSPI_t *p_base       = NULL;
    status_t sts_err_code = STATUS_SUCCESS;
    uint32_t source_clk_freq;

    /* Clear the state struct for this instance. */
    memset((void *)p_state_ptr, 0x00u, sizeof(sfspim_state_t));
    /* Save runtime structure pointers so irq handler can point to the correct state structure */
    sp_sfspim_state_ptr = p_state_ptr;
    p_state_ptr->p_base = p_base = gp_lpspi_base[instance];
    p_state_ptr->p_irq_id        = &g_lpspi_irq_id[instance];
    p_state_ptr->p_clk_name      = &g_lpspi_clk_names[instance];

    /* Get the LPSPI clock as configured in the clock manager */
    (void)clock_get_freq(*(p_state_ptr->p_clk_name), &source_clk_freq);
    DEV_ASSERT(0u < source_clk_freq);

    /* Reset the LPSPI registers to their default state */
    lpspi_init(p_base);
    /* Set for master mode */
    (void)lpspi_set_master_slave_mode(p_base, LPSPI_MASTER);
    if (p_init_config->xfer_width == LPSPI_SINGLE_BIT_XFER)
    {
        /* Set Pin configuration such that SDO=out and SDI=in */
        (void)lpspi_set_pin_config_mode(
            p_base, LPSPI_SDI_IN_SDO_OUT, LPSPI_DATA_OUT_RETAINED, true);
    }
    else
    {
        if (p_init_config->xfer_width == LPSPI_TWO_BIT_XFER)
        {
            (void)lpspi_set_pin_config_mode(
                p_base, LPSPI_SDI_IN_OUT, LPSPI_DATA_OUT_TRISTATE, true);
        }
        else
        {
            (void)lpspi_set_pin_config_mode(
                p_base, LPSPI_SDI_IN_OUT, LPSPI_DATA_OUT_TRISTATE, false);
        }
    }
    /* Calculate the FIFO size for the LPSPI */
    lpspi_get_fifo_sizes(p_base, &(p_state_ptr->fifo_size));

    /* Configure bus for this device. If NULL is passed, we assume the caller has
     * preconfigured the bus and doesn't wish to re-configure it again for this transfer.
     * Do nothing for p_calc_baud_rate. If the user wants to know the p_calc_baud_rate
     * then they can call this function separately.
     */
    sts_err_code = sfspim_configure_bus(p_init_config, NULL);
    if (sts_err_code != STATUS_SUCCESS)
    {
        return sts_err_code;
    }
    /* When TX is null the value sent on the bus will be 0 */
    p_state_ptr->dummy = 0;
    /* Initialize the semaphore */
    sts_err_code = osif_sema_create(&(p_state_ptr->sema_blocking_xfer), 0);
    DEV_ASSERT(sts_err_code == STATUS_SUCCESS);
#ifdef _RAM_INTERRUPT_TABLE_
    /* Install LPSPI irq handler */
    INT_SYS_InstallHandler(*(p_state_ptr->p_irq_id),
                           g_SPSPIIntSrvcRoutine[instance],
                           &(p_state_ptr->pfunc_primary_isr));
#endif
    /* Enable the interrupt */
    int_enable_irq(*(p_state_ptr->p_irq_id));
    /* Finally, enable LPSPI */
    lpspi_enable(p_base);
    return sts_err_code;
}

status_t
sfspim_deinit(void)
{
    DEV_ASSERT(sp_sfspim_state_ptr != NULL);
    const sfspim_state_t *p_state_ptr  = sp_sfspim_state_ptr;
    LPSPI_t              *p_base       = p_state_ptr->p_base;
    status_t              sts_err_code = STATUS_SUCCESS;

    /* Check if a transfer is still in progress */
    DEV_ASSERT(p_state_ptr->b_xfer_in_progress == false);

    /* Reset the LPSPI registers to their default state, inlcuding disabling the LPSPI */
    lpspi_init(p_base);
    /* Disable the interrupt */
    int_disable_irq(*(p_state_ptr->p_irq_id));
#ifdef _RAM_INTERRUPT_TABLE_
    /* Restore LPSPI irq handler */
    INT_SYS_InstallHandler(*(p_state_ptr->p_irq_id), p_state_ptr->pfunc_primary_isr, (isr_t *)0);
#endif
    /* Clear the state pointer. */
    sp_sfspim_state_ptr = NULL;

    /* Destroy the semaphore */
    sts_err_code = osif_sema_destroy(&(p_state_ptr->sema_blocking_xfer));
    DEV_ASSERT(sts_err_code == STATUS_SUCCESS);
    return sts_err_code;
}

status_t
sfspim_cmd_wrap_passthrough(sfspim_cmd_wrapper_t *p_cmd_wrap, uint32_t timeout)
{
    DEV_ASSERT(sp_sfspim_state_ptr != NULL);
    /* Instantiate local variable of type lpspi_state_t and point to global state */
    sfspim_state_t *p_state_ptr = sp_sfspim_state_ptr;
    LPSPI_t        *p_base      = p_state_ptr->p_base;
    status_t        error       = STATUS_SUCCESS;
    status_t        sts_err_code;

    /* Check if another transfer is in progress */
    if (lpspi_get_status_flag(p_base, LPSPI_MODULE_BUSY))
    {
        return STATUS_BUSY;
    }

    sfspim_layer_convert(p_cmd_wrap);
    /* Dummy wait to ensure the semaphore is 0, no need to check result */
    (void)osif_sema_wait(&(p_state_ptr->sema_blocking_xfer), 0);
    p_state_ptr->b_blocking = true;
    sfspim_start_xfer(true);
    /* As this is a synchronous transfer, wait until the transfer is complete.*/
    sts_err_code = osif_sema_wait(&(p_state_ptr->sema_blocking_xfer), timeout);

    /* If a timeout occurs, stop the transfer by setting the b_xfer_in_progress to false and
     * disabling interrupts, then return the timeout error status.
     */
    if (sts_err_code == STATUS_TIMEOUT)
    {
        /* Set isBlocking variable to false to avoid dummy semaphore post. */
        p_state_ptr->b_blocking = false;
        /* Complete transfer. */
        sfspim_complete_xfer();
        return (STATUS_TIMEOUT);
    }

    sfspim_disable_teie_intrpts();
    lpspi_set_int_mode(p_base, LPSPI_TRANSFER_COMPLETE, false);
    (void)lpspi_clear_status_flag(p_base, LPSPI_TRANSFER_COMPLETE);

    return error;
}

/*******************************************************************************
 * Static functions
 ******************************************************************************/
static void
sfspim_full_duplex_isr(void)
{
    DEV_ASSERT(sp_sfspim_state_ptr != NULL);
    sfspim_state_t *p_state_ptr = sp_sfspim_state_ptr;
    LPSPI_t        *p_base      = p_state_ptr->p_base;
    uint32_t        status_bm;
    DEV_ASSERT(lpspi_is_master(p_base) == true);

    status_bm = lpspi_get_status_bm(p_base);
    lpspi_clear_status_bm(p_base, status_bm & (uint32_t)LPSPI_ALL_STATUS);
    /* If an error is detected the transfer will be aborted */
    if ((status_bm & (1U << LPSPI_TRANSMIT_ERROR)) && (p_state_ptr->p_tx_buffer != NULL))
    {
        status_bm &= ~(1U << LPSPI_TRANSMIT_ERROR);
        (void)sfspim_abort_xfer();
        p_state_ptr->xfer_status = SFSPIM_TRANSMIT_FAIL;
        return;
    }
    if ((status_bm & (1U << LPSPI_RECEIVE_ERROR)) && (p_state_ptr->p_rx_buffer != NULL))
    {
        status_bm &= ~(1U << LPSPI_RECEIVE_ERROR);
        (void)sfspim_abort_xfer();
        p_state_ptr->xfer_status = SFSPIM_RECEIVE_FAIL;
        return;
    }

    /* RECEIVE IRQ handler: Check read buffer only if there are remaining bytes to read. */
    if (status_bm & (1U << LPSPI_RX_DATA_FLAG))
    {
        status_bm &= ~(1U << LPSPI_RX_DATA_FLAG);
        sfspim_full_duplex_read_rx_buffer();
    }
    /* Transmit data */
    if (status_bm & (1U << LPSPI_TX_DATA_FLAG))
    {
        status_bm &= ~(1U << LPSPI_TX_DATA_FLAG);
        if (p_state_ptr->state_machine == SFSPIM_POSTPONE_EVENT)
        {
            p_state_ptr->state_machine = SFSPIM_TRANSIT_TO_STOP_PHASE;
            /* Disable continuous PCS */
            if (p_state_ptr->b_pcs_continuous == true)
            {
#if defined(_SPI_TCR_NONCOMMON_REG_)
                p_state_ptr->reg_tcr_persist &= ~(LPSPI_TCR_CONT_MASK | LPSPI_TCR_CONTC_MASK);
                p_base->TCR = p_state_ptr->reg_tcr_persist;
#else  /* _SPI_TCR_NONCOMMON_REG_ */
                lpspi_clear_cont_cmd_bit(p_base);
#endif /* _SPI_TCR_NONCOMMON_REG_ */
            }
        }
        if (!p_state_ptr->b_tx_frobidden)
        {
            sfspim_full_duplex_fillup_tx_buffer();
        }
        if (p_state_ptr->state_machine == SFSPIM_TRANSIT_TO_STOP_PHASE)
        {
            lpspi_set_int_mode(p_base, LPSPI_TX_DATA_FLAG, false);
            lpspi_set_int_mode(p_base, LPSPI_TRANSFER_COMPLETE, true);
        }
    }
    /* Check if we're done with this transfer.*/
    if (status_bm & (1U << LPSPI_TRANSFER_COMPLETE))
    {
        status_bm &= ~(1U << LPSPI_TRANSFER_COMPLETE);
        if (p_state_ptr->state_machine == SFSPIM_TRANSIT_TO_STOP_PHASE)
        {
            p_state_ptr->state_machine = SFSPIM_STOP_PHASE;
            sfspim_complete_xfer();
        }
    }
}

static void
sfspim_full_duplex_fillup_tx_buffer(void)
{
    sfspim_state_t *p_state_ptr  = sp_sfspim_state_ptr;
    LPSPI_t        *p_base       = p_state_ptr->p_base;
    uint32_t        word_to_send = 0;
    uint16_t        num_bytes;
    uint8_t         available_space =
        (uint8_t)(p_state_ptr->fifo_size - (uint8_t)lpspi_read_tx_count(p_base));
    uint8_t one_byte_data;
    bool    b_exit = false;

    /* Fill the TX buffer. */
    while (!b_exit)
    {
        /* Get the number of bytes which can be written in a single 32 bits word. */
        if ((p_state_ptr->bytes_per_frame - p_state_ptr->tx_frame_cntr) <= (uint16_t)4)
        {
            num_bytes = (uint16_t)(p_state_ptr->bytes_per_frame - p_state_ptr->tx_frame_cntr);
        }
        else
        {
            num_bytes = 4U;
        }
        word_to_send = 0;

        if (p_state_ptr->state_machine == SFSPIM_CMD_PHASE)
        {
            word_to_send = (uint32_t)p_state_ptr->cmd_buf[p_state_ptr->tx_prog_cntr];
            p_state_ptr->tx_prog_cntr++;
            if (p_state_ptr->tx_prog_cntr >= p_state_ptr->sig_cmd_len)
            {
                p_state_ptr->tx_prog_cntr = 0;
                if (p_state_ptr->sig_addr_len)
                {
                    p_state_ptr->state_machine = SFSPIM_ADDR_PHASE;
                }
                else if (p_state_ptr->dummy_len)
                {
                    p_state_ptr->state_machine = SFSPIM_DUMMY_PHASE;
                }
                else if (p_state_ptr->data_len)
                {
                    p_state_ptr->state_machine = SFSPIM_DATA_PHASE;
                }
                else
                {
                    if (p_state_ptr->b_pcs_continuous == true)
                    {
                        p_state_ptr->state_machine = SFSPIM_POSTPONE_EVENT;
                    }
                    else
                    {
                        p_state_ptr->state_machine = SFSPIM_TRANSIT_TO_STOP_PHASE;
                    }
                    p_state_ptr->b_tx_frobidden = true;
                    b_exit                      = true;
                }
            }
        }
        else if (p_state_ptr->state_machine == SFSPIM_ADDR_PHASE)
        {
            word_to_send = (uint32_t)p_state_ptr->addr_buf[p_state_ptr->tx_prog_cntr];
            p_state_ptr->tx_prog_cntr++;
            if (p_state_ptr->tx_prog_cntr >= p_state_ptr->sig_addr_len)
            {
                p_state_ptr->tx_prog_cntr = 0;
                if (p_state_ptr->dummy_len)
                {
                    p_state_ptr->state_machine = SFSPIM_DUMMY_PHASE;
                }
                else
                {
                    if (p_state_ptr->data_len)
                    {
                        p_state_ptr->state_machine = SFSPIM_DATA_PHASE;
                    }
                    else
                    {
                        if (p_state_ptr->b_pcs_continuous == true)
                        {
                            p_state_ptr->state_machine = SFSPIM_POSTPONE_EVENT;
                        }
                        else
                        {
                            p_state_ptr->state_machine = SFSPIM_TRANSIT_TO_STOP_PHASE;
                        }
                        p_state_ptr->b_tx_frobidden = true;
                        b_exit                      = true;
                    }
                }
            }
        }
        else if (p_state_ptr->state_machine == SFSPIM_DUMMY_PHASE)
        {
            word_to_send = (uint32_t)p_state_ptr->dummy_symbol;
            p_state_ptr->tx_prog_cntr++;
            if (p_state_ptr->tx_prog_cntr >= p_state_ptr->dummy_len)
            {
                p_state_ptr->tx_prog_cntr = 0;
                if (p_state_ptr->data_len)
                {
                    p_state_ptr->state_machine = SFSPIM_DATA_PHASE;
                }
                else
                {
                    if (p_state_ptr->b_pcs_continuous == true)
                    {
                        p_state_ptr->state_machine = SFSPIM_POSTPONE_EVENT;
                    }
                    else
                    {
                        p_state_ptr->state_machine = SFSPIM_TRANSIT_TO_STOP_PHASE;
                    }
                    p_state_ptr->b_tx_frobidden = true;
                    b_exit                      = true;
                }
            }
        }
        else if (p_state_ptr->state_machine == SFSPIM_DATA_PHASE)
        {
            if (!p_state_ptr->b_access_read)
            {
                one_byte_data = p_state_ptr->p_data_buf_ptr[p_state_ptr->tx_prog_cntr];
                word_to_send  = (uint32_t)one_byte_data;
            }
            else
            {
                word_to_send = (uint32_t)p_state_ptr->dummy_symbol;
            }
            p_state_ptr->tx_prog_cntr++;
            if (p_state_ptr->tx_prog_cntr >= p_state_ptr->data_len)
            {
                p_state_ptr->tx_prog_cntr = 0;
                if (p_state_ptr->b_pcs_continuous == true)
                {
                    p_state_ptr->state_machine = SFSPIM_POSTPONE_EVENT;
                }
                else
                {
                    p_state_ptr->state_machine = SFSPIM_TRANSIT_TO_STOP_PHASE;
                }
                p_state_ptr->b_tx_frobidden = true;
                b_exit                      = true;
            }
        }
        else
        {
            b_exit = true;
            continue;
        }
        lpspi_write_data(p_base, word_to_send);
        available_space = (uint8_t)(available_space - 1U);
        if (available_space == 0U)
        {
            b_exit = true;
        }
    }
}

static void
sfspim_full_duplex_read_rx_buffer(void)
{
    sfspim_state_t *p_state_ptr = sp_sfspim_state_ptr;
    LPSPI_t        *p_base      = p_state_ptr->p_base;
    uint32_t        received_word;
    uint16_t        num_bytes;
    // uint16_t              loop;
    uint8_t filled_space = (uint8_t)lpspi_read_rx_count(p_base);
    bool    b_exit       = false;

    while (!b_exit)
    {
        received_word = lpspi_read_data(p_base);
        /* Get the number of bytes which can be read from this 32 bites */
        if ((p_state_ptr->bytes_per_frame - p_state_ptr->rx_frame_cntr) <= (uint16_t)4)
        {
            num_bytes = (uint16_t)(p_state_ptr->bytes_per_frame - p_state_ptr->rx_frame_cntr);
        }
        else
        {
            num_bytes = 4U;
        }
        /* Generate the word which will be write in buffer. */
        if (!p_state_ptr->rx_mute_cnt)
        {
            if (p_state_ptr->b_access_read && (p_state_ptr->rx_prog_cntr < p_state_ptr->data_len))
            {
                p_state_ptr->p_data_buf_ptr[p_state_ptr->rx_prog_cntr] = (uint8_t)received_word;
                p_state_ptr->rx_prog_cntr++;
            }
        }
        else
        {
            p_state_ptr->rx_mute_cnt--;
        }

        filled_space = (uint8_t)(filled_space - 1U);
        if (filled_space == 0U)
        {
            b_exit = true;
        }
    }
}

static void
sfspim_disable_teie_intrpts(void)
{
    DEV_ASSERT(sp_sfspim_state_ptr != NULL);
    sfspim_state_t *p_state_ptr = sp_sfspim_state_ptr;
    LPSPI_t        *p_base      = p_state_ptr->p_base;

    lpspi_set_int_mode(p_base, LPSPI_TRANSMIT_ERROR, false);
    lpspi_set_int_mode(p_base, LPSPI_RECEIVE_ERROR, false);
    (void)lpspi_clear_status_flag(p_base, LPSPI_TRANSMIT_ERROR);
    (void)lpspi_clear_status_flag(p_base, LPSPI_RECEIVE_ERROR);
}

static void
sfspim_layer_convert(sfspim_cmd_wrapper_t *p_cmd_wrap)
{
    DEV_ASSERT(sp_sfspim_state_ptr != NULL);
    sfspim_state_t *p_state_ptr = sp_sfspim_state_ptr;
    LPSPI_t        *p_base      = p_state_ptr->p_base;

    DEV_ASSERT(NULL != p_cmd_wrap);
    /* Check that we're not busy. */
    // DEV_ASSERT(lpspi_get_status_flag(p_base, LPSPI_MODULE_BUSY) == false);

    /* Verify if the number of bytes is divided by number of bytes/frame. */
    DEV_ASSERT((p_cmd_wrap->data_len % p_state_ptr->bytes_per_frame) == (uint16_t)0);

    lpspi_tx_cmd_config_t tx_cmd_cfg = p_state_ptr->tx_cmd_cfg;
    lpspi_set_tx_cmd_reg_tcr(p_base, &tx_cmd_cfg);
#if defined(_SPI_TCR_NONCOMMON_REG_)
    uint32_t reg = (((uint32_t)tx_cmd_cfg.clk_polarity << LPSPI_TCR_CPOL_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.clk_phase << LPSPI_TCR_CPHA_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.pre_div << LPSPI_TCR_PRESCALE_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.which_pcs << LPSPI_TCR_PCS_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.b_lsb_first << LPSPI_TCR_LSBF_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.b_byte_swap << LPSPI_TCR_BYSW_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.b_cont_xfer << LPSPI_TCR_CONT_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.b_cont_cmd << LPSPI_TCR_CONTC_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.b_rx_mask << LPSPI_TCR_RXMSK_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.b_tx_mask << LPSPI_TCR_TXMSK_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.width << LPSPI_TCR_WIDTH_SHIFT) |
                    ((uint32_t)(tx_cmd_cfg.frame_size - 1UL) << LPSPI_TCR_FRAMESZ_SHIFT));
    while (p_base->TCR != reg)
        ;
    p_state_ptr->reg_tcr_persist = p_base->TCR;
#endif /* _SPI_TCR_NONCOMMON_REG_ */
    if (p_state_ptr->xfer_type == SFSPIM_USING_INTERRUPTS)
    {
        /* Fill out the other members of the run-time state structure. */
        p_state_ptr->p_tx_buffer    = p_cmd_wrap->p_data_buf_ptr;
        p_state_ptr->p_rx_buffer    = p_cmd_wrap->p_data_buf_ptr;
        p_state_ptr->tx_frame_cntr  = 0;
        p_state_ptr->rx_frame_cntr  = 0;
        p_state_ptr->tx_remain_cntr = p_cmd_wrap->sig_cmd_len + p_cmd_wrap->sig_addr_len +
                                      p_cmd_wrap->dummy_len + p_cmd_wrap->data_len;
        p_state_ptr->rx_remain_cntr = p_state_ptr->tx_remain_cntr;

        // uartPrintBuf((uint8_t*)p_cmd_wrap, sizeof(sfspim_cmd_wrapper_t));
        /* Fill out the other members of the run-time state structure. */
        p_state_ptr->sig_cmd_len  = p_cmd_wrap->sig_cmd_len;
        p_state_ptr->sig_addr_len = p_cmd_wrap->sig_addr_len;
        p_state_ptr->dummy_len    = p_cmd_wrap->dummy_len;
        p_state_ptr->data_len     = p_cmd_wrap->data_len;
        if (p_state_ptr->b_lsb)
        {
            *((uint32_t *)p_state_ptr->cmd_buf)  = p_cmd_wrap->sig_cmd;
            *((uint32_t *)p_state_ptr->addr_buf) = p_cmd_wrap->sig_addr;
        }
        else
        {
            /** MSB payload first on timing sequence. */
            uint8_t pos;
            uint8_t
                order_by_le; /**< ordering by little endian. exactly fit Cortex-M0 endianness. */
            for (pos = 0, order_by_le = p_state_ptr->sig_cmd_len - 1;
                 pos < p_state_ptr->sig_cmd_len;
                 pos++, order_by_le--)
            {
                p_state_ptr->cmd_buf[pos] = *(((uint8_t *)&(p_cmd_wrap->sig_cmd)) + order_by_le);
            }
            for (pos = 0, order_by_le = p_state_ptr->sig_addr_len - 1;
                 pos < p_state_ptr->sig_addr_len;
                 pos++, order_by_le--)
            {
                p_state_ptr->addr_buf[pos] = *(((uint8_t *)&(p_cmd_wrap->sig_addr)) + order_by_le);
            }
        }
        p_state_ptr->dummy_symbol      = p_cmd_wrap->dummy_symbol;
        p_state_ptr->p_data_buf_ptr    = p_cmd_wrap->p_data_buf_ptr;
        p_state_ptr->state_machine     = SFSPIM_CMD_PHASE;
        p_state_ptr->b_keep_continuing = false;
        p_state_ptr->b_tx_frobidden    = false;
        if (p_cmd_wrap->b_access_read)
        {
            p_state_ptr->b_access_read = true;
        }
        else
        {
            p_state_ptr->b_access_read = false;
        }
        p_state_ptr->tx_prog_cntr = 0;
        p_state_ptr->rx_prog_cntr = 0;
        p_state_ptr->tx_mute_cnt  = 0;
        p_state_ptr->rx_mute_cnt  = (uint16_t)p_state_ptr->sig_cmd_len +
                                   (uint16_t)p_state_ptr->sig_addr_len +
                                   (uint16_t)p_state_ptr->dummy_len;
        if (!p_state_ptr->b_access_read)
        {
            p_state_ptr->rx_mute_cnt += p_state_ptr->data_len;
        }
    }
    else
    {
    }
}

static void
sfspim_start_xfer(bool b_one_wire_bidirection)
{
    DEV_ASSERT(sp_sfspim_state_ptr != NULL);
    sfspim_state_t *p_state_ptr = sp_sfspim_state_ptr;
    LPSPI_t        *p_base      = p_state_ptr->p_base;

#if defined(_SPI_TCR_NONCOMMON_REG_)
    p_state_ptr->reg_tcr_persist &=
        ~(LPSPI_TCR_CONTC_MASK | LPSPI_TCR_RXMSK_MASK | LPSPI_TCR_TXMSK_MASK);
#endif /* _SPI_TCR_NONCOMMON_REG_ */
    /* Check that we're not busy. */
    DEV_ASSERT(lpspi_get_status_flag(p_base, LPSPI_MODULE_BUSY) == false);

    /* Clean RX and TX buffers */
    lpspi_set_flush_fifo_cmd(p_base, true, true);
    /* The second flush command is used to avoid the case when one word is still in shifter. */
    lpspi_set_flush_fifo_cmd(p_base, true, true);

    if (p_state_ptr->b_pcs_continuous == true)
    {
#if defined(_SPI_TCR_NONCOMMON_REG_)
        p_state_ptr->reg_tcr_persist |= (LPSPI_TCR_CONTC_MASK);
#else  /* _SPI_TCR_NONCOMMON_REG_ */
        lpspi_set_cont_cmd_bit(p_base);
#endif /* _SPI_TCR_NONCOMMON_REG_ */
    }
    p_state_ptr->xfer_status = SFSPIM_TRANSFER_OK;
    /* Clear all interrupts sources */
    (void)lpspi_clear_status_flag(p_base, LPSPI_ALL_STATUS);
    /* Enable fault interrupts sources */
    lpspi_set_int_mode(p_base, LPSPI_TRANSMIT_ERROR, true);
    lpspi_set_int_mode(p_base, LPSPI_RECEIVE_ERROR, true);

    /* Configure rx_remain_cntr depending on transfer type.*/
    if (p_state_ptr->b_indep_data_wire && p_state_ptr->b_full_duplex)
    {
#if defined(_SPI_TCR_NONCOMMON_REG_)
        p_state_ptr->reg_tcr_persist &= ~(LPSPI_TCR_TXMSK_MASK);
        p_state_ptr->reg_tcr_persist &= ~(LPSPI_TCR_RXMSK_MASK);
#else  /* _SPI_TCR_NONCOMMON_REG_ */
        lpspi_clear_tx_mask_bit(p_base);
        lpspi_clear_rx_mask_bit(p_base);
#endif /* _SPI_TCR_NONCOMMON_REG_ */
    }
    else if (!p_state_ptr->b_full_duplex)
    {
#if defined(_SPI_TCR_NONCOMMON_REG_)
        p_state_ptr->reg_tcr_persist &= ~(LPSPI_TCR_TXMSK_MASK);
        p_state_ptr->reg_tcr_persist |= (LPSPI_TCR_RXMSK_MASK);
#else  /* _SPI_TCR_NONCOMMON_REG_ */
        lpspi_clear_tx_mask_bit(p_base);
        lpspi_set_rx_mask_bit(p_base);
#endif /* _SPI_TCR_NONCOMMON_REG_ */
    }

#if defined(_SPI_TCR_NONCOMMON_REG_)
    p_base->TCR = p_state_ptr->reg_tcr_persist;
#endif /* _SPI_TCR_NONCOMMON_REG_ */
    if (true == p_state_ptr->b_alt_pcs)
    {
        if (LPSPI_ACTIVE_LOW == p_state_ptr->pcs_polarity)
        {
            p_base->XCSR &= ~LPSPI_XCSR_MCSO_MASK;
        }
        else
        {
            p_base->XCSR |= LPSPI_XCSR_MCSO_MASK;
        }
        p_base->XCSR |= LPSPI_XCSR_MCSEN_MASK;
    }

    /* Configure watermarks */
    lpspi_set_rx_watermarks(p_base, 0U);
    lpspi_set_tx_watermarks(p_base, 2U);

    if (p_state_ptr->xfer_type == SFSPIM_USING_INTERRUPTS)
    {
        /* Update transfer status */
        p_state_ptr->b_xfer_in_progress = true;
        if (p_state_ptr->b_full_duplex)
        {
            lpspi_set_int_mode(p_base, LPSPI_RX_DATA_FLAG, true);
            lpspi_set_int_mode(p_base, LPSPI_TX_DATA_FLAG, true);
        }
        else if (b_one_wire_bidirection)
        {
            lpspi_set_int_mode(p_base, LPSPI_RX_DATA_FLAG, false);
            lpspi_set_int_mode(p_base, LPSPI_TX_DATA_FLAG, true);
        }
        else
        {
            lpspi_set_int_mode(p_base, LPSPI_RX_DATA_FLAG, true);
            lpspi_set_int_mode(p_base, LPSPI_TX_DATA_FLAG, true);
        }
    }
    else
    {
    }
}

static void
sfspim_complete_xfer(void)
{
    DEV_ASSERT(sp_sfspim_state_ptr != NULL);
    sfspim_state_t *p_state_ptr = sp_sfspim_state_ptr;
    LPSPI_t        *p_base      = p_state_ptr->p_base;
    /* The transfer is complete.*/
    p_state_ptr->b_xfer_in_progress = false;
    if (p_state_ptr->xfer_type == SFSPIM_USING_DMA)
    {
        /* Disable LPSPI DMA request */
        lpspi_set_rx_dma_cmd(p_base, false);
        lpspi_set_tx_dma_cmd(p_base, false);
    }
    else
    {
        /* Disable (clear) interrupt requests */
        lpspi_set_int_mode(p_base, LPSPI_RX_DATA_FLAG, false);
        lpspi_set_int_mode(p_base, LPSPI_TX_DATA_FLAG, false);
    }

    sfspim_disable_teie_intrpts();
    lpspi_set_int_mode(p_base, LPSPI_TRANSFER_COMPLETE, false);
    (void)lpspi_clear_status_flag(p_base, LPSPI_TRANSFER_COMPLETE);
    if (p_state_ptr->b_blocking == true)
    {
        (void)osif_sema_post(&(p_state_ptr->sema_blocking_xfer));
        p_state_ptr->b_blocking = false;
    }
    if (p_state_ptr->callback != NULL)
    {
        p_state_ptr->callback(p_state_ptr, SPI_EVENT_END_TRANSFER, p_state_ptr->p_callback_param);
    }
    if (true == p_state_ptr->b_alt_pcs)
    {
        if (LPSPI_ACTIVE_LOW == p_state_ptr->pcs_polarity)
        {
            p_base->XCSR &= ~LPSPI_XCSR_MCSO_MASK;
        }
        else
        {
            p_base->XCSR |= LPSPI_XCSR_MCSO_MASK;
        }
        p_base->XCSR &= ~LPSPI_XCSR_MCSEN_MASK;
    }
}

static status_t
sfspim_abort_xfer(void)
{
    DEV_ASSERT(sp_sfspim_state_ptr != NULL);
    sfspim_state_t *p_state_ptr = sp_sfspim_state_ptr;
    LPSPI_t        *p_base      = p_state_ptr->p_base;
    /* Stop the running transfer. */
    sfspim_complete_xfer();
    lpspi_set_flush_fifo_cmd(p_base, true, true);
    /* The second flush command is used to avoid the case when one word is still in shifter. */
    lpspi_set_flush_fifo_cmd(p_base, true, true);
    return STATUS_SUCCESS;
}

static status_t
sfspim_configure_bus(const sfspim_config_t *p_init_config, uint32_t *p_calc_baud_rate)
{
    DEV_ASSERT(p_init_config != NULL);
    DEV_ASSERT(sp_sfspim_state_ptr != NULL);
    /* Instantiate local variable of type lpspi_state_t and point to global state */
    sfspim_state_t *p_state_ptr = sp_sfspim_state_ptr;
    LPSPI_t        *p_base      = p_state_ptr->p_base;
    uint32_t        baud_rate;

    /* The Transmit Command Register (TCR) Prescale value is calculated as part of the baud rate
     calculation. The value is stored in the run-time state structure for later programming
     in the TCR. */
    uint32_t tcr_prescale_value;

    /* First, per the spec, we need to disable the LPSPI module before setting the delay */

    if (lpspi_disable(p_base) != STATUS_SUCCESS)
    {
        /* If error is returned, the LPSPI is busy */
        return STATUS_ERROR;
    }

    /* Check the bit count to make sure it falls within the boundary conditions */
    if ((p_init_config->bits_per_frame < 8U) || (p_init_config->bits_per_frame > 4096U))
    {
        return STATUS_ERROR;
    }

    /* Configure internal state structure for LPSPI */
    p_state_ptr->bits_per_frame    = p_init_config->bits_per_frame;
    p_state_ptr->pcs_polarity      = p_init_config->pcs_polarity;
    p_state_ptr->b_pcs_continuous  = p_init_config->b_pcs_continuous;
    p_state_ptr->b_alt_pcs         = p_init_config->b_alt_pcs;
    p_state_ptr->b_indep_data_wire = p_init_config->b_indep_data_wire;
    p_state_ptr->b_full_duplex     = p_init_config->b_full_duplex;
    (void)clock_get_freq(*(p_state_ptr->p_clk_name), &(p_state_ptr->src_clk_freq));
    if (p_init_config->xfer_width == LPSPI_SINGLE_BIT_XFER)
    {
        if (!p_init_config->b_indep_data_wire)
        {
            DEV_ASSERT(p_init_config->b_full_duplex == false);
        }
    }
    else
    {
        DEV_ASSERT(p_init_config->b_indep_data_wire == false);
    }
    p_state_ptr->b_lsb = p_init_config->b_lsb_first;
    /* Save transfer type DMA/Interrupt */
    p_state_ptr->xfer_type = p_init_config->xfer_type;
    /* Update transfer status */
    p_state_ptr->b_xfer_in_progress = false;
    p_state_ptr->b_blocking         = false;
    /* Calculate the bytes/frame for lpspiState->bytesPerFrame. */
    p_state_ptr->bytes_per_frame = (uint16_t)((p_state_ptr->bits_per_frame + 7U) / 8U);
    /* Due to DMA limitations frames of 3 bytes/frame will be internally handled as 4 bytes/frame.
     */
    if (p_state_ptr->bytes_per_frame == 3U)
    {
        p_state_ptr->bytes_per_frame = 4U;
    }
    /* Due to some limitations all frames bigger than 4 bytes/frame must be composed only from 4
     * bytes chunks. */
    if (p_state_ptr->bytes_per_frame > 4U)
    {
        p_state_ptr->bytes_per_frame = (((p_state_ptr->bytes_per_frame - 1U) / 4U) + 1U) * 4U;
    }
    /* Store DMA channel number used in transfer */
    p_state_ptr->rx_dma_channel = p_init_config->rx_dma_channel;
    p_state_ptr->tx_dma_channel = p_init_config->tx_dma_channel;
    /* Store callback */
    p_state_ptr->callback         = p_init_config->callback;
    p_state_ptr->p_callback_param = p_init_config->p_callback_param;
    /* Configure the desired PCS polarity */
    (void)lpspi_set_pcs_polarity_mode(
        p_base, p_init_config->which_pcs, p_init_config->pcs_polarity);
    /* Set up the baud rate */
    baud_rate = lpspi_set_baud_rate(
        p_base, p_init_config->baud_rate, p_state_ptr->src_clk_freq, &tcr_prescale_value);
#if 0
    /* Enable sampling point delay */
    lpspi_set_sampling_point(p_base, true);
#endif /* _CW900X_ */
    /* Now, re-enable the LPSPI module */
    lpspi_enable(p_base);
    /* If the baud rate return is "0", it means there was an error */
    if (baud_rate == (uint32_t)0)
    {
        return STATUS_ERROR;
    }
    /* If the user wishes to know the calculated baud rate, then pass it back */
    if (p_calc_baud_rate != NULL)
    {
        *p_calc_baud_rate = baud_rate;
    }
    /* Write the TCR for this transfer. */
    lpspi_tx_cmd_config_t tx_cmd_cfg = {.frame_size   = p_state_ptr->bits_per_frame,
                                        .width        = p_init_config->xfer_width,
                                        .b_tx_mask    = false,
                                        .b_rx_mask    = false,
                                        .b_cont_cmd   = false,
                                        .b_cont_xfer  = p_init_config->b_pcs_continuous,
                                        .b_byte_swap  = true,
                                        .b_lsb_first  = p_init_config->b_lsb_first,
                                        .which_pcs    = p_init_config->which_pcs,
                                        .pre_div      = tcr_prescale_value,
                                        .clk_phase    = p_init_config->clk_phase,
                                        .clk_polarity = p_init_config->clk_polarity};
    p_state_ptr->tx_cmd_cfg          = tx_cmd_cfg;
    lpspi_set_tx_cmd_reg_tcr(p_base, &tx_cmd_cfg);
#if defined(_SPI_TCR_NONCOMMON_REG_)
    uint32_t reg = (((uint32_t)tx_cmd_cfg.clk_polarity << LPSPI_TCR_CPOL_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.clk_phase << LPSPI_TCR_CPHA_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.pre_div << LPSPI_TCR_PRESCALE_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.which_pcs << LPSPI_TCR_PCS_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.b_lsb_first << LPSPI_TCR_LSBF_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.b_byte_swap << LPSPI_TCR_BYSW_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.b_cont_xfer << LPSPI_TCR_CONT_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.b_cont_cmd << LPSPI_TCR_CONTC_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.b_rx_mask << LPSPI_TCR_RXMSK_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.b_tx_mask << LPSPI_TCR_TXMSK_SHIFT) |
                    ((uint32_t)tx_cmd_cfg.width << LPSPI_TCR_WIDTH_SHIFT) |
                    ((uint32_t)(tx_cmd_cfg.frame_size - 1UL) << LPSPI_TCR_FRAMESZ_SHIFT));
    while (p_base->TCR != reg)
        ;
    p_state_ptr->reg_tcr_persist = p_base->TCR;
#endif /* _SPI_TCR_NONCOMMON_REG_ */

    return STATUS_SUCCESS;
}

/*** end of file ***/
