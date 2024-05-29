/**
 * @file clock_cw900x.h
 * @brief Header file for the cw900x clock driver.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef CLOCK_GATING_H
#define CLOCK_GATING_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "device_registers.h"
#include "status.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/** @brief TClk clock frequency. */
#define NUMBER_OF_TCLK_INPUTS 3U

/** @brief The maximum number of system clock dividers and system clock divider
 * indexes. */
#define CORE_CLK_INDEX 0U
#define SLOW_CLK_INDEX 1U
#define SYS_CLK_MAX_NO 2U

/**
 * @brief SIM CLK32KSEL clock source select
 * Implements: sim_rtc_clk_sel_src_t_Class
 */
typedef enum
{
    SIM_RTCCLK_SEL_SOSCDIV1_CLK = 0x0U, /**< SOSCDIV1 clock          */
    SIM_RTCCLK_SEL_LPO_32K      = 0x1U, /**< 32 kHz LPO clock        */
    SIM_RTCCLK_SEL_RTC_CLKIN    = 0x2U, /**< RTC_CLKIN clock         */
    SIM_RTCCLK_SEL_FIRCDIV1_CLK = 0x3U, /**< FIRCDIV1 clock          */
} sim_rtc_clk_sel_src_t;

/**
 * @brief SIM LPOCLKSEL clock source select
 * Implements: sim_lpoclk_sel_src_t_Class
 */
typedef enum
{
    SIM_LPO_CLK_SEL_LPO_128K = 0x0, /**< 128 kHz LPO clock */
    SIM_LPO_CLK_SEL_NO_CLOCK = 0x1, /**< No clock */
    SIM_LPO_CLK_SEL_LPO_32K  = 0x2, /**< 32 kHz LPO clock */
    SIM_LPO_CLK_SEL_LPO_1K   = 0x3, /**< 1 kHz LPO clock*/
} sim_lpoclk_sel_src_t;

/**
 * @brief SIM CLKOUT select
 * Implements: sim_clkout_src_t_Class
 */
typedef enum
{
    SIM_CLKOUT_SEL_SYSTEM_SCG_CLKOUT    = 0U,  /**< SCG CLKOUT  */
    SIM_CLKOUT_SEL_SYSTEM_SOSC_DIV2_CLK = 2U,  /**< SOSC DIV2 CLK  */
    SIM_CLKOUT_SEL_SYSTEM_SIRC_DIV2_CLK = 4U,  /**< SIRC DIV2 CLK  */
    SIM_CLKOUT_SEL_SYSTEM_FIRC_DIV2_CLK = 6U,  /**< FIRC DIV2 CLK  */
    SIM_CLKOUT_SEL_SYSTEM_HCLK          = 7U,  /**< HCLK  */
    SIM_CLKOUT_SEL_SYSTEM_SPLL_DIV2_CLK = 8U,  /**< SPLL DIV2 CLK  */
    SIM_CLKOUT_SEL_SYSTEM_BUS_CLK       = 9U,  /**< BUS_CLK  */
    SIM_CLKOUT_SEL_SYSTEM_LPO_128K_CLK  = 10U, /**< LPO_CLK 128 Khz */
    SIM_CLKOUT_SEL_SYSTEM_LPO_CLK       = 12U, /**< LPO_CLK as selected by SIM LPO CLK Select    */
    SIM_CLKOUT_SEL_SYSTEM_RTC_CLK       = 14U, /**< RTC CLK as selected by SIM CLK 32 KHz Select */
} sim_clkout_src_t;

/**
 * @brief SIM CLKOUT divider
 * Implements: sim_clkout_div_t_Class
 */
typedef enum
{
    SIM_CLKOUT_DIV_BY_1 = 0x0U, /**< Divided by 1 */
    SIM_CLKOUT_DIV_BY_2 = 0x1U, /**< Divided by 2 */
    SIM_CLKOUT_DIV_BY_3 = 0x2U, /**< Divided by 3 */
    SIM_CLKOUT_DIV_BY_4 = 0x3U, /**< Divided by 4 */
    SIM_CLKOUT_DIV_BY_5 = 0x4U, /**< Divided by 5 */
    SIM_CLKOUT_DIV_BY_6 = 0x5U, /**< Divided by 6 */
    SIM_CLKOUT_DIV_BY_7 = 0x6U, /**< Divided by 7 */
    SIM_CLKOUT_DIV_BY_8 = 0x7U, /**< Divided by 8 */
} sim_clkout_div_t;

/**
 * @brief SIM ClockOut configuration.
 * Implements: sim_clock_out_config_t_Class
 */
typedef struct
{
    bool             b_initialize; /**< Initialize or not the ClockOut clock.  */
    bool             b_enable;     /**< SIM ClockOut enable.                   */
    sim_clkout_src_t source;       /**< SIM ClockOut source select.            */
    sim_clkout_div_t divider;      /**< SIM ClockOut divide ratio.             */
} sim_clock_out_config_t;

/**
 * @brief SIM LPO Clocks configuration.
 * Implements: sim_lpo_clock_config_t_Class
 */
typedef struct
{
    bool                  b_initialize;    /**< Initialize or not the LPO clock. */
    sim_rtc_clk_sel_src_t source_rtc_clk;  /**< RTC_CLK source select. */
    sim_lpoclk_sel_src_t  source_lpo_clk;  /**< LPO clock source select. */
    bool                  b_enable_lpo32k; /**< MSCM Clock Gating Control enable. */
    bool                  b_enable_lpo1k;  /**< MSCM Clock Gating Control enable. */
} sim_lpo_clock_config_t;

/**
 * @brief SIM  Platform Gate Clock configuration.
 * Implements: sim_tclk_config_t_Class
 */
typedef struct
{
    bool     b_initialize;                    /**< Initialize or not the TCLK clock. */
    uint32_t tclkFreq[NUMBER_OF_TCLK_INPUTS]; /**< TCLKx frequency. */
    uint32_t extPinSrc[FTM_INSTANCE_COUNT];   /**< FTMx frequency.  */
} sim_tclk_config_t;

/**
 * @brief Debug trace clock source select
 * Implements: clock_trace_src_t_Class
 */
typedef enum
{
    CLOCK_TRACE_SRC_CORE_CLK = 0x0 /**< core clock */
} clock_trace_src_t;

/**
 * @brief SIM configure structure.
 * Implements: sim_clock_config_t_Class
 */
typedef struct
{
    sim_clock_out_config_t clock_out_config; /**< Clock Out configuration. */
    sim_lpo_clock_config_t lpo_clock_config; /**< Low Power Clock configuration. */
    sim_tclk_config_t      tclk_config;      /**< TCLK, FTM option Clock configuration. */
} sim_clock_config_t;

/**
 * @brief SCG system clock source.
 * Implements: scg_system_clock_src_t_Class
 */
typedef enum
{
    SCG_SYSTEM_CLOCK_SRC_SYS_OSC = 1U,  /**< System OSC. */
    SCG_SYSTEM_CLOCK_SRC_SIRC    = 2U,  /**< Slow IRC.   */
    SCG_SYSTEM_CLOCK_SRC_FIRC    = 3U,  /**< Fast IRC.   */
    SCG_SYSTEM_CLOCK_SRC_NONE    = 255U /**< MAX value.  */
} scg_system_clock_src_t;

/**
 * @brief SCG system clock divider value.
 * Implements: scg_system_clock_div_t_Class
 */
typedef enum
{
    SCG_SYSTEM_CLOCK_DIV_BY_1  = 0U,  /**< Divided by 1. */
    SCG_SYSTEM_CLOCK_DIV_BY_2  = 1U,  /**< Divided by 2. */
    SCG_SYSTEM_CLOCK_DIV_BY_3  = 2U,  /**< Divided by 3. */
    SCG_SYSTEM_CLOCK_DIV_BY_4  = 3U,  /**< Divided by 4. */
    SCG_SYSTEM_CLOCK_DIV_BY_5  = 4U,  /**< Divided by 5. */
    SCG_SYSTEM_CLOCK_DIV_BY_6  = 5U,  /**< Divided by 6. */
    SCG_SYSTEM_CLOCK_DIV_BY_7  = 6U,  /**< Divided by 7. */
    SCG_SYSTEM_CLOCK_DIV_BY_8  = 7U,  /**< Divided by 8. */
    SCG_SYSTEM_CLOCK_DIV_BY_9  = 8U,  /**< Divided by 9. */
    SCG_SYSTEM_CLOCK_DIV_BY_10 = 9U,  /**< Divided by 10. */
    SCG_SYSTEM_CLOCK_DIV_BY_11 = 10U, /**< Divided by 11. */
    SCG_SYSTEM_CLOCK_DIV_BY_12 = 11U, /**< Divided by 12. */
    SCG_SYSTEM_CLOCK_DIV_BY_13 = 12U, /**< Divided by 13. */
    SCG_SYSTEM_CLOCK_DIV_BY_14 = 13U, /**< Divided by 14. */
    SCG_SYSTEM_CLOCK_DIV_BY_15 = 14U, /**< Divided by 15. */
    SCG_SYSTEM_CLOCK_DIV_BY_16 = 15U, /**< Divided by 16. */
} scg_system_clock_div_t;

/**
 * @brief SCG system clock configuration.
 * Implements: scg_system_clock_config_t_Class
 */
typedef struct
{
    scg_system_clock_div_t div_slow; /**< Slow clock divider.      */
    scg_system_clock_div_t div_core; /**< Core clock divider.      */
    scg_system_clock_src_t src;      /**< System clock source.     */
} scg_system_clock_config_t;

/**
 * @name SCG Clockout.
 * @{
 */

/**
 * @brief SCG ClockOut type.
 * Implements: scg_clockout_src_t_Class
 */
typedef enum
{
    SCG_CLOCKOUT_SRC_SCG_SLOW = 0U, /**< SCG SLOW.   */
    SCG_CLOCKOUT_SRC_SOSC     = 1U, /**< System OSC. */
    SCG_CLOCKOUT_SRC_SIRC     = 2U, /**< Slow IRC.   */
    SCG_CLOCKOUT_SRC_FIRC     = 3U, /**< Fast IRC.   */
    SCG_CLOCKOUT_SRC_SPLL     = 6U, /**< System PLL. */
} scg_clockout_src_t;
/** @} */

/**
 * @brief SCG asynchronous clock divider value.
 * Implements: scg_async_clock_div_t_Class
 */
typedef enum
{
    SCG_ASYNC_CLOCK_DISABLE   = 0U, /**< Clock output is disabled.  */
    SCG_ASYNC_CLOCK_DIV_BY_1  = 1U, /**< Divided by 1.              */
    SCG_ASYNC_CLOCK_DIV_BY_2  = 2U, /**< Divided by 2.              */
    SCG_ASYNC_CLOCK_DIV_BY_4  = 3U, /**< Divided by 4.              */
    SCG_ASYNC_CLOCK_DIV_BY_8  = 4U, /**< Divided by 8.              */
    SCG_ASYNC_CLOCK_DIV_BY_16 = 5U, /**< Divided by 16.             */
    SCG_ASYNC_CLOCK_DIV_BY_32 = 6U, /**< Divided by 32.             */
    SCG_ASYNC_CLOCK_DIV_BY_64 = 7U  /**< Divided by 64.             */
} scg_async_clock_div_t;

/**
 * @brief SCG system OSC configuration.
 * Implements: scg_sosc_config_t_Class
 */
typedef struct
{
    uint32_t              freq;         /**< System OSC frequency. */
    scg_async_clock_div_t div1;         /**< Asynchronous peripheral source. */
    scg_async_clock_div_t div2;         /**< Asynchronous peripheral source. */
    bool                  b_initialize; /**< Initialize or not the System OSC module. */
} scg_sosc_config_t;

/**
 * @brief SCG slow IRC clock configuration.
 * Implements: scg_sirc_config_t_Class
 */
typedef struct
{
    scg_async_clock_div_t div1;         /**< Asynchronous peripheral source. */
    scg_async_clock_div_t div2;         /**< Asynchronous peripheral source. */
    bool                  b_initialize; /**< Initialize or not the SIRC module. */
} scg_sirc_config_t;

/**
 * @brief SCG fast IRC clock configuration.
 * Implements: scg_firc_config_t_Class
 */
typedef struct
{
    scg_async_clock_div_t div1;         /**< Asynchronous peripheral source. */
    scg_async_clock_div_t div2;         /**< Asynchronous peripheral source. */
    bool                  b_initialize; /**< Initialize or not the FIRC module. */
} scg_firc_config_t;

/**
 * @brief SCG system PLL predivider.
 */
typedef enum
{
    SCG_SPLL_CLOCK_PREDIV_BY_1 = 0U,
    SCG_SPLL_CLOCK_PREDIV_BY_2 = 1U,
    SCG_SPLL_CLOCK_PREDIV_BY_3 = 2U,
    SCG_SPLL_CLOCK_PREDIV_BY_4 = 3U,
    SCG_SPLL_CLOCK_PREDIV_BY_5 = 4U,
    SCG_SPLL_CLOCK_PREDIV_BY_6 = 5U,
    SCG_SPLL_CLOCK_PREDIV_BY_7 = 6U,
    SCG_SPLL_CLOCK_PREDIV_BY_8 = 7U
} scg_spll_clock_prediv_t;

/**
 * @brief SCG system PLL multiplier.
 */
typedef enum
{
    SCG_SPLL_CLOCK_MULTIPLY_BY_16 = 0U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_17 = 1U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_18 = 2U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_19 = 3U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_20 = 4U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_21 = 5U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_22 = 6U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_23 = 7U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_24 = 8U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_25 = 9U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_26 = 10U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_27 = 11U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_28 = 12U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_29 = 13U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_30 = 14U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_31 = 15U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_32 = 16U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_33 = 17U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_34 = 18U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_35 = 19U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_36 = 20U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_37 = 21U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_38 = 22U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_39 = 23U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_40 = 24U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_41 = 25U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_42 = 26U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_43 = 27U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_44 = 28U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_45 = 29U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_46 = 30U,
    SCG_SPLL_CLOCK_MULTIPLY_BY_47 = 31U
} scg_spll_clock_multiply_t;

/**
 * @brief SCG system PLL configuration.
 * Implements: scg_spll_config_t_Class
 */
typedef struct
{
    scg_async_clock_div_t div1;         /**< Asynchronous peripheral source. */
    scg_async_clock_div_t div2;         /**< Asynchronous peripheral source. */
    bool                  b_initialize; /**< Initialize or not the System PLL module. */
} scg_spll_config_t;

/**
 * @brief SCG RTC configuration.
 * Implements: scg_rtc_config_t_Class
 */
typedef struct
{
    uint32_t rtc_clk_in_freq; /**< RTC_CLKIN frequency. */
    bool     b_initialize;    /**< Initialize or not the RTC. */
} scg_rtc_config_t;

/**
 * @brief SCG Clock Mode Configuration structure.
 * Implements: scg_clock_mode_config_t_Class
 */
typedef struct
{
    scg_system_clock_config_t rccr_config;     /**< Run Clock Control configuration.   */
    scg_system_clock_config_t vccr_config;     /**< VLPR Clock Control configuration.  */
    scg_system_clock_config_t hccr_config;     /**< HSRUN Clock Control configuration. */
    scg_system_clock_src_t    alternate_clock; /**< Alternate clock used during initialization */
    bool                      b_initialize; /**< Initialize or not the Clock Mode Configuration */
} scg_clock_mode_config_t;

/**
 * @brief SCG ClockOut Configuration structure.
 * Implements: scg_clockout_config_t_Class
 */
typedef struct
{
    scg_clockout_src_t source;       /**< ClockOut source select. */
    bool               b_initialize; /**< Initialize or not the ClockOut. */
} scg_clockout_config_t;

/**
 * @brief SCG configure structure.
 * Implements: scg_config_t_Class
 */
typedef struct
{
    scg_sirc_config_t       sirc_config;       /**< Slow internal reference clock configuration. */
    scg_firc_config_t       firc_config;       /**< Fast internal reference clock configuration. */
    scg_sosc_config_t       sosc_config;       /**< System oscillator configuration. */
    scg_spll_config_t       spll_config;       /**< System Phase locked loop configuration. */
    scg_rtc_config_t        rtc_config;        /**< Real Time Clock configuration. */
    scg_clockout_config_t   clock_out_config;  /**< SCG ClockOut Configuration. */
    scg_clock_mode_config_t clock_mode_config; /**< SCG Clock Mode Configuration. */
} scg_config_t;

/** @brief PCC clock source select
 *  Implements: peripheral_clock_source_t_Class
 */
typedef uint8_t peripheral_clock_source_t;

#define CLK_SRC_OFF       0x00U /**< Clock is off */
#define CLK_SRC_SOSC      0x01U /**< OSCCLK - System Oscillator Bus Clock */
#define CLK_SRC_SIRC      0x02U /**< SCGIRCLK - Slow IRC Clock */
#define CLK_SRC_FIRC      0x03U /**< SCGFIRCLK - Fast IRC Clock */
#define CLK_SRC_SPLL      0x06U /**< SCGPCLK System PLL clock */
#define CLK_SRC_SOSC_DIV1 0x01U /**< OSCCLK - System Oscillator Bus Clock */
#define CLK_SRC_SIRC_DIV1 0x02U /**< SCGIRCLK - Slow IRC Clock */
#define CLK_SRC_FIRC_DIV1 0x03U /**< SCGFIRCLK - Fast IRC Clock */
#define CLK_SRC_SPLL_DIV1 0x06U /**< SCGPCLK System PLL clock */
#define CLK_SRC_SOSC_DIV2 0x01U /**< OSCCLK - System Oscillator Bus Clock */
#define CLK_SRC_SIRC_DIV2 0x02U /**< SCGIRCLK - Slow IRC Clock */
#define CLK_SRC_FIRC_DIV2 0x03U /**< SCGFIRCLK - Fast IRC Clock */
#define CLK_SRC_SPLL_DIV2 0x06U /**< SCGPCLK System PLL clock */
#ifdef FEATURE_HAS_LPO_PERIPHERAL_CLOCK_SOURCE
#define CLK_SRC_LPO 0x07U       /**< LPO clock */
#endif

/** SPLL output clock sourece */
#define SPLL_CLKOUT_96M  0x00 /**< SPLL_CLK - 96MHz  */
#define SPLL_CLKOUT_120M 0x01 /**< SPLL_CLK - 120MHz  */

/** PUF engine clock sourece */
#define PUF_CLK_OFF       0x00 /**< clock sourece - Disable     */
#define PUF_CLK_SPLLDIV3  0x01 /**< clock sourece - SPLL_CLK/3  */
#define PUF_CLK_SPLLDIV1  0x02 /**< clock sourece - SPLL_CLK/1  */
#define PUF_CLK_SPLLDIV2  0x03 /**< clock sourece - SPLL_CLK/2  */
#define PUF_CLK_FIRC_DIV1 0x04 /**< clock sourece - FIRCDIV1_CLK  */
#define PUF_CLK_SIRC_DIV1 0x05 /**< clock sourece - SIRCDIV1_CLK  */
#define PUF_CLK_SPLL_DIV2 0x06 /**< clock sourece - SPLLDIV2_CLK  */
#define PUF_CLK_SPLL_DIV1 0x07 /**< clock sourece - SPLLDIV1_CLK  */

/** @brief PCC fractional value select
 *  Implements: peripheral_clock_frac_t_Class
 */
typedef enum
{
    MULTIPLY_BY_ONE = 0x00U, /**< Fractional value is zero */
    MULTIPLY_BY_TWO = 0x01U  /**< Fractional value is one */
} peripheral_clock_frac_t;

/** @brief PCC divider value select
 *  Implements: peripheral_clock_divider_t_Class
 */
typedef enum
{
    DIVIDE_BY_ONE   = 0x00U, /**< Divide by 1 (pass-through, no clock divide) */
    DIVIDE_BY_TWO   = 0x01U, /**< Divide by 2 */
    DIVIDE_BY_THREE = 0x02U, /**< Divide by 3 */
    DIVIDE_BY_FOUR  = 0x03U, /**< Divide by 4 */
    DIVIDE_BY_FIVE  = 0x04U, /**< Divide by 5 */
    DIVIDE_BY_SIX   = 0x05U, /**< Divide by 6 */
    DIVIDE_BY_SEVEN = 0x06U, /**< Divide by 7 */
    DIVIDE_BY_EIGTH = 0x07U  /**< Divide by 8 */
} peripheral_clock_divider_t;

/** @brief PCC peripheral instance clock configuration.
 *  Implements: peripheral_clock_config_t_Class
 */
typedef struct
{
    clock_names_t              clk_name;
    bool                       b_clk_gate; /**< Peripheral clock gate.   */
    peripheral_clock_source_t  clk_src;    /**< Peripheral clock source. */
    peripheral_clock_frac_t    frac;       /**< Peripheral clock fractional value. */
    peripheral_clock_divider_t divider;    /**< Peripheral clock divider value. */
} peripheral_clock_config_t;

/** @brief PCC configuration.
 *  Implements: pcc_config_t_Class
 */
typedef struct
{
    uint32_t                   count;               /**< Number of peripherals to be configured. */
    peripheral_clock_config_t *p_peripheral_clocks; /**< Pointer to the peripheral clock
                                                    configurations array. */
} pcc_config_t;

/**
 * @brief Clock configuration structure.
 * Implements: clock_manager_user_config_t_Class
 */
typedef struct
{
    scg_config_t       scg_config; /**< SCG Clock configuration. */
    sim_clock_config_t sim_config; /**< SIM Clock configuration. */
    pcc_config_t       pcc_config; /**< PCC Clock configuration. */
} clock_manager_user_config_t;

/**
 * @brief Power mode.
 * Implements: pwr_modes_t_Class
 */
typedef enum
{
    NO_MODE    = 0U,
    RUN_MODE   = (1U << 0U),
    VLPR_MODE  = (1U << 1U),
    HSRUN_MODE = (1U << 2U),
    STOP_MODE  = (1U << 3U),
    VLPS_MODE  = (1U << 4U),
    ALL_MODES  = 0x7FFFFFFF
} pwr_modes_t;

/**
 * @brief XOSC reference clock select (internal oscillator is bypassed or not)
 * Implements: xosc_ref_t_Class
 */
typedef enum
{
    XOSC_EXT_REF = 0U, /**< Internal oscillator is bypassed, external reference clock requested. */
    XOSC_INT_OSC = 1U, /**< Internal oscillator of XOSC requested. */
} xosc_ref_t;

/** @brief module clock configuration.
 *  Implements: module_clk_config_t_Class
 */
typedef struct
{
    bool          b_gating; /**< Clock gating. */
    clock_names_t source;   /**< Clock source input (some modules don't have protocol clock) */
    uint16_t      mul;      /**< Multiplier (some modules don't have fractional) */
    uint16_t      div;      /**< Divider (some modules don't have divider) */

} module_clk_config_t;

/**
 * @brief System clock configuration.
 * Implements: sys_clk_config_t_Class
 */
typedef struct
{
    clock_names_t src;                 /**< System clock source. */
    uint16_t dividers[SYS_CLK_MAX_NO]; /**< System clock dividers. Value by which system clock is
                                          divided. 0 means that system clock is not divided. */
} sys_clk_config_t;

/**
 * @brief Clock source configuration.
 * Implements: clock_source_config_t_Class
 */
typedef struct
{
    bool       b_enable;  /**< Enable/disable clock source. */
    xosc_ref_t ref_clk;   /**< Bypass option. It applies to external oscillator clock sources */
    uint32_t   ref_freq;  /**< Frequency of the input reference clock. It
                            applies to external oscillator clock sources */
    uint16_t mul;         /**< Multiplier. It applies to PLL clock sources. */
    uint16_t div;         /**< Divider. It applies to PLL clock sources. */

    uint16_t output_div1; /**< First output divider. It's used as protocol clock
                           * by modules. Zero means that divider is disabled.   /
                           *   Possible values 0(disabled), 1, 2, 4, 8, 16, 32,
                           * 64; all the other values are not valid. /
                           */
    uint16_t output_div2; /**< Second output divider. It's used as protocol clock
                           * by modules. Zero means that divider is disabled.   /
                           *   Possible values 0(disabled), 1, 2, 4, 8, 16, 32,
                           * 64; all the other values are not valid. /
                           */
} clock_source_config_t;

/**
 * @brief The clock notification type.
 * Implements: clock_manager_notify_t_Class
 */
typedef enum
{
    CLOCK_MANAGER_NOTIFY_RECOVER = 0x00U, /**< Notify IP to recover to previous work state.      */
    CLOCK_MANAGER_NOTIFY_BEFORE  = 0x01U, /**< Notify IP that system will change clock setting.  */
    CLOCK_MANAGER_NOTIFY_AFTER   = 0x02U, /**< Notify IP that have changed to new clock setting. */
} clock_manager_notify_t;

/**
 * @brief The callback type, indicates what kinds of notification this callback handles.
 * Implements: clock_manager_callback_type_t_Class
 */
typedef enum
{
    CLOCK_MANAGER_CALLBACK_BEFORE = 0x01U, /**< Callback handles BEFORE notification.          */
    CLOCK_MANAGER_CALLBACK_AFTER  = 0x02U, /**< Callback handles AFTER notification.           */
    CLOCK_MANAGER_CALLBACK_BEFORE_AFTER = 0x03U /**<Callback handles BEFORE and AFTER notification*/
} clock_manager_callback_type_t;

/**
 * @brief Clock transition policy.
 * Implements: clock_manager_policy_t_Class
 */
typedef enum
{
    CLOCK_MANAGER_POLICY_AGREEMENT, /**< Clock transfers gracefully. */
    CLOCK_MANAGER_POLICY_FORCIBLE   /**< Clock transfers forcefully. */
} clock_manager_policy_t;

/**
 * @brief Clock notification structure passed to clock callback function.
 * Implements: clock_notify_struct_t_Class
 */
typedef struct
{
    uint8_t                target_clock_config_index; /**< Target clock configuration index. */
    clock_manager_policy_t policy;                    /**< Clock transition policy. */
    clock_manager_notify_t notify_type;               /**< Clock notification type. */
} clock_notify_struct_t;

/**
 * @brief Type of clock callback functions.
 */
typedef status_t (*clock_manager_callback_t)(clock_notify_struct_t *p_notify,
                                             void                  *p_callback_data);
/**
 * @brief Structure for callback function and its parameter.
 * Implements: clock_manager_callback_user_config_t_Class
 */
typedef struct
{
    clock_manager_callback_t      callback;        /**< Entry of callback function. */
    clock_manager_callback_type_t callback_type;   /**< Callback type. */
    void                         *p_callback_data; /**< Parameter of callback function. */
} clock_manager_callback_user_config_t;

/**
 * @brief Clock manager state structure.
 * Implements: clock_manager_state_t_Class
 */
typedef struct
{
    clock_manager_user_config_t const **pp_config_table;  /**< Pointer to clock configure table.*/
    uint8_t                             clock_config_num; /**< Number of clock configurations.  */
    uint8_t                             cur_config_index; /**< Index of current configuration.  */
    clock_manager_callback_user_config_t **pp_callback_config; /**< Pointer to callback table. */
    uint8_t                                callback_num;       /**< Number of clock callbacks. */
    uint8_t error_callback_index; /**< Index of callback returns error. */
} clock_manager_state_t;

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
typedef clock_manager_user_config_t clock_user_config_t;

/*******************************************************************************
 * Exported variables
 ******************************************************************************/
/** @brief Peripheral features list
 *         Constant array storing the mappings between clock names of the
 * peripherals and feature lists.
 */
extern const uint8_t g_peripheral_features_list[CLOCK_NAME_COUNT];

extern uint32_t g_tclk_freq[NUMBER_OF_TCLK_INPUTS]; /**< TCLKx clocks */

/** @brief EXTAL0 clock frequency. */
extern uint32_t g_xtal0_clk_freq;

/** @brief RTC_CLKIN clock frequency. */
extern uint32_t g_rtc_clk_in_freq;

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Gets the clock frequency for a specific clock name.
 *
 * This function checks the current clock configurations and then calculates
 * the clock frequency for a specific clock name defined in clock_names_t.
 * Clock modules must be properly configured before using this function.
 * See features.h for supported clock names for different chip families.
 * The returned value is in Hertz. If it cannot find the clock name
 * or the name is not supported for a specific chip family, it returns an
 * STATUS_UNSUPPORTED. If frequency is required for a peripheral and the
 * module is not clocked, then STATUS_MCU_GATED_OFF status is returned.
 * Frequency is returned if a valid address is provided. If frequency is
 * required for a peripheral that doesn't support protocol clock, the zero
 * value is provided.
 *
 * @param[in] clock_name Clock names defined in clock_names_t
 * @param[out] p_frequency Returned clock frequency value in Hertz
 * @return status  Error code defined in status_t
 */
status_t clock_get_freq(clock_names_t clock_name, uint32_t *p_frequency);

/**
 * @brief Set clock configuration according to pre-defined structure.
 *
 * This function sets system to target clock configuration; It sets the
 * clock modules registers for clock mode change.
 *
 * @param[in] p_config  Pointer to configuration structure.
 * @return Error code.
 * @note If external clock is used in the target mode, please make sure it is
 * enabled, for example, if the external oscillator is used, please setup
 * correctly.
 * @note If the configuration structure is NULL, the function will set a
 * default configuration for clock.
 */
status_t clock_init(clock_user_config_t const *p_config);

/**
 * @brief Configures module clock
 *
 * This function configures a module clock according to the configuration.
 * If no configuration is provided (p_module_clk_config is null), then a default
 * one is used p_module_clk_config must be passed as null when module doesn't
 * support protocol clock.
 *
 * @param[in] peripheral_clock   Clock name of the configured module clock
 * @param[in] p_module_clk_config   Pointer to the configuration structure.
 */
void clock_set_module_clock(clock_names_t              peripheral_clock,
                            const module_clk_config_t *p_module_clk_config);

/**
 * @brief Configures the system clocks.
 *
 * This function configures the system clocks (core, bus and flash clocks)
 * in the specified power mode. If no power mode is specified (null parameter)
 * then it is the current power mode.
 *
 * @param[in] p_mode  Pointer to power mode for which the configured system clocks apply
 * @param[in] p_sys_clk_config  Pointer to the system clocks configuration structure.
 */
status_t clock_set_system_clock(const pwr_modes_t      *p_mode,
                                const sys_clk_config_t *p_sys_clk_config);

/**
 * @brief Gets the system clock source.
 *
 * This function gets the current system clock source.
 *
 * @param[out] p_sys_clk_config of the current system clock source.
 */
void clock_get_system_clock_source(sys_clk_config_t *p_sys_clk_config);

/**
 * @brief This function configures a clock source.
 *
 * The clock source is configured based on the provided configuration.
 * All values from the previous configuration of clock source are
 * overwritten. If no configuration is provided, then a default one is used.
 *
 * @param[in] clock_source  Clock name of the configured clock source
 * @param[in] p_clk_src_config Pointer to the configuration structure
 * @return Status of clock source initialization
 */
status_t clock_set_clock_source(clock_names_t                clock_source,
                                const clock_source_config_t *p_clk_src_config);

/**
 * @brief Install pre-defined clock configurations.
 *
 * This function installs the pre-defined clock configuration table to
 * clock manager.
 *
 * @param[in] pp_clock_configs_ptr Pointer to the clock configuration table.
 * @param[in] configs_number Number of clock configurations in table.
 * @param[in] pp_callbacks_ptr Pointer to the callback configuration table.
 * @param[in] callbacks_number Number of callback configurations in table.
 *
 * @return Error code.
 */
status_t clock_init_config(clock_manager_user_config_t const    **pp_clock_configs_ptr,
                           uint8_t                                configs_number,
                           clock_manager_callback_user_config_t **pp_callbacks_ptr,
                           uint8_t                                callbacks_number);

/**
 * @brief Set system clock configuration according to pre-defined structure.
 *
 * This function sets system to target clock configuration; before
 * transition, clock manager will send notifications to all drivers
 * registered to the callback table.  When graceful policy is used, if some
 * drivers are not ready to change, clock transition will not occur, all
 * drivers still work in previous configuration and error is returned. When
 * forceful policy is used, all drivers should stop work and system changes
 * to new clock configuration. The function should be called only on run
 * mode.
 *
 * @param[in] target_config_index Index of the clock configuration.
 * @param[in] policy Transaction policy, graceful or forceful.
 *
 * @return Error code.
 *
 * @note If external clock is used in the target mode, please make sure it
 * is enabled, for example, if the external oscillator is used, please setup
 * EREFS/HGO correctly and make sure OSCINIT is set.
 */
status_t clock_update_config(uint8_t target_config_index, clock_manager_policy_t policy);

/**
 * @brief Get current system clock configuration.
 *
 * @return Current clock configuration index.
 */
uint8_t clock_get_current_config(void);

/**
 * @brief Get the callback which returns error in last clock switch.
 *
 * When graceful policy is used, if some IP is not ready to change clock
 * setting, the callback will return error and system stay in current
 * configuration. Applications can use this function to check which
 * IP callback returns error.
 *
 * @return Pointer to the callback which returns error.
 */
clock_manager_callback_user_config_t *clock_get_error_callback(void);

/**
 * @brief Get the callback which returns error in last clock switch.
 *
 * Switch PCC gating and await it stable.
 *
 * @param[in] clock_name   Clock name of the configured module clock
 */
void clock_switch_pcc_freq_await_stable(clock_names_t clock_name);

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_GATING_H */

/*** end of file ***/
