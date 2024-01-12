/**
 * @file crc_driver.c
 * @brief This file provides access to the crc module.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "crc_driver.h"
#include "crc_access.h"
#include "device_registers.h"

/*******************************************************************************
 * Private macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/* Initial checksum */
#define CRC_INITIAL_SEED (0U)

/*******************************************************************************
 * Static variables
 ******************************************************************************/
/** @brief Table of base addresses for CRC instances. */
static CRC_t *const sp_crc_base[] = CRC_BASE_PTRS;

/*******************************************************************************
 * Global functions
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : crc_init_set
 * Description   : This function initializes the module to default configuration
 * (Initial checksum: 0U,
 * Default polynomial: 0x1021U,
 * Type of read transpose: CRC_TRANSPOSE_NONE,
 * Type of write transpose: CRC_TRANSPOSE_NONE,
 * No complement of checksum read, 32-bit CRC).
 *
 *END**************************************************************************/
void
crc_init_set(CRC_t *const p_base)
{
    /* Set CRC mode to 32-bit */
    crc_set_protocol_width(p_base, CRC_BITS_32);

    /* Set read/write transpose and complement checksum to none */
    crc_set_write_transpose(p_base, CRC_TRANSPOSE_NONE);
    crc_set_read_transpose(p_base, CRC_TRANSPOSE_NONE);
    crc_set_fxor_mode(p_base, false);

    /* Write polynomial to 0x1021U */
    crc_set_poly_reg(p_base, FEATURE_CRC_DEFAULT_POLYNOMIAL);

    /* Write seed to zero */
    crc_set_seed_or_data_mode(p_base, true);
    crc_set_data_reg(p_base, CRC_INITIAL_SEED);
    crc_set_seed_or_data_mode(p_base, false);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : crc_set_seed_reg
 * Description   : This function set seed value for new of CRC module computation
 *
 *END**************************************************************************/
void
crc_set_seed_reg(CRC_t *const p_base, uint32_t seed_value)
{
    crc_set_seed_or_data_mode(p_base, true);
    /* Write a seed - initial checksum */
    crc_set_data_reg(p_base, seed_value);
    crc_set_seed_or_data_mode(p_base, false);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : crc_get_result_data
 * Description   : This function returns the current result of the CRC calculation.
 *
 *END**************************************************************************/
uint32_t
crc_get_result_data(const CRC_t *const p_base)
{
    crc_bit_width_t width = crc_get_protocol_width(p_base);
    crc_transpose_t transpose;
    uint32_t        result;

    if (width == CRC_BITS_16)
    {
        transpose = crc_get_read_transpose(p_base);
        if ((transpose == CRC_TRANSPOSE_BITS_AND_BYTES) || (transpose == CRC_TRANSPOSE_BYTES))
        {
            /* Returns upper 16 bits of CRC because of transposition in 16-bit
             * mode */
            result = (uint32_t)crc_get_data_hreg(p_base);
        }
        else
        {
            result = (uint32_t)crc_get_data_lreg(p_base);
        }
    }
    else
    {
        result = crc_get_data_reg(p_base);
    }
    return result;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : crc_init
 * Description   : This function initializes CRC driver based on user
 * configuration input. The user must make sure that the clock is enabled.
 *
 * Implements    : crc_init_activity
 *END**************************************************************************/
status_t
crc_init(uint32_t instance, const crc_user_config_t *p_user_config_ptr)
{
    DEV_ASSERT(instance < CRC_INSTANCE_COUNT);
    DEV_ASSERT(p_user_config_ptr != NULL);
    CRC_t   *p_base     = sp_crc_base[instance];
    status_t ret_status = STATUS_SUCCESS;

    /* Set the default configuration */
    crc_init_set(p_base);
    /* Set the CRC configuration */
    ret_status = crc_set_config(instance, p_user_config_ptr);

    return ret_status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : crc_deinit
 * Description   : This function sets the default configuration.
 *
 * Implements    : crc_deinit_activity
 *END**************************************************************************/
status_t
crc_deinit(uint32_t instance)
{
    DEV_ASSERT(instance < CRC_INSTANCE_COUNT);
    CRC_t *p_base = sp_crc_base[instance];

    /* Set the default configuration */
    crc_init_set(p_base);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : crc_get_crc32
 * Description   : This function appends 32-bit data to the current CRC
 * calculation and returns new result. If the b_new_seed is true, seed set and
 * result are calculated from the seed new value (new CRC calculation).
 *
 * Implements    : crc_get_crc32_activity
 *END**************************************************************************/
uint32_t
crc_get_crc32(uint32_t instance, uint32_t data, bool b_new_seed, uint32_t seed)
{
    DEV_ASSERT(instance < CRC_INSTANCE_COUNT);
    CRC_t *p_base = sp_crc_base[instance];

    /* If b_new_seed is true then write a seed to initial checksum */
    if (b_new_seed)
    {
        /* Write a seed - initial checksum */
        crc_set_seed_reg(p_base, seed);
    }

    /* Write 32-bit data */
    crc_set_data_reg(p_base, data);

    /* Result of the CRC calculation */
    return crc_get_result_data(p_base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : crc_get_crc16
 * Description   : This function appends 16-bit data to the current CRC
 * calculation and returns new result. If the b_new_seed is true, seed set and
 * result are calculated from the seed new value (new CRC calculation).
 *
 * Implements    : crc_get_crc16_activity
 *END**************************************************************************/
uint32_t
crc_get_crc16(uint32_t instance, uint16_t data, bool b_new_seed, uint32_t seed)
{
    DEV_ASSERT(instance < CRC_INSTANCE_COUNT);
    CRC_t *p_base = sp_crc_base[instance];

    /* If b_new_seed is true then write a seed to initial checksum */
    if (b_new_seed)
    {
        /* Write a seed - initial checksum */
        crc_set_seed_reg(p_base, seed);
    }
    /* Write 16-bit data */
    crc_set_data_lreg(p_base, data);

    /* Result of the CRC calculation */
    return crc_get_result_data(p_base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : crc_get_crc8
 * Description   : This function appends 8-bit data to the current CRC
 * calculation and returns new result. If the b_new_seed is true, seed set and
 * result are calculated from the seed new value (new CRC calculation).
 *
 * Implements    : crc_get_crc8_activity
 *END**************************************************************************/
uint32_t
crc_get_crc8(uint32_t instance, uint8_t data, bool b_new_seed, uint32_t seed)
{
    DEV_ASSERT(instance < CRC_INSTANCE_COUNT);
    CRC_t *p_base = sp_crc_base[instance];

    /* If b_new_seed is true then write a seed to initial checksum */
    if (b_new_seed)
    {
        /* Write a seed - initial checksum */
        crc_set_seed_reg(p_base, seed);
    }
    /* Write 8-bit data */
    crc_set_data_llreg(p_base, data);

    /* Result of the CRC calculation */
    return crc_get_result_data(p_base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : crc_write_data
 * Description   : This function appends a block of bytes to the current CRC
 * calculation.
 *
 * Implements    : crc_write_data_activity
 *END**************************************************************************/
void
crc_write_data(uint32_t instance, const uint8_t *p_data, uint32_t data_size)
{
    DEV_ASSERT(instance < CRC_INSTANCE_COUNT);
    DEV_ASSERT(p_data != NULL);
    uint32_t idx;
    CRC_t   *p_base = sp_crc_base[instance];

    /* 8-bit writes till end of p_data buffer */
    for (idx = 0U; idx < data_size; idx++)
    {
        crc_set_data_llreg(p_base, p_data[idx]);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : crc_get_result
 * Description   : This function returns the current result of the CRC calculation.
 *
 * Implements    : crc_get_result_activity
 *END**************************************************************************/
uint32_t
crc_get_result(uint32_t instance)
{
    DEV_ASSERT(instance < CRC_INSTANCE_COUNT);
    const CRC_t *p_base = sp_crc_base[instance];

    /* Result of the CRC calculation */
    return crc_get_result_data(p_base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : crc_set_config
 * Description   : This function configures the CRC module from a user
 * configuration structure.
 *
 * Implements    : crc_set_config_activity
 *END**************************************************************************/
status_t
crc_set_config(uint32_t instance, const crc_user_config_t *p_user_config_ptr)
{
    DEV_ASSERT(instance < CRC_INSTANCE_COUNT);
    DEV_ASSERT(p_user_config_ptr != NULL);
    CRC_t *p_base = sp_crc_base[instance];

    /* Set CRC mode */
    crc_set_protocol_width(p_base, p_user_config_ptr->crc_width);
    /* Set transposes options */
    crc_set_read_transpose(p_base, p_user_config_ptr->read_transpose);
    /* Set CRC polynomial */
    crc_set_poly_reg(p_base, p_user_config_ptr->polynomial);
    /* Set writes transposes */
    crc_set_write_transpose(p_base, p_user_config_ptr->write_transpose);
    /* Sets complement or inversion checksum */
    crc_set_fxor_mode(p_base, p_user_config_ptr->b_complement_checksum);
    /* Write a seed - initial checksum */
    crc_set_seed_reg(p_base, p_user_config_ptr->seed);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : crc_get_config
 * Description   : This function Get configures of the CRC module currently
 *
 * Implements    : crc_get_config_activity
 *END**************************************************************************/
status_t
crc_get_config(uint32_t instance, crc_user_config_t *const p_user_config_ptr)
{
    DEV_ASSERT(instance < CRC_INSTANCE_COUNT);
    DEV_ASSERT(p_user_config_ptr != NULL);
    const CRC_t *const p_base = sp_crc_base[instance];

    /* Gets CRC mode */
    p_user_config_ptr->crc_width = crc_get_protocol_width(p_base);
    /* Gets transposes and complement options */
    p_user_config_ptr->read_transpose = crc_get_read_transpose(p_base);
    /* Get a polynomial */
    p_user_config_ptr->polynomial = crc_get_poly_reg(p_base);
    /* Gets transposes options */
    p_user_config_ptr->write_transpose = crc_get_write_transpose(p_base);
    /* Gets complement or inversion checksum */
    p_user_config_ptr->b_complement_checksum = crc_get_fxor_mode(p_base);
    /* Get a seed - initial checksum */
    p_user_config_ptr->seed = crc_get_data_reg(p_base);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : crc_get_default_config
 * Description   : This function Get default configures the CRC module for user
 * configuration structure
 *
 * Implements    : crc_get_default_config_activity
 *END**************************************************************************/
status_t
crc_get_default_config(crc_user_config_t *const p_user_config_ptr)
{
    DEV_ASSERT(p_user_config_ptr != NULL);

    /* Gets CRC mode default is 16 bit */
    p_user_config_ptr->crc_width = FEATURE_CRC_DEFAULT_WIDTH;
    /* Gets default a polynomial default is reset value */
    p_user_config_ptr->polynomial = FEATURE_CRC_DEFAULT_POLYNOMIAL;
    /* Gets default read transposes none */
    p_user_config_ptr->read_transpose = FEATURE_CRC_DEFAULT_READ_TRANSPOSE;
    /* Gets default write transpose none */
    p_user_config_ptr->write_transpose = FEATURE_CRC_DEFAULT_WRITE_TRANSPOSE;
    /* Gets default no complement or inversion checksum */
    p_user_config_ptr->b_complement_checksum = false;
    /* Gets default a seed - initial checksum */
    p_user_config_ptr->seed = FEATURE_CRC_DEFAULT_SEED;

    return STATUS_SUCCESS;
}

/*** end of file ***/
