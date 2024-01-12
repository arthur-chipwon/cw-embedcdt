/**
 * @file clock_gating.c
 * @brief This file provides access to the cw900x clock driver.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stddef.h> /* This header is included for bool type */
#include "device_registers.h"
#include "clock_gating.h"
#include "scg_access.h"
#include "pcc_access.h"
#include "smc_access.h"
#include "sim_access.h"
#include "interrupt_manager.h"

/*******************************************************************************
 * Private macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/**
 * @brief LPO 128K fixed clock frequency.
 */
#define LPO_128K_FREQUENCY 128000UL

/**
 * @brief LPO 32K fixed clock frequency.
 */
#define LPO_32K_FREQUENCY 32000UL

/**
 * @brief LPO 1K fixed clock frequency.
 */
#define LPO_1K_FREQUENCY 1000UL

/**
 * @brief Running modes.
 */
#define HIGH_SPEED_RUNNING_MODE (1UL << 7U)
#define RUN_SPEED_RUNNING_MODE  (1UL << 0U)
#define VLPR_SPEED_RUNNING_MODE (1UL << 2U)

#define MODES_MAX_NO 7U

#define CLOCK_MAX_FREQ_VLPR_MODE                                     \
    {                               /* SYS_CLK  SLOW_CLK */          \
        {0UL, 0UL},                 /**< Invalid entry */            \
            {4000000UL, 1000000UL}, /**< SOSC maximum frequencies */ \
            {4000000UL, 1000000UL}, /**< SIRC maximum frequencies */ \
            {4000000UL, 1000000UL}, /**< FIRC maximum frequencies */ \
            {0UL, 0UL},             /**< Invalid entry */            \
            {0UL, 0UL},             /**< Invalid entry */            \
            {4000000UL, 1000000UL}, /**< SPLL maximum frequencies */ \
    }

#define CLOCK_MAX_FREQ_RUN_MODE                                        \
    {                                 /* SYS_CLK  SLOW_CLK */          \
        {0UL, 0UL},                   /**< Invalid entry */            \
            {48000000UL, 24000000UL}, /**< SOSC maximum frequencies */ \
            {48000000UL, 24000000UL}, /**< SIRC maximum frequencies */ \
            {48000000UL, 24000000UL}, /**< FIRC maximum frequencies */ \
            {0UL, 0UL},               /**< Invalid entry */            \
            {0UL, 0UL},               /**< Invalid entry */            \
            {48000000UL, 24000000UL}, /**< SPLL maximum frequencies */ \
    }

#define CLOCK_MAX_FREQ_USB_MODE                                        \
    {                                 /* SYS_CLK  SLOW_CLK */          \
        {0UL, 0UL},                   /**< Invalid entry */            \
            {60000000UL, 30000000UL}, /**< SOSC maximum frequencies */ \
            {60000000UL, 30000000UL}, /**< SIRC maximum frequencies */ \
            {60000000UL, 30000000UL}, /**< FIRC maximum frequencies */ \
            {0UL, 0UL},               /**< Invalid entry */            \
            {0UL, 0UL},               /**< Invalid entry */            \
            {60000000UL, 30000000UL}, /**< SPLL maximum frequencies */ \
    }
/**
 * @brief Number of peripheral clocks.
 */
#if defined(PCC_LPSPI0_INDEX)
#define TMP_LPSPI0 1U
#else
#define TMP_LPSPI0 0U
#endif
#if defined(PCC_LPSPI1_INDEX)
#define TMP_LPSPI1 1U
#else
#define TMP_LPSPI1 0U
#endif
#if defined(PCC_LPSPI2_INDEX)
#define TMP_LPSPI2 1U
#else
#define TMP_LPSPI2 0U
#endif
#if defined(PCC_FTM0_INDEX)
#define TMP_FTM0 1U
#else
#define TMP_FTM0 0U
#endif
#if defined(PCC_FTM1_INDEX)
#define TMP_FTM1 1U
#else
#define TMP_FTM1 0U
#endif
#if defined(PCC_LPTMR0_INDEX)
#define TMP_LPTMR0 1U
#else
#define TMP_LPTMR0 0U
#endif
#if defined(PCC_LPI2C0_INDEX)
#define TMP_LPI2C0 1U
#else
#define TMP_LPI2C0 0U
#endif
#if defined(PCC_LPI2C1_INDEX)
#define TMP_LPI2C1 1U
#else
#define TMP_LPI2C1 0U
#endif
#if defined(PCC_LPUART0_INDEX)
#define TMP_LPUART0 1U
#else
#define TMP_LPUART0 0U
#endif
#if defined(PCC_LPUART1_INDEX)
#define TMP_LPUART1 1U
#else
#define TMP_LPUART1 0U
#endif
#if defined(PCC_LPUART2_INDEX)
#define TMP_LPUART2 1U
#else
#define TMP_LPUART2 0U
#endif
#if defined(PCC_FSUSB_INDEX)
#define TMP_FSUSB 1U
#else
#define TMP_FSUSB 0U
#endif
#if defined(PCC_PUF_INDEX)
#define TMP_PUF 1U
#else
#define TMP_PUF 0U
#endif
#if defined(PCC_HSSPI_INDEX)
#define TMP_HSSPI 1U
#else
#define TMP_HSSPI 0U
#endif

#define CLOCK_PERIPHERALS_COUNT                                                             \
    (TMP_LPSPI0 + TMP_LPSPI1 + TMP_LPSPI2 + TMP_FTM0 + TMP_FTM1 + TMP_LPTMR0 + TMP_LPI2C0 + \
     TMP_LPI2C1 + TMP_LPUART0 + TMP_LPUART1 + TMP_LPUART2 + TMP_FSUSB + TMP_PUF + TMP_HSSPI)

/**
 * @brief SCG system clock type.
 * Implements scg_system_clock_type_t_Class
 */
typedef enum
{
    SCG_SYSTEM_CLOCK_CORE, /**< Core clock.        */
    SCG_SYSTEM_CLOCK_BUS,  /**< BUS clock.         */
    SCG_SYSTEM_CLOCK_SLOW, /**< System slow clock. */
    SCG_SYSTEM_CLOCK_MAX,  /**< Max value.         */
} scg_system_clock_type_t;

/**
 * @brief SCG asynchronous clock type.
 * Implements scg_async_clock_type_t_Class
 */
typedef enum
{
    SCG_ASYNC_CLOCK_DIV1 = 0U, /**< Clock divider 1  */
    SCG_ASYNC_CLOCK_DIV2 = 1U, /**< Clock divider 2  */
    SCG_ASYNC_CLOCK_MAX  = 2U, /**< Max value.       */
} scg_async_clock_type_t;

/**
 * @brief SCG system clock modes.
 * Implements scg_system_clock_mode_t_Class
 */
typedef enum
{
    SCG_SYSTEM_CLOCK_MODE_CURRENT = 0U, /**< Current mode.            */
    SCG_SYSTEM_CLOCK_MODE_RUN     = 1U, /**< Run mode.                */
    SCG_SYSTEM_CLOCK_MODE_VLPR    = 2U, /**< Very Low Power Run mode. */
    SCG_SYSTEM_CLOCK_MODE_HSRUN   = 3U, /**< High Speed Run mode.     */
    SCG_SYSTEM_CLOCK_MODE_NONE          /**< MAX value.               */
} scg_system_clock_mode_t;

/*******************************************************************************
 * Private typedefs
 ******************************************************************************/

/*******************************************************************************
 * Global variables
 ******************************************************************************/
/* This frequency values should be set by different boards. */
/* SIM */
uint32_t g_tclk_freq[NUMBER_OF_TCLK_INPUTS]; /**< TCLKx clocks    */

/* RTC */
uint32_t g_rtc_clk_in_freq; /**< RTC CLKIN clock */

/* SCG */
uint32_t g_xtal0_clk_freq; /**< EXTAL0 clock    */

/** @brief Clock name mappings
 *         Constant array storing the mappings between clock names and
 * peripheral clock control indexes. If there is no peripheral clock control
 * index for a clock name, then the corresponding value is PCC_INVALID_INDEX.
 */
const uint16_t g_clock_name_mappings[] = PCC_CLOCK_NAME_MAPPINGS;

/** @brief Peripheral features list
 *         Constant array storing the mappings between clock names of the
 * peripherals and feature lists.
 */
const uint8_t g_peripheral_features_list[] = PERIPHERAL_FEATURES;

/*******************************************************************************
 * Static variables
 ******************************************************************************/
static clock_manager_state_t s_clock_state;

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
static void clock_get_default_config(clock_manager_user_config_t *p_config);

static status_t clock_get_scg_clock_freq(clock_names_t clock_name, uint32_t *p_frequency);

static status_t clock_get_sim_clock_freq(clock_names_t clock_name, uint32_t *p_frequency);

static status_t clock_get_pcc_clock_freq(clock_names_t clock_name, uint32_t *p_frequency);

static uint32_t clock_get_peripheral_clock(clock_names_t          clock_name,
                                           scg_async_clock_type_t divider);

static scg_system_clock_mode_t clock_get_current_run_mode(void);

static status_t clock_transition_system_clcok(const scg_system_clock_config_t *p_to_clk);

static uint32_t clock_get_sim_clk_out_freq(void);

static uint32_t clock_get_scg_clk_out_freq(void);

static uint32_t clock_get_sim_rtc_clk_freq(void);

static status_t clock_configure_tmp_sys_clk(void);

static status_t clock_configure_modules_from_scg(const scg_config_t *p_scg_config);

static status_t clock_configure_sirc(bool enable, const scg_sirc_config_t *p_sirc_config);

static status_t clock_configure_firc(bool enable, const scg_firc_config_t *p_firc_config);

static status_t clock_configure_sosc(bool enable, const scg_sosc_config_t *p_sosc_config);

static uint32_t clock_get_system_clock_freq(scg_system_clock_type_t type);

static status_t clock_set_sys_clk_config(scg_system_clock_mode_t          mode,
                                         scg_system_clock_config_t const *p_config);

static uint32_t clock_get_sys_async_freq(clock_names_t clock_source, scg_async_clock_type_t type);

static uint32_t clock_get_sirc_freq(void);

static uint32_t clock_get_firc_freq(void);

static uint32_t clock_get_sys_osc_freq(void);

static uint32_t clock_get_lpo_freq(void);

static status_t clock_transition_to_tmp_sys_clk(scg_system_clock_src_t p_current_sys_clk_src);

static void clock_get_current_sys_clk_config(scg_system_clock_config_t *p_sys_clock_config);

static status_t clock_set_scg_config(const scg_config_t *p_scg_config);

static status_t clock_get_ftm_option_freq(clock_names_t clock_name, uint32_t *p_frequency);

static void clcok_set_pcc_config(const pcc_config_t *p_peripheral_clk_config);

static void clock_set_sim_config(const sim_clock_config_t *p_sim_clock_config);

static scg_async_clock_div_t clock_convert_async_divider_value(uint16_t divider);

static status_t clock_set_sirc_clk_source(bool                         b_clock_source_enable,
                                          const clock_source_config_t *p_clk_src_config,
                                          scg_async_clock_div_t        divider1,
                                          scg_async_clock_div_t        divider2);

static status_t clock_set_firc_clk_source(bool                         b_clock_source_enable,
                                          const clock_source_config_t *p_clk_src_config,
                                          scg_async_clock_div_t        divider1,
                                          scg_async_clock_div_t        divider2);

static status_t clock_set_sosc_clk_source(bool                         b_clock_source_enable,
                                          const clock_source_config_t *p_clk_src_config,
                                          scg_async_clock_div_t        divider1,
                                          scg_async_clock_div_t        divider2);

static clock_names_t clock_get_default_module_clk_cfg_source(void);

static scg_system_clock_mode_t clock_get_sys_clk_mode(const pwr_modes_t mode);

static scg_system_clock_src_t clock_get_sys_clk_source(clock_names_t src);

static uint32_t clock_get_src_freq(scg_system_clock_src_t src);

static status_t clock_check_pcc_clock(clock_names_t clock_name);

static void clock_get_sys_clock_config(scg_system_clock_mode_t    sys_clock_mode,
                                       scg_system_clock_config_t *p_sys_clk_config_ptr);

static status_t clock_get_other_sim_clock_freq(clock_names_t clock_name, uint32_t *p_frequency);

/*******************************************************************************
 * Global functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : clock_init
 * Description   : This function sets the system to target configuration, it
 * only sets the clock modules registers for clock mode change, but not send
 * notifications to drivers.
 *
 * Implements    : clock_init_activity
 * END**************************************************************************/
status_t
clock_init(clock_manager_user_config_t const *p_config)
{
    status_t                           result;
    clock_manager_user_config_t        config_default;
    clock_manager_user_config_t const *p_cfg = p_config;

    DEV_ASSERT(clock_get_current_run_mode() == SCG_SYSTEM_CLOCK_MODE_RUN);

    if (p_config == NULL)
    {
        /* Get default configuration */
        clock_get_default_config(&config_default);
        p_cfg = &config_default;
    }

    /* Set SCG settings. */
    result = clock_set_scg_config(&p_cfg->scg_config);

    if (STATUS_SUCCESS == result)
    {
        /* Set SIM settings. */
        clock_set_sim_config(&p_cfg->sim_config);

        /* Set PCC settings. */
        clcok_set_pcc_config(&p_cfg->pcc_config);
    }

    return result;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_set_module_clock
 * Description   : This function enables a peripheral clock
 *
 * Implements    : clock_set_module_clock_activity
 * END**************************************************************************/
void
clock_set_module_clock(clock_names_t              peripheral_clock,
                       const module_clk_config_t *p_module_clk_config)
{
    uint32_t                   source = 0U, divider = 0U, multiplier = 0U;
    module_clk_config_t        default_module_clk_cfg;
    const module_clk_config_t *p_module_clk_cfg;

    /* Configuration is not provided, a default one will be used. */
    if (p_module_clk_config == NULL)
    {
        default_module_clk_cfg.b_gating = true;
        default_module_clk_cfg.source   = clock_get_default_module_clk_cfg_source();
        default_module_clk_cfg.mul      = 1U;
        default_module_clk_cfg.div      = 1U;
        p_module_clk_cfg                = &default_module_clk_cfg;
    }
    else
    {
        p_module_clk_cfg = p_module_clk_config;
    }

    /* Check that clock gating is configurable from PCC */

    /* Check that protocol clock is supported by the peripheral corresponding to the clock name */
    if ((g_peripheral_features_list[peripheral_clock] &
         (HAS_PROTOCOL_CLOCK_FROM_ASYNC1 | HAS_PROTOCOL_CLOCK_FROM_ASYNC2)) != 0U)
    {
        switch (p_module_clk_cfg->source)
        {
            case SIRC_CLK:
            {
                source = CLK_SRC_SIRC;
            }
            break;
            case FIRC_CLK:
            {
                source = CLK_SRC_FIRC;
            }
            break;
            case SOSC_CLK:
            {
                source = CLK_SRC_SOSC;
            }
            break;
            default:
            {
                /* Invalid name of the clock source. */
                DEV_ASSERT(false);
            }
            break;
        }
    }

    /* Check that divider is supported by the peripheral corresponding to
     * the clock name */
    if ((g_peripheral_features_list[peripheral_clock] & HAS_DIVIDER) != 0U)
    {
        DEV_ASSERT((0U < ((uint32_t)p_module_clk_cfg->div)) &&
                   (((uint32_t)p_module_clk_cfg->div) <= (1UL << PCC_PCCn_PCD_WIDTH)));
        divider = ((uint32_t)p_module_clk_cfg->div) - 1U;
    }

    /* Check that multiplier is supported by the peripheral corresponding to
     * the clock name */
    if ((g_peripheral_features_list[peripheral_clock] & HAS_MULTIPLIER) != 0U)
    {
        DEV_ASSERT((0U < ((uint32_t)p_module_clk_cfg->mul)) &&
                   (((uint32_t)p_module_clk_cfg->mul) <= (1UL << PCC_PCCn_FRAC_WIDTH)));
        multiplier = ((uint32_t)p_module_clk_cfg->mul) - 1U;
    }

    /* Disable the peripheral clock */
    pcc_set_clk_mode(PCC, peripheral_clock, false);

    if (p_module_clk_cfg->b_gating)
    {
        /* Set peripheral clock control */
        pcc_set_peripheral_clk_control(PCC, peripheral_clock, true, source, divider, multiplier);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_set_system_clock
 * Description   : This function configures the system clocks (core, bus and
 * flash clocks).
 *
 * Implements    : clock_set_system_clock_activity
 * END**************************************************************************/
status_t
clock_set_system_clock(const pwr_modes_t *p_mode, const sys_clk_config_t *p_sys_clk_config)
{
    status_t ret_code = STATUS_SUCCESS;

    scg_system_clock_mode_t   sys_clock_mode, current_sys_clock_mode = clock_get_current_run_mode();
    scg_system_clock_config_t sys_clock_config;

    static const scg_system_clock_div_t sysClkDivMappings[] = {SCG_SYSTEM_CLOCK_DIV_BY_1,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_1,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_2,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_3,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_4,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_5,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_6,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_7,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_8,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_9,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_10,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_11,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_12,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_13,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_14,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_15,
                                                               SCG_SYSTEM_CLOCK_DIV_BY_16};

    /* The power mode is not provided, the current one will be used by default.*/
    if (p_mode == NULL)
    {
        sys_clock_mode = current_sys_clock_mode;
    }
    else
    {
        sys_clock_mode = clock_get_sys_clk_mode(*p_mode);
    }

    /* System clock configuration is not provided, a default one will be used. */
    if (p_sys_clk_config == NULL)
    {
        /* Find a valid clock source. */
        clock_get_sys_clock_config(sys_clock_mode, &sys_clock_config);
    }
    else
    {
        /* The system clock source from input configuration structure must be valid. */
        DEV_ASSERT((clock_get_sirc_freq() != 0U) || (p_sys_clk_config->src != SIRC_CLK));
        DEV_ASSERT((clock_get_firc_freq() != 0U) || (p_sys_clk_config->src != FIRC_CLK));
        DEV_ASSERT((clock_get_sys_osc_freq() != 0U) || (p_sys_clk_config->src != SOSC_CLK));

        sys_clock_config.src = clock_get_sys_clk_source(p_sys_clk_config->src);

        DEV_ASSERT(p_sys_clk_config->dividers[0U] != 0U);
        DEV_ASSERT(p_sys_clk_config->dividers[1U] != 0U);

        sys_clock_config.div_core = sysClkDivMappings[p_sys_clk_config->dividers[0U]];
        sys_clock_config.div_slow = sysClkDivMappings[p_sys_clk_config->dividers[1U]];
    }

    /* System clock is configured in the current mode,
     * set configuration and wait until the system clock is changed. */
    if (sys_clock_mode == current_sys_clock_mode)
    {
        ret_code = clock_transition_system_clcok(&sys_clock_config);
    }
    /* System clock is not configured in the current mode, just set configuration. */
    else
    {
        ret_code = clock_set_sys_clk_config(sys_clock_mode, &sys_clock_config);
    }
    return ret_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_get_system_clock_source
 * Description   : This function gets the current system clock source
 *
 * Implements    : clock_get_system_clock_source_activity
 * END**************************************************************************/
void
clock_get_system_clock_source(sys_clk_config_t *p_sys_clk_config)
{
    switch (scg_get_system_clk_source(SCG))
    {
        case SCG_SYSTEM_CLOCK_SRC_SYS_OSC:
        {
            p_sys_clk_config->src = SOSC_CLK;
        }
        break;
        case SCG_SYSTEM_CLOCK_SRC_SIRC:
        {
            p_sys_clk_config->src = SIRC_CLK;
        }
        break;
        case SCG_SYSTEM_CLOCK_SRC_FIRC:
        {
            p_sys_clk_config->src = FIRC_CLK;
        }
        break;
        default:
        {
            /* Invalid system clock source. */
            DEV_ASSERT(false);
            p_sys_clk_config->src = SIRC_CLK;
        }
        break;
    }

    /* Core divider */
    p_sys_clk_config->dividers[0U] = (uint16_t)(scg_get_core_clk_divider_ratio(SCG) + 1U);

    /* Slow divider */
    p_sys_clk_config->dividers[1U] = (uint16_t)(scg_get_slow_clk_divider_ratio(SCG) + 1U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_set_clock_source
 * Description   : This function configures a clock source
 *
 * Implements    : clock_set_clock_source_activity
 * END**************************************************************************/
status_t
clock_set_clock_source(clock_names_t clock_source, const clock_source_config_t *p_clk_src_config)
{
    status_t ret_code              = STATUS_SUCCESS;
    bool     b_clock_source_enable = true;

    scg_async_clock_div_t divider1, divider2;

    if (p_clk_src_config == NULL)
    {
        divider1 = SCG_ASYNC_CLOCK_DIV_BY_1;
        divider2 = SCG_ASYNC_CLOCK_DIV_BY_1;
    }
    else
    {
        divider1 = clock_convert_async_divider_value(p_clk_src_config->output_div1);
        divider2 = clock_convert_async_divider_value(p_clk_src_config->output_div2);
    }

    /* Check whether the command enables and disables the clock source */
    if ((p_clk_src_config != NULL) && (p_clk_src_config->b_enable == false))
    {
        b_clock_source_enable = false;
    }

    switch (clock_source)
    {
        case SIRC_CLK:
        {
            ret_code = clock_set_sirc_clk_source(
                b_clock_source_enable, p_clk_src_config, divider1, divider2);
        }
        break;

        case FIRC_CLK:
        {
            ret_code = clock_set_firc_clk_source(
                b_clock_source_enable, p_clk_src_config, divider1, divider2);
        }
        break;

        case SOSC_CLK:
        {
            ret_code = clock_set_sosc_clk_source(
                b_clock_source_enable, p_clk_src_config, divider1, divider2);
        }
        break;

        case SIM_LPO_CLK:
        {
            if (b_clock_source_enable)
            {
                sim_set_lpo_clks(
                    SIM, true, true, SIM_LPO_CLK_SEL_LPO_128K, SIM_RTCCLK_SEL_SOSCDIV1_CLK);
            }
            else
            {
                sim_set_lpo_clks(
                    SIM, false, false, SIM_LPO_CLK_SEL_LPO_128K, SIM_RTCCLK_SEL_SOSCDIV1_CLK);
            }
        }
        break;

        default:
        {
            /* Invalid name of the clock source */
            DEV_ASSERT(false);
        }
        break;
    }

    return ret_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_get_freq
 * Description   : This function returns the frequency of a given clock
 *
 * Implements    : clock_get_freq_activity
 * END**************************************************************************/
status_t
clock_get_freq(clock_names_t clock_name, uint32_t *p_frequency)
{
    status_t returnCode;

    /* Frequency of the clock name from SCG */
    if (clock_name < SCG_END_OF_CLOCKS)
    {
        returnCode = clock_get_scg_clock_freq(clock_name, p_frequency);
    }
    /* Frequency of the clock name from SIM */
    else if (clock_name < SIM_END_OF_CLOCKS)
    {
        returnCode = clock_get_sim_clock_freq(clock_name, p_frequency);
    }
    /* Frequency of the clock name from PCC */
    else if (clock_name < PCC_END_OF_CLOCKS)
    {
        returnCode = clock_get_pcc_clock_freq(clock_name, p_frequency);
    }
    /* Invalid clock name */
    else
    {
        returnCode = STATUS_UNSUPPORTED;
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_init_config
 * Description   : Install pre-defined clock configurations.
 * This function installs the pre-defined clock configuration table to the
 * clock manager.
 *
 * Implements    : clock_init_config_activity
 *END**************************************************************************/
status_t
clock_init_config(clock_manager_user_config_t const    **pp_clock_configs_ptr,
                  uint8_t                                configs_number,
                  clock_manager_callback_user_config_t **pp_callbacks_ptr,
                  uint8_t                                callbacks_number)
{
    DEV_ASSERT(pp_clock_configs_ptr != NULL);
    DEV_ASSERT(pp_callbacks_ptr != NULL);

    s_clock_state.pp_config_table    = pp_clock_configs_ptr;
    s_clock_state.clock_config_num   = configs_number;
    s_clock_state.pp_callback_config = pp_callbacks_ptr;
    s_clock_state.callback_num       = callbacks_number;

    /*
     * error_callback_index is the index of the callback which returns error
     * during clock mode switch. If all callbacks return success, then the
     * error_callback_index is callbacks_number.
     */
    s_clock_state.error_callback_index = callbacks_number;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_update_config
 * Description   : Send notification and change system clock configuration.
 * This function sends the notification to all callback functions, if all
 * callbacks return OK or forceful policy is used, this function will change
 * system clock configuration. The function should be called only on run mode.
 *
 * Implements    : clock_update_config_activity
 *END**************************************************************************/
status_t
clock_update_config(uint8_t target_config_index, clock_manager_policy_t policy)
{
    uint8_t                                     callback_idx;
    bool                                        b_successful_set_config; /* Set config status */
    status_t                                    ret            = STATUS_SUCCESS;
    status_t                                    callback_state = STATUS_SUCCESS;
    const clock_manager_callback_user_config_t *p_callback_config;
    clock_notify_struct_t                       notify_struct;

    /* Check clock configuration index is out of range. */
    DEV_ASSERT(target_config_index < s_clock_state.clock_config_num);

    notify_struct.target_clock_config_index = target_config_index;
    notify_struct.policy                    = policy;

    /* Disable interrupts */
    int_disable_irq_global();
    /* Set error_callback_index as callback_num, which means no callback error now.*/
    s_clock_state.error_callback_index = s_clock_state.callback_num;

    /* First step: Send "BEFORE" notification. */
    notify_struct.notify_type = CLOCK_MANAGER_NOTIFY_BEFORE;

    /* Send notification to all callback. */
    for (callback_idx = 0; callback_idx < s_clock_state.callback_num; callback_idx++)
    {
        p_callback_config = s_clock_state.pp_callback_config[callback_idx];
        if ((p_callback_config) &&
            (p_callback_config->callback_type != CLOCK_MANAGER_CALLBACK_AFTER))
        {
            if (STATUS_SUCCESS !=
                (*p_callback_config->callback)(&notify_struct, p_callback_config->p_callback_data))
            {
                s_clock_state.error_callback_index = callback_idx;

                if (CLOCK_MANAGER_POLICY_AGREEMENT == policy)
                {
                    /* Save the error callback index. */
                    ret = STATUS_MCU_NOTIFY_BEFORE_ERROR;
                    break;
                }
            }
        }
    }

    /* If all callback success or forceful policy is used. */
    if ((STATUS_SUCCESS == ret) || (policy == CLOCK_MANAGER_POLICY_FORCIBLE))
    {
        /* clock mode switch. */
        ret                     = clock_init(s_clock_state.pp_config_table[target_config_index]);
        b_successful_set_config = (STATUS_SUCCESS == ret);

        s_clock_state.cur_config_index = target_config_index;
    }
    else
    {
        /* Unsuccessful setConfiguration */
        b_successful_set_config = false;
    }

    if (b_successful_set_config)
    {
        notify_struct.notify_type = CLOCK_MANAGER_NOTIFY_AFTER;

        for (callback_idx = 0; callback_idx < s_clock_state.callback_num; callback_idx++)
        {
            p_callback_config = s_clock_state.pp_callback_config[callback_idx];
            if ((p_callback_config) &&
                (p_callback_config->callback_type != CLOCK_MANAGER_CALLBACK_BEFORE))
            {
                callback_state = (*p_callback_config->callback)(&notify_struct,
                                                                p_callback_config->p_callback_data);
            }
            if (STATUS_SUCCESS != callback_state)
            {
                s_clock_state.error_callback_index = callback_idx;
                if (CLOCK_MANAGER_POLICY_AGREEMENT == policy)
                {
                    /* Save the error callback index. */
                    ret = STATUS_MCU_NOTIFY_AFTER_ERROR;
                    break;
                }
            }
        }
    }
    else /* Error occurs, need to send "RECOVER" notification. */
    {
        notify_struct.notify_type = CLOCK_MANAGER_NOTIFY_RECOVER;
        for (;;)
        {
            p_callback_config = s_clock_state.pp_callback_config[callback_idx];
            if (p_callback_config != NULL)
            {
                (void)(*p_callback_config->callback)(&notify_struct,
                                                     p_callback_config->p_callback_data);
            }
            if (callback_idx == 0U)
            {
                break;
            }
            callback_idx--;
        }
    }

    /* Enable interrupts */
    int_enable_irq_global();

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_get_current_config
 * Description   : Get current clock configuration index.
 *
 * Implements    : clock_get_current_config_activity
 *END**************************************************************************/
uint8_t
clock_get_current_config(void)
{
    return s_clock_state.cur_config_index;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_get_error_callback
 * Description   : Get the callback which returns error in last clock switch.
 *
 * Implements    : clock_get_error_callback_activity
 *END**************************************************************************/
clock_manager_callback_user_config_t *
clock_get_error_callback(void)
{
    clock_manager_callback_user_config_t *p_ret_value;

    /* If all callbacks return success. */
    if (s_clock_state.error_callback_index >= s_clock_state.clock_config_num)
    {
        p_ret_value = NULL;
    }
    else
    {
        p_ret_value = s_clock_state.pp_callback_config[s_clock_state.error_callback_index];
    }
    return p_ret_value;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_switch_pcc_freq_await_stable
 * Description   : Switch PCC gating and await it stable.
 *END**************************************************************************/
void
clock_switch_pcc_freq_await_stable(clock_names_t clock_name)
{
    /** Reset the PCC PLL about FSUSB, then await PLL state is locked. */
    uint32_t address =
        (uint32_t) & (PCC->PCCn[g_clock_name_mappings[clock_name]]); /**< 40065XXXh */
    REG_BIT_SET32(address, 0x00000001); /**< Gate is switched to 120MHz PLL */
    while (!(REG_READ32(address) & 0x00000002))
        ; /**< Await till locked. */
}

/*******************************************************************************
 * Static functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 * Function Name : clock_set_scg_config
 * Description   : This function configures the SCG blocks
 * END**************************************************************************/
static status_t
clock_set_scg_config(const scg_config_t *p_scg_config)
{
    status_t status = STATUS_SUCCESS;
    DEV_ASSERT(p_scg_config != NULL);

    if (p_scg_config != NULL)
    {
        /* Configure a temporary system clock source: FIRC */
        status = clock_configure_tmp_sys_clk();

        if (status == STATUS_SUCCESS)
        {
            /* Configure clock sources from SCG */
            status = clock_configure_modules_from_scg(p_scg_config);
        }

        if (status == STATUS_SUCCESS)
        {
            g_rtc_clk_in_freq = 0U; /* reset the value RTC_clk frequency. */
            if (p_scg_config->rtc_config.b_initialize)
            {
                /* RTC Clock settings. */
                g_rtc_clk_in_freq = p_scg_config->rtc_config.rtc_clk_in_freq;
            }

            /* Configure SCG ClockOut. */
            if (p_scg_config->clock_out_config.b_initialize)
            {
                /* ClockOut settings. */
                scg_set_clockout_source_sel(SCG, (uint32_t)p_scg_config->clock_out_config.source);
            }

            /* Configure SCG clock modes. */
            if (p_scg_config->clock_mode_config.b_initialize)
            {
                /* Configure SCG clock modes */
                status = clock_set_sys_clk_config(SCG_SYSTEM_CLOCK_MODE_RUN,
                                                  &(p_scg_config->clock_mode_config.rccr_config));
                if (status == STATUS_SUCCESS)
                {
                    status = clock_set_sys_clk_config(
                        SCG_SYSTEM_CLOCK_MODE_VLPR, &(p_scg_config->clock_mode_config.vccr_config));
                }
            }
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : clcok_set_pcc_config
 * Description   : This function configures the PCC block
 * END**************************************************************************/
static void
clcok_set_pcc_config(const pcc_config_t *p_peripheral_clk_config)
{
    DEV_ASSERT(p_peripheral_clk_config != NULL);

    uint32_t idx;

    if ((p_peripheral_clk_config != NULL) && (p_peripheral_clk_config->p_peripheral_clocks != NULL))
    {
        for (idx = 0U; idx < p_peripheral_clk_config->count; idx++)
        {
            /* Disable the peripheral clock */
            pcc_set_clk_mode(
                PCC, p_peripheral_clk_config->p_peripheral_clocks[idx].clk_name, false);

            /* Set peripheral clock control */
            pcc_set_peripheral_clk_control(
                PCC,
                p_peripheral_clk_config->p_peripheral_clocks[idx].clk_name,
                p_peripheral_clk_config->p_peripheral_clocks[idx].b_clk_gate,
                (uint32_t)p_peripheral_clk_config->p_peripheral_clocks[idx].clk_src,
                (uint32_t)p_peripheral_clk_config->p_peripheral_clocks[idx].divider,
                (uint32_t)p_peripheral_clk_config->p_peripheral_clocks[idx].frac);
        }
    }
}

/*FUNCTION**********************************************************************
 * Function Name : clock_set_sim_config
 * Description   : This function configures the SIM block
 * END**************************************************************************/
static void
clock_set_sim_config(const sim_clock_config_t *p_sim_clock_config)
{
    DEV_ASSERT(p_sim_clock_config != NULL);
    uint8_t idx;

    /* OSC 8MHz settings. */
    SIM->OSCCTR = (SIM->OSCCTR & ~SIM_OSCCTR_FBIN_MASK) | SIM_OSCCTR_FBIN(0x3C);
    /* USB PHY settings. */
    SIM->USBHMCR = (SIM->USBHMCR & ~0xFFFFFF) | 0xAFFFF;

    /* ClockOut settings. */
    if (p_sim_clock_config->clock_out_config.b_initialize)
    {
        sim_set_clockout(SIM,
                         p_sim_clock_config->clock_out_config.b_enable,
                         p_sim_clock_config->clock_out_config.source,
                         p_sim_clock_config->clock_out_config.divider);
    }

    /* Low Power Clock settings from SIM. */
    if (p_sim_clock_config->lpo_clock_config.b_initialize)
    {
        sim_set_lpo_clks(SIM,
                         p_sim_clock_config->lpo_clock_config.b_enable_lpo1k,
                         p_sim_clock_config->lpo_clock_config.b_enable_lpo32k,
                         p_sim_clock_config->lpo_clock_config.source_lpo_clk,
                         p_sim_clock_config->lpo_clock_config.source_rtc_clk);
    }

    /* TCLK Clock settings. */
    if (p_sim_clock_config->tclk_config.b_initialize)
    {
        for (idx = 0; idx < NUMBER_OF_TCLK_INPUTS; idx++)
        {
            g_tclk_freq[idx] = p_sim_clock_config->tclk_config.tclkFreq[idx];
        }
        /* FTMOPT0 clock settings */
        for (idx = 0; idx < FTM_INSTANCE_COUNT; idx++)
        {
            sim_set_ext_pin_source_ftm(SIM, idx, p_sim_clock_config->tclk_config.extPinSrc[idx]);
        }
    }
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_default_config
 * Description   : This function gets the system to a default configuration, it
 * only gets the clock modules registers for clock mode change, but not send
 * notifications to drivers.
 * END**************************************************************************/
static void
clock_get_default_config(clock_manager_user_config_t *p_config)
{
    uint32_t                         idx                                            = 0U;
    static peripheral_clock_config_t peripheral_clk_config[CLOCK_PERIPHERALS_COUNT] = {
#ifdef PCC_ADC0_INDEX
        {
            .clk_name   = ADC0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_ADC1_INDEX
        {
            .clk_name   = ADC1_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_CMP0_INDEX
        {
            .clk_name   = CMP0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_CRC_INDEX
        {
            .clk_name   = CRC0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_ENET_INDEX
        {
            .clk_name   = ENET0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_EWM_INDEX
        {
            .clk_name   = EWM0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_FlexCAN0_INDEX
        {
            .clk_name   = FlexCAN0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_FlexCAN1_INDEX
        {
            .clk_name   = FlexCAN1_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_FlexCAN2_INDEX
        {
            .clk_name   = FlexCAN2_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_FlexIO_INDEX
        {
            .clk_name   = FLEXIO0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_FTFM_INDEX
        {
            .clk_name   = FTFM0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_FTFC_INDEX
        {
            .clk_name   = FTFC0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_FTM0_INDEX
        {
            .clk_name   = FTM0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_FTM1_INDEX
        {
            .clk_name   = FTM1_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_FTM2_INDEX
        {
            .clk_name   = FTM2_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_FTM3_INDEX
        {
            .clk_name   = FTM3_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_FTM4_INDEX
        {
            .clk_name   = FTM4_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_FTM5_INDEX
        {
            .clk_name   = FTM5_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_FTM6_INDEX
        {
            .clk_name   = FTM6_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_FTM7_INDEX
        {
            .clk_name   = FTM7_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_LPI2C0_INDEX
        {
            .clk_name   = LPI2C0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_LPI2C1_INDEX
        {
            .clk_name   = LPI2C1_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_LPIT_INDEX
        {
            .clk_name   = LPIT0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_LPSPI0_INDEX
        {
            .clk_name   = LPSPI0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_LPSPI1_INDEX
        {
            .clk_name   = LPSPI1_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_LPSPI2_INDEX
        {
            .clk_name   = LPSPI2_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_LPTMR0_INDEX
        {
            .clk_name   = LPTMR0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_LPUART0_INDEX
        {
            .clk_name   = LPUART0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_LPUART1_INDEX
        {
            .clk_name   = LPUART1_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_LPUART2_INDEX
        {
            .clk_name   = LPUART2_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_SIRC_DIV1,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_PDB0_INDEX
        {
            .clk_name   = PDB0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_PDB1_INDEX
        {
            .clk_name   = PDB1_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_PORTA_INDEX
        {
            .clk_name   = PORTA_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_PORTB_INDEX
        {
            .clk_name   = PORTB_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_PORTC_INDEX
        {
            .clk_name   = PORTC_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_PORTD_INDEX
        {
            .clk_name   = PORTD_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_PORTE_INDEX
        {
            .clk_name   = PORTE_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_QSPI_INDEX
        {
            .clk_name   = QSPI0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_RTC_INDEX
        {
            .clk_name   = RTC0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_SAI0_INDEX
        {
            .clk_name   = SAI0_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_SAI1_INDEX
        {
            .clk_name   = SAI1_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_FSUSB_INDEX
#if defined(SPLL_96MHZ)
        {
            .clk_name   = FSUSB_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = SPLL_CLKOUT_96M,
        },
#else // SPLL 120MHZ
        {
            .clk_name   = FSUSB_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_OFF,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = SPLL_CLKOUT_120M,
        },
#endif
#endif
#ifdef PCC_PUF_INDEX
        {
            .clk_name   = PUF_CLK,
            .b_clk_gate = true,
            .clk_src    = PUF_CLK_SPLLDIV2,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
#ifdef PCC_HSSPI_INDEX
        {
            .clk_name   = HSSPI_CLK,
            .b_clk_gate = true,
            .clk_src    = CLK_SRC_FIRC_DIV2,
            .frac       = MULTIPLY_BY_ONE,
            .divider    = DIVIDE_BY_ONE,
        },
#endif
    };

    /* SCG */
    /* SIRCDIV */
    p_config->scg_config.sirc_config.b_initialize = true;                     /**< Initialize */
    p_config->scg_config.sirc_config.div1         = SCG_ASYNC_CLOCK_DIV_BY_1; /**< SIRCDIV1  */
    p_config->scg_config.sirc_config.div2         = SCG_ASYNC_CLOCK_DIV_BY_1; /**< SIRCDIV2  */

    /* FIRCDIV */
    p_config->scg_config.firc_config.b_initialize = true;                     /**< Initialize */
    p_config->scg_config.firc_config.div1         = SCG_ASYNC_CLOCK_DIV_BY_1; /**< FIRCDIV1   */
    p_config->scg_config.firc_config.div2         = SCG_ASYNC_CLOCK_DIV_BY_1; /**< FIRCDIV2   */

    p_config->scg_config.rtc_config.b_initialize    = true;                   /**< Initialize  */
    p_config->scg_config.rtc_config.rtc_clk_in_freq = 0U;                     /**< RTC_CLKIN   */

    /* SOSCDIV */
    p_config->scg_config.sosc_config.b_initialize = true;                     /**< Initialize */
    p_config->scg_config.sosc_config.freq         = 8000000U;                 /**< Frequency  */
    p_config->scg_config.sosc_config.div1         = SCG_ASYNC_CLOCK_DIV_BY_1; /**< SOSCDIV1    */
    p_config->scg_config.sosc_config.div2         = SCG_ASYNC_CLOCK_DIV_BY_1; /**< SOSCDIV2    */

    p_config->scg_config.clock_out_config.b_initialize = true;                /**< Initialize    */
    p_config->scg_config.clock_out_config.source = SCG_CLOCKOUT_SRC_FIRC; /**< SCG CLKOUTSEL     */

    p_config->scg_config.clock_mode_config.b_initialize = true;           /**< Initialize */
    /**< RCCR - Run Clock Control Register          */
    p_config->scg_config.clock_mode_config.rccr_config.src = SCG_SYSTEM_CLOCK_SRC_FIRC; /**< SCS */
    p_config->scg_config.clock_mode_config.rccr_config.div_core =
        SCG_SYSTEM_CLOCK_DIV_BY_1; /**< DIVCORE    */
    p_config->scg_config.clock_mode_config.rccr_config.div_slow =
        SCG_SYSTEM_CLOCK_DIV_BY_2; /**< DIVSLOW    */

    /**< VCCR - VLPR Clock Control Register        */
    p_config->scg_config.clock_mode_config.vccr_config.src = SCG_SYSTEM_CLOCK_SRC_SIRC; /**< SCS */
    p_config->scg_config.clock_mode_config.vccr_config.div_core =
        SCG_SYSTEM_CLOCK_DIV_BY_2; /**< DIVCORE    */
    p_config->scg_config.clock_mode_config.vccr_config.div_slow =
        SCG_SYSTEM_CLOCK_DIV_BY_4; /**< DIVSLOW    */

    /* PCC */
    p_config->pcc_config.p_peripheral_clocks = peripheral_clk_config; /**< Peripheral
                                                                   clock control
                                                                   configurations
                                                                 */
    p_config->pcc_config.count = CLOCK_PERIPHERALS_COUNT; /**< Number of the peripheral clock
                                                          control configurations  */

    /* SIM */
    /**< Clock Out configuration.           */
    p_config->sim_config.clock_out_config.b_initialize = true;           /**< Initialize    */
    p_config->sim_config.clock_out_config.b_enable     = false;          /**< CLKOUTEN      */
    p_config->sim_config.clock_out_config.source =
        SIM_CLKOUT_SEL_SYSTEM_SCG_CLKOUT;                                /**< CLKOUTSEL */
    p_config->sim_config.clock_out_config.divider = SIM_CLKOUT_DIV_BY_1; /**< CLKOUTDIV     */
    /**< Low Power Clock configuration.     */
    p_config->sim_config.lpo_clock_config.b_initialize    = true; /**< Initialize    */
    p_config->sim_config.lpo_clock_config.b_enable_lpo1k  = true; /**< LPO1KCLKEN    */
    p_config->sim_config.lpo_clock_config.b_enable_lpo32k = true; /**< LPO32KCLKEN   */
    p_config->sim_config.lpo_clock_config.source_lpo_clk =
        SIM_LPO_CLK_SEL_LPO_128K;                                 /**< LPOCLKSEL     */
    p_config->sim_config.lpo_clock_config.source_rtc_clk =
        SIM_RTCCLK_SEL_SOSCDIV1_CLK;                              /**< RTCCLKSEL */
    /**< TCLK CLOCK configuration. */
    p_config->sim_config.tclk_config.b_initialize = true; /**< Initialize    */
    p_config->sim_config.tclk_config.tclkFreq[0]  = 0U;   /**< TCLK0         */
    p_config->sim_config.tclk_config.tclkFreq[1]  = 0U;   /**< TCLK0         */
    p_config->sim_config.tclk_config.tclkFreq[2]  = 0U;   /**< TCLK0         */
    for (idx = 0; idx < FTM_INSTANCE_COUNT; idx++)
    {
        p_config->sim_config.tclk_config.extPinSrc[idx] = 0U; /**< FTMx ext pin source */
    }
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_scg_clock_freq
 * Description   : This function returns the frequency of a given clock from SCG
 * END**************************************************************************/
static status_t
clock_get_scg_clock_freq(clock_names_t clock_name, uint32_t *p_frequency)
{
    status_t return_code = STATUS_SUCCESS;
    uint32_t freq        = 0U;

    switch (clock_name)
    {
        /* Main clocks */
        case CORE_CLK:
        {
            freq = clock_get_system_clock_freq(SCG_SYSTEM_CLOCK_CORE);
        }
        break;
        case BUS_CLK:
        {
            freq = clock_get_system_clock_freq(SCG_SYSTEM_CLOCK_BUS);
        }
        break;
        case SLOW_CLK:
        {
            freq = clock_get_system_clock_freq(SCG_SYSTEM_CLOCK_SLOW);
        }
        break;
        case CLKOUT_CLK:
        {
            freq = clock_get_sim_clk_out_freq();
        }
        break;

        /* Other internal clocks used by peripherals. */
        case SIRC_CLK:
        {
            freq = clock_get_sirc_freq();
        }
        break;
        case FIRC_CLK:
        {
            freq = clock_get_firc_freq();
        }
        break;
        case SOSC_CLK:
        {
            freq = clock_get_sys_osc_freq();
        }
        break;
        case RTC_CLKIN_CLK:
        {
            freq = g_rtc_clk_in_freq;
        }
        break;
        case SCG_CLKOUT_CLK:
        {
            freq = clock_get_scg_clk_out_freq();
        }
        break;
        case SIRCDIV1_CLK:
        {
            freq = clock_get_sys_async_freq(SIRC_CLK, SCG_ASYNC_CLOCK_DIV1);
        }
        break;
        case SIRCDIV2_CLK:
        {
            freq = clock_get_sys_async_freq(SIRC_CLK, SCG_ASYNC_CLOCK_DIV2);
        }
        break;
        case FIRCDIV1_CLK:
        {
            freq = clock_get_sys_async_freq(FIRC_CLK, SCG_ASYNC_CLOCK_DIV1);
        }
        break;
        case FIRCDIV2_CLK:
        {
            freq = clock_get_sys_async_freq(FIRC_CLK, SCG_ASYNC_CLOCK_DIV2);
        }
        break;
        case SOSCDIV1_CLK:
        {
            freq = clock_get_sys_async_freq(SOSC_CLK, SCG_ASYNC_CLOCK_DIV1);
        }
        break;
        case SOSCDIV2_CLK:
        {
            freq = clock_get_sys_async_freq(SOSC_CLK, SCG_ASYNC_CLOCK_DIV2);
        }
        break;
        default:
        {
            return_code = STATUS_UNSUPPORTED;
        }
        break;
    }

    if (p_frequency != NULL)
    {
        *p_frequency = freq;
    }

    return return_code;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_sim_clock_freq
 * Description   : This function returns the frequency of a given clock from SIM
 * END**************************************************************************/
static status_t
clock_get_sim_clock_freq(clock_names_t clock_name, uint32_t *p_frequency)
{
    status_t return_code = STATUS_SUCCESS;
    uint32_t clockPinSelect;
    uint32_t freq = 0U;

    switch (clock_name)
    {
        /* SIM clocks */
        case SIM_FTM0_CLOCKSEL:
        {
            clockPinSelect = sim_get_ftm0_ext_clk_pin_mode(SIM);
            if (clockPinSelect < NUMBER_OF_TCLK_INPUTS)
            {
                freq = g_tclk_freq[clockPinSelect];
            }
        }
        break;
        case SIM_FTM1_CLOCKSEL:
        {
            clockPinSelect = sim_get_ftm1_ext_clk_pin_mode(SIM);
            if (clockPinSelect < NUMBER_OF_TCLK_INPUTS)
            {
                freq = g_tclk_freq[clockPinSelect];
            }
        }
        break;
        default:
        {
            return_code = clock_get_other_sim_clock_freq(clock_name, &freq);
        }
        break;
    }

    /* get interface clock of some special module */
    if ((clock_name > SIM_LPO_128K_CLK) && (clock_name <= SIM_MSCM_CLK) &&
        (return_code == STATUS_SUCCESS))
    {
        freq = clock_get_system_clock_freq(SCG_SYSTEM_CLOCK_BUS);
    }
    /* return status mcu gate off if frequency is 0. */
    if ((freq == 0UL) && (return_code != STATUS_UNSUPPORTED))
    {
        return_code = STATUS_MCU_GATED_OFF;
    }

    if (p_frequency != NULL)
    {
        *p_frequency = freq;
    }

    return return_code;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_other_sim_clock_freq
 * Description   : This function returns the frequency of other clock from SIM
 * END**************************************************************************/
static status_t
clock_get_other_sim_clock_freq(clock_names_t clock_name, uint32_t *p_frequency)
{
    status_t return_code = STATUS_SUCCESS;
    uint32_t freq        = 0;

    switch (clock_name)
    {
        case SIM_CLKOUTSELL:
        {
            freq = clock_get_sim_clk_out_freq();
        }
        break;
        case SIM_RTCCLK_CLK:
        {
            freq = clock_get_sim_rtc_clk_freq();
        }
        break;
        case SIM_LPO_CLK:
        {
            freq = clock_get_lpo_freq();
        }
        break;
        case SIM_LPO_1K_CLK:
        {
            freq = ((sim_get_lpo32k_status(SIM)) && (sim_get_lpo1k_status(SIM))) ? LPO_1K_FREQUENCY
                                                                                 : 0UL;
        }
        break;
        case SIM_LPO_32K_CLK:
        {
            freq = sim_get_lpo32k_status(SIM) ? LPO_32K_FREQUENCY : 0UL;
        }
        break;
        case SIM_LPO_128K_CLK:
        {
            freq = LPO_128K_FREQUENCY;
        }
        break;
        default:
        {
            /* Do nothing */
        }
        break;
    }
    if (p_frequency != NULL)
    {
        *p_frequency = freq;
    }

    return return_code;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_sys_clock_config
 * Description   : get the system clock config
 * END**************************************************************************/
static void
clock_get_sys_clock_config(scg_system_clock_mode_t    sys_clock_mode,
                           scg_system_clock_config_t *p_sys_clk_config_ptr)
{
    static const scg_system_clock_div_t def_sys_clk[TMP_SYS_CLK_NO][TMP_SYS_DIV_NO] =
        TMP_SYSTEM_CLOCK_CONFIGS;
    bool b_get_clock_freq_state;

    if (clock_get_firc_freq() != 0U)
    {
        b_get_clock_freq_state           = true;
        (*p_sys_clk_config_ptr).src      = SCG_SYSTEM_CLOCK_SRC_FIRC;
        (*p_sys_clk_config_ptr).div_core = def_sys_clk[TMP_FIRC_CLK][TMP_SYS_DIV];
        (*p_sys_clk_config_ptr).div_slow = def_sys_clk[TMP_FIRC_CLK][TMP_SLOW_DIV];
    }
    else if (clock_get_sys_osc_freq() != 0U)
    {
        b_get_clock_freq_state           = true;
        (*p_sys_clk_config_ptr).src      = SCG_SYSTEM_CLOCK_SRC_SYS_OSC;
        (*p_sys_clk_config_ptr).div_core = def_sys_clk[TMP_SOSC_CLK][TMP_SYS_DIV];
        (*p_sys_clk_config_ptr).div_slow = def_sys_clk[TMP_SOSC_CLK][TMP_SLOW_DIV];
    }
    else
    {
        b_get_clock_freq_state = false;
    }
    if (b_get_clock_freq_state == false)
    {
        if (clock_get_sirc_freq() != 0U)
        {
            (*p_sys_clk_config_ptr).src = SCG_SYSTEM_CLOCK_SRC_SIRC;
            if (sys_clock_mode == SCG_SYSTEM_CLOCK_MODE_VLPR)
            {
                (*p_sys_clk_config_ptr).div_core = FEATURE_VLPR_SYS_CLK;
                (*p_sys_clk_config_ptr).div_slow = FEATURE_VLPR_SLOW_CLK;
            }
            else
            {
                (*p_sys_clk_config_ptr).div_core = def_sys_clk[TMP_SIRC_CLK][TMP_SYS_DIV];
                (*p_sys_clk_config_ptr).div_slow = def_sys_clk[TMP_SIRC_CLK][TMP_SLOW_DIV];
            }
        }
        else
        {
            /* Can't reach this point, at least one clock source is functional.
             * This code is written to avoid MISRA 15.7 (no 'else' at end of 'if
             * ... else if' chain) */
            DEV_ASSERT(false);
        }
    }

    (void)sys_clock_mode;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_check_pcc_clock
 * Description   : Checks if PCC clock name is valid and if the module is enabled.
 * END**************************************************************************/
static status_t
clock_check_pcc_clock(clock_names_t clock_name)
{
    status_t return_code = STATUS_SUCCESS;

    /* Invalid PCC clock names. */
    if ((clock_name <= SIM_END_OF_CLOCKS) || (clock_name == PCC_END_OF_BUS_CLOCKS) ||
        (clock_name == PCC_END_OF_SYS_CLOCKS) || (clock_name == PCC_END_OF_SLOW_CLOCKS) ||
        (clock_name == PCC_END_OF_ASYNCH_DIV1_CLOCKS) ||
        (clock_name == PCC_END_OF_ASYNCH_DIV2_CLOCKS))
    {
        return_code = STATUS_UNSUPPORTED;
    }
    else if (pcc_get_clk_mode(PCC, clock_name) == false)
    {
        /* Module is not clocked. */
        return_code = STATUS_MCU_GATED_OFF;
    }
    else
    {
        return_code = STATUS_SUCCESS;
    }

    return return_code;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_pcc_clock_freq
 * Description   : This function returns the clock frequency of peripheral
 * functional clock.
 * END**************************************************************************/
static status_t
clock_get_pcc_clock_freq(clock_names_t clock_name, uint32_t *p_frequency)
{
    status_t return_code    = STATUS_SUCCESS;
    uint32_t freq           = 0U;
    uint32_t interface_freq = 0U;

    return_code = clock_check_pcc_clock(clock_name);

    if (return_code == STATUS_SUCCESS)
    {
        if ((g_peripheral_features_list[clock_name] & HAS_INT_CLOCK_FROM_BUS_CLOCK) != 0U)
        {
            /* Check whether BUS CLOCK is clocked. */
            interface_freq = clock_get_system_clock_freq(SCG_SYSTEM_CLOCK_BUS);
            return_code =
                (status_t)((interface_freq == 0U) ? STATUS_MCU_GATED_OFF : STATUS_SUCCESS);
        }
        else if ((g_peripheral_features_list[clock_name] & HAS_INT_CLOCK_FROM_SYS_CLOCK) != 0U)
        {
            /* Check whether SYS CLOCK is clocked. */
            interface_freq = clock_get_system_clock_freq(SCG_SYSTEM_CLOCK_CORE);
            return_code =
                (status_t)((interface_freq == 0U) ? STATUS_MCU_GATED_OFF : STATUS_SUCCESS);
        }
        else if ((g_peripheral_features_list[clock_name] & HAS_INT_CLOCK_FROM_SLOW_CLOCK) != 0U)
        {
            /* Check whether SLOW CLOCK is clocked. */
            interface_freq = clock_get_system_clock_freq(SCG_SYSTEM_CLOCK_SLOW);
            return_code =
                (status_t)((interface_freq == 0U) ? STATUS_MCU_GATED_OFF : STATUS_SUCCESS);
        }
        else
        { /* It's an issue in peripheral features list, each peripheral must
             have one interface clock. */
            DEV_ASSERT(false);
        }

        if (return_code == STATUS_SUCCESS)
        {
            /* Check whether peripheral has protocol clock (functional clock) */
            if ((g_peripheral_features_list[clock_name] &
                 (HAS_PROTOCOL_CLOCK_FROM_ASYNC1 | HAS_PROTOCOL_CLOCK_FROM_ASYNC2)) != 0U)
            {
                if ((g_peripheral_features_list[clock_name] & HAS_PROTOCOL_CLOCK_FROM_ASYNC1) != 0U)
                {
                    /* Check whether the functional clock is clocked */
                    freq = clock_get_peripheral_clock(clock_name, SCG_ASYNC_CLOCK_DIV1);
                }

                if ((g_peripheral_features_list[clock_name] & HAS_PROTOCOL_CLOCK_FROM_ASYNC2) != 0U)
                {
                    /* Check whether the functional clock is clocked */
                    freq = clock_get_peripheral_clock(clock_name, SCG_ASYNC_CLOCK_DIV2);
                }

                if (freq == 0U)
                {
                    return_code = STATUS_MCU_GATED_OFF;
                }
            }
            else
            {
                freq = interface_freq;
            }
        }
    }
    (void)interface_freq;

    /* If frequency reference is provided, write this value */
    if (p_frequency != NULL)
    {
        *p_frequency = freq;
    }

    return return_code;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_ftm_option_freq
 * Description   : Internal function used by clock_get_peripheral_clock function
 * END**************************************************************************/
static status_t
clock_get_ftm_option_freq(clock_names_t clock_name, uint32_t *p_frequency)
{
    status_t return_code = STATUS_SUCCESS;

    switch (clock_name)
    {
        case FTM0_CLK:
        {
            return_code = clock_get_sim_clock_freq(SIM_FTM0_CLOCKSEL, p_frequency);
        }
        break;
        case FTM1_CLK:
        {
            return_code = clock_get_sim_clock_freq(SIM_FTM1_CLOCKSEL, p_frequency);
        }
        break;
#if FTM_INSTANCE_COUNT > 2U
        case FTM2_CLK:
        {
            return_code = clock_get_sim_clock_freq(SIM_FTM2_CLOCKSEL, p_frequency);
        }
        break;
#endif
#if FTM_INSTANCE_COUNT > 3U
        case FTM3_CLK:
        {
            return_code = clock_get_sim_clock_freq(SIM_FTM3_CLOCKSEL, p_frequency);
        }
        break;
#endif
#if FTM_INSTANCE_COUNT > 4U
        case FTM4_CLK:
        {
            return_code = clock_get_sim_clock_freq(SIM_FTM4_CLOCKSEL, p_frequency);
        }
        break;
#endif
#if FTM_INSTANCE_COUNT > 5U
        case FTM5_CLK:
        {
            return_code = clock_get_sim_clock_freq(SIM_FTM5_CLOCKSEL, p_frequency);
        }
        break;
#endif
#if FTM_INSTANCE_COUNT > 6U
        case FTM6_CLK:
        {
            return_code = clock_get_sim_clock_freq(SIM_FTM6_CLOCKSEL, p_frequency);
        }
        break;
#endif
#if FTM_INSTANCE_COUNT > 7U
        case FTM7_CLK:
        {
            return_code = clock_get_sim_clock_freq(SIM_FTM7_CLOCKSEL, p_frequency);
        }
        break;
#endif
        default:
        {
            /* Do nothing */
        }
        break;
    }

    return return_code;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_peripheral_clock
 * Description   : Internal function used by clock_get_pcc_clock_freq function
 * END**************************************************************************/
static uint32_t
clock_get_peripheral_clock(clock_names_t clock_name, scg_async_clock_type_t divider)
{
    uint32_t frequency  = 0;
    uint32_t frac_value = pcc_get_frac_value_sel(PCC, clock_name);
    uint32_t div_value  = pcc_get_divider_sel(PCC, clock_name);

    /* Check division factor */
    if (((uint32_t)frac_value) <= ((uint32_t)div_value))
    {
        /* Check clock gate */
        if (pcc_get_clk_mode(PCC, clock_name))
        {
            /* Check clock source */
            switch (pcc_get_clk_source_sel(PCC, clock_name))
            {
                case (uint32_t)CLK_SRC_SOSC:
                {
                    frequency = clock_get_sys_async_freq(SOSC_CLK, divider);
                }
                break;
                case (uint32_t)CLK_SRC_SIRC:
                {
                    frequency = clock_get_sys_async_freq(SIRC_CLK, divider);
                }
                break;
                case (uint32_t)CLK_SRC_FIRC:
                {
                    frequency = clock_get_sys_async_freq(FIRC_CLK, divider);
                }
                break;
#if FEATURE_HAS_LPO_PERIPHERAL_CLOCK_SOURCE
                case (uint32_t)CLK_SRC_LPO:
                {
                    (void)clock_get_sim_clock_freq(SIM_LPO_CLK, &frequency);
                    div_value = 0U;
                }
                break;
#endif
                default:
                {
                    (void)clock_get_ftm_option_freq(clock_name, &frequency);
                    div_value = 0U;
                }
                break;
            }

            frequency = frequency / (div_value + 1U);
            frequency = frequency * (frac_value + 1U);
        }
    }

    return frequency;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_current_run_mode
 * Description   : Internal function used by clock_set_scg_config function
 * END**************************************************************************/
static scg_system_clock_mode_t
clock_get_current_run_mode(void)
{
    scg_system_clock_mode_t mode;

    /* Get the current running mode */
    switch (smc_get_current_running_mode(SMC))
    {
        /* High speed run mode */
        case HIGH_SPEED_RUNNING_MODE:
        {
            mode = SCG_SYSTEM_CLOCK_MODE_HSRUN;
        }
        break;
        /* Run mode */
        case RUN_SPEED_RUNNING_MODE:
        {
            mode = SCG_SYSTEM_CLOCK_MODE_RUN;
        }
        break;
        /* Very low power run mode */
        case VLPR_SPEED_RUNNING_MODE:
        {
            mode = SCG_SYSTEM_CLOCK_MODE_VLPR;
        }
        break;
        /* This should never happen - core has to be in some run mode to execute
         * code */
        default:
        {
            mode = SCG_SYSTEM_CLOCK_MODE_NONE;
        }
        break;
    }

    return mode;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_transition_system_clcok
 * Description   : Internal function used by
 * clock_configure_tmp_sys_clk and clock_configure_modules_from_scg functions
 * END**************************************************************************/
static status_t
clock_transition_system_clcok(const scg_system_clock_config_t *p_to_clk)
{
    scg_system_clock_mode_t run_mode;
    status_t                ret_value = STATUS_SUCCESS;
    uint32_t                timeout;

    /* Check destination clock */
    DEV_ASSERT(p_to_clk != NULL);
    DEV_ASSERT(p_to_clk->src != SCG_SYSTEM_CLOCK_SRC_NONE);

    /* Get & Convert Run mode from SMC to SCG defines*/
    run_mode = clock_get_current_run_mode();

    /* Check the current mode */
    DEV_ASSERT(run_mode != SCG_SYSTEM_CLOCK_MODE_NONE);

    /* Update run mode configuration */
    ret_value = clock_set_sys_clk_config(run_mode, p_to_clk);

    if (ret_value == STATUS_SUCCESS)
    {
        /* Wait for system clock to transition. */
        timeout = 10U;

        do
        {
            timeout--;
        } while ((scg_get_system_clk_source(SCG) != ((uint32_t)p_to_clk->src)) && (timeout > 0U));

        if (timeout == 0U)
        {
            ret_value = STATUS_TIMEOUT;
        }
    }

    return ret_value;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_sim_clk_out_freq
 * Description   : Internal function used by clock_get_freq function
 * END**************************************************************************/
static uint32_t
clock_get_sim_clk_out_freq(void)
{
    uint32_t frequency;

    if (sim_get_clockout_status(SIM))
    {
        switch (sim_get_clockout_selector_value(SIM))
        {
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_SCG_CLKOUT):
            {
                frequency = clock_get_scg_clk_out_freq();
            }
            break;
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_SOSC_DIV2_CLK):
            {
                frequency = clock_get_sys_async_freq(SOSC_CLK, SCG_ASYNC_CLOCK_DIV2);
            }
            break;
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_SIRC_DIV2_CLK):
            {
                frequency = clock_get_sys_async_freq(SIRC_CLK, SCG_ASYNC_CLOCK_DIV2);
            }
            break;
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_FIRC_DIV2_CLK):
            {
                frequency = clock_get_sys_async_freq(FIRC_CLK, SCG_ASYNC_CLOCK_DIV2);
            }
            break;
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_HCLK):
            {
                frequency = clock_get_system_clock_freq(SCG_SYSTEM_CLOCK_CORE);
            }
            break;
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_BUS_CLK):
            {
                frequency = clock_get_system_clock_freq(SCG_SYSTEM_CLOCK_BUS);
            }
            break;
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_LPO_128K_CLK):
            {
                frequency = LPO_128K_FREQUENCY;
            }
            break;
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_LPO_CLK):
            {
                frequency = clock_get_lpo_freq();
            }
            break;
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_RTC_CLK):
            {
                frequency = clock_get_sim_rtc_clk_freq();
            }
            break;
            default:
            {
                /* Invalid SIM CLKOUT selection.*/
                frequency = 0U;
            }
            break;
        }

        /* Apply Divide Ratio */
        frequency /= (sim_get_clockout_divider_value(SIM) + 1U);
    }
    else
    {
        /* Output disabled. */
        frequency = 0U;
    }

    return frequency;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_scg_clk_out_freq
 * Description   : Internal function used by clock_get_freq function
 * END**************************************************************************/
static uint32_t
clock_get_scg_clk_out_freq(void)
{
    uint32_t frequency;

    switch (scg_get_clockout_source_sel(SCG))
    {
        case ((uint32_t)SCG_CLOCKOUT_SRC_SCG_SLOW):
        {
            frequency = clock_get_system_clock_freq(SCG_SYSTEM_CLOCK_SLOW);
        }
        break;
        case ((uint32_t)SCG_CLOCKOUT_SRC_SOSC):
        {
            frequency = clock_get_sys_osc_freq();
        }
        break;
        case ((uint32_t)SCG_CLOCKOUT_SRC_SIRC):
        {
            frequency = clock_get_sirc_freq();
        }
        break;
        case ((uint32_t)SCG_CLOCKOUT_SRC_FIRC):
        {
            frequency = clock_get_firc_freq();
        }
        break;
        default:
        {
            /* Invalid SCG CLKOUT selection.*/
            frequency = 0U;
        }
        break;
    }

    return frequency;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_sim_rtc_clk_freq
 * Description   : Internal function used by clock_get_freq function
 * END**************************************************************************/
static uint32_t
clock_get_sim_rtc_clk_freq(void)
{
    uint32_t frequency;

    /* Check RTCCLK Select */
    switch (sim_get_rtc_clk_src(SIM))
    {
        case ((uint32_t)SIM_RTCCLK_SEL_SOSCDIV1_CLK):
        {
            frequency = clock_get_sys_async_freq(SOSC_CLK, SCG_ASYNC_CLOCK_DIV1);
        }
        break;
        case ((uint32_t)SIM_RTCCLK_SEL_LPO_32K):
        {
            frequency = sim_get_lpo32k_status(SIM) ? LPO_32K_FREQUENCY : 0UL;
        }
        break;
        case ((uint32_t)SIM_RTCCLK_SEL_RTC_CLKIN):
            frequency = g_rtc_clk_in_freq;
            break;
        case ((uint32_t)SIM_RTCCLK_SEL_FIRCDIV1_CLK):
            frequency = clock_get_sys_async_freq(FIRC_CLK, SCG_ASYNC_CLOCK_DIV1);
            break;
        default:
            /* Invalid RTCCLK selection.*/
            frequency = 0U;
            break;
    }

    return frequency;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_configure_sirc
 * Description   : Configures SIRC module based on provided configuration.
 * END**************************************************************************/
static status_t
clock_configure_sirc(bool b_enable, const scg_sirc_config_t *p_sirc_config)
{
    status_t                 status = STATUS_SUCCESS;
    scg_sirc_config_t        sirc_default_config;
    const scg_sirc_config_t *p_sirc_cfg;
    uint32_t                 timeout;

    if (p_sirc_config == NULL)
    {
        sirc_default_config.div1 = SCG_ASYNC_CLOCK_DIV_BY_1;
        sirc_default_config.div2 = SCG_ASYNC_CLOCK_DIV_BY_1;

        p_sirc_cfg = &sirc_default_config;
    }
    else
    {
        p_sirc_cfg = p_sirc_config;
    }

    /* Configure SIRC. */
    if (b_enable && (status == STATUS_SUCCESS))
    {
        /* Now start to set up SIRC clock. */
        /* Step 1. Setup dividers. */
        scg_set_sirc_async_config(SCG, p_sirc_cfg->div1, p_sirc_cfg->div2);

        /* Step 2. Set SIRC configuration: frequency range. */

        /* Step 3. Set SIRC control: enable clock, configure source in STOP and
         * VLP modes, configure lock feature. */

        /* Wait for SIRC to initialize */
        timeout = SIRC_STABILIZATION_TIMEOUT;
        while ((clock_get_sirc_freq() == 0U) && (timeout > 0U))
        {
            timeout--;
        }

        if (timeout == 0U)
        {
            status = STATUS_TIMEOUT;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_configure_firc
 * Description   : Configures FIRC module based on provided configuration.
 * END**************************************************************************/
static status_t
clock_configure_firc(bool b_enable, const scg_firc_config_t *p_firc_config)
{
    status_t                 status = STATUS_SUCCESS;
    scg_firc_config_t        firc_default_config;
    const scg_firc_config_t *p_firc_cfg;
    uint32_t                 timeout;

    if (p_firc_config == NULL)
    {
        firc_default_config.div1 = SCG_ASYNC_CLOCK_DIV_BY_1;
        firc_default_config.div2 = SCG_ASYNC_CLOCK_DIV_BY_1;

        p_firc_cfg = &firc_default_config;
    }
    else
    {
        p_firc_cfg = p_firc_config;
    }

    /* Configure FIRC. */
    if (b_enable && (status == STATUS_SUCCESS))
    {
        /* Now start to set up FIRC clock. */
        /* Step 1. Setup dividers. */
        scg_set_firc_async_config(SCG, p_firc_cfg->div1, p_firc_cfg->div2);

        /* Step 2. Set FIRC configuration. */

        /* Step 3. Enable clock, config regulator and locking feature. */

        /* Wait for FIRC to initialize */
        timeout = FIRC_STABILIZATION_TIMEOUT;
        while ((clock_get_firc_freq() == 0U) && (timeout > 0U))
        {
            timeout--;
        }

        if (timeout == 0U)
        {
            status = STATUS_TIMEOUT;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_configure_sosc
 * Description   : Configures SOSC module based on provided configuration.
 * END**************************************************************************/
static status_t
clock_configure_sosc(bool b_enable, const scg_sosc_config_t *p_sosc_config)
{
    status_t                 status = STATUS_SUCCESS;
    scg_sosc_config_t        sosc_default_config;
    const scg_sosc_config_t *p_sosc_cfg;
    uint32_t                 timeout;

    if (p_sosc_config == NULL)
    {
        sosc_default_config.div1 = SCG_ASYNC_CLOCK_DIV_BY_1;
        sosc_default_config.div2 = SCG_ASYNC_CLOCK_DIV_BY_1;
        sosc_default_config.freq = 8000000U;

        p_sosc_cfg = &sosc_default_config;
    }
    else
    {
        p_sosc_cfg = p_sosc_config;
    }

    /* Configure SOSC. */
    if (b_enable && (status == STATUS_SUCCESS))
    {
        /* Now start to set up OSC clock. */
        /* Step 1. Setup dividers. */
        scg_set_sosc_async_config(SCG, p_sosc_cfg->div1, p_sosc_cfg->div2);

        /* Step 2. Set OSC configuration. */

        /* Step 3. Enable clock, configure monitor, lock register. */

        g_xtal0_clk_freq = p_sosc_cfg->freq;

        /* Wait for System OSC to initialize */
        timeout = SOSC_STABILIZATION_TIMEOUT;
        while ((clock_get_sys_osc_freq() == 0U) && (timeout > 0U))
        {
            timeout--;
        }

        if (timeout == 0U)
        {
            status = STATUS_TIMEOUT;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_configure_tmp_sys_clk
 * Description   : Configures and transitions to a temporary system clock
 * source: FIRC
 * END**************************************************************************/
static status_t
clock_configure_tmp_sys_clk(void)
{
    status_t                            status = STATUS_SUCCESS;
    scg_system_clock_config_t           sys_clock_config;
    static const scg_system_clock_div_t tmpSysClk[TMP_SYS_CLK_NO][TMP_SYS_DIV_NO] =
        TMP_SYSTEM_CLOCK_CONFIGS;

    /* If the current system clock source is not FIRC:
     * 1. Enable FIRC (if it's not enabled)
     * 2. Switch to FIRC.
     */
    if (scg_get_system_clk_source(SCG) != ((uint32_t)SCG_SYSTEM_CLOCK_SRC_FIRC))
    {
        /* If FIRC is not on, then FIRC is configured
         * with the default configuration */
        if (clock_get_firc_freq() == 0UL)
        {
            status = clock_configure_firc(true, NULL);
        }

        /* FIRC is enabled, transition the system clock source to FIRC. */
        if (status == STATUS_SUCCESS)
        {
            sys_clock_config.src      = SCG_SYSTEM_CLOCK_SRC_FIRC;
            sys_clock_config.div_core = tmpSysClk[TMP_FIRC_CLK][TMP_SYS_DIV];
            sys_clock_config.div_slow = tmpSysClk[TMP_FIRC_CLK][TMP_SLOW_DIV];
            status                    = clock_transition_system_clcok(&sys_clock_config);
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_configure_modules_from_scg
 * Description   : Configures all modules from SCG (SIRC, FIRC, SOSC and SPLL)
 * END**************************************************************************/
static status_t
clock_configure_modules_from_scg(const scg_config_t *p_scg_config)
{
    status_t                            status = STATUS_SUCCESS;
    scg_system_clock_config_t           sys_clock_config;
    const scg_system_clock_config_t    *p_next_sys_clock_config;
    static const scg_system_clock_div_t tmpSysClk[TMP_SYS_CLK_NO][TMP_SYS_DIV_NO] =
        TMP_SYSTEM_CLOCK_CONFIGS;

    /* Configure all clock sources that are different from the
     * current system clock source FIRC (SIRC, SOSC, SPLL). */
    status =
        clock_configure_sirc(p_scg_config->sirc_config.b_initialize, &p_scg_config->sirc_config);
    if (status == STATUS_SUCCESS)
    {
        status = clock_configure_sosc(p_scg_config->sosc_config.b_initialize,
                                      &p_scg_config->sosc_config);
    }

    /* Get the next system clock source */
    switch (clock_get_current_run_mode())
    {
        case SCG_SYSTEM_CLOCK_MODE_RUN:
        {
            p_next_sys_clock_config = &p_scg_config->clock_mode_config.rccr_config;
        }
        break;
        case SCG_SYSTEM_CLOCK_MODE_VLPR:
        {
            p_next_sys_clock_config = &p_scg_config->clock_mode_config.vccr_config;
        }
        break;
        default:
        {
            DEV_ASSERT(false);
            p_next_sys_clock_config = NULL;
        }
        break;
    }

    if (status == STATUS_SUCCESS)
    {
        /* The current system clock source is FIRC.
         * Verify whether the next system clock source is FIRC. */
        if (p_next_sys_clock_config->src == SCG_SYSTEM_CLOCK_SRC_FIRC)
        {
            /* If they are the same, search for a temporary system clock source
             * (use one of the following sources: SPLL, SOSC, SIRC)
             * Assume that a temporary clock is not found status = ERROR. */
            status = STATUS_ERROR;

            /* SOSC is enabled and SPLL configuration for system clock source is
             * not valid */
            if (p_scg_config->sosc_config.b_initialize && (status == STATUS_ERROR))
            {
                sys_clock_config.src      = SCG_SYSTEM_CLOCK_SRC_SYS_OSC;
                sys_clock_config.div_core = tmpSysClk[TMP_SOSC_CLK][TMP_SYS_DIV];
                sys_clock_config.div_slow = tmpSysClk[TMP_SOSC_CLK][TMP_SLOW_DIV];
                status                    = clock_transition_system_clcok(&sys_clock_config);
            }

            /* SIRC is enabled and SOSC configuration for system clock source is
             * not valid */
            if (p_scg_config->sirc_config.b_initialize && (status == STATUS_ERROR))
            {
                sys_clock_config.src      = SCG_SYSTEM_CLOCK_SRC_SIRC;
                sys_clock_config.div_core = tmpSysClk[TMP_SIRC_CLK][TMP_SYS_DIV];
                sys_clock_config.div_slow = tmpSysClk[TMP_SIRC_CLK][TMP_SLOW_DIV];
                status                    = clock_transition_system_clcok(&sys_clock_config);
            }

            /* Transitioned to a temporary system clock source. */
            if (status == STATUS_SUCCESS)
            {
                /* Configure the remaining clock source (FIRC). */
                status = clock_configure_firc(p_scg_config->firc_config.b_initialize,
                                              &p_scg_config->firc_config);

                if (status == STATUS_SUCCESS)
                {
                    /* Transition to the next system clock source. */
                    sys_clock_config.src      = p_next_sys_clock_config->src;
                    sys_clock_config.div_core = p_next_sys_clock_config->div_core;
                    sys_clock_config.div_slow = p_next_sys_clock_config->div_slow;
                    status                    = clock_transition_system_clcok(&sys_clock_config);
                }
            }
        }
        else
        { /* Transition to the next system clock source. */
            sys_clock_config.src      = p_next_sys_clock_config->src;
            sys_clock_config.div_core = p_next_sys_clock_config->div_core;
            sys_clock_config.div_slow = p_next_sys_clock_config->div_slow;
            status                    = clock_transition_system_clcok(&sys_clock_config);

            if (status == STATUS_SUCCESS)
            {
                /* Configure the remaining clock source (FIRC) */
                status = clock_configure_firc(p_scg_config->firc_config.b_initialize,
                                              &p_scg_config->firc_config);
            }
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_system_clock_freq
 * Description   : This function gets the SCG system clock frequency, these
 * clocks are used for core, platform, external and bus clock domains.
 * END**************************************************************************/
static uint32_t
clock_get_system_clock_freq(scg_system_clock_type_t type)
{
    uint32_t freq;

    DEV_ASSERT(type < SCG_SYSTEM_CLOCK_MAX);

    switch (scg_get_system_clk_source(SCG))
    {
        case ((uint32_t)SCG_SYSTEM_CLOCK_SRC_SYS_OSC):
        {
            freq = clock_get_sys_osc_freq();
        }
        break;
        case ((uint32_t)SCG_SYSTEM_CLOCK_SRC_SIRC):
        {
            freq = clock_get_sirc_freq();
        }
        break;
        case ((uint32_t)SCG_SYSTEM_CLOCK_SRC_FIRC):
        {
            freq = clock_get_firc_freq();
        }
        break;
        default:
        {
            freq = 0U;
        }
        break;
    }

    freq /= (scg_get_core_clk_divider_ratio(SCG) + 1U);

    switch (type)
    {
        case SCG_SYSTEM_CLOCK_CORE:
        {
            /* Intentionally left blank */
        }
        break;
        case SCG_SYSTEM_CLOCK_BUS:
        {
            freq /= (1U);
        }
        break;
        case SCG_SYSTEM_CLOCK_SLOW:
        {
            freq /= (scg_get_slow_clk_divider_ratio(SCG) + 1U);
        }
        break;
        default:
        {
            freq = 0U;
        }
        break;
    }

    return freq;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_src_freq
 * Description   : Gets source frequency.
 * END**************************************************************************/
static uint32_t
clock_get_src_freq(scg_system_clock_src_t src)
{
    uint32_t src_freq = 0U;

    switch (src)
    {
        case SCG_SYSTEM_CLOCK_SRC_SYS_OSC:
        {
            src_freq = clock_get_sys_osc_freq();
        }
        break;
        case SCG_SYSTEM_CLOCK_SRC_SIRC:
        {
            src_freq = clock_get_sirc_freq();
        }
        break;
        case SCG_SYSTEM_CLOCK_SRC_FIRC:
        {
            src_freq = clock_get_firc_freq();
        }
        break;
        default:
        {
            src_freq = 0U;
        }
        break;
    }

    return src_freq;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_set_sys_clk_config
 * Description   : This function sets the system configuration for the specified
 * mode.
 * END**************************************************************************/
static status_t
clock_set_sys_clk_config(scg_system_clock_mode_t mode, scg_system_clock_config_t const *p_config)
{
    status_t status   = STATUS_SUCCESS;
    uint32_t src_freq = 0U;

    /* The maximum clock frequencies of system clocks in all power modes */
    static const uint32_t max_sys_clks_in_vlpr[MODES_MAX_NO][SYS_CLK_MAX_NO] =
        CLOCK_MAX_FREQ_VLPR_MODE;
#if defined(SPLL_96MHZ)
    static const uint32_t max_sys_clks_in_run[MODES_MAX_NO][SYS_CLK_MAX_NO] =
        CLOCK_MAX_FREQ_RUN_MODE;
#else // SPLL 120MHZ
    static const uint32_t max_sys_clks_in_usb[MODES_MAX_NO][SYS_CLK_MAX_NO] =
        CLOCK_MAX_FREQ_USB_MODE;
#endif
    const uint32_t sys_freq_mul = ((uint32_t)p_config->div_core) + 1UL;
    const uint32_t slow_freq_mul =
        (((uint32_t)p_config->div_core) + 1UL) * (((uint32_t)p_config->div_slow) + 1UL);

    DEV_ASSERT(mode != SCG_SYSTEM_CLOCK_MODE_CURRENT);

    src_freq = clock_get_src_freq(p_config->src);
    src_freq >>= 4U;

    switch (mode)
    {
        case SCG_SYSTEM_CLOCK_MODE_RUN: /**< Run mode.                */
        {
            // if (PCC->PCCn[PCC_FSUSB_INDEX] & PCC_PCCn_PCD_MASK)
#if defined(SPLL_96MHZ)
            {
                /* Verify the frequencies of sys and slow clocks. */
                if ((src_freq > (sys_freq_mul *
                                 (max_sys_clks_in_run[(uint32_t)p_config->src][CORE_CLK_INDEX] >>
                                  4U))) || /* Sys(core) clock */
                    (src_freq > (slow_freq_mul *
                                 (max_sys_clks_in_run[(uint32_t)p_config->src][SLOW_CLK_INDEX] >>
                                  4U)))) /* Slow clock */
                {
                    /* Configuration for the next system clock source is not valid.  */
                    status = STATUS_ERROR;
                }
            }
#else // SPLL 120MHZ
            {
                /* Verify the frequencies of sys and slow clocks. */
                if ((src_freq > (sys_freq_mul *
                                 (max_sys_clks_in_usb[(uint32_t)p_config->src][CORE_CLK_INDEX] >>
                                  4U))) || /* Sys(core) clock */
                    (src_freq > (slow_freq_mul *
                                 (max_sys_clks_in_usb[(uint32_t)p_config->src][SLOW_CLK_INDEX] >>
                                  4U)))) /* Slow clock */
                {
                    /* Configuration for the next system clock source is not valid.  */
                    status = STATUS_ERROR;
                }
            }
#endif
            if (status == STATUS_SUCCESS)
            {
                scg_set_run_clk_control(SCG,
                                        (uint32_t)p_config->src,
                                        (uint32_t)p_config->div_core,
                                        (uint32_t)p_config->div_slow);
            }
        }
        break;
        case SCG_SYSTEM_CLOCK_MODE_VLPR: /**< Very Low Power Run mode. */
        {
            DEV_ASSERT(SCG_SYSTEM_CLOCK_SRC_SIRC == p_config->src);
            /* Verify the frequencies of sys slow clocks. */
            if ((src_freq >
                 (sys_freq_mul * (max_sys_clks_in_vlpr[(uint32_t)p_config->src][CORE_CLK_INDEX] >>
                                  4U))) || /* Sys(core) clock */
                (src_freq >
                 (slow_freq_mul * (max_sys_clks_in_vlpr[(uint32_t)p_config->src][SLOW_CLK_INDEX] >>
                                   4U)))) /* Slow clock */
            {
                /* Configuration for the next system clock source is not valid.  */
                status = STATUS_ERROR;
            }
            else
            {
                scg_set_vlpr_clk_control(SCG,
                                         (uint32_t)p_config->src,
                                         (uint32_t)p_config->div_core,
                                         (uint32_t)p_config->div_slow);
            }
        }
        break;
        default:
        {
            /* Invalid mode */
            DEV_ASSERT(false);
        }
        break;
    }
    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_sys_async_freq
 * Description   : Gets SCG asynchronous clock frequency from a clock source.
 * END**************************************************************************/
static uint32_t
clock_get_sys_async_freq(clock_names_t clock_source, scg_async_clock_type_t type)
{
    uint32_t freq, div = 0U;

    switch (type)
    {
        case SCG_ASYNC_CLOCK_DIV1:
        {
            switch (clock_source)
            {
                case FIRC_CLK:
                {
                    freq = clock_get_firc_freq();
                    div  = scg_get_firc_first_async_divider(SCG);
                }
                break;
                case SIRC_CLK:
                {
                    freq = clock_get_sirc_freq();
                    div  = scg_get_sirc_first_async_divider(SCG);
                }
                break;
                case SOSC_CLK:
                {
                    freq = clock_get_sys_osc_freq();
                    div  = scg_get_sosc_first_async_divider(SCG);
                }
                break;
                default:
                {
                    /* Invalid clock source type */
                    freq = 0U;
                    DEV_ASSERT(false);
                }
                break;
            }
        }
        break;
        case SCG_ASYNC_CLOCK_DIV2:
        {
            switch (clock_source)
            {
                case FIRC_CLK:
                {
                    freq = clock_get_firc_freq();
                    div  = scg_get_firc_second_async_divider(SCG);
                }
                break;
                case SIRC_CLK:
                {
                    freq = clock_get_sirc_freq();
                    div  = scg_get_sirc_second_async_divider(SCG);
                }
                break;
                case SOSC_CLK:
                {
                    freq = clock_get_sys_osc_freq();
                    div  = scg_get_sosc_second_async_divide(SCG);
                }
                break;
                default:
                {
                    /* Invalid clock source type */
                    freq = 0U;
                    DEV_ASSERT(false);
                }
                break;
            }
        }
        break;
        default:
        {
            /* Invalid async clock source */
            freq = 0U;
            DEV_ASSERT(false);
        }
        break;
    }

    if (div != 0U)
    {
        freq = (freq >> (div - 1U));
    }
    else /* Output disabled. */
    {
        freq = 0U;
    }

    return freq;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_sys_osc_freq
 * Description   : Gets SCG System OSC clock frequency (SYSOSC).
 * END**************************************************************************/
static uint32_t
clock_get_sys_osc_freq(void)
{
    uint32_t ret_value;
    ret_value = g_xtal0_clk_freq;
    // ret_value = 0U;

    return ret_value;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_sirc_freq
 * Description   : Gets SCG Slow IRC clock frequency (SIRC).
 * END**************************************************************************/
static uint32_t
clock_get_sirc_freq(void)
{
    uint32_t ret_value = FEATURE_SCG_SIRC_HIGH_RANGE_FREQ;

    return ret_value;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_firc_freq
 * Description   : Gets SCG Fast IRC clock frequency (FIRC).
 * END**************************************************************************/
static uint32_t
clock_get_firc_freq(void)
{
    uint32_t ret_value;

    // if (PCC->PCCn[PCC_FSUSB_INDEX] & PCC_PCCn_PCD_MASK)
#if defined(SPLL_96MHZ)
    {
        ret_value = FEATURE_SCG_FIRC_FREQ0;
    }
#else // SPLL 120MHZ
    {
        ret_value = FEATURE_SCG_FIRC_FREQ1;
    }
#endif
    return ret_value;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_lpo_freq
 * Description   : Gets SIM LPO clock frequency (LPO).
 * END**************************************************************************/
static uint32_t
clock_get_lpo_freq(void)
{
    uint32_t freq = 0U;

    switch (sim_get_lpo_clk_selector_value(SIM))
    {
        case 0U: /* SIM_LPO_CLK_SEL_LPO_128K */
        {
            freq = LPO_128K_FREQUENCY;
        }
        break;
        case 1U: /* SIM_LPO_CLK_SEL_NO_CLOCK: */
        {
            freq = 0U;
        }
        break;
        case 2U: /* SIM_LPO_CLK_SEL_LPO_32K: */
        {
            freq = sim_get_lpo32k_status(SIM) ? LPO_32K_FREQUENCY : 0UL;
        }
        break;
        case 3U: /* SIM_LPO_CLK_SEL_LPO_1K:  */
        {
            freq = ((sim_get_lpo32k_status(SIM)) && (sim_get_lpo1k_status(SIM))) ? LPO_1K_FREQUENCY
                                                                                 : 0UL;
        }
        break;
        default: /* Invalid LPOCLKSEL selection.*/
        {
            DEV_ASSERT(false);
        }
        break;
    }

    return freq;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_transition_to_tmp_sys_clk
 * Description   : Transition to a temporary system clock
 * END**************************************************************************/
static status_t
clock_transition_to_tmp_sys_clk(scg_system_clock_src_t p_current_sys_clk_src)
{
    scg_system_clock_config_t sys_clock_config;

    static const scg_system_clock_div_t tmp_sys_clk[TMP_SYS_CLK_NO][TMP_SYS_DIV_NO] =
        TMP_SYSTEM_CLOCK_CONFIGS;

    status_t status = STATUS_ERROR;

    /* FIRC is functional, it is not the current system clock source, no valid
     * source has been found yet. */
    if ((clock_get_firc_freq() != 0U) && (p_current_sys_clk_src != SCG_SYSTEM_CLOCK_SRC_FIRC) &&
        (status != STATUS_SUCCESS))
    {
        sys_clock_config.src      = SCG_SYSTEM_CLOCK_SRC_FIRC;
        sys_clock_config.div_core = tmp_sys_clk[TMP_FIRC_CLK][TMP_SYS_DIV];
        sys_clock_config.div_slow = tmp_sys_clk[TMP_FIRC_CLK][TMP_SLOW_DIV];
        status                    = clock_transition_system_clcok(&sys_clock_config);
    }

    /* SOSC is functional, it is not the current system clock source, no valid
     * source has been found yet. */
    if ((clock_get_sys_osc_freq() != 0U) &&
        (p_current_sys_clk_src != SCG_SYSTEM_CLOCK_SRC_SYS_OSC) && (status != STATUS_SUCCESS))
    {
        sys_clock_config.src      = SCG_SYSTEM_CLOCK_SRC_SYS_OSC;
        sys_clock_config.div_core = tmp_sys_clk[TMP_SOSC_CLK][TMP_SYS_DIV];
        sys_clock_config.div_slow = tmp_sys_clk[TMP_SOSC_CLK][TMP_SLOW_DIV];
        status                    = clock_transition_system_clcok(&sys_clock_config);
    }

    /* SIRC is functional, it is not the current system clock source, no valid
     * source has been found yet. */
    if ((clock_get_sirc_freq() != 0U) && (p_current_sys_clk_src != SCG_SYSTEM_CLOCK_SRC_SIRC) &&
        (status != STATUS_SUCCESS))
    {
        sys_clock_config.src      = SCG_SYSTEM_CLOCK_SRC_SIRC;
        sys_clock_config.div_core = tmp_sys_clk[TMP_SIRC_CLK][TMP_SYS_DIV];
        sys_clock_config.div_slow = tmp_sys_clk[TMP_SIRC_CLK][TMP_SLOW_DIV];
        status                    = clock_transition_system_clcok(&sys_clock_config);
    }

    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_transition_to_tmp_sys_clk
 * Description   : Transition to a temporary system clock
 * END**************************************************************************/
static void
clock_get_current_sys_clk_config(scg_system_clock_config_t *p_sys_clock_config)
{
    static const scg_system_clock_div_t
        sys_clk_div_mappings[((uint32_t)SCG_SYSTEM_CLOCK_DIV_BY_16) + 1U] = {
            SCG_SYSTEM_CLOCK_DIV_BY_1,
            SCG_SYSTEM_CLOCK_DIV_BY_2,
            SCG_SYSTEM_CLOCK_DIV_BY_3,
            SCG_SYSTEM_CLOCK_DIV_BY_4,
            SCG_SYSTEM_CLOCK_DIV_BY_5,
            SCG_SYSTEM_CLOCK_DIV_BY_6,
            SCG_SYSTEM_CLOCK_DIV_BY_7,
            SCG_SYSTEM_CLOCK_DIV_BY_8,
            SCG_SYSTEM_CLOCK_DIV_BY_9,
            SCG_SYSTEM_CLOCK_DIV_BY_10,
            SCG_SYSTEM_CLOCK_DIV_BY_11,
            SCG_SYSTEM_CLOCK_DIV_BY_12,
            SCG_SYSTEM_CLOCK_DIV_BY_13,
            SCG_SYSTEM_CLOCK_DIV_BY_14,
            SCG_SYSTEM_CLOCK_DIV_BY_15,
            SCG_SYSTEM_CLOCK_DIV_BY_16};

    /* Save the current system clock source configuration */
    switch (scg_get_system_clk_source(SCG))
    {
        case 1U:
        {
            p_sys_clock_config->src = SCG_SYSTEM_CLOCK_SRC_SYS_OSC;
            break;
        }
        case 2U:
        {
            p_sys_clock_config->src = SCG_SYSTEM_CLOCK_SRC_SIRC;
            break;
        }
        case 3U:
        {
            p_sys_clock_config->src = SCG_SYSTEM_CLOCK_SRC_FIRC;
            break;
        }
        default:
        {
            /* Invalid system clock value */
            DEV_ASSERT(false);
            break;
        }
    }
    p_sys_clock_config->div_core = sys_clk_div_mappings[scg_get_core_clk_divider_ratio(SCG)];
    p_sys_clock_config->div_slow = sys_clk_div_mappings[scg_get_slow_clk_divider_ratio(SCG)];
}

/*FUNCTION**********************************************************************
 * Function Name : clock_convert_async_divider_value
 * Description   : Converts an integer value to asynchronous divider value type.
 * END**************************************************************************/
static scg_async_clock_div_t
clock_convert_async_divider_value(uint16_t divider)
{
    scg_async_clock_div_t ret_value;

    switch (divider)
    {
        case (1U << 6U):
        {
            ret_value = SCG_ASYNC_CLOCK_DIV_BY_64;
        }
        break;
        case (1U << 5U):
        {
            ret_value = SCG_ASYNC_CLOCK_DIV_BY_32;
        }
        break;
        case (1U << 4U):
        {
            ret_value = SCG_ASYNC_CLOCK_DIV_BY_16;
        }
        break;
        case (1U << 3U):
        {
            ret_value = SCG_ASYNC_CLOCK_DIV_BY_8;
        }
        break;
        case (1U << 2U):
        {
            ret_value = SCG_ASYNC_CLOCK_DIV_BY_4;
        }
        break;
        case (1U << 1U):
        {
            ret_value = SCG_ASYNC_CLOCK_DIV_BY_2;
        }
        break;
        case (1U << 0U):
        {
            ret_value = SCG_ASYNC_CLOCK_DIV_BY_1;
        }
        break;
        case 0U:
            /* Pass - through */
        default:
        {
            ret_value = SCG_ASYNC_CLOCK_DISABLE;
        }
        break;
    }
    return ret_value;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_default_module_clk_cfg_source
 * Description   : Gets default module clock source.
 * END**************************************************************************/
static clock_names_t
clock_get_default_module_clk_cfg_source(void)
{
    clock_names_t ret = CLOCK_NAME_COUNT;

    if (clock_get_sirc_freq() != 0U)
    {
        ret = SIRC_CLK;
    }
    else if (clock_get_firc_freq() != 0U)
    {
        ret = FIRC_CLK;
    }
    else if (clock_get_sys_osc_freq() != 0U)
    {
        ret = SOSC_CLK;
    }
    else
    {
        ret = CLOCK_NAME_COUNT;
    }

    return ret;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_sys_clk_mode
 * Description   : Gets default system clock mode.
 * END**************************************************************************/
static scg_system_clock_mode_t
clock_get_sys_clk_mode(const pwr_modes_t mode)
{
    scg_system_clock_mode_t sys_clock_mode;

    switch (mode)
    {
        /* High speed run mode */
        case HSRUN_MODE:
        {
            sys_clock_mode = SCG_SYSTEM_CLOCK_MODE_HSRUN;
        }
        break;
        /* Run mode */
        case RUN_MODE:
        {
            sys_clock_mode = SCG_SYSTEM_CLOCK_MODE_RUN;
        }
        break;
        /* Very low power run mode */
        case VLPR_MODE:
        {
            sys_clock_mode = SCG_SYSTEM_CLOCK_MODE_VLPR;
        }
        break;
        /* This should never happen - input power mode is invalid */
        default:
        {
            sys_clock_mode = SCG_SYSTEM_CLOCK_MODE_NONE;
            DEV_ASSERT(false);
        }
        break;
    }

    return sys_clock_mode;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_get_sys_clk_source
 * Description   : Gets default system clock source.
 * END**************************************************************************/
static scg_system_clock_src_t
clock_get_sys_clk_source(clock_names_t src)
{
    scg_system_clock_src_t source;

    switch (src)
    {
        case SIRC_CLK:
        {
            source = SCG_SYSTEM_CLOCK_SRC_SIRC;
        }
        break;

        case FIRC_CLK:
        {
            source = SCG_SYSTEM_CLOCK_SRC_FIRC;
        }
        break;

        case SOSC_CLK:
        {
            source = SCG_SYSTEM_CLOCK_SRC_SYS_OSC;
        }
        break;

        default:
        {
            source = SCG_SYSTEM_CLOCK_SRC_NONE;
            DEV_ASSERT(false);
        }
        break;
    }

    return source;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_set_sirc_clk_source
 * Description   : Sets SIRC clock.
 * END**************************************************************************/
static status_t
clock_set_sirc_clk_source(bool                         b_clock_source_enable,
                          const clock_source_config_t *p_clk_src_config,
                          scg_async_clock_div_t        divider1,
                          scg_async_clock_div_t        divider2)
{
    status_t                  ret_code = STATUS_SUCCESS;
    scg_system_clock_config_t current_sys_clock_config;
    bool                      b_revert_tmp_sysclk_transition = false;
    scg_sirc_config_t         scg_sirc_config;

    /* Check whether the current system clock source is SIRC */
    if (scg_get_system_clk_source(SCG) == ((uint32_t)SCG_SYSTEM_CLOCK_SRC_SIRC))
    {
        if (b_clock_source_enable == false)
        {
            /* Can't disable SIRC, it's used as system clock source */
            ret_code = STATUS_ERROR;
        }
        else
        {
            /* Save the current system clock source configuration */
            clock_get_current_sys_clk_config(&current_sys_clock_config);

            /* Transition to a temporary system clock source */
            ret_code = clock_transition_to_tmp_sys_clk(SCG_SYSTEM_CLOCK_SRC_SIRC);

            b_revert_tmp_sysclk_transition = (ret_code == STATUS_SUCCESS) ? true : false;
        }
    }

    if (ret_code == STATUS_SUCCESS)
    {
        if (p_clk_src_config == NULL)
        {
            ret_code = clock_configure_sirc(true, NULL);
        }
        else
        {
            scg_sirc_config.div1 = divider1;
            scg_sirc_config.div2 = divider2;

            ret_code = clock_configure_sirc(b_clock_source_enable, &scg_sirc_config);
        }

        /* If system clock source was SIRC and SIRC has been set successfully,
         * then transition back to SIRC */
        if ((ret_code == STATUS_SUCCESS) && (b_revert_tmp_sysclk_transition == true))
        {
            ret_code = clock_transition_system_clcok(&current_sys_clock_config);
        }
    }

    return ret_code;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_set_firc_clk_source
 * Description   : Sets FIRC clock.
 * END**************************************************************************/
static status_t
clock_set_firc_clk_source(bool                         b_clock_source_enable,
                          const clock_source_config_t *p_clk_src_config,
                          scg_async_clock_div_t        divider1,
                          scg_async_clock_div_t        divider2)
{
    status_t                  ret_code = STATUS_SUCCESS;
    scg_system_clock_config_t current_sys_clock_config;
    bool                      b_revert_tmp_sysclk_transition = false;
    scg_firc_config_t         scg_firc_config;

    /* Check whether the current system clock source is FIRC */
    if (scg_get_system_clk_source(SCG) == ((uint32_t)SCG_SYSTEM_CLOCK_SRC_FIRC))
    {
        if (b_clock_source_enable == false)
        {
            /* Can't disable FIRC, it's used as system clock source. */
            ret_code = STATUS_ERROR;
        }
        else
        {
            /* Save the current system clock source configuration. */
            clock_get_current_sys_clk_config(&current_sys_clock_config);

            /* Transition to a temporary system clock source. */
            ret_code = clock_transition_to_tmp_sys_clk(SCG_SYSTEM_CLOCK_SRC_FIRC);

            b_revert_tmp_sysclk_transition = (ret_code == STATUS_SUCCESS) ? true : false;
        }
    }

    if (ret_code == STATUS_SUCCESS)
    {
        if (p_clk_src_config == NULL)
        {
            ret_code = clock_configure_firc(b_clock_source_enable, NULL);
        }
        else
        {
            scg_firc_config.div1 = divider1;
            scg_firc_config.div2 = divider2;

            ret_code = clock_configure_firc(b_clock_source_enable, &scg_firc_config);
        }

        /* If system clock source was FIRC and FIRC has been set successfully,
         * then transition back to FIRC */
        if ((ret_code == STATUS_SUCCESS) && (b_revert_tmp_sysclk_transition == true))
        {
            ret_code = clock_transition_system_clcok(&current_sys_clock_config);
        }
    }

    return ret_code;
}

/*FUNCTION**********************************************************************
 * Function Name : clock_set_sosc_clk_source
 * Description   : Sets SOSC clock.
 * END**************************************************************************/
static status_t
clock_set_sosc_clk_source(bool                         b_clock_source_enable,
                          const clock_source_config_t *p_clk_src_config,
                          scg_async_clock_div_t        divider1,
                          scg_async_clock_div_t        divider2)
{
    status_t                  ret_code = STATUS_SUCCESS;
    scg_system_clock_config_t current_sys_clock_config;
    bool                      b_revert_tmp_sysclk_transition = false;
    scg_sosc_config_t         scg_sosc_config;

    /* Check whether the current system clock source is SOSC */
    if (scg_get_system_clk_source(SCG) == ((uint32_t)SCG_SYSTEM_CLOCK_SRC_SYS_OSC))
    {
        if (b_clock_source_enable == false)
        {
            /* Can't disable SOSC, it's used as system clock source. */
            ret_code = STATUS_ERROR;
        }
        else
        {
            /* Save the current system clock source configuration. */
            clock_get_current_sys_clk_config(&current_sys_clock_config);

            /* Transition to a temporary system clock source. */
            ret_code = clock_transition_to_tmp_sys_clk(SCG_SYSTEM_CLOCK_SRC_SYS_OSC);

            b_revert_tmp_sysclk_transition = (ret_code == STATUS_SUCCESS) ? true : false;
        }
    }

    if (ret_code == STATUS_SUCCESS)
    {
        if (p_clk_src_config == NULL)
        {
            ret_code = clock_configure_sosc(b_clock_source_enable, NULL);
        }
        else
        {
            scg_sosc_config.freq = p_clk_src_config->ref_freq;

            scg_sosc_config.div1 = divider1;
            scg_sosc_config.div2 = divider2;

            ret_code = clock_configure_sosc(b_clock_source_enable, &scg_sosc_config);
        }

        /* If system clock source was SOSC and SOSC has been set successfully,
         * then transition back to SOSC */
        if ((ret_code == STATUS_SUCCESS) && (b_revert_tmp_sysclk_transition == true))
        {
            ret_code = clock_transition_system_clcok(&current_sys_clock_config);
        }
    }

    return ret_code;
}

/*** end of file ***/
