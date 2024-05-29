/**
 * @file flash_driver.c
 * @brief Flash erase and program operations
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "flash_driver.h"

/*******************************************************************************
 * Global functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : flash_init
 * Description   : Initializes Flash module by clearing status error bit
 * and reporting the memory configuration via SSD configuration structure.
 *
 * Implements    : flash_init_activity
 *END**************************************************************************/
flash_drv_status_t
flash_init(const flash_user_config_t *const p_user_conf, flash_ssd_config_t *const p_ssd_config)
{
#ifdef DEV_ERROR_DETECT
    DEV_ASSERT(p_user_conf != NULL);
    DEV_ASSERT(p_ssd_config != NULL);
#endif
    flash_drv_status_t ret = FTFx_OK;

    p_ssd_config->pflash_base = p_user_conf->pflash_base;
    p_ssd_config->pflash_size = p_user_conf->pflash_size;
    p_ssd_config->ee_ram_base = p_user_conf->ee_ram_base;
    p_ssd_config->callback    = p_user_conf->callback;

    return (ret);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flash_cmd_sequence
 * Description   : Perform command write sequence on Flash.
 * It is internal function, called by driver APIs only.
 *
 * Implements    : flash_cmd_sequence_activity
 *END**************************************************************************/
START_FUNCTION_DEFINITION_RAMSECTION
flash_drv_status_t
flash_cmd_sequence(const flash_ssd_config_t *p_ssd_config)
{
#ifdef DEV_ERROR_DETECT
    DEV_ASSERT(p_ssd_config != NULL);
#endif
    flash_drv_status_t ret; /* Return code variable */
    DISABLE_INTERRUPTS();

    /* Clear CCIF to launch command */
    FTFC->FSTAT |= FTFC_FSTAT_CCIF_MASK;

    while (0U == (FTFC->FSTAT & FTFC_FSTAT_CCIF_MASK))
    {
        /* Wait till CCIF bit is set
         * Serve callback function if counter reaches limitation
         */
        if (NULL_CALLBACK != p_ssd_config->callback)
        {
            (p_ssd_config->callback)();
        }
    }

    ret = (flash_drv_status_t)(FTFC->FSTAT & (FTFC_FSTAT_MGSTAT0_MASK | FTFC_FSTAT_FPVIOL_MASK |
                                              FTFC_FSTAT_ACCERR_MASK | FTFC_FSTAT_RDCOLERR_MASK));

    ENABLE_INTERRUPTS();
    return (ret);
}
END_FUNCTION_DEFINITION_RAMSECTION

/*FUNCTION**********************************************************************
 *
 * Function Name : flash_erase_all_block
 * Description   : Erases all Flash memory, initializes the FlexRAM, verifies
 *                  all memory contents, and then releases the MCU security.
 *
 * Implements    : flash_erase_all_block_activity
 *END**************************************************************************/
flash_drv_status_t
flash_erase_all_block(const flash_ssd_config_t *p_ssd_config,
                      flash_command_sequence_t  pflash_cmd_sequence)
{
#ifdef DEV_ERROR_DETECT
    DEV_ASSERT(p_ssd_config != NULL);
    DEV_ASSERT(pflash_cmd_sequence != NULL);
#endif
    flash_drv_status_t ret; /* Return code variable */

    /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1
     * to clear */
    CLEAR_FTFC_FSTAT_ERROR_BITS;

    /* Passing parameter to the command */
    FTFC_FCCOB0 = FTFx_ERASE_ALL_BLOCK;

    /* Calling flash command sequence function to execute the command */
    ret = pflash_cmd_sequence(p_ssd_config);

    return (ret);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flash_verify_all_block
 * Description   : Checks to see if the P-Flash have been erased to the specified read
 *                  margin level, if applicable, and releases security if the readout passes.
 *
 * Implements    : flash_verify_all_block_activity
 *END**************************************************************************/
flash_drv_status_t
flash_verify_all_block(const flash_ssd_config_t *p_ssd_config,
                       uint8_t                   margin_level,
                       flash_command_sequence_t  pflash_cmd_sequence)
{
#ifdef DEV_ERROR_DETECT
    DEV_ASSERT(p_ssd_config != NULL);
    DEV_ASSERT(pflash_cmd_sequence != NULL);
#endif
    flash_drv_status_t ret; /* Return code variable */

    /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1
     * to clear */
    CLEAR_FTFC_FSTAT_ERROR_BITS;

    /* Passing parameter to the command */
    FTFC_FCCOB0 = FTFx_VERIFY_ALL_BLOCK;
    FTFC_FCCOB1 = margin_level;

    /* Calling flash command sequence function to execute the command */
    ret = pflash_cmd_sequence(p_ssd_config);

    return (ret);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flash_erase_sector
 * Description   : Erases one or more sectors in P-Flash memory.
 *                  This API always returns FTFx_OK if size provided by the user is
 *                  zero regardless of the input validation.
 *
 * Implements    : flash_erase_sector_activity
 *END**************************************************************************/
flash_drv_status_t
flash_erase_sector(const flash_ssd_config_t *p_ssd_config,
                   uint32_t                  dest,
                   uint32_t                  size,
                   flash_command_sequence_t  pflash_cmd_sequence)
{
#ifdef DEV_ERROR_DETECT
    DEV_ASSERT(p_ssd_config != NULL);
    DEV_ASSERT(pflash_cmd_sequence != NULL);
#endif
    flash_drv_status_t ret = FTFx_OK;    /* Return code variable */
    uint32_t           sector_size;      /* Size of one sector   */
    uint32_t           temp;             /* Temporary variable   */
    uint32_t           temp_size = size; /* Temporary of size variation */

    {
        temp = p_ssd_config->pflash_base;
        if ((dest >= temp) && (dest < (temp + p_ssd_config->pflash_size)))
        {
#ifdef DEV_ERROR_DETECT
            DEV_ASSERT((dest % FEATURE_FLS_PF_SECTOR_CMD_ADDRESS_ALIGMENT) == 0U);
#endif
            dest -= temp;
            sector_size = (uint32_t)FEATURE_FLS_PF_BLOCK_SECTOR_SIZE;
        }
        else
        {
            ret         = FTFx_ERR_ACCERR;
            temp_size   = 0U;
            sector_size = 0U;
        }
    }

    /* check if the size is sector alignment or not */
    if ((temp_size & (sector_size - 1U)) != 0U)
    {
        /* return an error code FTFx_ERR_SIZE */
        ret = FTFx_ERR_SIZE;
    }

    while ((temp_size > 0U) && (FTFx_OK == ret))
    {
        /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write
         * 1 to clear */
        CLEAR_FTFC_FSTAT_ERROR_BITS;

        /* Passing parameter to the command */
        FTFC_FCCOB0 = FTFx_ERASE_SECTOR;
        FTFC_FCCOB1 = GET_BIT_16_23(dest);
        FTFC_FCCOB2 = GET_BIT_8_15(dest);
        FTFC_FCCOB3 = GET_BIT_0_7(dest);

        /* Calling flash command sequence function to execute the command */
        ret = pflash_cmd_sequence(p_ssd_config);

        /* Update size and destination address */
        temp_size -= sector_size;
        dest += sector_size;
    }

    return (ret);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flash_verify_section
 * Description   : Checks if a section of the P-Flash memory is erased to
 *                  the specified read margin level.
 *
 * Implements    : flash_verify_section_activity
 *END**************************************************************************/
flash_drv_status_t
flash_verify_section(const flash_ssd_config_t *p_ssd_config,
                     uint32_t                  dest,
                     uint16_t                  number,
                     uint8_t                   margin_level,
                     flash_command_sequence_t  pflash_cmd_sequence)
{
#ifdef DEV_ERROR_DETECT
    DEV_ASSERT(p_ssd_config != NULL);
    DEV_ASSERT(pflash_cmd_sequence != NULL);
#endif
    flash_drv_status_t ret = FTFx_OK; /* Return code variable */
    uint32_t           temp;

    /* Check if the destination is aligned or not */
    {
        temp = p_ssd_config->pflash_base;
        if ((dest >= temp) && (dest < (temp + p_ssd_config->pflash_size)))
        {
#ifdef DEV_ERROR_DETECT
            DEV_ASSERT((dest % FEATURE_FLS_PF_SECTION_CMD_ADDRESS_ALIGMENT) == 0U);
#endif
            dest -= temp;
        }
        else
        {
            ret = FTFx_ERR_ACCERR;
        }
    }
    if (FTFx_OK == ret)
    {
        /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write
         * 1 to clear */
        CLEAR_FTFC_FSTAT_ERROR_BITS;

        /* Passing parameter to the command */
        FTFC_FCCOB0 = FTFx_VERIFY_SECTION;
        FTFC_FCCOB1 = GET_BIT_16_23(dest);
        FTFC_FCCOB2 = GET_BIT_8_15(dest);
        FTFC_FCCOB3 = GET_BIT_0_7(dest);
        FTFC_FCCOB4 = GET_BIT_8_15(number);
        FTFC_FCCOB5 = GET_BIT_0_7(number);
        FTFC_FCCOB6 = margin_level;

        /* Calling flash command sequence function to execute the command */
        ret = pflash_cmd_sequence(p_ssd_config);
    }

    return (ret);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flash_read_once
 * Description   : Read out a reserved 64 byte field located in the P-Flash IFR
 * via given number of record. See the corresponding reference manual to get the
 * correct value of this number.
 *
 * Implements    : flash_read_once_activity
 *END**************************************************************************/
flash_drv_status_t
flash_read_once(const flash_ssd_config_t *p_ssd_config,
                uint8_t                   record_index,
                uint8_t                  *p_data_array,
                flash_command_sequence_t  pflash_cmd_sequence)
{
#ifdef DEV_ERROR_DETECT
    DEV_ASSERT(p_ssd_config != NULL);
    DEV_ASSERT(p_data_array != NULL);
    DEV_ASSERT(pflash_cmd_sequence != NULL);
#endif
    flash_drv_status_t ret;  /* Return code variable */
    uint32_t           temp; /* Temporary variable */
    uint8_t            idx;

    /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1
     * to clear */
    CLEAR_FTFC_FSTAT_ERROR_BITS;

    /* Passing parameter to the command */
    FTFC_FCCOB0 = FTFx_READ_ONCE;
    FTFC_FCCOB1 = record_index;

    /* Calling flash command sequence function to execute the command */
    ret = pflash_cmd_sequence(p_ssd_config);

    /* Checking for the success of command execution */
    if (FTFx_OK == ret)
    {
        /* Read the data from the FCCOB registers into the p_data_array */
        for (idx = 0U; idx < FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE; idx++)
        {
            temp              = FTFC_BASE + idx + 0x08U;
            p_data_array[idx] = *(uint8_t *)temp;
        }
    }

    return (ret);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flash_program_once
 * Description   : Program to a reserved 64 byte field located in the
 * P-Flash IFR via given number of record. See the corresponding reference
 * manual to get correct value of this number.
 *
 * Implements    : flash_program_once_activity
 *END**************************************************************************/
flash_drv_status_t
flash_program_once(const flash_ssd_config_t *p_ssd_config,
                   uint8_t                   record_index,
                   const uint8_t            *p_data_array,
                   flash_command_sequence_t  pflash_cmd_sequence)
{
#ifdef DEV_ERROR_DETECT
    DEV_ASSERT(p_ssd_config != NULL);
    DEV_ASSERT(p_data_array != NULL);
    DEV_ASSERT(pflash_cmd_sequence != NULL);
#endif
    flash_drv_status_t ret;  /* Return code variable */
    uint32_t           temp; /* Temporary variable */
    uint8_t            idx;

    /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1
     * to clear */
    CLEAR_FTFC_FSTAT_ERROR_BITS;

    /* Passing parameter to the command */
    FTFC_FCCOB0 = FTFx_PROGRAM_ONCE;
    FTFC_FCCOB1 = record_index;

    for (idx = 0U; idx < FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE; idx++)
    {
        temp             = FTFC_BASE + idx + 0x08U;
        *(uint8_t *)temp = p_data_array[idx];
    }

    /* Calling flash command sequence function to execute the command */
    ret = pflash_cmd_sequence(p_ssd_config);

    return (ret);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flash_program
 * Description   : Program 8 consecutive bytes (for program phrase command) on P-Flash block.
 *                  This API always returns FTFx_OK if size provided by user is zero
 *                  regardless of the input validation.
 *
 * Implements    : flash_program_activity
 *END**************************************************************************/
flash_drv_status_t
flash_program(const flash_ssd_config_t *p_ssd_config,
              uint32_t                  dest,
              uint32_t                  size,
              const uint8_t            *p_data,
              flash_command_sequence_t  pflash_cmd_sequence)
{
#ifdef DEV_ERROR_DETECT
    DEV_ASSERT(p_ssd_config != NULL);
    DEV_ASSERT(p_data != NULL);
    DEV_ASSERT(pflash_cmd_sequence != NULL);
#endif
    flash_drv_status_t ret = FTFx_OK; /* Return code variable */
    uint32_t           temp;
    uint8_t            idx;

    if ((size & (FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE - 1U)) != 0U)
    {
        ret = FTFx_ERR_SIZE;
    }
    else
    {
        {
            temp = p_ssd_config->pflash_base;
            if ((dest >= temp) && (dest < (temp + p_ssd_config->pflash_size)))
            {
                dest -= temp;
            }
            else
            {
                ret = FTFx_ERR_ACCERR;
            }
        }
        while ((size > 0U) && (FTFx_OK == ret))
        {
            /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register.
             * Write 1 to clear */
            CLEAR_FTFC_FSTAT_ERROR_BITS;

            /* Passing parameter to the command */
            FTFC_FCCOB0 = FTFx_PROGRAM_PHRASE;
            FTFC_FCCOB1 = GET_BIT_16_23(dest);
            FTFC_FCCOB2 = GET_BIT_8_15(dest);
            FTFC_FCCOB3 = GET_BIT_0_7(dest);

            for (idx = 0U; idx < FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE; idx++)
            {
                temp               = FTFC_BASE + idx + 0x08U;
                *(uint8_t *)(temp) = p_data[idx];
            }

            /* Calling flash command sequence function to execute the command */
            ret = pflash_cmd_sequence(p_ssd_config);

            /* Update destination address for next iteration */
            dest += FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE;
            /* Update size for next iteration */
            size -= FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE;
            /* Increment the source address by 1 */
            p_data += FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE;
        }
    }

    return (ret);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flash_program_check
 * Description   : Tests a previously programmed P-Flash to see if it reads
 *                  correctly at the specified margin level. This API always
 *                  returns FTFx_OK if size provided by user is zero
 *                  regardless of the input validation
 *
 * Implements    : flash_program_check_activity
 *END**************************************************************************/
flash_drv_status_t
flash_program_check(const flash_ssd_config_t *p_ssd_config,
                    uint32_t                  dest,
                    uint32_t                  size,
                    const uint8_t            *p_expecte_data,
                    uint32_t                 *p_fail_addr,
                    uint8_t                   margin_level,
                    flash_command_sequence_t  pflash_cmd_sequence)
{
#ifdef DEV_ERROR_DETECT
    DEV_ASSERT(p_ssd_config != NULL);
    DEV_ASSERT(p_expecte_data != NULL);
    DEV_ASSERT(p_fail_addr != NULL);
    DEV_ASSERT(pflash_cmd_sequence != NULL);
#endif
    flash_drv_status_t ret = FTFx_OK;    /* Return code variable */
    uint32_t           offset_addr;      /* Offset address to convert to internal memory address */
    uint32_t           temp;             /* Temporary variable                                   */
    uint32_t           temp_size = size; /* Temporary of size variation */
    uint8_t            idx;

    if ((temp_size & (FEATURE_FLS_PF_CHECK_CMD_ADDRESS_ALIGMENT - 1U)) != 0U)
    {
        ret = FTFx_ERR_SIZE;
    }
    else
    {
        /* Check if the destination is aligned or not */
        {
            offset_addr = p_ssd_config->pflash_base;
            if ((dest >= offset_addr) && (dest < (offset_addr + p_ssd_config->pflash_size)))
            {
                dest -= offset_addr;
            }
            else
            {
                ret       = FTFx_ERR_ACCERR;
                temp_size = 0U;
            }
        }
        while (temp_size > 0U)
        {
            /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register.
             * Write 1 to clear */
            CLEAR_FTFC_FSTAT_ERROR_BITS;

            /* Passing parameter to the command */
            FTFC_FCCOB0 = FTFx_PROGRAM_CHECK;
            FTFC_FCCOB1 = GET_BIT_16_23(dest);
            FTFC_FCCOB2 = GET_BIT_8_15(dest);
            FTFC_FCCOB3 = GET_BIT_0_7(dest);
            FTFC_FCCOB4 = margin_level;

            for (idx = 0U; idx < FEATURE_FLS_PF_CHECK_CMD_ADDRESS_ALIGMENT; idx++)
            {
                temp               = FTFC_BASE + idx + 0x0CU;
                *(uint8_t *)(temp) = p_expecte_data[idx];
            }

            /* Calling flash command sequence function to execute the command */
            ret = pflash_cmd_sequence(p_ssd_config);

            /* Checking for the success of command execution */
            if (FTFx_OK != ret)
            {
                {
                    *p_fail_addr = dest + offset_addr;
                }
                temp_size = FEATURE_FLS_PF_CHECK_CMD_ADDRESS_ALIGMENT;
            }
            /* Update size for next iteration */
            temp_size -= FEATURE_FLS_PF_CHECK_CMD_ADDRESS_ALIGMENT;
            /* Increment the source address by 1 */
            p_expecte_data += FEATURE_FLS_PF_CHECK_CMD_ADDRESS_ALIGMENT;
            /* Update destination address for next iteration */
            dest += FEATURE_FLS_PF_CHECK_CMD_ADDRESS_ALIGMENT;
        }
    }

    return (ret);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flash_check_sum
 * Description   : Performs 32 bit sum of each byte data over a specified Flash
 * memory range without carry which provides rapid method for checking data
 * integrity.
 *
 * Implements    : flash_check_sum_activity
 *END**************************************************************************/
flash_drv_status_t
flash_check_sum(const flash_ssd_config_t *p_ssd_config,
                uint32_t                  dest,
                uint32_t                  size,
                uint32_t                 *p_sum)
{
#ifdef DEV_ERROR_DETECT
    DEV_ASSERT(p_ssd_config != NULL);
    DEV_ASSERT(p_sum != NULL);
#endif
    flash_drv_status_t ret     = FTFx_OK; /* Return code variable           */
    uint32_t           counter = 0U;      /* Counter for callback operation */
    uint32_t           data;              /* Data read from Flash address   */
    uint32_t           end_address;       /* PFlash end address             */
    uint32_t           temp_size = size;  /* Temporary of size variation    */

    /* Calculating Flash end address */
    end_address = dest + temp_size;

    /* Check for valid range of the target addresses */
    if ((dest < p_ssd_config->pflash_base) ||
        (end_address > (p_ssd_config->pflash_base + p_ssd_config->pflash_size)))
    {
        ret       = FTFx_ERR_RANGE;
        temp_size = 0U;
    }
    *p_sum = 0U;
    /* Doing sum operation */
    while (temp_size > 0U)
    {
        data = *(uint8_t *)(dest);
        *p_sum += data;
        dest += 1U;
        temp_size -= 1U;
        ++counter;

        /* Check if need to serve callback function */
        if (counter >= FLASH_CALLBACK_CS)
        {
            /* Serve callback function if counter reaches limitation */
            if (NULL_CALLBACK != p_ssd_config->callback)
            {
                p_ssd_config->callback();
            }
            /* Reset counter */
            counter = 0U;
        }
    }

    return (ret);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flash_program_section
 * Description   : Program the data found in the Section Program Buffer
 * to previously erased locations in the Flash memory. Data is preloaded into
 * the Section Program Buffer by writing to the acceleration Ram and FlexRam
 * while it is set to function as a RAM. The Section Program Buffer is limited
 * to the value of FlexRam divides by a ratio. Refer to the associate reference
 * manual to get correct value of this ratio.
 *
 * Implements    : flash_program_section_activity
 *END**************************************************************************/
flash_drv_status_t
flash_program_section(const flash_ssd_config_t *p_ssd_config,
                      uint32_t                  dest,
                      uint16_t                  number,
                      flash_command_sequence_t  pflash_cmd_sequence)
{
#ifdef DEV_ERROR_DETECT
    DEV_ASSERT(p_ssd_config != NULL);
    DEV_ASSERT(pflash_cmd_sequence != NULL);
#endif
    flash_drv_status_t ret = FTFx_OK; /* Return code variable */
    uint32_t           temp;

    temp = p_ssd_config->pflash_base;
    if ((dest >= temp) && (dest < (temp + p_ssd_config->pflash_size)))
    {
#ifdef DEV_ERROR_DETECT
        DEV_ASSERT((dest % FEATURE_FLS_PF_SECTION_CMD_ADDRESS_ALIGMENT) == 0U);
#endif
        dest -= temp;
    }
    else
    {
        ret = FTFx_ERR_ACCERR;
    }

    if (ret == FTFx_OK)
    {
        /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register.
         * Write 1 to clear
         */
        CLEAR_FTFC_FSTAT_ERROR_BITS;
        /* Passing parameter to command */
        FTFC_FCCOB0 = FTFx_PROGRAM_SECTION;
        FTFC_FCCOB1 = GET_BIT_16_23(dest);
        FTFC_FCCOB2 = GET_BIT_8_15(dest);
        FTFC_FCCOB3 = GET_BIT_0_7(dest);
        FTFC_FCCOB4 = GET_BIT_8_15(number);
        FTFC_FCCOB5 = GET_BIT_0_7(number);

        /* Calling flash command sequence function to execute the command */
        ret = pflash_cmd_sequence(p_ssd_config);
    }

    return (ret);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flash_erase_block
 * Description   : Erases all addresses in an individual P-Flash block. For the
 *                  derivatives including multiply logical P-Flash blocks, this
 *                  API erases a single block in a single call.
 *
 * Implements    : flash_erase_block_activity
 *END**************************************************************************/
flash_drv_status_t
flash_erase_block(const flash_ssd_config_t *p_ssd_config,
                  uint32_t                  dest,
                  flash_command_sequence_t  pflash_cmd_sequence)
{
#ifdef DEV_ERROR_DETECT
    DEV_ASSERT(p_ssd_config != NULL);
    DEV_ASSERT(pflash_cmd_sequence != NULL);
#endif
    flash_drv_status_t ret = FTFx_OK; /* Return code variable */
    uint32_t           temp;          /* Temporary variable   */

    /* Check if the destination is aligned or not */
    temp = p_ssd_config->pflash_base;
    if ((dest >= temp) && (dest < (temp + p_ssd_config->pflash_size)))
    {
#ifdef DEV_ERROR_DETECT
        DEV_ASSERT((dest % FEATURE_FLS_PF_BLOCK_CMD_ADDRESS_ALIGMENT) == 0U);
#endif
        dest -= temp;
    }
    else
    {
        ret = FTFx_ERR_ACCERR;
    }

    if (FTFx_OK == ret)
    {
        /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write
         * 1 to clear */
        CLEAR_FTFC_FSTAT_ERROR_BITS;

        /* Passing parameter to the command */
        FTFC_FCCOB0 = FTFx_ERASE_BLOCK;
        FTFC_FCCOB1 = GET_BIT_16_23(dest);
        FTFC_FCCOB2 = GET_BIT_8_15(dest);
        FTFC_FCCOB3 = GET_BIT_0_7(dest);

        /* Calling flash command sequence function to execute the command */
        ret = pflash_cmd_sequence(p_ssd_config);
    }

    return (ret);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flash_verify_block
 * Description   : Checks to see if an entire P-Flash block has been erased to
 *                  the specified margin level. For the derivatives including multiply
 *                  logical P-Flash blocks, this API erases a single block in a single call.
 *
 * Implements    : flash_verify_block_activity
 *END**************************************************************************/
flash_drv_status_t
flash_verify_block(const flash_ssd_config_t *p_ssd_config,
                   uint32_t                  dest,
                   uint8_t                   margin_level,
                   flash_command_sequence_t  pflash_cmd_sequence)
{
#ifdef DEV_ERROR_DETECT
    DEV_ASSERT(p_ssd_config != NULL);
    DEV_ASSERT(pflash_cmd_sequence != NULL);
#endif
    flash_drv_status_t ret = FTFx_OK; /* Return code variable */
    uint32_t           temp;

    /* Check if the destination is aligned or not */
    temp = p_ssd_config->pflash_base;
    if ((dest >= temp) && (dest < (temp + p_ssd_config->pflash_size)))
    {
        dest -= temp;
    }
    else
    {
        ret = FTFx_ERR_ACCERR;
    }

    if (FTFx_OK == ret)
    {
        /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write
         * 1 to clear */
        CLEAR_FTFC_FSTAT_ERROR_BITS;

        /* Passing parameter to the command */
        FTFC_FCCOB0 = FTFx_VERIFY_BLOCK;
        FTFC_FCCOB1 = GET_BIT_16_23(dest);
        FTFC_FCCOB2 = GET_BIT_8_15(dest);
        FTFC_FCCOB3 = GET_BIT_0_7(dest);
        FTFC_FCCOB4 = margin_level;

        /* Calling flash command sequence function to execute the command */
        ret = pflash_cmd_sequence(p_ssd_config);
    }

    return (ret);
}

/*** end of file ***/
