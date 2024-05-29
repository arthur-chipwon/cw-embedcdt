/**
 * @file rpmc_spec.h
 * @brief Defined macros from External Architecture Specification Rev. 0.7 for RPMC.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef RPMC_SPEC_H
#define RPMC_SPEC_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
// #define _TEST_VARIED_OPCODE_

// #define _PUFS_VERIF_
// #define _RPMC_REQUEST_CNTR_ONLY_
#ifndef _PUFS_VERIF_
// #define _RPMC_ROOT_KEY_MFG_MADE_
#ifndef _RPMC_ROOT_KEY_MFG_MADE_
// #define _RPMC_ROOT_KEY_EXIST_
#endif /* _RPMC_ROOT_KEY_MFG_MADE_ */
#endif /* _PUFS_VERIF_ */

/** SHA-2(v2, 2001 A.D.) SHA-256, message digest bit length 256 bits, byte length 32 bytes */
#define RPMC_SHA256_MSG_DIGEST_SIZE (32u)

#ifndef _TEST_VARIED_OPCODE_
#define RPMC_READ_OPCODE  (0x96u)
#define RPMC_WRITE_OPCODE (0x9Bu)
#else /* _TEST_VARIED_OPCODE_ */
#define RPMC_READ_OPCODE  (0x95u)
#define RPMC_WRITE_OPCODE (0x9Au)
#endif /* _TEST_VARIED_OPCODE_ */

/** Command Type */
#define RPMC_CT_WRITE_ROOT_KEY_REG  (0x00u)
#define RPMC_CT_UPDATE_HMAC_KEY_REG (0x01u)
#define RPMC_CT_INCR_MONOTONIC_CNTR (0x02u)
#define RPMC_CT_REQ_MONOTONIC_CNTR  (0x03u)
#define RPMC_CT_UNSPECIFIED         (0x04u)
#define RPMC_CT_RESERVED            (0xFFu)

#define RPMC_STS_CLEAR                 (0x00u)
#define RPMC_STS_BUSY                  (0x01u)
#define RPMC_STS_ROOT_KEY_ERROR        (0x02u)
#define RPMC_STS_UPDATE_HMAC_KEY_ERROR (0x02u)
#define RPMC_STS_CNTR_OUT_OF_RANGE     (0x04u)
#define RPMC_STS_NO_HMAC_KEY_ERROR     (0x08u)
#define RPMC_STS_CNTR_DATA_ERROR       (0x10u)
#define RPMC_STS_SUCCESS               (0x80u)

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
#pragma pack(1)
/**
 * @brief RPMC Command Header.
 * @param items 4.
 * @param size 4(4h).
 */
typedef struct _rpmc_cmd_header_t
{
    uint8_t opcode;       /**< Offset: 0(0h). */
    uint8_t cmd_type;     /**< Offset: 1(1h). */
    uint8_t counter_addr; /**< Offset: 2(2h). */
    uint8_t reserved;     /**< Offset: 3(3h). */
} rpmc_cmd_header_t;

/**
 * @brief RPMC Write Root Key Register. Lead member has skipped first two bytes data.
 * @param items 4.
 * @param size 62(3Eh).
 */
typedef struct _rpmc_write_root_key_t
{
    uint8_t counter_addr;             /**< Offset: 0(0h). */
    uint8_t reserved;                 /**< Offset: 1(1h). */
    uint8_t root_key[32u];            /**< Offset: 2(2h). */
    uint8_t truncated_signature[28u]; /**< Offset: 34(22h). */
} rpmc_write_root_key_t;

/**
 * @brief RPMC Update HMAC Key Register. Lead member has skipped first two bytes data.
 * @param items 4.
 * @param size 38(26h).
 */
typedef struct _rpmc_update_hmac_key_t
{
    uint8_t counter_addr;   /**< Offset: 0(0h). */
    uint8_t reserved;       /**< Offset: 1(1h). */
    uint8_t key_data[4u];   /**< Offset: 2(2h). */
    uint8_t signature[32u]; /**< Offset: 6(6h). */
} rpmc_update_hmac_key_t;

/**
 * @brief RPMC Increment Monotonic Counter. Lead member has skipped first two bytes data.
 * @param items 4.
 * @param size 38(26h).
 */
typedef struct _rpmc_incr_monotonic_cntr_t
{
    uint8_t counter_addr;     /**< Offset: 0(0h). */
    uint8_t reserved;         /**< Offset: 1(1h). */
    uint8_t counter_data[4u]; /**< Offset: 2(2h). */
    uint8_t signature[32u];   /**< Offset: 6(6h). */
} rpmc_incr_monotonic_cntr_t;

/**
 * @brief RPMC Request Monotonic Counter. Lead member has skipped first two bytes data.
 * @param items 4.
 * @param size 46(2Eh).
 */
typedef struct _rpmc_req_monotonic_cntr_t
{
    uint8_t counter_addr;   /**< Offset: 0(0h). */
    uint8_t reserved;       /**< Offset: 1(1h). */
    uint8_t tag[12u];       /**< Offset: 2(2h). */
    uint8_t signature[32u]; /**< Offset: 14(Eh). */
} rpmc_req_monotonic_cntr_t;

/**
 * @brief RPMC Read Response/Status Data. To acknowledge host command then response the payload
 * form.
 * @param items 4.
 * @param size 49(31h).
 */
typedef struct _rpmc_read_status_data_t
{
    uint8_t extended_status;  /**< Offset: 0(0h). */
    uint8_t tag[12u];         /**< Offset: 1(1h). */
    uint8_t counter_data[4u]; /**< Offset: 13(Dh). */
    uint8_t signature[32u];   /**< Offset: 17(11h). */
} rpmc_read_status_data_t;
#pragma pack()

typedef union _rpmc_txn_buf_t
{
    uint8_t                    data[64u];       /**< Offset: 0(0h). */
    uint8_t                    counter_addr;    /**< Offset: 0(0h). */
    rpmc_write_root_key_t      write_root_key;  /**< Offset: 0(0h). */
    rpmc_update_hmac_key_t     update_hmac_key; /**< Offset: 0(0h). */
    rpmc_incr_monotonic_cntr_t incr_monot_cntr; /**< Offset: 0(0h). */
    rpmc_req_monotonic_cntr_t  req_monot_cntr;  /**< Offset: 0(0h). */
} rpmc_txn_buf_t;

typedef union _rpmc_resp_buf_t
{
    uint8_t                 data[64u];        /**< Offset: 0(0h). */
    rpmc_read_status_data_t read_status_data; /**< Offset: 0(0h). */
} rpmc_resp_buf_t;

/*******************************************************************************
 * Exported variables
 ******************************************************************************/

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* RPMC_SPEC_H */

/*** end of file ***/
