/**
 * @file usbd_hid.h
 * @brief An implementation of helper functions for USB device interface.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef USBD_HID_H
#define USBD_HID_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "fsusb_driver.h"
#include "osif_driver.h"
#include "usb_standard.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/* ENDPOINT NUMBER */
#define USBD_EP_NB0 (0u)
#define USBD_EP_NB1 (1u)
#define USBD_EP_NB2 (2u)
#define USBD_EP_NB3 (3u)
#define USBD_EP_NB4 (4u)
#define USBD_EP_NB5 (5u)
#define USBD_EP_NB6 (6u)

/*******************************************************************************
 * Typedefs
 ******************************************************************************/

/*******************************************************************************
 * Exported variables
 ******************************************************************************/

/*******************************************************************************
 * Global inline function definitions(C99 compliance)
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Initial routine for USB setting.
 */
void usbd_init(void);

/**
 * @brief Deinitial routine for USB setting.
 */
void usbd_deinit(void);

/**
 * @brief Install a buffer for dumping data to system memory at the receiving done moment while
 * Endpoint 4 fetched a piece of data, which is stored in SRAM, form host to device hardware module.
 *
 * @param[in] p_buf The buffer in system memory, where is located as start address.
 * @param[in] buf_size The size of the buffer is allocated.
 */
void usbd_install_ep4_rcv_buffer(uint8_t *p_buf, uint32_t buf_size);

/**
 * @brief Install a buffer for dumping data to system memory at the receiving done moment while
 * Endpoint 6 fetched a piece of data, which is stored in SRAM, form host to device hardware module.
 *
 * @param[in] p_buf The buffer in system memory, where is located as start address.
 * @param[in] buf_size The size of the buffer is allocated.
 */
void usbd_install_ep6_rcv_buffer(uint8_t *p_buf, uint32_t buf_size);

/**
 * @brief Device reset.
 */
void usbd_device_reset(void);

/**
 * @brief To reset USB device if receive bus reset event.
 *
 * @return bool The flag denotes USB has been reset if return true.
 */
bool usbd_check_bus_rst_immediately(void);

/**
 * @brief To detect bus line state.
 *
 * @return bool The flag denotes active on USB bus line if return true.
 */
bool usbd_detect_bus_acitve(void);

/**
 * @brief To check bus power state.
 *
 * @return bool The flag denotes bus power, has ever normally came from host, if return true.
 * The flag will be clear till device reset or bus reset.
 */
bool usbd_check_bus_power_on(void);

/**
 * @brief To check control setup(end-point 0) occurring receiving setup packet double time.
 *
 * @return bool The flag denotes receiving setup packet again while an uncompleted setup packet has
 * been in process. There is duplication occurring if return true.
 */
bool usbd_check_ctrl_setup_duplication(void);

/**
 * @brief To check control setup(end-point 0) occurring receiving zero length of setup packet.
 *
 * @return bool There is zero length of setup packet occurring if return true.
 */
bool usbd_check_ctrl_setup_zero_length(void);

/**
 * @brief To handle all the events of USB control pipe.
 */
void usbd_ctrl_pipe_proc(void);

/**
 * @brief To handle all the events of USB interrupt-out pipe, which implys the direction from host
 * to device.
 */
void usbd_intrpt_out_pipe_proc(void);

/**
 * @brief To handle all the events of USB interrupt-in pipe, which implies teh direction from device
 * to host.
 */
void usbd_intrpt_in_pipe_proc(void);

/**
 * @brief To handle all the events of USB bulk-out pipe, which implies the direction from host to
 * device.
 */
void usbd_bulk_out_pipe_proc(void);

/**
 * @brief To handle all the events of USB bulk-in pipe, which implies the direction from device to
 * host.
 */
void usbd_bulk_in_pipe_proc(void);

/**
 * @brief A monitor task during this USB device has been mounted on a host.
 */
void usbd_status_monitor_proc(void);

/**
 * @brief Receive an income transaction for a specified end-point.
 *
 * @param[in] group_ep_num To indicate which end-point has new arrival event.
 * @param[in] p_income A pointer indicates income buffer pointer.
 * @param[in] byte_cnt Indicate received buffer length.
 */
void usbd_pipe_sram1_acquire_job(uint8_t group_ep_num, uint8_t *p_income, uint32_t byte_cnt)
    __attribute__((weak));

/**
 * @brief Prepare to dispatch job for a specified end-point.
 *
 * @param[in] group_ep_num To indicate which end-point has new arrival event.
 * @param[in] p_source A pointer indicates out-going buffer pointer.
 * @param[in] byte_cnt Indicate transmit data length.
 */
void usbd_pipe_sram1_queue_job(uint8_t group_ep_num, uint8_t *p_source, uint32_t byte_cnt);

/**
 * @brief Receive an income transaction for a specified end-point.
 *
 * @param[in] group_ep_num To indicate which end-point has new arrival event.
 * @param[in] p_income A pointer indicates income buffer pointer.
 * @param[in] byte_cnt Indicate received buffer length.
 */
void usbd_pipe_sram3_acquire_job(uint8_t group_ep_num, uint8_t *p_income, uint32_t byte_cnt)
    __attribute__((weak));

/**
 * @brief Prepare to dispatch job for a specified end-point.
 *
 * @param[in] group_ep_num To indicate which end-point has new arrival event.
 * @param[in] p_source A pointer indicates out-going buffer pointer.
 * @param[in] byte_cnt Indicate transmit data length.
 */
void usbd_pipe_sram3_queue_job(uint8_t group_ep_num, uint8_t *p_source, uint32_t byte_cnt);

#ifdef __cplusplus
}
#endif

#endif /* USBD_HID_H */

/*** end of file ***/
