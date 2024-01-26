/**
 * @file edma_driver.c
 * @brief This file provides access to the edma module.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "edma_driver.h"
#include "clock_gating.h"
#include "interrupt_manager.h"

/*******************************************************************************
 * Static variables
 ******************************************************************************/
/** @brief Array of base addresses for DMA instances. */
static DMA_t *const sp_edma_base[DMA_INSTANCE_COUNT] = DMA_BASE_PTRS;

/** @brief Array of base addresses for DMAMUX instances. */
static DMAMUX_t *const sp_dmamux_base[DMAMUX_INSTANCE_COUNT] = DMAMUX_BASE_PTRS;

/** @brief Array of default DMA channel interrupt handlers. */
static const IRQn_t s_edma_irq_id[FEATURE_DMA_VIRTUAL_CHANNELS_INTERRUPT_LINES] = DMA_CHN_IRQS;

#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
/** @brief Array for eDMA & DMAMUX clock sources. */
static const clock_names_t s_edma_clock_names[DMA_INSTANCE_COUNT] = FEATURE_DMA_CLOCK_NAMES;
#endif /* (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT) */

/** @brief EDMA global structure to maintain eDMA state */
static edma_state_t *sp_virt_edma_state;

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
static void edma_clear_int_status(uint8_t virtual_channel);
static void edma_clear_stcd(edma_software_tcd_t *stcd);
static void edma_clear_structure(uint8_t *sructPtr, size_t size);
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
static bool edma_valid_transfer_size(edma_transfer_size_t size);
#endif

/*******************************************************************************
 * Global functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : edma_set_cx
 * Description   : Cancels the remaining data transfer.
 *END**************************************************************************/
void
edma_set_cx(DMA_t *p_base)
{
    uint32_t reg_val;
    reg_val = p_base->CR;
    reg_val &= ~(DMA_CR_CX_MASK);
    reg_val |= DMA_CR_CX(1U);
    p_base->CR = reg_val;
    while (((p_base->CR & DMA_CR_CX_MASK) >> DMA_CR_CX_SHIFT) != 0UL)
    {
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_set_ecx
 * Description   : Cancels the remaining data transfer and treat it as error.
 *END**************************************************************************/
void
edma_set_ecx(DMA_t *p_base)
{
    uint32_t reg_val;
    reg_val = p_base->CR;
    reg_val &= ~(DMA_CR_ECX_MASK);
    reg_val |= DMA_CR_ECX(1U);
    p_base->CR = reg_val;
    while (((p_base->CR & DMA_CR_ECX_MASK) >> DMA_CR_ECX_SHIFT) != 0UL)
    {
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_set_dma_request
 * Description   : Enable/Disable dma request for channel or all channels.
 *END**************************************************************************/
void
edma_set_dma_request(DMA_t *p_base, uint8_t channel, bool b_enable)
{
    if (b_enable)
    {
        p_base->SERQ = channel;
    }
    else
    {
        p_base->CERQ = channel;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_tcd_clear_reg
 * Description   : Set registers to 0 for hardware TCD of eDMA channel.
 *END**************************************************************************/
void
edma_tcd_clear_reg(DMA_t *p_base, uint8_t channel)
{
    p_base->TCD[channel].NBYTES.MLNO   = 0U;
    p_base->TCD[channel].SADDR         = 0U;
    p_base->TCD[channel].SOFF          = 0;
    p_base->TCD[channel].ATTR          = 0U;
    p_base->TCD[channel].SLAST         = 0;
    p_base->TCD[channel].DADDR         = 0U;
    p_base->TCD[channel].DOFF          = 0;
    p_base->TCD[channel].CITER.ELINKNO = 0U;
    p_base->TCD[channel].DLASTSGA      = 0;
    p_base->TCD[channel].CSR           = 0U;
    p_base->TCD[channel].BITER.ELINKNO = 0U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_tcd_set_attribute
 * Description   : Configures the transfer attribute for eDMA channel.
 *END**************************************************************************/
void
edma_tcd_set_attribute(DMA_t               *p_base,
                       uint8_t              channel,
                       edma_modulo_t        src_modulo,
                       edma_modulo_t        dest_modulo,
                       edma_transfer_size_t src_transfer_size,
                       edma_transfer_size_t dest_transfer_size)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    uint16_t reg_val;
    reg_val = (uint16_t)(DMA_TCD_ATTR_SMOD(src_modulo) | DMA_TCD_ATTR_SSIZE(src_transfer_size));
    reg_val |= (uint16_t)(DMA_TCD_ATTR_DMOD(dest_modulo) | DMA_TCD_ATTR_DSIZE(dest_transfer_size));
    p_base->TCD[channel].ATTR = reg_val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_tcd_set_nbytes
 * Description   : Configures the nbytes for eDMA channel.
 *END**************************************************************************/
void
edma_tcd_set_nbytes(DMA_t *p_base, uint8_t channel, uint32_t nbytes)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif

    if (((p_base->CR & DMA_CR_EMLM_MASK) >> DMA_CR_EMLM_SHIFT) != 0UL)
    {
        bool b_ml_off_no = false;
        if (((p_base->TCD[channel].NBYTES.MLOFFNO & DMA_TCD_NBYTES_MLOFFNO_SMLOE_MASK) >>
             DMA_TCD_NBYTES_MLOFFNO_SMLOE_SHIFT) == 0UL)
        {
            if (((p_base->TCD[channel].NBYTES.MLOFFNO & DMA_TCD_NBYTES_MLOFFNO_DMLOE_MASK) >>
                 DMA_TCD_NBYTES_MLOFFNO_DMLOE_SHIFT) == 0UL)
            {
                p_base->TCD[channel].NBYTES.MLOFFNO = (nbytes & DMA_TCD_NBYTES_MLOFFNO_NBYTES_MASK);
                b_ml_off_no                         = true;
            }
        }
        if (!b_ml_off_no)
        {
            uint32_t reg_val;
            reg_val = p_base->TCD[channel].NBYTES.MLOFFYES;
            reg_val &= ~(DMA_TCD_NBYTES_MLOFFYES_NBYTES_MASK);
            reg_val |= DMA_TCD_NBYTES_MLOFFYES_NBYTES(nbytes);
            p_base->TCD[channel].NBYTES.MLOFFYES = reg_val;
        }
    }
    else
    {
        p_base->TCD[channel].NBYTES.MLNO = nbytes;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_tcd_set_minor_loop_offset
 * Description   : Configures the minor loop offset for the TCD.
 *END**************************************************************************/
void
edma_tcd_set_minor_loop_offset(DMA_t *p_base, uint8_t channel, int32_t offset)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif

    if (((p_base->CR & DMA_CR_EMLM_MASK) >> DMA_CR_EMLM_SHIFT) != 0UL)
    {
        bool b_ml_off_no = false;
        if (((p_base->TCD[channel].NBYTES.MLOFFNO & DMA_TCD_NBYTES_MLOFFNO_SMLOE_MASK) >>
             DMA_TCD_NBYTES_MLOFFNO_SMLOE_SHIFT) != 0UL)
        {
            b_ml_off_no = true;
        }
        if (((p_base->TCD[channel].NBYTES.MLOFFNO & DMA_TCD_NBYTES_MLOFFNO_DMLOE_MASK) >>
             DMA_TCD_NBYTES_MLOFFNO_DMLOE_SHIFT) != 0UL)
        {
            b_ml_off_no = true;
        }
        if (b_ml_off_no)
        {
            uint32_t reg_val;
            reg_val = p_base->TCD[channel].NBYTES.MLOFFYES;
            reg_val &= ~(DMA_TCD_NBYTES_MLOFFYES_MLOFF_MASK);
            reg_val |= DMA_TCD_NBYTES_MLOFFYES_MLOFF(offset);
            p_base->TCD[channel].NBYTES.MLOFFYES = reg_val;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_tcd_set_scatter_gather_link
 * Description   : Configures the memory address of the next TCD, in
 *                 Scatter/Gather mode.
 *
 *END**************************************************************************/
void
edma_tcd_set_scatter_gather_link(DMA_t *p_base, uint8_t channel, uint32_t next_tcd_addr)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    p_base->TCD[channel].DLASTSGA = next_tcd_addr;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_tcd_set_channel_minor_link
 * Description   : Set Channel minor link for hardware TCD.
 *END**************************************************************************/
void
edma_tcd_set_channel_minor_link(DMA_t   *p_base,
                                uint8_t  channel,
                                uint32_t link_channel,
                                bool     b_enable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
    DEV_ASSERT(link_channel < FEATURE_DMA_CHANNELS);
#endif

    uint16_t reg_val;
    reg_val = p_base->TCD[channel].BITER.ELINKYES;
    reg_val &= (uint16_t) ~(DMA_TCD_BITER_ELINKYES_ELINK_MASK);
    reg_val |= (uint16_t)DMA_TCD_BITER_ELINKYES_ELINK(b_enable ? 1UL : 0UL);
    p_base->TCD[channel].BITER.ELINKYES = reg_val;
    reg_val                             = p_base->TCD[channel].CITER.ELINKYES;
    reg_val &= (uint16_t) ~(DMA_TCD_CITER_ELINKYES_ELINK_MASK);
    reg_val |= (uint16_t)DMA_TCD_CITER_ELINKYES_ELINK(b_enable ? 1UL : 0UL);
    p_base->TCD[channel].CITER.ELINKYES = reg_val;

    if (b_enable)
    {
        reg_val = p_base->TCD[channel].BITER.ELINKYES;
        reg_val &= (uint16_t) ~(DMA_TCD_BITER_ELINKYES_LINKCH_MASK);
        reg_val |= (uint16_t)DMA_TCD_BITER_ELINKYES_LINKCH(link_channel);
        p_base->TCD[channel].BITER.ELINKYES = reg_val;

        reg_val = p_base->TCD[channel].CITER.ELINKYES;
        reg_val &= (uint16_t) ~(DMA_TCD_CITER_ELINKYES_LINKCH_MASK);
        reg_val |= (uint16_t)DMA_TCD_CITER_ELINKYES_LINKCH(link_channel);
        p_base->TCD[channel].CITER.ELINKYES = reg_val;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_TCD_TCDSetMajorCount
 * Description   : Sets the major iteration count according to minor loop
 *                 channel link setting.
 *END**************************************************************************/
void
edma_tcd_set_major_count(DMA_t *p_base, uint8_t channel, uint32_t count)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    uint16_t reg_val = 0;
    if ((p_base->TCD[channel].BITER.ELINKNO & DMA_TCD_BITER_ELINKNO_ELINK_MASK) ==
        DMA_TCD_BITER_ELINKNO_ELINK_MASK)
    {
        reg_val = p_base->TCD[channel].BITER.ELINKYES;
        reg_val &= (uint16_t) ~(DMA_TCD_BITER_ELINKYES_BITER_MASK);
        reg_val |= (uint16_t)DMA_TCD_BITER_ELINKYES_BITER(count);
        p_base->TCD[channel].BITER.ELINKYES = reg_val;

        reg_val = p_base->TCD[channel].CITER.ELINKYES;
        reg_val &= (uint16_t) ~(DMA_TCD_CITER_ELINKYES_CITER_LE_MASK);
        reg_val |= (uint16_t)DMA_TCD_CITER_ELINKYES_CITER_LE(count);
        p_base->TCD[channel].CITER.ELINKYES = reg_val;
    }
    else
    {
        reg_val = p_base->TCD[channel].BITER.ELINKNO;
        reg_val &= (uint16_t) ~(DMA_TCD_BITER_ELINKNO_BITER_MASK);
        reg_val |= (uint16_t)DMA_TCD_BITER_ELINKNO_BITER(count);
        p_base->TCD[channel].BITER.ELINKNO = reg_val;

        reg_val = p_base->TCD[channel].CITER.ELINKNO;
        reg_val &= (uint16_t) ~(DMA_TCD_CITER_ELINKNO_CITER_MASK);
        reg_val |= (uint16_t)DMA_TCD_CITER_ELINKNO_CITER(count);
        p_base->TCD[channel].CITER.ELINKNO = reg_val;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_tcd_get_current_major_count
 * Description   : Gets the current major iteration count according to minor
 *                 loop channel link setting.
 *END**************************************************************************/
uint32_t
edma_tcd_get_current_major_count(const DMA_t *p_base, uint8_t channel)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    uint16_t result = 0U;
    if ((p_base->TCD[channel].BITER.ELINKNO & DMA_TCD_BITER_ELINKNO_ELINK_MASK) ==
        DMA_TCD_BITER_ELINKNO_ELINK_MASK)
    {
        result = (uint16_t)((p_base->TCD[channel].CITER.ELINKYES &
                             DMA_TCD_CITER_ELINKYES_CITER_LE_MASK) >>
                            DMA_TCD_CITER_ELINKYES_CITER_LE_SHIFT);
    }
    else
    {
        result =
            (uint16_t)((p_base->TCD[channel].CITER.ELINKNO & DMA_TCD_CITER_ELINKNO_CITER_MASK) >>
                       DMA_TCD_CITER_ELINKNO_CITER_SHIFT);
    }
    return (uint32_t)result;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_init_set
 * Description   : Initializes eDMA module to known state.
 *END**************************************************************************/
void
edma_init_set(DMA_t *p_base)
{
    uint8_t idx;
    /* Clear the bit of CR register */
    uint32_t reg_val;
    reg_val = p_base->CR;
    reg_val &= ~(DMA_CR_CLM_MASK);
    reg_val &= ~(DMA_CR_CX_MASK);
    reg_val &= ~(DMA_CR_ECX_MASK);
    reg_val &= ~(DMA_CR_EDBG_MASK);
    reg_val &= ~(DMA_CR_EMLM_MASK);
    reg_val &= ~(DMA_CR_ERCA_MASK);
    p_base->CR = reg_val;

    for (idx = 0; idx < FEATURE_DMA_CHANNELS; idx++)
    {
        edma_tcd_clear_reg(p_base, idx);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : dmamux_init
 * Description   : Initialize the dmamux module to the reset state.
 *END**************************************************************************/
void
dmamux_init(DMAMUX_t *p_base)
{
    uint8_t idx;

    for (idx = 0; idx < FEATURE_DMAMUX_CHANNELS; idx++)
    {
        p_base->CHCFG[idx] = 0;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_init
 * Description   : Initializes the eDMA module.
 *
 * Implements    : edma_init_activity
 *END**************************************************************************/
status_t
edma_init(edma_state_t                      *p_edma_state,
          const edma_user_config_t          *p_user_config,
          edma_chn_state_t *const            p_chn_state_array[],
          const edma_channel_config_t *const p_chn_config_array[],
          uint32_t                           chn_count)
{
    uint32_t index           = 0U;
    DMA_t   *p_edma_reg_base = NULL;
    IRQn_t   irq_number      = NotAvail_IRQn;
    status_t edma_status     = STATUS_SUCCESS;
    status_t chn_init_status = STATUS_SUCCESS;
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    uint32_t freq                 = 0U;
    status_t clock_manager_status = STATUS_SUCCESS;
#endif

    /* Check the state and configuration structure pointers are valid */
    DEV_ASSERT((p_edma_state != NULL) && (p_user_config != NULL));

    /* Check the module has not already been initialized */
    DEV_ASSERT(sp_virt_edma_state == NULL);

#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    /* Check that eDMA modules are clock gated on */
    for (index = 0U; index < (uint32_t)DMA_INSTANCE_COUNT; index++)
    {
        clock_manager_status = clock_get_freq(s_edma_clock_names[index], &freq);
        DEV_ASSERT(clock_manager_status == STATUS_SUCCESS);
    }
#endif /* (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT) */

    /* Save the runtime state structure for the driver */
    sp_virt_edma_state = p_edma_state;

    /* Clear the state structure. */
    edma_clear_structure((uint8_t *)sp_virt_edma_state, sizeof(edma_state_t));

    /* Init all DMA instances */
    for (index = 0U; index < (uint32_t)DMA_INSTANCE_COUNT; index++)
    {
        p_edma_reg_base = sp_edma_base[index];

        /* Init eDMA module on hardware level. */
        edma_init_set(p_edma_reg_base);

        /* Set arbitration mode */
        edma_set_channel_arbitration_mode(p_edma_reg_base, p_user_config->chn_arbitration);
        /* Set 'Halt on error' configuration */
        edma_set_halt_on_error(p_edma_reg_base, p_user_config->b_halt_on_error);
    }

    /* Register all edma channel interrupt handlers into vector table. */
    for (index = 0U; index < (uint32_t)FEATURE_DMA_VIRTUAL_CHANNELS_INTERRUPT_LINES; index++)
    {
        /* Enable channel interrupt ID. */
        irq_number = s_edma_irq_id[index];
        int_enable_irq(irq_number);
    }

    /* Initialize all DMAMUX instances */
    for (index = 0U; index < (uint32_t)DMAMUX_INSTANCE_COUNT; index++)
    {
        dmamux_init(sp_dmamux_base[index]);
    }

    /* Initialize the channels based on configuration list */
    if ((p_chn_state_array != NULL) && (p_chn_config_array != NULL))
    {
        for (index = 0U; index < chn_count; index++)
        {
            chn_init_status =
                edma_channel_init(p_chn_state_array[index], p_chn_config_array[index]);
            if (chn_init_status != STATUS_SUCCESS)
            {
                edma_status = chn_init_status;
            }
        }
    }

    return edma_status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_deinit
 * Description   : Deinitialize EDMA.
 *
 * Implements    : edma_deinit_activity
 *END**************************************************************************/
status_t
edma_deinit(void)
{
    uint32_t                index       = 0U;
    IRQn_t                  irq_number  = NotAvail_IRQn;
    const edma_chn_state_t *p_chn_state = NULL;

    if (sp_virt_edma_state != NULL)
    {
        /* Release all edma channel. */
        for (index = 0U; index < (uint32_t)FEATURE_DMA_VIRTUAL_CHANNELS; index++)
        {
            /* Release all channels. */
            p_chn_state = sp_virt_edma_state->p_virt_chn_state[index];
            if (p_chn_state != NULL)
            {
                (void)edma_release_channel(p_chn_state->virt_chn);
            }
        }
        for (index = 0U; index < (uint32_t)FEATURE_DMA_VIRTUAL_CHANNELS_INTERRUPT_LINES; index++)
        {
            /* Disable channel interrupts. */
            irq_number = s_edma_irq_id[index];
            int_disable_irq(irq_number);
        }
    }

    sp_virt_edma_state = NULL;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_channel_init
 * Description   : Initialize EDMA channel.
 *
 * Implements    : edma_channel_init_activity
 *END**************************************************************************/
status_t
edma_channel_init(edma_chn_state_t            *p_edma_channel_state,
                  const edma_channel_config_t *p_edma_channel_config)
{
    /* Check the state and configuration structure pointers are valid */
    DEV_ASSERT((p_edma_channel_state != NULL) && (p_edma_channel_config != NULL));

    /* Check if the module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check the channel has not already been allocated */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[p_edma_channel_config->virt_chn_config] ==
               NULL);

    /* Check if the channel defined by user in the channel configuration
     * structure is valid */
    DEV_ASSERT(p_edma_channel_config->virt_chn_config < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance =
        (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(p_edma_channel_config->virt_chn_config);

    /* Get DMA channel from virtual channel */
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(p_edma_channel_config->virt_chn_config);

    /* Get virtual channel value */
    uint8_t virtual_channel = p_edma_channel_config->virt_chn_config;

    /* Get status */
    status_t ret_status = STATUS_SUCCESS;

    /* Load corresponding DMA instance pointer */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];

    /* Reset the channel state structure to default value. */
    edma_clear_structure((uint8_t *)p_edma_channel_state, sizeof(edma_chn_state_t));

    ret_status = edma_set_channel_request_and_trigger(p_edma_channel_config->virt_chn_config,
                                                      (uint8_t)p_edma_channel_config->source,
                                                      p_edma_channel_config->b_enable_trigger);

    /* Clear the TCD registers for this channel */
    edma_tcd_clear_reg(p_edma_reg_base, dma_channel);

    if (ret_status == STATUS_SUCCESS)
    {
        /* Set virtual channel state */
        sp_virt_edma_state->p_virt_chn_state[virtual_channel] = p_edma_channel_state;
        /* Set virtual channel value */
        sp_virt_edma_state->p_virt_chn_state[virtual_channel]->virt_chn = virtual_channel;
        /* Set virtual channel status to normal */
        sp_virt_edma_state->p_virt_chn_state[virtual_channel]->status = EDMA_CHN_NORMAL;

        /* Set the channel priority, as defined in the configuration, only if
         * fixed arbitration mode is selected */
        if ((edma_get_channel_arbitration_mode(p_edma_reg_base) ==
             EDMA_ARBITRATION_FIXED_PRIORITY) &&
            (p_edma_channel_config->channel_priority != EDMA_CHN_DEFAULT_PRIORITY))
        {
            edma_set_channel_priority(
                p_edma_reg_base, dma_channel, p_edma_channel_config->channel_priority);
        }
        /* Install the user callback */
        ret_status = edma_install_callback(p_edma_channel_config->virt_chn_config,
                                           p_edma_channel_config->callback,
                                           p_edma_channel_config->p_callback_param);
    }

    return ret_status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_install_callback
 * Description   : Register callback function and parameter.
 *
 * Implements    : edma_install_callback_activity
 *END**************************************************************************/
status_t
edma_install_callback(uint8_t virtual_channel, edma_callback_t callback, void *p_parameter)
{
    /* Check the channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check the channel is allocated */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    sp_virt_edma_state->p_virt_chn_state[virtual_channel]->callback    = callback;
    sp_virt_edma_state->p_virt_chn_state[virtual_channel]->p_parameter = p_parameter;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_release_channel
 * Description   : Free eDMA channel's hardware and software resource.
 *
 * Implements    : edma_release_channel_activity
 *END**************************************************************************/
status_t
edma_release_channel(uint8_t virtual_channel)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check the DMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Get pointer to channel state */
    edma_chn_state_t *p_chn_state = sp_virt_edma_state->p_virt_chn_state[virtual_channel];

    /* Check that virtual channel is initialized */
    DEV_ASSERT(p_chn_state != NULL);

    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];

    /* Stop edma channel. */
    edma_set_dma_request(p_edma_reg_base, dma_channel, false);

    /* Reset the channel state structure to default value. */
    edma_clear_structure((uint8_t *)p_chn_state, sizeof(edma_chn_state_t));

    sp_virt_edma_state->p_virt_chn_state[virtual_channel] = NULL;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_irq_handler
 * Description   : EDMA IRQ handler.
 *END**************************************************************************/
void
edma_irq_handler(uint8_t virtual_channel)
{
    const edma_chn_state_t *p_chn_state = sp_virt_edma_state->p_virt_chn_state[virtual_channel];

    edma_clear_int_status(virtual_channel);

    if (p_chn_state != NULL)
    {
        if (p_chn_state->callback != NULL)
        {
            p_chn_state->callback(p_chn_state->p_parameter, p_chn_state->status);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_error_irq_handler
 * Description   : EDMA error IRQ handler
 *END**************************************************************************/
void
edma_error_irq_handler(uint8_t virtual_channel)
{
    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_set_dma_request(p_edma_reg_base, dma_channel, false);
    edma_chn_state_t *p_chn_state = sp_virt_edma_state->p_virt_chn_state[virtual_channel];
    if (p_chn_state != NULL)
    {
        edma_clear_int_status(virtual_channel);
        edma_clear_error_int_status_flag(p_edma_reg_base, dma_channel);
        p_chn_state->status = EDMA_CHN_ERROR;
        if (p_chn_state->callback != NULL)
        {
            p_chn_state->callback(p_chn_state->p_parameter, p_chn_state->status);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_config_single_block_transfer
 * Description   : Configures a DMA single block transfer.
 *
 * Implements    : edma_config_single_block_transfer_activity
 *END**************************************************************************/
status_t
edma_config_single_block_transfer(uint8_t              virtual_channel,
                                  edma_transfer_type_t type,
                                  uint32_t             src_addr,
                                  uint32_t             dest_addr,
                                  edma_transfer_size_t transfer_size,
                                  uint32_t             data_buffer_size)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    /* Check if the value passed for 'transfer_size' is valid */
    DEV_ASSERT(edma_valid_transfer_size(transfer_size));
#endif

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    DMA_t   *p_edma_reg_base = sp_edma_base[dma_instance];
    status_t ret_status      = STATUS_SUCCESS;

    /* Compute the transfer offset, based on transfer size.
     * The number of bytes transferred in each source read/destination write
     * is obtained with the following formula:
     *    source_read_size = 2^SSIZE
     *    destination_write_size = 2^DSIZE
     */
    uint8_t transfer_offset = (uint8_t)(1U << ((uint8_t)transfer_size));

    /* Check that source address and destination address are valid */
    DEV_ASSERT((src_addr % transfer_offset) == 0U);
    DEV_ASSERT((dest_addr % transfer_offset) == 0U);

    /* The number of bytes to be transferred (buffer size) must
     * be a multiple of the source read/destination write size
     */
    if ((data_buffer_size % transfer_offset) != 0U)
    {
        ret_status = STATUS_ERROR;
    }

    if (ret_status == STATUS_SUCCESS)
    {
        /* Clear transfer control descriptor for the current channel */
        edma_tcd_clear_reg(p_edma_reg_base, dma_channel);

#ifdef FEATURE_DMA_ENGINE_STALL
        /* Configure the DMA Engine to stall for a number of cycles after each
         * R/W */
        edma_tcd_set_engine_stall(p_edma_reg_base, dma_channel, EDMA_ENGINE_STALL_4_CYCLES);
#endif

        edma_set_minor_loop_mapping(p_edma_reg_base, true);

        /* Configure source and destination addresses */
        edma_tcd_set_src_addr(p_edma_reg_base, dma_channel, src_addr);
        edma_tcd_set_dest_addr(p_edma_reg_base, dma_channel, dest_addr);

        /* Set transfer size (1B/2B/4B/16B/32B) */
        edma_tcd_set_attribute(p_edma_reg_base,
                               dma_channel,
                               EDMA_MODULO_OFF,
                               EDMA_MODULO_OFF,
                               transfer_size,
                               transfer_size);

        /* Configure source/destination offset. */
        switch (type)
        {
            case EDMA_TRANSFER_PERIPH2MEM:
                edma_tcd_set_src_offset(p_edma_reg_base, dma_channel, 0);
                edma_tcd_set_dest_offset(p_edma_reg_base, dma_channel, (int8_t)transfer_offset);
                break;
            case EDMA_TRANSFER_MEM2PERIPH:
                edma_tcd_set_src_offset(p_edma_reg_base, dma_channel, (int8_t)transfer_offset);
                edma_tcd_set_dest_offset(p_edma_reg_base, dma_channel, 0);
                break;
            case EDMA_TRANSFER_MEM2MEM:
                edma_tcd_set_src_offset(p_edma_reg_base, dma_channel, (int8_t)transfer_offset);
                edma_tcd_set_dest_offset(p_edma_reg_base, dma_channel, (int8_t)transfer_offset);
                break;
            case EDMA_TRANSFER_PERIPH2PERIPH:
                edma_tcd_set_src_offset(p_edma_reg_base, dma_channel, 0);
                edma_tcd_set_dest_offset(p_edma_reg_base, dma_channel, 0);
                break;
            default:
                /* This should never be reached - all the possible values have
                 * been handled. */
                break;
        }

        /* Set the total number of bytes to be transfered */
        edma_tcd_set_nbytes(p_edma_reg_base, dma_channel, data_buffer_size);

        /* Set major iteration count to 1 (single block mode) */
        edma_tcd_set_major_count(p_edma_reg_base, dma_channel, 1U);

        /* Enable interrupt when the transfer completes */
        edma_tcd_set_major_complete_int(p_edma_reg_base, dma_channel, true);

        /* Set virtual channel status to normal */
        sp_virt_edma_state->p_virt_chn_state[virtual_channel]->status = EDMA_CHN_NORMAL;
    }

    return ret_status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_config_multi_block_transfer
 * Description   : Configures a DMA single block transfer.
 *
 * Implements    : edma_config_multi_block_transfer_activity
 *END**************************************************************************/
status_t
edma_config_multi_block_transfer(uint8_t              virtual_channel,
                                 edma_transfer_type_t type,
                                 uint32_t             src_addr,
                                 uint32_t             dest_addr,
                                 edma_transfer_size_t transfer_size,
                                 uint32_t             block_size,
                                 uint32_t             block_count,
                                 bool                 b_disable_req_on_completion)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    status_t ret_status = STATUS_SUCCESS;

#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    /* Compute the transfer offset, based on transfer size.
     * The number of bytes transferred in each source read/destination write
     * is obtained with the following formula:
     *    source_read_size = 2^SSIZE
     *    destination_write_size = 2^DSIZE
     */
    uint8_t transfer_offset = (uint8_t)(1U << ((uint8_t)transfer_size));

    /* Check that source address and destination address are valid */
    DEV_ASSERT((src_addr % transfer_offset) == 0U);
    DEV_ASSERT((dest_addr % transfer_offset) == 0U);
#endif /* (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT) */

    /* Configure the transfer for one data block */
    ret_status = edma_config_single_block_transfer(
        virtual_channel, type, src_addr, dest_addr, transfer_size, block_size);

    if (ret_status == STATUS_SUCCESS)
    {
        DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];

        /* Set the number of data blocks */
        edma_tcd_set_major_count(p_edma_reg_base, dma_channel, block_count);

        /* Enable/disable requests upon completion */
        edma_tcd_set_disable_dma_request(p_edma_reg_base, dma_channel, b_disable_req_on_completion);
    }

    return ret_status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : edma_config_loop_transfer
 * Description   : Configures the DMA transfer in a loop.
 *
 * Implements    : edma_config_loop_transfer_activity
 *END**************************************************************************/
status_t
edma_config_loop_transfer(uint8_t virtual_channel, const edma_transfer_config_t *p_transfer_config)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Check the transfer configuration structure is valid */
    DEV_ASSERT(p_transfer_config != NULL);

#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    /* Compute the transfer offset, based on transfer size.
     * The number of bytes transferred in each source read/destination write
     * is obtained with the following formula:
     *    source_read_size = 2^SSIZE
     *    destination_write_size = 2^DSIZE
     */
    uint8_t src_transfer_offset = (uint8_t)(1U << ((uint8_t)p_transfer_config->src_transfer_size));
    uint8_t dest_transfer_offset =
        (uint8_t)(1U << ((uint8_t)p_transfer_config->dest_transfer_size));

    /* Check that source address and destination address are valid */
    DEV_ASSERT((p_transfer_config->src_addr % src_transfer_offset) == 0U);
    DEV_ASSERT((p_transfer_config->dest_addr % dest_transfer_offset) == 0U);
#endif /* (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT) */

    /* Check the minor/major loop properties are defined */
    DEV_ASSERT(p_transfer_config->p_loop_transfer_config != NULL);

    /* If the modulo feature is enabled, check alignment of addresses */
    DEV_ASSERT((p_transfer_config->src_modulo == EDMA_MODULO_OFF) ||
               ((p_transfer_config->src_addr %
                 (((uint32_t)1U) << (uint32_t)p_transfer_config->src_modulo)) == 0U));
    DEV_ASSERT((p_transfer_config->dest_modulo == EDMA_MODULO_OFF) ||
               ((p_transfer_config->dest_addr %
                 (((uint32_t)1U) << (uint32_t)p_transfer_config->dest_modulo)) == 0U));

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];

    edma_set_minor_loop_mapping(p_edma_reg_base, true);

    /* Write the configuration in the transfer control descriptor registers */
    edma_push_config_to_reg(virtual_channel, p_transfer_config);

    /* Set virtual channel status to normal */
    sp_virt_edma_state->p_virt_chn_state[virtual_channel]->status = EDMA_CHN_NORMAL;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_config_scatter_gather_transfer
 * Description   : Configure eDMA for scatter/gather operation
 *
 * Implements    : edma_config_scatter_gather_transfer_activity
 *END**************************************************************************/
status_t
edma_config_scatter_gather_transfer(uint8_t                           virtual_channel,
                                    edma_software_tcd_t              *p_stcd,
                                    edma_transfer_size_t              transfer_size,
                                    uint32_t                          bytes_on_each_request,
                                    const edma_scatter_gather_list_t *p_src_list,
                                    const edma_scatter_gather_list_t *p_dest_list,
                                    uint8_t                           tcd_count)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Check the input arrays for scatter/gather operation are valid */
    DEV_ASSERT((p_stcd != NULL) && (p_src_list != NULL) && (p_dest_list != NULL));

#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    /* Check if the value passed for 'transfer_size' is valid */
    DEV_ASSERT(edma_valid_transfer_size(transfer_size));
#endif

    uint8_t                     idx                = 0U;
    uint16_t                    transfer_offset    = 0U;
    uint32_t                    stcd_aligned_addr  = STCD_ADDR(p_stcd);
    edma_software_tcd_t        *p_edma_sw_tcd_addr = (edma_software_tcd_t *)stcd_aligned_addr;
    edma_loop_transfer_config_t edma_loop_config;
    edma_transfer_config_t      edma_transfer_config;
    status_t                    ret_status = STATUS_SUCCESS;

    /* Set virtual channel status to normal */
    sp_virt_edma_state->p_virt_chn_state[virtual_channel]->status = EDMA_CHN_NORMAL;

    /* Compute the transfer offset, based on transfer size.
     * The number of bytes transferred in each source read/destination write
     * is obtained with the following formula:
     *    source_read_size = 2^SSIZE
     *    destination_write_size = 2^DSIZE
     */
    transfer_offset = (uint16_t)(1UL << ((uint16_t)transfer_size));

    /* The number of bytes to be transferred on each request must
     * be a multiple of the source read/destination write size
     */
    if ((bytes_on_each_request % transfer_offset) != 0U)
    {
        ret_status = STATUS_ERROR;
    }

    /* Clear the configuration structures before initializing them. */
    edma_clear_structure((uint8_t *)(&edma_transfer_config), sizeof(edma_transfer_config_t));
    edma_clear_structure((uint8_t *)(&edma_loop_config), sizeof(edma_loop_transfer_config_t));

    /* Configure the transfer for scatter/gather mode. */
    edma_transfer_config.src_last_addr_adjust                        = 0;
    edma_transfer_config.dest_last_addr_adjust                       = 0;
    edma_transfer_config.src_modulo                                  = EDMA_MODULO_OFF;
    edma_transfer_config.dest_modulo                                 = EDMA_MODULO_OFF;
    edma_transfer_config.src_transfer_size                           = transfer_size;
    edma_transfer_config.dest_transfer_size                          = transfer_size;
    edma_transfer_config.minor_byte_transfer_count                   = bytes_on_each_request;
    edma_transfer_config.b_interrupt_enable                          = true;
    edma_transfer_config.b_scatter_gather_enable                     = true;
    edma_transfer_config.p_loop_transfer_config                      = &edma_loop_config;
    edma_transfer_config.p_loop_transfer_config->b_src_offset_enable = false;
    edma_transfer_config.p_loop_transfer_config->b_dst_offset_enable = false;
    edma_transfer_config.p_loop_transfer_config->b_minor_loop_chn_link_enable = false;
    edma_transfer_config.p_loop_transfer_config->b_major_loop_chn_link_enable = false;

    /* Copy scatter/gather lists to transfer configuration*/
    for (idx = 0U; (idx < tcd_count) && (ret_status == STATUS_SUCCESS); idx++)
    {
        edma_transfer_config.src_addr  = p_src_list[idx].address;
        edma_transfer_config.dest_addr = p_dest_list[idx].address;
        if ((p_src_list[idx].length != p_dest_list[idx].length) ||
            (p_src_list[idx].type != p_dest_list[idx].type))
        {
            ret_status = STATUS_ERROR;
            break;
        }
        edma_transfer_config.p_loop_transfer_config->major_loop_iteration_count =
            p_src_list[idx].length / bytes_on_each_request;

        switch (p_src_list[idx].type)
        {
            case EDMA_TRANSFER_PERIPH2MEM:
                /* Configure Source Read. */
                edma_transfer_config.src_offset = 0;
                /* Configure Dest Write. */
                edma_transfer_config.dest_offset = (int16_t)transfer_offset;
                break;
            case EDMA_TRANSFER_MEM2PERIPH:
                /* Configure Source Read. */
                edma_transfer_config.src_offset = (int16_t)transfer_offset;
                /* Configure Dest Write. */
                edma_transfer_config.dest_offset = 0;
                break;
            case EDMA_TRANSFER_MEM2MEM:
                /* Configure Source Read. */
                edma_transfer_config.src_offset = (int16_t)transfer_offset;
                /* Configure Dest Write. */
                edma_transfer_config.dest_offset = (int16_t)transfer_offset;
                break;
            case EDMA_TRANSFER_PERIPH2PERIPH:
                /* Configure Source Read. */
                edma_transfer_config.src_offset = 0;
                /* Configure Dest Write. */
                edma_transfer_config.dest_offset = 0;
                break;
            default:
                /* This should never be reached - all the possible values have
                 * been handled. */
                break;
        }

        /* Configure the pointer to next software TCD structure; for the last
         * one, this address should be 0 */
        if (idx == ((uint8_t)(tcd_count - 1U)))
        {
            edma_transfer_config.scatter_gather_next_desc_addr = 0U;
        }
        else
        {
            edma_software_tcd_t *p_tcd_next_addr               = &p_edma_sw_tcd_addr[idx];
            edma_transfer_config.scatter_gather_next_desc_addr = ((uint32_t)p_tcd_next_addr);
        }

        if (idx == 0U)
        {
            /* Push the configuration for the first descriptor to registers */
            edma_push_config_to_reg(virtual_channel, &edma_transfer_config);
        }
        else
        {
            /* Copy configuration to software TCD structure */
            edma_push_config_to_stcd(&edma_transfer_config, &p_edma_sw_tcd_addr[idx - 1U]);
        }
    }

    return ret_status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_start_channel
 * Description   : Starts an eDMA channel.
 *
 * Implements    : edma_start_channel_activity
 *END**************************************************************************/
status_t
edma_start_channel(uint8_t virtual_channel)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Enable requests for current channel */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_set_dma_request(p_edma_reg_base, dma_channel, true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_stop_channel
 * Description   : Stops an eDMA channel.
 *
 * Implements    : edma_stop_channel_activity
 *END**************************************************************************/
status_t
edma_stop_channel(uint8_t virtual_channel)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Disable requests for current channel */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_set_dma_request(p_edma_reg_base, dma_channel, false);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_set_channel_request_and_trigger
 * Description   : Sets DMA channel request source in DMAMUX and controls
 *                 the DMA channel periodic triggering.
 *
 * Implements    : edma_set_channel_request_and_trigger_activity
 *END**************************************************************************/
status_t
edma_set_channel_request_and_trigger(uint8_t virtual_channel, uint8_t request, bool b_enable_trigger)
{
    /* Check the virtual channel number is valid */
    DEV_ASSERT(virtual_channel < (uint32_t)FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Retrieve the DMAMUX instance serving this request */
    uint8_t dmamux_instance = (uint8_t)FEATURE_DMAMUX_REQ_SRC_TO_INSTANCE(request);

    /* Get request index for the corresponding DMAMUX instance */
    uint8_t dmamux_request = (uint8_t)FEATURE_DMAMUX_REQ_SRC_TO_CH(request);

    /* Get DMAMUX channel for the selected request */
    uint8_t dmamux_channel = (uint8_t)FEATURE_DMAMUX_DMA_CH_TO_CH(virtual_channel);

    /* Retrieve the appropriate DMAMUX instance */
    DMAMUX_t *p_dmamux_reg_base = sp_dmamux_base[dmamux_instance];

    /* Set request and trigger */
    dmamux_set_channel_cmd(p_dmamux_reg_base, dmamux_channel, false);
    dmamux_set_channel_source(p_dmamux_reg_base, dmamux_channel, dmamux_request);
    dmamux_set_channel_trigger(p_dmamux_reg_base, dmamux_channel, b_enable_trigger);
    dmamux_set_channel_cmd(p_dmamux_reg_base, dmamux_channel, true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_clear_tcd
 * Description   : Clears all registers to 0 for the hardware TCD.
 *
 * Implements    : edma_clear_tcd_activity
 *END**************************************************************************/
void
edma_clear_tcd(uint8_t virtual_channel)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Clear the TCD memory */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_tcd_clear_reg(p_edma_reg_base, dma_channel);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_set_src_addr
 * Description   : Configures the source address for the eDMA channel.
 *
 * Implements    : edma_set_src_addr_activity
 *END**************************************************************************/
void
edma_set_src_addr(uint8_t virtual_channel, uint32_t address)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Set channel TCD source address */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_tcd_set_src_addr(p_edma_reg_base, dma_channel, address);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_set_src_offset
 * Description   : Configures the source address signed offset for the eDMA channel.
 *
 * Implements    : edma_set_src_offset_activity
 *END**************************************************************************/
void
edma_set_src_offset(uint8_t virtual_channel, int16_t offset)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Set channel TCD source offset */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_tcd_set_src_offset(p_edma_reg_base, dma_channel, offset);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_set_src_transfer_size
 * Description   : Configures the source read data chunk size (transferred in a
 *                 read sequence).
 *
 * Implements    : edma_set_src_transfer_size_activity
 *END**************************************************************************/
void
edma_set_src_transfer_size(uint8_t virtual_channel, edma_transfer_size_t size)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Set channel TCD source transfer size */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_tcd_set_src_transfer_size(p_edma_reg_base, dma_channel, size);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_set_src_last_addr_adjust
 * Description   : Configures the source address last adjustment.
 *
 * Implements    : edma_set_src_last_addr_adjust_activity
 *END**************************************************************************/
void
edma_set_src_last_addr_adjust(uint8_t virtual_channel, int32_t adjust)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Set channel TCD source last adjustment */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_tcd_set_src_last_adjust(p_edma_reg_base, dma_channel, adjust);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_set_dest_last_addr_adjust
 * Description   : Configures the source address last adjustment.
 *
 * Implements    : edma_set_dest_last_addr_adjust_activity
 *END**************************************************************************/
void
edma_set_dest_last_addr_adjust(uint8_t virtual_channel, int32_t adjust)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Set channel TCD source last adjustment */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_tcd_set_dest_last_adjust(p_edma_reg_base, dma_channel, adjust);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_set_dest_addr
 * Description   : Configures the destination address for the eDMA channel.
 *
 * Implements    : edma_set_dest_addr_activity
 *END**************************************************************************/
void
edma_set_dest_addr(uint8_t virtual_channel, uint32_t address)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Set channel TCD destination address */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_tcd_set_dest_addr(p_edma_reg_base, dma_channel, address);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_set_dest_offset
 * Description   : Configures the destination address signed offset for the eDMA
 *                 channel.
 *
 * Implements    : edma_set_dest_offset_activity
 *END**************************************************************************/
void
edma_set_dest_offset(uint8_t virtual_channel, int16_t offset)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Set channel TCD destination offset */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_tcd_set_dest_offset(p_edma_reg_base, dma_channel, offset);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_set_dest_transfer_size
 * Description   : Configures the destination data chunk size (transferred in a
 *                 write sequence).
 *
 * Implements    : edma_set_dest_transfer_size_activity
 *END**************************************************************************/
void
edma_set_dest_transfer_size(uint8_t virtual_channel, edma_transfer_size_t size)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Set channel TCD source transfer size */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_tcd_set_dest_transfer_size(p_edma_reg_base, dma_channel, size);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_set_minor_loop_block_size
 * Description   : Configures the number of bytes to be transferred in each
 *                 service request of the channel.
 *
 * Implements    : edma_set_minor_loop_block_size_activity
 *END**************************************************************************/
void
edma_set_minor_loop_block_size(uint8_t virtual_channel, uint32_t nbytes)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Set channel TCD minor loop block size */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_tcd_set_nbytes(p_edma_reg_base, dma_channel, nbytes);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_set_major_loop_iteration_count
 * Description   : Configures the number of major loop iterations.
 *
 * Implements    : edma_set_major_loop_iteration_count_activity
 *END**************************************************************************/
void
edma_set_major_loop_iteration_count(uint8_t virtual_channel, uint32_t majorLoopCount)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Set the major loop iteration count */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_tcd_set_major_count(p_edma_reg_base, dma_channel, majorLoopCount);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_get_remaining_major_iterations_count
 * Description   : Returns the remaining major loop iteration count.
 *
 * Implements    : edma_get_remaining_major_iterations_count_activity
 *END**************************************************************************/
uint32_t
edma_get_remaining_major_iterations_count(uint8_t virtual_channel)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Retrieve the number of minor loops yet to be triggered */
    const DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    uint32_t     count           = edma_tcd_get_current_major_count(p_edma_reg_base, dma_channel);

    return count;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_set_scatter_gather_link
 * Description   : Configures the memory address of the next TCD, in
 *                 scatter/gather mode.
 *
 * Implements    : edma_set_scatter_gather_link_activity
 *END**************************************************************************/
void
edma_set_scatter_gather_link(uint8_t virtual_channel, uint32_t next_tcd_addr)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Configures the memory address of the next TCD */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_tcd_set_scatter_gather_link(p_edma_reg_base, dma_channel, next_tcd_addr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_disable_request
 * Description   : Disables/Enables the DMA request after the major loop
 *                 completes for the TCD.
 *
 * Implements    : edma_disable_request_activity
 *END**************************************************************************/
void
edma_disable_request(uint8_t virtual_channel, bool b_disable)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Disables/Enables the DMA request upon TCD completion */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_tcd_set_disable_dma_request(p_edma_reg_base, dma_channel, b_disable);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_configure_interrupt
 * Description   : Disables/Enables the channel interrupt requests.
 *
 * Implements    : edma_configure_interrupt_activity
 *END**************************************************************************/
void
edma_configure_interrupt(uint8_t virtual_channel, edma_channel_interrupt_t int_src, bool b_enable)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Disables/Enables the channel interrupt requests. */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    switch (int_src)
    {
        case EDMA_CHN_ERR_INT:
            /* Enable channel interrupt request when error conditions occur */
            break;
        case EDMA_CHN_HALF_MAJOR_LOOP_INT:
            /* Enable channel interrupt request when major iteration count
             * reaches halfway point */
            edma_tcd_set_major_half_complete_int(p_edma_reg_base, dma_channel, b_enable);
            break;
        case EDMA_CHN_MAJOR_LOOP_INT:
            /* Enable channel interrupt request when major iteration count
             * reaches zero */
            edma_tcd_set_major_complete_int(p_edma_reg_base, dma_channel, b_enable);
            break;
        default:
            /* This branch should never be reached if driver API is used
             * properly */
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_cancel_transfer
 * Description   : Cancels the running transfer for this channel.
 *
 * Implements    : edma_cancel_transfer_activity
 *END**************************************************************************/
void
edma_cancel_transfer(bool b_error)
{
    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    uint32_t dma_instance = 0U;

    for (dma_instance = 0U; dma_instance < (uint32_t)DMA_INSTANCE_COUNT; dma_instance++)
    {
        /* Cancel the running transfer. */
        DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
        if (b_error)
        {
            edma_set_ecx(p_edma_reg_base);
        }
        else
        {
            edma_set_cx(p_edma_reg_base);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_trigger_sw_request
 * Description   : Triggers a sw request for the current channel.
 *
 * Implements    : edma_trigger_sw_request_activity
 *END**************************************************************************/
void
edma_trigger_sw_request(uint8_t virtual_channel)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    /* Trigger the channel transfer. */
    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_trigger_channel_start(p_edma_reg_base, dma_channel);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_push_config_to_stcd
 * Description   : Copy the configuration to the software TCD structure.
 *
 * Implements    : edma_push_config_to_stcd_activity
 *END**************************************************************************/
void
edma_push_config_to_stcd(const edma_transfer_config_t *p_config, edma_software_tcd_t *p_stcd)
{
    if ((p_config != NULL) && (p_stcd != NULL))
    {
        /* Clear the array of software TCDs passed by the user */
        edma_clear_stcd(p_stcd);

        /* Set the software TCD fields */
        p_stcd->ATTR   = (uint16_t)(DMA_TCD_ATTR_SMOD(p_config->src_modulo) |
                                  DMA_TCD_ATTR_SSIZE(p_config->src_transfer_size) |
                                  DMA_TCD_ATTR_DMOD(p_config->dest_modulo) |
                                  DMA_TCD_ATTR_DSIZE(p_config->dest_transfer_size));
        p_stcd->SADDR  = p_config->src_addr;
        p_stcd->SOFF   = p_config->src_offset;
        p_stcd->NBYTES = p_config->minor_byte_transfer_count;
        p_stcd->SLAST  = p_config->src_last_addr_adjust;
        p_stcd->DADDR  = p_config->dest_addr;
        p_stcd->DOFF   = p_config->dest_offset;
        p_stcd->CITER  = (uint16_t)p_config->p_loop_transfer_config->major_loop_iteration_count;
        if (p_config->b_scatter_gather_enable)
        {
            p_stcd->DLAST_SGA = (int32_t)p_config->scatter_gather_next_desc_addr;
        }
        else
        {
            p_stcd->DLAST_SGA = p_config->dest_last_addr_adjust;
        }

        p_stcd->CSR =
            (uint16_t)(((p_config->b_interrupt_enable ? 1UL : 0UL) << DMA_TCD_CSR_INTMAJOR_SHIFT) |
                       ((p_config->b_scatter_gather_enable ? 1UL : 0UL) << DMA_TCD_CSR_ESG_SHIFT));

        p_stcd->BITER = (uint16_t)p_config->p_loop_transfer_config->major_loop_iteration_count;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_push_config_to_reg
 * Description   : Copy the configuration to the TCD registers.
 *
 * Implements    : edma_push_config_to_reg_activity
 *END**************************************************************************/
void
edma_push_config_to_reg(uint8_t virtual_channel, const edma_transfer_config_t *p_tcd)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    /* Check the transfer configuration structure is valid */
    DEV_ASSERT(p_tcd != NULL);

    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];

    /* Clear TCD registers */
    edma_tcd_clear_reg(p_edma_reg_base, dma_channel);

#ifdef FEATURE_DMA_ENGINE_STALL
    /* Configure the DMA Engine to stall for a number of cycles after each R/W
     */
    edma_tcd_set_engine_stall(p_edma_reg_base, dma_channel, EDMA_ENGINE_STALL_4_CYCLES);
#endif

    /* Set source and destination addresses */
    edma_tcd_set_src_addr(p_edma_reg_base, dma_channel, p_tcd->src_addr);
    edma_tcd_set_dest_addr(p_edma_reg_base, dma_channel, p_tcd->dest_addr);
    /* Set source/destination modulo feature and transfer size */
    edma_tcd_set_attribute(p_edma_reg_base,
                           dma_channel,
                           p_tcd->src_modulo,
                           p_tcd->dest_modulo,
                           p_tcd->src_transfer_size,
                           p_tcd->dest_transfer_size);
    /* Set source/destination offset and last adjustment; for scatter/gather
     * operation, destination last adjustment is the address of the next TCD
     * structure to be loaded by the eDMA engine */
    edma_tcd_set_src_offset(p_edma_reg_base, dma_channel, p_tcd->src_offset);
    edma_tcd_set_dest_offset(p_edma_reg_base, dma_channel, p_tcd->dest_offset);
    edma_tcd_set_src_last_adjust(p_edma_reg_base, dma_channel, p_tcd->src_last_addr_adjust);

    if (p_tcd->b_scatter_gather_enable)
    {
        edma_tcd_set_scatter_gather(p_edma_reg_base, dma_channel, true);
        edma_tcd_set_scatter_gather_link(
            p_edma_reg_base, dma_channel, p_tcd->scatter_gather_next_desc_addr);
    }
    else
    {
        edma_tcd_set_scatter_gather(p_edma_reg_base, dma_channel, false);
        edma_tcd_set_dest_last_adjust(p_edma_reg_base, dma_channel, p_tcd->dest_last_addr_adjust);
    }

    /* Configure channel interrupt */
    edma_tcd_set_major_complete_int(p_edma_reg_base, dma_channel, p_tcd->b_interrupt_enable);

    /* If loop configuration is available, copy minor/major loop setup to
     * registers */
    if (p_tcd->p_loop_transfer_config != NULL)
    {
        edma_tcd_set_smloe(
            p_edma_reg_base, dma_channel, p_tcd->p_loop_transfer_config->b_src_offset_enable);
        edma_tcd_set_dmloe(
            p_edma_reg_base, dma_channel, p_tcd->p_loop_transfer_config->b_dst_offset_enable);
        edma_tcd_set_minor_loop_offset(
            p_edma_reg_base, dma_channel, p_tcd->p_loop_transfer_config->minor_loop_offset);
        edma_tcd_set_nbytes(p_edma_reg_base, dma_channel, p_tcd->minor_byte_transfer_count);

        edma_tcd_set_channel_minor_link(
            p_edma_reg_base,
            dma_channel,
            p_tcd->p_loop_transfer_config->minor_loop_chn_link_number,
            p_tcd->p_loop_transfer_config->b_minor_loop_chn_link_enable);
        edma_tcd_set_channel_major_link(
            p_edma_reg_base,
            dma_channel,
            p_tcd->p_loop_transfer_config->major_loop_chn_link_number,
            p_tcd->p_loop_transfer_config->b_major_loop_chn_link_enable);

        edma_tcd_set_major_count(p_edma_reg_base,
                                 dma_channel,
                                 p_tcd->p_loop_transfer_config->major_loop_iteration_count);
    }
    else
    {
        edma_tcd_set_nbytes(p_edma_reg_base, dma_channel, p_tcd->minor_byte_transfer_count);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_get_channel_status
 * Description   : Returns the eDMA channel ret_status.
 *
 * Implements    : edma_get_channel_status_activity
 *END**************************************************************************/
edma_chn_status_t
edma_get_channel_status(uint8_t virtual_channel)
{
    /* Check that virtual channel number is valid */
    DEV_ASSERT(virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS);

    /* Check that eDMA module is initialized */
    DEV_ASSERT(sp_virt_edma_state != NULL);

    /* Check that virtual channel is initialized */
    DEV_ASSERT(sp_virt_edma_state->p_virt_chn_state[virtual_channel] != NULL);

    return sp_virt_edma_state->p_virt_chn_state[virtual_channel]->status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_get_dma_reg_base_addr
 * Description   : Returns the DMA register base address.
 *
 * Implements    : edma_get_dma_reg_base_addr
 *END**************************************************************************/
DMA_t *
edma_get_dma_reg_base_addr(uint32_t instance)
{
    /* Check that instance is valid */
    DEV_ASSERT(instance < DMA_INSTANCE_COUNT);

    return sp_edma_base[instance];
}

#if defined(CUSTOM_DMA_IRQ)
#else
/** @brief DMA IRQ handler with the same name in the startup code*/
void
DMA0_IRQHandler(void)
{
    edma_irq_handler(0U);
}

/** @brief DMA IRQ handler with the same name in the startup code*/
void
DMA1_IRQHandler(void)
{
    edma_irq_handler(1U);
}

/** @brief DMA IRQ handler with the same name in the startup code*/
void
DMA2_IRQHandler(void)
{
    edma_irq_handler(2U);
}

/** @brief DMA IRQ handler with the same name in the startup code*/
void
DMA3_IRQHandler(void)
{
    edma_irq_handler(3U);
}
#endif /* DMA_IRQ_HANDLER_TEMPLATE */

#if defined(CUSTOM_DMA_ERR_IRQ)
#else
/** @brief DMA ERROR IRQ handler with the same name in the startup code*/
void
DMA_Error_IRQHandler(void)
{
    const DMA_t *p_edma_reg_base = edma_get_dma_reg_base_addr(0U);
    uint32_t     error           = edma_get_error_int_status_flag(p_edma_reg_base);
    uint8_t      virtual_channel = 0U;

    for (virtual_channel = 0U; virtual_channel < FEATURE_DMA_VIRTUAL_CHANNELS; virtual_channel++)
    {
        if ((error & EDMA_ERR_LSB_MASK) != 0UL)
        {
            edma_error_irq_handler(virtual_channel);
        }
        error = error >> 1U;
    }
}
#endif

/*******************************************************************************
 * Static functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : edma_clear_int_status
 * Description   : Clear done and interrupt ret_status.
 *
 *END**************************************************************************/
static void
edma_clear_int_status(uint8_t virtual_channel)
{
    /* Get DMA instance from virtual channel */
    uint8_t dma_instance = (uint8_t)FEATURE_DMA_VCH_TO_INSTANCE(virtual_channel);

    /* Get DMA channel from virtual channel*/
    uint8_t dma_channel = (uint8_t)FEATURE_DMA_VCH_TO_CH(virtual_channel);

    DMA_t *p_edma_reg_base = sp_edma_base[dma_instance];
    edma_clear_done_status_flag(p_edma_reg_base, dma_channel);
    edma_clear_int_status_flag(p_edma_reg_base, dma_channel);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_clear_stcd
 * Description   : Clear the software tcd structure.
 *
 *END**************************************************************************/
static void
edma_clear_stcd(edma_software_tcd_t *p_stcd)
{
    edma_clear_structure((uint8_t *)p_stcd, sizeof(edma_software_tcd_t));
}

#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
/*FUNCTION**********************************************************************
 *
 * Function Name : edma_valid_transfer_size
 * Description   : Check if the transfer size value is legal (0/1/2/4/5).
 *
 *END**************************************************************************/
static bool
edma_valid_transfer_size(edma_transfer_size_t size)
{
    bool b_is_valid;
    switch (size)
    {
        case EDMA_TRANSFER_SIZE_1B:
        case EDMA_TRANSFER_SIZE_2B:
        case EDMA_TRANSFER_SIZE_4B:
            b_is_valid = true;
            break;
        default:
            b_is_valid = false;
            break;
    }
    return b_is_valid;
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_clear_structure
 * Description   : Clears all bytes at the passed structure pointer.
 *
 *END**************************************************************************/
static void
edma_clear_structure(uint8_t *p_struct_ptr, size_t size)
{
    while (size > 0U)
    {
        *p_struct_ptr = 0;
        p_struct_ptr++;
        size--;
    }
}

/*** end of file ***/
