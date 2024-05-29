/**
 * @file ftm_driver.c
 * @brief FTM configure and set operations.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#include "ftm_driver.h"
#include "ftm_access.h"
/*******************************************************************************
 * Static variables
 ******************************************************************************/
FTM_t *const g_ftmBase[FTM_INSTANCE_COUNT] = FTM_BASE_PTRS;

/*! @brief Interrupt vectors for the FTM peripheral. */
const IRQn_t g_ftmIrqId[FTM_INSTANCE_COUNT][FEATURE_FTM_CHANNEL_COUNT] = FTM_IRQS;

/*! @brief Interrupt vectors for the FTM peripheral. */
const IRQn_t g_ftmOverflowIrqId[FTM_INSTANCE_COUNT] = FTM_Overflow_IRQS;

/*! @brief Pointer to runtime state structure. */
ftm_state_t *ftmStatePtr[FTM_INSTANCE_COUNT] = {NULL};

/*! @brief  Select external clock pin or clock source for peripheral */
static const clock_names_t g_ftmExtClockSel[FTM_INSTANCE_COUNT] = {
    FTM0_CLK,
    FTM1_CLK,
};

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
static status_t ftm_init_pwm_duty_cycle_chn(uint32_t instance, const ftm_pwm_param_t *param);
static void     ftm_init_pwm_independent_chn(uint32_t instance, const ftm_pwm_param_t *param);
static void     ftm_init_pwm_combined_chn(uint32_t instance, const ftm_pwm_param_t *param);

/*******************************************************************************
 * Global functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_init
 * Description   : Initializes the FTM driver and get the clock frequency value
 * which select one of three possible clock sources for the FTM counter.
 *
 * Implements    : ftm_init_activity
 *END**************************************************************************/
status_t
ftm_init(uint32_t instance, const ftm_user_config_t *info, ftm_state_t *state)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(info != NULL);
    DEV_ASSERT(state != NULL);
    DEV_ASSERT((info->ftmClockSource == FTM_CLOCK_SOURCE_SYSTEMCLK) ||
               (info->ftmClockSource == FTM_CLOCK_SOURCE_FIXEDCLK) ||
               (info->ftmClockSource == FTM_CLOCK_SOURCE_EXTERNALCLK));
    FTM_t   *ftmBase = g_ftmBase[instance];
    status_t status  = STATUS_SUCCESS;
    uint8_t  index   = 0U;

    /* Check if this instance is already initialized */
    if (ftmStatePtr[instance] != NULL)
    {
        status = STATUS_ERROR;
    }
    else
    {
        /* Configure state structure. */
        state->ftmClockSource = info->ftmClockSource;
        state->ftmMode        = FTM_MODE_NOT_INITIALIZED;
        state->ftmPeriod      = 0U;
        ftmStatePtr[instance] = state;

        for (index = 0U; index < FEATURE_FTM_CHANNEL_COUNT; index++)
        {
            state->measurementResults[index]      = 0U;
            state->channelsCallbacksParams[index] = NULL;
            state->channelsCallbacks[index]       = NULL;
            state->enableNotification[index]      = false;
        }

        /* The reset operation doesn't care about write protection. ftm_reset will
         * disable this protection.*/
        ftm_reset(ftmBase);
        ftm_init_module(ftmBase, info->ftmPrescaler);
        /* Get clock name used to configure the FlexTimer module */
        state->ftmSourceClockFrequency = ftm_get_frequency(instance);
        /* Check the functional clock is selected for FTM */
        DEV_ASSERT(state->ftmSourceClockFrequency > 0U);
    }

    if (STATUS_SUCCESS == status)
    {
        /* Check if the mode operation in PWM mode */
        if ((FTM_MODE_EDGE_ALIGNED_PWM == info->ftmMode) ||
            (FTM_MODE_CEN_ALIGNED_PWM == info->ftmMode) ||
            (FTM_MODE_OUTPUT_COMPARE == info->ftmMode) ||
            (FTM_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE == info->ftmMode))
        {
            /* Configure sync for between registers and buffers */
            status = ftm_set_sync(instance, &(info->syncMethod));
        }

        /* Enable the generation of initialization trigger on chip module */
        ftm_set_init_trigger(ftmBase, info->enableInitializationTrigger);
        ftm_set_bdm_mode(ftmBase, info->BDMMode);

        /* Check if enable interrupt in counter mode */
        if (info->isTofIsrEnabled)
        {
            ftm_set_timer_overflow_int(ftmBase, true);
            int_enable_irq(g_ftmOverflowIrqId[instance]);
        }
        else
        {
            ftm_set_timer_overflow_int(ftmBase, false);
            int_disable_irq(g_ftmOverflowIrqId[instance]);
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_init_module
 * Description   : Initializes the FTM. This function will enable the flexTimer module
 * and selects one pre-scale factor for the FTM clock source.
 *END**************************************************************************/
void
ftm_init_module(FTM_t *const ftmBase, ftm_clock_ps_t ftmClockPrescaler)
{
    /* Use FTM mode */
    ftm_enable(ftmBase, true);
    ftm_set_clock_ps(ftmBase, ftmClockPrescaler);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_reset
 * Description   : Resets the FTM registers. All the register use in the driver should be
 * reset to default value of each register.
 *END**************************************************************************/
void
ftm_reset(FTM_t *const ftmBase)
{
    uint8_t chnIndex;

    /* WPDIS is set when WPEN bit is read as a 1 and then 1 is written to WPDIS */
    ((ftmBase)->FMS) &= 0U;
    /* This is the reset value for MODE register. WPDIS bit is set to disable write protection */
    ((ftmBase)->MODE) = 0x00000004U;
    ((ftmBase)->SC) &= 0U;
    ((ftmBase)->CNT)   = 0U;
    ((ftmBase)->MOD)   = 0U;
    ((ftmBase)->CNTIN) = 0U;
    ((ftmBase)->STATUS) &= 0U;
    ((ftmBase)->SYNC)     = 0U;
    ((ftmBase)->OUTINIT)  = 0U;
    ((ftmBase)->OUTMASK)  = 0U;
    ((ftmBase)->COMBINE)  = 0U;
    ((ftmBase)->DEADTIME) = 0U;
    ((ftmBase)->EXTTRIG) &= 0U;
    ((ftmBase)->POL)     = 0U;
    ((ftmBase)->FILTER)  = 0U;
    ((ftmBase)->FLTCTRL) = 0U;
    ((ftmBase)->QDCTRL)  = 0U;
    ((ftmBase)->CONF)    = 0U;
    ((ftmBase)->FLTPOL)  = 0U;
    ((ftmBase)->SYNCONF) = 0U;
    ((ftmBase)->INVCTRL) = 0U;
    ((ftmBase)->SWOCTRL) = 0U;
    ((ftmBase)->PWMLOAD) = 0U;
    ((ftmBase)->HCR)     = 0U;
#if FEATURE_FTM_HAS_SUPPORTED_DITHERING
#if (FTM_INSTANCE_COUNT > 2U)
    if ((ftmBase == FTM1) || (ftmBase == FTM2))
#else
    if (ftmBase == FTM1)
#endif
    {
        ((ftmBase)->MOD_MIRROR) = 0U;
    }
#endif
    /* Set to reset value all CnV and CnSC registers */
    for (chnIndex = 0; chnIndex < FEATURE_FTM_CHANNEL_COUNT; chnIndex++)
    {
        ((ftmBase)->CONTROLS[chnIndex].CnSC) &= 0U;
        ((ftmBase)->CONTROLS[chnIndex].CnV) = 0U;
#if FEATURE_FTM_HAS_SUPPORTED_DITHERING
#if (FTM_INSTANCE_COUNT > 2U)
        if ((ftmBase == FTM1) || (ftmBase == FTM2))
#else
        if (ftmBase == FTM1)
#endif
        {
            ((ftmBase)->CV_MIRROR[chnIndex]) = 0U;
        }
#endif
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_clear_status_flags
 * Description   : This function is used to clear the FTM status flags.
 *
 * Implements : ftm_clear_status_flags_activity
 *END**************************************************************************/
void
ftm_clear_status_flags(uint32_t instance, uint32_t flagMask)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    FTM_t   *ftmBase  = g_ftmBase[instance];
    uint32_t chnlMask = (flagMask & 0x000000FFU);
    uint8_t  channel  = 0U;

    /* Clear the timer overflow flag by writing a 0 to the bit while it is set */
    if ((flagMask & (uint32_t)FTM_TIME_OVER_FLOW_FLAG) != 0x0U)
    {
        ftm_clear_timer_overflow(ftmBase);
    }

    /* Clear fault flag by writing a 0 to the bit while it is set */
    if ((flagMask & (uint32_t)FTM_FAULT_FLAG) != 0x0U)
    {
        ftm_clear_faults_Isr(ftmBase);
    }

    /* Check reload flag by writing a 0 to the bit while it is set */
    if ((flagMask & (uint32_t)FTM_RELOAD_FLAG) != 0x0U)
    {
        ftm_clear_reload_flag(ftmBase);
    }

    /* Clear channel trigger flag */
    if ((flagMask & (uint32_t)FTM_CHANNEL_TRIGGER_FLAG) != 0x0U)
    {
        ftm_clear_chn_trigger_flag(ftmBase);
    }

    /* Clear the channel status flags by writing a 0 to the bit */
    for (channel = 0U; channel < FEATURE_FTM_CHANNEL_COUNT; channel++)
    {
        if ((chnlMask & 0x00000001U) != 0x0U)
        {
            ftm_clear_chn_event_status(ftmBase, channel);
        }
        chnlMask = chnlMask >> 1U;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_get_frequency
 * Description   : Retrieves the frequency of the clock source feeding the FTM counter.
 * Function will return a 0 if no clock source is selected and the FTM counter is disabled.
 * The returned value is clock sources for the FTM counter.
 *
 * Implements    : ftm_get_frequency_activity
 *END**************************************************************************/
uint32_t
ftm_get_frequency(uint32_t instance)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    FTM_t const       *ftmBase    = g_ftmBase[instance];
    status_t           returnCode = STATUS_SUCCESS;
    uint8_t            clkPs;
    uint32_t           frequency = 0U;
    const ftm_state_t *state     = ftmStatePtr[instance];
    DEV_ASSERT(state != NULL);
    clkPs = (uint8_t)(1U << ftm_get_clock_ps(ftmBase));

    switch (state->ftmClockSource)
    {
        case FTM_CLOCK_SOURCE_EXTERNALCLK:
            returnCode = clock_get_freq(g_ftmExtClockSel[instance], &frequency);
            break;
        case FTM_CLOCK_SOURCE_FIXEDCLK:
            /* Get the clock frequency value */
            returnCode = clock_get_freq(SIM_RTCCLK_CLK, &frequency);
            break;
        case FTM_CLOCK_SOURCE_SYSTEMCLK:
            /* Get the clock frequency value */
            returnCode = clock_get_freq(CORE_CLK, &frequency);
            break;
        default:
            /* Nothing to do */
            break;
    }

    /* Checks the functional clock of FTM module */
    (void)returnCode;
    DEV_ASSERT(returnCode == STATUS_SUCCESS);

    return (uint32_t)(frequency / clkPs);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_set_sync
 * Description   : This function configure the synchronization for PWM register
 * (CnV, MOD, CINT, HCR, OUTMASK).If this function is used whit wrong parameters
 * it's possible to generate wrong waveform. Registers synchronization need to
 * be configured for PWM and output compare mode.
 *
 * Implements : ftm_set_sync_activity
 *END**************************************************************************/
status_t
ftm_set_sync(uint32_t instance, const ftm_pwm_sync_t *param)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(param != NULL);
    FTM_t   *ftmBase      = g_ftmBase[instance];
    status_t retStatus    = STATUS_SUCCESS;
    bool     hardwareSync = param->hardwareSync0 || param->hardwareSync1 || param->hardwareSync2;

    /* Software and hardware triggers are not allowed in the same time */
    if ((param->softwareSync && hardwareSync) || (true != (param->softwareSync || hardwareSync)))
    {
        retStatus = STATUS_ERROR;
    }
    else if (param->softwareSync)
    {
        /* Configure sync for OUTMASK register */
        ftm_set_outmask_sw_sync(ftmBase, true);
        /* Configure sync for INVCTRL register */
        ftm_set_invctrl_sw_sync(ftmBase, true);
        /* Configure sync for SWOCTRL register */
        ftm_set_swoctrl_sw_sync(ftmBase, true);
        /* Configure sync for MOD, HCR, CNTIN, and CnV registers */
        ftm_set_mod_cntin_cv_sw_sync(ftmBase, true);
        /* Configure synchronization method (waiting next loading point or now) */
        ftm_set_counter_sw_sync(ftmBase, param->syncPoint);
    }
    else
    {
        /* Configure sync for OUTMASK register */
        ftm_set_outmask_hw_sync(ftmBase, true);
        /* Configure sync for INVCTRL register */
        ftm_set_invctrl_hw_sync(ftmBase, true);
        /* Configure sync for SWOCTRL register */
        ftm_set_swoctrl_hw_sync(ftmBase, true);
        /* Configure sync for MOD, HCR, CNTIN, and CnV registers */
        ftm_set_mod_cntin_cv_hw_sync(ftmBase, true);
        /* Configure synchronization method (waiting next loading point or now) */
        ftm_set_counter_hw_sync(ftmBase, (bool)param->syncPoint);
    }

    if (STATUS_SUCCESS == retStatus)
    {
        /* Enhanced PWM sync is used */
        ftm_set_pwm_sync(ftmBase, true);
        /* Configure trigger source for sync */
        ftm_set_hw_sync_trigger_src(ftmBase, 0U, param->hardwareSync0);
        ftm_set_hw_sync_trigger_src(ftmBase, 1U, param->hardwareSync1);
        ftm_set_hw_sync_trigger_src(ftmBase, 2U, param->hardwareSync2);
        /* Configure loading points */
        ftm_set_max_loading(ftmBase, param->maxLoadingPoint);
        ftm_set_min_loading(ftmBase, param->minLoadingPoint);
        /* Configure sync for OUTMASK register */
        ftm_set_outmask_pwm_sync(ftmBase, (bool)param->maskRegSync);
        /* Configure sync for INVCTRL register */
        ftm_set_invctrl_pwm_sync(ftmBase, param->inverterSync);
        /* Configure sync for SWOCTRL register */
        ftm_set_swoctrl_pwm_sync(ftmBase, param->outRegSync);
        /* Configure sync for MOD, HCR, CNTIN, and CnV registers */
        ftm_set_cntin_pwm_sync(ftmBase, param->initCounterSync);
        /* Configure if FTM clears TRIGj (j=0,1,2) when the hardware trigger j is detected. */
        ftm_set_hw_trigger_sync(ftmBase, param->autoClearTrigger);
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_set_chn_trigger
 * Description   : Enables or disables the generation of the FTM peripheral timer channel trigger.
 * Enables or disables the generation of the FTM peripheral timer channel trigger when the
 * FTM counter is equal to its initial value.
 *END**************************************************************************/
void
ftm_set_chn_trigger(FTM_t *const ftmBase, uint8_t channel, bool enable)
{
    DEV_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);
    uint32_t value = 0U;

    if (channel < 2U)
    {
        value = (uint32_t)FTM_EXTTRIG_CH0TRIG_MASK << (uint32_t)channel;
    }
    else if (channel < 6U)
    {
        value = (uint32_t)FTM_EXTTRIG_CH2TRIG_MASK << ((uint32_t)(channel)-2U);
    }
    else
    {
        value = (uint32_t)FTM_EXTTRIG_CH6TRIG_MASK << ((uint32_t)(channel)-6U);
    }

    if (true == enable)
    {
        ((ftmBase)->EXTTRIG) |= value;
    }
    else
    {
        ((ftmBase)->EXTTRIG) &= ~value;
    }
}

// Counter mode

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_init_counter
 * Description   : Initializes the FTM counter. This function provides access to the
 * FTM counter settings. The counter can be run in Up counting or Up-down counting modes.
 * To run the counter in Free running mode, choose Up counting option and provide
 * 0x0 for the countStartVal and 0xFFFF for countFinalVal. Please call this
 * function only when FTM is used as timer/counter. User must call the FTM_DRV_Deinit
 * and the ftm_init to Re-Initialize the FTM before calling ftm_init_counter
 * for the second time and afterwards.
 *
 * Implements    : ftm_init_counter_activity
 *END**************************************************************************/
status_t
ftm_init_counter(uint32_t instance, const ftm_timer_param_t *timer)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(timer != NULL);
    DEV_ASSERT((FTM_MODE_UP_TIMER == timer->mode) || (FTM_MODE_UP_DOWN_TIMER == timer->mode));
    FTM_t       *ftmBase   = g_ftmBase[instance];
    ftm_state_t *state     = ftmStatePtr[instance];
    status_t     retStatus = STATUS_SUCCESS;
    uint8_t      channel   = 0U;

    if ((NULL != state) && (FTM_MODE_NOT_INITIALIZED == state->ftmMode))
    {
        /* Disable counter clock */
        ftm_set_clock_src(ftmBase, FTM_CLOCK_SOURCE_NONE);
        /* Clear the overflow flag */
        ftm_clear_timer_overflow(ftmBase);
        /* Set counter initial and maximum values */
        ftm_set_counter_init_val(ftmBase, timer->initialValue);
        ftm_set_mod(ftmBase, timer->finalValue);
        /* Disable the quadrature decoder mode */
        ftm_set_quad_decoder(ftmBase, false);
        /* Use FTM as counter, disable all the channels */
        for (channel = 0U; channel < FEATURE_FTM_CHANNEL_COUNT; channel++)
        {
            ftm_set_chn_edge_level(ftmBase, channel, 0U);
        }

        /* Check the FTM counter modes */
        if (FTM_MODE_UP_TIMER == timer->mode)
        {
            ftm_set_cpwms(ftmBase, false);
        }
        else if (FTM_MODE_UP_DOWN_TIMER == timer->mode)
        {
            ftm_set_cpwms(ftmBase, true);
        }
        else
        {
            /* Do nothing */
        }

        state->ftmMode = timer->mode;
    }
    else
    {
        retStatus = STATUS_ERROR;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_counter_start
 * Description   : Starts the FTM counter.
 *
 * Implements    : ftm_counter_start_activity
 *END**************************************************************************/
status_t
ftm_counter_start(uint32_t instance)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    FTM_t *ftmBase = g_ftmBase[instance];

    const ftm_state_t *state = ftmStatePtr[instance];
    /* Check the clock source is available for FTM counter */
    DEV_ASSERT(state->ftmSourceClockFrequency > 0U);
    /* Enable counter clock */
    ftm_set_clock_src(ftmBase, state->ftmClockSource);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_counter_stop
 * Description   : Stops the FTM counter.
 *
 * Implements    : ftm_counter_stop_activity
 *END**************************************************************************/
status_t
ftm_counter_stop(uint32_t instance)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    FTM_t *ftmBase = g_ftmBase[instance];

    /* Stop the FTM counter */
    ftm_set_clock_src(ftmBase, FTM_CLOCK_SOURCE_NONE);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_counter_read
 * Description   : Reads back the current value of the FTM counter.
 *
 * Implements    : ftm_counter_read_activity
 *END**************************************************************************/
uint32_t
ftm_counter_read(uint32_t instance)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    FTM_t const *ftmBase = g_ftmBase[instance];

    return ftm_get_counter(ftmBase);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_mc_get_default_config
 * Description   : This function will get the default configuration values
 * in the structure which is used as a common use-case.
 * Return        : None
 * Implements    : ftm_mc_get_default_config_activity
 *END**************************************************************************/
void
ftm_mc_get_default_config(ftm_timer_param_t *const config)
{
    DEV_ASSERT(config != NULL);

    config->mode         = FTM_MODE_UP_TIMER;
    config->initialValue = 0U;
    config->finalValue   = 65535U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_convert_freq_to_period_ticks
 * Description   : This function converts the input parameters representing
 * frequency in Hz to a period value in ticks needed by the hardware timer.
 *
 * Implements    : ftm_convert_freq_to_period_ticks_activity
 *END**************************************************************************/
uint16_t
ftm_convert_freq_to_period_ticks(uint32_t instance, uint32_t freqencyHz)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(freqencyHz != 0U);
    uint32_t           uFTMhz;
    const ftm_state_t *state = ftmStatePtr[instance];
    uFTMhz                   = state->ftmSourceClockFrequency;

    return (uint16_t)(uFTMhz / freqencyHz);
}

// PWM mode
/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_init_pwm
 * Description   : Configures duty cycle and frequency and starts outputting
 * PWM on specified channels.
 *
 * Implements    : ftm_init_pwm_activity
 *END**************************************************************************/
status_t
ftm_init_pwm(uint32_t instance, const ftm_pwm_param_t *param)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(param != NULL);
    status_t     retVal      = STATUS_SUCCESS;
    uint8_t      fltChannel  = 0U;
    uint8_t      faultChnNum = 0U;
    uint32_t     tempInst    = instance;
    ftm_state_t *state       = ftmStatePtr[instance];
    FTM_t       *ftmBase     = g_ftmBase[instance];

    if ((NULL != state) && (FTM_MODE_NOT_INITIALIZED == state->ftmMode))
    {
        /* Disable counter clock */
        ftm_set_clock_src(ftmBase, FTM_CLOCK_SOURCE_NONE);
        /* Clear the overflow flag */
        ftm_clear_timer_overflow(ftmBase);
        /* Disable write protection */
        ftm_set_write_protect(ftmBase, false);
        /* Configure FTM mode */
        state->ftmMode = param->mode;
        /* Configure independent PWM channels */
        ftm_init_pwm_independent_chn(instance, param);
        /* Configure combined PWM channels */
        ftm_init_pwm_combined_chn(instance, param);
        /* Set enable outputs to be set to Initial/default value */
        ftm_set_init_chn_output(ftmBase, true);
        /* Enable faults (if faults were configured) */
        if ((param->faultConfig)->faultMode != FTM_FAULT_CONTROL_DISABLED)
        {
            /* Configure PWM Output behavior */
            ftm_set_pwm_fault_behavior(
                ftmBase, ((param->faultConfig)->pwmOutputStateOnFault) ? true : false);
            /* Configure fault filter value */
            ftm_set_fault_input_filter_val(ftmBase, ((param->faultConfig)->faultFilterValue));
            /* Check the FTM instances */
            if (tempInst <= 3U)
            {
                faultChnNum = (uint8_t)FTM_FEATURE_FAULT_CHANNELS;
            }
            else
            {
                faultChnNum = (uint8_t)(FTM_FEATURE_FAULT_CHANNELS >> 1U);
            }

            for (fltChannel = 0U; fltChannel < faultChnNum; fltChannel++)
            {
                if (true ==
                    (param->faultConfig)->ftmFaultChannelParam[fltChannel].faultChannelEnabled)
                {
                    /* Enable fault channel */
                    ftm_set_fault_input(ftmBase, fltChannel, true);
                    /* Configure fault filter */
                    ftm_set_fault_input_filter(
                        ftmBase,
                        fltChannel,
                        ((param->faultConfig)->ftmFaultChannelParam[fltChannel].faultFilterEnabled)
                            ? true
                            : false);
                    /* Configure fault outputs */
                    ftm_set_chn_fault_input_polarity(
                        ftmBase,
                        fltChannel,
                        (((param->faultConfig)
                              ->ftmFaultChannelParam[fltChannel]
                              .ftmFaultPinPolarity == FTM_POLARITY_HIGH)
                             ? true
                             : false));
                }
            }

            /* Set fault interrupt */
            if (true == ((param->faultConfig)->pwmFaultInterrupt))
            {
                ftm_set_fault_int(ftmBase, true);
            }

            /* Enable fault control */
            ftm_set_fault_control_mode(ftmBase, (uint32_t)(param->faultConfig)->faultMode);
        }

        /* Configure PWM mode: edge or center aligned */
        ftm_set_cpwms(ftmBase, (param->mode == FTM_MODE_CEN_ALIGNED_PWM) ? true : false);
        /* Calculate frequency of the give FTM hardware module - all channels will run at the same
         * frequency */
        state->ftmPeriod = ftm_convert_freq_to_period_ticks(instance, param->uFrequencyHZ);
        /* Based on reference manual, in PWM mode CNTIN is to be set 0*/
        ftm_set_counter_init_val(ftmBase, 0U);
        /* Write MOD register with the value of the period */
        /* For center aligned mode MOD register should be divided by 2 */
        /* For edge aligned mode period is determined by: MOD-CNTIN+1 */
        if (param->mode == FTM_MODE_CEN_ALIGNED_PWM)
        {
            ftm_set_mod(ftmBase, (uint16_t)(state->ftmPeriod >> 1U));
        }
        else
        {
            ftm_set_mod(ftmBase, (uint16_t)(state->ftmPeriod - 1U));
        }

        /* Update the duty cycle */
        retVal = ftm_init_pwm_duty_cycle_chn(instance, param);

        if (STATUS_SUCCESS == retVal)
        {
            /* Configure dead time for combine mode */
            ftm_set_dead_time_count(ftmBase, param->deadTimeValue);
            ftm_set_dead_time_prescale(ftmBase, param->deadTimePrescaler);
            ftm_enable(ftmBase, true);
            //FTM_DRV_SetPwmSyncMode(ftmBase, true);
            /* Set clock source to start counter */
            ftm_set_clock_src(ftmBase, state->ftmClockSource);
        }
        else
        {
            /* Restore FTM mode if initialization fails */
            state->ftmMode = FTM_MODE_NOT_INITIALIZED;
        }
    }
    else
    {
        retVal = STATUS_ERROR;
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_update_pwm_channel
 * Description   : This function will update the duty cycle of PWM output.
 * - If the type of update in the duty cycle, this function will convert the input parameters
 *representing frequency in Hz to a period value in ticks needed by the hardware timer. Period is
 *calculated depending on the operating mode of the FTM module and is stored in internal state
 *structure. firstEdge and secondEdge can have value between 0 - FTM_MAX_DUTY_CYCLE(0 = 0% duty and
 *FTM_MAX_DUTY_CYCLE = 100% duty). secondEdge value is used only whenFTM module is used in PWM
 *combine mode.
 * - If the type of update in ticks, this function will get value in ticks to the hardware timer.
 * firstEdge and secondEdge variables can have value between 0 and ftmPeriod is stored in the state
 *structure.
 * - in the modified combine mode, the firstEdge parameter is fixed value and only can modify the
 *secondEdge variables which is the initial value in the channel (n+1) match edge when the FTM
 *counter has been ran.
 *
 * Implements    : ftm_update_pwm_channel_activity
 *END**************************************************************************/
status_t
ftm_update_pwm_channel(uint32_t                instance,
                       uint8_t                 channel,
                       ftm_pwm_update_option_t typeOfUpdate,
                       uint16_t                firstEdge,
                       uint16_t                secondEdge,
                       bool                    softwareTrigger)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);
    uint16_t     hwFirstEdge  = 0U;
    uint16_t     hwSecondEdge = 0U;
    uint16_t     ftmPeriod    = 0U;
    uint8_t      chnlPairNum  = (uint8_t)(channel >> 1U);
    FTM_t       *ftmBase      = g_ftmBase[instance];
    ftm_state_t *state        = ftmStatePtr[instance];
    status_t     retStatus    = STATUS_SUCCESS;

    /* Get the newest period in the MOD register */
    ftmPeriod = ftm_get_mod(ftmBase);
    /* Check the mode operation in FTM module */
    if (state->ftmMode == FTM_MODE_CEN_ALIGNED_PWM)
    {
        ftmPeriod = (uint16_t)(ftmPeriod << 1U);
    }
    else if ((state->ftmMode == FTM_MODE_EDGE_ALIGNED_PWM) ||
             (state->ftmMode == FTM_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE))
    {
        ftmPeriod = (uint16_t)(ftmPeriod + 1U);
    }
    else
    {
        retStatus = STATUS_ERROR;
    }

    /* Check the type of update for PWM */
    if (FTM_PWM_UPDATE_IN_DUTY_CYCLE == typeOfUpdate)
    {
        if ((firstEdge <= FTM_MAX_DUTY_CYCLE) && (secondEdge <= FTM_MAX_DUTY_CYCLE))
        {
            /* Calculate DutyCycle based of the previously calculated frequency*/
            /* For greater resolution the DutyCycle values are in the range [0. FTM_MAX_DUTY_CYCLE]
             *  where 0 = 0% or PWM always at Low and FTM_MAX_DUTY_CYCLE = 100% or PWM always HIGH;
             *  a value of 0x4000 is equivalent of 50% DutyCycle. */
            hwFirstEdge  = (uint16_t)((ftmPeriod * firstEdge) >> FTM_DUTY_TO_TICKS_SHIFT);
            hwSecondEdge = (uint16_t)((ftmPeriod * secondEdge) >> FTM_DUTY_TO_TICKS_SHIFT);
            /* adjust DutyCycle if 100% value is to be achieved. */
            if (FTM_MAX_DUTY_CYCLE == firstEdge)
            {
                /* If expected duty is 100% then increase by 1 the value that is to be written
                 *  to Hardware so it will exceed value of period */
                hwFirstEdge = (uint16_t)(hwFirstEdge + 1U);
            }
        }
        else
        {
            retStatus = STATUS_ERROR;
        }
    }
    else
    {
        if ((firstEdge <= ftmPeriod) && (secondEdge <= ftmPeriod))
        {
            hwFirstEdge  = firstEdge;
            hwSecondEdge = secondEdge;
        }
        else
        {
            retStatus = STATUS_ERROR;
        }
    }

    if (STATUS_SUCCESS == retStatus)
    {
        if (true == ftm_get_dual_chn_combine(ftmBase, chnlPairNum))
        {
            if (true == ftm_get_dual_chn_mofcombine(ftmBase, chnlPairNum))
            {
                /* Check the clock source for FTM counter is disabled or not */
                if (ftm_get_clock_src(ftmBase) == 0U)
                {
                    ftm_set_chn_count_val(ftmBase, (uint8_t)(chnlPairNum * 2U), hwFirstEdge);
                }
            }
            else
            {
                ftm_set_chn_count_val(ftmBase, (uint8_t)(chnlPairNum * 2U), hwFirstEdge);
            }

            /* Modify the initial value in the channel (n+1) match edge */
            ftm_set_chn_count_val(ftmBase, (uint8_t)((chnlPairNum * 2U) + 1U), hwSecondEdge);
        }
        else
        {
            /* Channel value is divided by 2 for up down counter mode to keep same duty */
            if (true == ftm_get_cpwms(ftmBase))
            {
                ftm_set_chn_count_val(ftmBase, channel, (uint16_t)(hwFirstEdge >> 1U));
            }
            else
            {
                ftm_set_chn_count_val(ftmBase, channel, hwFirstEdge);
            }
        }

        /* Software trigger is generated to change CnV registers */
        /* Before this please configure sync mechanism to use software trigger */
        ftm_set_sw_trigger(ftmBase, softwareTrigger);

        /* Store the PWM period in the state structure */
        state->ftmPeriod = ftmPeriod;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_init_input_capture
 * Description   : Configures Channel Input Capture for either getting time-stamps on edge detection
 * or on signal measurement . When the edge specified in the captureMode
 * argument occurs on the channel the FTM counter is captured into the CnV register.
 * The user will have to read the CnV register separately to get this value. The filter
 * function is disabled if the filterVal argument passed in is 0. The filter function
 * is available only on channels 0,1,2,3.
 *
 * Implements    : ftm_init_input_capture_activity
 *END**************************************************************************/
status_t
ftm_init_input_capture(uint32_t instance, const ftm_input_param_t *param)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(param != NULL);
    DEV_ASSERT(param->nMaxCountValue > 0U);
    FTM_t       *ftmBase   = g_ftmBase[instance];
    uint8_t      index     = 0U;
    uint8_t      hwChannel = 0U;
    ftm_state_t *state     = ftmStatePtr[instance];
    status_t     retStatus = STATUS_SUCCESS;

    if ((NULL != state) && ((FTM_MODE_NOT_INITIALIZED == state->ftmMode) ||
                            (FTM_MODE_EDGE_ALIGNED_PWM == state->ftmMode) ||
                            (FTM_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE == state->ftmMode)))
    {
        if (state->ftmMode == FTM_MODE_NOT_INITIALIZED)
        {
            ftm_set_clock_src(ftmBase, FTM_CLOCK_SOURCE_NONE);
            ftm_set_counter_init_val(ftmBase, 0U);
            ftm_set_mod(ftmBase, param->nMaxCountValue);
            ftm_set_cpwms(ftmBase, false);
        }
        else
        {
            /* In this case clock source is configured from PWM init. */
            state->ftmMode = FTM_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE;
        }

        /* Get curent MOD value */
        state->ftmModValue = ftm_get_mod(ftmBase);

        for (index = 0U; index < param->nNumChannels; index++)
        {
            hwChannel = param->inputChConfig[index].hwChannelId;

            /* Enable filtering for input channels */
            if (hwChannel < CHAN4_IDX)
            {
                if (true == param->inputChConfig[index].filterEn)
                {
                    ftm_set_chn_input_capture_filter(
                        ftmBase, hwChannel, (uint8_t)param->inputChConfig[index].filterValue);
                }
                else
                {
                    ftm_set_chn_input_capture_filter(ftmBase, hwChannel, 0U);
                }
            }

            if (FTM_EDGE_DETECT == param->inputChConfig[index].inputMode)
            {
                /* Set input capture mode */
                ftm_set_chn_msnba_mode(ftmBase, hwChannel, 0U);
                /* Set the event which will generate the interrupt */
                ftm_set_chn_edge_level(
                    ftmBase, hwChannel, (uint8_t)param->inputChConfig[index].edgeAlignement);
                /* Enable interrupt request for the current channel */
                ftm_enable_chn_int(ftmBase, hwChannel);
                int_enable_irq(FTM0_Ch0_7_IRQn); //(g_ftmIrqId[instance][hwChannel]);
            }
            else
            {
                /* Do nothing */
            }

            if (STATUS_SUCCESS != retStatus)
            {
                break;
            }
        }

        if (STATUS_SUCCESS == retStatus)
        {
            /* Update mode. */
            if (state->ftmMode == FTM_MODE_NOT_INITIALIZED)
            {
                state->ftmMode = FTM_MODE_INPUT_CAPTURE;
                /* Set clock source to start the counter. */
                ftm_set_clock_src(ftmBase, state->ftmClockSource);
            }
        }
        else
        {
            if (state->ftmMode == FTM_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE)
            {
                state->ftmMode = FTM_MODE_EDGE_ALIGNED_PWM;
            }
        }
    }
    else
    {
        retStatus = STATUS_ERROR;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_deinit_input_capture
 * Description   : Disables Channel Input Capture
 *
 * Implements    : ftm_deinit_input_capture_activity
 *END**************************************************************************/
status_t
ftm_deinit_input_capture(uint32_t instance, const ftm_input_param_t *param)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(param != NULL);
    FTM_t       *ftmBase     = g_ftmBase[instance];
    uint8_t      chnlPairNum = 0U;
    uint8_t      index       = 0U;
    uint8_t      hwChannel   = 0U;
    ftm_state_t *state       = ftmStatePtr[instance];
    DEV_ASSERT(state != NULL);

    /* Remain configuration for PWM */
    if (state->ftmMode != FTM_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE)
    {
        /* FTM counter is disabled */
        ftm_set_clock_src(ftmBase, FTM_CLOCK_SOURCE_NONE);
        ftm_set_counter_init_val(ftmBase, 0U);
        ftm_set_mod(ftmBase, 0xFFFFU);
        ftm_set_cpwms(ftmBase, false);
    }

    for (index = 0U; index < param->nNumChannels; index++)
    {
        hwChannel   = param->inputChConfig[index].hwChannelId;
        chnlPairNum = (uint8_t)(hwChannel >> 1U);
        /* Disable filtering for input channels */
        if (hwChannel < CHAN4_IDX)
        {
            ftm_set_chn_input_capture_filter(ftmBase, hwChannel, 0U);
        }

        ftm_set_dual_chn_combine(ftmBase, chnlPairNum, false);
        ftm_set_dual_edge_capture(ftmBase, chnlPairNum, false);
        ftm_set_chn_edge_level(ftmBase, hwChannel, (uint8_t)0U);
        ftm_disable_chn_int(ftmBase, hwChannel);
        ftm_clear_chn_event_flag(ftmBase, hwChannel);

        ftm_set_chn_edge_level(ftmBase, (hwChannel | 1U), (uint8_t)0U);
        ftm_disable_chn_int(ftmBase, (hwChannel | 1U));
        ftm_clear_chn_event_flag(ftmBase, (hwChannel | 1U));
    }

    if (state->ftmMode == FTM_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE)
    {
        state->ftmMode = FTM_MODE_EDGE_ALIGNED_PWM;
    }
    else
    {
        state->ftmMode = FTM_MODE_NOT_INITIALIZED;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_set_chn_input_capture_filter
 * Description   : Sets the FTM peripheral timer channel input capture filter value.
 *END**************************************************************************/
void
ftm_set_chn_input_capture_filter(FTM_t *const ftmBase, uint8_t channel, uint8_t value)
{
    DEV_ASSERT(CHAN4_IDX > channel);

    switch (channel)
    {
        case CHAN0_IDX:
            FTM_RMW_FILTER(ftmBase, FTM_FILTER_CH0FVAL_MASK, FTM_FILTER_CH0FVAL(value));
            break;
        case CHAN1_IDX:
            FTM_RMW_FILTER(ftmBase, FTM_FILTER_CH1FVAL_MASK, FTM_FILTER_CH1FVAL(value));
            break;
        case CHAN2_IDX:
            FTM_RMW_FILTER(ftmBase, FTM_FILTER_CH2FVAL_MASK, FTM_FILTER_CH2FVAL(value));
            break;
        case CHAN3_IDX:
            FTM_RMW_FILTER(ftmBase, FTM_FILTER_CH3FVAL_MASK, FTM_FILTER_CH3FVAL(value));
            break;
        default:
            /* Nothing to do */
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_init_output_compare
 * Description   : Configures the FTM to generate timed pulses
 * When the FTM counter matches the value of compareVal argument (this is
 * written into CnV register), the channel output is changed based on what is specified
 * in the compareMode argument.
 *
 * Implements    : ftm_init_output_compare_activity
 *END**************************************************************************/
status_t
ftm_init_output_compare(uint32_t instance, const ftm_output_cmp_param_t *param)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(param != NULL);
    DEV_ASSERT(param->maxCountValue > 0U);
    FTM_t       *ftmBase     = g_ftmBase[instance];
    uint8_t      index       = 0U;
    uint8_t      hwChannel   = 0U;
    uint8_t      chnlPairNum = 0U;
    ftm_state_t *state       = ftmStatePtr[instance];
    status_t     retStatus   = STATUS_SUCCESS;

    if ((NULL != state) && (FTM_MODE_NOT_INITIALIZED == state->ftmMode))
    {
        ftm_set_clock_src(ftmBase, FTM_CLOCK_SOURCE_NONE);
        ftm_set_cpwms(ftmBase, false);
        /* Clear the overflow flag */
        ftm_clear_timer_overflow(ftmBase);
        ftm_set_counter_init_val(ftmBase, 0U);
        ftm_set_mod(ftmBase, param->maxCountValue);
        ftm_set_counter(ftmBase, 0U);
        ftm_set_quad_decoder(ftmBase, false);
        /* Use FTM as counter, disable all the channels */
        for (index = 0U; index < param->nNumOutputChannels; index++)
        {
            DEV_ASSERT(param->maxCountValue >= param->outputChannelConfig[index].comparedValue);
            hwChannel   = param->outputChannelConfig[index].hwChannelId;
            chnlPairNum = (uint8_t)(hwChannel >> 1U);
            ftm_set_dual_chn_mofcombine(ftmBase, chnlPairNum, false);
            ftm_set_dual_chn_combine(ftmBase, chnlPairNum, false);
            ftm_set_dual_edge_capture(ftmBase, chnlPairNum, false);
            /* Set Channel Output mode */
            ftm_set_chn_edge_level(
                ftmBase, hwChannel, (uint8_t)(param->outputChannelConfig[index].chMode));
            /* Enter counter mode for all configured channels */
            ftm_set_chn_msnba_mode(ftmBase, hwChannel, 1U);
            /* Write initial count value for all channels */
            ftm_set_chn_count_val(
                ftmBase, hwChannel, param->outputChannelConfig[index].comparedValue);
            /* Enable channel output */
            ftm_enable_pwm_chn_outputs(ftmBase, hwChannel);
            /* Enable the generation a trigger on chip module */
            ftm_set_chn_trigger(
                ftmBase, hwChannel, param->outputChannelConfig[index].enableExternalTrigger);
        }

        /* Set software trigger */
        ftm_set_sw_trigger(ftmBase, true);
        state->ftmMode = param->mode;
        /* Set clock source to start the counter */
        ftm_set_clock_src(ftmBase, state->ftmClockSource);
    }
    else
    {
        retStatus = STATUS_ERROR;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_deinit_output_compare
 * Description   : Disables compare match output control and clears FTM timer configuration
 *
 * Implements    : ftm_deinit_output_compare_activity
 *END**************************************************************************/
status_t
ftm_deinit_output_compare(uint32_t instance, const ftm_output_cmp_param_t *param)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(param != NULL);
    FTM_t       *ftmBase   = g_ftmBase[instance];
    uint8_t      index     = 0U;
    uint8_t      hwChannel = 0U;
    ftm_state_t *state     = ftmStatePtr[instance];
    DEV_ASSERT(state != NULL);

    /* Stop the FTM counter */
    ftm_set_clock_src(ftmBase, FTM_CLOCK_SOURCE_NONE);
    /* Clear the overflow flag */
    ftm_clear_timer_overflow(ftmBase);
    ftm_set_counter_init_val(ftmBase, 0U);
    for (index = 0U; index < param->nNumOutputChannels; index++)
    {
        hwChannel = param->outputChannelConfig[index].hwChannelId;
        /* Disable Channel Output mode */
        ftm_set_chn_edge_level(ftmBase, hwChannel, (uint8_t)0U);
        /* Write initial count value for all channels to 0xFFFF */
        ftm_set_chn_count_val(ftmBase, hwChannel, 0U);
        /* Disable channel output */
        ftm_disable_pwm_channel_outputs(ftmBase, hwChannel);
    }

    /* Clear out the registers */
    ftm_set_mod(ftmBase, 0U);
    ftm_set_counter(ftmBase, 0U);
    state->ftmMode = FTM_MODE_NOT_INITIALIZED;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_quad_decode_start
 * Description   : Configures the parameters needed and activates quadrature
 * decode mode.
 *
 * Implements    : ftm_quad_decode_start_activity
 *END**************************************************************************/
status_t
ftm_quad_decode_start(uint32_t instance, const ftm_quad_decode_config_t *config)
{
    DEV_ASSERT((instance == 1U) || (instance == 2U));
    DEV_ASSERT(config != NULL);
    FTM_t       *ftmBase   = g_ftmBase[instance];
    ftm_state_t *state     = ftmStatePtr[instance];
    status_t     retStatus = STATUS_SUCCESS;

    if ((NULL != state) && (FTM_MODE_NOT_INITIALIZED == state->ftmMode))
    {
        /* Disable Quadrature Decoder */
        ftm_set_quad_decoder(ftmBase, false);
        ftm_set_clock_src(ftmBase, FTM_CLOCK_SOURCE_NONE);
        /* Configure Quadrature Decoder */
        /* Set decoder mode Speed and direction or Phase A and Phase B encoding */
        ftm_set_quad_mode(ftmBase, (uint8_t)config->mode);
        /* Set filter state for Phase A (enable/disable) */
        ftm_set_quad_phase_a_filter(ftmBase, config->phaseAConfig.phaseInputFilter);
        /* Set Phase A filter value if phase filter is enabled */
        if (config->phaseAConfig.phaseInputFilter)
        {
            ftm_set_chn_input_capture_filter(
                ftmBase, CHAN0_IDX, config->phaseAConfig.phaseFilterVal);
        }

        /* Set filter state for Phase B (enable/disable) */
        ftm_set_quad_phase_b_filter(ftmBase, config->phaseBConfig.phaseInputFilter);
        /* Set Phase B filter value if phase filter is enabled */
        if (config->phaseBConfig.phaseInputFilter)
        {
            ftm_set_chn_input_capture_filter(
                ftmBase, CHAN1_IDX, config->phaseBConfig.phaseFilterVal);
        }

        /* Set polarity for Phase A and Phase B */
        ftm_set_quad_phase_a_polarity(ftmBase, (uint8_t)config->phaseAConfig.phasePolarity);
        ftm_set_quad_phase_b_polarity(ftmBase, (uint8_t)config->phaseBConfig.phasePolarity);
        /* Configure counter (initial value and maximum value) */
        ftm_set_counter_init_val(ftmBase, config->initialVal);
        ftm_set_mod(ftmBase, config->maxVal);
        ftm_set_counter(ftmBase, config->initialVal);
        /* Enable Quadrature Decoder */
        ftm_set_quad_decoder(ftmBase, true);
        state->ftmMode = FTM_MODE_QUADRATURE_DECODER;
    }
    else
    {
        retStatus = STATUS_ERROR;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_quad_decode_stop
 * Description   : De-activates quadrature decoder mode.
 *
 * Implements    : ftm_quad_decode_stop_activity
 *END**************************************************************************/
status_t
ftm_quad_decode_stop(uint32_t instance)
{
    DEV_ASSERT((instance == 1U) || (instance == 2U));
    FTM_t       *ftmBase = g_ftmBase[instance];
    ftm_state_t *state   = ftmStatePtr[instance];
    DEV_ASSERT(state != NULL);

    /* Disable Quadrature decoder */
    ftm_set_quad_decoder(ftmBase, false);
    state->ftmMode = FTM_MODE_NOT_INITIALIZED;

    return STATUS_SUCCESS;
}

/*******************************************************************************
 * Static functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_init_pwm_duty_cycle_chn
 * Description   : This function will update the duty cycle for the PWM signal
 * at the initialization.
 *
 *END**************************************************************************/
static status_t
ftm_init_pwm_duty_cycle_chn(uint32_t instance, const ftm_pwm_param_t *param)
{
    uint8_t  index     = 0U;
    uint8_t  hwChannel = 0U;
    status_t retVal    = STATUS_SUCCESS;

    for (index = 0U; index < param->nNumIndependentPwmChannels; index++)
    {
        hwChannel = param->pwmIndependentChannelConfig[index].hwChannelId;
        /* Write CnV registers and setup duty cycle and phase values */
        retVal = ftm_update_pwm_channel(instance,
                                        hwChannel,
                                        FTM_PWM_UPDATE_IN_DUTY_CYCLE,
                                        param->pwmIndependentChannelConfig[index].uDutyCyclePercent,
                                        0U,
                                        false);
    }

    for (index = 0U; index < param->nNumCombinedPwmChannels; index++)
    {
        hwChannel = param->pwmCombinedChannelConfig[index].hwChannelId;
        /* Write CnV registers and setup duty cycle and phase values */
        retVal = ftm_update_pwm_channel(instance,
                                        hwChannel,
                                        FTM_PWM_UPDATE_IN_DUTY_CYCLE,
                                        param->pwmCombinedChannelConfig[index].firstEdge,
                                        param->pwmCombinedChannelConfig[index].secondEdge,
                                        false);
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_init_pwm_independent_chn
 * Description   : Configures the PWM signal for the independent channel.
 *
 *END**************************************************************************/
static void
ftm_init_pwm_independent_chn(uint32_t instance, const ftm_pwm_param_t *param)
{
    FTM_t  *ftmBase     = g_ftmBase[instance];
    uint8_t index       = 0U;
    uint8_t channelId   = 0U;
    uint8_t chnlPairNum = 0U;

    /* Configure independent PWM channels */
    for (index = 0U; index < param->nNumIndependentPwmChannels; index++)
    {
        channelId   = param->pwmIndependentChannelConfig[index].hwChannelId;
        chnlPairNum = (uint8_t)(channelId >> 1U);
        /* Configure POL bits for fail safe state */
        ftm_set_chn_output_polarity(
            ftmBase, channelId, (bool)param->pwmIndependentChannelConfig[index].safeState);

        /* Configure polarity of the PWM signal taking into consideration POL and ELSA/ELSB */
        if ((uint32_t)(param->pwmIndependentChannelConfig[index].safeState) ==
            (uint32_t)(param->pwmIndependentChannelConfig[index].polarity))
        {
            ftm_set_chn_edge_level(ftmBase, channelId, (uint8_t)1U);
        }
        else
        {
            ftm_set_chn_edge_level(ftmBase, channelId, (uint8_t)2U);
        }

        if (param->pwmIndependentChannelConfig[index].enableSecondChannelOutput)
        {
            /* Configure dead time, and enable complementary channel. */
            ftm_set_dual_chn_dead_time(
                ftmBase, chnlPairNum, param->pwmIndependentChannelConfig[index].deadTime);
            ftm_set_chn_edge_level(ftmBase, channelId + 1U, (uint8_t)2U);
            ftm_set_dual_chn_comp(ftmBase, chnlPairNum, true);
            if (param->pwmIndependentChannelConfig[index].secondChannelPolarity ==
                FTM_MAIN_INVERTED)
            {
                ftm_set_chn_output_polarity(
                    ftmBase,
                    channelId + 1U,
                    (bool)param->pwmIndependentChannelConfig[index].safeState);
            }
            else
            {
                ftm_set_chn_output_polarity(
                    ftmBase,
                    channelId + 1U,
                    !((bool)param->pwmIndependentChannelConfig[index].safeState));
            }
        }
        else
        {
            ftm_set_dual_chn_comp(ftmBase, chnlPairNum, false);
        }

        /* Disable combined mode. */
        ftm_set_dual_edge_capture(ftmBase, chnlPairNum, false);
        ftm_set_dual_chn_combine(ftmBase, chnlPairNum, false);
        ftm_set_dual_chn_mofcombine(ftmBase, chnlPairNum, false);
        /* Set MSB and MSA bits*/
        ftm_set_chn_msnba_mode(ftmBase, channelId, 3U);
        /* Configure fault mode */
        ftm_set_dual_chn_fault(
            ftmBase,
            chnlPairNum,
            ((param->faultConfig)->faultMode != FTM_FAULT_CONTROL_DISABLED) ? true : false);
        /* Enable sync control for channels*/
        ftm_set_dual_chn_pwm_sync(ftmBase, chnlPairNum, true);
        /* Enable the generation a trigger on chip module */
        ftm_set_chn_trigger(
            ftmBase, channelId, param->pwmIndependentChannelConfig[index].enableExternalTrigger);

        /* Write FTMn_PWMLOAD register to enable synchronized loading points for the given channel
         */
        ftm_enable_pwm_chn_outputs(ftmBase, channelId);
        if (param->pwmIndependentChannelConfig[index].enableSecondChannelOutput)
        {
            ftm_enable_pwm_chn_outputs(ftmBase, channelId + 1U);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ftm_init_pwm_combined_chn
 * Description   : Configures the PWM signal for the combined channel.
 *
 *END**************************************************************************/
static void
ftm_init_pwm_combined_chn(uint32_t instance, const ftm_pwm_param_t *param)
{
    FTM_t  *ftmBase     = g_ftmBase[instance];
    uint8_t index       = 0U;
    uint8_t channelId   = 0U;
    uint8_t chnlPairNum = 0U;

    /* Configure combined PWM channels */
    for (index = 0U; index < param->nNumCombinedPwmChannels; index++)
    {
        channelId   = param->pwmCombinedChannelConfig[index].hwChannelId;
        chnlPairNum = (uint8_t)(channelId >> 1U);
        /* Check if the channel id is even number */
        DEV_ASSERT((channelId % 2U) == 0U);

        /* Configure POL bits for fail safe state */
        ftm_set_chn_output_polarity(
            ftmBase, channelId, (bool)param->pwmCombinedChannelConfig[index].mainChannelSafeState);

        /* Configure polarity of the PWM signal taking into consideration POL and ELSA/ELSB */
        if ((uint32_t)(param->pwmCombinedChannelConfig[index].mainChannelSafeState) ==
            (uint32_t)(param->pwmCombinedChannelConfig[index].mainChannelPolarity))
        {
            ftm_set_chn_edge_level(ftmBase, channelId, (uint8_t)1U);
        }
        else
        {
            ftm_set_chn_edge_level(ftmBase, channelId, (uint8_t)2U);
        }

        ftm_set_dual_edge_capture(ftmBase, chnlPairNum, false);
        /* Set MSB and MSA bits */
        ftm_set_chn_msnba_mode(ftmBase, channelId, 3U);

        /* Enable channel (n) output */
        ftm_enable_pwm_chn_outputs(ftmBase, channelId);
        /* Configure channel n+1 if it necessary. */
        if (param->pwmCombinedChannelConfig[index].enableSecondChannelOutput)
        {
            channelId = channelId + 1U;
            /* Configure POL bits for fail safe state */
            ftm_set_chn_output_polarity(
                ftmBase,
                channelId,
                (bool)param->pwmCombinedChannelConfig[index].secondChannelSafeState);
            ftm_set_chn_edge_level(ftmBase, channelId, (uint8_t)2U);
            /* Configure polarity of the second channel relative to main channel polarity. */
            if (param->pwmCombinedChannelConfig[index].secondChannelSafeState ==
                param->pwmCombinedChannelConfig[index].mainChannelSafeState)
            {
                if (param->pwmCombinedChannelConfig[index].secondChannelPolarity ==
                    FTM_MAIN_DUPLICATED)
                {
                    /* If dead time is enabled and COMPx = 0 the channel n+1 is automatically
                     * disabled. */
                    DEV_ASSERT(!(param->pwmCombinedChannelConfig[index].deadTime));
                    ftm_set_dual_chn_comp(ftmBase, chnlPairNum, false);
                }
                else
                {
                    ftm_set_dual_chn_comp(ftmBase, chnlPairNum, true);
                }
            }
            else
            {
                if (param->pwmCombinedChannelConfig[index].secondChannelPolarity ==
                    FTM_MAIN_DUPLICATED)
                {
                    ftm_set_dual_chn_comp(ftmBase, chnlPairNum, true);
                }
                else
                {
                    /* If dead time is enabled and COMPx = 0 the channel n+1 is automatically
                     * disabled. */
                    DEV_ASSERT(!(param->pwmCombinedChannelConfig[index].deadTime));
                    ftm_set_dual_chn_comp(ftmBase, chnlPairNum, false);
                }
            }
            /* Enable channel (n+1) output */
            ftm_enable_pwm_chn_outputs(ftmBase, (uint8_t)(channelId));
        }

        /* Set fault control for the channel */
        ftm_set_dual_chn_fault(
            ftmBase,
            chnlPairNum,
            ((param->faultConfig)->faultMode != FTM_FAULT_CONTROL_DISABLED) ? true : false);
        /* Enable sync control for channels */
        ftm_set_dual_chn_pwm_sync(ftmBase, chnlPairNum, true);
        /* Enable the combine mode */
        ftm_set_dual_chn_combine(ftmBase, chnlPairNum, true);
        /* Configure the modified combine mode */
        ftm_set_dual_chn_mofcombine(
            ftmBase, chnlPairNum, param->pwmCombinedChannelConfig[index].enableModifiedCombine);
        /* Configure dead time */
        ftm_set_dual_chn_dead_time(
            ftmBase, chnlPairNum, param->pwmCombinedChannelConfig[index].deadTime);
        /* Enable the generation a trigger on the channel (n) */
        channelId = (uint8_t)(chnlPairNum << 1U);
        ftm_set_chn_trigger(
            ftmBase, channelId, param->pwmCombinedChannelConfig[index].enableExternalTrigger);
        /* Enable the generation a trigger on the channel (n+1) */
        channelId = channelId + 1U;
        ftm_set_chn_trigger(ftmBase,
                            channelId,
                            param->pwmCombinedChannelConfig[index].enableExternalTriggerOnNextChn);
    }
}
