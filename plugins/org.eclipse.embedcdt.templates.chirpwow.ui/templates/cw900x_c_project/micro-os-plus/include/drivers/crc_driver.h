/**
 * @file crc_driver.h
 * @brief Header file for the crc driver.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef CRC_DRIVER_H
#define CRC_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "device_registers.h"
#include "status.h"
#include <stddef.h>

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/**
 * @brief CRC type of transpose of read write data
 * Implements : crc_transpose_t_Class
 */
typedef enum
{
    CRC_TRANSPOSE_NONE           = 0x00U, /**< No transpose */
    CRC_TRANSPOSE_BITS           = 0x01U, /**< Transpose bits in bytes */
    CRC_TRANSPOSE_BITS_AND_BYTES = 0x02U, /**< Transpose bytes and bits in bytes */
    CRC_TRANSPOSE_BYTES          = 0x03U  /**< Transpose bytes */
} crc_transpose_t;

/**
 * @brief CRC bit width
 * Implements : crc_bit_width_t_Class
 */
typedef enum
{
    CRC_BITS_16 = 0U, /**< Generate 16-bit CRC code */
    CRC_BITS_32 = 1U, /**< Generate 32-bit CRC code */
    CRC_BITS_8  = 2U, /**< Generate 8-bit CRC code */
    CRC_BITS_5  = 3U  /**< Generate 5-bit CRC code */
} crc_bit_width_t;

/**
 * @brief CRC configuration structure.
 * Implements : crc_user_config_t_Class
 */
typedef struct
{
    crc_bit_width_t crc_width;             /**< Selects bit CRC protocol. */
    uint32_t        polynomial;            /**< CRC Polynomial, MSBit first.<br/>
                                            *   Example polynomial: 0x1021U = x^12+x^5+1 */
    crc_transpose_t read_transpose;        /**< Type Of Transpose For Read. */
    crc_transpose_t write_transpose;       /**< Type Of Transpose For Writes. */
    bool            b_complement_checksum; /**< True if the result shall be complement of
                                           the actual checksum. */
    uint32_t seed;                         /**< Starting checksum value. */
} crc_user_config_t;

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Initializes the CRC module
 *
 * This function initializes the module to default configuration
 * (Initial checksum: 0U,
 * Default polynomial: 0x1021U,
 * Type of read transpose: CRC_TRANSPOSE_NONE,
 * Type of write transpose: CRC_TRANSPOSE_NONE,
 * No complement of checksum read, 32-bit CRC)
 *
 * @param[in] p_base The CRC peripheral base address
 */
void crc_init_set(CRC_t *const p_base);

/**
 * @brief Returns the current result of the CRC calculation
 *
 * This function returns the current result of the CRC calculation
 *
 * @param[in] p_base The CRC peripheral base address
 * @return Result of CRC calculation
 */
uint32_t crc_get_result_data(const CRC_t *const p_base);

/**
 * @brief Sets seed value for CRC computation
 *
 * This function sets the 32 bits of CRC data register
 *
 * @param[in] p_base The CRC peripheral base address
 * @param[in] seed_value New seed data for CRC computation
 */
void crc_set_seed_reg(CRC_t *const p_base, uint32_t seed_value);

/**
 * @brief Initializes the CRC module
 *
 * This function initializes CRC driver based on user configuration input.
 * The user must make sure that the clock is enabled
 *
 * @param[in] instance The CRC instance number
 * @param[in] p_user_config_ptr Pointer to structure of initialization
 * @return Execution status (success)
 */
status_t crc_init(uint32_t instance, const crc_user_config_t *p_user_config_ptr);

/**
 * @brief Sets the default configuration
 *
 * This function sets the default configuration
 *
 * @param[in] instance The CRC instance number
 * @return Execution status (success)
 */
status_t crc_deinit(uint32_t instance);

/**
 * @brief Appends 32-bit data to the current CRC calculation and returns new result
 *
 * This function appends 32-bit data to the current CRC calculation
 * and returns new result. If the newSeed is true, seed set and
 * result are calculated from the seed new value (new CRC calculation)
 *
 * @param[in] instance The CRC instance number
 * @param[in] data Input data for CRC calculation
 * @param[in] b_new_seed Sets new CRC calculation
 *            - true: New seed set and used for new calculation.
 *            - false: Seed argument ignored, continues old calculation.
 * @param[in] seed New seed if newSeed is true, else ignored
 * @return New CRC result
 */
uint32_t crc_get_crc32(uint32_t instance, uint32_t data, bool b_new_seed, uint32_t seed);

/**
 * @brief Appends 16-bit data to the current CRC calculation and returns new result
 *
 * This function appends 16-bit data to the current CRC calculation
 * and returns new result. If the newSeed is true, seed set and
 * result are calculated from the seed new value (new CRC calculation)
 *
 * @param[in] instance The CRC instance number
 * @param[in] data Input data for CRC calculation
 * @param[in] b_new_seed Sets new CRC calculation
 *            - true: New seed set and used for new calculation.
 *            - false: Seed argument ignored, continues old calculation.
 * @param[in] seed New seed if newSeed is true, else ignored
 * @return New CRC result
 */
uint32_t crc_get_crc16(uint32_t instance, uint16_t data, bool b_new_seed, uint32_t seed);

/**
 * @brief Appends 8-bit data to the current CRC calculation and returns new result
 *
 * This function appends 8-bit data to the current CRC calculation
 * and returns new result. If the newSeed is true, seed set and
 * result are calculated from the seed new value (new CRC calculation)
 *
 * @param[in] instance The CRC instance number
 * @param[in] data Input data for CRC calculation
 * @param[in] b_new_seed Sets new CRC calculation
 *            - true: New seed set and used for new calculation.
 *            - false: Seed argument ignored, continues old calculation.
 * @param[in] seed New seed if newSeed is true, else ignored
 * @return New CRC result
 */
uint32_t crc_get_crc8(uint32_t instance, uint8_t data, bool b_new_seed, uint32_t seed);

/**
 * @brief Appends a block of bytes to the current CRC calculation
 *
 * This function appends a block of bytes to the current CRC calculation
 *
 * @param[in] instance The CRC instance number
 * @param[in] p_data Data for current CRC calculation
 * @param[in] data_size Length of data to be calculated
 */
void crc_write_data(uint32_t instance, const uint8_t *p_data, uint32_t data_size);

/**
 * @brief Returns the current result of the CRC calculation
 *
 * This function returns the current result of the CRC calculation
 *
 * @param[in] instance The CRC instance number
 * @return Result of CRC calculation
 */
uint32_t crc_get_result(uint32_t instance);

/**
 * @brief Configures the CRC module from a user configuration structure
 *
 * This function configures the CRC module from a user configuration structure
 *
 * @param[in] instance The CRC instance number
 * @param[in] p_user_config_ptr Pointer to structure of initialization
 * @return Execution status (success)
 */
status_t crc_set_config(uint32_t instance, const crc_user_config_t *p_user_config_ptr);

/**
 * @brief Get configures of the CRC module currently
 *
 * This function Get configures of the CRC module currently
 *
 * @param[in] instance The CRC instance number
 * @param[out] p_user_config_ptr Pointer to structure of initialization
 * @return Execution status (success)
 */
status_t crc_get_config(uint32_t instance, crc_user_config_t *const p_user_config_ptr);

/**
 * @brief Get default configures the CRC module for configuration structure
 *
 * This function Get default configures the CRC module for user
 * configuration structure
 *
 * @param[out] p_user_config_ptr Pointer to structure of initialization
 * @return Execution status (success)
 */
status_t crc_get_default_config(crc_user_config_t *const p_user_config_ptr);

#ifdef __cplusplus
}
#endif

#endif /* CRC_DRIVER_H */

/*** end of file ***/
