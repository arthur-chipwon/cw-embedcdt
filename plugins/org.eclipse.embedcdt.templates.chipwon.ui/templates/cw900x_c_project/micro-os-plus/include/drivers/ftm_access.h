/**
 * @file ftm_access.h
 * @brief FTM driver header file.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef FTM_ACCESS_H
#define FTM_ACCESS_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "interrupt_manager.h"
#include "device_registers.h"
#include "status.h"
#include "callbacks.h"
#include "clock_gating.h"

/*******************************************************************************
 * Static functions
 ******************************************************************************/
/*!
 * @brief Enables the FTM peripheral timer group.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable FTM mode selection
 *                   - true : All registers including FTM-specific registers are available
 *                   - false: Only the TPM-compatible registers are available
 */
static inline void
ftm_enable(FTM_t *const ftmBase, bool enable)
{
    FTM_RMW_MODE(ftmBase, FTM_MODE_FTMEN_MASK, FTM_MODE_FTMEN(enable));
}

/*!
 * @brief Sets the FTM clock divider.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] ps The FTM peripheral clock pre-scale divider
 */
static inline void
ftm_set_clock_ps(FTM_t *const ftmBase, ftm_clock_ps_t ps)
{
    FTM_RMW_SC(ftmBase, FTM_SC_PS_MASK, FTM_SC_PS(ps));
}

/*!
 * @brief Reads the FTM clock divider.
 *
 * @param[in] ftmBase The FTM base address pointer
 *
 * @return The FTM clock pre-scale divider
 */
static inline uint8_t
ftm_get_clock_ps(const FTM_t *ftmBase)
{
    return (uint8_t)((((ftmBase)->SC) & FTM_SC_PS_MASK) >> FTM_SC_PS_SHIFT);
}

/*!
 * @brief Sets sync mode for FTM OUTMASK register when using a software trigger.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of output mask synchronization
 *                   - true : The software trigger activates OUTMASK register sync
 *                   - false: The software trigger does not activate OUTMASK register sync
 */
static inline void
ftm_set_outmask_sw_sync(FTM_t *const ftmBase, bool enable)
{
    ftmBase->SYNCONF = (ftmBase->SYNCONF & ~FTM_SYNCONF_SWOM_MASK) | FTM_SYNCONF_SWOM(enable);
}

/*!
 * @brief Sets sync mode for FTM INVCTRL register when using a software trigger.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of State of inverting control synchronization
 *                   - true : The software trigger activates INVCTRL register sync
 *                   - false: The software trigger does not activate INVCTRL register sync
 */
static inline void
ftm_set_invctrl_sw_sync(FTM_t *const ftmBase, bool enable)
{
    ftmBase->SYNCONF = (ftmBase->SYNCONF & ~FTM_SYNCONF_SWINVC_MASK) | FTM_SYNCONF_SWINVC(enable);
}

/*!
 * @brief Sets sync mode for FTM SWOCTRL register when using a software trigger.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of software output control synchronization
 *                   - true : The software trigger activates SWOCTRL register sync
 *                   - false: The software trigger does not activate SWOCTRL register sync
 */
static inline void
ftm_set_swoctrl_sw_sync(FTM_t *const ftmBase, bool enable)
{
    ftmBase->SYNCONF = (ftmBase->SYNCONF & ~FTM_SYNCONF_SWSOC_MASK) | FTM_SYNCONF_SWSOC(enable);
}

/*!
 * @brief Sets sync mode for FTM MOD, CNTIN and CV registers when using a software trigger.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of registers synchronization
 *                   - true : The software trigger activates FTM MOD, CNTIN and CV registers sync
 *                   - false: The software trigger does not activate FTM MOD, CNTIN and CV registers
 * sync
 */
static inline void
ftm_set_mod_cntin_cv_sw_sync(FTM_t *const ftmBase, bool enable)
{
    ftmBase->SYNCONF = (ftmBase->SYNCONF & ~FTM_SYNCONF_SWWRBUF_MASK) | FTM_SYNCONF_SWWRBUF(enable);
}

/*!
 * @brief Sets sync mode for FTM counter register when using a software trigger.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] update_mode State of FTM counter synchronization
 *                   - true : The software trigger activates FTM counter sync
 *                   - false: The software trigger does not activate FTM counter sync
 */
static inline void
ftm_set_counter_sw_sync(FTM_t *const ftmBase, ftm_pwm_sync_mode_t update_mode)
{
    ftmBase->SYNCONF =
        (ftmBase->SYNCONF & ~FTM_SYNCONF_SWRSTCNT_MASK) | FTM_SYNCONF_SWRSTCNT(update_mode);
}

/*!
 * @brief Sets sync mode for FTM OUTMASK register when using a hardware trigger.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of output mask synchronization
 *                   - true : The hardware trigger activates OUTMASK register sync
 *                   - false: The hardware trigger does not activate OUTMASK register sync
 */
static inline void
ftm_set_outmask_hw_sync(FTM_t *const ftmBase, bool enable)
{
    ftmBase->SYNCONF = (ftmBase->SYNCONF & ~FTM_SYNCONF_HWOM_MASK) | FTM_SYNCONF_HWOM(enable);
}

/*!
 * @brief Sets sync mode for FTM INVCTRL register when using a hardware trigger.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of inverting control synchronization
 *                   - true : The hardware trigger activates INVCTRL register sync
 *                   - false: The hardware trigger does not activate INVCTRL register sync
 */
static inline void
ftm_set_invctrl_hw_sync(FTM_t *const ftmBase, bool enable)
{
    ftmBase->SYNCONF = (ftmBase->SYNCONF & ~FTM_SYNCONF_HWINVC_MASK) | FTM_SYNCONF_HWINVC(enable);
}

/*!
 * @brief Sets the sync mode for the FTM SWOCTRL register when using a hardware trigger.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of software output control synchronization
 *                   - true : The hardware trigger activates SWOCTRL register sync
 *                   - false: The hardware trigger does not activate SWOCTRL register sync
 */
static inline void
ftm_set_swoctrl_hw_sync(FTM_t *const ftmBase, bool enable)
{
    ftmBase->SYNCONF = (ftmBase->SYNCONF & ~FTM_SYNCONF_HWSOC_MASK) | FTM_SYNCONF_HWSOC(enable);
}

/*!
 * @brief Sets sync mode for FTM MOD, CNTIN and CV registers when using a hardware trigger.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of registers synchronization
 *                   - true : The hardware trigger activates  MOD, HCR, CNTIN, and CV registers sync
 *                   - false: The hardware trigger does not activate MOD, HCR, CNTIN, and CV
 * registers sync
 */
static inline void
ftm_set_mod_cntin_cv_hw_sync(FTM_t *const ftmBase, bool enable)
{
    ftmBase->SYNCONF = (ftmBase->SYNCONF & ~FTM_SYNCONF_HWWRBUF_MASK) | FTM_SYNCONF_HWWRBUF(enable);
}

/*!
 * @brief Sets sync mode for FTM counter register when using a hardware trigger.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of FTM counter synchronization
 *                   - true : The hardware trigger activates FTM counter sync
 *                   - false: The hardware trigger does not activate FTM counter sync
 */
static inline void
ftm_set_counter_hw_sync(FTM_t *const ftmBase, bool enable)
{
    ftmBase->SYNCONF =
        (ftmBase->SYNCONF & ~FTM_SYNCONF_HWRSTCNT_MASK) | FTM_SYNCONF_HWRSTCNT(enable);
}

/*!
 * @brief Sets the PWM synchronization mode to enhanced or legacy.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] mode State of synchronization mode
 *                   - true : Enhanced PWM synchronization is selected
 *                   - false: Legacy PWM synchronization is selected
 */
static inline void
ftm_set_pwm_sync(FTM_t *const ftmBase, bool mode)
{
    ftmBase->SYNCONF = (ftmBase->SYNCONF & ~FTM_SYNCONF_SYNCMODE_MASK) | FTM_SYNCONF_SYNCMODE(mode);
}

/*!
 * @brief Sets the FTM hardware synchronization trigger.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] trigger_num Number of trigger
 *                        - 0U: trigger 0
 *                        - 1U: trigger 1
 *                        - 2U: trigger 2
 * @param[in] enable State of trigger
 *                   - true : Enable hardware trigger from field trigger_num for PWM synchronization
 *                   - false: Disable hardware trigger from field trigger_num for PWM
 * synchronization
 */
static inline void
ftm_set_hw_sync_trigger_src(FTM_t *const ftmBase, uint8_t trigger_num, bool enable)
{
    DEV_ASSERT(trigger_num < 3U);

    if (enable)
    {
        ((ftmBase)->SYNC) |= ((uint32_t)(FTM_SYNC_TRIG0_MASK) << trigger_num);
    }
    else
    {
        ((ftmBase)->SYNC) &= ~((uint32_t)(FTM_SYNC_TRIG0_MASK) << trigger_num);
    }
}

/*!
 * @brief Enables or disables the FTM peripheral timer maximum loading points.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable Maximum loading point selection
 *                   - true : To enable maximum loading point
 *                   - false: To disable
 */
static inline void
ftm_set_max_loading(FTM_t *const ftmBase, bool enable)
{
    FTM_RMW_SYNC(ftmBase, FTM_SYNC_CNTMAX_MASK, FTM_SYNC_CNTMAX(enable));
}

/*!
 * @brief Enables or disables the FTM peripheral timer minimum loading points.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable Minimum loading point selection
 *                   - true : To enable minimum loading point
 *                   - false: To disable
 */
static inline void
ftm_set_min_loading(FTM_t *const ftmBase, bool enable)
{
    FTM_RMW_SYNC(ftmBase, FTM_SYNC_CNTMIN_MASK, FTM_SYNC_CNTMIN(enable));
}

/*!
 * @brief Determines when the OUTMASK register is updated with the value of its buffer.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable Output Mask synchronization selection
 *                   - true : OUTMASK register is updated only by PWM synchronization
 *                   - false: OUTMASK register is updated in all rising edges of the system clock
 */
static inline void
ftm_set_outmask_pwm_sync(FTM_t *const ftmBase, bool enable)
{
    FTM_RMW_SYNC(ftmBase, FTM_SYNC_SYNCHOM_MASK, FTM_SYNC_SYNCHOM(enable));
}

/*!
 * @brief Sets the INVCTRL register PWM synchronization mode.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] mode State of register synchronization
 *                   - true : INVCTRL register is updated by PWM sync
 *                   - false: INVCTRL register is updated at all rising edges of system clock
 */
static inline void
ftm_set_invctrl_pwm_sync(FTM_t *const ftmBase, ftm_reg_update_t mode)
{
    ftmBase->SYNCONF = (ftmBase->SYNCONF & ~FTM_SYNCONF_INVC_MASK) | FTM_SYNCONF_INVC(mode);
}

/*!
 * @brief Sets the SWOCTRL register PWM synchronization mode.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] mode State of register synchronization
 *                   - true : SWOCTRL register is updated by PWM sync
 *                   - false: SWOCTRL register is updated at all rising edges of system clock
 */
static inline void
ftm_set_swoctrl_pwm_sync(FTM_t *const ftmBase, ftm_reg_update_t mode)
{
    ftmBase->SYNCONF = (ftmBase->SYNCONF & ~FTM_SYNCONF_SWOC_MASK) | FTM_SYNCONF_SWOC(mode);
}

/*!
 * @brief Sets the CNTIN register PWM synchronization mode.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] mode State of register synchronization
 *                   - true : CNTIN register is updated by PWM sync
 *                   - false: CNTIN register is updated at all rising edges of system clock
 */
static inline void
ftm_set_cntin_pwm_sync(FTM_t *const ftmBase, ftm_reg_update_t mode)
{
    ftmBase->SYNCONF = (ftmBase->SYNCONF & ~FTM_SYNCONF_CNTINC_MASK) | FTM_SYNCONF_CNTINC(mode);
}

/*!
 * @brief Sets hardware trigger mode.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of hardware trigger mode
 *                   - true : FTM does not clear the TRIGx bit when the hardware trigger j is
 * detected
 *                   - false: FTM clears the TRIGx bit when the hardware trigger j is detected
 */
static inline void
ftm_set_hw_trigger_sync(FTM_t *const ftmBase, bool enable)
{
    ftmBase->SYNCONF =
        (ftmBase->SYNCONF & ~FTM_SYNCONF_HWTRIGMODE_MASK) | FTM_SYNCONF_HWTRIGMODE(enable);
}

/*!
 * @brief Enables or disables the generation of the trigger when the FTM counter is equal
 * to the CNTIN register.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of initialization trigger
 *                   - true : To enable
 *                   - false: To disable
 */
static inline void
ftm_set_init_trigger(FTM_t *const ftmBase, bool enable)
{
    ftmBase->EXTTRIG =
        (ftmBase->EXTTRIG & ~FTM_EXTTRIG_INITTRIGEN_MASK) | FTM_EXTTRIG_INITTRIGEN(enable);
}

static inline void
ftm_set_bdm_mode(FTM_t *const ftmBase, ftm_bdm_mode_t val)
{
    FTM_RMW_CONF(ftmBase, FTM_CONF_BDMMODE_MASK, FTM_CONF_BDMMODE(val));
}

/*!
 * @brief  Enables the FTM peripheral timer overflow interrupt.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] state - true : Overflow interrupt enabled
 *                  - false: Overflow interrupt disabled
 */
static inline void
ftm_set_timer_overflow_int(FTM_t *const ftmBase, bool state)
{
    FTM_RMW_SC(ftmBase, FTM_SC_TOIE_MASK, FTM_SC_TOIE(state));
}

/*!
 * @brief Sets the FTM clock source.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] clock The FTM peripheral clock selection
 *            - 00: No clock
 *            - 01: system clock
 *            - 10: fixed clock
 *            - 11: External clock
 */
static inline void
ftm_set_clock_src(FTM_t *const ftmBase, ftm_clock_source_t clock)
{
    FTM_RMW_SC(ftmBase, FTM_SC_CLKS_MASK, FTM_SC_CLKS(clock));
}

/*!
 * @brief Clears the timer overflow interrupt flag.
 *
 * @param[in] ftmBase The FTM base address pointer
 */
static inline void
ftm_clear_timer_overflow(FTM_t *const ftmBase)
{
    FTM_RMW_SC(ftmBase, FTM_SC_TOF_MASK, FTM_SC_TOF(0U));
#ifdef ERRATA_E9005
    /* Read-after-write sequence to guarantee required serialization of memory operations */
    ftmBase->SC;
#endif
}

/*!
 * @brief Clears all fault interrupt flags that are active.
 *
 * @param[in] ftmBase The FTM base address pointer
 */
static inline void
ftm_clear_faults_Isr(FTM_t *const ftmBase)
{
    FTM_RMW_FMS(ftmBase,
                FTM_FMS_FAULTF0_MASK | FTM_FMS_FAULTF_MASK,
                FTM_FMS_FAULTF0(0U) | FTM_FMS_FAULTF(0U));
    FTM_RMW_FMS(ftmBase,
                FTM_FMS_FAULTF1_MASK | FTM_FMS_FAULTF_MASK,
                FTM_FMS_FAULTF1(0U) | FTM_FMS_FAULTF(0U));
    FTM_RMW_FMS(ftmBase,
                FTM_FMS_FAULTF2_MASK | FTM_FMS_FAULTF_MASK,
                FTM_FMS_FAULTF2(0U) | FTM_FMS_FAULTF(0U));
    FTM_RMW_FMS(ftmBase,
                FTM_FMS_FAULTF3_MASK | FTM_FMS_FAULTF_MASK,
                FTM_FMS_FAULTF3(0U) | FTM_FMS_FAULTF(0U));
#ifdef ERRATA_E9005
    /* Read-after-write sequence to guarantee required serialization of memory operations */
    ftmBase->FMS;
#endif
}

/*!
 * @brief Clears the reload flag bit.
 *
 * @param[in] ftmBase The FTM base address pointer
 */
static inline void
ftm_clear_reload_flag(FTM_t *const ftmBase)
{
    FTM_RMW_SC(ftmBase, FTM_SC_RF_MASK, FTM_SC_RF(0U));
#ifdef ERRATA_E9005
    /* Read-after-write sequence to guarantee required serialization of memory operations */
    ftmBase->SC;
#endif
}

/*!
 * @brief Clear the channel trigger flag.
 *
 * @param[in] ftmBase The FTM base address pointer
 */
static inline void
ftm_clear_chn_trigger_flag(FTM_t *const ftmBase)
{
    FTM_RMW_EXTTRIG_REG(ftmBase, FTM_EXTTRIG_TRIGF_MASK, FTM_EXTTRIG_TRIGF(0UL));
}

/*!
 * @brief Clears the FTM peripheral timer all channel event status.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] channel The FTM peripheral channel number
 *
 * Implements : FTM_DRV_ClearChnEventStatus_activity
 */
static inline void
ftm_clear_chn_event_status(FTM_t *const ftmBase, uint8_t channel)
{
    DEV_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);

    ((ftmBase)->STATUS) &= (~(1UL << channel));
#ifdef ERRATA_E9005
    /* Read-after-write sequence to guarantee required serialization of memory operations */
    ftmBase->STATUS;
#endif
}

/*!
 * @brief Sets the FTM peripheral timer counter initial value.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] value initial value to be set
 */
static inline void
ftm_set_counter_init_val(FTM_t *const ftmBase, uint16_t value)
{
    FTM_RMW_CNTIN(ftmBase, FTM_CNTIN_INIT_MASK, FTM_CNTIN_INIT(value));
}

/*!
 * @brief Sets the FTM peripheral timer modulo value.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] value The value to be set to the timer modulo
 */
static inline void
ftm_set_mod(FTM_t *const ftmBase, uint16_t value)
{
    FTM_RMW_MOD(ftmBase, FTM_MOD_MOD_MASK, FTM_MOD_MOD(value));
}

/* Quadrature decoder control */
/*!
 * @brief Enables the channel quadrature decoder.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of channel quadrature decoder
 *                  - true : To enable
 *                  - false: To disable
 */
static inline void
ftm_set_quad_decoder(FTM_t *const ftmBase, bool enable)
{
    if (enable)
    {
        ((ftmBase)->QDCTRL) |= (1UL << FTM_QDCTRL_QUADEN_SHIFT);
    }
    else
    {
        ((ftmBase)->QDCTRL) &= ~(1UL << FTM_QDCTRL_QUADEN_SHIFT);
    }
}

/*!
 * @brief Sets the FTM peripheral timer channel edge level.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] channel The FTM peripheral channel number
 * @param[in] level ELSnB:ELSnA :00, 01, 10, 11
 */
static inline void
ftm_set_chn_edge_level(FTM_t *const ftmBase, uint8_t channel, uint8_t level)
{
    DEV_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);

    /* write ELSA bit */
    FTM_RMW_CnSCV_REG(ftmBase, channel, FTM_CnSC_ELSA_MASK, FTM_CnSC_ELSA((uint32_t)level & 0x01U));

    /* write ELSB bit */
    FTM_RMW_CnSCV_REG(
        ftmBase, channel, FTM_CnSC_ELSB_MASK, FTM_CnSC_ELSB(((uint32_t)level & 0x02U) >> 1U));
}

/*!
 * @brief Sets the FTM count direction bit.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] mode The Center-Aligned PWM selection
 *                 - 1U: Up counting mode
 *                 - 0U: Up down counting mode
 */
static inline void
ftm_set_cpwms(FTM_t *const ftmBase, bool mode)
{
    FTM_RMW_SC(ftmBase, FTM_SC_CPWMS_MASK, FTM_SC_CPWMS(mode));
}

/*!
 * @brief Returns the FTM peripheral current counter value.
 *
 * @param[in] ftmBase The FTM base address pointer
 *
 * @return The current FTM timer counter value
 *
 * Implements : ftm_get_counter_activity
 */
static inline uint16_t
ftm_get_counter(const FTM_t *ftmBase)
{
    return (uint16_t)((((ftmBase)->CNT) & FTM_CNT_COUNT_MASK) >> FTM_CNT_COUNT_SHIFT);
}

/*!
 * @brief Enables or disables the FTM write protection.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable The FTM write protection selection
 *                   - true : Write-protection is enabled
 *                   - false: Write-protection is disabled
 */
static inline void
ftm_set_write_protect(FTM_t *const ftmBase, bool enable)
{
    if (enable)
    {
        ftmBase->FMS = (ftmBase->FMS & ~FTM_FMS_WPEN_MASK) | FTM_FMS_WPEN(1U);
    }
    else
    {
        ftmBase->MODE = (ftmBase->MODE & ~FTM_MODE_WPDIS_MASK) | FTM_MODE_WPDIS(1U);
    }
}

/*!
 * @brief Initializes the channels output.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable Initialize the channels output
 *                   - true : The channels output is initialized according to the state of OUTINIT
 * reg
 *                   - false: No effect
 */
static inline void
ftm_set_init_chn_output(FTM_t *const ftmBase, bool enable)
{
    FTM_RMW_MODE(ftmBase, FTM_MODE_INIT_MASK, FTM_MODE_INIT(enable));
}

/*!
 * @brief Configures the behavior of the PWM outputs when a fault is detected
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of fault output
 *                   - true : Output pins are set tri-state,
 *                   - false: Pins are set to a safe state determined by POL bits
 */
static inline void
ftm_set_pwm_fault_behavior(FTM_t *const ftmBase, bool enable)
{
    if (enable)
    {
        ((ftmBase)->FLTCTRL) |= (1UL << FTM_FLTCTRL_FSTATE_SHIFT);
    }
    else
    {
        ((ftmBase)->FLTCTRL) &= ~(1UL << FTM_FLTCTRL_FSTATE_SHIFT);
    }
}

/*!
 * @brief Sets the fault input filter value.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] value Fault input filter value
 */
static inline void
ftm_set_fault_input_filter_val(FTM_t *const ftmBase, uint32_t value)
{
    FTM_RMW_FLTCTRL(ftmBase, FTM_FLTCTRL_FFVAL_MASK, FTM_FLTCTRL_FFVAL(value));
}

/*!
 * @brief Enables or disables the fault input.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] inputNum fault input to be configured, valid value 0, 1, 2, 3
 * @param[in] enable State of fault input
 *                   - true : To enable fault input
 *                   - false: To disable fault input
 */
static inline void
ftm_set_fault_input(FTM_t *const ftmBase, uint8_t inputNum, bool enable)
{
    if (enable)
    {
        ((ftmBase)->FLTCTRL) |= (1UL << inputNum);
    }
    else
    {
        ((ftmBase)->FLTCTRL) &= ~(1UL << inputNum);
    }
}

/*!
 * @brief Enables or disables the fault input filter.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] inputNum Fault input to be configured, valid value 0, 1, 2, 3
 * @param[in] enable State of fault input filter
 *                   - true : To enable fault input filter
 *                   - false: To disable fault input filter
 */
static inline void
ftm_set_fault_input_filter(FTM_t *const ftmBase, uint8_t inputNum, bool enable)
{
    if (enable)
    {
        ((ftmBase)->FLTCTRL) |= (1UL << (inputNum + FTM_FLTCTRL_FFLTR0EN_SHIFT));
    }
    else
    {
        ((ftmBase)->FLTCTRL) &= ~(1UL << (inputNum + FTM_FLTCTRL_FFLTR0EN_SHIFT));
    }
}

/*!
 * @brief Sets the FTM peripheral timer channel fault input polarity.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] fltChannel The FTM peripheral channel number
 * @param[in] polarity The polarity to be set
 *                - false : The fault input polarity is active high
 *                - true: The fault input polarity is active low
 */
static inline void
ftm_set_chn_fault_input_polarity(FTM_t *const ftmBase, uint8_t fltChannel, bool polarity)
{
    DEV_ASSERT(fltChannel < FTM_FEATURE_FAULT_CHANNELS);

    if (true == polarity)
    {
        ((ftmBase)->FLTPOL) &= ~(1UL << fltChannel);
    }
    else
    {
        ((ftmBase)->FLTPOL) |= (1UL << fltChannel);
    }
}

/*!
 * @brief Enables/disables the FTM peripheral timer fault interrupt.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] state Timer fault interrupt state
 *            - true : Fault control interrupt is enable
 *            - false: Fault control interrupt is disabled
 */
static inline void
ftm_set_fault_int(FTM_t *const ftmBase, bool state)
{
    FTM_RMW_MODE(ftmBase, FTM_MODE_FAULTIE_MASK, FTM_MODE_FAULTIE(state));
}

/*!
 * @brief Defines the FTM fault control mode.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] mode Fault control mode value
 * - FTM_FAULT_CONTROL_DISABLED: Fault control disabled
 * - FTM_FAULT_CONTROL_MAN_EVEN: Fault control enabled for even channel (0, 2, 4, 6) and manual
 * fault clearing.
 * - FTM_FAULT_CONTROL_MAN_ALL : Fault control enabled for all channels and manual fault clearing is
 * enabled.
 * - FTM_FAULT_CONTROL_AUTO_ALL: Fault control enabled for all channels and automatic fault clearing
 * is enabled.
 */
static inline void
ftm_set_fault_control_mode(FTM_t *const ftmBase, uint32_t mode)
{
    FTM_RMW_MODE(ftmBase, FTM_MODE_FAULTM_MASK, FTM_MODE_FAULTM(mode));
}

/*!
 * @brief Sets the FTM deadtime value.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] count The FTM peripheral pre-scaler divider
 *                  - 0U : no counts inserted
 *                  - 1U : 1 count is inserted
 *                  - 2U : 2 count is inserted
 *                  - ... up to a possible 63 counts
 */
static inline void
ftm_set_dead_time_count(FTM_t *const ftmBase, uint8_t count)
{
    DEV_ASSERT(count < 64U);

    FTM_RMW_DEADTIME(ftmBase, FTM_DEADTIME_DTVAL_MASK, FTM_DEADTIME_DTVAL(count));
}

/*!
 * @brief Sets the FTM dead time divider.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] divider The FTM peripheral pre-scaler divider
 *                    - FTM_DEADTIME_DIVID_BY_1 : Divide by 1
 *                    - FTM_DEADTIME_DIVID_BY_4 : Divide by 4
 *                    - FTM_DEADTIME_DIVID_BY_16: Divide by 16
 */
static inline void
ftm_set_dead_time_prescale(FTM_t *const ftmBase, ftm_deadtime_ps_t divider)
{
    FTM_RMW_DEADTIME(ftmBase, FTM_DEADTIME_DTPS_MASK, FTM_DEADTIME_DTPS((uint8_t)divider));
}

/*!
 * @brief Sets the FTM peripheral timer channel output polarity.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] channel The FTM peripheral channel number
 * @param[in] polarity The polarity to be set
 *            - true : The channel polarity is active low
 *            - false  : The channel polarity is active high
 */
static inline void
ftm_set_chn_output_polarity(FTM_t *const ftmBase, uint8_t channel, bool polarity)
{
    DEV_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);

    if (true != polarity)
    {
        ((ftmBase)->POL) &= ~(1UL << channel);
    }
    else
    {
        ((ftmBase)->POL) |= (1UL << channel);
    }
}

/*!
 * @brief Enables or disabled the FTM peripheral timer channel pair deadtime insertion.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] chnlPairNum The FTM peripheral channel pair number
 * @param[in] enable State of channel pair deadtime insertion
 *                   - true : To enable deadtime insertion
 *                   - false: To disable
 */
static inline void
ftm_set_dual_chn_dead_time(FTM_t *const ftmBase, uint8_t chnlPairNum, bool enable)
{
    DEV_ASSERT(chnlPairNum < FEATURE_FTM_CHANNEL_COUNT);

    if (enable)
    {
        ((ftmBase)->COMBINE) |= ((uint32_t)FTM_COMBINE_DTEN0_MASK
                                 << (chnlPairNum * FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH));
    }
    else
    {
        ((ftmBase)->COMBINE) &= ~((uint32_t)FTM_COMBINE_DTEN0_MASK
                                  << (chnlPairNum * FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH));
    }
}

/*!
 * @brief Enables or disables the FTM peripheral timer channel pair output complement mode.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] chnlPairNum The FTM peripheral channel pair number
 * @param[in] polarity State of channel pair output complement mode
 *            - true : The channel (n+1) output is the complement of the channel (n) output
 *            - false: The channel (n+1) output is the same as the channel (n) output
 */
static inline void
ftm_set_dual_chn_comp(FTM_t *const ftmBase, uint8_t chnlPairNum, bool polarity)
{
    DEV_ASSERT(chnlPairNum < (FEATURE_FTM_CHANNEL_COUNT >> 1U));

    if (polarity == true)
    {
        ((ftmBase)->COMBINE) |= (FTM_COMBINE_COMP0_MASK
                                 << ((uint32_t)(chnlPairNum)*FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH));
    }
    else
    {
        ((ftmBase)->COMBINE) &= ~(FTM_COMBINE_COMP0_MASK
                                  << ((uint32_t)(chnlPairNum)*FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH));
    }
}

/*!
 * @brief Enables the FTM peripheral timer dual edge capture mode.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] chnlPairNum The FTM peripheral channel pair number
 * @param[in] enable State of dual edge capture mode
 *                   - true : To enable dual edge capture
 *                   - false: To disable
 */
static inline void
ftm_set_dual_edge_capture(FTM_t *const ftmBase, uint8_t chnlPairNum, bool enable)
{
    DEV_ASSERT(chnlPairNum < (FEATURE_FTM_CHANNEL_COUNT >> 1U));

    if (enable)
    {
        ((ftmBase)->COMBINE) |= ((uint32_t)FTM_COMBINE_DECAPEN0_MASK
                                 << (chnlPairNum * FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH));
    }
    else
    {
        ((ftmBase)->COMBINE) &= ~((uint32_t)FTM_COMBINE_DECAPEN0_MASK
                                  << (chnlPairNum * FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH));
    }
}

/*!
 * @brief Enables or disables the FTM peripheral timer channel pair output combine mode.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] chnlPairNum The FTM peripheral channel pair number
 * @param[in] enable State of channel pair output combine mode
 *                   - true : Channels pair are combined
 *                   - false: Channels pair are independent
 */
static inline void
ftm_set_dual_chn_combine(FTM_t *const ftmBase, uint8_t chnlPairNum, bool enable)
{
    DEV_ASSERT(chnlPairNum < (FEATURE_FTM_CHANNEL_COUNT >> 1U));

    if (enable)
    {
        ((ftmBase)->COMBINE) |= (FTM_COMBINE_COMBINE0_MASK
                                 << ((uint32_t)(chnlPairNum)*FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH));
    }
    else
    {
        ((ftmBase)->COMBINE) &= ~(FTM_COMBINE_COMBINE0_MASK
                                  << ((uint32_t)(chnlPairNum)*FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH));
    }
}

/*!
 * @brief Enables the FTM peripheral timer channel modified combine mode.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] chnlPairNum The FTM peripheral channel pair number
 * @param[in] enable State of channel pair outputs modified combine
 *                   - true : To enable modified combine
 *                   - false: To disable modified combine
 */
static inline void
ftm_set_dual_chn_mofcombine(FTM_t *const ftmBase, uint8_t chnlPairNum, bool enable)
{
    DEV_ASSERT(chnlPairNum < (FEATURE_FTM_CHANNEL_COUNT >> 1U));

    if (enable)
    {
        ((ftmBase)->COMBINE) |= ((uint32_t)FTM_COMBINE_MCOMBINE0_MASK
                                 << (chnlPairNum * FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH));
    }
    else
    {
        ((ftmBase)->COMBINE) &= ~((uint32_t)FTM_COMBINE_MCOMBINE0_MASK
                                  << (chnlPairNum * FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH));
    }
}

/*!
 * @brief Sets the FTM peripheral timer channel mode.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] channel The FTM peripheral channel number
 * @param[in] selection The mode to be set valid value MSnB:MSnA :00, 01, 10, 11
 */
static inline void
ftm_set_chn_msnba_mode(FTM_t *const ftmBase, uint8_t channel, uint8_t selection)
{
    DEV_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);

    /* write MSA bit */
    FTM_RMW_CnSCV_REG(
        ftmBase, channel, FTM_CnSC_MSA_MASK, FTM_CnSC_MSA((uint32_t)selection & 0x01U));

    /* write MSB bit */
    FTM_RMW_CnSCV_REG(
        ftmBase, channel, FTM_CnSC_MSB_MASK, FTM_CnSC_MSB(((uint32_t)selection & 0x02U) >> 1U));
}

/*!
 * @brief Enables the FTM peripheral timer channel pair fault control.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] chnlPairNum The FTM peripheral channel pair number
 * @param[in] enable State of channel pair fault control
 *                   - true : To enable fault control
 *                   - false: To disable
 */
static inline void
ftm_set_dual_chn_fault(FTM_t *const ftmBase, uint8_t chnlPairNum, bool enable)
{
    DEV_ASSERT(chnlPairNum < (FEATURE_FTM_CHANNEL_COUNT >> 1U));

    if (enable)
    {
        ((ftmBase)->COMBINE) |= ((uint32_t)FTM_COMBINE_FAULTEN0_MASK
                                 << (chnlPairNum * FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH));
    }
    else
    {
        ((ftmBase)->COMBINE) &= ~((uint32_t)FTM_COMBINE_FAULTEN0_MASK
                                  << (chnlPairNum * FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH));
    }
}

/*!
 * @brief Enables or disables the FTM peripheral timer channel pair counter PWM sync.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] chnlPairNum The FTM peripheral channel pair number
 * @param[in] enable State of channel pair counter PWM sync
 *                   - true : To enable PWM synchronization
 *                   - false: To disable
 */
static inline void
ftm_set_dual_chn_pwm_sync(FTM_t *const ftmBase, uint8_t chnlPairNum, bool enable)
{
    DEV_ASSERT(chnlPairNum < (FEATURE_FTM_CHANNEL_COUNT >> 1U));

    if (enable)
    {
        ((ftmBase)->COMBINE) |= ((uint32_t)FTM_COMBINE_SYNCEN0_MASK
                                 << (chnlPairNum * FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH));
    }
    else
    {
        ((ftmBase)->COMBINE) &= ~((uint32_t)FTM_COMBINE_SYNCEN0_MASK
                                  << (chnlPairNum * FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH));
    }
}

/*!
 * @brief Enable PWM channel Outputs.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] channel The FTM channel

 */
static inline void
ftm_enable_pwm_chn_outputs(FTM_t *const ftmBase, uint8_t channel)
{
    FTM_RMW_SC(ftmBase,
               (1UL << (channel + FTM_FEATURE_OUTPUT_CHANNEL_OFFSET)),
               (1UL << (channel + FTM_FEATURE_OUTPUT_CHANNEL_OFFSET)));
}

/*!
 * @brief Returns the FTM peripheral counter modulo value.
 *
 * @param[in] ftmBase The FTM base address pointer
 *
 * @return FTM timer modulo value
 *
 * Implements : ftm_get_mod_activity
 */
static inline uint16_t
ftm_get_mod(const FTM_t *ftmBase)
{
    return (uint16_t)((((ftmBase)->MOD) & FTM_MOD_MOD_MASK) >> FTM_MOD_MOD_SHIFT);
}

/*!
 * @brief Verify if an channels pair is used in combine mode or not.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] chnlPairNum The FTM peripheral channel pair number
 *
 * @return Channel pair output combine mode status
 *         - true : Channels pair are combined
 *         - false: Channels pair are independent
 */
static inline bool
ftm_get_dual_chn_combine(const FTM_t *ftmBase, uint8_t chnlPairNum)
{
    DEV_ASSERT(chnlPairNum < (FEATURE_FTM_CHANNEL_COUNT >> 1U));

    return (((ftmBase)->COMBINE) &
            (FTM_COMBINE_COMBINE0_MASK
             << ((uint32_t)(chnlPairNum)*FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH))) != 0U;
}

/*!
 * @brief Verify if an channels pair is used in the modified combine mode or not.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] chnlPairNum The FTM peripheral channel pair number
 *
 * @return Channel pair output combine mode status
 *         - true : Channels pair are combined
 *         - false: Channels pair are independent
 */
static inline bool
ftm_get_dual_chn_mofcombine(const FTM_t *ftmBase, uint8_t chnlPairNum)
{
    DEV_ASSERT(chnlPairNum < (FEATURE_FTM_CHANNEL_COUNT >> 1U));

    return (((ftmBase)->COMBINE) &
            (FTM_COMBINE_MCOMBINE0_MASK
             << ((uint32_t)(chnlPairNum)*FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH))) != 0U;
}

/*!
 * @brief Reads the FTM clock source.
 *
 * @param[in] ftmBase The FTM base address pointer
 *
 * @return The FTM clock source selection
 *          - 00: No clock
 *          - 01: system clock
 *          - 10: fixed clock
 *          - 11: External clock
 */
static inline uint8_t
ftm_get_clock_src(const FTM_t *ftmBase)
{
    return (uint8_t)((((ftmBase)->SC) & FTM_SC_CLKS_MASK) >> FTM_SC_CLKS_SHIFT);
}

/*!
 * @brief Sets the FTM peripheral current counter value.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] value The FTM timer counter value to be set
 */
static inline void
ftm_set_counter(FTM_t *const ftmBase, uint16_t value)
{
    FTM_RMW_CNT(ftmBase, FTM_CNT_COUNT_MASK, FTM_CNT_COUNT(value));
}

/*!
 * @brief Sets the FTM peripheral timer channel counter value.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] channel The FTM peripheral channel number
 * @param[in] value Counter value to be set
 */
static inline void
ftm_set_chn_count_val(FTM_t *const ftmBase, uint8_t channel, uint16_t value)
{
    DEV_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);

    ((ftmBase)->CONTROLS[channel].CnV) = value;
}

/*!
 * @brief Gets the FTM count direction bit.
 *
 * @param[in] ftmBase The FTM base address pointer
 *
 * @return The Center-Aligned PWM selection
 *         - 1U: Up counting mode
 *         - 0U: Up down counting mode
 */
static inline bool
ftm_get_cpwms(const FTM_t *ftmBase)
{
    return ((ftmBase->SC & FTM_SC_CPWMS_MASK) >> FTM_SC_CPWMS_SHIFT) != 0U;
}

/*!
 * @brief Enables or disables the FTM peripheral timer software trigger.
 *
 * @param[in] ftmBase The FTM base address pointer.
 * @param[in] enable Software trigger selection
 *                   - true : Software trigger is selected
 *                   - false: Software trigger is not selected
 */
static inline void
ftm_set_sw_trigger(FTM_t *const ftmBase, bool enable)
{
    FTM_RMW_SYNC(ftmBase, FTM_SYNC_SWSYNC_MASK, FTM_SYNC_SWSYNC(enable));
}

/*!
 * @brief Enables the FTM peripheral timer channel(n) interrupt.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] channel The FTM peripheral channel number
 */
static inline void
ftm_enable_chn_int(FTM_t *const ftmBase, uint8_t channel)
{
    DEV_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);

    FTM_RMW_CnSCV_REG(ftmBase, channel, FTM_CnSC_CHIE_MASK, FTM_CnSC_CHIE(1U));
}

/*!
 * @brief Disables the FTM peripheral timer channel(n) interrupt.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] channel The FTM peripheral channel number
 */
static inline void
ftm_disable_chn_int(FTM_t *const ftmBase, uint8_t channel)
{
    DEV_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);

    FTM_RMW_CnSCV_REG(ftmBase, channel, FTM_CnSC_CHIE_MASK, FTM_CnSC_CHIE(0U));
}

/*!
 * @brief Clear the channel flag by writing a 0 to the CHF bit.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] channel The FTM peripheral channel number
 */
static inline void
ftm_clear_chn_event_flag(FTM_t *const ftmBase, uint8_t channel)
{
    DEV_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);

    FTM_RMW_CnSCV_REG(ftmBase, channel, FTM_CnSC_CHF_MASK, FTM_CnSC_CHF(0U));
}

/*!
 * @brief Disable PWM channel Outputs.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] channel The FTM channel
 */
static inline void
ftm_disable_pwm_channel_outputs(FTM_t *const ftmBase, uint8_t channel)
{
    uint32_t regValue = ((ftmBase)->SC);
    regValue          = regValue & (~(1UL << (channel + FTM_FEATURE_OUTPUT_CHANNEL_OFFSET)));
    ((ftmBase)->SC)   = (regValue);
}

/*!
 * @brief Sets the encoding mode used in quadrature decoding mode.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] quadMode Quadrature decoder mode selection
 *                     - 0U: Phase A and Phase B encoding mode
 *                     - 1U: Count and direction encoding mode
 */
static inline void
ftm_set_quad_mode(FTM_t *const ftmBase, uint8_t quadMode)
{
    FTM_RMW_QDCTRL(ftmBase, FTM_QDCTRL_QUADMODE_MASK, FTM_QDCTRL_QUADMODE(quadMode));
}

/*!
 * @brief Enables or disables the phase A input filter.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of phase A input filter
 *                   - true : Enables the phase input filter
 *                   - false: Disables the filter
 */
static inline void
ftm_set_quad_phase_a_filter(FTM_t *const ftmBase, bool enable)
{
    if (enable)
    {
        ((ftmBase)->QDCTRL) |= (1UL << FTM_QDCTRL_PHAFLTREN_SHIFT);
    }
    else
    {
        ((ftmBase)->QDCTRL) &= ~(1UL << FTM_QDCTRL_PHAFLTREN_SHIFT);
    }
}

/*!
 * @brief Enables or disables the phase B input filter.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of phase B input filter
 *                   - true : Enables the phase input filter
 *                   - false: Disables the filter
 */
static inline void
ftm_set_quad_phase_b_filter(FTM_t *const ftmBase, bool enable)
{
    if (enable)
    {
        ((ftmBase)->QDCTRL) |= (1UL << FTM_QDCTRL_PHBFLTREN_SHIFT);
    }
    else
    {
        ((ftmBase)->QDCTRL) &= ~(1UL << FTM_QDCTRL_PHBFLTREN_SHIFT);
    }
}

/*!
 * @brief Selects polarity for the quadrature decode phase A input.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] mode Phase A input polarity selection
 *                - 0U: Normal polarity
 *                - 1U: Inverted polarity
 */
static inline void
ftm_set_quad_phase_a_polarity(FTM_t *const ftmBase, uint8_t mode)
{
    FTM_RMW_QDCTRL(ftmBase, FTM_QDCTRL_PHAPOL_MASK, FTM_QDCTRL_PHAPOL(mode));
}

/*!
 * @brief Selects polarity for the quadrature decode phase B input.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] mode Phase B input polarity selection
 *                - 0U: Normal polarity
 *                - 1U: Inverted polarity
 */
static inline void
ftm_set_quad_phase_b_polarity(FTM_t *const ftmBase, uint8_t mode)
{
    FTM_RMW_QDCTRL(ftmBase, FTM_QDCTRL_PHBPOL_MASK, FTM_QDCTRL_PHBPOL(mode));
}

/*!
 * @brief Set the FTM reload interrupt enable.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable - true : Reload interrupt is  enabled
 *                   - false: Reload interrupt is disabled
 */
static inline void
ftm_set_re_int_enabled(FTM_t *const ftmBase, bool enable)
{
    FTM_RMW_SC(ftmBase, FTM_SC_RIE_MASK, FTM_SC_RIE(enable));
}

/*!
 * @brief Enable the half cycle reload.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of the half cycle match as a reload opportunity
 *                   - true : Half cycle reload is enabled
 *                   - false: Half cycle reload is disabled
 *
 * Implements : ftm_set_half_cycle_Activity
 */
static inline void
ftm_set_half_cycle(FTM_t *const ftmBase, bool enable)
{
    if (enable)
    {
        ((ftmBase)->PWMLOAD) |= (1UL << FTM_PWMLOAD_HCSEL_SHIFT);
    }
    else
    {
        ((ftmBase)->PWMLOAD) &= ~(1UL << FTM_PWMLOAD_HCSEL_SHIFT);
    }
}

/*!
 * @brief Get the state whether the FTM counter reached a reload point.
 *
 * @param[in] ftmBase The FTM base address pointer
 *
 * @return State of reload point
 *         - true : FTM counter reached a reload point
 *         - false: FTM counter did not reach a reload point
 */
static inline bool
ftm_get_reload_flag(const FTM_t *ftmBase)
{
    return ((ftmBase->SC & FTM_SC_RF_MASK) >> FTM_SC_RF_SHIFT) != 0U;
}

/*!
 * @brief Enables or disables the loading of MOD, CNTIN and CV with values of their write buffer.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] enable State of loading updated values
 *                   - true : To enable the loading of value of their buffer
 *                   - false: To disable the loading of value of their buffer
 *
 * Implements : ftm_set_pwm_load_Activity
 */
static inline void
ftm_set_pwm_load(FTM_t *const ftmBase, bool enable)
{
    if (enable)
    {
        ((ftmBase)->PWMLOAD) |= (1UL << FTM_PWMLOAD_LDOK_SHIFT);
    }
    else
    {
        ((ftmBase)->PWMLOAD) &= ~(1UL << FTM_PWMLOAD_LDOK_SHIFT);
    }
}

/*!
 * @brief Returns the FTM peripheral timer overflow interrupt flag.
 *
 * @param[in] ftmBase The FTM base address pointer
 *
 * @return State of Timer Overflow Flag
 *         - true : FTM counter has overflowed
 *         - false: FTM counter has not overflowed
 */
static inline bool
ftm_has_timer_over_flowed(const FTM_t *ftmBase)
{
    return ((ftmBase->SC & FTM_SC_TOF_MASK) >> FTM_SC_TOF_SHIFT) != 0U;
}

#ifdef __cplusplus
}
#endif

#endif /* FTM_ACCESS_H */
