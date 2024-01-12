/**
 * @file tpm_interface.h
 * @brief An implementation of runtime functions for TPM client service.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef TPM_INTERFACE_H
#define TPM_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "hsspi_driver.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
typedef struct _T_Status
{
    volatile bool slave_busy;
    volatile bool rx_wr_fifo;
    volatile bool rx_rd_ok;
    volatile bool rx_rd_start;
    volatile bool rx_wr_ok;
} T_Status;

typedef enum
{
    Write = 0,
    Read
} cmd_rw;

typedef struct _T_Last_CMD
{
    volatile uint32_t addr;
    volatile cmd_rw   rw;
    volatile uint8_t  length;
} T_Last_CMD;

typedef struct _tpm_state_t
{
    HSSPI_t             *p_base;
    const IRQn_t        *p_irq_id;
    const clock_names_t *p_clk_name;
    T_Status             irq_status;
    T_Last_CMD           last_cmd;
} tpm_state_t;

/*******************************************************************************
 * Exported variables
 ******************************************************************************/
extern tpm_state_t g_tpm_state;

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Initial routine for TPM client service.
 */
void tpm_init(void);

/**
 * @brief Deinitial routine for TPM client service.
 */
void tpm_deinit(void);

bool get_tpm_wr_cmd_flag(void);

void clear_tpm_wr_cmd_flag(void);

bool get_tpm_rd_cmd_flag(void);

void clear_tpm_rd_cmd_flag(void);

bool get_tpm_rd_cmd_ok_flag(void);

void clear_tpm_rd_cmd_ok_flag(void);

bool get_tpm_wr_fifo_flag(void);

void clear_tpm_wr_fifo_flag(void);

bool get_tpm_busy_flag(void);

uint32_t get_tpm_last_addr_rec(void);

bool get_tpm_last_cmd_is_read(void);

uint8_t get_tpm_last_len_rec(void);

void set_tpm_add_1_wait(void);

void set_tpm_locality(tpm_loc_num_sel_t loc_sel);

uint32_t get_tpm_did_vid_reg(void);

void set_tpm_did_vid_reg(uint32_t reg_value);

void set_tpm_vid(uint16_t vid);

void set_tpm_did(uint16_t did);

uint16_t get_tpm_vid(void);

uint16_t get_tpm_did(void);

uint32_t get_tpm_sts_reg(void);

void set_tpm_sts_reg(uint32_t reg_value);

bool get_tpm_sts_responseretry(void);

void set_tpm_sts_responseretry(bool b_enable);

bool get_tpm_sts_selftestdone(void);

void set_tpm_sts_selftestdone(bool b_enable);

bool get_tpm_sts_expect(void);

void set_tpm_sts_expect(bool b_enable);

bool get_tpm_sts_dataavail(void);

void set_tpm_sts_dataavail(bool b_enable);

bool get_tpm_sts_tpmgo(void);

void set_tpm_sts_tpmgo(bool b_enable);

bool get_tpm_sts_commandready(void);

void set_tpm_sts_commandready(bool b_enable);

bool get_tpm_sts_stsvalid(void);

void set_tpm_sts_stsvalid(bool b_valid);

uint16_t get_tpm_sts_burstcount(void);

void set_tpm_sts_burstcount(uint16_t count);

bool get_tpm_sts_commandcancel(void);

void set_tpm_sts_commandcancel(bool b_cancel);

bool get_tpm_sts_resetestablishment(void);

void set_tpm_sts_resetestablishment(bool b_reset);

tpm_sts_tpmfamily_t get_tpm_sts_tpmfamily(void);

void set_tpm_sts_tpmfamily(tpm_sts_tpmfamily_t select);

uint32_t get_tpm_access_reg(void);

void set_tpm_access_reg(uint32_t reg_value);

bool get_tpm_access_establishment(void);

void set_tpm_access_establishment(bool b_enable);

bool get_tpm_access_requestUse(void);

void set_tpm_access_requestUse(bool b_enable);

bool get_tpm_access_pendingRequest(void);

void set_tpm_access_pendingRequest(bool b_enable);

bool get_tpm_access_seize(void);

void set_tpm_access_seize(bool b_enable);

bool get_tpm_access_beenseize(void);

void set_tpm_access_beenseize(bool b_enable);

bool get_tpm_access_activeLocality(void);

void set_tpm_access_activeLocality(bool b_enable);

bool get_tpm_access_tpmRegValidSts(void);

void set_tpm_access_tpmRegValidSts(bool b_enable);

uint8_t get_tpm_intvector_sirqvec(void);

void set_tpm_intvector_sirqvec(uint8_t vector);

uint32_t get_tpm_inten_reg(void);

void set_tpm_inten_reg(uint32_t reg_value);

bool get_tpm_inten_dataavailint(void);

void set_tpm_inten_dataavailint(bool b_enable);

bool get_tpm_inten_stsvalidint(void);

void set_tpm_inten_stsvalidint(bool b_enable);

bool get_tpm_inten_localitychangeint(void);

void set_tpm_inten_localitychangeint(bool b_enable);

tpm_inten_typpol_t get_tpm_inten_typepolarity(void);

void set_tpm_inten_typepolarity(tpm_inten_typpol_t polarity);

bool get_tpm_inten_commandready(void);

void set_tpm_inten_commandready(bool b_enable);

bool get_tpm_inten_globalint(void);

void set_tpm_inten_globalint(bool b_enable);

uint32_t get_tpm_intstatus_reg(void);

void set_tpm_intstatus_reg(uint32_t reg_value);

bool get_tpm_intstatus_dataAvailInt(void);

void set_tpm_intstatus_dataAvailInt(bool b_enable);

bool get_tpm_intstatus_stsValidInt(void);

void set_tpm_intstatus_stsValidInt(bool b_enable);

bool get_tpm_intstatus_localityChangeInt(void);

void set_tpm_intstatus_localityChangeInt(bool b_enable);

bool get_tpm_intstatus_commandReadyInt(void);

void set_tpm_intstatus_commandReadyInt(bool b_enable);

uint32_t get_tpm_intfcap_reg(void);

void set_tpm_intfcap_reg(uint32_t reg_value);

bool get_tpm_intfcap_dataAvailInt(void);

void set_tpm_intfcap_dataAvailInt(bool b_enable);

bool get_tpm_intfcap_stsValidInt(void);

void set_tpm_intfcap_stsValidInt(bool b_enable);

bool get_tpm_intfcap_localityChangeInt(void);

void set_tpm_intfcap_localityChangeInt(bool b_enable);

bool get_tpm_intfcap_InterruptLevelHigh(void);

void set_tpm_intfcap_InterruptLevelHigh(bool b_enable);

bool get_tpm_intfcap_InterruptLevelLow(void);

void set_tpm_intfcap_InterruptLevelLow(bool b_enable);

bool get_tpm_intfcap_InterruptEdgeRising(void);

void set_tpm_intfcap_InterruptEdgeRising(bool b_enable);

bool get_tpm_intfcap_InterruptEdgeFalling(void);

void set_tpm_intfcap_InterruptEdgeFalling(bool b_enable);

bool get_tpm_intfcap_CommandReadyInt(void);

void set_tpm_intfcap_CommandReadyInt(bool b_enable);

bool get_tpm_intfcap_BurstCountStatic(void);

void set_tpm_intfcap_BurstCountStatic(bool b_enable);

tpm_intfcap_transfsize_t get_tpm_intfcap_DataTransferSize(void);

void set_tpm_intfcap_DataTransferSize(bool b_enable);

tpm_intfcap_interface_t get_tpm_intfcap_InterfaceVersion(void);

void set_tpm_intfcap_InterfaceVersion(bool b_enable);




#ifdef __cplusplus
}
#endif

#endif /* TPM_INTERFACE_H */

/*** end of file ***/
