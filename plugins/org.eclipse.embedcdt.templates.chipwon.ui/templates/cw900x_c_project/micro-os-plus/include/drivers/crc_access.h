/**
 * @file crc_access.h
 * @brief Static inline function for the CRC module.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef CRC_ACCESS_H
#define CRC_ACCESS_H

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
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/
/**
 * @brief Gets the current CRC result
 *
 * This function gets the current CRC result from the data register
 *
 * @param[in] p_base The CRC peripheral base address
 * @return Returns the current CRC result
 */
static inline uint32_t
crc_get_data_reg(const CRC_t *const p_base)
{
    return p_base->DATAu.DATA;
}

/**
 * @brief Sets the 32 bits of CRC data register
 *
 * This function sets the 32 bits of CRC data register
 *
 * @param[in] p_base The CRC peripheral base address
 * @param[in] value New data for CRC computation
 */
static inline void
crc_set_data_reg(CRC_t *const p_base, uint32_t value)
{
    p_base->DATAu.DATA = value;
}

/**
 * @brief Gets the upper 16 bits of the current CRC result
 *
 * This function gets the upper 16 bits of the current CRC result from the
 * data register
 *
 * @param[in] p_base The CRC peripheral base address
 * @return Returns the upper 16 bits of the current CRC result
 */
static inline uint16_t
crc_get_data_hreg(const CRC_t *const p_base)
{
    return p_base->DATAu.DATA_16.H;
}

/**
 * @brief Gets the lower 16 bits of the current CRC result
 *
 * This function gets the lower 16 bits of the current CRC result from the
 * data register
 *
 * @param[in] p_base The CRC peripheral base address
 * @return Returns the lower 16 bits of the current CRC result
 */
static inline uint16_t
crc_get_data_lreg(const CRC_t *const p_base)
{
    return p_base->DATAu.DATA_16.L;
}

/**
 * @brief Sets the lower 16 bits of CRC data register
 *
 * This function sets the lower 16 bits of CRC data register
 *
 * @param[in] p_base The CRC peripheral base address
 * @param[in] value New data for CRC computation
 */
static inline void
crc_set_data_lreg(CRC_t *const p_base, uint16_t value)
{
    p_base->DATAu.DATA_16.L = value;
}

/**
 * @brief Sets the Low Lower Byte - LL
 *
 * This function sets the Low Lower Byte - LL of CRC data register
 *
 * @param[in] p_base The CRC peripheral base address
 * @param[in] value New data for CRC computation
 */
static inline void
crc_set_data_llreg(CRC_t *const p_base, uint8_t value)
{
    p_base->DATAu.DATA_8.LL = value;
}

/**
 * @brief Gets the polynomial register value
 *
 * This function gets the polynomial register value
 *
 * @param[in] p_base The CRC peripheral base address
 * @return Returns the polynomial register value
 */
static inline uint32_t
crc_get_poly_reg(const CRC_t *const p_base)
{
    return p_base->GPOLY;
}

/**
 * @brief Sets the polynomial register
 *
 * This function sets the polynomial register
 *
 * @param[in] p_base The CRC peripheral base address
 * @param[in] value Polynomial value
 */
static inline void
crc_set_poly_reg(CRC_t *const p_base, uint32_t value)
{
    p_base->GPOLY = value;
}

/**
 * @brief Sets the CRC_DATA register mode
 *
 * This function sets the CRC_DATA register mode
 *
 * @param[in] p_base The CRC peripheral base address
 * @param[in] b_enable Enable CRC data register to use for seed value
 *            -true: use CRC data register for seed values.
 *            -false: use CRC data register for data values.
 */
static inline void
crc_set_seed_or_data_mode(CRC_t *const p_base, bool b_enable)
{
    uint32_t ctrl_temp = p_base->CTRL;

    ctrl_temp &= ~(CRC_CTRL_WAS_MASK);
    ctrl_temp |= CRC_CTRL_WAS(b_enable ? 1UL : 0UL);
    p_base->CTRL = ctrl_temp;
}

/**
 * @brief Gets complement read of CRC data register
 *
 * This function gets complement read of CRC data register.
 * Some CRC protocols require the final checksum to be XORed with 0xFFFFFFFF
 * or 0xFFFF. Complement mode enables "on the fly" complementing of read data
 *
 * @param[in] p_base The CRC peripheral base address
 * @return Complement read
 *         -true: Invert or complement the read value of the CRC Data register.
 *         -false: No XOR on reading.
 */
static inline bool
crc_get_fxor_mode(const CRC_t *const p_base)
{
    return ((p_base->CTRL & CRC_CTRL_FXOR_MASK) >> CRC_CTRL_FXOR_SHIFT) != 0U;
}

/**
 * @brief Sets complement read of CRC data register
 *
 * This function sets complement read of CRC data register.
 * Some CRC protocols require the final checksum to be XORed with 0xFFFFFFFF
 * or 0xFFFF. Complement mode enables "on the fly" complementing of read data
 *
 * @param[in] p_base The CRC peripheral base address
 * @param[in] b_enable Enable or disable complementing of read data
 */
static inline void
crc_set_fxor_mode(CRC_t *const p_base, bool b_enable)
{
    uint32_t ctrl_temp = p_base->CTRL;

    ctrl_temp &= ~(CRC_CTRL_FXOR_MASK);
    ctrl_temp |= CRC_CTRL_FXOR(b_enable ? 1UL : 0UL);
    p_base->CTRL = ctrl_temp;
}

/**
 * @brief Gets the CRC protocol width
 *
 * This function gets the CRC protocol width
 *
 * @param[in] p_base The CRC peripheral base address
 * @return CRC protocol width
 *         - CRC_BITS_16: 16-bit CRC protocol.
 *         - CRC_BITS_32: 32-bit CRC protocol.
 */
static inline crc_bit_width_t
crc_get_protocol_width(const CRC_t *const p_base)
{
    crc_bit_width_t ret_val = CRC_BITS_16;

    if (((p_base->CTRL & CRC_CTRL_TCRC_MASK) >> CRC_CTRL_TCRC_SHIFT) != 0U)
    {
        ret_val = CRC_BITS_32;
    }
    else if (((p_base->CTRL & 0x100) >> 8) != 0U)
    {
        ret_val = CRC_BITS_8;
    }
    else if (((p_base->CTRL & 0x20) >> 5) != 0U)
    {
        ret_val = CRC_BITS_5;
    }
    return ret_val;
}

/**
 * @brief Sets the CRC protocol width
 *
 * This function sets the CRC protocol width
 *
 * @param[in] p_base The CRC peripheral base address
 * @param[in] width The CRC protocol width
 *            - CRC_BITS_16: 16-bit CRC protocol.
 *            - CRC_BITS_32: 32-bit CRC protocol.
 */
static inline void
crc_set_protocol_width(CRC_t *const p_base, crc_bit_width_t width)
{
    uint32_t ctrl_temp = p_base->CTRL;
    ctrl_temp &= ~(CRC_CTRL_TCRC_MASK | CRC_CTRL_CRC8_MASK | CRC_CTRL_CRC5_MASK);
    if (CRC_BITS_32 == width)
    {
        ctrl_temp |= CRC_CTRL_TCRC(width);
    }
    else if (CRC_BITS_8 == width)
    {
        ctrl_temp |= CRC_CTRL_CRC8(true);
    }
    else if (CRC_BITS_5 == width)
    {
        ctrl_temp |= CRC_CTRL_CRC5(true);
    }
    p_base->CTRL = ctrl_temp;
}

/**
 * @brief Gets the CRC transpose type for writes
 *
 * This function gets the CRC transpose type for writes
 *
 * @param[in] p_base The CRC peripheral base address
 * @return CRC input transpose type for writes
 */
static inline crc_transpose_t
crc_get_write_transpose(const CRC_t *const p_base)
{
    crc_transpose_t type;
    uint32_t        temp = (p_base->CTRL & CRC_CTRL_TOT_MASK) >> CRC_CTRL_TOT_SHIFT;

    switch (temp)
    {
        case 1U:
        {
            type = CRC_TRANSPOSE_BITS;
        }
        break;
        case 2U:
        {
            type = CRC_TRANSPOSE_BITS_AND_BYTES;
        }
        break;
        case 3U:
        {
            type = CRC_TRANSPOSE_BYTES;
        }
        break;
        default:
        {
            type = CRC_TRANSPOSE_NONE;
        }
        break;
    }
    return type;
}

/**
 * @brief Sets the CRC transpose type for writes
 *
 * This function sets the CRC transpose type for writes
 *
 * @param[in] p_base The CRC peripheral base address
 * @param[in] transp The CRC input transpose type
 */
static inline void
crc_set_write_transpose(CRC_t *const p_base, crc_transpose_t transp)
{
    uint32_t ctrl_temp = p_base->CTRL;

    ctrl_temp &= ~(CRC_CTRL_TOT_MASK);
    ctrl_temp |= CRC_CTRL_TOT(transp);
    p_base->CTRL = ctrl_temp;
}

/**
 * @brief Gets the CRC transpose type for reads
 *
 * This function gets the CRC transpose type for reads
 *
 * @param[in] p_base The CRC peripheral base address
 * @return CRC output transpose type
 */
static inline crc_transpose_t
crc_get_read_transpose(const CRC_t *const p_base)
{
    crc_transpose_t type;
    uint32_t        temp = (p_base->CTRL & CRC_CTRL_TOTR_MASK) >> CRC_CTRL_TOTR_SHIFT;

    switch (temp)
    {
        case 1U:
            type = CRC_TRANSPOSE_BITS;
            break;
        case 2U:
            type = CRC_TRANSPOSE_BITS_AND_BYTES;
            break;
        case 3U:
            type = CRC_TRANSPOSE_BYTES;
            break;
        default:
            type = CRC_TRANSPOSE_NONE;
            break;
    }
    return type;
}

/**
 * @brief Sets the CRC transpose type for reads
 *
 * This function sets the CRC transpose type for reads
 *
 * @param[in] p_base The CRC peripheral base address
 * @param[in] transp The CRC output transpose type
 */
static inline void
crc_set_read_transpose(CRC_t *const p_base, crc_transpose_t transp)
{
    uint32_t ctrl_temp = p_base->CTRL;

    ctrl_temp &= ~(CRC_CTRL_TOTR_MASK);
    ctrl_temp |= CRC_CTRL_TOTR(transp);
    p_base->CTRL = ctrl_temp;
}

#ifdef __cplusplus
}
#endif

#endif /* CRC_ACCESS_H */

/*** end of file ***/
