/**
 * @file edma_driver.h
 * @brief Header file for the edma driver.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef EDMA_DRIVER_H
#define EDMA_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stddef.h>
#include "device_registers.h"
#include "status.h"
#include "edma_access.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/**
 * @brief Macro for the memory size needed for the software TCD.
 *
 * Software TCD is aligned to 32 bytes. We don't need a software TCD structure
 * for the first descriptor, since the configuration is pushed directly to
 * registers. To make sure the software TCD can meet the eDMA module requirement
 * regarding alignment, allocate memory for the remaining descriptors with extra
 * 31 bytes.
 */
#define STCD_SIZE(number)  (((number)*32U) - 1U)
#define STCD_ADDR(address) (((uint32_t)address + 31UL) & ~0x1FUL)

/**
 * @brief Macro for accessing the least significant bit of the ERR register.
 *
 * The erroneous channels are retrieved from ERR register by subsequently right
 * shifting all the ERR bits + "AND"-ing the result with this mask.
 */
#define EDMA_ERR_LSB_MASK 1U

/**
 * @brief eDMA channel interrupts.
 * Implements : edma_channel_interrupt_t_Class
 */
typedef enum
{
    EDMA_CHN_ERR_INT = 0U,        /**< Error interrupt */
    EDMA_CHN_HALF_MAJOR_LOOP_INT, /**< Half major loop interrupt. */
    EDMA_CHN_MAJOR_LOOP_INT       /**< Complete major loop interrupt. */
} edma_channel_interrupt_t;

/**
 * @brief eDMA modulo configuration
 * Implements : edma_modulo_t_Class
 */
typedef enum
{
    EDMA_MODULO_OFF = 0U,
    EDMA_MODULO_2B,
    EDMA_MODULO_4B,
    EDMA_MODULO_8B,
    EDMA_MODULO_16B,
    EDMA_MODULO_32B,
    EDMA_MODULO_64B,
    EDMA_MODULO_128B,
    EDMA_MODULO_256B,
    EDMA_MODULO_512B,
    EDMA_MODULO_1KB,
    EDMA_MODULO_2KB,
    EDMA_MODULO_4KB,
    EDMA_MODULO_8KB,
    EDMA_MODULO_16KB,
    EDMA_MODULO_32KB,
    EDMA_MODULO_64KB,
    EDMA_MODULO_128KB,
    EDMA_MODULO_256KB,
    EDMA_MODULO_512KB,
    EDMA_MODULO_1MB,
    EDMA_MODULO_2MB,
    EDMA_MODULO_4MB,
    EDMA_MODULO_8MB,
    EDMA_MODULO_16MB,
    EDMA_MODULO_32MB,
    EDMA_MODULO_64MB,
    EDMA_MODULO_128MB,
    EDMA_MODULO_256MB,
    EDMA_MODULO_512MB,
    EDMA_MODULO_1GB,
    EDMA_MODULO_2GB
} edma_modulo_t;

/**
 * @brief The user configuration structure for the eDMA driver.
 *
 * Use an instance of this structure with the edma_init() function. This
 * allows the user to configure settings of the EDMA peripheral with a
 * single function call. Implements : edma_user_config_t_Class
 */
typedef struct
{
    edma_arbitration_algorithm_t chn_arbitration; /**< eDMA channel arbitration. */
    bool                         b_halt_on_error; /**< Any error causes the HALT bit to set.
                                                 Subsequently, all service requests are ignored
                                                 until the HALT bit is cleared. */
} edma_user_config_t;

/**
 * @brief Channel status for eDMA channel.
 *
 * A structure describing the eDMA channel status. The user can get the
 * status by callback parameter or by calling EDMA_DRV_getStatus() function.
 * Implements : edma_chn_status_t_Class
 */
typedef enum
{
    EDMA_CHN_NORMAL = 0U, /**< eDMA channel normal state. */
    EDMA_CHN_ERROR        /**< An error occurred in the eDMA channel. */
} edma_chn_status_t;

/**
 * @brief Definition for the eDMA channel callback function.
 *
 * Prototype for the callback function registered in the eDMA driver.
 * Implements : edma_callback_t_Class
 */
typedef void (*edma_callback_t)(void *p_parameter, edma_chn_status_t status);

/**
 * @brief Data structure for the eDMA channel state.
 * Implements : edma_chn_state_t_Class
 */
typedef struct
{
    uint8_t         virt_chn;               /**< Virtual channel number. */
    edma_callback_t callback;               /**< Callback function pointer for the eDMA
                                               channel. It will be called at the eDMA
                                               channel complete and eDMA channel error. */
    void                      *p_parameter; /**< Parameter for the callback function pointer. */
    volatile edma_chn_status_t status;      /**< eDMA channel status. */
} edma_chn_state_t;

/**
 * @brief The user configuration structure for the an eDMA driver channel.
 *
 * Use an instance of this structure with the edma_channel_init()
 * function. This allows the user to configure settings of the EDMA channel
 * with a single function call. Implements : edma_channel_config_t_Class
 */
typedef struct
{
    edma_channel_priority_t channel_priority; /**< eDMA channel priority - only used when channel
                                                  arbitration mode is 'Fixed priority'. */
    uint8_t virt_chn_config;                  /**< eDMA virtual channel number */

    dma_request_source_t source;      /**< Selects the source of the DMA request
                                         for this channel */
    edma_callback_t callback;         /**< Callback that will be registered for this channel */
    void           *p_callback_param; /**< Parameter passed to the channel callback */
    bool            b_enable_trigger; /**< Enables the periodic trigger capability for the
                                      DMA channel. */
} edma_channel_config_t;

/**
 * @brief A type for the DMA transfer.
 * Implements : edma_transfer_type_t_Class
 */
typedef enum
{
    EDMA_TRANSFER_PERIPH2MEM = 0U, /**< Transfer from peripheral to memory */
    EDMA_TRANSFER_MEM2PERIPH,      /**< Transfer from memory to peripheral */
    EDMA_TRANSFER_MEM2MEM,         /**< Transfer from memory to memory */
    EDMA_TRANSFER_PERIPH2PERIPH    /**< Transfer from peripheral to peripheral
                                    */
} edma_transfer_type_t;

/**
 * @brief Data structure for configuring a discrete memory transfer.
 * Implements : edma_scatter_gather_list_t_Class
 */
typedef struct
{
    uint32_t             address; /**< Address of buffer. */
    uint32_t             length;  /**< Length of buffer. */
    edma_transfer_type_t type;    /**< Type of the DMA transfer */
} edma_scatter_gather_list_t;

/**
 * @brief Runtime state structure for the eDMA driver.
 *
 * This structure holds data that is used by the eDMA peripheral driver to
 * manage multi eDMA channels. The user passes the memory for this run-time
 * state structure and the eDMA driver populates the members. Implements :
 * edma_state_t_Class
 */
typedef struct
{
    edma_chn_state_t
        *volatile p_virt_chn_state[(uint32_t)FEATURE_DMA_VIRTUAL_CHANNELS]; /**< Pointer array
                                                                           storing channel state. */
} edma_state_t;

/**
 * @brief eDMA loop transfer configuration.
 *
 * This structure configures the basic minor/major loop attributes.
 * Implements : edma_loop_transfer_config_t_Class
 */
typedef struct
{
    uint32_t major_loop_iteration_count; /**< Number of major loop iterations. */
    bool     b_src_offset_enable;        /**< Selects whether the minor loop offset is
                                        applied to the source address upon minor
                                        loop completion. */
    bool b_dst_offset_enable;            /**< Selects whether the minor loop offset is
                                        applied to the destination address upon
                                        minor loop completion. */
    int32_t minor_loop_offset;           /**< Sign-extended offset applied to the source
                                          or destination address to form the
                                          next-state value after the minor loop
                                          completes. */
    bool b_minor_loop_chn_link_enable;   /**< Enables channel-to-channel linking on
                                      minor loop complete. */
    uint8_t minor_loop_chn_link_number;  /**< The number of the next channel to
                                        be started by DMA engine when minor
                                        loop completes. */
    bool b_major_loop_chn_link_enable;   /**< Enables channel-to-channel linking on
                                      major loop complete. */
    uint8_t major_loop_chn_link_number;  /**< The number of the next channel to
                                        be started by DMA engine when major
                                        loop completes. */
} edma_loop_transfer_config_t;

/**
 * @brief eDMA transfer size configuration.
 *
 * This structure configures the basic source/destination transfer
 * attribute. Implements : edma_transfer_config_t_Class
 */
typedef struct
{
    uint32_t             src_addr;          /**< Memory address pointing to the source data. */
    uint32_t             dest_addr;         /**< Memory address pointing to the destination data. */
    edma_transfer_size_t src_transfer_size; /**< Source data transfer size. */
    edma_transfer_size_t dest_transfer_size; /**< Destination data transfer size. */
    int16_t              src_offset;         /**< Sign-extended offset applied to the current
                                               source address  to form the next-state value as
                                               each source read/write  is completed. */
    int16_t dest_offset;                     /**< Sign-extended offset applied to the current
                                               destination address to form the next-state value
                                               as each source read/write is completed. */
    int32_t src_last_addr_adjust;            /**< Last source address adjustment. */
    int32_t dest_last_addr_adjust;           /**< Last destination address adjustment.
                                             Note here it is only valid when
                                             scatter/gather feature is not enabled. */
    edma_modulo_t src_modulo;                /**< Source address modulo. */
    edma_modulo_t dest_modulo;               /**< Destination address modulo. */
    uint32_t      minor_byte_transfer_count; /**< Number of bytes to be transferred in
                                             each service request of the channel. */
    bool     b_scatter_gather_enable;        /**< Enable scatter gather feature. */
    uint32_t scatter_gather_next_desc_addr;  /**< The address of the next descriptor
                                            to be used, when scatter/gather
                                            feature is enabled. Note: this value
                                            is not used when scatter/gather
                                                    feature is disabled. */
    bool b_interrupt_enable;                 /**< Enable the interrupt request when the major
                                             loop count completes */
    edma_loop_transfer_config_t *p_loop_transfer_config; /**< Pointer to loop transfer configuration
                                                        structure (defines minor/major loop
                                                        attributes) Note: this field is only used
                                                        when minor loop mapping is
                                                                enabled from DMA configuration. */
} edma_transfer_config_t;

/**
 * @brief eDMA TCD
 * Implements : edma_software_tcd_t_Class
 */
typedef struct
{
    uint32_t SADDR;     /**< TCD Source Address */
    int16_t  SOFF;      /**< Source address signed offset*/
    uint16_t ATTR;      /**< TCD Transfer Attributes */
    uint32_t NBYTES;    /**< Minor Byte Transfer Count */
    int32_t  SLAST;     /**< Last Source Address Adjustment */
    uint32_t DADDR;     /**< TCD Destination Address */
    int16_t  DOFF;      /**< Destination Address Signed Offset */
    uint16_t CITER;     /**< Current Major Iteration Count */
    int32_t  DLAST_SGA; /**< Destination Last Address Adjustment */
    uint16_t CSR;       /**< TCD Control and Status */
    uint16_t BITER;     /**< Starting Major Iteration Count */
} edma_software_tcd_t;

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @name eDMA Peripheral Driver
 * @{
 */

/**
 * @name eDMA peripheral driver module level functions
 * @{
 */

/** @brief DMA channel interrupt handler, implemented in driver c file. */
void edma_irq_handler(uint8_t virtual_channel);

/** @brief DMA error interrupt handler, implemented in driver c file. */
void edma_error_irq_handler(uint8_t virtual_channel);

/**
 * @brief Initializes the eDMA module.
 *
 * This function initializes the run-time state structure to provide the
 * eDMA channel allocation release, protect, and track the state for
 * channels. This function also resets the eDMA modules, initializes the
 * module to user-defined settings and default settings.
 * @param p_edma_state The pointer to the eDMA peripheral driver state
 * structure. The user passes the memory for this run-time state structure
 * and the eDMA peripheral driver populates the members. This run-time state
 * structure keeps track of the eDMA channels status. The memory must be
 * kept valid before calling the edma_deinit.
 * @param p_user_config User configuration structure for eDMA peripheral
 * drivers. The user populates the members of this structure and passes the
 * pointer of this structure into the function.
 * @param p_chn_state_array Array of pointers to run-time state structures for
 * eDMA channels; will populate the state structures inside the eDMA driver
 * state structure.
 * @param p_chn_config_array Array of pointers to channel initialization
 * structures.
 * @param chn_count The number of eDMA channels to be initialized.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t edma_init(edma_state_t                      *p_edma_state,
                   const edma_user_config_t          *p_user_config,
                   edma_chn_state_t *const            p_chn_state_array[],
                   const edma_channel_config_t *const p_chn_config_array[],
                   uint32_t                           chn_count);

/**
 * @brief De-initializes the eDMA module.
 *
 * This function resets the eDMA module to reset state and disables the
 * interrupt to the core.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t edma_deinit(void);

/** @} */

/**
 * @name eDMA peripheral driver channel management functions
 * @{
 */

/**
 * @brief Initializes an eDMA channel.
 *
 * This function initializes the run-time state structure for a eDMA
 * channel, based on user configuration. It will request the channel, set up
 * the channel priority and install the callback.
 *
 * @param p_edma_channel_state Pointer to the eDMA channel state structure. The
 * user passes the memory for this run-time state structure and the eDMA
 * peripheral driver populates the members. This run-time state structure
 * keeps track of the eDMA channel status. The memory must be kept valid
 * before calling the edma_release_channel.
 * @param p_edma_channel_config User configuration structure for eDMA channel.
 * The user populates the members of this structure and passes the pointer
 * of this structure into the function.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t edma_channel_init(edma_chn_state_t            *p_edma_channel_state,
                           const edma_channel_config_t *p_edma_channel_config);

/**
 * @brief Releases an eDMA channel.
 *
 * This function stops the eDMA channel and disables the interrupt of this
 * channel. The channel state structure can be released after this function
 * is called.
 *
 * @param virtual_channel eDMA virtual channel number.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t edma_release_channel(uint8_t virtual_channel);

/** @} */

/**
 * @name eDMA peripheral driver transfer setup functions
 * @{
 */

/**
 * @brief Copies the channel configuration to the TCD registers.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param p_tcd Pointer to the channel configuration structure.
 */
void edma_push_config_to_reg(uint8_t virtual_channel, const edma_transfer_config_t *p_tcd);

/**
 * @brief Copies the channel configuration to the software TCD structure.
 *
 * This function copies the properties from the channel configuration to the
 software TCD structure; the address
 * of the software TCD can be used to enable scatter/gather operation
 (pointer to the next TCD).

 * @param p_config Pointer to the channel configuration structure.
 * @param p_stcd Pointer to the software TCD structure.
 */
void edma_push_config_to_stcd(const edma_transfer_config_t *p_config, edma_software_tcd_t *p_stcd);

/**
 * @brief Configures a simple single block data transfer with DMA.
 *
 * This function configures the descriptor for a single block transfer.
 * The function considers contiguous memory blocks, thus it configures the
 * TCD source/destination offset fields to cover the data buffer without
 * gaps, according to "transferSize" parameter (the offset is equal to the
 * number of bytes transferred in a source read/destination write).
 *
 * NOTE: For memory-to-peripheral or peripheral-to-memory transfers, make
 * sure the transfer size is equal to the data buffer size of the peripheral
 * used, otherwise only truncated chunks of data may be transferred (e.g.
 * for a communication IP with an 8-bit data register the transfer size
 * should be 1B, whereas for a 32-bit data register, the transfer size
 * should be 4B). The rationale of this constraint is that, on the
 * peripheral side, the address offset is set to zero, allowing to
 * read/write data from/to the peripheral in a single source
 * read/destination write operation.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param type Transfer type (M->M, P->M, M->P, P->P).
 * @param src_addr A source register address or a source memory address.
 * @param dest_addr A destination register address or a destination memory
 * address.
 * @param transfer_size The number of bytes to be transferred on every DMA
 * write/read. Source/Dest share the same write/read size.
 * @param data_buffer_size The total number of bytes to be transferred.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS
 */
status_t edma_config_single_block_transfer(uint8_t              virtual_channel,
                                           edma_transfer_type_t type,
                                           uint32_t             src_addr,
                                           uint32_t             dest_addr,
                                           edma_transfer_size_t transfer_size,
                                           uint32_t             data_buffer_size);

/**
 * @brief Configures a multiple block data transfer with DMA.
 *
 * This function configures the descriptor for a multi-block transfer.
 * The function considers contiguous memory blocks, thus it configures the
 * TCD source/destination offset fields to cover the data buffer without
 * gaps, according to "transferSize" parameter (the offset is equal to the
 * number of bytes transferred in a source read/destination write). The
 * buffer is divided in multiple block, each block being transferred upon a
 * single DMA request.
 *
 * NOTE: For transfers to/from peripherals, make sure
 * the transfer size is equal to the data buffer size of the peripheral
 * used, otherwise only truncated chunks of data may be transferred (e.g.
 * for a communication IP with an 8-bit data register the transfer size
 * should be 1B, whereas for a 32-bit data register, the transfer size
 * should be 4B). The rationale of this constraint is that, on the
 * peripheral side, the address offset is set to zero, allowing to
 * read/write data from/to the peripheral in a single source
 * read/destination write operation.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param type Transfer type (M->M, P->M, M->P, P->P).
 * @param src_addr A source register address or a source memory address.
 * @param dest_addr A destination register address or a destination memory
 * address.
 * @param transfer_size The number of bytes to be transferred on every DMA
 * write/read. Source/Dest share the same write/read size.
 * @param block_size The total number of bytes inside a block.
 * @param block_count The total number of data blocks (one block is
 * transferred upon a DMA request).
 * @param b_disable_req_on_completion This parameter specifies whether the DMA
 * channel should be disabled when the transfer is complete (further
 * requests will remain untreated).
 *
 * @return STATUS_ERROR or STATUS_SUCCESS
 */
status_t edma_config_multi_block_transfer(uint8_t              virtual_channel,
                                          edma_transfer_type_t type,
                                          uint32_t             src_addr,
                                          uint32_t             dest_addr,
                                          edma_transfer_size_t transfer_size,
                                          uint32_t             block_size,
                                          uint32_t             block_count,
                                          bool                 b_disable_req_on_completion);

/**
 * @brief Configures the DMA transfer in loop mode.
 *
 * This function configures the DMA transfer in a loop chain. The user
 * passes a block of memory into this function that configures the loop
 * transfer properties (minor/major loop count, address offsets, channel
 * linking). The DMA driver copies the configuration to TCD registers, only
 * when the loop properties are set up correctly and minor loop mapping is
 * enabled for the eDMA module.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param p_transfer_config Pointer to the transfer configuration strucutre;
 * this structure defines fields for setting up the basic transfer and also
 * a pointer to a memory strucure that defines the loop chain properties
 * (minor/major).
 *
 * @return STATUS_ERROR or STATUS_SUCCESS
 */
status_t edma_config_loop_transfer(uint8_t                       virtual_channel,
                                   const edma_transfer_config_t *p_transfer_config);

/**
 * @brief Configures the DMA transfer in a scatter-gather mode.
 *
 * This function configures the descriptors into a single-ended chain. The
 * user passes blocks of memory into this function. The interrupt is
 * triggered only when the last memory block is completed. The memory block
 * information is passed with the edma_scatter_gather_list_t data structure,
 * which can tell the memory address and length. The DMA driver configures
 * the descriptor for each memory block, transfers the descriptor from the
 * first one to the last one, and stops.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param p_stcd Array of empty software TCD structures. The user must prepare
 * this memory block. We don't need a software TCD structure for the first
 * descriptor, since the configuration is pushed directly to registers.The
 * "stcd" buffer must align with 32 bytes; if not, an error occurs in the
 * eDMA driver. Thus, the required memory size for "stcd" is equal to
 * tcdCount * size_of(edma_software_tcd_t) - 1; the driver will take care of
 * the memory alignment if the provided memory buffer is big enough. For
 * proper allocation of the "stcd" buffer it is recommended to use STCD_SIZE
 * macro.
 * @param transfer_size The number of bytes to be transferred on every DMA
 * write/read.
 * @param bytes_on_each_request Bytes to be transferred in each DMA request.
 * @param p_src_list Data structure storing the address, length and type of
 * transfer (M->M, M->P, P->M, P->P) for the bytes to be transferred for
 * source memory blocks. If the source memory is peripheral, the length is
 * not used.
 * @param p_dest_list Data structure storing the address, length and type of
 * transfer (M->M, M->P, P->M, P->P) for the bytes to be transferred for
 * destination memory blocks. In the memory-to-memory transfer mode, the
 * user must ensure that the length of the destination scatter gather list
 * is equal to the source scatter gather list. If the destination memory is
 * a peripheral register, the length is not used.
 * @param tcd_count The number of TCD memory blocks contained in the scatter
 * gather list.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS
 */
status_t edma_config_scatter_gather_transfer(uint8_t              virtual_channel,
                                             edma_software_tcd_t *p_stcd,
                                             edma_transfer_size_t transfer_size,
                                             uint32_t             bytes_on_each_request,
                                             const edma_scatter_gather_list_t *p_src_list,
                                             const edma_scatter_gather_list_t *p_dest_list,
                                             uint8_t                           tcd_count);

/**
 * @brief Cancel the running transfer.
 *
 * This function cancels the current transfer, optionally signalling an error.
 *
 * @param b_error error If true, an error will be logged for the current transfer.
 */
void edma_cancel_transfer(bool b_error);

/** @} */

/**
 * @name eDMA Peripheral driver channel operation functions
 * @{
 */
/**
 * @brief Starts an eDMA channel.
 *
 * This function enables the eDMA channel DMA request.
 *
 * @param virtual_channel eDMA virtual channel number.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t edma_start_channel(uint8_t virtual_channel);

/**
 * @brief Stops the eDMA channel.
 *
 * This function disables the eDMA channel DMA request.
 *
 * @param virtual_channel eDMA virtual channel number.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t edma_stop_channel(uint8_t virtual_channel);

/**
 * @brief Configures the DMA request for the eDMA channel.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param request DMA request source.
 * @param b_enable_trigger DMA channel periodic trigger.
 *
 * @return STATUS_SUCCESS or STATUS_UNSUPPORTED.
 */
status_t edma_set_channel_request_and_trigger(uint8_t virtual_channel,
                                              uint8_t request,
                                              bool    b_enable_trigger);

/**
 * @brief Clears all registers to 0 for the channel's TCD.
 *
 * @param virtual_channel eDMA virtual channel number.
 */
void edma_clear_tcd(uint8_t virtual_channel);

/**
 * @brief Configures the source address for the eDMA channel.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param address The pointer to the source memory address.
 */
void edma_set_src_addr(uint8_t virtual_channel, uint32_t address);

/**
 * @brief Configures the source address signed offset for the eDMA channel.
 *
 * Sign-extended offset applied to the current source address to form the
 * next-state value as each source read is complete.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param offset Signed-offset for source address.
 */
void edma_set_src_offset(uint8_t virtual_channel, int16_t offset);

/**
 * @brief Configures the source data chunk size (transferred in a read
 * sequence).
 *
 * Source data read transfer size (1/2/4/16/32 bytes).
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param size Source transfer size.
 */
void edma_set_src_transfer_size(uint8_t virtual_channel, edma_transfer_size_t size);

/**
 * @brief Configures the source address last adjustment.
 *
 * Adjustment value added to the source address at the completion of the
 * major iteration count. This value can be applied to restore the source
 * address to the initial value, or adjust the address to reference the next
 * data structure.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param adjust Adjustment value.
 */
void edma_set_src_last_addr_adjust(uint8_t virtual_channel, int32_t adjust);

/**
 * @brief Configures the destination address for the eDMA channel.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param address The pointer to the destination memory address.
 */
void edma_set_dest_addr(uint8_t virtual_channel, uint32_t address);

/**
 * @brief Configures the destination address signed offset for the eDMA
 * channel.
 *
 * Sign-extended offset applied to the current destination address to form
 * the next-state value as each destination write is complete.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param offset signed-offset
 */
void edma_set_dest_offset(uint8_t virtual_channel, int16_t offset);

/**
 * @brief Configures the destination data chunk size (transferred in a write sequence).
 *
 * Destination data write transfer size (1/2/4/16/32 bytes).
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param size Destination transfer size.
 */
void edma_set_dest_transfer_size(uint8_t virtual_channel, edma_transfer_size_t size);

/**
 * @brief Configures the destination address last adjustment.
 *
 * Adjustment value added to the destination address at the completion of
 * the major iteration count. This value can be applied to restore the
 * destination address to the initial value, or adjust the address to
 * reference the next data structure.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param adjust Adjustment value.
 */
void edma_set_dest_last_addr_adjust(uint8_t virtual_channel, int32_t adjust);

/**
 * @brief Configures the number of bytes to be transferred in each service
 * request of the channel.
 *
 * Sets the number of bytes to be transferred each time a request is
 * received (one major loop iteration). This number needs to be a multiple
 * of the source/destination transfer size, as the data block will be
 * transferred within multiple read/write sequences (minor loops).
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param nbytes Number of bytes to be transferred in each service request of the channel
 */
void edma_set_minor_loop_block_size(uint8_t virtual_channel, uint32_t nbytes);

/**
 * @brief Configures the number of major loop iterations.
 *
 * Sets the number of major loop iterations; each major loop iteration will
 * be served upon a request for the current channel, transferring the data
 * block configured for the minor loop (NBYTES).
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param major_loop_count Number of major loop iterations.
 */
void edma_set_major_loop_iteration_count(uint8_t virtual_channel, uint32_t major_loop_count);

/**
 * @brief Returns the remaining major loop iteration count.
 *
 * Gets the number minor loops yet to be triggered (major loop iterations).
 *
 * @param virtual_channel eDMA virtual channel number.
 * @return number of major loop iterations yet to be triggered
 */
uint32_t edma_get_remaining_major_iterations_count(uint8_t virtual_channel);

/**
 * @brief Configures the memory address of the next TCD, in scatter/gather mode.
 *
 * This function configures the address of the next TCD to be loaded form
 * memory, when scatter/gather feature is enabled. This address points to
 * the beginning of a 0-modulo-32 byte region containing the next transfer
 * TCD to be loaded into this channel. The channel reload is performed as
 * the major iteration count completes. The scatter/gather address must be
 * 0-modulo-32-byte. Otherwise, a configuration error is reported.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param next_tcd_addr The address of the next TCD to be linked to this TCD.
 */
void edma_set_scatter_gather_link(uint8_t virtual_channel, uint32_t next_tcd_addr);

/**
 * @brief Disables/Enables the DMA request after the major loop completes for the TCD.
 *
 * If disabled, the eDMA hardware automatically clears the corresponding DMA
 * request when the current major iteration count reaches zero.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param b_disable Disable (true)/Enable (false) DMA request after TCD complete.
 */
void edma_disable_request(uint8_t virtual_channel, bool b_disable);

/**
 * @brief Disables/Enables the channel interrupt requests.
 *
 * This function enables/disables error, half major loop and complete major
 * loop interrupts for the current channel.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param int_src Interrupt event (error/half major loop/complete major loop).
 * @param b_enable Enable (true)/Disable (false) interrupts for the current channel.
 */
void edma_configure_interrupt(uint8_t                  virtual_channel,
                              edma_channel_interrupt_t int_src,
                              bool                     b_enable);

/**
 * @brief Triggers a sw request for the current channel.
 *
 * This function starts a transfer using the current channel (sw request).
 *
 * @param virtual_channel eDMA virtual channel number.
 */
void edma_trigger_sw_request(uint8_t virtual_channel);

/** @} */

/**
 * @name eDMA Peripheral callback and interrupt functions
 * @{
 */

/**
 * @brief Registers the callback function and the parameter for eDMA channel.
 *
 * This function registers the callback function and the parameter into the
 * eDMA channel state structure. The callback function is called when the
 * channel is complete or a channel error occurs. The eDMA driver passes the
 * channel status to this callback function to indicate whether it is caused
 * by the channel complete event or the channel error event.
 *
 * To un-register the callback function, set the callback function to "NULL"
 * and call this function.
 *
 * @param virtual_channel eDMA virtual channel number.
 * @param callback The pointer to the callback function.
 * @param p_parameter The pointer to the callback function's parameter.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t edma_install_callback(uint8_t         virtual_channel,
                               edma_callback_t callback,
                               void           *p_parameter);

/** @} */

/**
 * @name eDMA Peripheral driver miscellaneous functions
 * @{
 */
/**
 * @brief Gets the eDMA channel status.
 *
 * @param virtual_channel eDMA virtual channel number.
 *
 * @return Channel status.
 */
edma_chn_status_t edma_get_channel_status(uint8_t virtual_channel);

/** @} */

/**
 * @brief Returns DMA Register Base Address.
 *
 * Gets the address of the selected DMA module.
 *
 * @param instance DMA instance to be returned.
 * @return DMA register base address
 */
DMA_t *edma_get_dma_reg_base_addr(uint32_t instance);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* EDMA_DRIVER_H */

/*** end of file ***/
