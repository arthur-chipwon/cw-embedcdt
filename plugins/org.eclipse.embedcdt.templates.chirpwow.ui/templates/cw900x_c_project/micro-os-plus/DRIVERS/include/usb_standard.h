/**
 * @file usb_standard.h
 * @brief The definition of structures for USB standard.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef USB_STANDARD_H
#define USB_STANDARD_H

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
/** Descriptor type */
#define USB_DESC_DEVICE                (0x01u)
#define USB_DESC_CONFIGURATION         (0x02u)
#define USB_DESC_STRING                (0x03u)
#define USB_DESC_INTERFACE             (0x04u)
#define USB_DESC_ENDPOINT              (0x05u)
#define USB_DESC_DEVICE_QUALIFIER      (0x06u)
#define USB_DESC_OTHER_SPEED_CONFIG    (0x07u)
#define USB_DESC_INTERFACE_POWER       (0x08u)
#define USB_DESC_OTG                   (0x09u)
#define USB_DESC_DEBUG                 (0x0Au)
#define USB_DESC_INTERFACE_ASSOCIATION (0x0Bu)

/** USB Class Specifications */
#define USB_CLS_UAC    (0x01u) /**< Audio Class */
#define USB_CLS_CDC    (0x02u) /**< Communication Device Class (CDC) */
#define USB_CLS_HID    (0x03u) /**< Human Interface Device Class (HID) */
#define USB_CLS_PTP    (0x06u) /**< Primary Talent Pool Class (PTP) */
#define USB_CLS_PTR    (0x07u) /**< Printer Class */
#define USB_CLS_MSC    (0x08u) /**< Mass Storage Class */
#define USB_CLS_HUB    (0x09u) /**< Hub Class */
#define USB_CLS_CCID   (0x0Bu) /**< Chip/Smart Card Interface Device Class (CCID) */
#define USB_CLS_UVC    (0x0Eu) /**< USB Video Class (UVC) */
#define USB_CLS_BTH    (0xE0u) /**< Bluetooth Class */
#define USB_CLS_VENDOR (0xFFu) /**< Vendor Class */

/** Indexes of String Descriptor */
#define USB_STR_IDX000   (0x00u) /**< Index 00h(0) */
#define USB_STR_IDX001   (0x01u) /**< Index 01h(1) */
#define USB_STR_IDX002   (0x02u) /**< Index 02h(2) */
#define USB_STR_IDX003   (0x03u) /**< Index 03h(3) */
#define USB_STR_IDX004   (0x04u) /**< Index 04h(4) */
#define USB_STR_IDX005   (0x05u) /**< Index 05h(5) */
#define USB_STR_IDX006   (0x06u) /**< Index 06h(6) */
#define USB_STR_IDX007   (0x07u) /**< Index 07h(7) */
#define USB_STR_IDX008   (0x08u) /**< Index 08h(8) */
#define USB_STR_IDX009   (0x09u) /**< Index 09h(9) */
#define USB_STR_IDX010   (0x0Au) /**< Index 0Ah(10) */
#define USB_STR_IDX011   (0x0Bu) /**< Index 0Bh(11) */
#define USB_STR_IDX012   (0x0Cu) /**< Index 0Ch(12) */
#define USB_STR_IDX013   (0x0Du) /**< Index 0Dh(13) */
#define USB_STR_IDX014   (0x0Eu) /**< Index 0Eh(14) */
#define USB_STR_IDX015   (0x0Fu) /**< Index 0Fh(15) */
#define USB_STR_IDX016   (0x10u) /**< Index 10h(16) */
#define USB_STR_IDX017   (0x11u) /**< Index 11h(17) */
#define USB_STR_IDX018   (0x12u) /**< Index 12h(18) */
#define USB_STR_IDX019   (0x13u) /**< Index 13h(19) */
#define USB_STR_IDX020   (0x14u) /**< Index 14h(20) */
#define USB_STR_IDX021   (0x15u) /**< Index 15h(21) */
#define USB_STR_IDX022   (0x16u) /**< Index 16h(22) */
#define USB_STR_IDX023   (0x17u) /**< Index 17h(23) */
#define USB_STR_IDX024   (0x18u) /**< Index 18h(24) */
#define USB_STR_IDX025   (0x19u) /**< Index 19h(25) */
#define USB_STR_IDX026   (0x1Au) /**< Index 1Ah(26) */
#define USB_STR_IDX027   (0x1Bu) /**< Index 1Bh(27) */
#define USB_STR_IDX028   (0x1Cu) /**< Index 1Ch(28) */
#define USB_STR_IDX029   (0x1Du) /**< Index 1Dh(29) */
#define USB_STR_IDX030   (0x1Eu) /**< Index 1Eh(30) */
#define USB_STR_IDX031   (0x1Fu) /**< Index 1Fh(31) */
#define USB_STR_IDXXXX   (0x20u) /**< Index 20h(32) - FEh(254) Author fill it up by self if need. */
#define USB_STR_IDX255   (0xFFu) /**< Index FFh(255) */
#define USB_STR_IDX_RSVD (0xFFu) /**< Index FFh(255) */

/** Attributes in Configuration Descriptors */
#define USB_CFG_ATTR_SELF_POWERED  (0x40u) /**< Self-Powered */
#define USB_CFG_ATTR_REMOTE_WAKEUP (0x20u) /**< Remote Wakeup */

/** End-point Direction of End-point Descriptor */
#define USB_ENDPOINT_OUT (0x00u) /**< Out Direction from Host to Device */
#define USB_ENDPOINT_IN  (0x80u) /**< In Direction from Host to Device */

/** End-point Type of End-point Descriptor */
#define USB_EPT_CONTROL     (0x00u) /**< Control PID */
#define USB_EPT_ISOCHRONOUS (0x01u) /**< Isochronous PID */
#define USB_EPT_BULK        (0x02u) /**< Bulk PID */
#define USB_EPT_INTERRUPT   (0x03u) /**< Interrupt PID */

/** USB Standard Request Codes */
#define USB_REQ_GET_STATUS        (0x00u)
#define USB_REQ_CLEAR_FEATURE     (0x01u)
#define USB_REQ_SET_FEATURE       (0x03u)
#define USB_REQ_SET_ADDRESS       (0x05u)
#define USB_REQ_GET_DESCRIPTOR    (0x06u)
#define USB_REQ_SET_DESCRIPTOR    (0x07u)
#define USB_REQ_GET_CONFIGURATION (0x08u)
#define USB_REQ_SET_CONFIGURATION (0x09u)
#define USB_REQ_GET_INTERFACE     (0x0Au)
#define USB_REQ_SET_INTERFACE     (0x0Bu)

/** Request Error Code */
#define USB_REQ_EC_NO_ERROR        (0x00u)
#define USB_REQ_EC_NOT_READY       (0x01u)
#define USB_REQ_EC_WRONG_STATE     (0x02u)
#define USB_REQ_EC_POWER           (0x03u)
#define USB_REQ_EC_OUT_OF_RANGE    (0x04u)
#define USB_REQ_EC_INVALID_UNIT    (0x05u)
#define USB_REQ_EC_INVALID_CONTROL (0x06u)
#define USB_REQ_EC_INVALID_REQUEST (0x07u)
#define USB_REQ_EC_UNKNOW          (0xFFu)

/** bmReqType Attributions */
#if 1
#define USB_RT_DIR_HOST2DEV USB_ENDPOINT_OUT
#define USB_RT_DIR_DEV2HOST USB_ENDPOINT_IN
#else
#define USB_RT_DIR_HOST2DEV (0x00u)
#define USB_RT_DIR_DEV2HOST (0x80u)
#endif

/** Request Types */
#define USB_RT_TYPE_MASK     (0x60u)
#define USB_RT_TYPE_STANDARD (0x00u)
#define USB_RT_TYPE_CLASS    (0x20u)
#define USB_RT_TYPE_VENDOR   (0x40u)
#define USB_RT_TYPE_RESERVED (0x60u)

/** Request Types */
#define USB_RT_RECIPIENT_MASK      (0x1Fu)
#define USB_RT_RECIPIENT_DEVICE    (0x00u)
#define USB_RT_RECIPIENT_INTERFACE (0x01u)
#define USB_RT_RECIPIENT_ENDPOINT  (0x02u)
#define USB_RT_RECIPIENT_OTHER     (0x03u)

/** Features */
#define USB_REQ_FEAT_ENDPOINT_HALT     (0x00u)
#define USB_REQ_FEAT_DEV_REMOTE_WAKEUP (0x01u)
#define USB_REQ_FEAT_TEST_MODE         (0x02u)

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
#pragma pack(1)
/**
 * @brief USB Device Descriptor.
 * @param items 14.
 * @param size 18(12h).
 */
typedef struct _usb_std_dev_desc_t
{
    uint8_t  blength;        /**< bLength: Size of this descriptor in bytes */
    uint8_t  bdesc_type;     /**< bDescriptorType: DEVICE descriptor type */
    uint16_t bcdusb;         /**< bcdUSB: Binay Coded Decimal Spec. release */
    uint8_t  bdev_class;     /**< bDeviceClass: Class code assigned by the USB */
    uint8_t  bdev_sub_class; /**< bDeviceSubClass: Sub-class code assigned by the USB */
    uint8_t  bdev_protocol;  /**< bDeviceProtocol: Protocol code assigned by the USB */
    uint8_t  bmax_pkt_size0; /**< bMaxPacketSize0: Max packet size for EP0 */
    uint16_t idvendor;       /**< idVendor: Vendor ID. ATMEL = 0x03EB */
    uint16_t idproduct;      /**< idProduct: Product ID assigned by the manufacturer */
    uint16_t bcddev;         /**< bcdDevice: Device release number */
    uint8_t  imfr;           /**< iManufacturer: Index of manu. string descriptor */
    uint8_t  iproduct;       /**< iProduct: Index of prod. string descriptor */
    uint8_t  iserial_num;    /**< iSerialNumber: Index of S.N. string descriptor */
    uint8_t  bnum_cfg;       /**< bNumConfigurations: Number of possible configurations */
} usb_std_dev_desc_t;

/**
 * @brief USB Device Qualifier Descriptor.
 * @param items 9.
 * @param size 10(Ah).
 */
typedef struct _usb_std_dev_qualf_desc_t
{
    uint8_t  blength;        /**< bLength: Size of this descriptor in bytes */
    uint8_t  bdesc_type;     /**< bDescriptorType: DEVICE descriptor type */
    uint16_t bcdusb;         /**< bcdUSB: Binay Coded Decimal Spec. release */
    uint8_t  bdev_class;     /**< bDeviceClass: Class code assigned by the USB */
    uint8_t  bdev_sub_class; /**< bDeviceSubClass: Sub-class code assigned by the USB */
    uint8_t  bdev_protocol;  /**< bDeviceProtocol: Protocol code assigned by the USB */
    uint8_t  bmax_pkt_size0; /**< bMaxPacketSize0: Max packet size for EP0 */
    uint8_t  bnum_cfg;       /**< bNumConfigurations: Number of possible configurations */
    uint8_t  breserved;      /**< bReserved: Reserved for future use */
} usb_std_dev_qualf_desc_t;

/**
 * @brief USB Device Configuration Descriptor.
 * @param items 8.
 * @param size 9(9h).
 */
typedef struct _usb_std_cfg_desc_t
{
    uint8_t  blength;    /**< bLength: Size of this descriptor in bytes */
    uint8_t  bdesc_type; /**< bDescriptorType: CONFIGURATION descriptor type */
    uint16_t wtotal_len; /**< wTotalLength: total length of data returned */
    uint8_t  bnum_if;    /**< bNumInterfaces: number of interfaces for this conf. */
    uint8_t  bcfg_value; /**< bConfigurationValue: value for SetConfiguration resquest */
    uint8_t  icfg;       /**< iConfiguration: index of string descriptor */
    uint8_t  bmattrib;   /**< bmAttributes: Configuration characteristics */
    uint8_t  bmax_power; /**< bMaxPower: maximum power consumption */
} usb_std_cfg_desc_t;

/**
 * @brief USB Other Speed Configuration Descriptor.
 * @param items 9.
 * @param size 9(9h).
 */
typedef usb_std_cfg_desc_t usb_std_other_spd_cfg_desc_t;

/**
 * @brief USB Interface Descriptor.
 * @param items 9.
 * @param size 9(9h).
 */
typedef struct _usb_std_if_desc_t
{
    uint8_t blength;       /**< bLength: Size of this descriptor in bytes */
    uint8_t bdesc_type;    /**< bDescriptorType: INTERFACE descriptor type */
    uint8_t bif_number;    /**< bInterfaceNumber: Indicate interface number */
    uint8_t balt_setting;  /**< bAlternateSetting: value to select alternate setting */
    uint8_t bnum_ep;       /**< bNumEndpoints: Number of EP except EP 0 */
    uint8_t bif_class;     /**< bInterfaceClass: Class code assigned by the USB */
    uint8_t bif_sub_class; /**< bInterfaceSubClass: Sub-class code assigned by the USB */
    uint8_t bif_protocol;  /**< bInterfaceProtocol: Protocol code assigned by the USB */
    uint8_t iinterface;    /**< iInterface: Index of string descriptor */
} usb_std_if_desc_t;

/**
 * @brief USB Endpoint Descriptor.
 * @param items 6.
 * @param size 7(7h).
 */
typedef struct _usb_std_ep_desc_t
{
    uint8_t  blength;       /**< bLength: Size of this descriptor in bytes */
    uint8_t  bdesc_type;    /**< bDescriptorType: ENDPOINT descriptor type */
    uint8_t  bep_address;   /**< bEndpointAddress: Address of the endpoint */
    uint8_t  bmattrib;      /**< bmAttributes: Endpoint's attributes */
    uint16_t wmax_pkt_size; /**< wMaxPacketSize: Maximum packet size for this EP */
    uint8_t  binterval;     /**< bInterval: Interval for polling EP in ms */
} usb_std_ep_desc_t;

/**
 * @brief USB String Header Descriptor.
 * @param items 2.
 * @param size 2(2h).
 */
typedef struct _usb_std_str_desc_hdr_t
{
    uint8_t blength;    /**< bLength: Size of this descriptor in bytes */
    uint8_t bdesc_type; /**< bDescriptorType: STRING descriptor type */
} usb_std_str_desc_hdr_t;

typedef struct _u16_8_t
{
    uint8_t lbyte;
    uint8_t hbyte;
} u16_8_t;

typedef struct _u16_16_t
{
    uint16_t word;
} u16_16_t;

typedef union _u16_t
{
    u16_8_t  sb_type;
    u16_16_t db_type;
} u16_t;

/**
 * @brief USB Device Request Descriptor.
 * @param items 5.
 * @param size 8(8h).
 */
typedef struct _usb_dev_req_t
{
    uint8_t bmreq_type; /**< bmRequestType: Characteristics of request */
    uint8_t brequest;   /**< bRequest: Specific request */
    u16_t   wvalue;     /**< wValue: Word-sized field */
    u16_t   windex;     /**< wIndex: Word-sized field */
    u16_t   wlength;    /**< wLength: Number of bytes to request transfer */
} usb_dev_req_t;
#pragma pack()

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

#endif /* USB_STANDARD_H */

/*** end of file ***/
