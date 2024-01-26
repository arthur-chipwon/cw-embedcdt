/**
 * @file ftm_driver.h
 * @brief FTM driver header file.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef FTM_DRIVER_H
#define FTM_DRIVER_H

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
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/*! @brief Maximum value for PWM duty cycle */
#define FTM_MAX_DUTY_CYCLE (0x8000U)
/*! @brief Shift value which converts duty to ticks */
#define FTM_DUTY_TO_TICKS_SHIFT (15U)

/*!< @brief Channel number for CHAN0.*/
#define CHAN0_IDX (0U)
/*!< @brief Channel number for CHAN1.*/
#define CHAN1_IDX (1U)
/*!< @brief Channel number for CHAN2.*/
#define CHAN2_IDX (2U)
/*!< @brief Channel number for CHAN3.*/
#define CHAN3_IDX (3U)
/*!< @brief Channel number for CHAN4.*/
#define CHAN4_IDX (4U)
/*!< @brief Channel number for CHAN5.*/
#define CHAN5_IDX (5U)
/*!< @brief Channel number for CHAN6.*/
#define CHAN6_IDX (6U)
/*!< @brief Channel number for CHAN7.*/
#define CHAN7_IDX (7U)

/*!
 * @brief FTM_SC - Read and modify and write to Status And Control (RW)
 */
#define FTM_RMW_SC(base, mask, value) (((base)->SC) = ((((base)->SC) & ~(mask)) | (value)))

/*!
 * @brief FTM_CNT - Read and modify and write to Counter (RW)
 */
#define FTM_RMW_CNT(base, mask, value) (((base)->CNT) = ((((base)->CNT) & ~(mask)) | (value)))

/*!
 * @brief FTM_MODE -  Read and modify and write Counter Features Mode Selection (RW)
 */
#define FTM_RMW_MODE(base, mask, value) (((base)->MODE) = ((((base)->MODE) & ~(mask)) | (value)))

/*!
 * @brief SYNC -  Read and modify and write Synchronization (RW)
 */
#define FTM_RMW_SYNC(base, mask, value) (((base)->SYNC) = ((((base)->SYNC) & ~(mask)) | (value)))

/*!
 * @brief FTM_CONF -  Read and modify and write Configuration (RW)
 */
#define FTM_RMW_CONF(base, mask, value) (((base)->CONF) = ((((base)->CONF) & ~(mask)) | (value)))

/*!
 * @brief FTM_CNTIN - Read and modify and write Counter Initial Value (RW)
 */
#define FTM_RMW_CNTIN(base, mask, value) (((base)->CNTIN) = ((((base)->CNTIN) & ~(mask)) | (value)))

/*!
 * @brief FTM_MOD - Read and modify and write Modulo (RW)
 */
#define FTM_RMW_MOD(base, mask, value) (((base)->MOD) = ((((base)->MOD) & ~(mask)) | (value)))

/*!
 * @brief FTM_FMS -  Read and modify and write Fault Mode Status (RW)
 */
#define FTM_RMW_FMS(base, mask, value) (((base)->FMS) = ((((base)->FMS) & ~(mask)) | (value)))

/*!
 * @brief FTM_EXTTRIG - Read and modify and write External Trigger Control (RW)
 */
#define FTM_RMW_EXTTRIG_REG(base, mask, value) \
    (((base)->EXTTRIG) = ((((base)->EXTTRIG) & ~(mask)) | (value)))

/*!
 * @brief FTM_CnSCV -  Read and modify and write Channel (n) Status And Control (RW)
 */
#define FTM_RMW_CnSCV_REG(base, channel, mask, value) \
    (((base)->CONTROLS[channel].CnSC) = ((((base)->CONTROLS[channel].CnSC) & ~(mask)) | (value)))

/*!
 * @brief FTM_FLTCTRL -  Read and modify and write Fault Control (RW)
 */
#define FTM_RMW_FLTCTRL(base, mask, value) \
    (((base)->FLTCTRL) = ((((base)->FLTCTRL) & ~(mask)) | (value)))

/*!
 * @brief FTM_DEADTIME - Read and modify and write Dead-time Insertion Control (RW)
 */
#define FTM_RMW_DEADTIME(base, mask, value) \
    (((base)->DEADTIME) = ((((base)->DEADTIME) & ~(mask)) | (value)))

/*!
 * @brief FILTER -  Read and modify and write Filter (RW)
 */
#define FTM_RMW_FILTER(base, mask, value) \
    (((base)->FILTER) = ((((base)->FILTER) & ~(mask)) | (value)))

/*!
 * @brief QDCTRL -  Read and modify and write Quadrature Decoder Control And Status (RW)
 */
#define FTM_RMW_QDCTRL(base, mask, value) \
    (((base)->QDCTRL) = ((((base)->QDCTRL) & ~(mask)) | (value)))

/*!
 * @brief FlexTimer operation mode
 *
 * Implements : ftm_config_mode_t_Class
 */
typedef enum
{
    FTM_MODE_NOT_INITIALIZED                    = 0x00U, /*!< The driver is not initialized */
    FTM_MODE_INPUT_CAPTURE                      = 0x01U, /*!< Input capture */
    FTM_MODE_OUTPUT_COMPARE                     = 0x02U, /*!< Output compare */
    FTM_MODE_EDGE_ALIGNED_PWM                   = 0x03U, /*!< Edge aligned PWM */
    FTM_MODE_CEN_ALIGNED_PWM                    = 0x04U, /*!< Center aligned PWM */
    FTM_MODE_QUADRATURE_DECODER                 = 0x05U, /*!< Quadrature decoder */
    FTM_MODE_UP_TIMER                           = 0x06U, /*!< Timer with up counter */
    FTM_MODE_UP_DOWN_TIMER                      = 0x07U, /*!< timer with up-down counter */
    FTM_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE = 0x08U, /*!< Edge aligned PWM and input capture */
} ftm_config_mode_t;

/*!
 * @brief FlexTimer clock source selection
 *
 * Implements : ftm_clock_source_t_Class
 */
typedef enum
{
    FTM_CLOCK_SOURCE_NONE        = 0x00U, /*!< None use clock for FTM  */
    FTM_CLOCK_SOURCE_SYSTEMCLK   = 0x01U, /*!< System clock            */
    FTM_CLOCK_SOURCE_FIXEDCLK    = 0x02U, /*!< Fixed clock             */
    FTM_CLOCK_SOURCE_EXTERNALCLK = 0x03U  /*!< External clock          */
} ftm_clock_source_t;

/*!
 * @brief FlexTimer pre-scaler factor selection for the clock source.
 * In quadrature decoder mode set FTM_CLOCK_DIVID_BY_1
 *
 * Implements : ftm_clock_ps_t_Class
 */
typedef enum
{
    FTM_CLOCK_DIVID_BY_1   = 0x00U, /*!< Divide by 1   */
    FTM_CLOCK_DIVID_BY_2   = 0x01U, /*!< Divide by 2   */
    FTM_CLOCK_DIVID_BY_4   = 0x02U, /*!< Divide by 4   */
    FTM_CLOCK_DIVID_BY_8   = 0x03U, /*!< Divide by 8   */
    FTM_CLOCK_DIVID_BY_16  = 0x04U, /*!< Divide by 16  */
    FTM_CLOCK_DIVID_BY_32  = 0x05U, /*!< Divide by 32  */
    FTM_CLOCK_DIVID_BY_64  = 0x06U, /*!< Divide by 64  */
    FTM_CLOCK_DIVID_BY_128 = 0x07U  /*!< Divide by 128 */
} ftm_clock_ps_t;

/*!
 * @brief List of FTM interrupts
 *
 * Implements : ftm_interrupt_option_t_Class
 */
typedef enum
{
    FTM_CHANNEL0_INT_ENABLE       = 0x00000001U, /*!< Channel 0 interrupt */
    FTM_CHANNEL1_INT_ENABLE       = 0x00000002U, /*!< Channel 1 interrupt */
    FTM_CHANNEL2_INT_ENABLE       = 0x00000004U, /*!< Channel 2 interrupt */
    FTM_CHANNEL3_INT_ENABLE       = 0x00000008U, /*!< Channel 3 interrupt */
    FTM_CHANNEL4_INT_ENABLE       = 0x00000010U, /*!< Channel 4 interrupt */
    FTM_CHANNEL5_INT_ENABLE       = 0x00000020U, /*!< Channel 5 interrupt */
    FTM_CHANNEL6_INT_ENABLE       = 0x00000040U, /*!< Channel 6 interrupt */
    FTM_CHANNEL7_INT_ENABLE       = 0x00000080U, /*!< Channel 7 interrupt */
    FTM_FAULT_INT_ENABLE          = 0x00000100U, /*!< Fault interrupt */
    FTM_TIME_OVER_FLOW_INT_ENABLE = 0x00000200U, /*!< Time overflow interrupt */
    FTM_RELOAD_INT_ENABLE = 0x00000400U /*!< Reload interrupt; Available only on certain SoC's */
} ftm_interrupt_option_t;

/*!
 * @brief List of FTM flags
 *
 * Implements : ftm_status_flag_t_Class
 */
typedef enum
{
    FTM_CHANNEL0_FLAG        = 0x00000001U, /*!< Channel 0 Flag */
    FTM_CHANNEL1_FLAG        = 0x00000002U, /*!< Channel 1 Flag */
    FTM_CHANNEL2_FLAG        = 0x00000004U, /*!< Channel 2 Flag */
    FTM_CHANNEL3_FLAG        = 0x00000008U, /*!< Channel 3 Flag */
    FTM_CHANNEL4_FLAG        = 0x00000010U, /*!< Channel 4 Flag */
    FTM_CHANNEL5_FLAG        = 0x00000020U, /*!< Channel 5 Flag */
    FTM_CHANNEL6_FLAG        = 0x00000040U, /*!< Channel 6 Flag */
    FTM_CHANNEL7_FLAG        = 0x00000080U, /*!< Channel 7 Flag */
    FTM_FAULT_FLAG           = 0x00000100U, /*!< Fault Flag */
    FTM_TIME_OVER_FLOW_FLAG  = 0x00000200U, /*!< Time overflow Flag */
    FTM_RELOAD_FLAG          = 0x00000400U, /*!< Reload Flag; Available only on certain SoC's */
    FTM_CHANNEL_TRIGGER_FLAG = 0x00000800U  /*!< Channel trigger Flag */
} ftm_status_flag_t;

/*!
 * @brief FTM sync source
 *
 * Implements : ftm_reg_update_t_Class
 */
typedef enum
{
    FTM_SYSTEM_CLOCK = 0U, /*!< Register is updated with its buffer value at all rising
                            *   edges of system clock */
    FTM_PWM_SYNC = 1U      /*!< Register is updated with its buffer value at the
                            *   FTM synchronization */
} ftm_reg_update_t;

/*!
 * @brief FTM update register
 *
 * Implements : ftm_pwm_sync_mode_t_Class
 */
typedef enum
{
    FTM_WAIT_LOADING_POINTS = 0U, /*!< FTM register is updated at first loading point */
    FTM_UPDATE_NOW          = 1U  /*!< FTM register is updated immediately */
} ftm_pwm_sync_mode_t;

/*!
 * @brief FlexTimer pre-scaler factor for the dead-time insertion
 *
 * Implements : ftm_deadtime_ps_t_Class
 */
typedef enum
{
    FTM_DEADTIME_DIVID_BY_1  = 0x01U, /*!< Divide by 1   */
    FTM_DEADTIME_DIVID_BY_4  = 0x02U, /*!< Divide by 4   */
    FTM_DEADTIME_DIVID_BY_16 = 0x03U  /*!< Divide by 16  */
} ftm_deadtime_ps_t;

/*!
 * @brief Options for the FlexTimer behavior in BDM Mode
 *
 * Implements : ftm_bdm_mode_t_Class
 */
typedef enum
{
    FTM_BDM_MODE_00 = 0x00U, /*!< FTM counter stopped, CH(n)F bit can be set, FTM channels
                              *   in functional mode, writes to MOD,CNTIN and C(n)V registers bypass
                              *   the register buffers */
    FTM_BDM_MODE_01 = 0x01U, /*!< FTM counter stopped, CH(n)F bit is not set, FTM channels
                              *   outputs are forced to their safe value , writes to MOD,CNTIN and
                              *   C(n)V registers bypass the register buffers */
    FTM_BDM_MODE_10 = 0x02U, /*!< FTM counter stopped, CH(n)F bit is not set, FTM channels
                              *    outputs are frozen when chip enters in BDM mode, writes to MOD,
                              *    CNTIN and C(n)V registers bypass the register buffers */
    FTM_BDM_MODE_11 = 0x03U  /*!< FTM counter in functional mode, CH(n)F bit can be set,
                              *   FTM channels in functional mode, writes to MOD,CNTIN and C(n)V
                              *   registers is in fully functional mode */
} ftm_bdm_mode_t;

/*!
 * @brief FlexTimer state structure of the driver
 *
 * Implements : ftm_state_t_Class
 */
typedef struct
{
    ftm_clock_source_t ftmClockSource; /*!< Clock source used by FTM counter */
    ftm_config_mode_t  ftmMode;        /*!< Mode of operation for FTM */
    uint16_t ftmModValue; /*!< This field is used only in input capture mode to store MOD value */
    uint16_t ftmPeriod;   /*!< This field is used only in PWM mode to store signal period */
    uint32_t ftmSourceClockFrequency; /*!< The clock frequency is used for counting */
    uint16_t
        measurementResults[FEATURE_FTM_CHANNEL_COUNT]; /*!< This field is used only in input capture
                                                          mode to store edges time stamps */
    void *channelsCallbacksParams[FEATURE_FTM_CHANNEL_COUNT];   /*!< The parameters of callback
                                                                   function for channels events */
    ic_callback_t channelsCallbacks[FEATURE_FTM_CHANNEL_COUNT]; /*!< The callback function for
                                                                   channels events */
    bool
        enableNotification[FEATURE_FTM_CHANNEL_COUNT]; /*!< To save channels enable the notification
                                                          on the callback application */
} ftm_state_t;

/*!
 * @brief FlexTimer Registers sync parameters
 *        Please don't use software and hardware trigger simultaneously
 * Implements : ftm_pwm_sync_t_Class
 */
typedef struct
{
    bool softwareSync;                    /*!< True - enable software sync,
                                           *   False - disable software sync */
    bool hardwareSync0;                   /*!< True - enable hardware 0 sync,
                                           *   False - disable hardware 0 sync */
    bool hardwareSync1;                   /*!< True - enable hardware 1 sync,
                                           *   False - disable hardware 1 sync */
    bool hardwareSync2;                   /*!< True - enable hardware 2 sync,
                                           *   False - disable hardware 2 sync */
    bool maxLoadingPoint;                 /*!< True - enable maximum loading point,
                                           *   False - disable maximum loading point */
    bool minLoadingPoint;                 /*!< True - enable minimum loading point,
                                           *   False - disable minimum loading point */
    ftm_reg_update_t    inverterSync;     /*!< Configures INVCTRL sync */
    ftm_reg_update_t    outRegSync;       /*!< Configures SWOCTRL sync */
    ftm_reg_update_t    maskRegSync;      /*!< Configures OUTMASK sync */
    ftm_reg_update_t    initCounterSync;  /*!< Configures CNTIN sync */
    bool                autoClearTrigger; /*!< Available only for hardware trigger */
    ftm_pwm_sync_mode_t syncPoint;        /*!< Configure synchronization method
                                           *   (waiting next loading point or immediate) */
} ftm_pwm_sync_t;

/*!
 * @brief Configuration structure that the user needs to set
 *
 * Implements : ftm_user_config_t_Class
 */
typedef struct
{
    ftm_pwm_sync_t syncMethod;          /*!< Register sync options available in the
                                         *   ftm_sync_method_t enumeration  */
    ftm_config_mode_t ftmMode;          /*!< Mode of operation for FTM */
    ftm_clock_ps_t    ftmPrescaler;     /*!< Register pre-scaler options available in the
                                         *   ftm_clock_ps_t enumeration  */
    ftm_clock_source_t ftmClockSource;  /*!< Select clock source for FTM */
    ftm_bdm_mode_t     BDMMode;         /*!< Select FTM behavior in BDM mode */
    bool               isTofIsrEnabled; /*!< true: enable interrupt,
                                         *   false: write interrupt is disabled */
    bool
        enableInitializationTrigger;    /*!< true: enable the generation of initialization trigger
                                         *   false: disable the generation of initialization trigger */
} ftm_user_config_t;

/*!
 * @brief The configuration structure in timer mode
 *
 * Implements : ftm_timer_param_t_Class
 */
typedef struct
{
    ftm_config_mode_t mode;         /*!< FTM mode */
    uint16_t          initialValue; /*!< Initial counter value */
    uint16_t          finalValue;   /*!< Final counter value */
} ftm_timer_param_t;

/*!
 * @brief FlexTimer Configure type of PWM update in the duty cycle or in ticks
 *
 * Implements : ftm_pwm_update_option_t_Class
 */
typedef enum
{
    FTM_PWM_UPDATE_IN_DUTY_CYCLE = 0x00U, /*!< The type of PWM update in the duty cycle/pulse or
                                             also use in frequency update */
    FTM_PWM_UPDATE_IN_TICKS = 0x01U       /*!< The type of PWM update in ticks */
} ftm_pwm_update_option_t;

/*!
 * @brief The polarity of the channel output is configured in PWM signal
 *
 * Implements : ftm_polarity_t_Class
 */
typedef enum
{
    FTM_POLARITY_LOW  = 0x00U, /*!< The channel polarity is active LOW which is defined again */
    FTM_POLARITY_HIGH = 0x01U  /*!< The channel polarity is active HIGH which is defined again */
} ftm_polarity_t;

/*!
 * @brief FlexTimer PWM channel (n+1) polarity for combine mode
 *
 * Implements : ftm_second_channel_polarity_t_Class
 */
typedef enum
{
    FTM_MAIN_INVERTED = 0x01U,  /*!< The channel (n+1) output is the inverse of the
                                 *   channel (n) output  */
    FTM_MAIN_DUPLICATED = 0x00U /*!< The channel (n+1) output is the same as the
                                 *   channel (n) output */
} ftm_second_channel_polarity_t;

/*!
 * @brief FlexTimer fault control
 *
 * Implements : ftm_fault_mode_t_Class
 */
typedef enum
{
    FTM_FAULT_CONTROL_DISABLED = 0x00U, /*!< Fault control is disabled for all channels */
    FTM_FAULT_CONTROL_MAN_EVEN = 0x01U, /*!< Fault control is enabled for even channels
                                         *   only (channels 0, 2, 4, and 6), and the selected
                                         *   mode is the manual fault clearing */
    FTM_FAULT_CONTROL_MAN_ALL = 0x02U,  /*!< Fault control is enabled for all channels,
                                         *   and the selected mode is the manual fault clearing */
    FTM_FAULT_CONTROL_AUTO_ALL = 0x03U  /*!< Fault control is enabled for all channels, and
                                         *   the selected mode is the automatic fault clearing */
} ftm_fault_mode_t;

/*!
 * @brief Select level of the channel (n) output at the beginning
 *
 * Implements : ftm_safe_state_polarity_t_Class
 */
typedef enum
{
    FTM_LOW_STATE  = 0x00U, /*!< When fault is detected PWM channel is low. */
    FTM_HIGH_STATE = 0x01U  /*!< When fault is detected PWM channel is high. */
} ftm_safe_state_polarity_t;

/*!
 * @brief FlexTimer driver PWM Fault channel parameters
 *
 * Implements : ftm_pwm_ch_fault_param_t_Class
 */
typedef struct
{
    bool           faultChannelEnabled; /*!< Fault channel state */
    bool           faultFilterEnabled;  /*!< Fault channel filter state */
    ftm_polarity_t ftmFaultPinPolarity; /*!< Channel output state on fault */
} ftm_pwm_ch_fault_param_t;

/*!
 * @brief FlexTimer driver PWM Fault parameter
 *
 * Implements : ftm_pwm_fault_param_t_Class
 */
typedef struct
{
    bool    pwmOutputStateOnFault; /*!< Output pin state on fault (safe state or tri-state) */
    bool    pwmFaultInterrupt;     /*!< PWM fault interrupt state */
    uint8_t faultFilterValue;      /*!< Fault filter value */
    ftm_fault_mode_t faultMode;    /*!< Fault mode */
    ftm_pwm_ch_fault_param_t
        ftmFaultChannelParam[FTM_FEATURE_FAULT_CHANNELS]; /*!< Fault channels configuration */
} ftm_pwm_fault_param_t;

/*!
 * @brief FlexTimer driver independent PWM parameter
 *
 * Implements : ftm_independent_ch_param_t_Class
 */
typedef struct
{
    uint8_t        hwChannelId;       /*!< Physical hardware channel ID */
    ftm_polarity_t polarity;          /*!< Polarity of the PWM signal generated on MCU pin.*/
    uint16_t       uDutyCyclePercent; /*!< PWM pulse width, value should be between
                                       *   0 (0%) to FTM_MAX_DUTY_CYCLE (100%) */
    bool enableExternalTrigger; /*!< true: enable the generation of a trigger is used for on-chip
                                 * modules false: disable the generation of a trigger */
    ftm_safe_state_polarity_t
        safeState;              /*!< Logical state of the PWM channel n when an fault is detected
                                 *   and to set up the polarity of PWM signal on the channel (n+1) */
    bool enableSecondChannelOutput; /*!< Enable complementary mode on next channel */
    ftm_second_channel_polarity_t secondChannelPolarity; /*!< Polarity of the channel n+1 relative
                                                            to channel n in the complementary mode*/
    bool deadTime; /*!< Enable/disable dead time for channel */
} ftm_independent_ch_param_t;

/*!
 * @brief FlexTimer driver combined PWM parameter
 *
 * Implements : ftm_combined_ch_param_t_Class

 */
typedef struct
{
    uint8_t  hwChannelId; /*!< Physical hardware channel ID for channel (n) */
    uint16_t firstEdge;  /*!< First edge time. This time is relative to signal period. The value for
                          * this parameter is  between 0 and FTM_MAX_DUTY_CYCLE(0 = 0% from period
                          * and  FTM_MAX_DUTY_CYCLE = 100% from period) */
    uint16_t secondEdge; /*!< Second edge time. This time is relative to signal period. The value
                          * for this parameter is between 0 and FTM_MAX_DUTY_CYCLE(0 = 0% from
                          * period and FTM_MAX_DUTY_CYCLE = 100% from period) */
    bool deadTime;       /*!< Enable/disable dead time for channel */
    bool enableModifiedCombine; /*!< Enable/disable the modified combine mode for channels (n) and
                                   (n+1) */
    ftm_polarity_t
         mainChannelPolarity; /*!< Polarity of the PWM signal generated on MCU pin for channel n.*/
    bool enableSecondChannelOutput; /*!< Select if channel (n+1)  output is enabled/disabled for the
                                       complementary mode */
    ftm_second_channel_polarity_t
        secondChannelPolarity; /*!< Select channel (n+1) polarity relative to channel (n) in the
                                  complementary mode */
    bool
        enableExternalTrigger; /*!< The generation of the channel (n) trigger
                                *   true: enable the generation of a trigger on the channel (n)
                                *   false: disable the generation of a trigger on the channel (n) */
    bool enableExternalTriggerOnNextChn; /*!< The generation of the channel (n+1) trigger
                                          *   true: enable the generation of a trigger on the
                                          * channel (n+1) false: disable the generation of a trigger
                                          * on the channel (n+1) */
    ftm_safe_state_polarity_t
        mainChannelSafeState; /*!< The selection of the channel (n) state when fault is detected */
    ftm_safe_state_polarity_t
        secondChannelSafeState; /*!< The selection of the channel (n+1) state when fault is detected
                                 *   and set up the polarity of PWM signal on the channel (n+1) */
} ftm_combined_ch_param_t;

/*!
 * @brief FlexTimer driver PWM parameters
 *
 * Implements : ftm_pwm_param_t_Class
 */
typedef struct
{
    uint8_t           nNumIndependentPwmChannels; /*!< Number of independent PWM channels */
    uint8_t           nNumCombinedPwmChannels;    /*!< Number of combined PWM channels */
    ftm_config_mode_t mode;                       /*!< FTM mode */
    uint8_t           deadTimeValue;              /*!< Dead time value in [ticks] */
    ftm_deadtime_ps_t deadTimePrescaler;          /*!< Dead time pre-scaler value[ticks] */
    uint32_t          uFrequencyHZ;               /*!< PWM period in Hz */
    ftm_independent_ch_param_t
        *pwmIndependentChannelConfig;             /*!< Configuration for independent PWM channels */
    ftm_combined_ch_param_t
                          *pwmCombinedChannelConfig; /*!< Configuration for combined PWM channels */
    ftm_pwm_fault_param_t *faultConfig;              /*!< Configuration for PWM fault */
} ftm_pwm_param_t;

/*!
 * @brief Selects mode operation in the input capture
 *
 * Implements : ftm_input_op_mode_t_Class
 */
typedef enum
{
    FTM_EDGE_DETECT  = 0U, /*!< FTM edge detect */
    FTM_NO_OPERATION = 2U  /*!< FTM no operation */
} ftm_input_op_mode_t;

/*!
 * @brief FlexTimer input capture edge mode as rising edge or falling edge
 *
 * Implements : ftm_edge_alignment_mode_t_Class
 */
typedef enum
{
    FTM_NO_PIN_CONTROL = 0x00U, /*!< No trigger */
    FTM_RISING_EDGE    = 0x01U, /*!< Rising edge trigger */
    FTM_FALLING_EDGE   = 0x02U, /*!< Falling edge trigger */
    FTM_BOTH_EDGES     = 0x03U  /*!< Rising and falling edge trigger */
} ftm_edge_alignment_mode_t;

/*!
 * @brief FlexTimer input capture measurement type for dual edge input capture
 *
 * Implements : ftm_signal_measurement_mode_t_Class
 */
typedef enum
{
    FTM_NO_MEASUREMENT = 0x00U, /*!< No measurement */
    FTM_RISING_EDGE_PERIOD_MEASUREMENT =
        0x01U,                  /*!< Period measurement between two consecutive rising edges */
    FTM_FALLING_EDGE_PERIOD_MEASUREMENT =
        0x02U,                  /*!< Period measurement between two consecutive falling edges */
    FTM_PERIOD_ON_MEASUREMENT =
        0x03U, /*!< The time measurement taken for the pulse to remain ON or HIGH state */
    FTM_PERIOD_OFF_MEASUREMENT =
        0x04U  /*!< The time measurement taken for the pulse to remain OFF or LOW state */
} ftm_signal_measurement_mode_t;

/*!
 * @brief FlexTimer driver Input capture parameters for each channel
 *
 * Implements : ftm_input_ch_param_t_Class
 */
typedef struct
{
    uint8_t                       hwChannelId;     /*!< Physical hardware channel ID*/
    ftm_input_op_mode_t           inputMode;       /*!< FlexTimer module mode of operation  */
    ftm_edge_alignment_mode_t     edgeAlignement;  /*!< Edge alignment Mode for signal measurement*/
    ftm_signal_measurement_mode_t measurementType; /*!< Measurement Mode for signal measurement*/
    uint16_t                      filterValue;     /*!< Filter Value */
    bool                          filterEn;        /*!< Input capture filter state */
    bool                          continuousModeEn; /*!< Continuous measurement state */
    // void * channelsCallbacksParams;                     /*!< The parameters of callback functions
    // for channels events */ ic_callback_t channelsCallbacks;                    /*!< The callback
    // function for channels events */
} ftm_input_ch_param_t;

/*!
 * @brief FlexTimer driver input capture parameters
 *
 * Implements : ftm_input_param_t_Class
 */
typedef struct
{
    uint8_t  nNumChannels;   /*!< Number of input capture channel used */
    uint16_t nMaxCountValue; /*!< Maximum counter value. Minimum value is 0 for this mode */
    ftm_input_ch_param_t *inputChConfig; /*!< Input capture channels configuration */
} ftm_input_param_t;

/*!
 * @brief FlexTimer Mode configuration for output compare mode
 *
 * Implements : ftm_output_compare_mode_t_Class
 */
typedef enum
{
    FTM_DISABLE_OUTPUT  = 0x00U, /*!< No action on output pin */
    FTM_TOGGLE_ON_MATCH = 0x01U, /*!< Toggle on match */
    FTM_CLEAR_ON_MATCH  = 0x02U, /*!< Clear on match */
    FTM_SET_ON_MATCH    = 0x03U  /*!< Set on match */
} ftm_output_compare_mode_t;

/*!
 * @brief FlexTimer input capture type of the next output compare value
 *
 * Implements : ftm_output_compare_update_t_Class
 */
typedef enum
{
    FTM_RELATIVE_VALUE = 0x00U, /*!< Next compared value is relative to current value */
    FTM_ABSOLUTE_VALUE = 0x01U  /*!< Next compared value is absolute */
} ftm_output_compare_update_t;

/*!
 * @brief FlexTimer driver PWM parameters each channel in the output compare mode
 *
 * Implements : ftm_output_cmp_ch_param_t_Class
 */
typedef struct
{
    uint8_t                   hwChannelId;   /*!< Physical hardware channel ID*/
    ftm_output_compare_mode_t chMode;        /*!< Channel output mode*/
    uint16_t                  comparedValue; /*!< The compared value */
    bool enableExternalTrigger; /*!< true: enable the generation of a trigger is used for on-chip
                                 * modules false: disable the generation of a trigger */
} ftm_output_cmp_ch_param_t;

/*!
 * @brief FlexTimer driver PWM parameters which is configured for the list of channels
 *
 * Implements : ftm_output_cmp_param_t_Class
 */
typedef struct
{
    uint8_t                    nNumOutputChannels;  /*!< Number of output compare channels */
    ftm_config_mode_t          mode;                /*!< FlexTimer PWM operation mode */
    uint16_t                   maxCountValue;       /*!< Maximum count value in ticks */
    ftm_output_cmp_ch_param_t *outputChannelConfig; /*!< Output compare channels configuration */
} ftm_output_cmp_param_t;

/*!
 * @brief FlexTimer quadrature decode modes, phase encode or count and direction mode
 *
 * Implements : ftm_quad_decode_mode_t_Class
 */
typedef enum
{
    FTM_QUAD_PHASE_ENCODE  = 0x00U, /*!< Phase encoding mode                 */
    FTM_QUAD_COUNT_AND_DIR = 0x01U  /*!< Counter and direction encoding mode */
} ftm_quad_decode_mode_t;

/*!
 * @brief FlexTimer quadrature phase polarities, normal or inverted polarity
 *
 * Implements : ftm_quad_phase_polarity_t_Class
 */
typedef enum
{
    FTM_QUAD_PHASE_NORMAL = 0x00U, /*!< Phase input signal is not inverted before identifying
                                    *   the rising and falling edges of this signal */
    FTM_QUAD_PHASE_INVERT = 0x01U  /*!< Phase input signal is inverted before identifying
                                    *   the rising and falling edges of this signal */
} ftm_quad_phase_polarity_t;

/*!
 * @brief FlexTimer quadrature decoder channel parameters
 *
 * Implements : ftm_phase_params_t_Class
 */
typedef struct
{
    bool phaseInputFilter;                    /*!< false: disable phase filter,
                                               *   true: enable phase filter */
    uint8_t                   phaseFilterVal; /*!< Filter value (if input filter is enabled)*/
    ftm_quad_phase_polarity_t phasePolarity;  /*!< Phase polarity */
} ftm_phase_params_t;

/*!
 * @brief FTM quadrature configure structure
 *
 * Implements : ftm_quad_decode_config_t_Class
 */
typedef struct
{
    ftm_quad_decode_mode_t mode;         /*!< FTM_QUAD_PHASE_ENCODE or FTM_QUAD_COUNT_AND_DIR */
    uint16_t               initialVal;   /*!< Initial counter value*/
    uint16_t               maxVal;       /*!< Maximum counter value*/
    ftm_phase_params_t     phaseAConfig; /*!< Configuration for the input phase a */
    ftm_phase_params_t     phaseBConfig; /*!< Configuration for the input phase b */
} ftm_quad_decode_config_t;

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/*!
 * @brief Initializes the FTM driver.
 *
 * @param[in] instance The FTM peripheral instance number.
 * @param[in] info The FTM user configuration structure, see #ftm_user_config_t.
 * @param[out] state The FTM state structure of the driver.
 * @return operation status
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t ftm_init(uint32_t instance, const ftm_user_config_t *info, ftm_state_t *state);

/*!
 * @brief Initializes the FTM. This function will enable the flexTimer module
 * and selects one pre-scale factor for the FTM clock source.
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] ftmClockPrescaler The FTM peripheral clock pre-scale divider
 */
void ftm_init_module(FTM_t *const ftmBase, ftm_clock_ps_t ftmClockPrescaler);

/*!
 * @brief Resets the FTM registers. All the register use in the driver should be
 * reset to default value of each register.
 *
 * @param[in] ftmBase The FTM base address pointer
 */
void ftm_reset(FTM_t *const ftmBase);

/*!
 * @brief This function is used to clear the FTM status flags.
 *
 * @param[in] instance The FTM peripheral instance number.
 * @param[in] flagMask The status flags to clear. This is a logical OR of members of the
 *            enumeration ::ftm_status_flag_t
 */
void ftm_clear_status_flags(uint32_t instance, uint32_t flagMask);

/*!
 * @brief Retrieves the frequency of the clock source feeding the FTM counter.
 *
 * Function will return a 0 if no clock source is selected and the FTM counter is disabled
 *
 * @param [in] instance The FTM peripheral instance number.
 * @return The frequency of the clock source running the FTM counter (0 if counter is disabled)
 */
uint32_t ftm_get_frequency(uint32_t instance);

/*!
 * @brief This function configures sync mechanism for some FTM registers (MOD, CNINT, HCR,
 *          CnV, OUTMASK, INVCTRL, SWOCTRL).
 *
 * @param[in] instance The FTM peripheral instance number.
 * @param[in] param The sync configuration structure.
 * @return operation status
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t ftm_set_sync(uint32_t instance, const ftm_pwm_sync_t *param);

/*!
 * @brief Initialize the FTM counter.
 *
 * Starts the FTM counter. This function provides access to the
 * FTM counter settings. The counter can be run in Up counting and Up-down counting modes.
 * To run the counter in Free running mode, choose Up counting option and provide
 * 0x0 value for the initialValue and 0xFFFF for finalValue. Please call this
 * function only when FTM is used as timer/counter. User must call the FTM_DRV_Deinit
 * and the ftm_init to Re-Initialize the FTM before calling ftm_init_counter
 * for the second time and afterwards.
 *
 * @param[in] instance The FTM peripheral instance number.
 * @param[in] timer Timer configuration structure.
 * @return operation status
 *        - STATUS_SUCCESS : Initialized successfully.
 */
status_t ftm_init_counter(uint32_t instance, const ftm_timer_param_t *timer);

/*!
 * @brief Starts the FTM counter.
 *
 * @param[in] instance The FTM peripheral instance number.
 * @return operation status
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t ftm_counter_start(uint32_t instance);

/*!
 * @brief Stops the FTM counter.
 *
 * @param[in] instance The FTM peripheral instance number.
 * @return operation status
 *        - STATUS_SUCCESS : Completed successfully.
 */
status_t ftm_counter_stop(uint32_t instance);

/*!
 * @brief Reads back the current value of the FTM counter.
 *
 * @param[in] instance The FTM peripheral instance number.
 * @return The current counter value
 */
uint32_t ftm_counter_read(uint32_t instance);

/*!
 * @brief This function will get the default configuration values
 *        in the structure which is used as a common use-case.
 * @param[out] config Pointer to the structure in which the
 *             configuration will be saved.
 */
void ftm_mc_get_default_config(ftm_timer_param_t *const config);

/*!
 * @brief This function is used to covert the given frequency to period in ticks
 *
 * @param [in] instance The FTM peripheral instance number.
 * @param [in] freqencyHz Frequency value in Hz.
 *
 * @return The value in ticks of the frequency
 */
uint16_t ftm_convert_freq_to_period_ticks(uint32_t instance, uint32_t freqencyHz);

/*!
 * @brief Enables or disables the generation of the FTM peripheral timer channel trigger when the
 * FTM counter is equal to its initial value
 *
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] channel The FTM peripheral channel number
 * @param[in] enable Enables the generation of the channel trigger
 *                   - true : The generation of the channel trigger is enabled
 *                   - false: The generation of the channel trigger is disabled
 */
void ftm_set_chn_trigger(FTM_t *const ftmBase, uint8_t channel, bool enable);

/*!
 * @brief This function updates the waveform output in PWM mode (duty cycle and phase).
 *
 * @note: Regarding the type of updating PWM in the duty cycle, if the expected duty
 * is 100% then the value that is to be written to hardware will be exceed value of period.
 * It means that the FTM counter will not match the value of the CnV register in this case.
 *
 * @param [in] instance The FTM peripheral instance number.
 * @param [in] channel The channel number. In combined mode, the code finds the channel.
 * @param [in] typeOfUpdate The type of PWM update in the duty cycle/pulse or in ticks.
 * @param [in] firstEdge  Duty cycle or first edge time for PWM mode. Can take value between
 *                       0 - FTM_MAX_DUTY_CYCLE(0 = 0% from period  and FTM_MAX_DUTY_CYCLE = 100%
 * from period) Or value in ticks for the first of the PWM mode in which can have value between 0
 *                       and ftmPeriod is stored in the state structure.
 * @param [in] secondEdge Second edge time - only for combined mode. Can take value
 *                       between 0 - FTM_MAX_DUTY_CYCLE(0 = 0% from period  and FTM_MAX_DUTY_CYCLE =
 * 100% from period). Or value in ticks for the second of the PWM mode in which can have value
 * between 0 and ftmPeriod is stored in the state structure.
 * @param [in] softwareTrigger If true a software trigger is generate to update PWM parameters.
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t ftm_update_pwm_channel(uint32_t                instance,
                                uint8_t                 channel,
                                ftm_pwm_update_option_t typeOfUpdate,
                                uint16_t                firstEdge,
                                uint16_t                secondEdge,
                                bool                    softwareTrigger);

/*!
 * @brief Configures the duty cycle and frequency and starts the output of the PWM on
 * all channels configured in the param structure.
 * The independent channel configuration need to clarify the polarity and safe state as following:
 * - In the first channel, the POL bit is the value of safeState variable. In the second channel,
 * the POL bit is the same value of safeSate with the inverted channel and the POL bit is inverted
 * safeState with the duplicated channel.
 * - If the polarity and sate state are the value, it will be Low-true pulses. It means the
 *ELSB:ELSA = 0:1. Otherwise, it will be High-true pulses. It means the ELSB:ELSA = 1:0. Regarding
 *the combined channel configuration:
 * - In both channels, the POL bit is the same value with the safeState variable
 * - If the polarity and sate state are the value, it will be Low-true pulses. It means the
 *ELSB:ELSA = 0:1. Otherwise, it will be High-true pulses. It means the ELSB:ELSA = 1:0.
 * - COMP bit will be true when the polarity and safeState are the same value, the second channel is
 *inverted .the first channel or when the polarity and safeState are difference value, the second
 *channel is duplicated the first channel.
 * - COMP bit will be false when the polarity and safeState are the same value, the second channel
 *is duplicated .the first channel or when the polarity and safeState are difference value, the
 *second channel is inverted the first channel.
 *
 * @note: These configuration will impact to the FTM_DRV_SetSoftwareOutputChannelControl and
 *FTM_DRV_SetAllChnSoftwareOutputControl function. Because the software output control behavior
 *depends on the polarity and COMP bit.
 *
 * @param [in] instance The FTM peripheral instance number.
 * @param [in] param FTM driver PWM parameter to configure PWM options.
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t ftm_init_pwm(uint32_t instance, const ftm_pwm_param_t *param);

/*!
 * @brief This function configures the channel in the Input Capture mode for either getting
 * time-stamps on edge detection or on signal measurement. When the edge specified in the
 * captureMode argument occurs on the channel and then the FTM counter is captured into the CnV
 * register. The user have to read the CnV register separately to get this value. The filter
 * function is disabled if the filterVal argument passed as 0. The filter feature.
 * is available only on channels 0,1,2,3.
 *
 * @param [in] instance The FTM peripheral instance number.
 * @param [in] param Configuration of the input capture channel.
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t ftm_init_input_capture(uint32_t instance, const ftm_input_param_t *param);

/*!
 * @brief  Disables input capture mode and clears FTM timer configuration
 *
 * @param [in] instance The FTM peripheral instance number.
 * @param [in] param Configuration of the output compare channel.
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t ftm_deinit_input_capture(uint32_t instance, const ftm_input_param_t *param);

/*!
 * @brief Sets the FTM peripheral timer channel input capture filter value.
 * @param[in] ftmBase The FTM base address pointer
 * @param[in] channel The FTM peripheral channel number, only 0,1,2,3, channel 4, 5,6, 7 don't have
 * @param[in] value Filter value to be set
 */
void ftm_set_chn_input_capture_filter(FTM_t *const ftmBase, uint8_t channel, uint8_t value);

/*!
 * @brief Configures the FTM to generate timed pulses (Output compare mode).
 *
 * When the FTM counter matches the value of CnV, the channel output is changed based on what is
 * specified in the mode argument. The signal period can be modified using
 * param->maxCountValue. After this function when the max counter value and CnV are equal.
 * FTM_DRV_UpdateOutputCompareChannel function can be used to change CnV value.
 *
 * @param [in] instance The FTM peripheral instance number.
 * @param [in] param configuration of the output compare channels
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t ftm_init_output_compare(uint32_t instance, const ftm_output_cmp_param_t *param);

/*!
 * @brief  Disables compare match output control and clears FTM timer configuration
 *
 * @param [in] instance The FTM peripheral instance number.
 * @param [in] param Configuration of the output compare channel
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t ftm_deinit_output_compare(uint32_t instance, const ftm_output_cmp_param_t *param);

/*!
 * @brief Configures the quadrature mode and starts measurement
 *
 * @param [in] instance Instance number of the FTM module.
 * @param [in] config   Configuration structure(quadrature decode mode, polarity for both phases,
 *                      initial and maximum value for the counter, filter configuration).
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t ftm_quad_decode_start(uint32_t instance, const ftm_quad_decode_config_t *config);

/*!
 * @brief De-activates the quadrature decode mode.
 *
 * @param [in] instance Instance number of the FTM module.
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t ftm_quad_decode_stop(uint32_t instance);

#ifdef __cplusplus
}
#endif

#endif /* FTM_DRIVER_H */
