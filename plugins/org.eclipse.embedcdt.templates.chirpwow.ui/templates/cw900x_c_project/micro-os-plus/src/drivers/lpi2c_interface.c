/**
 * @file lpi2c_interface.c
 * @brief An application interface with respect to both master and slave
 * for I2C bus.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <clock_gating.h>
#include <string.h>
#include "lpi2c_interface.h"
#include "interrupt_manager.h"
#include "edma_driver.h"

/*******************************************************************************
 * Private macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/* Constraints used for baud rate computation */
#define CLKHI_MIN_VALUE   1U
#define CLKLO_MIN_VALUE   3U
#define CLKHI_MAX_VALUE   ((1U << LPI2C_MCCR0_CLKHI_WIDTH) - 1U)
#define CLKLO_MAX_VALUE   CLKHI_MAX_VALUE
#define DATAVD_MIN_VALUE  1U
#define SETHOLD_MIN_VALUE 2U
#define BUSIDLE_MAX_VALUE 0xFFFU

/*******************************************************************************
 * Private typedefs
 ******************************************************************************/
/*! @brief Direction of a LPI2C transfer - transmit or receive. */
typedef enum
{
    LPI2C_TX_REQ = 0, /*!< The driver will perform an I2C transmit transfer */
    LPI2C_RX_REQ = 1, /*!< The driver will perform an I2C receive transfer */
} lpi2c_transfer_direction_t;

/*!
 * @brief DMA internal parameters structure
 *
 * This structure is used in DMA transfers. It contains different
 * variables required for setting and maintaining a DMA transfer.
 */
typedef struct
{
    /*! @cond DRIVER_INTERNAL_USE_ONLY */
    uint8_t              dma_channel;       /* Channel number for the DMA channel */
    edma_transfer_type_t edma_xfer_type;    /* Type for the DMA transfer */
    uint32_t             i2c_data_reg_addr; /* An i2c data register address */
    uint8_t             *p_xfer_buffer;     /* Buffer used for transfer */
    uint32_t             xfer_size;         /* Size of the data to be transfered */
    lpi2c_transfer_direction_t
        xfer_direction; /* Tells if the driver will make a receive or transmit DMA transfer */
    /*! @endcond */
} lpi2c_dma_transfer_params_t;

/*******************************************************************************
 * Global variables
 ******************************************************************************/
lpi2cif_state_t g_lpi2cif_inst0_state = {0u};

const lpi2cif_master_config_t g_lpi2cif_inst0_master_config = {.dev_id_addr         = 50U,
                                                               .b_10bit_dev_id_addr = false,
                                                               .operating_mode = LPI2C_FAST_MODE,
                                                               .baud_rate      = 200000UL,
                                                               .xfer_type =
                                                                   LPI2CIF_USING_INTERRUPTS,
                                                               .dma_channel      = 0U,
                                                               .callback         = NULL,
                                                               .p_callback_param = NULL};

const lpi2cif_slave_config_t g_lpi2cif_inst0_slave_config = {.dev_id_addr         = 50U,
                                                             .b_10bit_dev_id_addr = false,
                                                             .operating_mode      = LPI2C_FAST_MODE,
                                                             .b_listening_on_demand = true,
                                                             .xfer_type = LPI2CIF_USING_INTERRUPTS,
                                                             .dma_channel      = 0U,
                                                             .callback         = NULL,
                                                             .p_callback_param = NULL};

lpi2cif_state_t g_lpi2cif_inst1_state = {0u};

const lpi2cif_master_config_t g_lpi2cif_inst1_master_config = {.dev_id_addr         = 50U,
                                                               .b_10bit_dev_id_addr = false,
                                                               .operating_mode = LPI2C_FAST_MODE,
                                                               .baud_rate      = 200000UL,
                                                               .xfer_type =
                                                                   LPI2CIF_USING_INTERRUPTS,
                                                               .dma_channel      = 0U,
                                                               .callback         = NULL,
                                                               .p_callback_param = NULL};

const lpi2cif_slave_config_t g_lpi2cif_inst1_slave_config = {.dev_id_addr         = 50U,
                                                             .b_10bit_dev_id_addr = false,
                                                             .operating_mode      = LPI2C_FAST_MODE,
                                                             .b_listening_on_demand = true,
                                                             .xfer_type = LPI2CIF_USING_INTERRUPTS,
                                                             .dma_channel      = 0U,
                                                             .callback         = NULL,
                                                             .p_callback_param = NULL};

/*******************************************************************************
 * Static variables
 ******************************************************************************/
/* Pointer to runtime state structure.*/
static lpi2cif_state_t *sp_lpi2cif_state_ptr[LPI2C_INSTANCE_COUNT] =
    FEATURE_LPI2C_STATE_STRUCTURES_NULL;

/* LPI2C DMA request sources */
static const uint8_t s_lpi2cif_dma_src[LPI2C_INSTANCE_COUNT][2] = LPI2C_EDMA_REQ;

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
static void lpi2cif_master_isr(uint32_t instance);

static void lpi2cif_master_handle_xmt_data_req(LPI2C_t *p_base, lpi2cif_master_state_t *p_master);

static void lpi2cif_master_handle_rcv_data_rdy(LPI2C_t *p_base, lpi2cif_master_state_t *p_master);

static status_t lpi2cif_master_reinit(uint32_t instance, lpi2cif_master_state_t *p_master);

static void lpi2cif_master_set_operating_mode(uint32_t instance, lpi2c_mode_t operating_mode);

static inline bool lpi2cif_master_chk_cmd_queue_empty(const lpi2cif_master_state_t *p_master);

static inline void lpi2cif_master_reset_queue(lpi2cif_master_state_t *p_master);

static inline void lpi2cif_master_queue_cmd(LPI2C_t                *p_base,
                                            lpi2cif_master_state_t *p_master,
                                            lpi2c_master_command_t  cmd,
                                            uint8_t                 data);

static inline void lpi2cif_master_send_queued_cmd(LPI2C_t                *p_base,
                                                  lpi2cif_master_state_t *p_master);

static void lpi2cif_master_send_address(LPI2C_t                *p_base,
                                        lpi2cif_master_state_t *p_master,
                                        bool                    receive);

static void lpi2cif_master_queue_data(LPI2C_t *p_base, lpi2cif_master_state_t *p_master);

static void lpi2cif_master_end_xfer(LPI2C_t                *p_base,
                                    lpi2cif_master_state_t *p_master,
                                    bool                    b_end_stop,
                                    bool                    resetFIFO);

static status_t lpi2cif_master_wait_xfer_end(uint32_t instance, uint32_t timeout);

static void lpi2cif_master_start_dma_xfer(uint32_t instance);

static void lpi2cif_master_complete_dma_xfer(void *parameter, edma_chn_status_t status);

static void lpi2cif_slave_isr(uint32_t instance);

static void lpi2cif_slave_handle_dev_id_addr_valid(uint32_t               instance,
                                                   LPI2C_t               *p_base,
                                                   lpi2cif_slave_state_t *p_slave);

static void lpi2cif_slave_handle_xmt_data_req(LPI2C_t *p_base, lpi2cif_slave_state_t *p_slave);

static void lpi2cif_slave_handle_rcv_data_rdy(const LPI2C_t         *p_base,
                                              lpi2cif_slave_state_t *p_slave);

static void lpi2cif_slave_handle_end_xfer(lpi2cif_slave_state_t *p_slave, LPI2C_t *p_base);

static void lpi2cif_slave_set_operating_mode(uint32_t instance, lpi2c_mode_t operating_mode);

static void lpi2cif_slave_end_xfer(LPI2C_t *p_base, lpi2cif_slave_state_t *p_slave);

static status_t lpi2cif_slave_wait_xfer_end(uint32_t instance, uint32_t timeout);

static void lpi2cif_slave_start_dma_xfer(uint32_t instance);

static void lpi2cif_configure_dma_xfer(uint32_t                           instance,
                                       const lpi2c_dma_transfer_params_t *dma_xfer_params);

/*******************************************************************************
 * Global functions
 ******************************************************************************/
/* Implementation of LPI2C0 master and slave handler named in startup code. */
void
LPI2C0_IRQHandler(void)
{
    DEV_ASSERT(sp_lpi2cif_state_ptr[INST_LPI2C0] != NULL);
    /* Check if module is master or slave */
    if (true == sp_lpi2cif_state_ptr[INST_LPI2C0]->b_is_master)
    {
        lpi2cif_master_isr(INST_LPI2C0);
    }
    else
    {
        lpi2cif_slave_isr(INST_LPI2C0);
    }
}

/* Implementation of LPI2C1 master and slave handler named in startup code. */
void
LPI2C1_IRQHandler(void)
{
    DEV_ASSERT(sp_lpi2cif_state_ptr[INST_LPI2C1] != NULL);
    /* Check if module is master or slave */
    if (true == sp_lpi2cif_state_ptr[INST_LPI2C1]->b_is_master)
    {
        lpi2cif_master_isr(INST_LPI2C1);
    }
    else
    {
        lpi2cif_slave_isr(INST_LPI2C1);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_init
 * Description   : initialize the I2C master mode driver
 *
 * Implements : LPI2C_DRV_MasterInit_Activity
 *END**************************************************************************/
status_t
lpi2cif_master_init(uint32_t                       instance,
                    lpi2cif_state_t               *p_state_ptr,
                    const lpi2cif_master_config_t *p_init_config)
{
    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(p_state_ptr != NULL);
    DEV_ASSERT(p_init_config != NULL);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] == NULL);
    LPI2C_t                 *p_base   = NULL;
    lpi2cif_master_state_t  *p_master = NULL;
    status_t                 sts_err_code;
    uint32_t                 input_clock;
    lpi2cif_mode_baud_rate_t mode_baud_rate;
    /** Clear the state struct for this instance. */
    memset((void *)p_state_ptr, 0x00u, sizeof(lpi2cif_state_t));
    /** Save runtime structure pointers so irq handler can point to the correct state structure. */
    sp_lpi2cif_state_ptr[instance] = p_state_ptr;
    p_state_ptr->p_base = p_base = gp_lpi2c_base[instance];
    p_state_ptr->p_irq_id        = &g_lpi2c_irq_id[instance];
    p_state_ptr->p_clk_name      = &g_lpi2c_clk_names[instance];
    p_state_ptr->b_is_master     = true;
    p_master                     = &(p_state_ptr->master);

    /* Check the protocol clock frequency */
    sts_err_code = clock_get_freq(*(p_state_ptr->p_clk_name), &input_clock);
    DEV_ASSERT(sts_err_code == STATUS_SUCCESS);
    DEV_ASSERT(input_clock > 0U);

    /* Initialize driver status structure */
    p_master->p_rx_buffer         = NULL;
    p_master->rx_cntr             = 0U;
    p_master->p_tx_buffer         = NULL;
    p_master->tx_cntr             = 0U;
    p_master->status              = STATUS_SUCCESS;
    p_master->b_i2c_idle          = true;
    p_master->dev_id_addr         = p_init_config->dev_id_addr;
    p_master->b_10bit_dev_id_addr = p_init_config->b_10bit_dev_id_addr;
    p_master->xfer_type           = p_init_config->xfer_type;
    /* Store DMA channel number used in transfer */
    p_master->dma_channel      = p_init_config->dma_channel;
    p_master->callback         = p_init_config->callback;
    p_master->p_callback_param = p_init_config->p_callback_param;
#if (LPI2C_HAS_HIGH_SPEED_MODE)
    p_master->master_code      = p_init_config->master_code;
    p_master->b_hs_in_progress = false;
#endif
    p_master->blocking     = false;
    p_master->baud_rate    = p_init_config->baud_rate;
    p_master->b_abort_xfer = false;

    /* Initialize the semaphore */
    sts_err_code = osif_sema_create(&(p_master->sema_blocking_xfer), 0);
    DEV_ASSERT(sts_err_code == STATUS_SUCCESS);

    lpi2cif_master_reset_queue(p_master);

    /* Enable lpi2c interrupt */
    int_enable_irq(*(p_state_ptr->p_irq_id));

    /* Initialize module */
    lpi2c_init(p_base);

    /* Set baud rate */
    mode_baud_rate.baud_rate = p_init_config->baud_rate;
#if (LPI2C_HAS_HIGH_SPEED_MODE)
    p_master->baud_rate_hs      = p_init_config->baud_rate_hs;
    mode_baud_rate.baud_rate_hs = p_init_config->baud_rate_hs;
#endif
    (void)lpi2cif_master_set_baud_rate(instance, p_init_config->operating_mode, &mode_baud_rate);

    /* Set slave address */
    lpi2cif_master_set_dev_id_addr(
        instance, p_init_config->dev_id_addr, p_init_config->b_10bit_dev_id_addr);

    /* Enable LPI2C master */
    lpi2c_set_master_enable(p_base, true);

    (void)sts_err_code;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_deinit
 * Description   : deinitialize the I2C master mode driver
 *
 * Implements : LPI2C_DRV_MasterDeinit_Activity
 *END**************************************************************************/
status_t
lpi2cif_master_deinit(uint32_t instance)
{
    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);
    LPI2C_t                      *p_base      = NULL;
    lpi2cif_state_t              *p_state_ptr = NULL;
    const lpi2cif_master_state_t *p_master    = NULL;
    p_base                                    = gp_lpi2c_base[instance];
    p_state_ptr                               = sp_lpi2cif_state_ptr[instance];
    p_master                                  = &(p_state_ptr->master);

    /* Destroy the semaphore */
    (void)osif_sema_destroy(&(p_master->sema_blocking_xfer));

    sp_lpi2cif_state_ptr[instance] = NULL;

    /* Disable master */
    lpi2c_set_master_enable(p_base, false);

    /* Disable i2c interrupt */
    int_disable_irq(*(p_state_ptr->p_irq_id));

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_cfg_bus_idle_timeout
 * Description   : Set bus idle timeout period for I2C master mode driver
 *
 * Implements : LPI2C_DRV_SetMasterBusIdleTimeout_Activity
 *END**************************************************************************/
void
lpi2cif_master_cfg_bus_idle_timeout(uint32_t instance, uint16_t timeout)
{
    LPI2C_t *p_base;
    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    uint16_t timeout_temp = timeout;

    /* Limit timeout value since BUSIDLE field is 12-bit long */
    if (timeout_temp > BUSIDLE_MAX_VALUE)
    {
        timeout_temp = BUSIDLE_MAX_VALUE;
    }

    p_base = gp_lpi2c_base[instance];

    /* Disable master */
    lpi2c_set_master_enable(p_base, false);

    /*Set BUSIDLE value for MCFGR2*/
    lpi2c_set_master_bus_idle_timeout(p_base, timeout_temp);

    /* Ensable master */
    lpi2c_set_master_enable(p_base, true);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_get_baud_rate
 * Description   : returns the currently configured baud rate
 *
 * Implements : LPI2C_DRV_MasterGetBaudRate_Activity
 *END**************************************************************************/
void
lpi2cif_master_get_baud_rate(uint32_t instance, lpi2cif_mode_baud_rate_t *p_mode_baud_rate)
{
    const LPI2C_t                *p_base;
    const lpi2cif_master_state_t *p_master;
    status_t                      sts_err_code;
    uint32_t                      prescaler;
    uint32_t                      clkLo;
    uint32_t                      clkHi;
    uint32_t                      input_clock;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);

    p_base   = gp_lpi2c_base[instance];
    p_master = &(sp_lpi2cif_state_ptr[instance]->master);

    /* Get the protocol clock frequency */
    sts_err_code = clock_get_freq(*(sp_lpi2cif_state_ptr[instance]->p_clk_name), &input_clock);
    DEV_ASSERT(sts_err_code == STATUS_SUCCESS);
    DEV_ASSERT(input_clock > 0U);

    /* Ignoring the glitch filter, the baud rate formula is:
            SCL_freq = Input_freq / (2^PRESCALER * (CLKLO + CLKHI + 2))
    */
    prescaler = (uint32_t)lpi2c_get_master_prescaler(p_base);
    clkHi     = (uint32_t)lpi2c_get_master_clk_high_period(p_base);
    clkLo     = (uint32_t)lpi2c_get_master_clk_low_period(p_base);

    p_mode_baud_rate->baud_rate =
        input_clock / (((uint32_t)1U << prescaler) * (clkLo + clkHi + (uint32_t)2U));

#if (LPI2C_HAS_HIGH_SPEED_MODE)
    if (p_master->operating_mode == LPI2C_HIGHSPEED_MODE)
    {
        clkHi = lpi2c_get_master_clk_high_period_hs(p_base);
        clkLo = lpi2c_get_master_clk_low_period_hs(p_base);

        p_mode_baud_rate->baud_rate_hs =
            input_clock / (((uint32_t)1U << prescaler) * (clkLo + clkHi + (uint32_t)2U));
    }
#endif

    (void)sts_err_code;
    (void)p_master;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_set_baud_rate
 * Description   : set the baud rate for any subsequent I2C communication
 *
 * Implements : LPI2C_DRV_MasterSetBaudRate_Activity
 *END**************************************************************************/
status_t
lpi2cif_master_set_baud_rate(uint32_t                        instance,
                             const lpi2c_mode_t              operating_mode,
                             const lpi2cif_mode_baud_rate_t *p_mode_baud_rate)
{
    LPI2C_t                      *p_base;
    const lpi2cif_master_state_t *p_master;
    status_t                      sts_err_code;
    uint32_t                      input_clock;
    uint32_t                      minPrescaler = 0U;
    uint32_t                      prescaler;
    uint32_t                      clkTotal;
    uint32_t                      clkLo;
    uint32_t                      clkHi;
    uint32_t                      setHold;
    uint32_t                      dataVd;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);

    p_base   = gp_lpi2c_base[instance];
    p_master = &(sp_lpi2cif_state_ptr[instance]->master);

    /* Check if driver is busy */
    if (!p_master->b_i2c_idle)
    {
        return STATUS_BUSY;
    }

    /* Get the protocol clock frequency */
    sts_err_code = clock_get_freq(*(sp_lpi2cif_state_ptr[instance]->p_clk_name), &input_clock);
    DEV_ASSERT(sts_err_code == STATUS_SUCCESS);
    DEV_ASSERT(input_clock > 0U);

    /* Disable master */
    lpi2c_set_master_enable(p_base, false);

    /* Ignoring the glitch filter, the baud rate formula is:
            SCL_freq = Input_freq / (2^PRESCALER * (CLKLO + CLKHI + 2))
            Assume CLKLO = CLKHI, SETHOLD = CLKHI, DATAVD = CLKHI/2
    */

    if (p_mode_baud_rate->baud_rate != 0U)
    {
        /* Compute minimum prescaler for which CLKLO and CLKHI values are in valid range. Always
         * round up. */
        minPrescaler = ((input_clock - 1U) / ((p_mode_baud_rate->baud_rate) *
                                              (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U))) +
                       (uint32_t)1U;
        for (prescaler = 0U; prescaler < 7U; prescaler++)
        {
            if (((uint32_t)1U << prescaler) >= minPrescaler)
            {
                break;
            }
        }

        /* Compute CLKLO and CLKHI values for this prescaler. Round to nearest integer. */
        clkTotal = (input_clock + ((p_mode_baud_rate->baud_rate << prescaler) >> 1U)) /
                   (p_mode_baud_rate->baud_rate << prescaler);
    }
    else
    {
        prescaler = 7U;
        clkTotal  = (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U);
    }

    if (clkTotal > (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U))
    {
        clkTotal = (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U);
    }

    /*
     * If we try to compute clk high and low values using clkTotal equal with 0
     * (this is the case when the baudrate is 0), we will get negative values for
     * them, so we set them to 0 for this case.
     */
    if (clkTotal <= 1U)
    {
        clkHi = 0U;
        clkLo = 0U;
    }
    else
    {
        clkHi = (clkTotal - 2U) / 2U;
        clkLo = clkTotal - 2U - clkHi;
    }

    if (clkHi < CLKHI_MIN_VALUE)
    {
        clkHi = CLKHI_MIN_VALUE;
    }
    if (clkLo < CLKLO_MIN_VALUE)
    {
        clkLo = CLKLO_MIN_VALUE;
    }

    /* Compute DATAVD and SETHOLD */
    setHold = clkHi;
    dataVd  = clkHi >> 1U;
    if (setHold < SETHOLD_MIN_VALUE)
    {
        setHold = SETHOLD_MIN_VALUE;
    }
    if (dataVd < DATAVD_MIN_VALUE)
    {
        dataVd = DATAVD_MIN_VALUE;
    }

    /* Apply settings */
    lpi2c_set_master_prescaler(p_base, (lpi2c_master_prescaler_t)prescaler);
    lpi2c_set_master_data_valid_delay(p_base, (uint8_t)dataVd);
    lpi2c_set_master_setup_hold_delay(p_base, (uint8_t)setHold);
    lpi2c_set_master_clk_high_period(p_base, (uint8_t)clkHi);
    lpi2c_set_master_clk_low_period(p_base, (uint8_t)clkLo);

#if (LPI2C_HAS_HIGH_SPEED_MODE)
    if (operating_mode == LPI2C_HIGHSPEED_MODE)
    {
        /* Compute settings for High-speed baud rate */
        /* Compute High-speed CLKLO and CLKHI values for the same prescaler. Round to nearest
         * integer. */
        clkTotal = (input_clock + ((p_mode_baud_rate->baud_rate_hs << prescaler) >> 1U)) /
                   (p_mode_baud_rate->baud_rate_hs << prescaler);
        if (clkTotal > (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U))
        {
            clkTotal = (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U);
        }

        clkHi = (clkTotal - 2U) / 3U;
        clkLo = clkTotal - 2U - clkHi;
        if (clkHi < CLKHI_MIN_VALUE)
        {
            clkHi = CLKHI_MIN_VALUE;
        }
        if (clkLo < CLKLO_MIN_VALUE)
        {
            clkLo = CLKLO_MIN_VALUE;
        }

        /* Compute High-speed DATAVD and SETHOLD */
        setHold = clkHi;
        dataVd  = clkHi >> 1U;
        if (setHold < SETHOLD_MIN_VALUE)
        {
            setHold = SETHOLD_MIN_VALUE;
        }
        if (dataVd < DATAVD_MIN_VALUE)
        {
            dataVd = DATAVD_MIN_VALUE;
        }

        /* Apply High-speed settings */
        lpi2c_set_master_data_valid_delay_hs(p_base, (uint8_t)dataVd);
        lpi2c_set_master_setup_hold_delay_hs(p_base, (uint8_t)setHold);
        lpi2c_set_master_clk_high_period_hs(p_base, (uint8_t)clkHi);
        lpi2c_set_master_clk_low_period_hs(p_base, (uint8_t)clkLo);
    }
#endif

    /* Perform other settings related to the chosen operating mode */
    lpi2cif_master_set_operating_mode(instance, operating_mode);

    /* Re-enable master */
    lpi2c_set_master_enable(p_base, true);

    (void)minPrescaler;
    (void)p_master;
    (void)sts_err_code;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_set_dev_id_addr
 * Description   : set the slave address for any subsequent I2C communication
 *
 * Implements : LPI2C_DRV_MasterSetSlaveAddr_Activity
 *END**************************************************************************/
void
lpi2cif_master_set_dev_id_addr(uint32_t       instance,
                               const uint16_t address,
                               const bool     b_10bit_dev_id_addr)
{
    lpi2cif_master_state_t *p_master;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);

    p_master = &(sp_lpi2cif_state_ptr[instance]->master);

    p_master->dev_id_addr         = address;
    p_master->b_10bit_dev_id_addr = b_10bit_dev_id_addr;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_send_data
 * Description   : perform a non-blocking send transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_MasterSendData_Activity
 *END**************************************************************************/
status_t
lpi2cif_master_send_data(uint32_t       instance,
                         const uint8_t *p_tx_buffer,
                         uint32_t       xfer_byte_count,
                         bool           b_end_stop)
{
    LPI2C_t                *p_base;
    lpi2cif_master_state_t *p_master;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);
    DEV_ASSERT(p_tx_buffer != NULL);
    DEV_ASSERT(xfer_byte_count > 0U);

    p_base   = gp_lpi2c_base[instance];
    p_master = &(sp_lpi2cif_state_ptr[instance]->master);

    /* Check if driver is busy */
    if (!p_master->b_i2c_idle)
    {
        return STATUS_BUSY;
    }

    /* Copy parameters to driver state structure */
    p_master->p_tx_buffer = p_tx_buffer;
    p_master->tx_cntr     = xfer_byte_count;
    p_master->b_end_stop  = b_end_stop;
    p_master->b_i2c_idle  = false;
    p_master->status      = STATUS_BUSY;

    if (p_master->xfer_type == LPI2CIF_USING_DMA)
    {
        lpi2c_set_master_int(p_base,
                             LPI2C_MASTER_FIFO_ERROR_INT | LPI2C_MASTER_ARBITRATION_LOST_INT |
                                 LPI2C_MASTER_NACK_DETECT_INT,
                             true);

        lpi2cif_master_start_dma_xfer(instance);
    }
    else
    {
        /* Initiate communication */
        lpi2cif_master_send_address(p_base, p_master, false);

        /* Queue data bytes to fill tx fifo */
        lpi2cif_master_queue_data(p_base, p_master);

        /* Set tx FIFO watermark */
        lpi2c_set_master_tx_watermark(p_base, 0U);

        /* Enable relevant events */
#if (LPI2C_HAS_ULTRA_FAST_MODE)
        if (p_master->operating_mode == LPI2C_ULTRAFAST_MODE)
        {
            /* Do not enable NACK event reporting in ultra-fast mode */
            lpi2c_set_master_int(p_base,
                                 LPI2C_MASTER_FIFO_ERROR_INT | LPI2C_MASTER_ARBITRATION_LOST_INT |
                                     LPI2C_MASTER_TRANSMIT_DATA_INT,
                                 true);
        }
        else
#endif
        {
            lpi2c_set_master_int(p_base,
                                 LPI2C_MASTER_FIFO_ERROR_INT | LPI2C_MASTER_ARBITRATION_LOST_INT |
                                     LPI2C_MASTER_NACK_DETECT_INT | LPI2C_MASTER_TRANSMIT_DATA_INT,
                                 true);
        }
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_send_data_blocking
 * Description   : perform a blocking send transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_MasterSendDataBlocking_Activity
 *END**************************************************************************/
status_t
lpi2cif_master_send_data_blocking(uint32_t       instance,
                                  const uint8_t *p_tx_buffer,
                                  uint32_t       xfer_byte_count,
                                  bool           b_end_stop,
                                  uint32_t       timeout)
{

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);
    DEV_ASSERT(p_tx_buffer != NULL);
    DEV_ASSERT(xfer_byte_count > 0U);

    lpi2cif_master_state_t *p_master = &(sp_lpi2cif_state_ptr[instance]->master);

    /* Check if driver is busy */
    if (!p_master->b_i2c_idle)
    {
        return STATUS_BUSY;
    }

    /* mark transfer as blocking */
    p_master->blocking = true;

    /* Dummy wait to ensure the semaphore is 0, no need to check result */
    (void)osif_sema_wait(&(p_master->sema_blocking_xfer), 0);

    (void)lpi2cif_master_send_data(instance, p_tx_buffer, xfer_byte_count, b_end_stop);

#if defined(REFER_TO_REF_DESIGN)
    /* Wait for transfer to end */
    return lpi2cif_master_wait_xfer_end(instance, timeout);
#else  /* REFER_TO_REF_DESIGN */
    status_t sts_err_code = lpi2cif_master_wait_xfer_end(instance, timeout);
    LPI2C_t *p_base       = gp_lpi2c_base[instance];
    for (; lpi2c_get_master_status_busy(p_base);)
    {
    }
    return sts_err_code;
#endif /* REFER_TO_REF_DESIGN */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_rcv_data
 * Description   : perform a non-blocking receive transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_MasterReceiveData_Activity
 *END**************************************************************************/
status_t
lpi2cif_master_rcv_data(uint32_t instance,
                        uint8_t *p_rx_buffer,
                        uint32_t xfer_byte_count,
                        bool     b_end_stop)
{
    LPI2C_t                *p_base;
    lpi2cif_master_state_t *p_master;
    uint16_t                rxBytes;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);
    DEV_ASSERT(p_rx_buffer != NULL);
    DEV_ASSERT(xfer_byte_count > 0U);
    DEV_ASSERT(xfer_byte_count <= 256U);

    p_base   = gp_lpi2c_base[instance];
    p_master = &(sp_lpi2cif_state_ptr[instance]->master);

    /* Check if driver is busy */
    if (!p_master->b_i2c_idle)
    {
        return STATUS_BUSY;
    }

#if (LPI2C_HAS_ULTRA_FAST_MODE)
    if (p_master->operating_mode == LPI2C_ULTRAFAST_MODE)
    {
        /* No reception possible in ultra-fast mode */
        return STATUS_ERROR;
    }
#endif

    /* Copy parameters to driver state structure */
    p_master->rx_cntr     = xfer_byte_count;
    p_master->b_i2c_idle  = false;
    p_master->b_end_stop  = b_end_stop;
    p_master->p_rx_buffer = p_rx_buffer;
    p_master->status      = STATUS_BUSY;

    if (p_master->xfer_type == LPI2CIF_USING_DMA)
    {
        lpi2c_set_master_int(p_base,
                             LPI2C_MASTER_FIFO_ERROR_INT | LPI2C_MASTER_ARBITRATION_LOST_INT |
                                 LPI2C_MASTER_NACK_DETECT_INT,
                             true);

        lpi2cif_master_start_dma_xfer(instance);
    }
    else
    {
        /* Initiate communication */
        lpi2cif_master_send_address(p_base, p_master, true);
        /* Queue receive command for xfer_byte_count bytes */
        lpi2cif_master_queue_cmd(
            p_base, p_master, LPI2C_MASTER_COMMAND_RECEIVE, (uint8_t)(xfer_byte_count - 1U));

        /* Set rx FIFO watermark */
        rxBytes = lpi2c_get_master_rx_fifo_size(p_base);
        if (rxBytes > xfer_byte_count)
        {
            rxBytes = (uint8_t)xfer_byte_count;
        }
        lpi2c_set_master_rx_watermark(p_base, (uint16_t)(rxBytes - 1U));

        /* Enable relevant events */
        if (!lpi2cif_master_chk_cmd_queue_empty(p_master))
        {
            /* Enable tx event too if there are commands in the software FIFO */
            lpi2c_set_master_int(p_base,
                                 LPI2C_MASTER_FIFO_ERROR_INT | LPI2C_MASTER_ARBITRATION_LOST_INT |
                                     LPI2C_MASTER_NACK_DETECT_INT | LPI2C_MASTER_TRANSMIT_DATA_INT |
                                     LPI2C_MASTER_RECEIVE_DATA_INT,
                                 true);
        }
        else
        {
            lpi2c_set_master_int(p_base,
                                 LPI2C_MASTER_FIFO_ERROR_INT | LPI2C_MASTER_ARBITRATION_LOST_INT |
                                     LPI2C_MASTER_NACK_DETECT_INT | LPI2C_MASTER_RECEIVE_DATA_INT,
                                 true);
        }
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_rcv_data_blocking
 * Description   : perform a blocking receive transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_MasterReceiveDataBlocking_Activity
 *END**************************************************************************/
status_t
lpi2cif_master_rcv_data_blocking(uint32_t instance,
                                 uint8_t *p_rx_buffer,
                                 uint32_t xfer_byte_count,
                                 bool     b_end_stop,
                                 uint32_t timeout)
{
    status_t sts_err_code = STATUS_SUCCESS;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);
    DEV_ASSERT(p_rx_buffer != NULL);
    DEV_ASSERT(xfer_byte_count > 0U);

    lpi2cif_master_state_t *p_master = &(sp_lpi2cif_state_ptr[instance]->master);

    /* Check if driver is busy */
    if (!p_master->b_i2c_idle)
    {
        return STATUS_BUSY;
    }

    /* mark transfer as blocking */
    p_master->blocking = true;

    /* Dummy wait to ensure the semaphore is 0, no need to check result */
    (void)osif_sema_wait(&(p_master->sema_blocking_xfer), 0);

    sts_err_code = lpi2cif_master_rcv_data(instance, p_rx_buffer, xfer_byte_count, b_end_stop);

#if (LPI2C_HAS_ULTRA_FAST_MODE)
    if (sts_err_code != STATUS_SUCCESS)
    {
        p_master->blocking = false;
        return sts_err_code;
    }
#endif

    (void)sts_err_code;

    /* Wait for transfer to end */
    return lpi2cif_master_wait_xfer_end(instance, timeout);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_get_xfer_status
 * Description   : return the current status of the I2C master transfer
 *
 * When performing an a-sync (non-blocking) transfer, the user can call this function
 * to ascertain the state of the current transfer. In addition, if the transfer is still
 * in progress, the user can get the number of words that should be receive.
 *
 * Implements : LPI2C_DRV_MasterGetTransferStatus_Activity
 *END**************************************************************************/
status_t
lpi2cif_master_get_xfer_status(uint32_t instance, uint32_t *p_remain_byte_count)
{
    const LPI2C_t                *p_base;
    const lpi2cif_master_state_t *p_master;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);

    p_base   = gp_lpi2c_base[instance];
    p_master = &(sp_lpi2cif_state_ptr[instance]->master);

    if ((p_remain_byte_count != NULL) && (p_master->xfer_type == LPI2CIF_USING_INTERRUPTS))
    {
        if (p_master->tx_cntr > 0U)
        {
            /* Send data */
            /* Remaining bytes = bytes in buffer + bytes in tx FIFO */
            *p_remain_byte_count =
                p_master->tx_cntr + (uint32_t)lpi2c_get_master_tx_fifo_count(p_base);
        }
        else if (p_master->rx_cntr > 0U)
        {
            /* Receive data */
            /* Remaining bytes = free space in buffer - bytes in rx FIFO */
            *p_remain_byte_count =
                p_master->rx_cntr - (uint32_t)lpi2c_get_master_rx_fifo_count(p_base);
        }
        else
        {
            *p_remain_byte_count = 0U;
        }
    }

    return p_master->status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_abort_xfer_data
 * Description   : abort a non-blocking I2C Master transmission or reception
 *
 * Implements : LPI2C_DRV_MasterAbortTransferData_Activity
 *END**************************************************************************/
status_t
lpi2cif_master_abort_xfer_data(uint32_t instance)
{
    LPI2C_t                *p_base;
    lpi2cif_master_state_t *p_master;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);

    p_base   = gp_lpi2c_base[instance];
    p_master = &(sp_lpi2cif_state_ptr[instance]->master);

    if (p_master->p_rx_buffer != NULL)
    {
        /* Aborting a reception not supported because hardware will continue the
           current command even if the FIFO is reset and this could last indefinitely */
        return STATUS_UNSUPPORTED;
    }

    /* End transfer: force stop generation, reset FIFOs */
    p_master->status = STATUS_I2C_ABORTED;
    lpi2cif_master_end_xfer(p_base, p_master, true, true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_get_default_config
 * Description   : Gets the default configuration structure for master
 *
 * Implements : LPI2C_DRV_MasterGetDefaultConfig_Activity
 *END**************************************************************************/
void
lpi2cif_master_get_default_config(lpi2cif_master_config_t *p_config)
{
    p_config->dev_id_addr         = 32U;
    p_config->b_10bit_dev_id_addr = false;
    p_config->operating_mode      = LPI2C_STANDARD_MODE;
    p_config->baud_rate           = 100000U;
    p_config->xfer_type           = LPI2CIF_USING_INTERRUPTS;
    p_config->dma_channel         = 0U;
    p_config->callback            = NULL;
    p_config->p_callback_param    = NULL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_init
 * Description   : initialize the I2C slave mode driver
 *
 * Implements : LPI2C_DRV_SlaveInit_Activity
 *END**************************************************************************/
status_t
lpi2cif_slave_init(uint32_t                      instance,
                   lpi2cif_state_t              *p_state_ptr,
                   const lpi2cif_slave_config_t *p_init_config)
{
    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(p_state_ptr != NULL);
    DEV_ASSERT(p_init_config != NULL);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] == NULL);
    LPI2C_t                 *p_base  = NULL;
    lpi2cif_slave_state_t   *p_slave = NULL;
    status_t                 sts_err_code;
    uint32_t                 input_clock;
    lpi2cif_mode_baud_rate_t mode_baud_rate;
    /** Clear the state struct for this instance. */
    memset((void *)p_state_ptr, 0x00u, sizeof(lpi2cif_state_t));
    /** Save runtime structure pointers so irq handler can point to the correct state structure. */
    sp_lpi2cif_state_ptr[instance] = p_state_ptr;
    p_state_ptr->p_base = p_base = gp_lpi2c_base[instance];
    p_state_ptr->p_irq_id        = &g_lpi2c_irq_id[instance];
    p_state_ptr->p_clk_name      = &g_lpi2c_clk_names[instance];
    p_state_ptr->b_is_master     = false;
    p_slave                      = &(p_state_ptr->slave);

    /* Check the protocol clock frequency */
    sts_err_code = clock_get_freq(*(p_state_ptr->p_clk_name), &input_clock);
    DEV_ASSERT(sts_err_code == STATUS_SUCCESS);

    /* Initialize driver status structure */
    p_slave->status                = STATUS_SUCCESS;
    p_slave->b_listening_on_demand = p_init_config->b_listening_on_demand;
    p_slave->callback              = p_init_config->callback;
    p_slave->p_callback_param      = p_init_config->p_callback_param;
    p_slave->p_tx_buffer           = NULL;
    p_slave->p_rx_buffer           = NULL;
    p_slave->tx_cntr               = 0U;
    p_slave->rx_cntr               = 0U;
    p_slave->xfer_type             = p_init_config->xfer_type;
    /* Store DMA channel number used in transfer */
    p_slave->dma_channel         = p_init_config->dma_channel;
    p_slave->b_xfer_in_progress  = false;
    p_slave->blocking            = false;
    p_slave->b_10bit_dev_id_addr = p_init_config->b_10bit_dev_id_addr;
    p_slave->num_rept_start      = 0U;

    /* Initialize the semaphore */
    sts_err_code = osif_sema_create(&(p_slave->sema_blocking_xfer), 0);
    DEV_ASSERT(sts_err_code == STATUS_SUCCESS);

    /* Enable lpi2c interrupt */
    int_enable_irq(*(p_state_ptr->p_irq_id));

    /* Initialize module */
    lpi2c_init(p_base);

    /* Configure slave address */
    lpi2c_set_slave_match_addr0(p_base, p_init_config->dev_id_addr);
    if (p_init_config->b_10bit_dev_id_addr)
    {
        lpi2c_set_slave_addr_config(p_base, LPI2C_SLAVE_ADDR_MATCH_0_10BIT);
    }
    else
    {
        lpi2c_set_slave_addr_config(p_base, LPI2C_SLAVE_ADDR_MATCH_0_7BIT);
    }

    /* Configure operating mode */
    lpi2cif_slave_set_operating_mode(instance, p_init_config->operating_mode);

    if (p_init_config->b_listening_on_demand)
    {
        if (p_slave->xfer_type == LPI2CIF_USING_DMA)
        {
            /* Activate events */
            lpi2c_set_slave_int(p_base,
                                LPI2C_SLAVE_BIT_ERROR_INT | LPI2C_SLAVE_FIFO_ERROR_INT |
                                    LPI2C_SLAVE_STOP_DETECT_INT | LPI2C_SLAVE_REPEATED_START_INT |
                                    LPI2C_SLAVE_ADDRESS_VALID_INT,
                                true);
        }
        if (p_slave->xfer_type == LPI2CIF_USING_INTERRUPTS)
        {
            /* Activate events */
// #if defined(ERRATA_E10792) /**< Mute this line by Arthur 20230327. */
#if 1 /**< For merging SDK to turn on the statements. By Arthur 20230327. */
            lpi2c_set_slave_int(p_base,
                                LPI2C_SLAVE_BIT_ERROR_INT | LPI2C_SLAVE_FIFO_ERROR_INT |
                                    LPI2C_SLAVE_STOP_DETECT_INT | LPI2C_SLAVE_REPEATED_START_INT |
                                    LPI2C_SLAVE_ADDRESS_VALID_INT | LPI2C_SLAVE_RECEIVE_DATA_INT,
                                true);

#else
            lpi2c_set_slave_int(p_base,
                                LPI2C_SLAVE_BIT_ERROR_INT | LPI2C_SLAVE_FIFO_ERROR_INT |
                                    LPI2C_SLAVE_STOP_DETECT_INT | LPI2C_SLAVE_REPEATED_START_INT |
                                    LPI2C_SLAVE_ADDRESS_VALID_INT | LPI2C_SLAVE_RECEIVE_DATA_INT |
                                    LPI2C_SLAVE_TRANSMIT_DATA_INT,
                                true);

#endif
        }

        /* Enable LPI2C slave */
        lpi2c_set_slave_enable(p_base, true);
    }

    (void)sts_err_code;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_deinit
 * Description   : de-initialize the I2C slave mode driver
 *
 * Implements : LPI2C_DRV_SlaveDeinit_Activity
 *END**************************************************************************/
status_t
lpi2cif_slave_deinit(uint32_t instance)
{
    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);
    LPI2C_t                     *p_base      = NULL;
    lpi2cif_state_t             *p_state_ptr = NULL;
    const lpi2cif_slave_state_t *p_slave     = NULL;
    p_base                                   = gp_lpi2c_base[instance];
    p_state_ptr                              = sp_lpi2cif_state_ptr[instance];
    p_slave                                  = &(p_state_ptr->slave);

    /* Destroy the semaphore */
    (void)osif_sema_destroy(&(p_slave->sema_blocking_xfer));

    if ((p_slave->xfer_type == LPI2CIF_USING_DMA) && (p_slave->b_listening_on_demand))
    {
        /* Disable LPI2C DMA requests. */
        (void)lpi2c_set_slave_rx_dma(p_base, false);
        (void)lpi2c_set_slave_tx_dma(p_base, false);
    }

    sp_lpi2cif_state_ptr[instance] = NULL;

    /* Disable LPI2C slave */
    lpi2c_set_slave_enable(p_base, false);

    /* Disable i2c interrupt */
    int_disable_irq(*(p_state_ptr->p_irq_id));

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_set_tx_buffer
 * Description   : Provide a buffer for transmitting data.
 *
 * Implements : LPI2C_DRV_SlaveSetTxBuffer_Activity
 *END**************************************************************************/
status_t
lpi2cif_slave_set_tx_buffer(uint32_t instance, const uint8_t *p_tx_buffer, uint32_t buf_size)
{
    lpi2cif_slave_state_t *p_slave;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);
    DEV_ASSERT(p_tx_buffer != NULL);
    DEV_ASSERT(buf_size > 0U);

    p_slave = &(sp_lpi2cif_state_ptr[instance]->slave);

    p_slave->p_tx_buffer = p_tx_buffer;
    p_slave->tx_cntr     = buf_size;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_set_rx_buffer
 * Description   : Provide a buffer for receiving data.
 *
 * Implements : LPI2C_DRV_SlaveSetRxBuffer_Activity
 *END**************************************************************************/
status_t
lpi2cif_slave_set_rx_buffer(uint32_t instance, uint8_t *p_rx_buffer, uint32_t buf_size)
{
    lpi2cif_slave_state_t *p_slave;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);
    DEV_ASSERT(p_rx_buffer != NULL);
    DEV_ASSERT(buf_size > 0U);

    p_slave = &(sp_lpi2cif_state_ptr[instance]->slave);

    p_slave->p_rx_buffer = p_rx_buffer;
    p_slave->rx_cntr     = buf_size;

    return STATUS_SUCCESS;
}

#if defined(REFER_TO_REF_DESIGN)
#else  /* REFER_TO_REF_DESIGN */
/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_release_buffer
 * Description   : Release connection of buffer for transmitting data and receiving data.
 *
 * Implements : LPI2C_DRV_SlaveSetRxBuffer_Activity
 *END**************************************************************************/
status_t
lpi2cif_slave_release_buffer(uint32_t       instance,
                             const uint8_t *p_tx_buffer,
                             uint32_t       tx_buf_size,
                             uint8_t       *p_rx_buffer,
                             uint32_t       rx_buf_size)
{
    lpi2cif_slave_state_t *p_slave;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);
    DEV_ASSERT(p_rx_buffer != NULL);
    DEV_ASSERT(rx_buf_size > 0U);

    p_slave = &(sp_lpi2cif_state_ptr[instance]->slave);

    p_slave->p_rx_buffer = NULL;
    p_slave->rx_cntr     = 0U;
    p_slave->p_tx_buffer = NULL;
    p_slave->tx_cntr     = 0U;

    return STATUS_SUCCESS;
}
#endif /* REFER_TO_REF_DESIGN */

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_send_data
 * Description   : perform a non-blocking send transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_SlaveSendData_Activity
 *END**************************************************************************/
status_t
lpi2cif_slave_send_data(uint32_t instance, const uint8_t *p_tx_buffer, uint32_t xfer_byte_count)
{
    LPI2C_t               *p_base;
    lpi2cif_slave_state_t *p_slave;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);
    DEV_ASSERT(p_tx_buffer != NULL);
    DEV_ASSERT(xfer_byte_count > 0U);

    p_base  = gp_lpi2c_base[instance];
    p_slave = &(sp_lpi2cif_state_ptr[instance]->slave);

    /* If the slave is in listening mode the user should not use this function or the blocking
     * counterpart. */
    DEV_ASSERT(p_slave->b_listening_on_demand == false);

    /* Check if slave is busy */
    if (p_slave->b_xfer_in_progress)
    {
        return STATUS_BUSY;
    }

    p_slave->p_tx_buffer = p_tx_buffer;
    p_slave->tx_cntr     = xfer_byte_count;
    p_slave->status      = STATUS_BUSY;

    if (p_slave->xfer_type == LPI2CIF_USING_DMA)
    {
        /* Activate events */
        lpi2c_set_slave_int(p_base,
                            LPI2C_SLAVE_BIT_ERROR_INT | LPI2C_SLAVE_FIFO_ERROR_INT |
                                LPI2C_SLAVE_STOP_DETECT_INT | LPI2C_SLAVE_REPEATED_START_INT |
                                LPI2C_SLAVE_ADDRESS_VALID_INT,
                            true);

        /* Enable LPI2C slave */
        lpi2c_set_slave_enable(p_base, true);

        p_slave->b_xfer_in_progress = true;

        lpi2cif_slave_start_dma_xfer(instance);
    }
    else
    {
        /* Activate events */
// #if defined(ERRATA_E10792) /**< Mute this line by Arthur 20230327. */
#if 1 /**< For merging SDK to turn on the statements. By Arthur 20230327. */
        lpi2c_set_slave_int(p_base,
                            LPI2C_SLAVE_BIT_ERROR_INT | LPI2C_SLAVE_FIFO_ERROR_INT |
                                LPI2C_SLAVE_STOP_DETECT_INT | LPI2C_SLAVE_REPEATED_START_INT |
                                LPI2C_SLAVE_ADDRESS_VALID_INT,
                            true);

#else
        lpi2c_set_slave_int(p_base,
                            LPI2C_SLAVE_BIT_ERROR_INT | LPI2C_SLAVE_FIFO_ERROR_INT |
                                LPI2C_SLAVE_STOP_DETECT_INT | LPI2C_SLAVE_REPEATED_START_INT |
                                LPI2C_SLAVE_ADDRESS_VALID_INT | LPI2C_SLAVE_TRANSMIT_DATA_INT,
                            true);
#endif

        /* Enable LPI2C slave */
        lpi2c_set_slave_enable(p_base, true);

        p_slave->b_xfer_in_progress = true;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_send_data_blocking
 * Description   : perform a blocking send transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_SlaveSendDataBlocking_Activity
 *END**************************************************************************/
status_t
lpi2cif_slave_send_data_blocking(uint32_t       instance,
                                 const uint8_t *p_tx_buffer,
                                 uint32_t       xfer_byte_count,
                                 uint32_t       timeout)
{
    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);
    DEV_ASSERT(p_tx_buffer != NULL);
    DEV_ASSERT(xfer_byte_count > 0U);

    lpi2cif_slave_state_t *p_slave = &(sp_lpi2cif_state_ptr[instance]->slave);

    /* Check if slave is busy */
    if (p_slave->b_xfer_in_progress)
    {
        return STATUS_BUSY;
    }

    /* mark transfer as blocking */
    p_slave->blocking = true;

    /* Dummy wait to ensure the semaphore is 0, no need to check result */
    (void)osif_sema_wait(&(p_slave->sema_blocking_xfer), 0);

    (void)lpi2cif_slave_send_data(instance, p_tx_buffer, xfer_byte_count);

    /* Wait for transfer to end */
    return lpi2cif_slave_wait_xfer_end(instance, timeout);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_rcv_data
 * Description   : perform a non-blocking receive transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_SlaveReceiveData_Activity
 *END**************************************************************************/
status_t
lpi2cif_slave_rcv_data(uint32_t instance, uint8_t *p_rx_buffer, uint32_t xfer_byte_count)
{
    LPI2C_t               *p_base;
    lpi2cif_slave_state_t *p_slave;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);
    DEV_ASSERT(p_rx_buffer != NULL);
    DEV_ASSERT(xfer_byte_count > 0U);

    p_base  = gp_lpi2c_base[instance];
    p_slave = &(sp_lpi2cif_state_ptr[instance]->slave);

    /* If the slave is in listening mode the user should not use this function or the blocking
     * counterpart. */
    DEV_ASSERT(p_slave->b_listening_on_demand == false);

    /* Check if slave is busy */
    if (p_slave->b_xfer_in_progress)
    {
        return STATUS_BUSY;
    }

    p_slave->p_rx_buffer = p_rx_buffer;
    p_slave->rx_cntr     = xfer_byte_count;
    p_slave->status      = STATUS_BUSY;

    if (p_slave->xfer_type == LPI2CIF_USING_DMA)
    {
        /* Activate events */
        lpi2c_set_slave_int(p_base,
                            LPI2C_SLAVE_BIT_ERROR_INT | LPI2C_SLAVE_FIFO_ERROR_INT |
                                LPI2C_SLAVE_STOP_DETECT_INT | LPI2C_SLAVE_REPEATED_START_INT |
                                LPI2C_SLAVE_ADDRESS_VALID_INT,
                            true);

        /* Enable LPI2C slave */
        lpi2c_set_slave_enable(p_base, true);

        p_slave->b_xfer_in_progress = true;

        lpi2cif_slave_start_dma_xfer(instance);
    }
    else
    {
        p_slave->b_xfer_in_progress = true;

        /* Activate events */
        lpi2c_set_slave_int(p_base,
                            LPI2C_SLAVE_BIT_ERROR_INT | LPI2C_SLAVE_FIFO_ERROR_INT |
                                LPI2C_SLAVE_STOP_DETECT_INT | LPI2C_SLAVE_REPEATED_START_INT |
                                LPI2C_SLAVE_ADDRESS_VALID_INT | LPI2C_SLAVE_RECEIVE_DATA_INT,
                            true);

        /* Enable LPI2C slave */
        lpi2c_set_slave_enable(p_base, true);
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_rcv_data_blocking
 * Description   : perform a blocking receive transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_SlaveReceiveDataBlocking_Activity
 *END**************************************************************************/
status_t
lpi2cif_slave_rcv_data_blocking(uint32_t instance,
                                uint8_t *p_rx_buffer,
                                uint32_t xfer_byte_count,
                                uint32_t timeout)
{
    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);
    DEV_ASSERT(p_rx_buffer != NULL);
    DEV_ASSERT(xfer_byte_count > 0U);

    lpi2cif_slave_state_t *p_slave = &(sp_lpi2cif_state_ptr[instance]->slave);

    /* Check if slave is busy */
    if (p_slave->b_xfer_in_progress)
    {
        return STATUS_BUSY;
    }

    /* mark transfer as blocking */
    p_slave->blocking = true;

    /* Dummy wait to ensure the semaphore is 0, no need to check result */
    (void)osif_sema_wait(&(p_slave->sema_blocking_xfer), 0);

    (void)lpi2cif_slave_rcv_data(instance, p_rx_buffer, xfer_byte_count);

    /* Wait for transfer to end */
    return lpi2cif_slave_wait_xfer_end(instance, timeout);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_get_xfer_status
 * Description   : return the current status of the I2C slave transfer
 *
 * When performing an a-sync (non-blocking) transfer, the user can call this function
 * to ascertain the state of the current transfer. In addition, if the transfer is still
 * in progress, the user can get the number of words that should be receive.
 *
 * Implements : LPI2C_DRV_SlaveGetTransferStatus_Activity
 *END**************************************************************************/
status_t
lpi2cif_slave_get_xfer_status(uint32_t instance, uint32_t *p_remain_byte_count)
{
    const lpi2cif_slave_state_t *p_slave;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);

    p_slave = &(sp_lpi2cif_state_ptr[instance]->slave);

    if ((p_remain_byte_count != NULL) && (p_slave->xfer_type == LPI2CIF_USING_INTERRUPTS))
    {
        if (p_slave->tx_cntr > 0U)
        {
            /* Send data */
            *p_remain_byte_count = p_slave->tx_cntr;
        }
        else if (p_slave->rx_cntr > 0U)
        {
            /* Receive data */
            *p_remain_byte_count = p_slave->rx_cntr;
        }
        else
        {
            *p_remain_byte_count = 0U;
        }
    }

    return p_slave->status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_abort_xfer_data
 * Description   : abort a non-blocking I2C Master transmission or reception
 *
 * Implements : LPI2C_DRV_SlaveAbortTransferData_Activity
 *END**************************************************************************/
status_t
lpi2cif_slave_abort_xfer_data(uint32_t instance)
{
    lpi2cif_slave_state_t *p_slave;
    LPI2C_t               *p_base;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);

    p_base  = gp_lpi2c_base[instance];
    p_slave = &(sp_lpi2cif_state_ptr[instance]->slave);

    if (!p_slave->b_listening_on_demand)
    {
        p_slave->status = STATUS_I2C_ABORTED;
        lpi2cif_slave_end_xfer(p_base, p_slave);
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_get_default_config
 * Description   : Gets the default configuration structure for slave
 *
 * Implements : LPI2C_DRV_SlaveGetDefaultConfig_Activity
 *END**************************************************************************/
void
lpi2cif_slave_get_default_config(lpi2cif_slave_config_t *p_config)
{
    p_config->dev_id_addr           = 32U;
    p_config->b_10bit_dev_id_addr   = false;
    p_config->b_listening_on_demand = true;
    p_config->operating_mode        = LPI2C_STANDARD_MODE;
    p_config->xfer_type             = LPI2CIF_USING_INTERRUPTS;
    p_config->dma_channel           = 0U;
    p_config->callback              = NULL;
    p_config->p_callback_param      = NULL;
}

/*******************************************************************************
 * Static functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_isr
 * Description   : handle non-blocking master operation when I2C interrupt occurs
 *
 *END**************************************************************************/
static void
lpi2cif_master_isr(uint32_t instance)
{
    LPI2C_t                *p_base;
    lpi2cif_master_state_t *p_master;
    uint32_t                status_bm;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);

    p_base   = gp_lpi2c_base[instance];
    p_master = &(sp_lpi2cif_state_ptr[instance]->master);

    status_bm = lpi2c_get_master_status_bm(p_base);
    /* Check which event caused the interrupt */
    if (lpi2c_get_master_xmt_data_req_event(p_base))
    {
        lpi2cif_master_handle_xmt_data_req(p_base, p_master);
    }

    if ((lpi2c_get_master_rcv_data_rdy_event(p_base)))
    {
        if (p_master->b_abort_xfer)
        {
            /* Signal transfer end for blocking transfers */
            if (p_master->blocking == true)
            {
                (void)osif_sema_post(&(p_master->sema_blocking_xfer));
            }

            p_master->status = STATUS_TIMEOUT;

            (void)lpi2cif_master_reinit(instance, p_master);
        }
        else
        {
            lpi2cif_master_handle_rcv_data_rdy(p_base, p_master);
        }
    }

    if (lpi2c_get_master_fifo_error_event(p_base))
    {
        /* FIFO error */
        lpi2c_clear_master_fifo_error_event(p_base);

#if (LPI2C_HAS_HIGH_SPEED_MODE)
        /* High-speed transfers end at STOP condition */
        p_master->b_hs_in_progress = false;
#endif
        p_master->status = STATUS_ERROR;

        /* End transfer: no stop generation (the module will handle that by itself
           if needed), reset FIFOs */
        lpi2cif_master_end_xfer(p_base, p_master, false, true);

        /* Signal transfer end for blocking transfers */
        if (p_master->blocking == true)
        {
            (void)osif_sema_post(&(p_master->sema_blocking_xfer));
        }

        if (p_master->callback != NULL)
        {
            p_master->callback(I2C_MASTER_EVENT_END_TRANSFER, p_master->p_callback_param);
        }
    }

    if (lpi2c_get_master_arbitr_lost_event(p_base))
    {
        /* Arbitration lost */
        lpi2c_clear_master_arbitr_lost_event(p_base);

        /* End transfer: no stop generation (the module will handle that by itself
           if needed), reset FIFOs */
        lpi2cif_master_end_xfer(p_base, p_master, false, true);

        /* Signal transfer end for blocking transfers */
        if (p_master->blocking == true)
        {
            (void)osif_sema_post(&(p_master->sema_blocking_xfer));
        }

        p_master->status = STATUS_I2C_ARBITRATION_LOST;

        if (p_master->callback != NULL)
        {
            p_master->callback(I2C_MASTER_EVENT_END_TRANSFER, p_master->p_callback_param);
        }
    }

    if (lpi2c_get_master_nack_det_event(p_base))
    {
        /* Received NACK */

#if (LPI2C_HAS_ULTRA_FAST_MODE)
        /* Ignore NACK in Ultra Fast mode */
        if (p_master->operating_mode != LPI2C_ULTRAFAST_MODE)
        {
#endif
            /* Signal transfer end for blocking transfers */
            if (p_master->blocking == true)
            {
                (void)osif_sema_post(&(p_master->sema_blocking_xfer));
            }

#if (LPI2C_HAS_HIGH_SPEED_MODE)
            /* High-speed transfers end at STOP condition */
            p_master->b_hs_in_progress = false;
#endif
            p_master->status = STATUS_I2C_RECEIVED_NACK;

            /* End transfer: no stop generation (the module will handle that by itself
               if needed), reset FIFOs */
            lpi2cif_master_end_xfer(p_base, p_master, false, true);

            if (p_master->callback != NULL)
            {
                p_master->callback(I2C_MASTER_EVENT_END_TRANSFER, p_master->p_callback_param);
            }

            /* Clear NACK flag */
            lpi2c_clear_master_nack_det_event(p_base);
#if (LPI2C_HAS_ULTRA_FAST_MODE)
        }
#endif
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_handle_xmt_data_req
 * Description   : handle a transmit request for master
 *
 *END**************************************************************************/
static void
lpi2cif_master_handle_xmt_data_req(LPI2C_t *p_base, lpi2cif_master_state_t *p_master)
{
    /* More data needed for transmission */
    if (!lpi2cif_master_chk_cmd_queue_empty(p_master))
    {
        /* If there are queued commands, send them */
        lpi2cif_master_send_queued_cmd(p_base, p_master);
    }
    else if (p_master->p_tx_buffer != NULL)
    {
        /* A transmission is in progress */
        if (p_master->tx_cntr == 0U)
        {
            /* There is no more data in buffer, the transmission is over */
            lpi2cif_master_end_xfer(p_base, p_master, p_master->b_end_stop, false);

            /* Signal transfer end for blocking transfers */
            if (p_master->blocking == true)
            {
                (void)osif_sema_post(&(p_master->sema_blocking_xfer));
            }

            p_master->status = STATUS_SUCCESS;

            if (p_master->callback != NULL)
            {
                p_master->callback(I2C_MASTER_EVENT_END_TRANSFER, p_master->p_callback_param);
            }
        }
        else
        {
            /* Queue data bytes to fill tx fifo */
            lpi2cif_master_queue_data(p_base, p_master);
        }
    }
    else
    {
        /* No more commands and no transmission in progress - disable tx event */
        lpi2c_set_master_int(p_base, (uint32_t)LPI2C_MASTER_TRANSMIT_DATA_INT, false);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterHandleReceiveDataRequest
 * Description   : handle a receive request for master
 *
 *END**************************************************************************/
static void
lpi2cif_master_handle_rcv_data_rdy(LPI2C_t *p_base, lpi2cif_master_state_t *p_master)
{
    /* Received data ready */
    DEV_ASSERT(p_master->p_rx_buffer != NULL);

    /* Transfer received data to user buffer */
    while ((lpi2c_get_master_rx_fifo_count(p_base) > 0U) && (p_master->rx_cntr > 0U))
    {
        p_master->p_rx_buffer[0U] = lpi2c_get_master_rcvd_data(p_base);
        p_master->p_rx_buffer++;
        p_master->rx_cntr--;
    }
    if (p_master->rx_cntr == 0U)
    {
        /* Done receiving */
        lpi2cif_master_end_xfer(p_base, p_master, p_master->b_end_stop, false);

        /* Signal transfer end for blocking transfers */
        if (p_master->blocking == true)
        {
            (void)osif_sema_post(&(p_master->sema_blocking_xfer));
        }

        p_master->status = STATUS_SUCCESS;

        if (p_master->callback != NULL)
        {
            p_master->callback(I2C_MASTER_EVENT_END_TRANSFER, p_master->p_callback_param);
        }
    }
    else if (p_master->rx_cntr <= lpi2c_get_master_rx_watermark(p_base))
    {
        /* Reduce rx watermark to receive the last few bytes */
        lpi2c_set_master_rx_watermark(p_base, (uint16_t)(p_master->rx_cntr - 1U));
    }
    else
    {
        /* Continue reception */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_reinit
 * Description   : re-initialize the I2C master
 *
 *END**************************************************************************/
static status_t
lpi2cif_master_reinit(uint32_t instance, lpi2cif_master_state_t *p_master)
{
    LPI2C_t                 *p_base;
    lpi2cif_mode_baud_rate_t mode_baud_rate;

    p_base = gp_lpi2c_base[instance];

    /* Re-initialize driver status structure */
    p_master->p_tx_buffer = NULL;
    p_master->tx_cntr     = 0;
    p_master->p_rx_buffer = NULL;
    p_master->rx_cntr     = 0;
    p_master->b_i2c_idle  = true;

    lpi2cif_master_reset_queue(p_master);

    /* Initialize module */
    lpi2c_init(p_base);

    /* Set baud rate */
    mode_baud_rate.baud_rate = p_master->baud_rate;
#if (LPI2C_HAS_HIGH_SPEED_MODE)
    mode_baud_rate.baud_rate_hs = p_master->baud_rate_hs;
#endif
    (void)lpi2cif_master_set_baud_rate(instance, p_master->operating_mode, &mode_baud_rate);

    /* Set slave address */
    lpi2cif_master_set_dev_id_addr(instance, p_master->dev_id_addr, p_master->b_10bit_dev_id_addr);

    /* Enable LPI2C master */
    lpi2c_set_master_enable(p_base, true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_set_operating_mode
 * Description   : sets the operating mode of the I2C master
 *
 *END**************************************************************************/
static void
lpi2cif_master_set_operating_mode(uint32_t instance, lpi2c_mode_t operating_mode)
{
    LPI2C_t                *p_base;
    lpi2cif_master_state_t *p_master;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);

    p_base   = gp_lpi2c_base[instance];
    p_master = &(sp_lpi2cif_state_ptr[instance]->master);

#if (LPI2C_HAS_ULTRA_FAST_MODE)
    if (operating_mode == LPI2C_ULTRAFAST_MODE)
    {
        lpi2c_set_master_pin_config(p_base, LPI2C_CFG_2PIN_OUTPUT_ONLY);
        lpi2c_set_master_nack_config(p_base, LPI2C_NACK_IGNORE);
    }
    else
#endif
    {
        lpi2c_set_master_pin_config(p_base, LPI2C_CFG_2PIN_OPEN_DRAIN);
        lpi2c_set_master_nack_config(p_base, LPI2C_NACK_RECEIVE);
    }

    p_master->operating_mode = operating_mode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_chk_cmd_queue_empty
 * Description   : checks if there are any commands in the master software queue
 *
 *END**************************************************************************/
static inline bool
lpi2cif_master_chk_cmd_queue_empty(const lpi2cif_master_state_t *p_master)
{
    DEV_ASSERT(p_master != NULL);

    return (p_master->cmd_queue.write_idx == p_master->cmd_queue.read_idx);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_reset_queue
 * Description   : resets the master software queue
 *
 *END**************************************************************************/
static inline void
lpi2cif_master_reset_queue(lpi2cif_master_state_t *p_master)
{
    DEV_ASSERT(p_master != NULL);

    p_master->cmd_queue.read_idx  = 0U;
    p_master->cmd_queue.write_idx = 0U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_queue_cmd
 * Description   : queues a command in the hardware FIFO or in the master software queue
 *
 *END**************************************************************************/
static inline void
lpi2cif_master_queue_cmd(LPI2C_t                *p_base,
                         lpi2cif_master_state_t *p_master,
                         lpi2c_master_command_t  cmd,
                         uint8_t                 data)
{
    DEV_ASSERT(p_master != NULL);
    DEV_ASSERT(p_base != NULL);

    uint16_t txFIFOCount = lpi2c_get_master_tx_fifo_count(p_base);
    uint16_t txFIFOSize  = lpi2c_get_master_tx_fifo_size(p_base);

    /* Check if there is room in the hardware FIFO */
    if (txFIFOCount < txFIFOSize)
    {
        lpi2c_set_master_queue_cmd_xmt(p_base, cmd, data);
    }
    else
    {
        /* Hardware FIFO full, use software FIFO */
        DEV_ASSERT(p_master->cmd_queue.write_idx < LPI2C_MASTER_CMD_QUEUE_SIZE);

        p_master->cmd_queue.cmd[p_master->cmd_queue.write_idx]  = cmd;
        p_master->cmd_queue.data[p_master->cmd_queue.write_idx] = data;
        p_master->cmd_queue.write_idx++;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_send_queued_cmd
 * Description   : transfers commands from the master software queue to the hardware FIFO
 *
 *END**************************************************************************/
static inline void
lpi2cif_master_send_queued_cmd(LPI2C_t *p_base, lpi2cif_master_state_t *p_master)
{
    DEV_ASSERT(p_master != NULL);
    DEV_ASSERT(p_base != NULL);

    uint16_t txFIFOCount = lpi2c_get_master_tx_fifo_count(p_base);
    uint16_t txFifoSize  = lpi2c_get_master_tx_fifo_size(p_base);

    while ((!lpi2cif_master_chk_cmd_queue_empty(p_master)) && (txFIFOCount < txFifoSize))
    {
        lpi2c_set_master_queue_cmd_xmt(p_base,
                                       p_master->cmd_queue.cmd[p_master->cmd_queue.read_idx],
                                       p_master->cmd_queue.data[p_master->cmd_queue.read_idx]);
        p_master->cmd_queue.read_idx++;

        txFIFOCount = lpi2c_get_master_tx_fifo_count(p_base);
    }

    if (lpi2cif_master_chk_cmd_queue_empty(p_master))
    {
        /* Reset queue */
        lpi2cif_master_reset_queue(p_master);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_send_address
 * Description   : send start event and slave address
 *                 parameter receive specifies the direction of the transfer
 *
 *END**************************************************************************/
static void
lpi2cif_master_send_address(LPI2C_t *p_base, lpi2cif_master_state_t *p_master, bool receive)
{
    uint8_t                addrByte;
    lpi2c_master_command_t startCommand;

    DEV_ASSERT(p_master != NULL);
    DEV_ASSERT(p_base != NULL);

#if (LPI2C_HAS_HIGH_SPEED_MODE)
    if ((p_master->operating_mode == LPI2C_HIGHSPEED_MODE) && (p_master->b_hs_in_progress == false))
    {
        /* Initiating High-speed mode - send master code first */
        lpi2cif_master_queue_cmd(
            p_base, p_master, LPI2C_MASTER_COMMAND_START_NACK, p_master->master_code);
        p_master->b_hs_in_progress = true;
    }

    if (p_master->b_hs_in_progress == true)
    {
        /* Use high-speed settings after start event in High Speed mode */
        startCommand = LPI2C_MASTER_COMMAND_START_HS;
    }
    else
#endif
    {
        /* Normal START command */
        startCommand = LPI2C_MASTER_COMMAND_START;
    }

    if (p_master->b_10bit_dev_id_addr)
    {
        /* 10-bit addressing */
        /* First address byte: 1111 0XXD, where XX are bits 10 and 9 of address, and D = 0(transmit)
         */
        addrByte = (uint8_t)(0xF0U + ((p_master->dev_id_addr >> 7U) & 0x6U) + (uint8_t)0U);
        lpi2cif_master_queue_cmd(p_base, p_master, startCommand, addrByte);
        /* Second address byte: Remaining 8 bits of address */
        addrByte = (uint8_t)(p_master->dev_id_addr & 0xFFU);
        lpi2cif_master_queue_cmd(p_base, p_master, LPI2C_MASTER_COMMAND_TRANSMIT, addrByte);
        if (receive == true)
        {
            /* Receiving from 10-bit slave - must send repeated start and resend first address byte
             */
            /* First address byte: 1111 0XXD, where XX are bits 10 and 9 of address, and D = 1
             * (receive) */
            addrByte = (uint8_t)(0xF0U + ((p_master->dev_id_addr >> 7U) & 0x6U) + (uint8_t)1U);
            lpi2cif_master_queue_cmd(p_base, p_master, startCommand, addrByte);
        }
    }
    else
    {
        /* 7-bit addressing */
        /* Address byte: slave 7-bit address + D = 0(transmit) or 1 (receive) */
        addrByte = (uint8_t)((p_master->dev_id_addr << 1U) + (uint8_t)receive);
        lpi2cif_master_queue_cmd(p_base, p_master, startCommand, addrByte);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_queue_data
 * Description   : queues transmit data in the LPI2C tx fifo until it is full
 *
 *END**************************************************************************/
static void
lpi2cif_master_queue_data(LPI2C_t *p_base, lpi2cif_master_state_t *p_master)
{
    DEV_ASSERT(p_master != NULL);
    DEV_ASSERT(p_base != NULL);

    uint16_t txFIFOCount = lpi2c_get_master_tx_fifo_count(p_base);
    uint16_t txFifoSize  = lpi2c_get_master_tx_fifo_size(p_base);

    /* Don't queue any data if there are commands in the software queue */
    if (lpi2cif_master_chk_cmd_queue_empty(p_master))
    {
        while ((p_master->tx_cntr > 0U) && (txFIFOCount < txFifoSize))
        {
            lpi2c_set_master_queue_cmd_xmt(
                p_base, LPI2C_MASTER_COMMAND_TRANSMIT, p_master->p_tx_buffer[0U]);
            p_master->p_tx_buffer++;
            p_master->tx_cntr--;

            txFIFOCount = lpi2c_get_master_tx_fifo_count(p_base);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_end_xfer
 * Description   : ends current transmission or reception
 *
 *END**************************************************************************/
static void
lpi2cif_master_end_xfer(LPI2C_t                *p_base,
                        lpi2cif_master_state_t *p_master,
                        bool                    b_end_stop,
                        bool                    resetFIFO)
{
    DEV_ASSERT(p_master != NULL);
    DEV_ASSERT(p_base != NULL);

    /* Disable all events */
    lpi2c_set_master_int(p_base,
                         LPI2C_MASTER_FIFO_ERROR_INT | LPI2C_MASTER_ARBITRATION_LOST_INT |
                             LPI2C_MASTER_NACK_DETECT_INT | LPI2C_MASTER_TRANSMIT_DATA_INT |
                             LPI2C_MASTER_RECEIVE_DATA_INT,
                         false);

    if (p_master->xfer_type == LPI2CIF_USING_DMA)
    {
        /* Stop DMA channel */
        (void)edma_stop_channel(p_master->dma_channel);

        /* Disable LPI2C DMA request. */
        if (p_master->rx_cntr != (uint16_t)0)
        {
            (void)lpi2c_set_master_rx_dma(p_base, false);
        }
        else
        {
            (void)lpi2c_set_master_tx_dma(p_base, false);
        }
    }

    if (resetFIFO == true)
    {
        /* Reset FIFOs if requested */
        lpi2c_reset_master_tx_fifo_cmd(p_base);
        lpi2c_reset_master_rx_fifo_cmd(p_base);
    }

    /* Queue STOP command if requested */
    if (b_end_stop == true)
    {
        lpi2c_set_master_queue_cmd_xmt(p_base, LPI2C_MASTER_COMMAND_STOP, 0U);
#if (LPI2C_HAS_HIGH_SPEED_MODE)
        p_master->b_hs_in_progress = false; /* High-speed transfers end at STOP condition */
#endif
    }

    p_master->p_tx_buffer = NULL;
    p_master->tx_cntr     = 0;
    p_master->p_rx_buffer = NULL;
    p_master->rx_cntr     = 0;
    p_master->b_i2c_idle  = true;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_wait_xfer_end
 * Description   : waits for the end of a blocking transfer
 *
 *END**************************************************************************/
static status_t
lpi2cif_master_wait_xfer_end(uint32_t instance, uint32_t timeout)
{
    LPI2C_t                *p_base;
    status_t                sts_err_code = STATUS_SUCCESS;
    lpi2cif_master_state_t *p_master;
    uint16_t                rxFifoFill = 0;

    p_base   = gp_lpi2c_base[instance];
    p_master = &(sp_lpi2cif_state_ptr[instance]->master);

    /* Wait for transfer to be completed by the IRQ */
    sts_err_code = osif_sema_wait(&(p_master->sema_blocking_xfer), timeout);

    if (sts_err_code == STATUS_TIMEOUT)
    {
        /* If master is sending data transfer is aborted now in case timeout occurred */
        if (p_master->tx_cntr != 0U)
        {
            lpi2cif_master_end_xfer(p_base, p_master, false, true);

            p_master->status = STATUS_TIMEOUT;
        }
        else
        {
            if (p_master->xfer_type == LPI2CIF_USING_DMA)
            {
                /* Stop DMA channel and activate interrupts */
                (void)edma_stop_channel(p_master->dma_channel);
            }

            /* Disable interrupts to check number of bytes in rx fifo */
            lpi2c_set_master_int(p_base, (uint32_t)LPI2C_MASTER_RECEIVE_DATA_INT, false);

            /* Check number of bytes in rx fifo */
            rxFifoFill = lpi2c_get_master_rx_fifo_count(p_base);

            /* In case both rx size and number of bytes in rx fifo is 0, then the transfer ended
             * successfully */
            if ((rxFifoFill == p_master->rx_cntr) && (p_master->rx_cntr == 0U))
            {
                p_master->status = STATUS_SUCCESS;

                /* Blocking transfer is over */
                p_master->blocking = false;

                return p_master->status;
            }

            /* Set watermark to rxFifoFill in case the rx size is grater than the number of bytes in
             * the rx  fifo */
            if (rxFifoFill < p_master->rx_cntr)
            {
                p_master->b_abort_xfer = true;
                lpi2c_set_master_rx_watermark(p_base, rxFifoFill);

                p_master->status = STATUS_TIMEOUT;
            }

            lpi2c_set_master_int(p_base, (uint32_t)LPI2C_MASTER_RECEIVE_DATA_INT, true);

            sts_err_code = osif_sema_wait(&(p_master->sema_blocking_xfer), timeout);
            if (sts_err_code == STATUS_TIMEOUT)
            {
                (void)lpi2cif_master_reinit(instance, p_master);
                p_master->status = STATUS_TIMEOUT;
            }

            p_master->b_abort_xfer = false;
        }
    }

    /* Blocking transfer is over */
    p_master->blocking = false;
    return p_master->status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_start_dma_xfer
 * Description   : starts the DMA transfer for master
 *
 *END**************************************************************************/
static void
lpi2cif_master_start_dma_xfer(uint32_t instance)
{
    LPI2C_t                    *p_base   = gp_lpi2c_base[instance];
    lpi2cif_master_state_t     *p_master = &(sp_lpi2cif_state_ptr[instance]->master);
    lpi2c_dma_transfer_params_t dma_xfer_params;
    bool                        receive = false;

    dma_xfer_params.dma_channel = p_master->dma_channel;
    if (p_master->tx_cntr > 0U)
    {
        /* Configure watermarks for transmit DMA for master */
        uint32_t txBytes = lpi2c_get_master_tx_fifo_size(p_base);
        if (txBytes > p_master->tx_cntr)
        {
            txBytes = p_master->tx_cntr;
        }
        lpi2c_set_master_tx_watermark(p_base, (uint16_t)(txBytes - 1U));

        dma_xfer_params.edma_xfer_type    = EDMA_TRANSFER_MEM2PERIPH;
        dma_xfer_params.i2c_data_reg_addr = (uint32_t)(&(p_base->MTDR));
        dma_xfer_params.p_xfer_buffer     = (uint8_t *)p_master->p_tx_buffer;
        dma_xfer_params.xfer_direction    = LPI2C_TX_REQ;
        dma_xfer_params.xfer_size         = p_master->tx_cntr;
    }
    else
    {
        /* Configure watermarks for receive DMA for master */
        lpi2c_set_master_rx_watermark(p_base, 0U);

        receive = true;

        dma_xfer_params.edma_xfer_type    = EDMA_TRANSFER_PERIPH2MEM;
        dma_xfer_params.i2c_data_reg_addr = (uint32_t)(&(p_base->MRDR));
        dma_xfer_params.p_xfer_buffer     = p_master->p_rx_buffer;
        dma_xfer_params.xfer_direction    = LPI2C_RX_REQ;
        dma_xfer_params.xfer_size         = p_master->rx_cntr;
    }

    (void)lpi2cif_configure_dma_xfer(instance, &dma_xfer_params);

    /* Disable DMA requests for channel when transfer is done */
    edma_disable_request(dma_xfer_params.dma_channel, true);

    /* Call callback function when all the bytes were transfered. */
    (void)edma_install_callback(
        dma_xfer_params.dma_channel, (lpi2cif_master_complete_dma_xfer), (void *)(instance));

    /* Start channel */
    (void)edma_start_channel(dma_xfer_params.dma_channel);

    lpi2cif_master_send_address(p_base, p_master, receive);

    /* Enable transmit/receive DMA requests */
    if (p_master->tx_cntr > (uint32_t)0U)
    {
        (void)lpi2c_set_master_tx_dma(p_base, true);
    }
    else
    {
        lpi2cif_master_queue_cmd(
            p_base, p_master, LPI2C_MASTER_COMMAND_RECEIVE, (uint8_t)(p_master->rx_cntr - 1U));
        (void)lpi2c_set_master_rx_dma(p_base, true);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_master_complete_dma_xfer
 * Description   : Finish up a transfer DMA for master. The main purpose of
 *                 this function is to create a function compatible with DMA
 *                 callback type
 *
 *END**************************************************************************/
static void
lpi2cif_master_complete_dma_xfer(void *parameter, edma_chn_status_t status)
{
    LPI2C_t                *p_base;
    lpi2cif_master_state_t *p_master;

    uint32_t instance = (uint32_t)parameter;

    p_base   = gp_lpi2c_base[instance];
    p_master = &(sp_lpi2cif_state_ptr[instance]->master);

    if ((p_master->tx_cntr > 0U) && (status != EDMA_CHN_ERROR))
    {
        p_master->tx_cntr = 0U;

        lpi2c_set_master_tx_watermark(p_base, 0U);

        /* Disable transmit data DMA requests */
        (void)lpi2c_set_master_tx_dma(p_base, false);

        /* Activate transmit data events */
        lpi2c_set_master_int(p_base, (uint32_t)LPI2C_MASTER_TRANSMIT_DATA_INT, true);
    }
    else
    {
        /* Signal transfer end for blocking transfers */
        lpi2cif_master_end_xfer(p_base, p_master, p_master->b_end_stop, false);

        if (p_master->blocking == true)
        {
            (void)osif_sema_post(&(p_master->sema_blocking_xfer));
        }

        /* Report status error if an error occurred in EDMA channel */
        if (status == EDMA_CHN_ERROR)
        {
            p_master->status = STATUS_ERROR;
        }
        else
        {
            p_master->status = STATUS_SUCCESS;
        }

        if (p_master->callback != NULL)
        {
            p_master->callback(I2C_MASTER_EVENT_END_TRANSFER, p_master->p_callback_param);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_isr
 * Description   : handle non-blocking slave operation when I2C interrupt occurs
 *
 *END**************************************************************************/
static void
lpi2cif_slave_isr(uint32_t instance)
{
    LPI2C_t               *p_base;
    lpi2cif_slave_state_t *p_slave;
    bool                   b_stop_detect         = false;
    bool                   b_repeat_start_detect = false;
#if defined(_I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_)
    bool b_ackstall_stop = false;
#else  /* _I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_ */
#endif /* _I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_ */
    uint32_t status_bm;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);

    p_base  = gp_lpi2c_base[instance];
    p_slave = &(sp_lpi2cif_state_ptr[instance]->slave);

    status_bm = lpi2c_get_slave_status_bm(p_base);
    /* Check for error event */
    if (lpi2c_get_slave_bit_error_event(p_base))
    {
        p_slave->status = STATUS_ERROR;
        lpi2c_clear_slave_bit_error_event(p_base);

// #if defined(ERRATA_E10792) /**< Mute this line by Arthur 20230327. */
#if 1 /**< For merging SDK to turn on the statements. By Arthur 20230327. */
        /* Deactivate interrupts for transmitting data */
        lpi2c_set_slave_int(p_base, (uint32_t)LPI2C_SLAVE_TRANSMIT_DATA_INT, false);
#endif

        lpi2cif_slave_handle_end_xfer(p_slave, p_base);
    }
#if (LPI2C_HAS_ULTRA_FAST_MODE)
    else if (lpi2c_get_slave_fifo_error_event(p_base))
    {
        /* In Ultra-Fast mode clock stretching is disabled, so it is possible to get
        this event if the slave can't keep up */
        p_slave->status = STATUS_I2C_RX_OVERRUN;
        lpi2c_clear_slave_fifo_error_event(p_base);

// #if defined(ERRATA_E10792) /**< Mute this line by Arthur 20230327. */
#if 1 /**< For merging SDK to turn on the statements. By Arthur 20230327. */
        /* Deactivate interrupts for transmitting data */
        lpi2c_set_slave_int(p_base, LPI2C_SLAVE_TRANSMIT_DATA_INT, false);
#endif

        lpi2cif_slave_handle_end_xfer(p_slave, p_base);
    }
#endif
    else
    {
        /* Check for repeated start or stop condition */
        b_stop_detect         = lpi2c_get_slave_stop_det_event(p_base);
        b_repeat_start_detect = lpi2c_get_slave_rept_start_event(p_base);

        if (b_repeat_start_detect)
        {
            p_slave->num_rept_start++;

            if ((p_slave->num_rept_start == 1U) && (p_slave->b_10bit_dev_id_addr))
            {
                b_repeat_start_detect = false;
                lpi2c_clear_slave_rept_start_event(p_base);
            }
        }

        if ((b_stop_detect == true) || (b_repeat_start_detect == true))
        {
            /* Either STOP or repeated START have the same meaning here: the current transfer is
             * over */
            lpi2c_clear_slave_stop_det_event(p_base);
            lpi2c_clear_slave_rept_start_event(p_base);

// #if defined(ERRATA_E10792) /**< Mute this line by Arthur 20230327. */
#if 1 /**< For merging SDK to turn on the statements. By Arthur 20230327. */
            /* Deactivate interrupts for transmitting data */
            lpi2c_set_slave_int(p_base, (uint32_t)LPI2C_SLAVE_TRANSMIT_DATA_INT, false);
#endif

            if (p_slave->status == STATUS_BUSY)
            {
                /* Report success if no error was recorded */
                p_slave->status = STATUS_SUCCESS;
            }

            lpi2cif_slave_handle_end_xfer(p_slave, p_base);
        }
        else
        {
            /* Check which event caused the interrupt */
            if (lpi2c_get_slave_addr_valid_event(p_base))
            {
                lpi2cif_slave_handle_dev_id_addr_valid(instance, p_base, p_slave);
#if defined(_I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_)
                b_ackstall_stop = true;
#else  /* _I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_ */
#endif /* _I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_ */
            }

            if (lpi2c_get_slave_xmt_data_event(p_base))
            {
                if (lpi2c_get_slave_int(p_base, (uint32_t)LPI2C_SLAVE_TRANSMIT_DATA_INT))
                {
                    lpi2cif_slave_handle_xmt_data_req(p_base, p_slave);
                }
            }

#if defined(_I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_)
            if (b_ackstall_stop)
            {
                b_ackstall_stop = false;
                lpi2c_set_slave_xmt_nack(p_base, LPI2C_SLAVE_TRANSMIT_ACK);
                lpi2c_set_slave_ack_stall(p_base, false);
            }
#else  /* _I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_ */
#endif /* _I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_ */

            if (lpi2c_get_slave_rcv_data_event(p_base))
            {
                if (lpi2c_get_slave_int(p_base, (uint32_t)LPI2C_SLAVE_RECEIVE_DATA_INT))
                {
                    lpi2cif_slave_handle_rcv_data_rdy(p_base, p_slave);
                }
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_handle_dev_id_addr_valid
 * Description   : handle an address valid event for slave
 *
 *END**************************************************************************/
static void
lpi2cif_slave_handle_dev_id_addr_valid(uint32_t               instance,
                                       LPI2C_t               *p_base,
                                       lpi2cif_slave_state_t *p_slave)
{
    uint16_t receivedAddr;

    receivedAddr = lpi2c_get_slave_rcvd_addr(p_base);
    if ((receivedAddr & 1U) != (uint16_t)0U)
    {
        /* Request from master to transmit data */
        if ((p_slave->callback != NULL) && p_slave->b_listening_on_demand)
        {
            p_slave->callback(I2C_SLAVE_EVENT_TX_REQ, p_slave->p_callback_param);
        }

// #if defined(ERRATA_E10792) /**< Mute this line by Arthur 20230327. */
#if 1 /**< For merging SDK to turn on the statements. By Arthur 20230327. */
        if (p_slave->xfer_type == LPI2CIF_USING_INTERRUPTS)
        {
            /* Enable interrupt for transmitting data */
            lpi2c_set_slave_int(
                gp_lpi2c_base[instance], (uint32_t)LPI2C_SLAVE_TRANSMIT_DATA_INT, true);
        }
#endif

        p_slave->b_tx_underrun_warn = false;

        if ((p_slave->xfer_type == LPI2CIF_USING_DMA) && p_slave->b_listening_on_demand)
        {
            (void)lpi2cif_slave_start_dma_xfer(instance);
        }
    }
    else
    {
        /* Request from master to receive data */
        if ((p_slave->callback != NULL) && p_slave->b_listening_on_demand)
        {
            p_slave->callback(I2C_SLAVE_EVENT_RX_REQ, p_slave->p_callback_param);
        }

        if ((p_slave->xfer_type == LPI2CIF_USING_DMA) && p_slave->b_listening_on_demand)
        {
            (void)lpi2cif_slave_start_dma_xfer(instance);
        }
    }

    p_slave->status = STATUS_BUSY;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_handle_xmt_data_req
 * Description   : handle a transmit data event for slave
 *
 *END**************************************************************************/
static void
lpi2cif_slave_handle_xmt_data_req(LPI2C_t *p_base, lpi2cif_slave_state_t *p_slave)
{
    if (p_slave->b_tx_underrun_warn == true)
    {
        /* Another Tx event after underflow warning means the dummy char was sent */
        p_slave->status = STATUS_I2C_TX_UNDERRUN;
    }

    if (p_slave->tx_cntr == 0U)
    {
        /* Out of data, call callback to allow user to provide a new buffer */
        if (p_slave->callback != NULL)
        {
            p_slave->callback(I2C_SLAVE_EVENT_TX_EMPTY, p_slave->p_callback_param);
        }
    }

    if (p_slave->tx_cntr == 0U)
    {
        /*
         * Still no data, record tx underflow event and send dummy char.
         * Special case after the last tx byte: the device will ask for more data
         * but the dummy char will not be sent if NACK and then STOP condition are
         * received from master. So only record a "warning" for now.
         */
        p_slave->b_tx_underrun_warn = true;
        lpi2c_set_slave_xmt_data(p_base, (uint8_t)0xFFU);
    }
    else
    {
        lpi2c_set_slave_xmt_data(p_base, p_slave->p_tx_buffer[0U]);
        p_slave->p_tx_buffer++;
        p_slave->tx_cntr--;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_handle_rcv_data_rdy
 * Description   : handle a receive data event for slave
 *
 *END**************************************************************************/
static void
lpi2cif_slave_handle_rcv_data_rdy(const LPI2C_t *p_base, lpi2cif_slave_state_t *p_slave)
{
    if (p_slave->rx_cntr == 0U)
    {
        /* No more room for data, call callback to allow user to provide a new buffer */
        if (p_slave->callback != NULL)
        {
            p_slave->callback(I2C_SLAVE_EVENT_RX_FULL, p_slave->p_callback_param);
        }
    }

    if (p_slave->rx_cntr == 0U)
    {
        /* Still no room for data, record rx overrun event and dummy read data */
        p_slave->status = STATUS_I2C_RX_OVERRUN;
        (void)lpi2c_get_slave_rcvd_data(p_base);
    }
    else
    {
        p_slave->p_rx_buffer[0U] = lpi2c_get_slave_rcvd_data(p_base);
        p_slave->p_rx_buffer++;
        p_slave->rx_cntr--;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_handle_end_xfer
 * Description   : handle slave end transfer operations
 *
 *END**************************************************************************/
static void
lpi2cif_slave_handle_end_xfer(lpi2cif_slave_state_t *p_slave, LPI2C_t *p_base)
{
    /* Check slave state */
    DEV_ASSERT(p_slave != NULL);

    /* Stop DMA channel if slave is transferring data in DMA mode */
    if (p_slave->xfer_type == LPI2CIF_USING_DMA)
    {
        (void)edma_stop_channel(p_slave->dma_channel);
    }

#if defined(_I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_)
    /* Enable clock stretching for ACKSTALL for next listening */
    lpi2c_set_slave_ack_stall(p_base, true);
#else  /* _I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_ */
#endif /* _I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_ */

    if (!p_slave->b_listening_on_demand)
    {

        lpi2cif_slave_end_xfer(p_base, p_slave);

        /* Signal transfer end for blocking transfers */
        if (p_slave->blocking == true)
        {
            (void)osif_sema_post(&(p_slave->sema_blocking_xfer));
        }
    }

    if (p_slave->callback != NULL)
    {
        p_slave->callback(I2C_SLAVE_EVENT_STOP, p_slave->p_callback_param);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_set_operating_mode
 * Description   : sets the operating mode of the I2C slave
 *
 *END**************************************************************************/
static void
lpi2cif_slave_set_operating_mode(uint32_t instance, lpi2c_mode_t operating_mode)
{
    LPI2C_t               *p_base;
    lpi2cif_slave_state_t *p_slave;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(sp_lpi2cif_state_ptr[instance] != NULL);

    p_base  = gp_lpi2c_base[instance];
    p_slave = &(sp_lpi2cif_state_ptr[instance]->slave);

#if (LPI2C_HAS_ULTRA_FAST_MODE)
    if (operating_mode == LPI2C_ULTRAFAST_MODE)
    {
        lpi2c_set_slave_ignore_nack(p_base, LPI2C_SLAVE_NACK_CONTINUE_TRANSFER);
        lpi2c_set_slave_xmt_nack(p_base, LPI2C_SLAVE_TRANSMIT_NACK);
        /* Disable all clock stretching in ultra-fast mode */
        lpi2c_set_slave_ack_stall(p_base, false);
        lpi2c_set_slave_tx_data_stall(p_base, false);
        lpi2c_set_slave_rx_stall(p_base, false);
        lpi2c_set_slave_addr_stall(p_base, false);
    }
    else
#endif
    {
        lpi2c_set_slave_ignore_nack(p_base, LPI2C_SLAVE_NACK_END_TRANSFER);
        lpi2c_set_slave_xmt_nack(p_base, LPI2C_SLAVE_TRANSMIT_ACK);
#if defined(_I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_)
        lpi2c_set_slave_ack_stall(p_base, true);
        lpi2c_set_slave_tx_data_stall(p_base, true);
        lpi2c_set_slave_rx_stall(p_base, true);
        lpi2c_set_slave_addr_stall(p_base, false);
#else  /* _I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_ */
        /* Enable clock stretching except ACKSTALL (we don't need to send ACK/NACK manually) */
        lpi2c_set_slave_ack_stall(p_base, false);
        lpi2c_set_slave_tx_data_stall(p_base, true);
        lpi2c_set_slave_rx_stall(p_base, true);
        lpi2c_set_slave_addr_stall(p_base, true);
#endif /* _I2C_SLAVE_ACKSTALL_WHILE_ADDR_PHASE_ */
    }

#if (LPI2C_HAS_HIGH_SPEED_MODE)
    if (operating_mode == LPI2C_HIGHSPEED_MODE)
    {
        /* Enable detection of the High-speed Mode master code */
        lpi2c_set_slave_hs_mode_det(p_base, true);
    }
    else
#endif
    {
        /* Disable detection of the High-speed Mode master code */
        lpi2c_set_slave_hs_mode_det(p_base, false);
    }

    p_slave->operating_mode = operating_mode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_end_xfer
 * Description   : ends current transmission or reception
 *
 *END**************************************************************************/
static void
lpi2cif_slave_end_xfer(LPI2C_t *p_base, lpi2cif_slave_state_t *p_slave)
{
    DEV_ASSERT(p_slave != NULL);
    DEV_ASSERT(p_base != NULL);

    /* Deactivate events */

    lpi2c_set_slave_int(p_base,
                        LPI2C_SLAVE_BIT_ERROR_INT | LPI2C_SLAVE_FIFO_ERROR_INT |
                            LPI2C_SLAVE_STOP_DETECT_INT | LPI2C_SLAVE_REPEATED_START_INT |
                            LPI2C_SLAVE_ADDRESS_VALID_INT | LPI2C_SLAVE_RECEIVE_DATA_INT |
                            LPI2C_SLAVE_TRANSMIT_DATA_INT,
                        false);

    /* For DMA we must disable the DMA request. */
    if (p_slave->xfer_type == LPI2CIF_USING_DMA)
    {
        if (p_slave->rx_cntr != (uint16_t)0)
        {
            (void)lpi2c_set_slave_rx_dma(p_base, false);
        }
        else
        {
            (void)lpi2c_set_slave_tx_dma(p_base, false);
        }
    }

    /* Disable LPI2C slave */
    lpi2c_set_slave_enable(p_base, false);

    p_slave->b_xfer_in_progress = false;
    p_slave->p_rx_buffer        = NULL;
    p_slave->rx_cntr            = 0U;
    p_slave->p_tx_buffer        = NULL;
    p_slave->tx_cntr            = 0U;
    p_slave->num_rept_start     = 0U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_wait_xfer_end
 * Description   : waits for the end of a blocking transfer
 *
 *END**************************************************************************/
static status_t
lpi2cif_slave_wait_xfer_end(uint32_t instance, uint32_t timeout)
{
    status_t               sts_err_code = STATUS_SUCCESS;
    LPI2C_t               *p_base;
    lpi2cif_slave_state_t *p_slave;

    p_base  = gp_lpi2c_base[instance];
    p_slave = &(sp_lpi2cif_state_ptr[instance]->slave);

    /* Wait for transfer to be completed by the IRQ */
    sts_err_code = osif_sema_wait(&(p_slave->sema_blocking_xfer), timeout);

    if (sts_err_code == STATUS_TIMEOUT)
    {
        lpi2cif_slave_end_xfer(p_base, p_slave);
        p_slave->status = STATUS_TIMEOUT;
    }

    /* Blocking transfer is over */
    p_slave->blocking = false;
    return p_slave->status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_slave_start_dma_xfer
 * Description   : starts the DMA transfer for slave
 *
 *END**************************************************************************/
static void
lpi2cif_slave_start_dma_xfer(uint32_t instance)
{
    LPI2C_t                     *p_base  = gp_lpi2c_base[instance];
    const lpi2cif_slave_state_t *p_slave = &(sp_lpi2cif_state_ptr[instance]->slave);
    lpi2c_dma_transfer_params_t  dma_xfer_params;

    if (p_slave->tx_cntr > (uint32_t)0U)
    {
        dma_xfer_params.dma_channel       = p_slave->dma_channel;
        dma_xfer_params.edma_xfer_type    = EDMA_TRANSFER_MEM2PERIPH;
        dma_xfer_params.i2c_data_reg_addr = (uint32_t)(&(p_base->STDR));
        dma_xfer_params.p_xfer_buffer     = (uint8_t *)p_slave->p_tx_buffer;
        dma_xfer_params.xfer_direction    = LPI2C_TX_REQ;
        dma_xfer_params.xfer_size         = p_slave->tx_cntr;
    }
    else
    {
        dma_xfer_params.dma_channel       = p_slave->dma_channel;
        dma_xfer_params.edma_xfer_type    = EDMA_TRANSFER_PERIPH2MEM;
        dma_xfer_params.i2c_data_reg_addr = (uint32_t)(&(p_base->SRDR));
        dma_xfer_params.p_xfer_buffer     = p_slave->p_rx_buffer;
        dma_xfer_params.xfer_direction    = LPI2C_RX_REQ;
        dma_xfer_params.xfer_size         = p_slave->rx_cntr;
    }

    (void)lpi2cif_configure_dma_xfer(instance, &dma_xfer_params);
    /* Adjustment added to source address at the beginning of TX buffer */
    edma_set_src_last_addr_adjust(dma_xfer_params.dma_channel, -(int32_t)(p_slave->tx_cntr));

    /* Start channel */
    (void)edma_start_channel(dma_xfer_params.dma_channel);

    /* Enable transmit/receive DMA requests */
    if (p_slave->tx_cntr > (uint32_t)0U)
    {
        (void)lpi2c_set_slave_tx_dma(p_base, true);
    }
    else
    {
        (void)lpi2c_set_slave_rx_dma(p_base, true);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lpi2cif_configure_dma_xfer
 * Description   : configures the DMA transfer
 *
 *END**************************************************************************/
static void
lpi2cif_configure_dma_xfer(uint32_t instance, const lpi2c_dma_transfer_params_t *dma_xfer_params)
{
    /* Configure DMA channel */
    if (dma_xfer_params->xfer_direction == LPI2C_TX_REQ)
    {
        (void)edma_set_channel_request_and_trigger(
            dma_xfer_params->dma_channel, s_lpi2cif_dma_src[instance][LPI2C_TX_REQ], false);
        (void)edma_config_multi_block_transfer(dma_xfer_params->dma_channel,
                                               dma_xfer_params->edma_xfer_type,
                                               (uint32_t)dma_xfer_params->p_xfer_buffer,
                                               (uint32_t)dma_xfer_params->i2c_data_reg_addr,
                                               EDMA_TRANSFER_SIZE_1B,
                                               (uint32_t)1U,
                                               (uint32_t)dma_xfer_params->xfer_size,
                                               false);
    }
    else
    {
        (void)edma_set_channel_request_and_trigger(
            dma_xfer_params->dma_channel, s_lpi2cif_dma_src[instance][LPI2C_RX_REQ], false);
        (void)edma_config_multi_block_transfer(dma_xfer_params->dma_channel,
                                               dma_xfer_params->edma_xfer_type,
                                               (uint32_t)dma_xfer_params->i2c_data_reg_addr,
                                               (uint32_t)dma_xfer_params->p_xfer_buffer,
                                               EDMA_TRANSFER_SIZE_1B,
                                               (uint32_t)1U,
                                               (uint32_t)dma_xfer_params->xfer_size,
                                               false);
    }
}

/*** end of file ***/
