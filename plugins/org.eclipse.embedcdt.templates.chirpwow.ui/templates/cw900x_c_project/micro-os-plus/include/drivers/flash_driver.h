/**
 * @file flash_driver.h
 * @brief Flash driver header file.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef FLASH_DRIVER_H
#define FLASH_DRIVER_H

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
#define FTFC_FCCOB3 FTFC->FCCOB[0]
#define FTFC_FCCOB2 FTFC->FCCOB[1]
#define FTFC_FCCOB1 FTFC->FCCOB[2]
#define FTFC_FCCOB0 FTFC->FCCOB[3]
#define FTFC_FCCOB7 FTFC->FCCOB[4]
#define FTFC_FCCOB6 FTFC->FCCOB[5]
#define FTFC_FCCOB5 FTFC->FCCOB[6]
#define FTFC_FCCOB4 FTFC->FCCOB[7]
#define FTFC_FCCOBB FTFC->FCCOB[8]
#define FTFC_FCCOBA FTFC->FCCOB[9]
#define FTFC_FCCOB9 FTFC->FCCOB[10]
#define FTFC_FCCOB8 FTFC->FCCOB[11]

#define CLEAR_FTFC_FSTAT_ERROR_BITS \
    FTFC->FSTAT =                   \
        (uint8_t)(FTFC_FSTAT_FPVIOL_MASK | FTFC_FSTAT_ACCERR_MASK | FTFC_FSTAT_RDCOLERR_MASK)

/* Word size 2 bytes */
#define FTFC_WORD_SIZE 0x0002U

/* Phrase size 8 bytes */
#define FTFC_PHRASE_SIZE 0x0008U

/*******************************************************************************
 * Flash hardware algorithm operation commands
 *******************************************************************************/
#define FTFx_VERIFY_BLOCK     0x00U
#define FTFx_VERIFY_SECTION   0x01U
#define FTFx_PROGRAM_CHECK    0x02U
#define FTFx_PROGRAM_PHRASE   0x07U
#define FTFx_ERASE_BLOCK      0x08U
#define FTFx_ERASE_SECTOR     0x09U
#define FTFx_PROGRAM_SECTION  0x0BU
#define FTFx_VERIFY_ALL_BLOCK 0x40U
#define FTFx_READ_ONCE        0x41U
#define FTFx_PROGRAM_ONCE     0x43U
#define FTFx_ERASE_ALL_BLOCK  0x44U

/** @brief Resume wait count used in FlashResume function */
#define RESUME_WAIT_CNT 0x20U

#define GET_BIT_0_7(value)   ((uint8_t)(((uint32_t)(value)) & 0xFFU))
#define GET_BIT_8_15(value)  ((uint8_t)((((uint32_t)(value)) >> 8) & 0xFFU))
#define GET_BIT_16_23(value) ((uint8_t)((((uint32_t)(value)) >> 16) & 0xFFU))
#define GET_BIT_24_31(value) ((uint8_t)(((uint32_t)(value)) >> 24))

/**
 * @brief Return Code Definition for FTFx SSD
 *
 * Implements: flash_drv_status_t_Class
 */
typedef enum
{
    FTFx_OK             = 0x0000U, /**< Function executes successfully */
    FTFx_ERR_MGSTAT0    = 0x0001U, /**< MGSTAT0 error */
    FTFx_ERR_PVIOL      = 0x0010U, /**< Protection violation */
    FTFx_ERR_ACCERR     = 0x0020U, /**< Flash Access error */
    FTFx_ERR_RDCOLERR   = 0x0040U, /**< Read Collision Error */
    FTFx_ERR_CHANGEPROT = 0x0100U, /**< Cannot change protection status */
    FTFx_ERR_NOEEE      = 0x0200U, /**< FlexRAM is not set for EEPROM use */
    FTFx_ERR_EFLASHONLY = 0x0400U, /**< FlexNVM is set for full EEPROM backup */
    FTFx_ERR_RAMRDY     = 0x0800U, /**< Program accel. RAM is not available */
    FTFx_ERR_RANGE      = 0x1000U, /**< Address is out of the valid range */
    FTFx_ERR_SIZE       = 0x2000U, /**< Misaligned size */
    FTFx_ERR_PARAM      = 0x4000U  /**< Invalid parameter */
} flash_drv_status_t;

/*******************************************************************************
 * CallBack function period
 *******************************************************************************/
#ifndef FLASH_CALLBACK_CS
/** @brief  Callback period count for flash_check_sum */
#define FLASH_CALLBACK_CS 0x0AU
#endif

/*******************************************************************************
 * Null Callback function definition
 *******************************************************************************/
/**
 * @name Null Callback function definition
 * @{
 */
/** @brief  Null callback */
#define NULL_CALLBACK ((flash_callback_t)0xFFFFFFFFU)
/** @}*/

/*******************************************************************************
 * Callback function prototype
 *******************************************************************************/
/** @brief Call back function pointer data type */
typedef void (*flash_callback_t)(void);

/*******************************************************************************
 * Flash SSD Configuration Structure
 *******************************************************************************/
/**
 * @brief Flash User Configuration Structure
 *
 * Implements: flash_user_config_t_Class
 */
typedef struct
{
    uint32_t         pflash_base; /**< The base address of P-Flash memory */
    uint32_t         pflash_size; /**< The size in byte of P-Flash memory */
    uint32_t         ee_ram_base; /**< The base address of FlexRAMor acceleration RAM memory */
    flash_callback_t callback;    /**< Call back function to service the time critical events */
} flash_user_config_t;

/**
 * @brief Flash SSD Configuration Structure
 *
 * The structure includes the static parameters for FTFx which are device-dependent.
 * The fields including PFlashBlockBase, PFlashBlockSize, EERAMBlockBase, and CallBack
 * are passed via flash_user_config_t.
 *
 * Implements: flash_ssd_config_t_Class
 */
typedef struct
{
    uint32_t         pflash_base; /**< The base address of P-Flash memory */
    uint32_t         pflash_size; /**< The size in byte of P-Flash memory */
    uint32_t         ee_ram_base; /**< The base address of FlexRAM or acceleration RAM memory */
    flash_callback_t callback;    /**< Call back function to service the time critical events */
} flash_ssd_config_t;

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/** @brief flash_cmd_sequence function pointer */
typedef flash_drv_status_t (*flash_command_sequence_t)(const flash_ssd_config_t *p_ssd_config);

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Initializes Flash.
 *
 * This API initializes Flash module by clearing status error
 * bit and reporting the memory configuration via SSD configuration structure.
 *
 * @param[in] p_user_conf The user configuration structure pointer.
 * @param[in] p_ssd_config The SSD configuration structure pointer.
 * @return operation status
 *        - FTFx_OK:    Operation was successful.
 */
flash_drv_status_t flash_init(const flash_user_config_t *const p_user_conf,
                              flash_ssd_config_t *const        p_ssd_config);

/**
 * @brief Flash command sequence.
 *
 * This API is used to perform command write sequence on Flash.
 * It is internal function, called by driver APIs only.
 *
 * @param[in] p_ssd_config The SSD configuration structure pointer.
 * @return operation status
 *        - FTFx_OK:             Operation was successful.
 *        - FTFx_ERR_ACCERR:     Operation failed due to an access error.
 *        - FTFx_ERR_PVIOL:      Operation failed due to a protection violation.
 *        - FTFx_ERR_MGSTAT0:    Operation failed due to an error was
 *                                detected during execution of an FTFx command.
 *        - FTFC_FSTAT_RDCOLERR_MASK:    Operation failed due to a read collision error.
 */
START_FUNCTION_DECLARATION_RAMSECTION
flash_drv_status_t flash_cmd_sequence(const flash_ssd_config_t *p_ssd_config)
    END_FUNCTION_DECLARATION_RAMSECTION;

/**
 * @brief Flash erase all blocks.
 *
 * This API erases all Flash memory, initializes the FlexRAM, verifies
 * all memory contents, and then releases the MCU security.
 *
 * @param[in] p_ssd_config The SSD configuration structure pointer.
 * @param[in] pflash_cmd_sequence Pointer to the Flash command sequence
 * function.
 * @return operation status
 *        - FTFx_OK:             Operation was successful.
 *        - FTFx_ERR_ACCERR:     Operation failed due to an access error.
 *        - FTFx_ERR_PVIOL:      Operation failed due to a protection violation.
 *        - FTFx_ERR_MGSTAT0:    Operation failed due to an error was
 *                                detected during execution of an FTFx command.
 */
flash_drv_status_t flash_erase_all_block(const flash_ssd_config_t *p_ssd_config,
                                         flash_command_sequence_t  pflash_cmd_sequence);

/**
 * @brief Flash verify all blocks.
 *
 * This function checks to see if the P-Flash have been erased to the specified read
 * margin level, if applicable, and releases security if the readout passes.
 *
 * @param[in] p_ssd_config The SSD configuration structure pointer.
 * @param[in] margin_level Read Margin Choice as follows:
 *                         - margin_level = 0x0U: use the Normal read level
 *                         - margin_level = 0x1U: use the User read
 *                         - margin_level = 0x2U: use the Factory read
 * @param[in] pflash_cmd_sequence Pointer to the Flash command sequence function.
 * @return operation status
 *        - FTFx_OK:             Operation was successful.
 *        - FTFx_ERR_ACCERR:     Operation failed due to an access error.
 *        - FTFx_ERR_MGSTAT0:    Operation failed due to an error was
 *                                detected during execution of an FTFx command.
 */
flash_drv_status_t flash_verify_all_block(const flash_ssd_config_t *p_ssd_config,
                                          uint8_t                   margin_level,
                                          flash_command_sequence_t  pflash_cmd_sequence);

/**
 * @brief Flash erase sector.
 *
 * This API erases one or more sectors in P-Flash.
 * This API always returns FTFx_OK if size provided by the user is
 * zero regardless of the input validation.
 *
 * @param[in] p_ssd_config The SSD configuration structure pointer.
 * @param[in] dest Address in the first sector to be erased.
 * @param[in] size Size to be erased in bytes. It is used to determine
 *                  number of sectors to be erased.
 * @param[in] pflash_cmd_sequence Pointer to the Flash command sequence function.
 *
 * @return operation status
 *        - FTFx_OK:             Operation was successful.
 *        - FTFx_ERR_ACCERR:     Operation failed due to an access error.
 *        - FTFx_ERR_PVIOL:      Operation failed due to a protection violation.
 *        - FTFx_ERR_MGSTAT0:    Operation failed due to an error was
 *                                detected during execution of an FTFx command.
 *        - FTFx_ERR_SIZE:       Operation failed due to misaligned size.
 */
flash_drv_status_t flash_erase_sector(const flash_ssd_config_t *p_ssd_config,
                                      uint32_t                  dest,
                                      uint32_t                  size,
                                      flash_command_sequence_t  pflash_cmd_sequence);

/**
 * @brief Flash verify section.
 *
 * This API checks if a section of the P-Flash memory is erased to the specified
 * read margin level.
 *
 * @param[in] p_ssd_config The SSD configuration structure pointer.
 * @param[in] dest Start address for the intended verify operation.
 * @param[in] number Number of alignment unit to be verified. Refer to corresponding
 *              reference manual to get correct information of alignment constrain.
 * @param[in] margin_level Read Margin Choice as follows:
 *                        - margin_level = 0x0U: use Normal read level
 *                        - margin_level = 0x1U: use the User read
 *                        - margin_level = 0x2U: use the Factory read
 * @param[in] pflash_cmd_sequence Pointer to the Flash command sequence function.
 * @return operation status
 *        - FTFx_OK:             Operation was successful.
 *        - FTFx_ERR_ACCERR:     Operation failed due to an access error.
 *        - FTFx_ERR_MGSTAT0:    Operation failed due to an error was
 *                                detected during execution of an FTFx command.
 */
flash_drv_status_t flash_verify_section(const flash_ssd_config_t *p_ssd_config,
                                        uint32_t                  dest,
                                        uint16_t                  number,
                                        uint8_t                   margin_level,
                                        flash_command_sequence_t  pflash_cmd_sequence);

/**
 * @brief Flash read once.
 *
 * This API is used to read out a reserved 64 byte field located in the
 * P-Flash IFR via given number of record. See the corresponding reference
 * manual to get the correct value of this number.
 *
 * @param[in] p_ssd_config The SSD configuration structure pointer.
 * @param[in] record_index The record index will be read. It can be from 0x0U
 *                        to 0x7U or from 0x0U to 0xF according to specific derivative.
 * @param[in] p_data_array Pointer to the array to return the data read by the read once command.
 * @param[in] pflash_cmd_sequence Pointer to the Flash command sequence function.
 * @return operation status
 *        - FTFx_OK:             Operation was successful.
 *        - FTFx_ERR_ACCERR:     Operation failed due to an access error.
 */
flash_drv_status_t flash_read_once(const flash_ssd_config_t *p_ssd_config,
                                   uint8_t                   record_index,
                                   uint8_t                  *p_data_array,
                                   flash_command_sequence_t  pflash_cmd_sequence);

/**
 * @brief Flash program once.
 *
 * This API is used to program to a reserved 64 byte field located in the
 * P-Flash IFR via given number of record. See the corresponding reference
 * manual to get correct value of this number.
 *
 * @param[in] p_ssd_config The SSD configuration structure pointer.
 * @param[in] record_index The record index will be read. It can be from 0x0U
 *                        to 0x7U or from 0x0U to 0xF according to specific derivative.
 * @param[in] p_data_array Pointer to the array from which data will be
 *                       taken for program once command.
 * @param[in] pflash_cmd_sequence Pointer to the Flash command sequence function.
 * @return operation status
 *        - FTFx_OK:             Operation was successful.
 *        - FTFx_ERR_ACCERR:     Operation failed due to an access error.
 *        - FTFx_ERR_MGSTAT0:    Operation failed due to an error was
 *                                detected during execution of an FTFx command.
 */
flash_drv_status_t flash_program_once(const flash_ssd_config_t *p_ssd_config,
                                      uint8_t                   record_index,
                                      const uint8_t            *p_data_array,
                                      flash_command_sequence_t  pflash_cmd_sequence);

/**
 * @brief Flash program
 *
 * This API is used to program 8 consecutive bytes (for program phrase command) on P-Flash.
 * This API always returns FTFx_OK if size provided by user is zero regardless of
 * the input validation
 *
 * @param[in] p_ssd_config The SSD configuration structure pointer.
 * @param[in] dest Start address for the intended program operation.
 * @param[in] size Size in byte to be programmed
 * @param[in] p_data Pointer of source address from which data has to
 *                  be taken for program operation.
 * @param[in] pflash_cmd_sequence Pointer to the Flash command sequence function.
 * @return operation status
 *        - FTFx_OK:             Operation was successful.
 *        - FTFx_ERR_ACCERR:     Operation failed due to an access error.
 *        - FTFx_ERR_MGSTAT0:    Operation failed due to an error was
 *                                detected during execution of an FTFx command.
 *        - FTFx_ERR_SIZE:       Operation failed due to misaligned size.
 */
flash_drv_status_t flash_program(const flash_ssd_config_t *p_ssd_config,
                                 uint32_t                  dest,
                                 uint32_t                  size,
                                 const uint8_t            *p_data,
                                 flash_command_sequence_t  pflash_cmd_sequence);

/**
 * @brief Flash program check
 *
 * This API tests a previously programmed P-Flashto see if it reads correctly
 * at the specified margin level. This API always returns FTFx_OK if size provided
 * by user is zero regardless of the input validation
 *
 * @param[in] p_ssd_config The SSD configuration structure pointer.
 * @param[in] dest Start address for the intended program check operation.
 * @param[in] size Size in byte to check accuracy of program operation
 * @param[in] p_expecte_data The pointer to the expected data.
 * @param[in] p_fail_addr Returned the first aligned failing address.
 * @param[in] margin_level Read margin choice as follows:
 *                        - margin_level = 0x1U: read at User margin 1/0 level.
 *                        - margin_level = 0x2U: read at Factory margin 1/0 level.
 * @param[in] pflash_cmd_sequence Pointer to the Flash command sequence function.
 * @return operation status
 *        - FTFx_OK:             Operation was successful.
 *        - FTFx_ERR_ACCERR:     Operation failed due to an access error.
 *        - FTFx_ERR_MGSTAT0:    Operation failed due to an error was
 *                                detected during execution of an FTFx command.
 */
flash_drv_status_t flash_program_check(const flash_ssd_config_t *p_ssd_config,
                                       uint32_t                  dest,
                                       uint32_t                  size,
                                       const uint8_t            *p_expecte_data,
                                       uint32_t                 *p_fail_addr,
                                       uint8_t                   margin_level,
                                       flash_command_sequence_t  pflash_cmd_sequence);

/**
 * @brief Calculates check sum.
 *
 * This API performs 32 bit sum of each byte data over a specified Flash memory
 * range without carry which provides rapid method for checking data integrity.
 *
 * @param[in] p_ssd_config The SSD configuration structure pointer.
 * @param[in] dest Start address of the Flash range to be summed.
 * @param[in] size Size in byte of the Flash range to be summed.
 * @param[in] pSum To return the sum value.
 * @return operation status
 *        - FTFx_OK:           Operation was successful.
 *        - FTFx_ERR_RANGE:    Operation failed due to addresses were out of
 *                              the valid range.
 */
flash_drv_status_t flash_check_sum(const flash_ssd_config_t *p_ssd_config,
                                   uint32_t                  dest,
                                   uint32_t                  size,
                                   uint32_t                 *pSum);

/**
 * @brief Flash program section
 *
 * This API will program the data found in the Section Program Buffer
 * to previously erased locations in the Flash memory. Data is preloaded
 * into the Section Program Buffer by writing to the acceleration Ram and
 * FlexRam while it is set to function as a RAM. The Section Program Buffer
 * is limited to the value of FlexRam divides by a ratio. Refer to the
 * associate reference manual to get correct value of this ratio.
 *
 * @param[in] p_ssd_config The SSD configuration structure pointer.
 * @param[in] dest Start address for the intended program operation.
 * @param[in] number Number of alignment unit to be programmed. Refer to associate
 *                    reference manual to get correct value of this alignment constrain.
 * @param[in] pflash_cmd_sequence Pointer to the Flash command sequence function.
 * @return operation status
 *        - FTFx_OK:             Operation was successful.
 *        - FTFx_ERR_ACCERR:     Operation failed due to an access error.
 *        - FTFx_ERR_PVIOL:      Operation failed due to a protection violation.
 *        - FTFx_ERR_MGSTAT0:    Operation failed due to an error was
 *                                detected during execution of an FTFx command.
 *        - FTFx_ERR_RAMRDY:     Operation failed due to FlexRAM was not
 *                                available for traditional RAM access.
 */
flash_drv_status_t flash_program_section(const flash_ssd_config_t *p_ssd_config,
                                         uint32_t                  dest,
                                         uint16_t                  number,
                                         flash_command_sequence_t  pflash_cmd_sequence);

/**
 * @brief Flash erase block
 *
 * This API erases all addresses in an individual P-Flash block.
 * For the derivatives including multiply logical P-Flash blocks,
 * this API erases a single block in a single call.
 *
 * @param[in] p_ssd_config The SSD configuration structure pointer.
 * @param[in] dest Start address for the intended erase operation.
 * @param[in] pflash_cmd_sequence Pointer to the Flash command sequence function.
 * @return operation status
 *        - FTFx_OK:             Operation was successful.
 *        - FTFx_ERR_ACCERR:     Operation failed due to an access error.
 *        - FTFx_ERR_PVIOL:      Operation failed due to a protection violation.
 *        - FTFx_ERR_MGSTAT0:    Operation failed due to an error was
 *                                detected during execution of an FTFx command.
 */
flash_drv_status_t flash_erase_block(const flash_ssd_config_t *p_ssd_config,
                                     uint32_t                  dest,
                                     flash_command_sequence_t  pflash_cmd_sequence);

/**
 * @brief Flash verify block
 *
 * This API checks to see if an entire P-Flash block has been erased to
 * the specified margin level. For the derivatives including multiply logical
 * P-Flash blocks, this API erases a single block in a single call.
 *
 * @param[in] p_ssd_config The SSD configuration structure pointer.
 * @param[in] dest Start address for the intended verify operation.
 * @param[in] margin_level Read Margin Choice as follows:
 *                        - margin_level = 0x0U: use Normal read level
 *                        - margin_level = 0x1U: use the User read
 *                        - margin_level = 0x2U: use the Factory read
 * @param[in] pflash_cmd_sequence Pointer to the Flash command sequence function.
 * @return operation status
 *        - FTFx_OK:             Operation was successful.
 *        - FTFx_ERR_ACCERR:     Operation failed due to an access error.
 *        - FTFx_ERR_MGSTAT0:    Operation failed due to an error was
 *                                detected during execution of an FTFx command.
 */
flash_drv_status_t flash_verify_block(const flash_ssd_config_t *p_ssd_config,
                                      uint32_t                  dest,
                                      uint8_t                   margin_level,
                                      flash_command_sequence_t  pflash_cmd_sequence);

#ifdef __cplusplus
}
#endif

#endif /* FLASH_DRIVER_H */

/*** end of file ***/
