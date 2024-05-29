/**
 * @file edma_access.h
 * @brief Static inline function for the EDMA module.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef EDMA_ACCESS_H
#define EDMA_ACCESS_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include "device_registers.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/**
 * @brief eDMA channel arbitration algorithm used for selection among channels.
 * Implements : edma_arbitration_algorithm_t_Class
 */
typedef enum
{
    EDMA_ARBITRATION_FIXED_PRIORITY = 0U, /**< Fixed Priority */
    EDMA_ARBITRATION_ROUND_ROBIN          /**< Round-Robin arbitration */
} edma_arbitration_algorithm_t;

/**
 * @brief eDMA channel priority setting
 * Implements : edma_channel_priority_t_Class
 */
typedef enum
{
    EDMA_CHN_PRIORITY_0       = 0U,
    EDMA_CHN_PRIORITY_1       = 1U,
    EDMA_CHN_PRIORITY_2       = 2U,
    EDMA_CHN_PRIORITY_3       = 3U,
    EDMA_CHN_DEFAULT_PRIORITY = 255U
} edma_channel_priority_t;

/**
 * @brief eDMA transfer configuration
 * Implements : edma_transfer_size_t_Class
 */
typedef enum
{
    EDMA_TRANSFER_SIZE_1B = 0x0U,
    EDMA_TRANSFER_SIZE_2B = 0x1U,
    EDMA_TRANSFER_SIZE_4B = 0x2U,
} edma_transfer_size_t;

#ifdef FEATURE_DMA_ENGINE_STALL
/**
 * @brief Specifies the number of cycles the DMA Engine is stalled.
 */
typedef enum
{
    EDMA_ENGINE_STALL_0_CYCLES = 0,
    EDMA_ENGINE_STALL_4_CYCLES = 2,
    EDMA_ENGINE_STALL_8_CYCLES = 3
} edma_engine_stall_t;
#endif

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/
/**
 * @brief Halts or does not halt the eDMA module when an error occurs.
 *
 * An error causes the HALT bit to be set. Subsequently, all service requests
 * are ignored until the HALT bit is cleared.
 *
 * @param p_base Register base address for eDMA module.
 * @param b_halt_on_error Halts (true) or not halt (false) eDMA module when an error occurs.
 */
static inline void
edma_set_halt_on_error(DMA_t *p_base, bool b_halt_on_error)
{
    uint32_t reg_val;
    reg_val = p_base->CR;
    reg_val &= ~(DMA_CR_HOE_MASK);
    reg_val |= DMA_CR_HOE(b_halt_on_error ? 1UL : 0UL);
    p_base->CR = reg_val;
}

/**
 * @brief Sets the eDMA channel priority.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param priority Priority of the DMA channel. Different channels should have
 * different priority setting inside a group.
 */
static inline void
edma_set_channel_priority(DMA_t *p_base, uint8_t channel, edma_channel_priority_t priority)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif

    uint8_t reg_val;
    uint8_t index = (uint8_t)FEATURE_DMA_CHN_TO_DCHPRI_INDEX(channel);
    reg_val       = p_base->DCHPRI[index];
    reg_val &= (uint8_t) ~(DMA_DCHPRI_CHPRI_MASK);
    reg_val |= (uint8_t)DMA_DCHPRI_CHPRI(priority);
    p_base->DCHPRI[index] = reg_val;
}

/**
 * @brief Sets the channel arbitration algorithm.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel_arbitration Round-Robin way or fixed priority way.
 */
static inline void
edma_set_channel_arbitration_mode(DMA_t *p_base, edma_arbitration_algorithm_t channel_arbitration)
{
    uint32_t reg_val;
    reg_val = p_base->CR;
    reg_val &= ~(DMA_CR_ERCA_MASK);
    reg_val |= DMA_CR_ERCA(channel_arbitration);
    p_base->CR = reg_val;
}

/**
 * @brief Gets the channel arbitration algorithm.
 *
 * @param p_base Register base address for eDMA module.
 * @return edma_arbitration_algorithm_t variable indicating the selected
 * channel arbitration: Round-Robin way or fixed priority way
 */
static inline edma_arbitration_algorithm_t
edma_get_channel_arbitration_mode(const DMA_t *p_base)
{
    edma_arbitration_algorithm_t ret_val;

    if (((p_base->CR >> DMA_CR_ERCA_SHIFT) & 1U) != 0U)
    {
        ret_val = EDMA_ARBITRATION_ROUND_ROBIN;
    }
    else
    {
        ret_val = EDMA_ARBITRATION_FIXED_PRIORITY;
    }
    return ret_val;
}

/**
 * @brief Enables/Disables the minor loop mapping.
 *
 * This function enables/disables the minor loop mapping feature.
 * If enabled, the NBYTES is redefined to include the individual enable fields
 * and the NBYTES field. The individual enable fields allow the minor loop
 * offset to be applied to the source address, the destination address, or both.
 * The NBYTES field is reduced when either offset is enabled.
 *
 * @param p_base Register base address for eDMA module.
 * @param b_enable Enables (true) or Disable (false) minor loop mapping.
 */
static inline void
edma_set_minor_loop_mapping(DMA_t *p_base, bool b_enable)
{
    uint32_t reg_val;
    reg_val = p_base->CR;
    reg_val &= ~(DMA_CR_EMLM_MASK);
    reg_val |= DMA_CR_EMLM(b_enable ? 1UL : 0UL);
    p_base->CR = reg_val;
}

/**
 * @brief Gets the eDMA error interrupt status.
 *
 * @param p_base Register base address for eDMA module.
 * @return 32 bit variable indicating error channels. If error happens on eDMA
 * channel n, the bit n of this variable is '1'. If not, the bit n of this
 * variable is '0'.
 */
static inline uint32_t
edma_get_error_int_status_flag(const DMA_t *p_base)
{
    return p_base->ERR;
}

/**
 * @brief Clears the error interrupt status for the eDMA channel or channels.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel Channel indicator.
 */
static inline void
edma_clear_error_int_status_flag(DMA_t *p_base, uint8_t channel)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    p_base->CERR = (uint8_t)channel;
}

/**
 * @brief Clears the done status for a channel or all channels.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel Channel indicator.
 */
static inline void
edma_clear_done_status_flag(DMA_t *p_base, uint8_t channel)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    p_base->CDNE = (uint8_t)channel;
}

/**
 * @brief Triggers the eDMA channel.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel Channel indicator.
 */
static inline void
edma_trigger_channel_start(DMA_t *p_base, uint8_t channel)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    p_base->SSRT = (uint8_t)channel;
}

/**
 * @brief Clears the interrupt status for the eDMA channel or all channels.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel Channel indicator.
 */
static inline void
edma_clear_int_status_flag(DMA_t *p_base, uint8_t channel)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    p_base->CINT = (uint8_t)channel;
}

#ifdef FEATURE_DMA_ENGINE_STALL
/**
 * @brief Configures DMA engine to stall for a number of cycles after each R/W.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel Channel indicator.
 * @param cycles Number of cycles the DMA engine is stalled after each R/W.
 */
static inline void
edma_tcd_set_engine_stall(DMA_t *p_base, uint8_t channel, edma_engine_stall_t cycles)
{
#ifdef DEV_ERROR_DETECT
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    uint16_t reg_val;
    reg_val = p_base->TCD[channel].CSR;
    reg_val &= ~(DMA_TCD_CSR_BWC_MASK);
    reg_val |= DMA_TCD_CSR_BWC(cycles);
    p_base->TCD[channel].CSR = reg_val;
}
#endif

/**
 * @brief Configures the source address for the hardware TCD.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param address The pointer to the source memory address.
 */
static inline void
edma_tcd_set_src_addr(DMA_t *p_base, uint8_t channel, uint32_t address)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    p_base->TCD[channel].SADDR = address;
}

/**
 * @brief Configures the source address signed offset for the hardware TCD.
 *
 * Sign-extended offset applied to the current source address to form the
 * next-state value as each source read is complete.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param offset signed-offset for source address.
 */
static inline void
edma_tcd_set_src_offset(DMA_t *p_base, uint8_t channel, int16_t offset)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    p_base->TCD[channel].SOFF = (uint16_t)offset;
}

/**
 * @brief Sets the source transfer size.
 *
 * Configures the source data read transfer size (1/2/4/16/32 bytes).
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param size Source transfer size.
 */
static inline void
edma_tcd_set_src_transfer_size(DMA_t *p_base, uint8_t channel, edma_transfer_size_t size)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    uint16_t reg_val;
    reg_val = p_base->TCD[channel].ATTR;
    reg_val &= (uint16_t)(~(DMA_TCD_ATTR_SSIZE_MASK));
    reg_val |= (uint16_t)(DMA_TCD_ATTR_SSIZE((uint16_t)size));
    p_base->TCD[channel].ATTR = reg_val;
}

/**
 * @brief Sets the destination transfer size.
 *
 * Configures the destination data write transfer size (1/2/4/16/32 bytes).
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param size Destination transfer size.
 */
static inline void
edma_tcd_set_dest_transfer_size(DMA_t *p_base, uint8_t channel, edma_transfer_size_t size)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    uint16_t reg_val;
    reg_val = p_base->TCD[channel].ATTR;
    reg_val &= (uint16_t)(~(DMA_TCD_ATTR_DSIZE_MASK));
    reg_val |= (uint16_t)(DMA_TCD_ATTR_DSIZE((uint16_t)size));
    p_base->TCD[channel].ATTR = reg_val;
}

/**
 * @brief Enables/disables the source minor loop offset feature for the TCD.
 *
 * Configures whether the minor loop offset is applied to the source address
 * upon minor loop completion.
 * NOTE: EMLM bit needs to be enabled prior to calling this function, otherwise
 * it has no effect.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param b_enable Enables (true) or disables (false) source minor loop offset.
 */
static inline void
edma_tcd_set_smloe(DMA_t *p_base, uint8_t channel, bool b_enable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    if (((p_base->CR >> DMA_CR_EMLM_SHIFT) & 1U) != 0U)
    {
        uint32_t reg_val;
        reg_val = p_base->TCD[channel].NBYTES.MLOFFYES;
        reg_val &= ~(DMA_TCD_NBYTES_MLOFFYES_SMLOE_MASK);
        reg_val |= DMA_TCD_NBYTES_MLOFFYES_SMLOE(b_enable ? 1UL : 0UL);
        p_base->TCD[channel].NBYTES.MLOFFYES = reg_val;
    }
}

/**
 * @brief Enables/disables the destination minor loop offset feature for the TCD.
 *
 * Configures whether the minor loop offset is applied to the destination
 * address upon minor loop completion. NOTE: EMLM bit needs to be enabled prior
 * to calling this function, otherwise it has no effect.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param b_enable Enables (true) or disables (false) destination minor loop offset.
 */
static inline void
edma_tcd_set_dmloe(DMA_t *p_base, uint8_t channel, bool b_enable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    if (((p_base->CR >> DMA_CR_EMLM_SHIFT) & 1U) != 0U)
    {
        uint32_t reg_val;
        reg_val = p_base->TCD[channel].NBYTES.MLOFFYES;
        reg_val &= ~(DMA_TCD_NBYTES_MLOFFYES_DMLOE_MASK);
        reg_val |= DMA_TCD_NBYTES_MLOFFYES_DMLOE(b_enable ? 1UL : 0UL);
        p_base->TCD[channel].NBYTES.MLOFFYES = reg_val;
    }
}

/**
 * @brief Configures the last source address adjustment for the TCD.
 *
 * Adjustment value added to the source address at the completion of the major
 * iteration count. This value can be applied to restore the source address to
 * the initial value, or adjust the address to reference the next data
 * structure.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param size adjustment value
 */
static inline void
edma_tcd_set_src_last_adjust(DMA_t *p_base, uint8_t channel, int32_t size)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    p_base->TCD[channel].SLAST = (uint32_t)size;
}

/**
 * @brief Configures the destination address for the TCD.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param address The pointer to the destination address.
 */
static inline void
edma_tcd_set_dest_addr(DMA_t *p_base, uint8_t channel, uint32_t address)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    p_base->TCD[channel].DADDR = address;
}

/**
 * @brief Configures the destination address signed offset for the TCD.
 *
 * Sign-extended offset applied to the current source address to form the
 * next-state value as each destination write is complete.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param offset signed-offset
 */
static inline void
edma_tcd_set_dest_offset(DMA_t *p_base, uint8_t channel, int16_t offset)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    p_base->TCD[channel].DOFF = (uint16_t)offset;
}

/**
 * @brief Configures the last source address adjustment.
 *
 * This function adds an adjustment value added to the source address at the
 * completion of the major iteration count. This value can be applied to restore
 * the source address to the initial value, or adjust the address to reference
 * the next data structure.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param adjust adjustment value
 */
static inline void
edma_tcd_set_dest_last_adjust(DMA_t *p_base, uint8_t channel, int32_t adjust)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    p_base->TCD[channel].DLASTSGA = (uint32_t)adjust;
}

/**
 * @brief Enables/Disables the scatter/gather feature for the TCD.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param b_enable Enables (true) /Disables (false) scatter/gather feature.
 */
static inline void
edma_tcd_set_scatter_gather(DMA_t *p_base, uint8_t channel, bool b_enable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    uint16_t reg_val;
    reg_val = p_base->TCD[channel].CSR;
    reg_val &= (uint16_t) ~(DMA_TCD_CSR_ESG_MASK);
    reg_val |= (uint16_t)DMA_TCD_CSR_ESG(b_enable ? 1UL : 0UL);
    p_base->TCD[channel].CSR = reg_val;
}

/**
 * @brief Configures the major channel link the TCD.
 *
 * If the major link is enabled, after the major loop counter is exhausted, the
 * eDMA engine initiates a channel service request at the channel defined by
 * these six bits by setting that channel start bits.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param major_link_channel channel number for major link
 * @param b_enable Enables (true) or Disables (false) channel major link.
 */
static inline void
edma_tcd_set_channel_major_link(DMA_t   *p_base,
                                uint8_t  channel,
                                uint32_t major_link_channel,
                                bool     b_enable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    uint16_t reg_val;
    reg_val = p_base->TCD[channel].CSR;
    reg_val &= (uint16_t) ~(DMA_TCD_CSR_MAJORLINKCH_MASK);
    reg_val |= (uint16_t)DMA_TCD_CSR_MAJORLINKCH(major_link_channel);
    p_base->TCD[channel].CSR = reg_val;
    reg_val                  = p_base->TCD[channel].CSR;
    reg_val &= (uint16_t) ~(DMA_TCD_CSR_MAJORELINK_MASK);
    reg_val |= (uint16_t)DMA_TCD_CSR_MAJORELINK(b_enable ? 1UL : 0UL);
    p_base->TCD[channel].CSR = reg_val;
}

/**
 * @brief Disables/Enables the DMA request after the major loop completes for
 * the TCD.
 *
 * If disabled, the eDMA hardware automatically clears the corresponding DMA
 * request when the current major iteration count reaches zero.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param disable Disable (true)/Enable (false) DMA request after TCD complete.
 */
static inline void
edma_tcd_set_disable_dma_request(DMA_t *p_base, uint8_t channel, bool disable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    uint16_t reg_val;
    reg_val = p_base->TCD[channel].CSR;
    reg_val &= (uint16_t) ~(DMA_TCD_CSR_DREQ_MASK);
    reg_val |= (uint16_t)DMA_TCD_CSR_DREQ(disable ? 1UL : 0UL);
    p_base->TCD[channel].CSR = reg_val;
}

/**
 * @brief Enables/Disables the half complete interrupt for the TCD.
 *
 * If set, the channel generates an interrupt request by setting the appropriate
 * bit in the interrupt register when the current major iteration count reaches
 * the halfway point. Specifically, the comparison performed by the eDMA engine
 * is (CITER == (BITER >> 1)). This half-way point interrupt request is provided
 * to support the double-buffered schemes or other types of data movement where
 * the processor needs an early indication of the transfer's process.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param b_enable Enable (true) /Disable (false) half complete interrupt.
 */
static inline void
edma_tcd_set_major_half_complete_int(DMA_t *p_base, uint8_t channel, bool b_enable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    uint16_t reg_val;
    reg_val = p_base->TCD[channel].CSR;
    reg_val &= (uint16_t) ~(DMA_TCD_CSR_INTHALF_MASK);
    reg_val |= (uint16_t)DMA_TCD_CSR_INTHALF(b_enable ? 1UL : 0UL);
    p_base->TCD[channel].CSR = reg_val;
}

/**
 * @brief Enables/Disables the interrupt after the major loop completes for the TCD.
 *
 * If enabled, the channel generates an interrupt request by setting the
 * appropriate bit in the interrupt register when the current major iteration
 * count reaches zero.
 *
 * @param p_base Register base address for eDMA module.
 * @param channel eDMA channel number.
 * @param b_enable Enable (true) /Disable (false) interrupt after TCD done.
 */
static inline void
edma_tcd_set_major_complete_int(DMA_t *p_base, uint8_t channel, bool b_enable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMA_CHANNELS);
#endif
    uint16_t reg_val;
    reg_val = p_base->TCD[channel].CSR;
    reg_val &= (uint16_t) ~(DMA_TCD_CSR_INTMAJOR_MASK);
    reg_val |= (uint16_t)DMA_TCD_CSR_INTMAJOR(b_enable ? 1UL : 0UL);
    p_base->TCD[channel].CSR = reg_val;
}

/**
 * @brief Enables/Disables the DMAMUX channel.
 *
 * Enables the hardware request. If enabled, the hardware request is  sent to
 * the corresponding DMA channel.
 *
 * @param p_base Register base address for DMAMUX module.
 * @param channel DMAMUX channel number.
 * @param enable Enables (true) or Disables (false) DMAMUX channel.
 */
static inline void
dmamux_set_channel_cmd(DMAMUX_t *p_base, uint8_t channel, bool b_enable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMAMUX_CHANNELS);
#endif
    uint32_t reg_index = FEATURE_DMAMUX_CHN_REG_INDEX(channel);
    uint8_t  reg_val   = p_base->CHCFG[reg_index];
    reg_val &= (uint8_t) ~(DMAMUX_CHCFG_ENBL_MASK);
    reg_val |= (uint8_t)DMAMUX_CHCFG_ENBL(b_enable ? 1U : 0U);
    p_base->CHCFG[reg_index] = reg_val;
}

/**
 * @brief Configure DMA Channel Trigger bit in DMAMUX.
 *
 * Enables/Disables DMA Channel Trigger bit in DMAMUX.
 *
 * @param p_base Register base address for DMAMUX module.
 * @param channel DMAMUX channel number.
 * @param b_enable/disable command.
 */
static inline void
dmamux_set_channel_trigger(DMAMUX_t *p_base, uint8_t channel, bool b_enable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMAMUX_CHANNELS);
#endif
    uint32_t reg_index = FEATURE_DMAMUX_CHN_REG_INDEX(channel);
    uint8_t  reg_val;
    reg_val = p_base->CHCFG[reg_index];
    reg_val &= (uint8_t) ~(DMAMUX_CHCFG_TRIG_MASK);
    reg_val |= (uint8_t)DMAMUX_CHCFG_TRIG(b_enable ? 1U : 0U);
    p_base->CHCFG[reg_index] = reg_val;
}

/**
 * @brief Configures the DMA request for the DMAMUX channel.
 *
 * Selects which DMA source is routed to a DMA channel. The DMA sources are
 * defined in the file <MCU>_Features.h
 *
 * @param p_base Register base address for DMAMUX module.
 * @param channel DMAMUX channel number.
 * @param source DMA request source.
 */
static inline void
dmamux_set_channel_source(DMAMUX_t *p_base, uint8_t channel, uint8_t source)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    DEV_ASSERT(channel < FEATURE_DMAMUX_CHANNELS);
#endif
    uint32_t reg_index = FEATURE_DMAMUX_CHN_REG_INDEX(channel);
    uint8_t  reg_val;
    reg_val = p_base->CHCFG[reg_index];
    reg_val &= (uint8_t) ~(DMAMUX_CHCFG_SOURCE_MASK);
    reg_val |= (uint8_t)DMAMUX_CHCFG_SOURCE(source);
    p_base->CHCFG[reg_index] = reg_val;
}

#ifdef __cplusplus
}
#endif

#endif /* EDMA_ACCESS_H */

/*** end of file ***/
