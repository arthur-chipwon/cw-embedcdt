/**
 * @file trgmux_driver.c
 * @brief This file provides access to the trgmux module.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stddef.h>
#include "trgmux_driver.h"

/*******************************************************************************
 * Private macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/* Number of possible outputs (target module) for TRGMUX IP */
#define TRGMUX_NUM_TARGET_MODULES \
    ((uint8_t)(sizeof(s_trgmux_target_module) / sizeof(trgmux_target_module_t)))
/* Number of SEL bitfields in one TRGMUX register */
#define TRGMUX_NUM_SEL_BITFIELDS_PER_REG (4U)
/* Get the index of the TRGMUX register */
#define TRGMUX_IDX_REG(x) ((uint8_t)((uint8_t)(x) / TRGMUX_NUM_SEL_BITFIELDS_PER_REG))
/* Get the index of the SEL bitfield inside TRGMUX register */
#define TRGMUX_IDX_SEL_BITFIELD_REG(x) ((uint8_t)((uint8_t)(x) % TRGMUX_NUM_SEL_BITFIELDS_PER_REG))

/*******************************************************************************
 * Static variables
 ******************************************************************************/
/** @brief Table of base addresses for TRGMUX instances. */
static TRGMUX_t *const sp_trgmux_base[TRGMUX_INSTANCE_COUNT] = TRGMUX_BASE_PTRS;

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
/**
 * @brief Restore the TRGMUX module to reset value.
 *
 * This function restores the TRGMUX module to reset value.
 *
 * @param[in] p_base   The TRGMUX peripheral base address
 * @return           Execution status:
 *   STATUS_SUCCESS
 *   STATUS_ERROR    If at least one of the target module register is locked.
 */
static status_t trgmux_reset(TRGMUX_t *const p_base);

/**
 * @brief Configures a source trigger for a target module.
 *
 * This function configures a TRGMUX link between a source trigger and a
 * target module, if the requested target module is not locked.
 *
 * @param[in] p_base           The TRGMUX peripheral base address
 * @param[in] trigger_source  One of the values in the trgmux_trigger_source_t enumeration
 * @param[in] target_module   One of the values in the trgmux_target_module_t enumeration
 */
static void trgmux_set_trig_source(TRGMUX_t *const               p_base,
                                   const trgmux_trigger_source_t trigger_source,
                                   const trgmux_target_module_t  target_module);

/**
 * @brief Get the source trigger configured for a target module.
 *
 * This function returns the TRGMUX source trigger linked to a selected target module.
 *
 * @param[in] p_base         The TRGMUX peripheral base address
 * @param[in] target_module One of the values in the trgmux_target_module_t enumeration
 * @return                 Enum value corresponding to the trigger source configured
 *                          for the given target module
 */
static trgmux_trigger_source_t trgmux_get_trig_source(const TRGMUX_t *const        p_base,
                                                      const trgmux_target_module_t target_module);

/**
 * @brief Lock the TRGMUX register of a target module.
 *
 * This function sets the LK bit of the TRGMUX register corresponding to
 * the selected target module. Please note that some TRGMUX registers can
 * contain up to 4 SEL bitfields, meaning that these registers can be used
 * to configure up to 4 target modules independently. Because the LK bit is
 * only one per register, the configuration of all target modules referred
 * from that register will be locked.
 *
 * @param[in] p_base         The TRGMUX peripheral base address
 * @param[in] target_module One of the values in the trgmux_target_module_t enumeration
 */
static void trgmux_set_lock(TRGMUX_t *const p_base, const trgmux_target_module_t target_module);

/**
 * @brief Get the Lock bit status of the TRGMUX register of a target module.
 *
 * This function gets the value of the LK bit from the TRGMUX register
 * corresponding to the selected target module.
 *
 * @param[in] p_base         The TRGMUX peripheral base address
 * @param[in] target_module One of the values in the trgmux_target_module_t enumeration
 * @return                 true or false depending on the state of the LK bit
 */
static bool trgmux_get_lock(const TRGMUX_t *const        p_base,
                            const trgmux_target_module_t target_module);

/*******************************************************************************
 * Global functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name :     trgmux_init
 * Description   :     This function first resets the source triggers of all
 * TRGMUX target modules to their default values, then configures the TRGMUX with
 * all the user defined in-out mappings. If at least one of the target modules is
 * locked, the function will not change any of the TRGMUX target modules and
 * return an error code. This example shows how to set up the
 * trgmux_user_config_t parameters and how to call the trgmux_init() function
 * with the required parameters: trgmux_user_config_t             trgmuxConfig;
 *   trgmux_inout_mapping_config_t    trgmuxInoutMappingConfig[] =
 *   {
 *      {TRGMUX_TRIG_SOURCE_TRGMUX_IN9,     TRGMUX_TARGET_MODULE_DMA_CH0, false},
 *      {TRGMUX_TRIG_SOURCE_FTM1_EXT_TRIG,  TRGMUX_TARGET_MODULE_TRGMUX_OUT4, true}
 *   };
 *   trgmuxConfig.num_in_out_mapping_configs = 2;
 *   trgmuxConfig.p_in_out_mapping_config     = trgmuxInoutMappingConfig;
 *   trgmux_init(instance, &trgmuxConfig);
 *
 * Implements    : trgmux_init_activity
 *END**************************************************************************/
status_t
trgmux_init(const uint32_t instance, const trgmux_user_config_t *const p_trgmux_user_config)
{
    DEV_ASSERT(instance < TRGMUX_INSTANCE_COUNT);
    DEV_ASSERT(p_trgmux_user_config != NULL);

    status_t  status;
    TRGMUX_t *p_base = sp_trgmux_base[instance];
    uint8_t   count;

    /* Reset source triggers of all TRGMUX target modules to default. */
    status = trgmux_reset(p_base);

    if (status == STATUS_SUCCESS)
    {
        /* Loop through all in-out mappings in the configuration and apply them in TRGMUX */
        for (count = 0U; count < p_trgmux_user_config->num_in_out_mapping_configs; count++)
        {
            trgmux_set_trig_source(
                p_base,
                p_trgmux_user_config->p_in_out_mapping_config[count].trigger_source,
                p_trgmux_user_config->p_in_out_mapping_config[count].target_module);
        }

        /* Loop through all in-out mappings in the configuration and lock them if required */
        for (count = 0U; count < p_trgmux_user_config->num_in_out_mapping_configs; count++)
        {
            if (p_trgmux_user_config->p_in_out_mapping_config[count].b_lock_target_module_reg)
            {
                trgmux_set_lock(p_base,
                                p_trgmux_user_config->p_in_out_mapping_config[count].target_module);
            }
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : trgmux_deinit
 * Description   : Reset to default values the source triggers corresponding to
 *                  all target modules, if none of the target modules is locked.
 *
 * Implements    : trgmux_deinit_activity
 *END**************************************************************************/
status_t
trgmux_deinit(const uint32_t instance)
{
    DEV_ASSERT(instance < TRGMUX_INSTANCE_COUNT);

    TRGMUX_t *p_base = sp_trgmux_base[instance];
    status_t  status;

    /* Reset source triggers of all TRGMUX target modules to default. */
    status = trgmux_reset(p_base);

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : trgmux_set_trig_source_for_target_module
 * Description   : This function configures a TRGMUX link between a source
 *                 trigger and a target module, if the requested target module is not locked.
 *
 * Implements    : trgmux_set_trig_source_for_target_module_activity
 *END**************************************************************************/
status_t
trgmux_set_trig_source_for_target_module(const uint32_t                instance,
                                         const trgmux_trigger_source_t trigger_source,
                                         const trgmux_target_module_t  target_module)
{
    DEV_ASSERT(instance < TRGMUX_INSTANCE_COUNT);

    TRGMUX_t *p_base = sp_trgmux_base[instance];
    status_t  status;
    bool      b_lock;

    b_lock = trgmux_get_lock(p_base, target_module);

    if (b_lock == true)
    {
        status = STATUS_ERROR;
    }
    else
    {
        /* Configure link between trigger source and target module. */
        trgmux_set_trig_source(p_base, trigger_source, target_module);
        status = STATUS_SUCCESS;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : trgmux_get_trig_source_for_target_module
 * Description   : This function returns the TRGMUX source trigger linked to a
 *                  selected target module.
 *
 * Implements    : trgmux_get_trig_source_for_target_module_activity
 *END**************************************************************************/
trgmux_trigger_source_t
trgmux_get_trig_source_for_target_module(const uint32_t               instance,
                                         const trgmux_target_module_t target_module)
{
    DEV_ASSERT(instance < TRGMUX_INSTANCE_COUNT);

    const TRGMUX_t *p_base = sp_trgmux_base[instance];

    return trgmux_get_trig_source(p_base, target_module);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : trgmux_set_lock_for_target_module
 * Description   : This function locks the TRGMUX register of a selected target module.
 *
 * Implements    : trgmux_set_lock_for_target_module_activity
 *END**************************************************************************/
void
trgmux_set_lock_for_target_module(const uint32_t               instance,
                                  const trgmux_target_module_t target_module)
{
    DEV_ASSERT(instance < TRGMUX_INSTANCE_COUNT);

    TRGMUX_t *p_base = sp_trgmux_base[instance];

    trgmux_set_lock(p_base, target_module);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : trgmux_get_lock_for_target_module
 * Description   : This function gets the value of the LK bit from the TRGMUX
 *                  register corresponding to the selected target module.
 *
 * Implements    : trgmux_get_lock_for_target_module_activity
 *END**************************************************************************/
bool
trgmux_get_lock_for_target_module(const uint32_t               instance,
                                  const trgmux_target_module_t target_module)
{
    DEV_ASSERT(instance < TRGMUX_INSTANCE_COUNT);

    const TRGMUX_t *p_base = sp_trgmux_base[instance];

    return trgmux_get_lock(p_base, target_module);
}

/*******************************************************************************
 * Static functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : trgmux_reset
 * Description   : This function restores the TRGMUX module to reset value.
 *
 *END**************************************************************************/
static status_t
trgmux_reset(TRGMUX_t *const p_base)
{
    DEV_ASSERT(p_base != NULL);

    /* Constant array storing the value of all TRGMUX output(target module) identifiers */
    static const trgmux_target_module_t s_trgmux_target_module[] = FEATURE_TRGMUX_TARGET_MODULE;
    uint8_t                             count                    = 0U;
    bool                                b_lock                   = false;
    status_t                            status                   = STATUS_ERROR;

    /* Check if any of the TRGMUX registers is locked */
    while ((count < TRGMUX_NUM_TARGET_MODULES) && (b_lock != true))
    {
        b_lock = trgmux_get_lock(p_base, s_trgmux_target_module[count]);
        count++;
    }

    /* Abort operations if at least one of the target module is locked. */
    if (b_lock == false)
    {
        /* Set all SEL bitfields of all TRGMUX registers to default value */
        for (count = 0U; count < TRGMUX_NUM_TARGET_MODULES; count++)
        {
            /* Write the TRGMUX register */
            p_base->TRGMUXn[TRGMUX_IDX_REG(s_trgmux_target_module[count])] &=
                ~((uint32_t)TRGMUX_TRGMUXn_SEL0_MASK
                  << (TRGMUX_TRGMUXn_SEL1_SHIFT *
                      TRGMUX_IDX_SEL_BITFIELD_REG(s_trgmux_target_module[count])));
        }

        status = STATUS_SUCCESS;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : trgmux_set_trig_source
 * Description   : This function configures a TRGMUX link between a source
 *                  trigger and a target module, if the requested target module is not locked.
 *
 *END**************************************************************************/
static void
trgmux_set_trig_source(TRGMUX_t *const               p_base,
                       const trgmux_trigger_source_t trigger_source,
                       const trgmux_target_module_t  target_module)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t reg_val;
    /* Read value of entire TRGMUX register in a temp variable */
    reg_val = p_base->TRGMUXn[TRGMUX_IDX_REG(target_module)];
    /* Clear first the SEL bitfield inside the TRGMUX register */
    reg_val &= ~((uint32_t)TRGMUX_TRGMUXn_SEL0_MASK
                 << (TRGMUX_TRGMUXn_SEL1_SHIFT * TRGMUX_IDX_SEL_BITFIELD_REG(target_module)));
    /* Configure the SEL bitfield to the desired value */
    reg_val |=
        ((uint32_t)trigger_source)
        << ((uint8_t)(TRGMUX_TRGMUXn_SEL1_SHIFT * TRGMUX_IDX_SEL_BITFIELD_REG(target_module)));
    /* Write back the TRGMUX register */
    p_base->TRGMUXn[TRGMUX_IDX_REG(target_module)] = reg_val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : trgmux_get_trig_source
 * Description   : This function returns the TRGMUX source trigger linked to
 *                  a selected target module.
 *
 *END**************************************************************************/
static trgmux_trigger_source_t
trgmux_get_trig_source(const TRGMUX_t *const p_base, const trgmux_target_module_t target_module)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t trig_source;
    /* Perform the update operation */
    trig_source = ((p_base->TRGMUXn[TRGMUX_IDX_REG(target_module)] >>
                    (TRGMUX_TRGMUXn_SEL1_SHIFT * TRGMUX_IDX_SEL_BITFIELD_REG(target_module))) &
                   TRGMUX_TRGMUXn_SEL0_MASK);

    return (trgmux_trigger_source_t)(trig_source);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : trgmux_set_lock
 * Description   : This function sets the LK bit of the TRGMUX register
 *                  corresponding to the selected target module.
 *
 *END**************************************************************************/
static void
trgmux_set_lock(TRGMUX_t *const p_base, const trgmux_target_module_t target_module)
{
    DEV_ASSERT(p_base != NULL);

    /* Perform the update operation */
    p_base->TRGMUXn[TRGMUX_IDX_REG(target_module)] |= (((uint32_t)1U) << TRGMUX_TRGMUXn_LK_SHIFT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : trgmux_get_lock
 * Description   : Get the Lock bit status of the TRGMUX register of a target module.
 *
 *END**************************************************************************/
static bool
trgmux_get_lock(const TRGMUX_t *const p_base, const trgmux_target_module_t target_module)
{
    DEV_ASSERT(p_base != NULL);

    uint32_t lock_val;
    bool     b_lock;

    /* Get the lock bit value */
    lock_val = ((p_base->TRGMUXn[TRGMUX_IDX_REG(target_module)] & TRGMUX_TRGMUXn_LK_MASK) >>
                TRGMUX_TRGMUXn_LK_SHIFT);

    b_lock = (lock_val == 0U) ? false : true;

    return b_lock;
}

/*** end of file ***/
