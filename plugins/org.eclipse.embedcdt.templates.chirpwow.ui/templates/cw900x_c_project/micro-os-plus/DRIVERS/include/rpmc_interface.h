/**
 * @file rpmc_interface.h
 * @brief An implementation of runtime functions for RPMC client service.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef RPMC_INTERFACE_H
#define RPMC_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "rpmc_spec.h"
#include "hsspi_driver.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/* RPMC supported maximum number of set of security key */
#define RPMC_MAX_NUM_SET_SE_KEY (2u)

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
typedef enum
{
    RPMC_READ_STATUS_DATA = 0u,
    RPMC_WRITE_ROOT_KEY_REG,
    RPMC_UPDATE_HMAC_KEY_REG,
    RPMC_INCR_MONOTONIC_CNTR,
    RPMC_REQ_MONOTONIC_CNTR,
    RPMC_TAIL_END
} rpmc_support_cmd_idx_t;

typedef enum
{
    RPMC_UNINITINALIZED_STATE = 0u,   /**<  */
    RPMC_INITINALIZED_STATE,          /**<  */
    RPMC_HMAC_KEY_INITINALIZED_STATE, /**<  */
} rpmc_internal_state_t;

typedef struct _rpmc_internal_register_t
{
    rpmc_internal_state_t state_at_boot[RPMC_MAX_NUM_SET_SE_KEY];
    uint8_t               root_key[RPMC_MAX_NUM_SET_SE_KEY][RPMC_SHA256_MSG_DIGEST_SIZE]
        __attribute__((aligned(4)));
    uint8_t hmac_key[RPMC_MAX_NUM_SET_SE_KEY][RPMC_SHA256_MSG_DIGEST_SIZE]
        __attribute__((aligned(4)));
    uint32_t monotonic_counter[RPMC_MAX_NUM_SET_SE_KEY] __attribute__((aligned(4)));
} rpmc_internal_register_t;

/*******************************************************************************
 * Exported variables
 ******************************************************************************/
extern rpmc_internal_register_t g_rpmc_intl_reg;

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Initial routine for RPMC client service.
 */
void rpmc_init(void);

/**
 * @brief Deinitial routine for RPMC client service.
 */
void rpmc_deinit(void);

bool rpmc_check_transaction_new_arrival(void);

rpmc_support_cmd_idx_t rpmc_get_transaction_info(void);

void rpmc_update_status_on_demand(uint8_t status);

rpmc_txn_buf_t *rpmc_get_transaction_buf_ptr(void);

rpmc_resp_buf_t *rpmc_get_response_buf_ptr(void);

void rpmc_complete_response_buf(void);

#ifdef __cplusplus
}
#endif

#endif /* RPMC_INTERFACE_H */

/*** end of file ***/
