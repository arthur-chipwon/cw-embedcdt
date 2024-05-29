/**
 * @file $(CMSIS_name).h
 * @brief CMSIS cortex-m0 device peripheral access layer header file.
 *          This file contains all the peripheral register's definitions.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef $(CMSIS_nameUppercase)_H
#define $(CMSIS_nameUppercase)_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/* IO definitions (access restrictions to peripheral registers) */
/**
 *   IO Type Qualifiers are used
 *   \li to specify the access to peripheral variables.
 *   \li for automatic generation of peripheral register debug information.
 */
#ifndef _IO
#ifdef __cplusplus
#define __I volatile       /**< Defines 'read only' permissions                 */
#else
#define __I volatile const /**< Defines 'read only' permissions           */
#endif
#define __O volatile       /**< Defines 'write only' permissions               */
#define _IO volatile       /**< Defines 'read / write' permissions             */
#define _NA const          /**< Defines 'reserved' permissions                 */
#endif

/**
 * @brief 32 bits memory read macro.
 */
#if !defined(REG_READ32)
#define REG_READ32(address) (*(volatile uint32_t *)(address))
#endif

/**
 * @brief 32 bits memory write macro.
 */
#if !defined(REG_WRITE32)
#define REG_WRITE32(address, value) ((*(volatile uint32_t *)(address)) = (uint32_t)(value))
#endif

/**
 * @brief 32 bits bits setting macro.
 */
#if !defined(REG_BIT_SET32)
#define REG_BIT_SET32(address, mask) ((*(volatile uint32_t *)(address)) |= (uint32_t)(mask))
#endif

/**
 * @brief 32 bits bits clearing macro.
 */
#if !defined(REG_BIT_CLEAR32)
#define REG_BIT_CLEAR32(address, mask) \
    ((*(volatile uint32_t *)(address)) &= ((uint32_t) ~((uint32_t)(mask))))
#endif

/**
 * @brief 32 bits bits toggling macro.
 */
#if !defined(REG_BIT_TOGGLE32)
#define REG_BIT_TOGGLE32(address, mask) ((*(volatile uint32_t *)(address)) ^= (uint32_t)(mask))
#endif

/**
 * @brief 32 bit clear bits and set with new value
 * @note It is user's responsability to make sure that value has only "mask"
 * bits set - (value&~mask)==0
 */
#if !defined(REG_RMW32)
#define REG_RMW32(address, mask, value) \
    (REG_WRITE32(                       \
        (address),                      \
        ((REG_READ32(address) & ((uint32_t) ~((uint32_t)(mask)))) | ((uint32_t)(value)))))
#endif

/**
 * @addtogroup Interrupt_vector_numbers_$(CMSIS_name) Interrupt vector numbers for $(CMSIS_nameUppercase)
 * @{
 */
/** Interrupt Number Definitions */
#define NUMBER_OF_INT_VECTORS 48u

/**
 * @brief Defines the Interrupt Numbers definitions
 *
 * This enumeration is used to configure the interrupts.
 *
 * Implements : IRQn_t_Class
 */
typedef enum
{
    /* Auxiliary constants */
    NotAvail_IRQn = -128, /**< Not available device specific interrupt */

    /* Core interrupts */
    NonMaskableInt_IRQn = -14, /**< Non Maskable Interrupt */
    HardFault_IRQn      = -13, /**< Cortex-M0 SV Hard Fault Interrupt */
    SVCall_IRQn         = -5,  /**< Cortex-M0 SV Call Interrupt */
    PendSV_IRQn         = -2,  /**< Cortex-M0 Pend SV Interrupt */
    SysTick_IRQn        = -1,  /**< Cortex-M0 System Tick Interrupt */

    /* Device specific interrupts */
    DMA0_IRQn            = 0u,  /**< DMA channel 0 transfer complete */
    DMA1_IRQn            = 1u,  /**< DMA channel 1 transfer complete */
    DMA2_IRQn            = 2u,  /**< DMA channel 2 transfer complete */
    DMA3_IRQn            = 3u,  /**< DMA channel 3 transfer complete */
    LPTMR0_IRQn          = 8u,  /**< LPTIMER interrupt request */
    PORT_IRQn            = 9u,  /**< Port A, B, C, D and E pin detect interrupt */
    FTM0_Ch0_7_IRQn      = 12u, /**< FTM0 Channel 0 to 7 interrupt */
    FTM0_Fault_IRQn      = 13u, /**< FTM0 Fault interrupt */
    FTM0_Ovf_Reload_IRQn = 14u, /**< FTM0 overflow / Reload interrupt */
    FTM1_Ch0_7_IRQn      = 15u, /**< FTM1 Channel 0 to 7 interrupt */
    FTM1_Fault_IRQn      = 16u, /**< FTM1 Fault interrupt */
    FTM1_Ovf_Reload_IRQn = 17u, /**< FTM1 overflow / Reload interrupt */
    FTFC_IRQn            = 18u, /**< Read collision and Double bit  fault detect */
    HSSPI_IRQn           = 19u, /**< HSSPI interrupt */
    LPI2C1_IRQn          = 20u, /**< LPI2C1 Interrupt */
    WDOG_IRQn            = 22u, /**< WDOG interrupt request out before wdg reset out */
    RCM_IRQn             = 23u, /**< RCM Asynchronous Interrupt */
    LPI2C0_IRQn          = 24u, /**< LPI2C0 Interrupt */
    PUF_IRQn             = 25u, /**< PUF Interrupt */
    LPSPI0_IRQn          = 26u, /**< LPSPI0 Interrupt */
    LPSPI1_IRQn          = 27u, /**< LPSPI1 Interrupt */
    FSUSB_IRQn           = 28u, /**< FSUSB interrupt request. */
    LPSPI2_IRQn          = 29u, /**< LPSPI2 Interrupt */
    LPUART1_RxTx_IRQn    = 30u, /**< LPUART1 Tx / Rx Interrupt */
    LPUART0_RxTx_IRQn    = 31u  /**< LPUART0 Tx / Rx Interrupt */
} IRQn_t;

/**
 * @}
 */ /* end of group Interrupt_vector_numbers_$(CMSIS_name) */

/* ----------------------------------------------------------------------
   -- Cortex M0 Core Configuration
   ---------------------------------------------------------------------- */

/**
 * @addtogroup Cortex_Core_Configuration Cortex M0 Core Configuration
 * @{
 */

#define __MPU_PRESENT          0 /**< Defines if an MPU is present or not */
#define __NVIC_PRIO_BITS       2 /**< Number of priority bits */
#define __Vendor_SysTickConfig 0 /**< SysTickConfig is defined */
#define __FPU_PRESENT          0 /**< Defines if an FPU is present or not */

/**
 * @}
 */ /* end of group Cortex_Core_Configuration */

/* ----------------------------------------------------------------------
  -- Device Peripheral Access Layer for $(CMSIS_nameUppercase)
  ----------------------------------------------------------------------- */

/**
 * @addtogroup Peripheral_access_layer_$(CMSIS_name) Device Peripheral Access Layer for $(CMSIS_nameUppercase)
 * @{
 */

/* @brief This module covers memory mapped registers available on SoC */

/* ----------------------------------------------------------------------
   -- CRC Peripheral Access Layer
   ---------------------------------------------------------------------- */

/**
 * @addtogroup CRC_Peripheral_Access_Layer CRC Peripheral Access Layer
 * @{
 */

/** CRC - Size of Registers Arrays */

/** CRC - Register Layout Typedef */
typedef struct
{
    union
    {                       /* offset: 0x0 */
        _IO uint32_t DATA;  /**< offset: 0x0 */
        struct
        {                   /* offset: 0x0 */
            _IO uint16_t L; /**< offset: 0x0 */
            _IO uint16_t H; /**< offset: 0x2 */
        } DATA_16;
        struct
        {                   /* offset: 0x0 */
            _IO uint8_t LL; /**< offset: 0x0 */
            _IO uint8_t LU; /**< offset: 0x1 */
            _IO uint8_t HL; /**< offset: 0x2 */
            _IO uint8_t HU; /**< offset: 0x3 */
        } DATA_8;
    } DATAu;
    _IO uint32_t GPOLY; /**< offset: 0x4 */
    _IO uint32_t CTRL;  /**< offset: 0x8 */
} CRC_t, *CRC_MemMapPtr;

/** Number of instances of the CRC module. */
#define CRC_INSTANCE_COUNT (1u)

/* CRC - Peripheral instance base addresses */
/** Peripheral CRC base address */
#define CRC_BASE (0x40032000u)
/** Peripheral CRC base pointer */
#define CRC ((CRC_t *)CRC_BASE)
/** Array initializer of CRC peripheral base addresses */
#define CRC_BASE_ADDRS \
    {                  \
        CRC_BASE       \
    }
/** Array initializer of CRC peripheral base pointers */
#define CRC_BASE_PTRS \
    {                 \
        CRC           \
    }

/* --------------------------------------------------------------------------
   -- CRC Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup CRC_Register_Masks CRC Register Masks
 * @{
 */

/* DATAu_DATA Bit Fields */
#define CRC_DATAu_DATA_LL_MASK  0xFFu
#define CRC_DATAu_DATA_LL_SHIFT 0u
#define CRC_DATAu_DATA_LL_WIDTH 8u
#define CRC_DATAu_DATA_LL(x) \
    (((uint32_t)(((uint32_t)(x)) << CRC_DATAu_DATA_LL_SHIFT)) & CRC_DATAu_DATA_LL_MASK)
#define CRC_DATAu_DATA_LU_MASK  0xFF00u
#define CRC_DATAu_DATA_LU_SHIFT 8u
#define CRC_DATAu_DATA_LU_WIDTH 8u
#define CRC_DATAu_DATA_LU(x) \
    (((uint32_t)(((uint32_t)(x)) << CRC_DATAu_DATA_LU_SHIFT)) & CRC_DATAu_DATA_LU_MASK)
#define CRC_DATAu_DATA_HL_MASK  0xFF0000u
#define CRC_DATAu_DATA_HL_SHIFT 16u
#define CRC_DATAu_DATA_HL_WIDTH 8u
#define CRC_DATAu_DATA_HL(x) \
    (((uint32_t)(((uint32_t)(x)) << CRC_DATAu_DATA_HL_SHIFT)) & CRC_DATAu_DATA_HL_MASK)
#define CRC_DATAu_DATA_HU_MASK  0xFF000000u
#define CRC_DATAu_DATA_HU_SHIFT 24u
#define CRC_DATAu_DATA_HU_WIDTH 8u
#define CRC_DATAu_DATA_HU(x) \
    (((uint32_t)(((uint32_t)(x)) << CRC_DATAu_DATA_HU_SHIFT)) & CRC_DATAu_DATA_HU_MASK)
/* DATAu_DATA_16_L Bit Fields */
#define CRC_DATAu_DATA_16_L_DATAL_MASK  0xFFFFu
#define CRC_DATAu_DATA_16_L_DATAL_SHIFT 0u
#define CRC_DATAu_DATA_16_L_DATAL_WIDTH 16u
#define CRC_DATAu_DATA_16_L_DATAL(x)                                    \
    (((uint16_t)(((uint16_t)(x)) << CRC_DATAu_DATA_16_L_DATAL_SHIFT)) & \
     CRC_DATAu_DATA_16_L_DATAL_MASK)
/* DATAu_DATA_16_H Bit Fields */
#define CRC_DATAu_DATA_16_H_DATAH_MASK  0xFFFFu
#define CRC_DATAu_DATA_16_H_DATAH_SHIFT 0u
#define CRC_DATAu_DATA_16_H_DATAH_WIDTH 16u
#define CRC_DATAu_DATA_16_H_DATAH(x)                                    \
    (((uint16_t)(((uint16_t)(x)) << CRC_DATAu_DATA_16_H_DATAH_SHIFT)) & \
     CRC_DATAu_DATA_16_H_DATAH_MASK)
/* DATAu_DATA_8_LL Bit Fields */
#define CRC_DATAu_DATA_8_LL_DATALL_MASK  0xFFu
#define CRC_DATAu_DATA_8_LL_DATALL_SHIFT 0u
#define CRC_DATAu_DATA_8_LL_DATALL_WIDTH 8u
#define CRC_DATAu_DATA_8_LL_DATALL(x)                                  \
    (((uint8_t)(((uint8_t)(x)) << CRC_DATAu_DATA_8_LL_DATALL_SHIFT)) & \
     CRC_DATAu_DATA_8_LL_DATALL_MASK)
/* DATAu_DATA_8_LU Bit Fields */
#define CRC_DATAu_DATA_8_LU_DATALU_MASK  0xFFu
#define CRC_DATAu_DATA_8_LU_DATALU_SHIFT 0u
#define CRC_DATAu_DATA_8_LU_DATALU_WIDTH 8u
#define CRC_DATAu_DATA_8_LU_DATALU(x)                                  \
    (((uint8_t)(((uint8_t)(x)) << CRC_DATAu_DATA_8_LU_DATALU_SHIFT)) & \
     CRC_DATAu_DATA_8_LU_DATALU_MASK)
/* DATAu_DATA_8_HL Bit Fields */
#define CRC_DATAu_DATA_8_HL_DATAHL_MASK  0xFFu
#define CRC_DATAu_DATA_8_HL_DATAHL_SHIFT 0u
#define CRC_DATAu_DATA_8_HL_DATAHL_WIDTH 8u
#define CRC_DATAu_DATA_8_HL_DATAHL(x)                                  \
    (((uint8_t)(((uint8_t)(x)) << CRC_DATAu_DATA_8_HL_DATAHL_SHIFT)) & \
     CRC_DATAu_DATA_8_HL_DATAHL_MASK)
/* DATAu_DATA_8_HU Bit Fields */
#define CRC_DATAu_DATA_8_HU_DATAHU_MASK  0xFFu
#define CRC_DATAu_DATA_8_HU_DATAHU_SHIFT 0u
#define CRC_DATAu_DATA_8_HU_DATAHU_WIDTH 8u
#define CRC_DATAu_DATA_8_HU_DATAHU(x)                                  \
    (((uint8_t)(((uint8_t)(x)) << CRC_DATAu_DATA_8_HU_DATAHU_SHIFT)) & \
     CRC_DATAu_DATA_8_HU_DATAHU_MASK)
/* GPOLY Bit Fields */
#define CRC_GPOLY_LOW_MASK   0xFFFFu
#define CRC_GPOLY_LOW_SHIFT  0u
#define CRC_GPOLY_LOW_WIDTH  16u
#define CRC_GPOLY_LOW(x)     (((uint32_t)(((uint32_t)(x)) << CRC_GPOLY_LOW_SHIFT)) & CRC_GPOLY_LOW_MASK)
#define CRC_GPOLY_HIGH_MASK  0xFFFF0000u
#define CRC_GPOLY_HIGH_SHIFT 16u
#define CRC_GPOLY_HIGH_WIDTH 16u
#define CRC_GPOLY_HIGH(x) \
    (((uint32_t)(((uint32_t)(x)) << CRC_GPOLY_HIGH_SHIFT)) & CRC_GPOLY_HIGH_MASK)
/* CTRL Bit Fields */
#define CRC_CTRL_CRC5_MASK  0x20u
#define CRC_CTRL_CRC5_SHIFT 5u
#define CRC_CTRL_CRC5_WIDTH 1u
#define CRC_CTRL_CRC5(x)    (((uint32_t)(((uint32_t)(x)) << CRC_CTRL_CRC5_SHIFT)) & CRC_CTRL_CRC5_MASK)
#define CRC_CTRL_CRC8_MASK  0x100u
#define CRC_CTRL_CRC8_SHIFT 8u
#define CRC_CTRL_CRC8_WIDTH 1u
#define CRC_CTRL_CRC8(x)    (((uint32_t)(((uint32_t)(x)) << CRC_CTRL_CRC8_SHIFT)) & CRC_CTRL_CRC8_MASK)
#define CRC_CTRL_TCRC_MASK  0x1000000u
#define CRC_CTRL_TCRC_SHIFT 24u
#define CRC_CTRL_TCRC_WIDTH 1u
#define CRC_CTRL_TCRC(x)    (((uint32_t)(((uint32_t)(x)) << CRC_CTRL_TCRC_SHIFT)) & CRC_CTRL_TCRC_MASK)
#define CRC_CTRL_WAS_MASK   0x2000000u
#define CRC_CTRL_WAS_SHIFT  25u
#define CRC_CTRL_WAS_WIDTH  1u
#define CRC_CTRL_WAS(x)     (((uint32_t)(((uint32_t)(x)) << CRC_CTRL_WAS_SHIFT)) & CRC_CTRL_WAS_MASK)
#define CRC_CTRL_FXOR_MASK  0x4000000u
#define CRC_CTRL_FXOR_SHIFT 26u
#define CRC_CTRL_FXOR_WIDTH 1u
#define CRC_CTRL_FXOR(x)    (((uint32_t)(((uint32_t)(x)) << CRC_CTRL_FXOR_SHIFT)) & CRC_CTRL_FXOR_MASK)
#define CRC_CTRL_TOTR_MASK  0x30000000u
#define CRC_CTRL_TOTR_SHIFT 28u
#define CRC_CTRL_TOTR_WIDTH 2u
#define CRC_CTRL_TOTR(x)    (((uint32_t)(((uint32_t)(x)) << CRC_CTRL_TOTR_SHIFT)) & CRC_CTRL_TOTR_MASK)
#define CRC_CTRL_TOT_MASK   0xC0000000u
#define CRC_CTRL_TOT_SHIFT  30u
#define CRC_CTRL_TOT_WIDTH  2u
#define CRC_CTRL_TOT(x)     (((uint32_t)(((uint32_t)(x)) << CRC_CTRL_TOT_SHIFT)) & CRC_CTRL_TOT_MASK)

/**
 * @}
 */ /* end of group CRC_Register_Masks */

/**
 * @}
 */ /* end of group CRC_Peripheral_Access_Layer */

/* --------------------------------------------------------------------------
   -- DMA Peripheral Access Layer
   -------------------------------------------------------------------------- */

/**
 * @addtogroup DMA_Peripheral_Access_Layer DMA Peripheral Access Layer
 * @{
 */

/** DMA - Size of Registers Arrays */
#define DMA_DCHPRI_COUNT 4u
#define DMA_TCD_COUNT    4u

/** DMA - Register Layout Typedef */
typedef struct
{
    _IO uint32_t CR;                       /**< offset: 0x0 */
    _NA uint8_t  RESERVED_0[8];
    _IO uint32_t ERQ;                      /**< offset: 0xC */
    _NA uint8_t  RESERVED_1[10];
    __O uint8_t  CERQ;                     /**< offset: 0x1A */
    __O uint8_t  SERQ;                     /**< offset: 0x1B */
    __O uint8_t  CDNE;                     /**< offset: 0x1C */
    __O uint8_t  SSRT;                     /**< offset: 0x1D */
    __O uint8_t  CERR;                     /**< offset: 0x1E */
    __O uint8_t  CINT;                     /**< offset: 0x1F */
    _NA uint8_t  RESERVED_2[4];
    _IO uint32_t INT;                      /**< offset: 0x24 */
    _NA uint8_t  RESERVED_3[4];
    _IO uint32_t ERR;                      /**< offset: 0x2C */
    _NA uint8_t  RESERVED_4[208];
    _IO uint8_t  DCHPRI[DMA_DCHPRI_COUNT]; /**< offset: 0x100 */
    _NA uint8_t  RESERVED_5[3836];
    struct
    {                              /* offset: 0x1000, array step: 0x20 */
        _IO uint32_t SADDR;        /**< offset: 0x1000, array step: 0x20 */
        _IO uint16_t SOFF;         /**< offset: 0x1004, array step: 0x20 */
        _IO uint16_t ATTR;         /**< offset: 0x1006, array step: 0x20 */
        union
        {                          /* offset: 0x1008, array step: 0x20 */
            _IO uint32_t MLNO;     /**< offset: 0x1008, array step: 0x20 */
            _IO uint32_t MLOFFNO;  /**< offset: 0x1008, array step: 0x20 */
            _IO uint32_t MLOFFYES; /**< offset: 0x1008, array step: 0x20 */
        } NBYTES;
        _IO uint32_t SLAST;        /**< offset: 0x100C, array step: 0x20 */
        _IO uint32_t DADDR;        /**< offset: 0x1010, array step: 0x20 */
        _IO uint16_t DOFF;         /**< offset: 0x1014, array step: 0x20 */
        union
        {                          /* offset: 0x1016, array step: 0x20 */
            _IO uint16_t ELINKNO;  /**< offset: 0x1016, array step: 0x20 */
            _IO uint16_t ELINKYES; /**< offset: 0x1016, array step: 0x20 */
        } CITER;
        _IO uint32_t DLASTSGA;     /**< offset: 0x1018, array step: 0x20 */
        _IO uint16_t CSR;          /**< offset: 0x101C, array step: 0x20 */
        union
        {                          /* offset: 0x101E, array step: 0x20 */
            _IO uint16_t ELINKNO;  /**< offset: 0x101E, array step: 0x20 */
            _IO uint16_t ELINKYES; /**< offset: 0x101E, array step: 0x20 */
        } BITER;
    } TCD[DMA_TCD_COUNT];
} DMA_t, *DMA_MemMapPtr;

/** Number of instances of the DMA module. */
#define DMA_INSTANCE_COUNT (1u)

/* DMA - Peripheral instance base addresses */
/** Peripheral DMA base address */
#define DMA_BASE (0x40008000u)
/** Peripheral DMA base pointer */
#define DMA ((DMA_t *)DMA_BASE)
/** Array initializer of DMA peripheral base addresses */
#define DMA_BASE_ADDRS \
    {                  \
        DMA_BASE       \
    }
/** Array initializer of DMA peripheral base pointers */
#define DMA_BASE_PTRS \
    {                 \
        DMA           \
    }
/** Number of interrupt vector arrays for the DMA module. */
#define DMA_IRQS_ARR_COUNT (2u)
/** Number of interrupt channels for the CHN type of DMA module. */
#define DMA_CHN_IRQS_CH_COUNT (4u)
/** Number of interrupt channels for the ERROR type of DMA module. */
#define DMA_ERROR_IRQS_CH_COUNT (1u)
/** Interrupt vectors for the DMA peripheral type */
#define DMA_CHN_IRQS                               \
    {                                              \
        DMA0_IRQn, DMA1_IRQn, DMA2_IRQn, DMA3_IRQn \
    }

/* --------------------------------------------------------------------------
   -- DMA Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup DMA_Register_Masks DMA Register Masks
 * @{
 */

/* CR Bit Fields */
#define DMA_CR_EDBG_MASK    0x2u
#define DMA_CR_EDBG_SHIFT   1u
#define DMA_CR_EDBG_WIDTH   1u
#define DMA_CR_EDBG(x)      (((uint32_t)(((uint32_t)(x)) << DMA_CR_EDBG_SHIFT)) & DMA_CR_EDBG_MASK)
#define DMA_CR_ERCA_MASK    0x4u
#define DMA_CR_ERCA_SHIFT   2u
#define DMA_CR_ERCA_WIDTH   1u
#define DMA_CR_ERCA(x)      (((uint32_t)(((uint32_t)(x)) << DMA_CR_ERCA_SHIFT)) & DMA_CR_ERCA_MASK)
#define DMA_CR_HOE_MASK     0x10u
#define DMA_CR_HOE_SHIFT    4u
#define DMA_CR_HOE_WIDTH    1u
#define DMA_CR_HOE(x)       (((uint32_t)(((uint32_t)(x)) << DMA_CR_HOE_SHIFT)) & DMA_CR_HOE_MASK)
#define DMA_CR_HALT_MASK    0x20u
#define DMA_CR_HALT_SHIFT   5u
#define DMA_CR_HALT_WIDTH   1u
#define DMA_CR_HALT(x)      (((uint32_t)(((uint32_t)(x)) << DMA_CR_HALT_SHIFT)) & DMA_CR_HALT_MASK)
#define DMA_CR_CLM_MASK     0x40u
#define DMA_CR_CLM_SHIFT    6u
#define DMA_CR_CLM_WIDTH    1u
#define DMA_CR_CLM(x)       (((uint32_t)(((uint32_t)(x)) << DMA_CR_CLM_SHIFT)) & DMA_CR_CLM_MASK)
#define DMA_CR_EMLM_MASK    0x80u
#define DMA_CR_EMLM_SHIFT   7u
#define DMA_CR_EMLM_WIDTH   1u
#define DMA_CR_EMLM(x)      (((uint32_t)(((uint32_t)(x)) << DMA_CR_EMLM_SHIFT)) & DMA_CR_EMLM_MASK)
#define DMA_CR_ECX_MASK     0x10000u
#define DMA_CR_ECX_SHIFT    16u
#define DMA_CR_ECX_WIDTH    1u
#define DMA_CR_ECX(x)       (((uint32_t)(((uint32_t)(x)) << DMA_CR_ECX_SHIFT)) & DMA_CR_ECX_MASK)
#define DMA_CR_CX_MASK      0x20000u
#define DMA_CR_CX_SHIFT     17u
#define DMA_CR_CX_WIDTH     1u
#define DMA_CR_CX(x)        (((uint32_t)(((uint32_t)(x)) << DMA_CR_CX_SHIFT)) & DMA_CR_CX_MASK)
#define DMA_CR_ACTIVE_MASK  0x80000000u
#define DMA_CR_ACTIVE_SHIFT 31u
#define DMA_CR_ACTIVE_WIDTH 1u
#define DMA_CR_ACTIVE(x)    (((uint32_t)(((uint32_t)(x)) << DMA_CR_ACTIVE_SHIFT)) & DMA_CR_ACTIVE_MASK)
/* ES Bit Fields */
/* ERQ Bit Fields */
#define DMA_ERQ_ERQ0_MASK  0x1u
#define DMA_ERQ_ERQ0_SHIFT 0u
#define DMA_ERQ_ERQ0_WIDTH 1u
#define DMA_ERQ_ERQ0(x)    (((uint32_t)(((uint32_t)(x)) << DMA_ERQ_ERQ0_SHIFT)) & DMA_ERQ_ERQ0_MASK)
#define DMA_ERQ_ERQ1_MASK  0x2u
#define DMA_ERQ_ERQ1_SHIFT 1u
#define DMA_ERQ_ERQ1_WIDTH 1u
#define DMA_ERQ_ERQ1(x)    (((uint32_t)(((uint32_t)(x)) << DMA_ERQ_ERQ1_SHIFT)) & DMA_ERQ_ERQ1_MASK)
#define DMA_ERQ_ERQ2_MASK  0x4u
#define DMA_ERQ_ERQ2_SHIFT 2u
#define DMA_ERQ_ERQ2_WIDTH 1u
#define DMA_ERQ_ERQ2(x)    (((uint32_t)(((uint32_t)(x)) << DMA_ERQ_ERQ2_SHIFT)) & DMA_ERQ_ERQ2_MASK)
#define DMA_ERQ_ERQ3_MASK  0x8u
#define DMA_ERQ_ERQ3_SHIFT 3u
#define DMA_ERQ_ERQ3_WIDTH 1u
#define DMA_ERQ_ERQ3(x)    (((uint32_t)(((uint32_t)(x)) << DMA_ERQ_ERQ3_SHIFT)) & DMA_ERQ_ERQ3_MASK)
/* EEI Bit Fields */
/* CEEI Bit Fields */
/* SEEI Bit Fields */
/* CERQ Bit Fields */
#define DMA_CERQ_CERQ_MASK  0xFu
#define DMA_CERQ_CERQ_SHIFT 0u
#define DMA_CERQ_CERQ_WIDTH 4u
#define DMA_CERQ_CERQ(x)    (((uint8_t)(((uint8_t)(x)) << DMA_CERQ_CERQ_SHIFT)) & DMA_CERQ_CERQ_MASK)
#define DMA_CERQ_CAER_MASK  0x40u
#define DMA_CERQ_CAER_SHIFT 6u
#define DMA_CERQ_CAER_WIDTH 1u
#define DMA_CERQ_CAER(x)    (((uint8_t)(((uint8_t)(x)) << DMA_CERQ_CAER_SHIFT)) & DMA_CERQ_CAER_MASK)
#define DMA_CERQ_NOP_MASK   0x80u
#define DMA_CERQ_NOP_SHIFT  7u
#define DMA_CERQ_NOP_WIDTH  1u
#define DMA_CERQ_NOP(x)     (((uint8_t)(((uint8_t)(x)) << DMA_CERQ_NOP_SHIFT)) & DMA_CERQ_NOP_MASK)
/* SERQ Bit Fields */
#define DMA_SERQ_SERQ_MASK  0xFu
#define DMA_SERQ_SERQ_SHIFT 0u
#define DMA_SERQ_SERQ_WIDTH 4u
#define DMA_SERQ_SERQ(x)    (((uint8_t)(((uint8_t)(x)) << DMA_SERQ_SERQ_SHIFT)) & DMA_SERQ_SERQ_MASK)
#define DMA_SERQ_SAER_MASK  0x40u
#define DMA_SERQ_SAER_SHIFT 6u
#define DMA_SERQ_SAER_WIDTH 1u
#define DMA_SERQ_SAER(x)    (((uint8_t)(((uint8_t)(x)) << DMA_SERQ_SAER_SHIFT)) & DMA_SERQ_SAER_MASK)
#define DMA_SERQ_NOP_MASK   0x80u
#define DMA_SERQ_NOP_SHIFT  7u
#define DMA_SERQ_NOP_WIDTH  1u
#define DMA_SERQ_NOP(x)     (((uint8_t)(((uint8_t)(x)) << DMA_SERQ_NOP_SHIFT)) & DMA_SERQ_NOP_MASK)
/* CDNE Bit Fields */
#define DMA_CDNE_CDNE_MASK  0xFu
#define DMA_CDNE_CDNE_SHIFT 0u
#define DMA_CDNE_CDNE_WIDTH 4u
#define DMA_CDNE_CDNE(x)    (((uint8_t)(((uint8_t)(x)) << DMA_CDNE_CDNE_SHIFT)) & DMA_CDNE_CDNE_MASK)
#define DMA_CDNE_CADN_MASK  0x40u
#define DMA_CDNE_CADN_SHIFT 6u
#define DMA_CDNE_CADN_WIDTH 1u
#define DMA_CDNE_CADN(x)    (((uint8_t)(((uint8_t)(x)) << DMA_CDNE_CADN_SHIFT)) & DMA_CDNE_CADN_MASK)
#define DMA_CDNE_NOP_MASK   0x80u
#define DMA_CDNE_NOP_SHIFT  7u
#define DMA_CDNE_NOP_WIDTH  1u
#define DMA_CDNE_NOP(x)     (((uint8_t)(((uint8_t)(x)) << DMA_CDNE_NOP_SHIFT)) & DMA_CDNE_NOP_MASK)
/* SSRT Bit Fields */
#define DMA_SSRT_SSRT_MASK  0xFu
#define DMA_SSRT_SSRT_SHIFT 0u
#define DMA_SSRT_SSRT_WIDTH 4u
#define DMA_SSRT_SSRT(x)    (((uint8_t)(((uint8_t)(x)) << DMA_SSRT_SSRT_SHIFT)) & DMA_SSRT_SSRT_MASK)
#define DMA_SSRT_SAST_MASK  0x40u
#define DMA_SSRT_SAST_SHIFT 6u
#define DMA_SSRT_SAST_WIDTH 1u
#define DMA_SSRT_SAST(x)    (((uint8_t)(((uint8_t)(x)) << DMA_SSRT_SAST_SHIFT)) & DMA_SSRT_SAST_MASK)
#define DMA_SSRT_NOP_MASK   0x80u
#define DMA_SSRT_NOP_SHIFT  7u
#define DMA_SSRT_NOP_WIDTH  1u
#define DMA_SSRT_NOP(x)     (((uint8_t)(((uint8_t)(x)) << DMA_SSRT_NOP_SHIFT)) & DMA_SSRT_NOP_MASK)
/* CERR Bit Fields */
#define DMA_CERR_CERR_MASK  0xFu
#define DMA_CERR_CERR_SHIFT 0u
#define DMA_CERR_CERR_WIDTH 4u
#define DMA_CERR_CERR(x)    (((uint8_t)(((uint8_t)(x)) << DMA_CERR_CERR_SHIFT)) & DMA_CERR_CERR_MASK)
#define DMA_CERR_CAEI_MASK  0x40u
#define DMA_CERR_CAEI_SHIFT 6u
#define DMA_CERR_CAEI_WIDTH 1u
#define DMA_CERR_CAEI(x)    (((uint8_t)(((uint8_t)(x)) << DMA_CERR_CAEI_SHIFT)) & DMA_CERR_CAEI_MASK)
#define DMA_CERR_NOP_MASK   0x80u
#define DMA_CERR_NOP_SHIFT  7u
#define DMA_CERR_NOP_WIDTH  1u
#define DMA_CERR_NOP(x)     (((uint8_t)(((uint8_t)(x)) << DMA_CERR_NOP_SHIFT)) & DMA_CERR_NOP_MASK)
/* CINT Bit Fields */
#define DMA_CINT_CINT_MASK  0xFu
#define DMA_CINT_CINT_SHIFT 0u
#define DMA_CINT_CINT_WIDTH 4u
#define DMA_CINT_CINT(x)    (((uint8_t)(((uint8_t)(x)) << DMA_CINT_CINT_SHIFT)) & DMA_CINT_CINT_MASK)
#define DMA_CINT_CAIR_MASK  0x40u
#define DMA_CINT_CAIR_SHIFT 6u
#define DMA_CINT_CAIR_WIDTH 1u
#define DMA_CINT_CAIR(x)    (((uint8_t)(((uint8_t)(x)) << DMA_CINT_CAIR_SHIFT)) & DMA_CINT_CAIR_MASK)
#define DMA_CINT_NOP_MASK   0x80u
#define DMA_CINT_NOP_SHIFT  7u
#define DMA_CINT_NOP_WIDTH  1u
#define DMA_CINT_NOP(x)     (((uint8_t)(((uint8_t)(x)) << DMA_CINT_NOP_SHIFT)) & DMA_CINT_NOP_MASK)
/* INT Bit Fields */
#define DMA_INT_INT0_MASK  0x1u
#define DMA_INT_INT0_SHIFT 0u
#define DMA_INT_INT0_WIDTH 1u
#define DMA_INT_INT0(x)    (((uint32_t)(((uint32_t)(x)) << DMA_INT_INT0_SHIFT)) & DMA_INT_INT0_MASK)
#define DMA_INT_INT1_MASK  0x2u
#define DMA_INT_INT1_SHIFT 1u
#define DMA_INT_INT1_WIDTH 1u
#define DMA_INT_INT1(x)    (((uint32_t)(((uint32_t)(x)) << DMA_INT_INT1_SHIFT)) & DMA_INT_INT1_MASK)
#define DMA_INT_INT2_MASK  0x4u
#define DMA_INT_INT2_SHIFT 2u
#define DMA_INT_INT2_WIDTH 1u
#define DMA_INT_INT2(x)    (((uint32_t)(((uint32_t)(x)) << DMA_INT_INT2_SHIFT)) & DMA_INT_INT2_MASK)
#define DMA_INT_INT3_MASK  0x8u
#define DMA_INT_INT3_SHIFT 3u
#define DMA_INT_INT3_WIDTH 1u
#define DMA_INT_INT3(x)    (((uint32_t)(((uint32_t)(x)) << DMA_INT_INT3_SHIFT)) & DMA_INT_INT3_MASK)
/* ERR Bit Fields */
#define DMA_ERR_ERR0_MASK  0x1u
#define DMA_ERR_ERR0_SHIFT 0u
#define DMA_ERR_ERR0_WIDTH 1u
#define DMA_ERR_ERR0(x)    (((uint32_t)(((uint32_t)(x)) << DMA_ERR_ERR0_SHIFT)) & DMA_ERR_ERR0_MASK)
#define DMA_ERR_ERR1_MASK  0x2u
#define DMA_ERR_ERR1_SHIFT 1u
#define DMA_ERR_ERR1_WIDTH 1u
#define DMA_ERR_ERR1(x)    (((uint32_t)(((uint32_t)(x)) << DMA_ERR_ERR1_SHIFT)) & DMA_ERR_ERR1_MASK)
#define DMA_ERR_ERR2_MASK  0x4u
#define DMA_ERR_ERR2_SHIFT 2u
#define DMA_ERR_ERR2_WIDTH 1u
#define DMA_ERR_ERR2(x)    (((uint32_t)(((uint32_t)(x)) << DMA_ERR_ERR2_SHIFT)) & DMA_ERR_ERR2_MASK)
#define DMA_ERR_ERR3_MASK  0x8u
#define DMA_ERR_ERR3_SHIFT 3u
#define DMA_ERR_ERR3_WIDTH 1u
#define DMA_ERR_ERR3(x)    (((uint32_t)(((uint32_t)(x)) << DMA_ERR_ERR3_SHIFT)) & DMA_ERR_ERR3_MASK)
/* HRS Bit Fields */
/* EARS Bit Fields */
/* DCHPRI Bit Fields */
#define DMA_DCHPRI_CHPRI_MASK  0xFu
#define DMA_DCHPRI_CHPRI_SHIFT 0u
#define DMA_DCHPRI_CHPRI_WIDTH 4u
#define DMA_DCHPRI_CHPRI(x) \
    (((uint8_t)(((uint8_t)(x)) << DMA_DCHPRI_CHPRI_SHIFT)) & DMA_DCHPRI_CHPRI_MASK)
#define DMA_DCHPRI_DPA_MASK  0x40u
#define DMA_DCHPRI_DPA_SHIFT 6u
#define DMA_DCHPRI_DPA_WIDTH 1u
#define DMA_DCHPRI_DPA(x) \
    (((uint8_t)(((uint8_t)(x)) << DMA_DCHPRI_DPA_SHIFT)) & DMA_DCHPRI_DPA_MASK)
#define DMA_DCHPRI_ECP_MASK  0x80u
#define DMA_DCHPRI_ECP_SHIFT 7u
#define DMA_DCHPRI_ECP_WIDTH 1u
#define DMA_DCHPRI_ECP(x) \
    (((uint8_t)(((uint8_t)(x)) << DMA_DCHPRI_ECP_SHIFT)) & DMA_DCHPRI_ECP_MASK)
/* TCD_SADDR Bit Fields */
#define DMA_TCD_SADDR_SADDR_MASK  0xFFFFFFFFu
#define DMA_TCD_SADDR_SADDR_SHIFT 0u
#define DMA_TCD_SADDR_SADDR_WIDTH 32u
#define DMA_TCD_SADDR_SADDR(x) \
    (((uint32_t)(((uint32_t)(x)) << DMA_TCD_SADDR_SADDR_SHIFT)) & DMA_TCD_SADDR_SADDR_MASK)
/* TCD_SOFF Bit Fields */
#define DMA_TCD_SOFF_SOFF_MASK  0xFFFFu
#define DMA_TCD_SOFF_SOFF_SHIFT 0u
#define DMA_TCD_SOFF_SOFF_WIDTH 16u
#define DMA_TCD_SOFF_SOFF(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_SOFF_SOFF_SHIFT)) & DMA_TCD_SOFF_SOFF_MASK)
/* TCD_ATTR Bit Fields */
#define DMA_TCD_ATTR_DSIZE_MASK  0x7u
#define DMA_TCD_ATTR_DSIZE_SHIFT 0u
#define DMA_TCD_ATTR_DSIZE_WIDTH 3u
#define DMA_TCD_ATTR_DSIZE(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_ATTR_DSIZE_SHIFT)) & DMA_TCD_ATTR_DSIZE_MASK)
#define DMA_TCD_ATTR_DMOD_MASK  0xF8u
#define DMA_TCD_ATTR_DMOD_SHIFT 3u
#define DMA_TCD_ATTR_DMOD_WIDTH 5u
#define DMA_TCD_ATTR_DMOD(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_ATTR_DMOD_SHIFT)) & DMA_TCD_ATTR_DMOD_MASK)
#define DMA_TCD_ATTR_SSIZE_MASK  0x700u
#define DMA_TCD_ATTR_SSIZE_SHIFT 8u
#define DMA_TCD_ATTR_SSIZE_WIDTH 3u
#define DMA_TCD_ATTR_SSIZE(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_ATTR_SSIZE_SHIFT)) & DMA_TCD_ATTR_SSIZE_MASK)
#define DMA_TCD_ATTR_SMOD_MASK  0xF800u
#define DMA_TCD_ATTR_SMOD_SHIFT 11u
#define DMA_TCD_ATTR_SMOD_WIDTH 5u
#define DMA_TCD_ATTR_SMOD(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_ATTR_SMOD_SHIFT)) & DMA_TCD_ATTR_SMOD_MASK)
/* TCD_NBYTES_MLNO Bit Fields */
#define DMA_TCD_NBYTES_MLNO_NBYTES_MASK  0xFFFFFFFFu
#define DMA_TCD_NBYTES_MLNO_NBYTES_SHIFT 0u
#define DMA_TCD_NBYTES_MLNO_NBYTES_WIDTH 32u
#define DMA_TCD_NBYTES_MLNO_NBYTES(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << DMA_TCD_NBYTES_MLNO_NBYTES_SHIFT)) & \
     DMA_TCD_NBYTES_MLNO_NBYTES_MASK)
/* TCD_NBYTES_MLOFFNO Bit Fields */
#define DMA_TCD_NBYTES_MLOFFNO_NBYTES_MASK  0x3FFFFFFFu
#define DMA_TCD_NBYTES_MLOFFNO_NBYTES_SHIFT 0u
#define DMA_TCD_NBYTES_MLOFFNO_NBYTES_WIDTH 30u
#define DMA_TCD_NBYTES_MLOFFNO_NBYTES(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << DMA_TCD_NBYTES_MLOFFNO_NBYTES_SHIFT)) & \
     DMA_TCD_NBYTES_MLOFFNO_NBYTES_MASK)
#define DMA_TCD_NBYTES_MLOFFNO_DMLOE_MASK  0x40000000u
#define DMA_TCD_NBYTES_MLOFFNO_DMLOE_SHIFT 30u
#define DMA_TCD_NBYTES_MLOFFNO_DMLOE_WIDTH 1u
#define DMA_TCD_NBYTES_MLOFFNO_DMLOE(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << DMA_TCD_NBYTES_MLOFFNO_DMLOE_SHIFT)) & \
     DMA_TCD_NBYTES_MLOFFNO_DMLOE_MASK)
#define DMA_TCD_NBYTES_MLOFFNO_SMLOE_MASK  0x80000000u
#define DMA_TCD_NBYTES_MLOFFNO_SMLOE_SHIFT 31u
#define DMA_TCD_NBYTES_MLOFFNO_SMLOE_WIDTH 1u
#define DMA_TCD_NBYTES_MLOFFNO_SMLOE(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << DMA_TCD_NBYTES_MLOFFNO_SMLOE_SHIFT)) & \
     DMA_TCD_NBYTES_MLOFFNO_SMLOE_MASK)
/* TCD_NBYTES_MLOFFYES Bit Fields */
#define DMA_TCD_NBYTES_MLOFFYES_NBYTES_MASK  0x3FFu
#define DMA_TCD_NBYTES_MLOFFYES_NBYTES_SHIFT 0u
#define DMA_TCD_NBYTES_MLOFFYES_NBYTES_WIDTH 10u
#define DMA_TCD_NBYTES_MLOFFYES_NBYTES(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << DMA_TCD_NBYTES_MLOFFYES_NBYTES_SHIFT)) & \
     DMA_TCD_NBYTES_MLOFFYES_NBYTES_MASK)
#define DMA_TCD_NBYTES_MLOFFYES_MLOFF_MASK  0x3FFFFC00u
#define DMA_TCD_NBYTES_MLOFFYES_MLOFF_SHIFT 10u
#define DMA_TCD_NBYTES_MLOFFYES_MLOFF_WIDTH 20u
#define DMA_TCD_NBYTES_MLOFFYES_MLOFF(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << DMA_TCD_NBYTES_MLOFFYES_MLOFF_SHIFT)) & \
     DMA_TCD_NBYTES_MLOFFYES_MLOFF_MASK)
#define DMA_TCD_NBYTES_MLOFFYES_DMLOE_MASK  0x40000000u
#define DMA_TCD_NBYTES_MLOFFYES_DMLOE_SHIFT 30u
#define DMA_TCD_NBYTES_MLOFFYES_DMLOE_WIDTH 1u
#define DMA_TCD_NBYTES_MLOFFYES_DMLOE(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << DMA_TCD_NBYTES_MLOFFYES_DMLOE_SHIFT)) & \
     DMA_TCD_NBYTES_MLOFFYES_DMLOE_MASK)
#define DMA_TCD_NBYTES_MLOFFYES_SMLOE_MASK  0x80000000u
#define DMA_TCD_NBYTES_MLOFFYES_SMLOE_SHIFT 31u
#define DMA_TCD_NBYTES_MLOFFYES_SMLOE_WIDTH 1u
#define DMA_TCD_NBYTES_MLOFFYES_SMLOE(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << DMA_TCD_NBYTES_MLOFFYES_SMLOE_SHIFT)) & \
     DMA_TCD_NBYTES_MLOFFYES_SMLOE_MASK)
/* TCD_SLAST Bit Fields */
#define DMA_TCD_SLAST_SLAST_MASK  0xFFFFFFFFu
#define DMA_TCD_SLAST_SLAST_SHIFT 0u
#define DMA_TCD_SLAST_SLAST_WIDTH 32u
#define DMA_TCD_SLAST_SLAST(x) \
    (((uint32_t)(((uint32_t)(x)) << DMA_TCD_SLAST_SLAST_SHIFT)) & DMA_TCD_SLAST_SLAST_MASK)
/* TCD_DADDR Bit Fields */
#define DMA_TCD_DADDR_DADDR_MASK  0xFFFFFFFFu
#define DMA_TCD_DADDR_DADDR_SHIFT 0u
#define DMA_TCD_DADDR_DADDR_WIDTH 32u
#define DMA_TCD_DADDR_DADDR(x) \
    (((uint32_t)(((uint32_t)(x)) << DMA_TCD_DADDR_DADDR_SHIFT)) & DMA_TCD_DADDR_DADDR_MASK)
/* TCD_DOFF Bit Fields */
#define DMA_TCD_DOFF_DOFF_MASK  0xFFFFu
#define DMA_TCD_DOFF_DOFF_SHIFT 0u
#define DMA_TCD_DOFF_DOFF_WIDTH 16u
#define DMA_TCD_DOFF_DOFF(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_DOFF_DOFF_SHIFT)) & DMA_TCD_DOFF_DOFF_MASK)
/* TCD_CITER_ELINKNO Bit Fields */
#define DMA_TCD_CITER_ELINKNO_CITER_MASK  0x7FFFu
#define DMA_TCD_CITER_ELINKNO_CITER_SHIFT 0u
#define DMA_TCD_CITER_ELINKNO_CITER_WIDTH 15u
#define DMA_TCD_CITER_ELINKNO_CITER(x)                                    \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CITER_ELINKNO_CITER_SHIFT)) & \
     DMA_TCD_CITER_ELINKNO_CITER_MASK)
#define DMA_TCD_CITER_ELINKNO_ELINK_MASK  0x8000u
#define DMA_TCD_CITER_ELINKNO_ELINK_SHIFT 15u
#define DMA_TCD_CITER_ELINKNO_ELINK_WIDTH 1u
#define DMA_TCD_CITER_ELINKNO_ELINK(x)                                    \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CITER_ELINKNO_ELINK_SHIFT)) & \
     DMA_TCD_CITER_ELINKNO_ELINK_MASK)
/* TCD_CITER_ELINKYES Bit Fields */
#define DMA_TCD_CITER_ELINKYES_CITER_LE_MASK  0x1FFu
#define DMA_TCD_CITER_ELINKYES_CITER_LE_SHIFT 0u
#define DMA_TCD_CITER_ELINKYES_CITER_LE_WIDTH 9u
#define DMA_TCD_CITER_ELINKYES_CITER_LE(x)                                    \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CITER_ELINKYES_CITER_LE_SHIFT)) & \
     DMA_TCD_CITER_ELINKYES_CITER_LE_MASK)
#define DMA_TCD_CITER_ELINKYES_LINKCH_MASK  0x1E00u
#define DMA_TCD_CITER_ELINKYES_LINKCH_SHIFT 9u
#define DMA_TCD_CITER_ELINKYES_LINKCH_WIDTH 4u
#define DMA_TCD_CITER_ELINKYES_LINKCH(x)                                    \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CITER_ELINKYES_LINKCH_SHIFT)) & \
     DMA_TCD_CITER_ELINKYES_LINKCH_MASK)
#define DMA_TCD_CITER_ELINKYES_ELINK_MASK  0x8000u
#define DMA_TCD_CITER_ELINKYES_ELINK_SHIFT 15u
#define DMA_TCD_CITER_ELINKYES_ELINK_WIDTH 1u
#define DMA_TCD_CITER_ELINKYES_ELINK(x)                                    \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CITER_ELINKYES_ELINK_SHIFT)) & \
     DMA_TCD_CITER_ELINKYES_ELINK_MASK)
/* TCD_DLASTSGA Bit Fields */
#define DMA_TCD_DLASTSGA_DLASTSGA_MASK  0xFFFFFFFFu
#define DMA_TCD_DLASTSGA_DLASTSGA_SHIFT 0u
#define DMA_TCD_DLASTSGA_DLASTSGA_WIDTH 32u
#define DMA_TCD_DLASTSGA_DLASTSGA(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << DMA_TCD_DLASTSGA_DLASTSGA_SHIFT)) & \
     DMA_TCD_DLASTSGA_DLASTSGA_MASK)
/* TCD_CSR Bit Fields */
#define DMA_TCD_CSR_START_MASK  0x1u
#define DMA_TCD_CSR_START_SHIFT 0u
#define DMA_TCD_CSR_START_WIDTH 1u
#define DMA_TCD_CSR_START(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CSR_START_SHIFT)) & DMA_TCD_CSR_START_MASK)
#define DMA_TCD_CSR_INTMAJOR_MASK  0x2u
#define DMA_TCD_CSR_INTMAJOR_SHIFT 1u
#define DMA_TCD_CSR_INTMAJOR_WIDTH 1u
#define DMA_TCD_CSR_INTMAJOR(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CSR_INTMAJOR_SHIFT)) & DMA_TCD_CSR_INTMAJOR_MASK)
#define DMA_TCD_CSR_INTHALF_MASK  0x4u
#define DMA_TCD_CSR_INTHALF_SHIFT 2u
#define DMA_TCD_CSR_INTHALF_WIDTH 1u
#define DMA_TCD_CSR_INTHALF(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CSR_INTHALF_SHIFT)) & DMA_TCD_CSR_INTHALF_MASK)
#define DMA_TCD_CSR_DREQ_MASK  0x8u
#define DMA_TCD_CSR_DREQ_SHIFT 3u
#define DMA_TCD_CSR_DREQ_WIDTH 1u
#define DMA_TCD_CSR_DREQ(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CSR_DREQ_SHIFT)) & DMA_TCD_CSR_DREQ_MASK)
#define DMA_TCD_CSR_ESG_MASK  0x10u
#define DMA_TCD_CSR_ESG_SHIFT 4u
#define DMA_TCD_CSR_ESG_WIDTH 1u
#define DMA_TCD_CSR_ESG(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CSR_ESG_SHIFT)) & DMA_TCD_CSR_ESG_MASK)
#define DMA_TCD_CSR_MAJORELINK_MASK  0x20u
#define DMA_TCD_CSR_MAJORELINK_SHIFT 5u
#define DMA_TCD_CSR_MAJORELINK_WIDTH 1u
#define DMA_TCD_CSR_MAJORELINK(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CSR_MAJORELINK_SHIFT)) & DMA_TCD_CSR_MAJORELINK_MASK)
#define DMA_TCD_CSR_ACTIVE_MASK  0x40u
#define DMA_TCD_CSR_ACTIVE_SHIFT 6u
#define DMA_TCD_CSR_ACTIVE_WIDTH 1u
#define DMA_TCD_CSR_ACTIVE(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CSR_ACTIVE_SHIFT)) & DMA_TCD_CSR_ACTIVE_MASK)
#define DMA_TCD_CSR_DONE_MASK  0x80u
#define DMA_TCD_CSR_DONE_SHIFT 7u
#define DMA_TCD_CSR_DONE_WIDTH 1u
#define DMA_TCD_CSR_DONE(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CSR_DONE_SHIFT)) & DMA_TCD_CSR_DONE_MASK)
#define DMA_TCD_CSR_MAJORLINKCH_MASK  0xF00u
#define DMA_TCD_CSR_MAJORLINKCH_SHIFT 8u
#define DMA_TCD_CSR_MAJORLINKCH_WIDTH 4u
#define DMA_TCD_CSR_MAJORLINKCH(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CSR_MAJORLINKCH_SHIFT)) & DMA_TCD_CSR_MAJORLINKCH_MASK)
#define DMA_TCD_CSR_BWC_MASK  0xC000u
#define DMA_TCD_CSR_BWC_SHIFT 14u
#define DMA_TCD_CSR_BWC_WIDTH 2u
#define DMA_TCD_CSR_BWC(x) \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_CSR_BWC_SHIFT)) & DMA_TCD_CSR_BWC_MASK)
/* TCD_BITER_ELINKNO Bit Fields */
#define DMA_TCD_BITER_ELINKNO_BITER_MASK  0x7FFFu
#define DMA_TCD_BITER_ELINKNO_BITER_SHIFT 0u
#define DMA_TCD_BITER_ELINKNO_BITER_WIDTH 15u
#define DMA_TCD_BITER_ELINKNO_BITER(x)                                    \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_BITER_ELINKNO_BITER_SHIFT)) & \
     DMA_TCD_BITER_ELINKNO_BITER_MASK)
#define DMA_TCD_BITER_ELINKNO_ELINK_MASK  0x8000u
#define DMA_TCD_BITER_ELINKNO_ELINK_SHIFT 15u
#define DMA_TCD_BITER_ELINKNO_ELINK_WIDTH 1u
#define DMA_TCD_BITER_ELINKNO_ELINK(x)                                    \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_BITER_ELINKNO_ELINK_SHIFT)) & \
     DMA_TCD_BITER_ELINKNO_ELINK_MASK)
/* TCD_BITER_ELINKYES Bit Fields */
#define DMA_TCD_BITER_ELINKYES_BITER_MASK  0x1FFu
#define DMA_TCD_BITER_ELINKYES_BITER_SHIFT 0u
#define DMA_TCD_BITER_ELINKYES_BITER_WIDTH 9u
#define DMA_TCD_BITER_ELINKYES_BITER(x)                                    \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_BITER_ELINKYES_BITER_SHIFT)) & \
     DMA_TCD_BITER_ELINKYES_BITER_MASK)
#define DMA_TCD_BITER_ELINKYES_LINKCH_MASK  0x1E00u
#define DMA_TCD_BITER_ELINKYES_LINKCH_SHIFT 9u
#define DMA_TCD_BITER_ELINKYES_LINKCH_WIDTH 4u
#define DMA_TCD_BITER_ELINKYES_LINKCH(x)                                    \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_BITER_ELINKYES_LINKCH_SHIFT)) & \
     DMA_TCD_BITER_ELINKYES_LINKCH_MASK)
#define DMA_TCD_BITER_ELINKYES_ELINK_MASK  0x8000u
#define DMA_TCD_BITER_ELINKYES_ELINK_SHIFT 15u
#define DMA_TCD_BITER_ELINKYES_ELINK_WIDTH 1u
#define DMA_TCD_BITER_ELINKYES_ELINK(x)                                    \
    (((uint16_t)(((uint16_t)(x)) << DMA_TCD_BITER_ELINKYES_ELINK_SHIFT)) & \
     DMA_TCD_BITER_ELINKYES_ELINK_MASK)

/**
 * @}
 */ /* end of group DMA_Register_Masks */

/**
 * @}
 */ /* end of group DMA_Peripheral_Access_Layer */

/* --------------------------------------------------------------------------
   -- DMAMUX Peripheral Access Layer
   -------------------------------------------------------------------------- */

/**
 * @addtogroup DMAMUX_Peripheral_Access_Layer DMAMUX Peripheral Access Layer
 * @{
 */

/** DMAMUX - Size of Registers Arrays */
#define DMAMUX_CHCFG_COUNT 4u

/** DMAMUX - Register Layout Typedef */
typedef struct
{
    _IO uint8_t CHCFG[DMAMUX_CHCFG_COUNT]; /**< Channel Configuration register,
                                              array offset: 0x0, array step: 0x1 */
} DMAMUX_t, *DMAMUX_MemMapPtr;

/** Number of instances of the DMAMUX module. */
#define DMAMUX_INSTANCE_COUNT (1u)

/* DMAMUX - Peripheral instance base addresses */
/** Peripheral DMAMUX base address */
#define DMAMUX_BASE (0x40021000u)
/** Peripheral DMAMUX base pointer */
#define DMAMUX ((DMAMUX_t *)DMAMUX_BASE)
/** Array initializer of DMAMUX peripheral base addresses */
#define DMAMUX_BASE_ADDRS \
    {                     \
        DMAMUX_BASE       \
    }
/** Array initializer of DMAMUX peripheral base pointers */
#define DMAMUX_BASE_PTRS \
    {                    \
        DMAMUX           \
    }

/* --------------------------------------------------------------------------
   -- DMAMUX Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup DMAMUX_Register_Masks DMAMUX Register Masks
 * @{
 */

/* CHCFG Bit Fields */
#define DMAMUX_CHCFG_SOURCE_MASK  0x3Fu
#define DMAMUX_CHCFG_SOURCE_SHIFT 0u
#define DMAMUX_CHCFG_SOURCE_WIDTH 6u
#define DMAMUX_CHCFG_SOURCE(x) \
    (((uint8_t)(((uint8_t)(x)) << DMAMUX_CHCFG_SOURCE_SHIFT)) & DMAMUX_CHCFG_SOURCE_MASK)
#define DMAMUX_CHCFG_TRIG_MASK  0x40u
#define DMAMUX_CHCFG_TRIG_SHIFT 6u
#define DMAMUX_CHCFG_TRIG_WIDTH 1u
#define DMAMUX_CHCFG_TRIG(x) \
    (((uint8_t)(((uint8_t)(x)) << DMAMUX_CHCFG_TRIG_SHIFT)) & DMAMUX_CHCFG_TRIG_MASK)
#define DMAMUX_CHCFG_ENBL_MASK  0x80u
#define DMAMUX_CHCFG_ENBL_SHIFT 7u
#define DMAMUX_CHCFG_ENBL_WIDTH 1u
#define DMAMUX_CHCFG_ENBL(x) \
    (((uint8_t)(((uint8_t)(x)) << DMAMUX_CHCFG_ENBL_SHIFT)) & DMAMUX_CHCFG_ENBL_MASK)

/**
 * @}
 */ /* end of group DMAMUX_Register_Masks */

/**
 * @}
 */ /* end of group DMAMUX_Peripheral_Access_Layer */

/* --------------------------------------------------------------------------
   -- FTFC Peripheral Access Layer
   -------------------------------------------------------------------------- */

/**
 * @addtogroup FTFC_Peripheral_Access_Layer FTFC Peripheral Access Layer
 * @{
 */

/** FTFC - Size of Registers Arrays */
#define FTFC_FCCOB_COUNT 12u
#define FTFC_FPROT_COUNT 4u

/** FTFC - Register Layout Typedef */
typedef struct
{
    _IO uint8_t FSTAT;                   /**< offset: 0x0 */
    _NA uint8_t RESERVED_0[3];
    _IO uint8_t FCCOB[FTFC_FCCOB_COUNT]; /**< offset: 0x4, array step: 0x1 */
    _NA uint8_t RESERVED_1[32];
} FTFC_t, *FTFC_MemMapPtr;

/** Number of instances of the FTFC module. */
#define FTFC_INSTANCE_COUNT (1u)

/* FTFC - Peripheral instance base addresses */
/** Peripheral FTFC base address */
#define FTFC_BASE (0x40020000u)
/** Peripheral FTFC base pointer */
#define FTFC ((FTFC_t *)FTFC_BASE)
/** Array initializer of FTFC peripheral base addresses */
#define FTFC_BASE_ADDRS \
    {                   \
        FTFC_BASE       \
    }
/** Array initializer of FTFC peripheral base pointers */
#define FTFC_BASE_PTRS \
    {                  \
        FTFC           \
    }
/** Number of interrupt vector arrays for the FTFC module. */
#define FTFC_IRQS_ARR_COUNT (2u)
/** Number of interrupt channels for the COMMAND_COMPLETE type of FTFC module.
 */
#define FTFC_COMMAND_COMPLETE_IRQS_CH_COUNT (1u)
/** Number of interrupt channels for the READ_COLLISION type of FTFC module. */
#define FTFC_READ_COLLISION_IRQS_CH_COUNT (1u)
/** Interrupt vectors for the FTFC peripheral type */
#define FTFC_COMMAND_COMPLETE_IRQS \
    {                              \
        FTFC_IRQn                  \
    }
#define FTFC_READ_COLLISION_IRQS \
    {                            \
        FTFC_IRQn                \
    }

/* --------------------------------------------------------------------------
   -- FTFC Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup FTFC_Register_Masks FTFC Register Masks
 * @{
 */

/* FSTAT Bit Fields */
#define FTFC_FSTAT_MGSTAT0_MASK  0x1u
#define FTFC_FSTAT_MGSTAT0_SHIFT 0u
#define FTFC_FSTAT_MGSTAT0_WIDTH 1u
#define FTFC_FSTAT_MGSTAT0(x) \
    (((uint8_t)(((uint8_t)(x)) << FTFC_FSTAT_MGSTAT0_SHIFT)) & FTFC_FSTAT_MGSTAT0_MASK)
#define FTFC_FSTAT_FPVIOL_MASK  0x10u
#define FTFC_FSTAT_FPVIOL_SHIFT 4u
#define FTFC_FSTAT_FPVIOL_WIDTH 1u
#define FTFC_FSTAT_FPVIOL(x) \
    (((uint8_t)(((uint8_t)(x)) << FTFC_FSTAT_FPVIOL_SHIFT)) & FTFC_FSTAT_FPVIOL_MASK)
#define FTFC_FSTAT_ACCERR_MASK  0x20u
#define FTFC_FSTAT_ACCERR_SHIFT 5u
#define FTFC_FSTAT_ACCERR_WIDTH 1u
#define FTFC_FSTAT_ACCERR(x) \
    (((uint8_t)(((uint8_t)(x)) << FTFC_FSTAT_ACCERR_SHIFT)) & FTFC_FSTAT_ACCERR_MASK)
#define FTFC_FSTAT_RDCOLERR_MASK  0x40u
#define FTFC_FSTAT_RDCOLERR_SHIFT 6u
#define FTFC_FSTAT_RDCOLERR_WIDTH 1u
#define FTFC_FSTAT_RDCOLERR(x) \
    (((uint8_t)(((uint8_t)(x)) << FTFC_FSTAT_RDCOLERR_SHIFT)) & FTFC_FSTAT_RDCOLERR_MASK)
#define FTFC_FSTAT_CCIF_MASK  0x80u
#define FTFC_FSTAT_CCIF_SHIFT 7u
#define FTFC_FSTAT_CCIF_WIDTH 1u
#define FTFC_FSTAT_CCIF(x) \
    (((uint8_t)(((uint8_t)(x)) << FTFC_FSTAT_CCIF_SHIFT)) & FTFC_FSTAT_CCIF_MASK)
/* FCNFG Bit Fields */
/* FSEC Bit Fields */
/* FOPT Bit Fields */
/* FCCOB Bit Fields */
#define FTFC_FCCOB_CCOBn_MASK  0xFFu
#define FTFC_FCCOB_CCOBn_SHIFT 0u
#define FTFC_FCCOB_CCOBn_WIDTH 8u
#define FTFC_FCCOB_CCOBn(x) \
    (((uint8_t)(((uint8_t)(x)) << FTFC_FCCOB_CCOBn_SHIFT)) & FTFC_FCCOB_CCOBn_MASK)
/* FPROT Bit Fields */
/* FEPROT Bit Fields */
/* FDPROT Bit Fields */
/* FCSESTAT Bit Fields */
/* FERSTAT Bit Fields */
/* FERCNFG Bit Fields */

/**
 * @}
 */ /* end of group FTFC_Register_Masks */

/**
 * @}
 */ /* end of group FTFC_Peripheral_Access_Layer */

/* --------------------------------------------------------------------------
   -- FTM Peripheral Access Layer
   -------------------------------------------------------------------------- */

/**
 * @addtogroup FTM_Peripheral_Access_Layer FTM Peripheral Access Layer
 * @{
 */

/** FTM - Size of Registers Arrays */
#define FTM_CONTROLS_COUNT  8u
#define FTM_CV_MIRROR_COUNT 8u

/** FTM - Register Layout Typedef */
typedef struct
{
    _IO uint32_t SC;                             /**< offset: 0x0 */
    _IO uint32_t CNT;                            /**< offset: 0x4 */
    _IO uint32_t MOD;                            /**< offset: 0x8 */
    struct
    {                                            /* offset: 0xC, array step: 0x8 */
        _IO uint32_t CnSC;                       /**< offset: 0xC, array step: 0x8 */
        _IO uint32_t CnV;                        /**< offset: 0x10, array step: 0x8 */
    } CONTROLS[FTM_CONTROLS_COUNT];
    _IO uint32_t CNTIN;                          /**< offset: 0x4C */
    _IO uint32_t STATUS;                         /**< offset: 0x50 */
    _IO uint32_t MODE;                           /**< offset: 0x54 */
    _IO uint32_t SYNC;                           /**< offset: 0x58 */
    _IO uint32_t OUTINIT;                        /**< offset: 0x5C */
    _IO uint32_t OUTMASK;                        /**< offset: 0x60 */
    _IO uint32_t COMBINE;                        /**< offset: 0x64 */
    _IO uint32_t DEADTIME;                       /**< offset: 0x68 */
    _IO uint32_t EXTTRIG;                        /**< offset: 0x6C */
    _IO uint32_t POL;                            /**< offset: 0x70 */
    _IO uint32_t FMS;                            /**< offset: 0x74 */
    _IO uint32_t FILTER;                         /**< offset: 0x78 */
    _IO uint32_t FLTCTRL;                        /**< offset: 0x7C */
    _IO uint32_t QDCTRL;                         /**< offset: 0x80 */
    _IO uint32_t CONF;                           /**< offset: 0x84 */
    _IO uint32_t FLTPOL;                         /**< offset: 0x88 */
    _IO uint32_t SYNCONF;                        /**< offset: 0x8C */
    _IO uint32_t INVCTRL;                        /**< offset: 0x90 */
    _IO uint32_t SWOCTRL;                        /**< offset: 0x94 */
    _IO uint32_t PWMLOAD;                        /**< offset: 0x98 */
    _IO uint32_t HCR;                            /**< offset: 0x9C */
    _IO uint32_t PAIR0DEADTIME;                  /**< offset: 0xA0 */
    _NA uint8_t  RESERVED_0[4];
    _IO uint32_t PAIR1DEADTIME;                  /**< offset: 0xA8 */
    _NA uint8_t  RESERVED_1[4];
    _IO uint32_t PAIR2DEADTIME;                  /**< offset: 0xB0 */
    _NA uint8_t  RESERVED_2[4];
    _IO uint32_t PAIR3DEADTIME;                  /**< offset: 0xB8 */
    _NA uint8_t  RESERVED_3[324];
    _IO uint32_t MOD_MIRROR;                     /**< offset: 0x200 */
    _IO uint32_t CV_MIRROR[FTM_CV_MIRROR_COUNT]; /**< offset: 0x204 */
} FTM_t, *FTM_MemMapPtr;

/** Number of instances of the FTM module. */
#define FTM_INSTANCE_COUNT (2u)

/* FTM - Peripheral instance base addresses */
/** Peripheral FTM0 base address */
#define FTM0_BASE (0x40038000u)
/** Peripheral FTM0 base pointer */
#define FTM0 ((FTM_t *)FTM0_BASE)
/** Peripheral FTM1 base address */
#define FTM1_BASE (0x40039000u)
/** Peripheral FTM1 base pointer */
#define FTM1 ((FTM_t *)FTM1_BASE)
/** Array initializer of FTM peripheral base addresses */
#define FTM_BASE_ADDRS       \
    {                        \
        FTM0_BASE, FTM1_BASE \
    }
/** Array initializer of FTM peripheral base pointers */
#define FTM_BASE_PTRS \
    {                 \
        FTM0, FTM1    \
    }
/** Number of interrupt vector arrays for the FTM module. */
#define FTM_IRQS_ARR_COUNT (4u)
/** Number of interrupt channels for the FTM module. */
#define FTM_IRQS_CH_COUNT (8u)
/** Number of interrupt channels for the Fault type of FTM module. */
#define FTM_Fault_IRQS_CH_COUNT (1u)
/** Number of interrupt channels for the Overflow type of FTM module. */
#define FTM_Overflow_IRQS_CH_COUNT (1u)
/** Number of interrupt channels for the Reload type of FTM module. */
#define FTM_Reload_IRQS_CH_COUNT (1u)
/** Interrupt vectors for the FTM peripheral type */
#define FTM_IRQS                                                                                 \
    {                                                                                            \
        {FTM0_Ch0_7_IRQn,                                                                        \
         FTM0_Ch0_7_IRQn,                                                                        \
         FTM0_Ch0_7_IRQn,                                                                        \
         FTM0_Ch0_7_IRQn,                                                                        \
         FTM0_Ch0_7_IRQn,                                                                        \
         FTM0_Ch0_7_IRQn,                                                                        \
         FTM0_Ch0_7_IRQn,                                                                        \
         FTM0_Ch0_7_IRQn},                                                                       \
        {                                                                                        \
            FTM1_Ch0_7_IRQn, FTM1_Ch0_7_IRQn, FTM1_Ch0_7_IRQn, FTM1_Ch0_7_IRQn, FTM1_Ch0_7_IRQn, \
                FTM1_Ch0_7_IRQn, FTM1_Ch0_7_IRQn, FTM1_Ch0_7_IRQn                                \
        }                                                                                        \
    }
#define FTM_Fault_IRQS                   \
    {                                    \
        FTM0_Fault_IRQn, FTM1_Fault_IRQn \
    }
#define FTM_Overflow_IRQS                          \
    {                                              \
        FTM0_Ovf_Reload_IRQn, FTM1_Ovf_Reload_IRQn \
    }
#define FTM_Reload_IRQS                            \
    {                                              \
        FTM0_Ovf_Reload_IRQn, FTM1_Ovf_Reload_IRQn \
    }

/* --------------------------------------------------------------------------
   -- FTM Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup FTM_Register_Masks FTM Register Masks
 * @{
 */

/* SC Bit Fields */
#define FTM_SC_PS_MASK      0x7u
#define FTM_SC_PS_SHIFT     0u
#define FTM_SC_PS_WIDTH     3u
#define FTM_SC_PS(x)        (((uint32_t)(((uint32_t)(x)) << FTM_SC_PS_SHIFT)) & FTM_SC_PS_MASK)
#define FTM_SC_CLKS_MASK    0x18u
#define FTM_SC_CLKS_SHIFT   3u
#define FTM_SC_CLKS_WIDTH   2u
#define FTM_SC_CLKS(x)      (((uint32_t)(((uint32_t)(x)) << FTM_SC_CLKS_SHIFT)) & FTM_SC_CLKS_MASK)
#define FTM_SC_CPWMS_MASK   0x20u
#define FTM_SC_CPWMS_SHIFT  5u
#define FTM_SC_CPWMS_WIDTH  1u
#define FTM_SC_CPWMS(x)     (((uint32_t)(((uint32_t)(x)) << FTM_SC_CPWMS_SHIFT)) & FTM_SC_CPWMS_MASK)
#define FTM_SC_RIE_MASK     0x40u
#define FTM_SC_RIE_SHIFT    6u
#define FTM_SC_RIE_WIDTH    1u
#define FTM_SC_RIE(x)       (((uint32_t)(((uint32_t)(x)) << FTM_SC_RIE_SHIFT)) & FTM_SC_RIE_MASK)
#define FTM_SC_RF_MASK      0x80u
#define FTM_SC_RF_SHIFT     7u
#define FTM_SC_RF_WIDTH     1u
#define FTM_SC_RF(x)        (((uint32_t)(((uint32_t)(x)) << FTM_SC_RF_SHIFT)) & FTM_SC_RF_MASK)
#define FTM_SC_TOIE_MASK    0x100u
#define FTM_SC_TOIE_SHIFT   8u
#define FTM_SC_TOIE_WIDTH   1u
#define FTM_SC_TOIE(x)      (((uint32_t)(((uint32_t)(x)) << FTM_SC_TOIE_SHIFT)) & FTM_SC_TOIE_MASK)
#define FTM_SC_TOF_MASK     0x200u
#define FTM_SC_TOF_SHIFT    9u
#define FTM_SC_TOF_WIDTH    1u
#define FTM_SC_TOF(x)       (((uint32_t)(((uint32_t)(x)) << FTM_SC_TOF_SHIFT)) & FTM_SC_TOF_MASK)
#define FTM_SC_PWMEN0_MASK  0x10000u
#define FTM_SC_PWMEN0_SHIFT 16u
#define FTM_SC_PWMEN0_WIDTH 1u
#define FTM_SC_PWMEN0(x)    (((uint32_t)(((uint32_t)(x)) << FTM_SC_PWMEN0_SHIFT)) & FTM_SC_PWMEN0_MASK)
#define FTM_SC_PWMEN1_MASK  0x20000u
#define FTM_SC_PWMEN1_SHIFT 17u
#define FTM_SC_PWMEN1_WIDTH 1u
#define FTM_SC_PWMEN1(x)    (((uint32_t)(((uint32_t)(x)) << FTM_SC_PWMEN1_SHIFT)) & FTM_SC_PWMEN1_MASK)
#define FTM_SC_PWMEN2_MASK  0x40000u
#define FTM_SC_PWMEN2_SHIFT 18u
#define FTM_SC_PWMEN2_WIDTH 1u
#define FTM_SC_PWMEN2(x)    (((uint32_t)(((uint32_t)(x)) << FTM_SC_PWMEN2_SHIFT)) & FTM_SC_PWMEN2_MASK)
#define FTM_SC_PWMEN3_MASK  0x80000u
#define FTM_SC_PWMEN3_SHIFT 19u
#define FTM_SC_PWMEN3_WIDTH 1u
#define FTM_SC_PWMEN3(x)    (((uint32_t)(((uint32_t)(x)) << FTM_SC_PWMEN3_SHIFT)) & FTM_SC_PWMEN3_MASK)
#define FTM_SC_PWMEN4_MASK  0x100000u
#define FTM_SC_PWMEN4_SHIFT 20u
#define FTM_SC_PWMEN4_WIDTH 1u
#define FTM_SC_PWMEN4(x)    (((uint32_t)(((uint32_t)(x)) << FTM_SC_PWMEN4_SHIFT)) & FTM_SC_PWMEN4_MASK)
#define FTM_SC_PWMEN5_MASK  0x200000u
#define FTM_SC_PWMEN5_SHIFT 21u
#define FTM_SC_PWMEN5_WIDTH 1u
#define FTM_SC_PWMEN5(x)    (((uint32_t)(((uint32_t)(x)) << FTM_SC_PWMEN5_SHIFT)) & FTM_SC_PWMEN5_MASK)
#define FTM_SC_PWMEN6_MASK  0x400000u
#define FTM_SC_PWMEN6_SHIFT 22u
#define FTM_SC_PWMEN6_WIDTH 1u
#define FTM_SC_PWMEN6(x)    (((uint32_t)(((uint32_t)(x)) << FTM_SC_PWMEN6_SHIFT)) & FTM_SC_PWMEN6_MASK)
#define FTM_SC_PWMEN7_MASK  0x800000u
#define FTM_SC_PWMEN7_SHIFT 23u
#define FTM_SC_PWMEN7_WIDTH 1u
#define FTM_SC_PWMEN7(x)    (((uint32_t)(((uint32_t)(x)) << FTM_SC_PWMEN7_SHIFT)) & FTM_SC_PWMEN7_MASK)
#define FTM_SC_FLTPS_MASK   0xF000000u
#define FTM_SC_FLTPS_SHIFT  24u
#define FTM_SC_FLTPS_WIDTH  4u
#define FTM_SC_FLTPS(x)     (((uint32_t)(((uint32_t)(x)) << FTM_SC_FLTPS_SHIFT)) & FTM_SC_FLTPS_MASK)
/* CNT Bit Fields */
#define FTM_CNT_COUNT_MASK  0xFFFFu
#define FTM_CNT_COUNT_SHIFT 0u
#define FTM_CNT_COUNT_WIDTH 16u
#define FTM_CNT_COUNT(x)    (((uint32_t)(((uint32_t)(x)) << FTM_CNT_COUNT_SHIFT)) & FTM_CNT_COUNT_MASK)
/* MOD Bit Fields */
#define FTM_MOD_MOD_MASK  0xFFFFu
#define FTM_MOD_MOD_SHIFT 0u
#define FTM_MOD_MOD_WIDTH 16u
#define FTM_MOD_MOD(x)    (((uint32_t)(((uint32_t)(x)) << FTM_MOD_MOD_SHIFT)) & FTM_MOD_MOD_MASK)
/* CnSC Bit Fields */
#define FTM_CnSC_DMA_MASK    0x1u
#define FTM_CnSC_DMA_SHIFT   0u
#define FTM_CnSC_DMA_WIDTH   1u
#define FTM_CnSC_DMA(x)      (((uint32_t)(((uint32_t)(x)) << FTM_CnSC_DMA_SHIFT)) & FTM_CnSC_DMA_MASK)
#define FTM_CnSC_ICRST_MASK  0x2u
#define FTM_CnSC_ICRST_SHIFT 1u
#define FTM_CnSC_ICRST_WIDTH 1u
#define FTM_CnSC_ICRST(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_CnSC_ICRST_SHIFT)) & FTM_CnSC_ICRST_MASK)
#define FTM_CnSC_ELSA_MASK      0x4u
#define FTM_CnSC_ELSA_SHIFT     2u
#define FTM_CnSC_ELSA_WIDTH     1u
#define FTM_CnSC_ELSA(x)        (((uint32_t)(((uint32_t)(x)) << FTM_CnSC_ELSA_SHIFT)) & FTM_CnSC_ELSA_MASK)
#define FTM_CnSC_ELSB_MASK      0x8u
#define FTM_CnSC_ELSB_SHIFT     3u
#define FTM_CnSC_ELSB_WIDTH     1u
#define FTM_CnSC_ELSB(x)        (((uint32_t)(((uint32_t)(x)) << FTM_CnSC_ELSB_SHIFT)) & FTM_CnSC_ELSB_MASK)
#define FTM_CnSC_MSA_MASK       0x10u
#define FTM_CnSC_MSA_SHIFT      4u
#define FTM_CnSC_MSA_WIDTH      1u
#define FTM_CnSC_MSA(x)         (((uint32_t)(((uint32_t)(x)) << FTM_CnSC_MSA_SHIFT)) & FTM_CnSC_MSA_MASK)
#define FTM_CnSC_MSB_MASK       0x20u
#define FTM_CnSC_MSB_SHIFT      5u
#define FTM_CnSC_MSB_WIDTH      1u
#define FTM_CnSC_MSB(x)         (((uint32_t)(((uint32_t)(x)) << FTM_CnSC_MSB_SHIFT)) & FTM_CnSC_MSB_MASK)
#define FTM_CnSC_CHIE_MASK      0x40u
#define FTM_CnSC_CHIE_SHIFT     6u
#define FTM_CnSC_CHIE_WIDTH     1u
#define FTM_CnSC_CHIE(x)        (((uint32_t)(((uint32_t)(x)) << FTM_CnSC_CHIE_SHIFT)) & FTM_CnSC_CHIE_MASK)
#define FTM_CnSC_CHF_MASK       0x80u
#define FTM_CnSC_CHF_SHIFT      7u
#define FTM_CnSC_CHF_WIDTH      1u
#define FTM_CnSC_CHF(x)         (((uint32_t)(((uint32_t)(x)) << FTM_CnSC_CHF_SHIFT)) & FTM_CnSC_CHF_MASK)
#define FTM_CnSC_TRIGMODE_MASK  0x100u
#define FTM_CnSC_TRIGMODE_SHIFT 8u
#define FTM_CnSC_TRIGMODE_WIDTH 1u
#define FTM_CnSC_TRIGMODE(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_CnSC_TRIGMODE_SHIFT)) & FTM_CnSC_TRIGMODE_MASK)
#define FTM_CnSC_CHIS_MASK  0x200u
#define FTM_CnSC_CHIS_SHIFT 9u
#define FTM_CnSC_CHIS_WIDTH 1u
#define FTM_CnSC_CHIS(x)    (((uint32_t)(((uint32_t)(x)) << FTM_CnSC_CHIS_SHIFT)) & FTM_CnSC_CHIS_MASK)
#define FTM_CnSC_CHOV_MASK  0x400u
#define FTM_CnSC_CHOV_SHIFT 10u
#define FTM_CnSC_CHOV_WIDTH 1u
#define FTM_CnSC_CHOV(x)    (((uint32_t)(((uint32_t)(x)) << FTM_CnSC_CHOV_SHIFT)) & FTM_CnSC_CHOV_MASK)
/* CnV Bit Fields */
#define FTM_CnV_VAL_MASK  0xFFFFu
#define FTM_CnV_VAL_SHIFT 0u
#define FTM_CnV_VAL_WIDTH 16u
#define FTM_CnV_VAL(x)    (((uint32_t)(((uint32_t)(x)) << FTM_CnV_VAL_SHIFT)) & FTM_CnV_VAL_MASK)
/* CNTIN Bit Fields */
#define FTM_CNTIN_INIT_MASK  0xFFFFu
#define FTM_CNTIN_INIT_SHIFT 0u
#define FTM_CNTIN_INIT_WIDTH 16u
#define FTM_CNTIN_INIT(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_CNTIN_INIT_SHIFT)) & FTM_CNTIN_INIT_MASK)
/* STATUS Bit Fields */
#define FTM_STATUS_CH0F_MASK  0x1u
#define FTM_STATUS_CH0F_SHIFT 0u
#define FTM_STATUS_CH0F_WIDTH 1u
#define FTM_STATUS_CH0F(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_STATUS_CH0F_SHIFT)) & FTM_STATUS_CH0F_MASK)
#define FTM_STATUS_CH1F_MASK  0x2u
#define FTM_STATUS_CH1F_SHIFT 1u
#define FTM_STATUS_CH1F_WIDTH 1u
#define FTM_STATUS_CH1F(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_STATUS_CH1F_SHIFT)) & FTM_STATUS_CH1F_MASK)
#define FTM_STATUS_CH2F_MASK  0x4u
#define FTM_STATUS_CH2F_SHIFT 2u
#define FTM_STATUS_CH2F_WIDTH 1u
#define FTM_STATUS_CH2F(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_STATUS_CH2F_SHIFT)) & FTM_STATUS_CH2F_MASK)
#define FTM_STATUS_CH3F_MASK  0x8u
#define FTM_STATUS_CH3F_SHIFT 3u
#define FTM_STATUS_CH3F_WIDTH 1u
#define FTM_STATUS_CH3F(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_STATUS_CH3F_SHIFT)) & FTM_STATUS_CH3F_MASK)
#define FTM_STATUS_CH4F_MASK  0x10u
#define FTM_STATUS_CH4F_SHIFT 4u
#define FTM_STATUS_CH4F_WIDTH 1u
#define FTM_STATUS_CH4F(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_STATUS_CH4F_SHIFT)) & FTM_STATUS_CH4F_MASK)
#define FTM_STATUS_CH5F_MASK  0x20u
#define FTM_STATUS_CH5F_SHIFT 5u
#define FTM_STATUS_CH5F_WIDTH 1u
#define FTM_STATUS_CH5F(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_STATUS_CH5F_SHIFT)) & FTM_STATUS_CH5F_MASK)
#define FTM_STATUS_CH6F_MASK  0x40u
#define FTM_STATUS_CH6F_SHIFT 6u
#define FTM_STATUS_CH6F_WIDTH 1u
#define FTM_STATUS_CH6F(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_STATUS_CH6F_SHIFT)) & FTM_STATUS_CH6F_MASK)
#define FTM_STATUS_CH7F_MASK  0x80u
#define FTM_STATUS_CH7F_SHIFT 7u
#define FTM_STATUS_CH7F_WIDTH 1u
#define FTM_STATUS_CH7F(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_STATUS_CH7F_SHIFT)) & FTM_STATUS_CH7F_MASK)
/* MODE Bit Fields */
#define FTM_MODE_FTMEN_MASK  0x1u
#define FTM_MODE_FTMEN_SHIFT 0u
#define FTM_MODE_FTMEN_WIDTH 1u
#define FTM_MODE_FTMEN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_MODE_FTMEN_SHIFT)) & FTM_MODE_FTMEN_MASK)
#define FTM_MODE_INIT_MASK   0x2u
#define FTM_MODE_INIT_SHIFT  1u
#define FTM_MODE_INIT_WIDTH  1u
#define FTM_MODE_INIT(x)     (((uint32_t)(((uint32_t)(x)) << FTM_MODE_INIT_SHIFT)) & FTM_MODE_INIT_MASK)
#define FTM_MODE_WPDIS_MASK  0x4u
#define FTM_MODE_WPDIS_SHIFT 2u
#define FTM_MODE_WPDIS_WIDTH 1u
#define FTM_MODE_WPDIS(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_MODE_WPDIS_SHIFT)) & FTM_MODE_WPDIS_MASK)
#define FTM_MODE_PWMSYNC_MASK  0x8u
#define FTM_MODE_PWMSYNC_SHIFT 3u
#define FTM_MODE_PWMSYNC_WIDTH 1u
#define FTM_MODE_PWMSYNC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_MODE_PWMSYNC_SHIFT)) & FTM_MODE_PWMSYNC_MASK)
#define FTM_MODE_CAPTEST_MASK  0x10u
#define FTM_MODE_CAPTEST_SHIFT 4u
#define FTM_MODE_CAPTEST_WIDTH 1u
#define FTM_MODE_CAPTEST(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_MODE_CAPTEST_SHIFT)) & FTM_MODE_CAPTEST_MASK)
#define FTM_MODE_FAULTM_MASK  0x60u
#define FTM_MODE_FAULTM_SHIFT 5u
#define FTM_MODE_FAULTM_WIDTH 2u
#define FTM_MODE_FAULTM(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_MODE_FAULTM_SHIFT)) & FTM_MODE_FAULTM_MASK)
#define FTM_MODE_FAULTIE_MASK  0x80u
#define FTM_MODE_FAULTIE_SHIFT 7u
#define FTM_MODE_FAULTIE_WIDTH 1u
#define FTM_MODE_FAULTIE(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_MODE_FAULTIE_SHIFT)) & FTM_MODE_FAULTIE_MASK)
/* SYNC Bit Fields */
#define FTM_SYNC_CNTMIN_MASK  0x1u
#define FTM_SYNC_CNTMIN_SHIFT 0u
#define FTM_SYNC_CNTMIN_WIDTH 1u
#define FTM_SYNC_CNTMIN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNC_CNTMIN_SHIFT)) & FTM_SYNC_CNTMIN_MASK)
#define FTM_SYNC_CNTMAX_MASK  0x2u
#define FTM_SYNC_CNTMAX_SHIFT 1u
#define FTM_SYNC_CNTMAX_WIDTH 1u
#define FTM_SYNC_CNTMAX(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNC_CNTMAX_SHIFT)) & FTM_SYNC_CNTMAX_MASK)
#define FTM_SYNC_REINIT_MASK  0x4u
#define FTM_SYNC_REINIT_SHIFT 2u
#define FTM_SYNC_REINIT_WIDTH 1u
#define FTM_SYNC_REINIT(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNC_REINIT_SHIFT)) & FTM_SYNC_REINIT_MASK)
#define FTM_SYNC_SYNCHOM_MASK  0x8u
#define FTM_SYNC_SYNCHOM_SHIFT 3u
#define FTM_SYNC_SYNCHOM_WIDTH 1u
#define FTM_SYNC_SYNCHOM(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNC_SYNCHOM_SHIFT)) & FTM_SYNC_SYNCHOM_MASK)
#define FTM_SYNC_TRIG0_MASK  0x10u
#define FTM_SYNC_TRIG0_SHIFT 4u
#define FTM_SYNC_TRIG0_WIDTH 1u
#define FTM_SYNC_TRIG0(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNC_TRIG0_SHIFT)) & FTM_SYNC_TRIG0_MASK)
#define FTM_SYNC_TRIG1_MASK  0x20u
#define FTM_SYNC_TRIG1_SHIFT 5u
#define FTM_SYNC_TRIG1_WIDTH 1u
#define FTM_SYNC_TRIG1(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNC_TRIG1_SHIFT)) & FTM_SYNC_TRIG1_MASK)
#define FTM_SYNC_TRIG2_MASK  0x40u
#define FTM_SYNC_TRIG2_SHIFT 6u
#define FTM_SYNC_TRIG2_WIDTH 1u
#define FTM_SYNC_TRIG2(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNC_TRIG2_SHIFT)) & FTM_SYNC_TRIG2_MASK)
#define FTM_SYNC_SWSYNC_MASK  0x80u
#define FTM_SYNC_SWSYNC_SHIFT 7u
#define FTM_SYNC_SWSYNC_WIDTH 1u
#define FTM_SYNC_SWSYNC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNC_SWSYNC_SHIFT)) & FTM_SYNC_SWSYNC_MASK)
/* OUTINIT Bit Fields */
#define FTM_OUTINIT_CH0OI_MASK  0x1u
#define FTM_OUTINIT_CH0OI_SHIFT 0u
#define FTM_OUTINIT_CH0OI_WIDTH 1u
#define FTM_OUTINIT_CH0OI(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTINIT_CH0OI_SHIFT)) & FTM_OUTINIT_CH0OI_MASK)
#define FTM_OUTINIT_CH1OI_MASK  0x2u
#define FTM_OUTINIT_CH1OI_SHIFT 1u
#define FTM_OUTINIT_CH1OI_WIDTH 1u
#define FTM_OUTINIT_CH1OI(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTINIT_CH1OI_SHIFT)) & FTM_OUTINIT_CH1OI_MASK)
#define FTM_OUTINIT_CH2OI_MASK  0x4u
#define FTM_OUTINIT_CH2OI_SHIFT 2u
#define FTM_OUTINIT_CH2OI_WIDTH 1u
#define FTM_OUTINIT_CH2OI(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTINIT_CH2OI_SHIFT)) & FTM_OUTINIT_CH2OI_MASK)
#define FTM_OUTINIT_CH3OI_MASK  0x8u
#define FTM_OUTINIT_CH3OI_SHIFT 3u
#define FTM_OUTINIT_CH3OI_WIDTH 1u
#define FTM_OUTINIT_CH3OI(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTINIT_CH3OI_SHIFT)) & FTM_OUTINIT_CH3OI_MASK)
#define FTM_OUTINIT_CH4OI_MASK  0x10u
#define FTM_OUTINIT_CH4OI_SHIFT 4u
#define FTM_OUTINIT_CH4OI_WIDTH 1u
#define FTM_OUTINIT_CH4OI(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTINIT_CH4OI_SHIFT)) & FTM_OUTINIT_CH4OI_MASK)
#define FTM_OUTINIT_CH5OI_MASK  0x20u
#define FTM_OUTINIT_CH5OI_SHIFT 5u
#define FTM_OUTINIT_CH5OI_WIDTH 1u
#define FTM_OUTINIT_CH5OI(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTINIT_CH5OI_SHIFT)) & FTM_OUTINIT_CH5OI_MASK)
#define FTM_OUTINIT_CH6OI_MASK  0x40u
#define FTM_OUTINIT_CH6OI_SHIFT 6u
#define FTM_OUTINIT_CH6OI_WIDTH 1u
#define FTM_OUTINIT_CH6OI(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTINIT_CH6OI_SHIFT)) & FTM_OUTINIT_CH6OI_MASK)
#define FTM_OUTINIT_CH7OI_MASK  0x80u
#define FTM_OUTINIT_CH7OI_SHIFT 7u
#define FTM_OUTINIT_CH7OI_WIDTH 1u
#define FTM_OUTINIT_CH7OI(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTINIT_CH7OI_SHIFT)) & FTM_OUTINIT_CH7OI_MASK)
/* OUTMASK Bit Fields */
#define FTM_OUTMASK_CH0OM_MASK  0x1u
#define FTM_OUTMASK_CH0OM_SHIFT 0u
#define FTM_OUTMASK_CH0OM_WIDTH 1u
#define FTM_OUTMASK_CH0OM(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTMASK_CH0OM_SHIFT)) & FTM_OUTMASK_CH0OM_MASK)
#define FTM_OUTMASK_CH1OM_MASK  0x2u
#define FTM_OUTMASK_CH1OM_SHIFT 1u
#define FTM_OUTMASK_CH1OM_WIDTH 1u
#define FTM_OUTMASK_CH1OM(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTMASK_CH1OM_SHIFT)) & FTM_OUTMASK_CH1OM_MASK)
#define FTM_OUTMASK_CH2OM_MASK  0x4u
#define FTM_OUTMASK_CH2OM_SHIFT 2u
#define FTM_OUTMASK_CH2OM_WIDTH 1u
#define FTM_OUTMASK_CH2OM(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTMASK_CH2OM_SHIFT)) & FTM_OUTMASK_CH2OM_MASK)
#define FTM_OUTMASK_CH3OM_MASK  0x8u
#define FTM_OUTMASK_CH3OM_SHIFT 3u
#define FTM_OUTMASK_CH3OM_WIDTH 1u
#define FTM_OUTMASK_CH3OM(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTMASK_CH3OM_SHIFT)) & FTM_OUTMASK_CH3OM_MASK)
#define FTM_OUTMASK_CH4OM_MASK  0x10u
#define FTM_OUTMASK_CH4OM_SHIFT 4u
#define FTM_OUTMASK_CH4OM_WIDTH 1u
#define FTM_OUTMASK_CH4OM(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTMASK_CH4OM_SHIFT)) & FTM_OUTMASK_CH4OM_MASK)
#define FTM_OUTMASK_CH5OM_MASK  0x20u
#define FTM_OUTMASK_CH5OM_SHIFT 5u
#define FTM_OUTMASK_CH5OM_WIDTH 1u
#define FTM_OUTMASK_CH5OM(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTMASK_CH5OM_SHIFT)) & FTM_OUTMASK_CH5OM_MASK)
#define FTM_OUTMASK_CH6OM_MASK  0x40u
#define FTM_OUTMASK_CH6OM_SHIFT 6u
#define FTM_OUTMASK_CH6OM_WIDTH 1u
#define FTM_OUTMASK_CH6OM(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTMASK_CH6OM_SHIFT)) & FTM_OUTMASK_CH6OM_MASK)
#define FTM_OUTMASK_CH7OM_MASK  0x80u
#define FTM_OUTMASK_CH7OM_SHIFT 7u
#define FTM_OUTMASK_CH7OM_WIDTH 1u
#define FTM_OUTMASK_CH7OM(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_OUTMASK_CH7OM_SHIFT)) & FTM_OUTMASK_CH7OM_MASK)
/* COMBINE Bit Fields */
#define FTM_COMBINE_COMBINE0_MASK  0x1u
#define FTM_COMBINE_COMBINE0_SHIFT 0u
#define FTM_COMBINE_COMBINE0_WIDTH 1u
#define FTM_COMBINE_COMBINE0(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_COMBINE0_SHIFT)) & FTM_COMBINE_COMBINE0_MASK)
#define FTM_COMBINE_COMP0_MASK  0x2u
#define FTM_COMBINE_COMP0_SHIFT 1u
#define FTM_COMBINE_COMP0_WIDTH 1u
#define FTM_COMBINE_COMP0(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_COMP0_SHIFT)) & FTM_COMBINE_COMP0_MASK)
#define FTM_COMBINE_DECAPEN0_MASK  0x4u
#define FTM_COMBINE_DECAPEN0_SHIFT 2u
#define FTM_COMBINE_DECAPEN0_WIDTH 1u
#define FTM_COMBINE_DECAPEN0(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_DECAPEN0_SHIFT)) & FTM_COMBINE_DECAPEN0_MASK)
#define FTM_COMBINE_DECAP0_MASK  0x8u
#define FTM_COMBINE_DECAP0_SHIFT 3u
#define FTM_COMBINE_DECAP0_WIDTH 1u
#define FTM_COMBINE_DECAP0(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_DECAP0_SHIFT)) & FTM_COMBINE_DECAP0_MASK)
#define FTM_COMBINE_DTEN0_MASK  0x10u
#define FTM_COMBINE_DTEN0_SHIFT 4u
#define FTM_COMBINE_DTEN0_WIDTH 1u
#define FTM_COMBINE_DTEN0(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_DTEN0_SHIFT)) & FTM_COMBINE_DTEN0_MASK)
#define FTM_COMBINE_SYNCEN0_MASK  0x20u
#define FTM_COMBINE_SYNCEN0_SHIFT 5u
#define FTM_COMBINE_SYNCEN0_WIDTH 1u
#define FTM_COMBINE_SYNCEN0(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_SYNCEN0_SHIFT)) & FTM_COMBINE_SYNCEN0_MASK)
#define FTM_COMBINE_FAULTEN0_MASK  0x40u
#define FTM_COMBINE_FAULTEN0_SHIFT 6u
#define FTM_COMBINE_FAULTEN0_WIDTH 1u
#define FTM_COMBINE_FAULTEN0(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_FAULTEN0_SHIFT)) & FTM_COMBINE_FAULTEN0_MASK)
#define FTM_COMBINE_MCOMBINE0_MASK  0x80u
#define FTM_COMBINE_MCOMBINE0_SHIFT 7u
#define FTM_COMBINE_MCOMBINE0_WIDTH 1u
#define FTM_COMBINE_MCOMBINE0(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_MCOMBINE0_SHIFT)) & FTM_COMBINE_MCOMBINE0_MASK)
#define FTM_COMBINE_COMBINE1_MASK  0x100u
#define FTM_COMBINE_COMBINE1_SHIFT 8u
#define FTM_COMBINE_COMBINE1_WIDTH 1u
#define FTM_COMBINE_COMBINE1(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_COMBINE1_SHIFT)) & FTM_COMBINE_COMBINE1_MASK)
#define FTM_COMBINE_COMP1_MASK  0x200u
#define FTM_COMBINE_COMP1_SHIFT 9u
#define FTM_COMBINE_COMP1_WIDTH 1u
#define FTM_COMBINE_COMP1(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_COMP1_SHIFT)) & FTM_COMBINE_COMP1_MASK)
#define FTM_COMBINE_DECAPEN1_MASK  0x400u
#define FTM_COMBINE_DECAPEN1_SHIFT 10u
#define FTM_COMBINE_DECAPEN1_WIDTH 1u
#define FTM_COMBINE_DECAPEN1(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_DECAPEN1_SHIFT)) & FTM_COMBINE_DECAPEN1_MASK)
#define FTM_COMBINE_DECAP1_MASK  0x800u
#define FTM_COMBINE_DECAP1_SHIFT 11u
#define FTM_COMBINE_DECAP1_WIDTH 1u
#define FTM_COMBINE_DECAP1(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_DECAP1_SHIFT)) & FTM_COMBINE_DECAP1_MASK)
#define FTM_COMBINE_DTEN1_MASK  0x1000u
#define FTM_COMBINE_DTEN1_SHIFT 12u
#define FTM_COMBINE_DTEN1_WIDTH 1u
#define FTM_COMBINE_DTEN1(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_DTEN1_SHIFT)) & FTM_COMBINE_DTEN1_MASK)
#define FTM_COMBINE_SYNCEN1_MASK  0x2000u
#define FTM_COMBINE_SYNCEN1_SHIFT 13u
#define FTM_COMBINE_SYNCEN1_WIDTH 1u
#define FTM_COMBINE_SYNCEN1(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_SYNCEN1_SHIFT)) & FTM_COMBINE_SYNCEN1_MASK)
#define FTM_COMBINE_FAULTEN1_MASK  0x4000u
#define FTM_COMBINE_FAULTEN1_SHIFT 14u
#define FTM_COMBINE_FAULTEN1_WIDTH 1u
#define FTM_COMBINE_FAULTEN1(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_FAULTEN1_SHIFT)) & FTM_COMBINE_FAULTEN1_MASK)
#define FTM_COMBINE_MCOMBINE1_MASK  0x8000u
#define FTM_COMBINE_MCOMBINE1_SHIFT 15u
#define FTM_COMBINE_MCOMBINE1_WIDTH 1u
#define FTM_COMBINE_MCOMBINE1(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_MCOMBINE1_SHIFT)) & FTM_COMBINE_MCOMBINE1_MASK)
#define FTM_COMBINE_COMBINE2_MASK  0x10000u
#define FTM_COMBINE_COMBINE2_SHIFT 16u
#define FTM_COMBINE_COMBINE2_WIDTH 1u
#define FTM_COMBINE_COMBINE2(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_COMBINE2_SHIFT)) & FTM_COMBINE_COMBINE2_MASK)
#define FTM_COMBINE_COMP2_MASK  0x20000u
#define FTM_COMBINE_COMP2_SHIFT 17u
#define FTM_COMBINE_COMP2_WIDTH 1u
#define FTM_COMBINE_COMP2(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_COMP2_SHIFT)) & FTM_COMBINE_COMP2_MASK)
#define FTM_COMBINE_DECAPEN2_MASK  0x40000u
#define FTM_COMBINE_DECAPEN2_SHIFT 18u
#define FTM_COMBINE_DECAPEN2_WIDTH 1u
#define FTM_COMBINE_DECAPEN2(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_DECAPEN2_SHIFT)) & FTM_COMBINE_DECAPEN2_MASK)
#define FTM_COMBINE_DECAP2_MASK  0x80000u
#define FTM_COMBINE_DECAP2_SHIFT 19u
#define FTM_COMBINE_DECAP2_WIDTH 1u
#define FTM_COMBINE_DECAP2(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_DECAP2_SHIFT)) & FTM_COMBINE_DECAP2_MASK)
#define FTM_COMBINE_DTEN2_MASK  0x100000u
#define FTM_COMBINE_DTEN2_SHIFT 20u
#define FTM_COMBINE_DTEN2_WIDTH 1u
#define FTM_COMBINE_DTEN2(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_DTEN2_SHIFT)) & FTM_COMBINE_DTEN2_MASK)
#define FTM_COMBINE_SYNCEN2_MASK  0x200000u
#define FTM_COMBINE_SYNCEN2_SHIFT 21u
#define FTM_COMBINE_SYNCEN2_WIDTH 1u
#define FTM_COMBINE_SYNCEN2(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_SYNCEN2_SHIFT)) & FTM_COMBINE_SYNCEN2_MASK)
#define FTM_COMBINE_FAULTEN2_MASK  0x400000u
#define FTM_COMBINE_FAULTEN2_SHIFT 22u
#define FTM_COMBINE_FAULTEN2_WIDTH 1u
#define FTM_COMBINE_FAULTEN2(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_FAULTEN2_SHIFT)) & FTM_COMBINE_FAULTEN2_MASK)
#define FTM_COMBINE_MCOMBINE2_MASK  0x800000u
#define FTM_COMBINE_MCOMBINE2_SHIFT 23u
#define FTM_COMBINE_MCOMBINE2_WIDTH 1u
#define FTM_COMBINE_MCOMBINE2(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_MCOMBINE2_SHIFT)) & FTM_COMBINE_MCOMBINE2_MASK)
#define FTM_COMBINE_COMBINE3_MASK  0x1000000u
#define FTM_COMBINE_COMBINE3_SHIFT 24u
#define FTM_COMBINE_COMBINE3_WIDTH 1u
#define FTM_COMBINE_COMBINE3(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_COMBINE3_SHIFT)) & FTM_COMBINE_COMBINE3_MASK)
#define FTM_COMBINE_COMP3_MASK  0x2000000u
#define FTM_COMBINE_COMP3_SHIFT 25u
#define FTM_COMBINE_COMP3_WIDTH 1u
#define FTM_COMBINE_COMP3(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_COMP3_SHIFT)) & FTM_COMBINE_COMP3_MASK)
#define FTM_COMBINE_DECAPEN3_MASK  0x4000000u
#define FTM_COMBINE_DECAPEN3_SHIFT 26u
#define FTM_COMBINE_DECAPEN3_WIDTH 1u
#define FTM_COMBINE_DECAPEN3(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_DECAPEN3_SHIFT)) & FTM_COMBINE_DECAPEN3_MASK)
#define FTM_COMBINE_DECAP3_MASK  0x8000000u
#define FTM_COMBINE_DECAP3_SHIFT 27u
#define FTM_COMBINE_DECAP3_WIDTH 1u
#define FTM_COMBINE_DECAP3(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_DECAP3_SHIFT)) & FTM_COMBINE_DECAP3_MASK)
#define FTM_COMBINE_DTEN3_MASK  0x10000000u
#define FTM_COMBINE_DTEN3_SHIFT 28u
#define FTM_COMBINE_DTEN3_WIDTH 1u
#define FTM_COMBINE_DTEN3(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_DTEN3_SHIFT)) & FTM_COMBINE_DTEN3_MASK)
#define FTM_COMBINE_SYNCEN3_MASK  0x20000000u
#define FTM_COMBINE_SYNCEN3_SHIFT 29u
#define FTM_COMBINE_SYNCEN3_WIDTH 1u
#define FTM_COMBINE_SYNCEN3(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_SYNCEN3_SHIFT)) & FTM_COMBINE_SYNCEN3_MASK)
#define FTM_COMBINE_FAULTEN3_MASK  0x40000000u
#define FTM_COMBINE_FAULTEN3_SHIFT 30u
#define FTM_COMBINE_FAULTEN3_WIDTH 1u
#define FTM_COMBINE_FAULTEN3(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_FAULTEN3_SHIFT)) & FTM_COMBINE_FAULTEN3_MASK)
#define FTM_COMBINE_MCOMBINE3_MASK  0x80000000u
#define FTM_COMBINE_MCOMBINE3_SHIFT 31u
#define FTM_COMBINE_MCOMBINE3_WIDTH 1u
#define FTM_COMBINE_MCOMBINE3(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_COMBINE_MCOMBINE3_SHIFT)) & FTM_COMBINE_MCOMBINE3_MASK)
/* DEADTIME Bit Fields */
#define FTM_DEADTIME_DTVAL_MASK  0x3Fu
#define FTM_DEADTIME_DTVAL_SHIFT 0u
#define FTM_DEADTIME_DTVAL_WIDTH 6u
#define FTM_DEADTIME_DTVAL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_DEADTIME_DTVAL_SHIFT)) & FTM_DEADTIME_DTVAL_MASK)
#define FTM_DEADTIME_DTPS_MASK  0xC0u
#define FTM_DEADTIME_DTPS_SHIFT 6u
#define FTM_DEADTIME_DTPS_WIDTH 2u
#define FTM_DEADTIME_DTPS(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_DEADTIME_DTPS_SHIFT)) & FTM_DEADTIME_DTPS_MASK)
#define FTM_DEADTIME_DTVALEX_MASK  0xF0000u
#define FTM_DEADTIME_DTVALEX_SHIFT 16u
#define FTM_DEADTIME_DTVALEX_WIDTH 4u
#define FTM_DEADTIME_DTVALEX(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_DEADTIME_DTVALEX_SHIFT)) & FTM_DEADTIME_DTVALEX_MASK)
/* EXTTRIG Bit Fields */
#define FTM_EXTTRIG_CH2TRIG_MASK  0x1u
#define FTM_EXTTRIG_CH2TRIG_SHIFT 0u
#define FTM_EXTTRIG_CH2TRIG_WIDTH 1u
#define FTM_EXTTRIG_CH2TRIG(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_EXTTRIG_CH2TRIG_SHIFT)) & FTM_EXTTRIG_CH2TRIG_MASK)
#define FTM_EXTTRIG_CH3TRIG_MASK  0x2u
#define FTM_EXTTRIG_CH3TRIG_SHIFT 1u
#define FTM_EXTTRIG_CH3TRIG_WIDTH 1u
#define FTM_EXTTRIG_CH3TRIG(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_EXTTRIG_CH3TRIG_SHIFT)) & FTM_EXTTRIG_CH3TRIG_MASK)
#define FTM_EXTTRIG_CH4TRIG_MASK  0x4u
#define FTM_EXTTRIG_CH4TRIG_SHIFT 2u
#define FTM_EXTTRIG_CH4TRIG_WIDTH 1u
#define FTM_EXTTRIG_CH4TRIG(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_EXTTRIG_CH4TRIG_SHIFT)) & FTM_EXTTRIG_CH4TRIG_MASK)
#define FTM_EXTTRIG_CH5TRIG_MASK  0x8u
#define FTM_EXTTRIG_CH5TRIG_SHIFT 3u
#define FTM_EXTTRIG_CH5TRIG_WIDTH 1u
#define FTM_EXTTRIG_CH5TRIG(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_EXTTRIG_CH5TRIG_SHIFT)) & FTM_EXTTRIG_CH5TRIG_MASK)
#define FTM_EXTTRIG_CH0TRIG_MASK  0x10u
#define FTM_EXTTRIG_CH0TRIG_SHIFT 4u
#define FTM_EXTTRIG_CH0TRIG_WIDTH 1u
#define FTM_EXTTRIG_CH0TRIG(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_EXTTRIG_CH0TRIG_SHIFT)) & FTM_EXTTRIG_CH0TRIG_MASK)
#define FTM_EXTTRIG_CH1TRIG_MASK  0x20u
#define FTM_EXTTRIG_CH1TRIG_SHIFT 5u
#define FTM_EXTTRIG_CH1TRIG_WIDTH 1u
#define FTM_EXTTRIG_CH1TRIG(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_EXTTRIG_CH1TRIG_SHIFT)) & FTM_EXTTRIG_CH1TRIG_MASK)
#define FTM_EXTTRIG_INITTRIGEN_MASK  0x40u
#define FTM_EXTTRIG_INITTRIGEN_SHIFT 6u
#define FTM_EXTTRIG_INITTRIGEN_WIDTH 1u
#define FTM_EXTTRIG_INITTRIGEN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_EXTTRIG_INITTRIGEN_SHIFT)) & FTM_EXTTRIG_INITTRIGEN_MASK)
#define FTM_EXTTRIG_TRIGF_MASK  0x80u
#define FTM_EXTTRIG_TRIGF_SHIFT 7u
#define FTM_EXTTRIG_TRIGF_WIDTH 1u
#define FTM_EXTTRIG_TRIGF(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_EXTTRIG_TRIGF_SHIFT)) & FTM_EXTTRIG_TRIGF_MASK)
#define FTM_EXTTRIG_CH6TRIG_MASK  0x100u
#define FTM_EXTTRIG_CH6TRIG_SHIFT 8u
#define FTM_EXTTRIG_CH6TRIG_WIDTH 1u
#define FTM_EXTTRIG_CH6TRIG(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_EXTTRIG_CH6TRIG_SHIFT)) & FTM_EXTTRIG_CH6TRIG_MASK)
#define FTM_EXTTRIG_CH7TRIG_MASK  0x200u
#define FTM_EXTTRIG_CH7TRIG_SHIFT 9u
#define FTM_EXTTRIG_CH7TRIG_WIDTH 1u
#define FTM_EXTTRIG_CH7TRIG(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_EXTTRIG_CH7TRIG_SHIFT)) & FTM_EXTTRIG_CH7TRIG_MASK)
/* POL Bit Fields */
#define FTM_POL_POL0_MASK  0x1u
#define FTM_POL_POL0_SHIFT 0u
#define FTM_POL_POL0_WIDTH 1u
#define FTM_POL_POL0(x)    (((uint32_t)(((uint32_t)(x)) << FTM_POL_POL0_SHIFT)) & FTM_POL_POL0_MASK)
#define FTM_POL_POL1_MASK  0x2u
#define FTM_POL_POL1_SHIFT 1u
#define FTM_POL_POL1_WIDTH 1u
#define FTM_POL_POL1(x)    (((uint32_t)(((uint32_t)(x)) << FTM_POL_POL1_SHIFT)) & FTM_POL_POL1_MASK)
#define FTM_POL_POL2_MASK  0x4u
#define FTM_POL_POL2_SHIFT 2u
#define FTM_POL_POL2_WIDTH 1u
#define FTM_POL_POL2(x)    (((uint32_t)(((uint32_t)(x)) << FTM_POL_POL2_SHIFT)) & FTM_POL_POL2_MASK)
#define FTM_POL_POL3_MASK  0x8u
#define FTM_POL_POL3_SHIFT 3u
#define FTM_POL_POL3_WIDTH 1u
#define FTM_POL_POL3(x)    (((uint32_t)(((uint32_t)(x)) << FTM_POL_POL3_SHIFT)) & FTM_POL_POL3_MASK)
#define FTM_POL_POL4_MASK  0x10u
#define FTM_POL_POL4_SHIFT 4u
#define FTM_POL_POL4_WIDTH 1u
#define FTM_POL_POL4(x)    (((uint32_t)(((uint32_t)(x)) << FTM_POL_POL4_SHIFT)) & FTM_POL_POL4_MASK)
#define FTM_POL_POL5_MASK  0x20u
#define FTM_POL_POL5_SHIFT 5u
#define FTM_POL_POL5_WIDTH 1u
#define FTM_POL_POL5(x)    (((uint32_t)(((uint32_t)(x)) << FTM_POL_POL5_SHIFT)) & FTM_POL_POL5_MASK)
#define FTM_POL_POL6_MASK  0x40u
#define FTM_POL_POL6_SHIFT 6u
#define FTM_POL_POL6_WIDTH 1u
#define FTM_POL_POL6(x)    (((uint32_t)(((uint32_t)(x)) << FTM_POL_POL6_SHIFT)) & FTM_POL_POL6_MASK)
#define FTM_POL_POL7_MASK  0x80u
#define FTM_POL_POL7_SHIFT 7u
#define FTM_POL_POL7_WIDTH 1u
#define FTM_POL_POL7(x)    (((uint32_t)(((uint32_t)(x)) << FTM_POL_POL7_SHIFT)) & FTM_POL_POL7_MASK)
/* FMS Bit Fields */
#define FTM_FMS_FAULTF0_MASK  0x1u
#define FTM_FMS_FAULTF0_SHIFT 0u
#define FTM_FMS_FAULTF0_WIDTH 1u
#define FTM_FMS_FAULTF0(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FMS_FAULTF0_SHIFT)) & FTM_FMS_FAULTF0_MASK)
#define FTM_FMS_FAULTF1_MASK  0x2u
#define FTM_FMS_FAULTF1_SHIFT 1u
#define FTM_FMS_FAULTF1_WIDTH 1u
#define FTM_FMS_FAULTF1(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FMS_FAULTF1_SHIFT)) & FTM_FMS_FAULTF1_MASK)
#define FTM_FMS_FAULTF2_MASK  0x4u
#define FTM_FMS_FAULTF2_SHIFT 2u
#define FTM_FMS_FAULTF2_WIDTH 1u
#define FTM_FMS_FAULTF2(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FMS_FAULTF2_SHIFT)) & FTM_FMS_FAULTF2_MASK)
#define FTM_FMS_FAULTF3_MASK  0x8u
#define FTM_FMS_FAULTF3_SHIFT 3u
#define FTM_FMS_FAULTF3_WIDTH 1u
#define FTM_FMS_FAULTF3(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FMS_FAULTF3_SHIFT)) & FTM_FMS_FAULTF3_MASK)
#define FTM_FMS_FAULTIN_MASK  0x20u
#define FTM_FMS_FAULTIN_SHIFT 5u
#define FTM_FMS_FAULTIN_WIDTH 1u
#define FTM_FMS_FAULTIN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FMS_FAULTIN_SHIFT)) & FTM_FMS_FAULTIN_MASK)
#define FTM_FMS_WPEN_MASK    0x40u
#define FTM_FMS_WPEN_SHIFT   6u
#define FTM_FMS_WPEN_WIDTH   1u
#define FTM_FMS_WPEN(x)      (((uint32_t)(((uint32_t)(x)) << FTM_FMS_WPEN_SHIFT)) & FTM_FMS_WPEN_MASK)
#define FTM_FMS_FAULTF_MASK  0x80u
#define FTM_FMS_FAULTF_SHIFT 7u
#define FTM_FMS_FAULTF_WIDTH 1u
#define FTM_FMS_FAULTF(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FMS_FAULTF_SHIFT)) & FTM_FMS_FAULTF_MASK)
/* FILTER Bit Fields */
#define FTM_FILTER_CH0FVAL_MASK  0xFu
#define FTM_FILTER_CH0FVAL_SHIFT 0u
#define FTM_FILTER_CH0FVAL_WIDTH 4u
#define FTM_FILTER_CH0FVAL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FILTER_CH0FVAL_SHIFT)) & FTM_FILTER_CH0FVAL_MASK)
#define FTM_FILTER_CH1FVAL_MASK  0xF0u
#define FTM_FILTER_CH1FVAL_SHIFT 4u
#define FTM_FILTER_CH1FVAL_WIDTH 4u
#define FTM_FILTER_CH1FVAL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FILTER_CH1FVAL_SHIFT)) & FTM_FILTER_CH1FVAL_MASK)
#define FTM_FILTER_CH2FVAL_MASK  0xF00u
#define FTM_FILTER_CH2FVAL_SHIFT 8u
#define FTM_FILTER_CH2FVAL_WIDTH 4u
#define FTM_FILTER_CH2FVAL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FILTER_CH2FVAL_SHIFT)) & FTM_FILTER_CH2FVAL_MASK)
#define FTM_FILTER_CH3FVAL_MASK  0xF000u
#define FTM_FILTER_CH3FVAL_SHIFT 12u
#define FTM_FILTER_CH3FVAL_WIDTH 4u
#define FTM_FILTER_CH3FVAL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FILTER_CH3FVAL_SHIFT)) & FTM_FILTER_CH3FVAL_MASK)
/* FLTCTRL Bit Fields */
#define FTM_FLTCTRL_FAULT0EN_MASK  0x1u
#define FTM_FLTCTRL_FAULT0EN_SHIFT 0u
#define FTM_FLTCTRL_FAULT0EN_WIDTH 1u
#define FTM_FLTCTRL_FAULT0EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FLTCTRL_FAULT0EN_SHIFT)) & FTM_FLTCTRL_FAULT0EN_MASK)
#define FTM_FLTCTRL_FAULT1EN_MASK  0x2u
#define FTM_FLTCTRL_FAULT1EN_SHIFT 1u
#define FTM_FLTCTRL_FAULT1EN_WIDTH 1u
#define FTM_FLTCTRL_FAULT1EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FLTCTRL_FAULT1EN_SHIFT)) & FTM_FLTCTRL_FAULT1EN_MASK)
#define FTM_FLTCTRL_FAULT2EN_MASK  0x4u
#define FTM_FLTCTRL_FAULT2EN_SHIFT 2u
#define FTM_FLTCTRL_FAULT2EN_WIDTH 1u
#define FTM_FLTCTRL_FAULT2EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FLTCTRL_FAULT2EN_SHIFT)) & FTM_FLTCTRL_FAULT2EN_MASK)
#define FTM_FLTCTRL_FAULT3EN_MASK  0x8u
#define FTM_FLTCTRL_FAULT3EN_SHIFT 3u
#define FTM_FLTCTRL_FAULT3EN_WIDTH 1u
#define FTM_FLTCTRL_FAULT3EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FLTCTRL_FAULT3EN_SHIFT)) & FTM_FLTCTRL_FAULT3EN_MASK)
#define FTM_FLTCTRL_FFLTR0EN_MASK  0x10u
#define FTM_FLTCTRL_FFLTR0EN_SHIFT 4u
#define FTM_FLTCTRL_FFLTR0EN_WIDTH 1u
#define FTM_FLTCTRL_FFLTR0EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FLTCTRL_FFLTR0EN_SHIFT)) & FTM_FLTCTRL_FFLTR0EN_MASK)
#define FTM_FLTCTRL_FFLTR1EN_MASK  0x20u
#define FTM_FLTCTRL_FFLTR1EN_SHIFT 5u
#define FTM_FLTCTRL_FFLTR1EN_WIDTH 1u
#define FTM_FLTCTRL_FFLTR1EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FLTCTRL_FFLTR1EN_SHIFT)) & FTM_FLTCTRL_FFLTR1EN_MASK)
#define FTM_FLTCTRL_FFLTR2EN_MASK  0x40u
#define FTM_FLTCTRL_FFLTR2EN_SHIFT 6u
#define FTM_FLTCTRL_FFLTR2EN_WIDTH 1u
#define FTM_FLTCTRL_FFLTR2EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FLTCTRL_FFLTR2EN_SHIFT)) & FTM_FLTCTRL_FFLTR2EN_MASK)
#define FTM_FLTCTRL_FFLTR3EN_MASK  0x80u
#define FTM_FLTCTRL_FFLTR3EN_SHIFT 7u
#define FTM_FLTCTRL_FFLTR3EN_WIDTH 1u
#define FTM_FLTCTRL_FFLTR3EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FLTCTRL_FFLTR3EN_SHIFT)) & FTM_FLTCTRL_FFLTR3EN_MASK)
#define FTM_FLTCTRL_FFVAL_MASK  0xF00u
#define FTM_FLTCTRL_FFVAL_SHIFT 8u
#define FTM_FLTCTRL_FFVAL_WIDTH 4u
#define FTM_FLTCTRL_FFVAL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FLTCTRL_FFVAL_SHIFT)) & FTM_FLTCTRL_FFVAL_MASK)
#define FTM_FLTCTRL_FSTATE_MASK  0x8000u
#define FTM_FLTCTRL_FSTATE_SHIFT 15u
#define FTM_FLTCTRL_FSTATE_WIDTH 1u
#define FTM_FLTCTRL_FSTATE(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FLTCTRL_FSTATE_SHIFT)) & FTM_FLTCTRL_FSTATE_MASK)
/* QDCTRL Bit Fields */
#define FTM_QDCTRL_QUADEN_MASK  0x1u
#define FTM_QDCTRL_QUADEN_SHIFT 0u
#define FTM_QDCTRL_QUADEN_WIDTH 1u
#define FTM_QDCTRL_QUADEN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_QDCTRL_QUADEN_SHIFT)) & FTM_QDCTRL_QUADEN_MASK)
#define FTM_QDCTRL_TOFDIR_MASK  0x2u
#define FTM_QDCTRL_TOFDIR_SHIFT 1u
#define FTM_QDCTRL_TOFDIR_WIDTH 1u
#define FTM_QDCTRL_TOFDIR(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_QDCTRL_TOFDIR_SHIFT)) & FTM_QDCTRL_TOFDIR_MASK)
#define FTM_QDCTRL_QUADIR_MASK  0x4u
#define FTM_QDCTRL_QUADIR_SHIFT 2u
#define FTM_QDCTRL_QUADIR_WIDTH 1u
#define FTM_QDCTRL_QUADIR(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_QDCTRL_QUADIR_SHIFT)) & FTM_QDCTRL_QUADIR_MASK)
#define FTM_QDCTRL_QUADMODE_MASK  0x8u
#define FTM_QDCTRL_QUADMODE_SHIFT 3u
#define FTM_QDCTRL_QUADMODE_WIDTH 1u
#define FTM_QDCTRL_QUADMODE(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_QDCTRL_QUADMODE_SHIFT)) & FTM_QDCTRL_QUADMODE_MASK)
#define FTM_QDCTRL_PHBPOL_MASK  0x10u
#define FTM_QDCTRL_PHBPOL_SHIFT 4u
#define FTM_QDCTRL_PHBPOL_WIDTH 1u
#define FTM_QDCTRL_PHBPOL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_QDCTRL_PHBPOL_SHIFT)) & FTM_QDCTRL_PHBPOL_MASK)
#define FTM_QDCTRL_PHAPOL_MASK  0x20u
#define FTM_QDCTRL_PHAPOL_SHIFT 5u
#define FTM_QDCTRL_PHAPOL_WIDTH 1u
#define FTM_QDCTRL_PHAPOL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_QDCTRL_PHAPOL_SHIFT)) & FTM_QDCTRL_PHAPOL_MASK)
#define FTM_QDCTRL_PHBFLTREN_MASK  0x40u
#define FTM_QDCTRL_PHBFLTREN_SHIFT 6u
#define FTM_QDCTRL_PHBFLTREN_WIDTH 1u
#define FTM_QDCTRL_PHBFLTREN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_QDCTRL_PHBFLTREN_SHIFT)) & FTM_QDCTRL_PHBFLTREN_MASK)
#define FTM_QDCTRL_PHAFLTREN_MASK  0x80u
#define FTM_QDCTRL_PHAFLTREN_SHIFT 7u
#define FTM_QDCTRL_PHAFLTREN_WIDTH 1u
#define FTM_QDCTRL_PHAFLTREN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_QDCTRL_PHAFLTREN_SHIFT)) & FTM_QDCTRL_PHAFLTREN_MASK)
/* CONF Bit Fields */
#define FTM_CONF_LDFQ_MASK     0x1Fu
#define FTM_CONF_LDFQ_SHIFT    0u
#define FTM_CONF_LDFQ_WIDTH    5u
#define FTM_CONF_LDFQ(x)       (((uint32_t)(((uint32_t)(x)) << FTM_CONF_LDFQ_SHIFT)) & FTM_CONF_LDFQ_MASK)
#define FTM_CONF_BDMMODE_MASK  0xC0u
#define FTM_CONF_BDMMODE_SHIFT 6u
#define FTM_CONF_BDMMODE_WIDTH 2u
#define FTM_CONF_BDMMODE(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_CONF_BDMMODE_SHIFT)) & FTM_CONF_BDMMODE_MASK)
#define FTM_CONF_GTBEEN_MASK  0x200u
#define FTM_CONF_GTBEEN_SHIFT 9u
#define FTM_CONF_GTBEEN_WIDTH 1u
#define FTM_CONF_GTBEEN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_CONF_GTBEEN_SHIFT)) & FTM_CONF_GTBEEN_MASK)
#define FTM_CONF_GTBEOUT_MASK  0x400u
#define FTM_CONF_GTBEOUT_SHIFT 10u
#define FTM_CONF_GTBEOUT_WIDTH 1u
#define FTM_CONF_GTBEOUT(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_CONF_GTBEOUT_SHIFT)) & FTM_CONF_GTBEOUT_MASK)
#define FTM_CONF_ITRIGR_MASK  0x800u
#define FTM_CONF_ITRIGR_SHIFT 11u
#define FTM_CONF_ITRIGR_WIDTH 1u
#define FTM_CONF_ITRIGR(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_CONF_ITRIGR_SHIFT)) & FTM_CONF_ITRIGR_MASK)
/* FLTPOL Bit Fields */
#define FTM_FLTPOL_FLT0POL_MASK  0x1u
#define FTM_FLTPOL_FLT0POL_SHIFT 0u
#define FTM_FLTPOL_FLT0POL_WIDTH 1u
#define FTM_FLTPOL_FLT0POL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FLTPOL_FLT0POL_SHIFT)) & FTM_FLTPOL_FLT0POL_MASK)
#define FTM_FLTPOL_FLT1POL_MASK  0x2u
#define FTM_FLTPOL_FLT1POL_SHIFT 1u
#define FTM_FLTPOL_FLT1POL_WIDTH 1u
#define FTM_FLTPOL_FLT1POL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FLTPOL_FLT1POL_SHIFT)) & FTM_FLTPOL_FLT1POL_MASK)
#define FTM_FLTPOL_FLT2POL_MASK  0x4u
#define FTM_FLTPOL_FLT2POL_SHIFT 2u
#define FTM_FLTPOL_FLT2POL_WIDTH 1u
#define FTM_FLTPOL_FLT2POL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FLTPOL_FLT2POL_SHIFT)) & FTM_FLTPOL_FLT2POL_MASK)
#define FTM_FLTPOL_FLT3POL_MASK  0x8u
#define FTM_FLTPOL_FLT3POL_SHIFT 3u
#define FTM_FLTPOL_FLT3POL_WIDTH 1u
#define FTM_FLTPOL_FLT3POL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_FLTPOL_FLT3POL_SHIFT)) & FTM_FLTPOL_FLT3POL_MASK)
/* SYNCONF Bit Fields */
#define FTM_SYNCONF_HWTRIGMODE_MASK  0x1u
#define FTM_SYNCONF_HWTRIGMODE_SHIFT 0u
#define FTM_SYNCONF_HWTRIGMODE_WIDTH 1u
#define FTM_SYNCONF_HWTRIGMODE(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNCONF_HWTRIGMODE_SHIFT)) & FTM_SYNCONF_HWTRIGMODE_MASK)
#define FTM_SYNCONF_CNTINC_MASK  0x4u
#define FTM_SYNCONF_CNTINC_SHIFT 2u
#define FTM_SYNCONF_CNTINC_WIDTH 1u
#define FTM_SYNCONF_CNTINC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNCONF_CNTINC_SHIFT)) & FTM_SYNCONF_CNTINC_MASK)
#define FTM_SYNCONF_INVC_MASK  0x10u
#define FTM_SYNCONF_INVC_SHIFT 4u
#define FTM_SYNCONF_INVC_WIDTH 1u
#define FTM_SYNCONF_INVC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNCONF_INVC_SHIFT)) & FTM_SYNCONF_INVC_MASK)
#define FTM_SYNCONF_SWOC_MASK  0x20u
#define FTM_SYNCONF_SWOC_SHIFT 5u
#define FTM_SYNCONF_SWOC_WIDTH 1u
#define FTM_SYNCONF_SWOC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNCONF_SWOC_SHIFT)) & FTM_SYNCONF_SWOC_MASK)
#define FTM_SYNCONF_SYNCMODE_MASK  0x80u
#define FTM_SYNCONF_SYNCMODE_SHIFT 7u
#define FTM_SYNCONF_SYNCMODE_WIDTH 1u
#define FTM_SYNCONF_SYNCMODE(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNCONF_SYNCMODE_SHIFT)) & FTM_SYNCONF_SYNCMODE_MASK)
#define FTM_SYNCONF_SWRSTCNT_MASK  0x100u
#define FTM_SYNCONF_SWRSTCNT_SHIFT 8u
#define FTM_SYNCONF_SWRSTCNT_WIDTH 1u
#define FTM_SYNCONF_SWRSTCNT(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNCONF_SWRSTCNT_SHIFT)) & FTM_SYNCONF_SWRSTCNT_MASK)
#define FTM_SYNCONF_SWWRBUF_MASK  0x200u
#define FTM_SYNCONF_SWWRBUF_SHIFT 9u
#define FTM_SYNCONF_SWWRBUF_WIDTH 1u
#define FTM_SYNCONF_SWWRBUF(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNCONF_SWWRBUF_SHIFT)) & FTM_SYNCONF_SWWRBUF_MASK)
#define FTM_SYNCONF_SWOM_MASK  0x400u
#define FTM_SYNCONF_SWOM_SHIFT 10u
#define FTM_SYNCONF_SWOM_WIDTH 1u
#define FTM_SYNCONF_SWOM(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNCONF_SWOM_SHIFT)) & FTM_SYNCONF_SWOM_MASK)
#define FTM_SYNCONF_SWINVC_MASK  0x800u
#define FTM_SYNCONF_SWINVC_SHIFT 11u
#define FTM_SYNCONF_SWINVC_WIDTH 1u
#define FTM_SYNCONF_SWINVC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNCONF_SWINVC_SHIFT)) & FTM_SYNCONF_SWINVC_MASK)
#define FTM_SYNCONF_SWSOC_MASK  0x1000u
#define FTM_SYNCONF_SWSOC_SHIFT 12u
#define FTM_SYNCONF_SWSOC_WIDTH 1u
#define FTM_SYNCONF_SWSOC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNCONF_SWSOC_SHIFT)) & FTM_SYNCONF_SWSOC_MASK)
#define FTM_SYNCONF_HWRSTCNT_MASK  0x10000u
#define FTM_SYNCONF_HWRSTCNT_SHIFT 16u
#define FTM_SYNCONF_HWRSTCNT_WIDTH 1u
#define FTM_SYNCONF_HWRSTCNT(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNCONF_HWRSTCNT_SHIFT)) & FTM_SYNCONF_HWRSTCNT_MASK)
#define FTM_SYNCONF_HWWRBUF_MASK  0x20000u
#define FTM_SYNCONF_HWWRBUF_SHIFT 17u
#define FTM_SYNCONF_HWWRBUF_WIDTH 1u
#define FTM_SYNCONF_HWWRBUF(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNCONF_HWWRBUF_SHIFT)) & FTM_SYNCONF_HWWRBUF_MASK)
#define FTM_SYNCONF_HWOM_MASK  0x40000u
#define FTM_SYNCONF_HWOM_SHIFT 18u
#define FTM_SYNCONF_HWOM_WIDTH 1u
#define FTM_SYNCONF_HWOM(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNCONF_HWOM_SHIFT)) & FTM_SYNCONF_HWOM_MASK)
#define FTM_SYNCONF_HWINVC_MASK  0x80000u
#define FTM_SYNCONF_HWINVC_SHIFT 19u
#define FTM_SYNCONF_HWINVC_WIDTH 1u
#define FTM_SYNCONF_HWINVC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNCONF_HWINVC_SHIFT)) & FTM_SYNCONF_HWINVC_MASK)
#define FTM_SYNCONF_HWSOC_MASK  0x100000u
#define FTM_SYNCONF_HWSOC_SHIFT 20u
#define FTM_SYNCONF_HWSOC_WIDTH 1u
#define FTM_SYNCONF_HWSOC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SYNCONF_HWSOC_SHIFT)) & FTM_SYNCONF_HWSOC_MASK)
/* INVCTRL Bit Fields */
#define FTM_INVCTRL_INV0EN_MASK  0x1u
#define FTM_INVCTRL_INV0EN_SHIFT 0u
#define FTM_INVCTRL_INV0EN_WIDTH 1u
#define FTM_INVCTRL_INV0EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_INVCTRL_INV0EN_SHIFT)) & FTM_INVCTRL_INV0EN_MASK)
#define FTM_INVCTRL_INV1EN_MASK  0x2u
#define FTM_INVCTRL_INV1EN_SHIFT 1u
#define FTM_INVCTRL_INV1EN_WIDTH 1u
#define FTM_INVCTRL_INV1EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_INVCTRL_INV1EN_SHIFT)) & FTM_INVCTRL_INV1EN_MASK)
#define FTM_INVCTRL_INV2EN_MASK  0x4u
#define FTM_INVCTRL_INV2EN_SHIFT 2u
#define FTM_INVCTRL_INV2EN_WIDTH 1u
#define FTM_INVCTRL_INV2EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_INVCTRL_INV2EN_SHIFT)) & FTM_INVCTRL_INV2EN_MASK)
#define FTM_INVCTRL_INV3EN_MASK  0x8u
#define FTM_INVCTRL_INV3EN_SHIFT 3u
#define FTM_INVCTRL_INV3EN_WIDTH 1u
#define FTM_INVCTRL_INV3EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_INVCTRL_INV3EN_SHIFT)) & FTM_INVCTRL_INV3EN_MASK)
/* SWOCTRL Bit Fields */
#define FTM_SWOCTRL_CH0OC_MASK  0x1u
#define FTM_SWOCTRL_CH0OC_SHIFT 0u
#define FTM_SWOCTRL_CH0OC_WIDTH 1u
#define FTM_SWOCTRL_CH0OC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH0OC_SHIFT)) & FTM_SWOCTRL_CH0OC_MASK)
#define FTM_SWOCTRL_CH1OC_MASK  0x2u
#define FTM_SWOCTRL_CH1OC_SHIFT 1u
#define FTM_SWOCTRL_CH1OC_WIDTH 1u
#define FTM_SWOCTRL_CH1OC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH1OC_SHIFT)) & FTM_SWOCTRL_CH1OC_MASK)
#define FTM_SWOCTRL_CH2OC_MASK  0x4u
#define FTM_SWOCTRL_CH2OC_SHIFT 2u
#define FTM_SWOCTRL_CH2OC_WIDTH 1u
#define FTM_SWOCTRL_CH2OC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH2OC_SHIFT)) & FTM_SWOCTRL_CH2OC_MASK)
#define FTM_SWOCTRL_CH3OC_MASK  0x8u
#define FTM_SWOCTRL_CH3OC_SHIFT 3u
#define FTM_SWOCTRL_CH3OC_WIDTH 1u
#define FTM_SWOCTRL_CH3OC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH3OC_SHIFT)) & FTM_SWOCTRL_CH3OC_MASK)
#define FTM_SWOCTRL_CH4OC_MASK  0x10u
#define FTM_SWOCTRL_CH4OC_SHIFT 4u
#define FTM_SWOCTRL_CH4OC_WIDTH 1u
#define FTM_SWOCTRL_CH4OC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH4OC_SHIFT)) & FTM_SWOCTRL_CH4OC_MASK)
#define FTM_SWOCTRL_CH5OC_MASK  0x20u
#define FTM_SWOCTRL_CH5OC_SHIFT 5u
#define FTM_SWOCTRL_CH5OC_WIDTH 1u
#define FTM_SWOCTRL_CH5OC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH5OC_SHIFT)) & FTM_SWOCTRL_CH5OC_MASK)
#define FTM_SWOCTRL_CH6OC_MASK  0x40u
#define FTM_SWOCTRL_CH6OC_SHIFT 6u
#define FTM_SWOCTRL_CH6OC_WIDTH 1u
#define FTM_SWOCTRL_CH6OC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH6OC_SHIFT)) & FTM_SWOCTRL_CH6OC_MASK)
#define FTM_SWOCTRL_CH7OC_MASK  0x80u
#define FTM_SWOCTRL_CH7OC_SHIFT 7u
#define FTM_SWOCTRL_CH7OC_WIDTH 1u
#define FTM_SWOCTRL_CH7OC(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH7OC_SHIFT)) & FTM_SWOCTRL_CH7OC_MASK)
#define FTM_SWOCTRL_CH0OCV_MASK  0x100u
#define FTM_SWOCTRL_CH0OCV_SHIFT 8u
#define FTM_SWOCTRL_CH0OCV_WIDTH 1u
#define FTM_SWOCTRL_CH0OCV(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH0OCV_SHIFT)) & FTM_SWOCTRL_CH0OCV_MASK)
#define FTM_SWOCTRL_CH1OCV_MASK  0x200u
#define FTM_SWOCTRL_CH1OCV_SHIFT 9u
#define FTM_SWOCTRL_CH1OCV_WIDTH 1u
#define FTM_SWOCTRL_CH1OCV(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH1OCV_SHIFT)) & FTM_SWOCTRL_CH1OCV_MASK)
#define FTM_SWOCTRL_CH2OCV_MASK  0x400u
#define FTM_SWOCTRL_CH2OCV_SHIFT 10u
#define FTM_SWOCTRL_CH2OCV_WIDTH 1u
#define FTM_SWOCTRL_CH2OCV(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH2OCV_SHIFT)) & FTM_SWOCTRL_CH2OCV_MASK)
#define FTM_SWOCTRL_CH3OCV_MASK  0x800u
#define FTM_SWOCTRL_CH3OCV_SHIFT 11u
#define FTM_SWOCTRL_CH3OCV_WIDTH 1u
#define FTM_SWOCTRL_CH3OCV(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH3OCV_SHIFT)) & FTM_SWOCTRL_CH3OCV_MASK)
#define FTM_SWOCTRL_CH4OCV_MASK  0x1000u
#define FTM_SWOCTRL_CH4OCV_SHIFT 12u
#define FTM_SWOCTRL_CH4OCV_WIDTH 1u
#define FTM_SWOCTRL_CH4OCV(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH4OCV_SHIFT)) & FTM_SWOCTRL_CH4OCV_MASK)
#define FTM_SWOCTRL_CH5OCV_MASK  0x2000u
#define FTM_SWOCTRL_CH5OCV_SHIFT 13u
#define FTM_SWOCTRL_CH5OCV_WIDTH 1u
#define FTM_SWOCTRL_CH5OCV(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH5OCV_SHIFT)) & FTM_SWOCTRL_CH5OCV_MASK)
#define FTM_SWOCTRL_CH6OCV_MASK  0x4000u
#define FTM_SWOCTRL_CH6OCV_SHIFT 14u
#define FTM_SWOCTRL_CH6OCV_WIDTH 1u
#define FTM_SWOCTRL_CH6OCV(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH6OCV_SHIFT)) & FTM_SWOCTRL_CH6OCV_MASK)
#define FTM_SWOCTRL_CH7OCV_MASK  0x8000u
#define FTM_SWOCTRL_CH7OCV_SHIFT 15u
#define FTM_SWOCTRL_CH7OCV_WIDTH 1u
#define FTM_SWOCTRL_CH7OCV(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_SWOCTRL_CH7OCV_SHIFT)) & FTM_SWOCTRL_CH7OCV_MASK)
/* PWMLOAD Bit Fields */
#define FTM_PWMLOAD_CH0SEL_MASK  0x1u
#define FTM_PWMLOAD_CH0SEL_SHIFT 0u
#define FTM_PWMLOAD_CH0SEL_WIDTH 1u
#define FTM_PWMLOAD_CH0SEL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PWMLOAD_CH0SEL_SHIFT)) & FTM_PWMLOAD_CH0SEL_MASK)
#define FTM_PWMLOAD_CH1SEL_MASK  0x2u
#define FTM_PWMLOAD_CH1SEL_SHIFT 1u
#define FTM_PWMLOAD_CH1SEL_WIDTH 1u
#define FTM_PWMLOAD_CH1SEL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PWMLOAD_CH1SEL_SHIFT)) & FTM_PWMLOAD_CH1SEL_MASK)
#define FTM_PWMLOAD_CH2SEL_MASK  0x4u
#define FTM_PWMLOAD_CH2SEL_SHIFT 2u
#define FTM_PWMLOAD_CH2SEL_WIDTH 1u
#define FTM_PWMLOAD_CH2SEL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PWMLOAD_CH2SEL_SHIFT)) & FTM_PWMLOAD_CH2SEL_MASK)
#define FTM_PWMLOAD_CH3SEL_MASK  0x8u
#define FTM_PWMLOAD_CH3SEL_SHIFT 3u
#define FTM_PWMLOAD_CH3SEL_WIDTH 1u
#define FTM_PWMLOAD_CH3SEL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PWMLOAD_CH3SEL_SHIFT)) & FTM_PWMLOAD_CH3SEL_MASK)
#define FTM_PWMLOAD_CH4SEL_MASK  0x10u
#define FTM_PWMLOAD_CH4SEL_SHIFT 4u
#define FTM_PWMLOAD_CH4SEL_WIDTH 1u
#define FTM_PWMLOAD_CH4SEL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PWMLOAD_CH4SEL_SHIFT)) & FTM_PWMLOAD_CH4SEL_MASK)
#define FTM_PWMLOAD_CH5SEL_MASK  0x20u
#define FTM_PWMLOAD_CH5SEL_SHIFT 5u
#define FTM_PWMLOAD_CH5SEL_WIDTH 1u
#define FTM_PWMLOAD_CH5SEL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PWMLOAD_CH5SEL_SHIFT)) & FTM_PWMLOAD_CH5SEL_MASK)
#define FTM_PWMLOAD_CH6SEL_MASK  0x40u
#define FTM_PWMLOAD_CH6SEL_SHIFT 6u
#define FTM_PWMLOAD_CH6SEL_WIDTH 1u
#define FTM_PWMLOAD_CH6SEL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PWMLOAD_CH6SEL_SHIFT)) & FTM_PWMLOAD_CH6SEL_MASK)
#define FTM_PWMLOAD_CH7SEL_MASK  0x80u
#define FTM_PWMLOAD_CH7SEL_SHIFT 7u
#define FTM_PWMLOAD_CH7SEL_WIDTH 1u
#define FTM_PWMLOAD_CH7SEL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PWMLOAD_CH7SEL_SHIFT)) & FTM_PWMLOAD_CH7SEL_MASK)
#define FTM_PWMLOAD_HCSEL_MASK  0x100u
#define FTM_PWMLOAD_HCSEL_SHIFT 8u
#define FTM_PWMLOAD_HCSEL_WIDTH 1u
#define FTM_PWMLOAD_HCSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PWMLOAD_HCSEL_SHIFT)) & FTM_PWMLOAD_HCSEL_MASK)
#define FTM_PWMLOAD_LDOK_MASK  0x200u
#define FTM_PWMLOAD_LDOK_SHIFT 9u
#define FTM_PWMLOAD_LDOK_WIDTH 1u
#define FTM_PWMLOAD_LDOK(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PWMLOAD_LDOK_SHIFT)) & FTM_PWMLOAD_LDOK_MASK)
#define FTM_PWMLOAD_GLEN_MASK  0x400u
#define FTM_PWMLOAD_GLEN_SHIFT 10u
#define FTM_PWMLOAD_GLEN_WIDTH 1u
#define FTM_PWMLOAD_GLEN(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PWMLOAD_GLEN_SHIFT)) & FTM_PWMLOAD_GLEN_MASK)
#define FTM_PWMLOAD_GLDOK_MASK  0x800u
#define FTM_PWMLOAD_GLDOK_SHIFT 11u
#define FTM_PWMLOAD_GLDOK_WIDTH 1u
#define FTM_PWMLOAD_GLDOK(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PWMLOAD_GLDOK_SHIFT)) & FTM_PWMLOAD_GLDOK_MASK)
/* HCR Bit Fields */
#define FTM_HCR_HCVAL_MASK  0xFFFFu
#define FTM_HCR_HCVAL_SHIFT 0u
#define FTM_HCR_HCVAL_WIDTH 16u
#define FTM_HCR_HCVAL(x)    (((uint32_t)(((uint32_t)(x)) << FTM_HCR_HCVAL_SHIFT)) & FTM_HCR_HCVAL_MASK)
/* PAIR0DEADTIME Bit Fields */
#define FTM_PAIR0DEADTIME_DTVAL_MASK  0x3Fu
#define FTM_PAIR0DEADTIME_DTVAL_SHIFT 0u
#define FTM_PAIR0DEADTIME_DTVAL_WIDTH 6u
#define FTM_PAIR0DEADTIME_DTVAL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PAIR0DEADTIME_DTVAL_SHIFT)) & FTM_PAIR0DEADTIME_DTVAL_MASK)
#define FTM_PAIR0DEADTIME_DTPS_MASK  0xC0u
#define FTM_PAIR0DEADTIME_DTPS_SHIFT 6u
#define FTM_PAIR0DEADTIME_DTPS_WIDTH 2u
#define FTM_PAIR0DEADTIME_DTPS(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PAIR0DEADTIME_DTPS_SHIFT)) & FTM_PAIR0DEADTIME_DTPS_MASK)
#define FTM_PAIR0DEADTIME_DTVALEX_MASK  0xF0000u
#define FTM_PAIR0DEADTIME_DTVALEX_SHIFT 16u
#define FTM_PAIR0DEADTIME_DTVALEX_WIDTH 4u
#define FTM_PAIR0DEADTIME_DTVALEX(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << FTM_PAIR0DEADTIME_DTVALEX_SHIFT)) & \
     FTM_PAIR0DEADTIME_DTVALEX_MASK)
/* PAIR1DEADTIME Bit Fields */
#define FTM_PAIR1DEADTIME_DTVAL_MASK  0x3Fu
#define FTM_PAIR1DEADTIME_DTVAL_SHIFT 0u
#define FTM_PAIR1DEADTIME_DTVAL_WIDTH 6u
#define FTM_PAIR1DEADTIME_DTVAL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PAIR1DEADTIME_DTVAL_SHIFT)) & FTM_PAIR1DEADTIME_DTVAL_MASK)
#define FTM_PAIR1DEADTIME_DTPS_MASK  0xC0u
#define FTM_PAIR1DEADTIME_DTPS_SHIFT 6u
#define FTM_PAIR1DEADTIME_DTPS_WIDTH 2u
#define FTM_PAIR1DEADTIME_DTPS(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PAIR1DEADTIME_DTPS_SHIFT)) & FTM_PAIR1DEADTIME_DTPS_MASK)
#define FTM_PAIR1DEADTIME_DTVALEX_MASK  0xF0000u
#define FTM_PAIR1DEADTIME_DTVALEX_SHIFT 16u
#define FTM_PAIR1DEADTIME_DTVALEX_WIDTH 4u
#define FTM_PAIR1DEADTIME_DTVALEX(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << FTM_PAIR1DEADTIME_DTVALEX_SHIFT)) & \
     FTM_PAIR1DEADTIME_DTVALEX_MASK)
/* PAIR2DEADTIME Bit Fields */
#define FTM_PAIR2DEADTIME_DTVAL_MASK  0x3Fu
#define FTM_PAIR2DEADTIME_DTVAL_SHIFT 0u
#define FTM_PAIR2DEADTIME_DTVAL_WIDTH 6u
#define FTM_PAIR2DEADTIME_DTVAL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PAIR2DEADTIME_DTVAL_SHIFT)) & FTM_PAIR2DEADTIME_DTVAL_MASK)
#define FTM_PAIR2DEADTIME_DTPS_MASK  0xC0u
#define FTM_PAIR2DEADTIME_DTPS_SHIFT 6u
#define FTM_PAIR2DEADTIME_DTPS_WIDTH 2u
#define FTM_PAIR2DEADTIME_DTPS(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PAIR2DEADTIME_DTPS_SHIFT)) & FTM_PAIR2DEADTIME_DTPS_MASK)
#define FTM_PAIR2DEADTIME_DTVALEX_MASK  0xF0000u
#define FTM_PAIR2DEADTIME_DTVALEX_SHIFT 16u
#define FTM_PAIR2DEADTIME_DTVALEX_WIDTH 4u
#define FTM_PAIR2DEADTIME_DTVALEX(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << FTM_PAIR2DEADTIME_DTVALEX_SHIFT)) & \
     FTM_PAIR2DEADTIME_DTVALEX_MASK)
/* PAIR3DEADTIME Bit Fields */
#define FTM_PAIR3DEADTIME_DTVAL_MASK  0x3Fu
#define FTM_PAIR3DEADTIME_DTVAL_SHIFT 0u
#define FTM_PAIR3DEADTIME_DTVAL_WIDTH 6u
#define FTM_PAIR3DEADTIME_DTVAL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PAIR3DEADTIME_DTVAL_SHIFT)) & FTM_PAIR3DEADTIME_DTVAL_MASK)
#define FTM_PAIR3DEADTIME_DTPS_MASK  0xC0u
#define FTM_PAIR3DEADTIME_DTPS_SHIFT 6u
#define FTM_PAIR3DEADTIME_DTPS_WIDTH 2u
#define FTM_PAIR3DEADTIME_DTPS(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_PAIR3DEADTIME_DTPS_SHIFT)) & FTM_PAIR3DEADTIME_DTPS_MASK)
#define FTM_PAIR3DEADTIME_DTVALEX_MASK  0xF0000u
#define FTM_PAIR3DEADTIME_DTVALEX_SHIFT 16u
#define FTM_PAIR3DEADTIME_DTVALEX_WIDTH 4u
#define FTM_PAIR3DEADTIME_DTVALEX(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << FTM_PAIR3DEADTIME_DTVALEX_SHIFT)) & \
     FTM_PAIR3DEADTIME_DTVALEX_MASK)
/* MOD_MIRROR Bit Fields */
#define FTM_MOD_MIRROR_FRACMOD_MASK  0xF800u
#define FTM_MOD_MIRROR_FRACMOD_SHIFT 11u
#define FTM_MOD_MIRROR_FRACMOD_WIDTH 5u
#define FTM_MOD_MIRROR_FRACMOD(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_MOD_MIRROR_FRACMOD_SHIFT)) & FTM_MOD_MIRROR_FRACMOD_MASK)
#define FTM_MOD_MIRROR_MOD_MASK  0xFFFF0000u
#define FTM_MOD_MIRROR_MOD_SHIFT 16u
#define FTM_MOD_MIRROR_MOD_WIDTH 16u
#define FTM_MOD_MIRROR_MOD(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_MOD_MIRROR_MOD_SHIFT)) & FTM_MOD_MIRROR_MOD_MASK)
/* CV_MIRROR Bit Fields */
#define FTM_CV_MIRROR_FRACVAL_MASK  0xF800u
#define FTM_CV_MIRROR_FRACVAL_SHIFT 11u
#define FTM_CV_MIRROR_FRACVAL_WIDTH 5u
#define FTM_CV_MIRROR_FRACVAL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_CV_MIRROR_FRACVAL_SHIFT)) & FTM_CV_MIRROR_FRACVAL_MASK)
#define FTM_CV_MIRROR_VAL_MASK  0xFFFF0000u
#define FTM_CV_MIRROR_VAL_SHIFT 16u
#define FTM_CV_MIRROR_VAL_WIDTH 16u
#define FTM_CV_MIRROR_VAL(x) \
    (((uint32_t)(((uint32_t)(x)) << FTM_CV_MIRROR_VAL_SHIFT)) & FTM_CV_MIRROR_VAL_MASK)

/**
 * @}
 */ /* end of group FTM_Register_Masks */

/**
 * @}
 */ /* end of group FTM_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------
   -- GPIO Peripheral Access Layer
   ---------------------------------------------------------------------- */

/**
 * @addtogroup GPIO_Peripheral_Access_Layer GPIO Peripheral Access Layer
 * @{
 */

/** GPIO - Size of Registers Arrays */

/** GPIO - Register Layout Typedef */
typedef struct
{
    _IO uint32_t PDOR; /**< offset: 0x0 */
    __O uint32_t PSOR; /**< offset: 0x4 */
    __O uint32_t PCOR; /**< offset: 0x8 */
    __O uint32_t PTOR; /**< offset: 0xC */
    __I uint32_t PDIR; /**< offset: 0x10 */
    _IO uint32_t PDDR; /**< offset: 0x14 */
    _IO uint32_t PIDR; /**< offset: 0x18 */
} GPIO_t, *GPIO_MemMapPtr;

/** Number of instances of the GPIO module. */
#define GPIO_INSTANCE_COUNT (5u)

/* GPIO - Peripheral instance base addresses */
/** Peripheral PTA base address */
#define PTA_BASE (0x400FF000u)
/** Peripheral PTA base pointer */
#define PTA ((GPIO_t *)PTA_BASE)
/** Peripheral PTB base address */
#define PTB_BASE (0x400FF040u)
/** Peripheral PTB base pointer */
#define PTB ((GPIO_t *)PTB_BASE)
/** Peripheral PTC base address */
#define PTC_BASE (0x400FF080u)
/** Peripheral PTC base pointer */
#define PTC ((GPIO_t *)PTC_BASE)
/** Peripheral PTD base address */
#define PTD_BASE (0x400FF0C0u)
/** Peripheral PTD base pointer */
#define PTD ((GPIO_t *)PTD_BASE)
/** Peripheral PTE base address */
#define PTE_BASE (0x400FF100u)
/** Peripheral PTE base pointer */
#define PTE ((GPIO_t *)PTE_BASE)
/** Array initializer of GPIO peripheral base addresses */
#define GPIO_BASE_ADDRS                                  \
    {                                                    \
        PTA_BASE, PTB_BASE, PTC_BASE, PTD_BASE, PTE_BASE \
    }
/** Array initializer of GPIO peripheral base pointers */
#define GPIO_BASE_PTRS          \
    {                           \
        PTA, PTB, PTC, PTD, PTE \
    }

/* --------------------------------------------------------------------------
   -- GPIO Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup GPIO_Register_Masks GPIO Register Masks
 * @{
 */

/* PDOR Bit Fields */
#define GPIO_PDOR_PDO_MASK  0xFFFFu
#define GPIO_PDOR_PDO_SHIFT 0u
#define GPIO_PDOR_PDO_WIDTH 16u
#define GPIO_PDOR_PDO(x)    (((uint32_t)(((uint32_t)(x)) << GPIO_PDOR_PDO_SHIFT)) & GPIO_PDOR_PDO_MASK)
/* PSOR Bit Fields */
#define GPIO_PSOR_PTSO_MASK  0xFFFFu
#define GPIO_PSOR_PTSO_SHIFT 0u
#define GPIO_PSOR_PTSO_WIDTH 16u
#define GPIO_PSOR_PTSO(x) \
    (((uint32_t)(((uint32_t)(x)) << GPIO_PSOR_PTSO_SHIFT)) & GPIO_PSOR_PTSO_MASK)
/* PCOR Bit Fields */
#define GPIO_PCOR_PTCO_MASK  0xFFFFu
#define GPIO_PCOR_PTCO_SHIFT 0u
#define GPIO_PCOR_PTCO_WIDTH 16u
#define GPIO_PCOR_PTCO(x) \
    (((uint32_t)(((uint32_t)(x)) << GPIO_PCOR_PTCO_SHIFT)) & GPIO_PCOR_PTCO_MASK)
/* PTOR Bit Fields */
#define GPIO_PTOR_PTTO_MASK  0xFFFFu
#define GPIO_PTOR_PTTO_SHIFT 0u
#define GPIO_PTOR_PTTO_WIDTH 16u
#define GPIO_PTOR_PTTO(x) \
    (((uint32_t)(((uint32_t)(x)) << GPIO_PTOR_PTTO_SHIFT)) & GPIO_PTOR_PTTO_MASK)
/* PDIR Bit Fields */
#define GPIO_PDIR_PDI_MASK  0xFFFFu
#define GPIO_PDIR_PDI_SHIFT 0u
#define GPIO_PDIR_PDI_WIDTH 16u
#define GPIO_PDIR_PDI(x)    (((uint32_t)(((uint32_t)(x)) << GPIO_PDIR_PDI_SHIFT)) & GPIO_PDIR_PDI_MASK)
/* PDDR Bit Fields */
#define GPIO_PDDR_PDD_MASK  0xFFFFu
#define GPIO_PDDR_PDD_SHIFT 0u
#define GPIO_PDDR_PDD_WIDTH 16u
#define GPIO_PDDR_PDD(x)    (((uint32_t)(((uint32_t)(x)) << GPIO_PDDR_PDD_SHIFT)) & GPIO_PDDR_PDD_MASK)
/* PIDR Bit Fields */
#define GPIO_PIDR_PID_MASK  0xFFFFu
#define GPIO_PIDR_PID_SHIFT 0u
#define GPIO_PIDR_PID_WIDTH 16u
#define GPIO_PIDR_PID(x)    (((uint32_t)(((uint32_t)(x)) << GPIO_PIDR_PID_SHIFT)) & GPIO_PIDR_PID_MASK)

/**
 * @}
 */ /* end of group GPIO_Register_Masks */

/**
 * @}
 */ /* end of group GPIO_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------
   -- LPI2C Peripheral Access Layer
   ---------------------------------------------------------------------- */

/**
 * @addtogroup LPI2C_Peripheral_Access_Layer LPI2C Peripheral Access Layer
 * @{
 */

/** LPI2C - Size of Registers Arrays */

/** LPI2C - Register Layout Typedef */
typedef struct
{
    __I uint32_t VERID;  /**< offset: 0x0 */
    __I uint32_t PARAM;  /**< offset: 0x4 */
    _NA uint8_t  RESERVED_0[8];
    _IO uint32_t MCR;    /**< offset: 0x10 */
    _IO uint32_t MSR;    /**< offset: 0x14 */
    _IO uint32_t MIER;   /**< offset: 0x18 */
    _IO uint32_t MDER;   /**< offset: 0x1C */
    _IO uint32_t MCFGR0; /**< offset: 0x20 */
    _IO uint32_t MCFGR1; /**< offset: 0x24 */
    _IO uint32_t MCFGR2; /**< offset: 0x28 */
    _IO uint32_t MCFGR3; /**< offset: 0x2C */
    _NA uint8_t  RESERVED_1[16];
    _IO uint32_t MDMR;   /**< offset: 0x40 */
    _NA uint8_t  RESERVED_2[4];
    _IO uint32_t MCCR0;  /**< offset: 0x48 */
    _NA uint8_t  RESERVED_3[4];
    _IO uint32_t MCCR1;  /**< offset: 0x50 */
    _NA uint8_t  RESERVED_4[4];
    _IO uint32_t MFCR;   /**< offset: 0x58 */
    __I uint32_t MFSR;   /**< offset: 0x5C */
    __O uint32_t MTDR;   /**< offset: 0x60 */
    _NA uint8_t  RESERVED_5[12];
    __I uint32_t MRDR;   /**< offset: 0x70 */
    _NA uint8_t  RESERVED_6[156];
    _IO uint32_t SCR;    /**< offset: 0x110 */
    _IO uint32_t SSR;    /**< offset: 0x114 */
    _IO uint32_t SIER;   /**< offset: 0x118 */
    _IO uint32_t SDER;   /**< offset: 0x11C */
    _NA uint8_t  RESERVED_7[4];
    _IO uint32_t SCFGR1; /**< offset: 0x124 */
    _IO uint32_t SCFGR2; /**< offset: 0x128 */
    _NA uint8_t  RESERVED_8[20];
    _IO uint32_t SAMR;   /**< offset: 0x140 */
    _NA uint8_t  RESERVED_9[12];
    __I uint32_t SASR;   /**< offset: 0x150 */
    _IO uint32_t STAR;   /**< offset: 0x154 */
    _NA uint8_t  RESERVED_10[8];
    __O uint32_t STDR;   /**< offset: 0x160 */
    _NA uint8_t  RESERVED_11[12];
    __I uint32_t SRDR;   /**< offset: 0x170 */
} LPI2C_t, *LPI2C_MemMapPtr;

/** Number of instances of the LPI2C module. */
#define LPI2C_INSTANCE_COUNT (2u)

/* LPI2C - Peripheral instance base addresses */
/** Peripheral LPI2C0 base address */
#define LPI2C0_BASE (0x40066000u)
/** Peripheral LPI2C0 base pointer */
#define LPI2C0 ((LPI2C_t *)LPI2C0_BASE)
/** Peripheral LPI2C1 base address */
#define LPI2C1_BASE (0x40067000u)
/** Peripheral LPI2C1 base pointer */
#define LPI2C1 ((LPI2C_t *)LPI2C1_BASE)
/** Array initializer of LPI2C peripheral base addresses */
#define LPI2C_BASE_ADDRS         \
    {                            \
        LPI2C0_BASE, LPI2C1_BASE \
    }
/** Array initializer of LPI2C peripheral base pointers */
#define LPI2C_BASE_PTRS \
    {                   \
        LPI2C0, LPI2C1  \
    }
/** Number of interrupt vector arrays for the LPI2C module. */
#define LPI2C_IRQS_ARR_COUNT (2u)
/** Number of interrupt channels for the MASTER type of LPI2C module. */
#define LPI2C_MASTER_IRQS_CH_COUNT (1u)
/** Number of interrupt channels for the SLAVE type of LPI2C module. */
#define LPI2C_SLAVE_IRQS_CH_COUNT (1u)
/** Interrupt vectors for the LPI2C peripheral type */
#define LPI2C_IRQS               \
    {                            \
        LPI2C0_IRQn, LPI2C1_IRQn \
    }

/* --------------------------------------------------------------------------
   -- LPI2C Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup LPI2C_Register_Masks LPI2C Register Masks
 * @{
 */

/* VERID Bit Fields */
#define LPI2C_VERID_FEATURE_MASK  0xFFFFu
#define LPI2C_VERID_FEATURE_SHIFT 0u
#define LPI2C_VERID_FEATURE_WIDTH 16u
#define LPI2C_VERID_FEATURE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_VERID_FEATURE_SHIFT)) & LPI2C_VERID_FEATURE_MASK)
#define LPI2C_VERID_MINOR_MASK  0xFF0000u
#define LPI2C_VERID_MINOR_SHIFT 16u
#define LPI2C_VERID_MINOR_WIDTH 8u
#define LPI2C_VERID_MINOR(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_VERID_MINOR_SHIFT)) & LPI2C_VERID_MINOR_MASK)
#define LPI2C_VERID_MAJOR_MASK  0xFF000000u
#define LPI2C_VERID_MAJOR_SHIFT 24u
#define LPI2C_VERID_MAJOR_WIDTH 8u
#define LPI2C_VERID_MAJOR(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_VERID_MAJOR_SHIFT)) & LPI2C_VERID_MAJOR_MASK)
/* PARAM Bit Fields */
#define LPI2C_PARAM_MTXFIFO_MASK  0xFu
#define LPI2C_PARAM_MTXFIFO_SHIFT 0u
#define LPI2C_PARAM_MTXFIFO_WIDTH 4u
#define LPI2C_PARAM_MTXFIFO(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_PARAM_MTXFIFO_SHIFT)) & LPI2C_PARAM_MTXFIFO_MASK)
#define LPI2C_PARAM_MRXFIFO_MASK  0xF00u
#define LPI2C_PARAM_MRXFIFO_SHIFT 8u
#define LPI2C_PARAM_MRXFIFO_WIDTH 4u
#define LPI2C_PARAM_MRXFIFO(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_PARAM_MRXFIFO_SHIFT)) & LPI2C_PARAM_MRXFIFO_MASK)
/* MCR Bit Fields */
#define LPI2C_MCR_MEN_MASK    0x1u
#define LPI2C_MCR_MEN_SHIFT   0u
#define LPI2C_MCR_MEN_WIDTH   1u
#define LPI2C_MCR_MEN(x)      (((uint32_t)(((uint32_t)(x)) << LPI2C_MCR_MEN_SHIFT)) & LPI2C_MCR_MEN_MASK)
#define LPI2C_MCR_RST_MASK    0x2u
#define LPI2C_MCR_RST_SHIFT   1u
#define LPI2C_MCR_RST_WIDTH   1u
#define LPI2C_MCR_RST(x)      (((uint32_t)(((uint32_t)(x)) << LPI2C_MCR_RST_SHIFT)) & LPI2C_MCR_RST_MASK)
#define LPI2C_MCR_DOZEN_MASK  0x4u
#define LPI2C_MCR_DOZEN_SHIFT 2u
#define LPI2C_MCR_DOZEN_WIDTH 1u
#define LPI2C_MCR_DOZEN(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCR_DOZEN_SHIFT)) & LPI2C_MCR_DOZEN_MASK)
#define LPI2C_MCR_DBGEN_MASK  0x8u
#define LPI2C_MCR_DBGEN_SHIFT 3u
#define LPI2C_MCR_DBGEN_WIDTH 1u
#define LPI2C_MCR_DBGEN(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCR_DBGEN_SHIFT)) & LPI2C_MCR_DBGEN_MASK)
#define LPI2C_MCR_RTF_MASK  0x100u
#define LPI2C_MCR_RTF_SHIFT 8u
#define LPI2C_MCR_RTF_WIDTH 1u
#define LPI2C_MCR_RTF(x)    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCR_RTF_SHIFT)) & LPI2C_MCR_RTF_MASK)
#define LPI2C_MCR_RRF_MASK  0x200u
#define LPI2C_MCR_RRF_SHIFT 9u
#define LPI2C_MCR_RRF_WIDTH 1u
#define LPI2C_MCR_RRF(x)    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCR_RRF_SHIFT)) & LPI2C_MCR_RRF_MASK)
/* MSR Bit Fields */
#define LPI2C_MSR_TDF_MASK   0x1u
#define LPI2C_MSR_TDF_SHIFT  0u
#define LPI2C_MSR_TDF_WIDTH  1u
#define LPI2C_MSR_TDF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_MSR_TDF_SHIFT)) & LPI2C_MSR_TDF_MASK)
#define LPI2C_MSR_RDF_MASK   0x2u
#define LPI2C_MSR_RDF_SHIFT  1u
#define LPI2C_MSR_RDF_WIDTH  1u
#define LPI2C_MSR_RDF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_MSR_RDF_SHIFT)) & LPI2C_MSR_RDF_MASK)
#define LPI2C_MSR_EPF_MASK   0x100u
#define LPI2C_MSR_EPF_SHIFT  8u
#define LPI2C_MSR_EPF_WIDTH  1u
#define LPI2C_MSR_EPF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_MSR_EPF_SHIFT)) & LPI2C_MSR_EPF_MASK)
#define LPI2C_MSR_SDF_MASK   0x200u
#define LPI2C_MSR_SDF_SHIFT  9u
#define LPI2C_MSR_SDF_WIDTH  1u
#define LPI2C_MSR_SDF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_MSR_SDF_SHIFT)) & LPI2C_MSR_SDF_MASK)
#define LPI2C_MSR_NDF_MASK   0x400u
#define LPI2C_MSR_NDF_SHIFT  10u
#define LPI2C_MSR_NDF_WIDTH  1u
#define LPI2C_MSR_NDF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_MSR_NDF_SHIFT)) & LPI2C_MSR_NDF_MASK)
#define LPI2C_MSR_ALF_MASK   0x800u
#define LPI2C_MSR_ALF_SHIFT  11u
#define LPI2C_MSR_ALF_WIDTH  1u
#define LPI2C_MSR_ALF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_MSR_ALF_SHIFT)) & LPI2C_MSR_ALF_MASK)
#define LPI2C_MSR_FEF_MASK   0x1000u
#define LPI2C_MSR_FEF_SHIFT  12u
#define LPI2C_MSR_FEF_WIDTH  1u
#define LPI2C_MSR_FEF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_MSR_FEF_SHIFT)) & LPI2C_MSR_FEF_MASK)
#define LPI2C_MSR_PLTF_MASK  0x2000u
#define LPI2C_MSR_PLTF_SHIFT 13u
#define LPI2C_MSR_PLTF_WIDTH 1u
#define LPI2C_MSR_PLTF(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MSR_PLTF_SHIFT)) & LPI2C_MSR_PLTF_MASK)
#define LPI2C_MSR_DMF_MASK  0x4000u
#define LPI2C_MSR_DMF_SHIFT 14u
#define LPI2C_MSR_DMF_WIDTH 1u
#define LPI2C_MSR_DMF(x)    (((uint32_t)(((uint32_t)(x)) << LPI2C_MSR_DMF_SHIFT)) & LPI2C_MSR_DMF_MASK)
#define LPI2C_MSR_MBF_MASK  0x1000000u
#define LPI2C_MSR_MBF_SHIFT 24u
#define LPI2C_MSR_MBF_WIDTH 1u
#define LPI2C_MSR_MBF(x)    (((uint32_t)(((uint32_t)(x)) << LPI2C_MSR_MBF_SHIFT)) & LPI2C_MSR_MBF_MASK)
#define LPI2C_MSR_BBF_MASK  0x2000000u
#define LPI2C_MSR_BBF_SHIFT 25u
#define LPI2C_MSR_BBF_WIDTH 1u
#define LPI2C_MSR_BBF(x)    (((uint32_t)(((uint32_t)(x)) << LPI2C_MSR_BBF_SHIFT)) & LPI2C_MSR_BBF_MASK)
/* MIER Bit Fields */
#define LPI2C_MIER_TDIE_MASK  0x1u
#define LPI2C_MIER_TDIE_SHIFT 0u
#define LPI2C_MIER_TDIE_WIDTH 1u
#define LPI2C_MIER_TDIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MIER_TDIE_SHIFT)) & LPI2C_MIER_TDIE_MASK)
#define LPI2C_MIER_RDIE_MASK  0x2u
#define LPI2C_MIER_RDIE_SHIFT 1u
#define LPI2C_MIER_RDIE_WIDTH 1u
#define LPI2C_MIER_RDIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MIER_RDIE_SHIFT)) & LPI2C_MIER_RDIE_MASK)
#define LPI2C_MIER_EPIE_MASK  0x100u
#define LPI2C_MIER_EPIE_SHIFT 8u
#define LPI2C_MIER_EPIE_WIDTH 1u
#define LPI2C_MIER_EPIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MIER_EPIE_SHIFT)) & LPI2C_MIER_EPIE_MASK)
#define LPI2C_MIER_SDIE_MASK  0x200u
#define LPI2C_MIER_SDIE_SHIFT 9u
#define LPI2C_MIER_SDIE_WIDTH 1u
#define LPI2C_MIER_SDIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MIER_SDIE_SHIFT)) & LPI2C_MIER_SDIE_MASK)
#define LPI2C_MIER_NDIE_MASK  0x400u
#define LPI2C_MIER_NDIE_SHIFT 10u
#define LPI2C_MIER_NDIE_WIDTH 1u
#define LPI2C_MIER_NDIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MIER_NDIE_SHIFT)) & LPI2C_MIER_NDIE_MASK)
#define LPI2C_MIER_ALIE_MASK  0x800u
#define LPI2C_MIER_ALIE_SHIFT 11u
#define LPI2C_MIER_ALIE_WIDTH 1u
#define LPI2C_MIER_ALIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MIER_ALIE_SHIFT)) & LPI2C_MIER_ALIE_MASK)
#define LPI2C_MIER_FEIE_MASK  0x1000u
#define LPI2C_MIER_FEIE_SHIFT 12u
#define LPI2C_MIER_FEIE_WIDTH 1u
#define LPI2C_MIER_FEIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MIER_FEIE_SHIFT)) & LPI2C_MIER_FEIE_MASK)
#define LPI2C_MIER_PLTIE_MASK  0x2000u
#define LPI2C_MIER_PLTIE_SHIFT 13u
#define LPI2C_MIER_PLTIE_WIDTH 1u
#define LPI2C_MIER_PLTIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MIER_PLTIE_SHIFT)) & LPI2C_MIER_PLTIE_MASK)
#define LPI2C_MIER_DMIE_MASK  0x4000u
#define LPI2C_MIER_DMIE_SHIFT 14u
#define LPI2C_MIER_DMIE_WIDTH 1u
#define LPI2C_MIER_DMIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MIER_DMIE_SHIFT)) & LPI2C_MIER_DMIE_MASK)
/* MDER Bit Fields */
#define LPI2C_MDER_TDDE_MASK  0x1u
#define LPI2C_MDER_TDDE_SHIFT 0u
#define LPI2C_MDER_TDDE_WIDTH 1u
#define LPI2C_MDER_TDDE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MDER_TDDE_SHIFT)) & LPI2C_MDER_TDDE_MASK)
#define LPI2C_MDER_RDDE_MASK  0x2u
#define LPI2C_MDER_RDDE_SHIFT 1u
#define LPI2C_MDER_RDDE_WIDTH 1u
#define LPI2C_MDER_RDDE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MDER_RDDE_SHIFT)) & LPI2C_MDER_RDDE_MASK)
/* MCFGR0 Bit Fields */
#define LPI2C_MCFGR0_HREN_MASK  0x1u
#define LPI2C_MCFGR0_HREN_SHIFT 0u
#define LPI2C_MCFGR0_HREN_WIDTH 1u
#define LPI2C_MCFGR0_HREN(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCFGR0_HREN_SHIFT)) & LPI2C_MCFGR0_HREN_MASK)
#define LPI2C_MCFGR0_HRPOL_MASK  0x2u
#define LPI2C_MCFGR0_HRPOL_SHIFT 1u
#define LPI2C_MCFGR0_HRPOL_WIDTH 1u
#define LPI2C_MCFGR0_HRPOL(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCFGR0_HRPOL_SHIFT)) & LPI2C_MCFGR0_HRPOL_MASK)
#define LPI2C_MCFGR0_HRSEL_MASK  0x4u
#define LPI2C_MCFGR0_HRSEL_SHIFT 2u
#define LPI2C_MCFGR0_HRSEL_WIDTH 1u
#define LPI2C_MCFGR0_HRSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCFGR0_HRSEL_SHIFT)) & LPI2C_MCFGR0_HRSEL_MASK)
#define LPI2C_MCFGR0_CIRFIFO_MASK  0x100u
#define LPI2C_MCFGR0_CIRFIFO_SHIFT 8u
#define LPI2C_MCFGR0_CIRFIFO_WIDTH 1u
#define LPI2C_MCFGR0_CIRFIFO(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCFGR0_CIRFIFO_SHIFT)) & LPI2C_MCFGR0_CIRFIFO_MASK)
#define LPI2C_MCFGR0_RDMO_MASK  0x200u
#define LPI2C_MCFGR0_RDMO_SHIFT 9u
#define LPI2C_MCFGR0_RDMO_WIDTH 1u
#define LPI2C_MCFGR0_RDMO(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCFGR0_RDMO_SHIFT)) & LPI2C_MCFGR0_RDMO_MASK)
/* MCFGR1 Bit Fields */
#define LPI2C_MCFGR1_PRESCALE_MASK  0x7u
#define LPI2C_MCFGR1_PRESCALE_SHIFT 0u
#define LPI2C_MCFGR1_PRESCALE_WIDTH 3u
#define LPI2C_MCFGR1_PRESCALE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCFGR1_PRESCALE_SHIFT)) & LPI2C_MCFGR1_PRESCALE_MASK)
#define LPI2C_MCFGR1_AUTOSTOP_MASK  0x100u
#define LPI2C_MCFGR1_AUTOSTOP_SHIFT 8u
#define LPI2C_MCFGR1_AUTOSTOP_WIDTH 1u
#define LPI2C_MCFGR1_AUTOSTOP(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCFGR1_AUTOSTOP_SHIFT)) & LPI2C_MCFGR1_AUTOSTOP_MASK)
#define LPI2C_MCFGR1_IGNACK_MASK  0x200u
#define LPI2C_MCFGR1_IGNACK_SHIFT 9u
#define LPI2C_MCFGR1_IGNACK_WIDTH 1u
#define LPI2C_MCFGR1_IGNACK(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCFGR1_IGNACK_SHIFT)) & LPI2C_MCFGR1_IGNACK_MASK)
#define LPI2C_MCFGR1_TIMECFG_MASK  0x400u
#define LPI2C_MCFGR1_TIMECFG_SHIFT 10u
#define LPI2C_MCFGR1_TIMECFG_WIDTH 1u
#define LPI2C_MCFGR1_TIMECFG(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCFGR1_TIMECFG_SHIFT)) & LPI2C_MCFGR1_TIMECFG_MASK)
#define LPI2C_MCFGR1_MATCFG_MASK  0x70000u
#define LPI2C_MCFGR1_MATCFG_SHIFT 16u
#define LPI2C_MCFGR1_MATCFG_WIDTH 3u
#define LPI2C_MCFGR1_MATCFG(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCFGR1_MATCFG_SHIFT)) & LPI2C_MCFGR1_MATCFG_MASK)
#define LPI2C_MCFGR1_PINCFG_MASK  0x7000000u
#define LPI2C_MCFGR1_PINCFG_SHIFT 24u
#define LPI2C_MCFGR1_PINCFG_WIDTH 3u
#define LPI2C_MCFGR1_PINCFG(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCFGR1_PINCFG_SHIFT)) & LPI2C_MCFGR1_PINCFG_MASK)
/* MCFGR2 Bit Fields */
#define LPI2C_MCFGR2_BUSIDLE_MASK  0xFFFu
#define LPI2C_MCFGR2_BUSIDLE_SHIFT 0u
#define LPI2C_MCFGR2_BUSIDLE_WIDTH 12u
#define LPI2C_MCFGR2_BUSIDLE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCFGR2_BUSIDLE_SHIFT)) & LPI2C_MCFGR2_BUSIDLE_MASK)
#define LPI2C_MCFGR2_FILTSCL_MASK  0xF0000u
#define LPI2C_MCFGR2_FILTSCL_SHIFT 16u
#define LPI2C_MCFGR2_FILTSCL_WIDTH 4u
#define LPI2C_MCFGR2_FILTSCL(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCFGR2_FILTSCL_SHIFT)) & LPI2C_MCFGR2_FILTSCL_MASK)
#define LPI2C_MCFGR2_FILTSDA_MASK  0xF000000u
#define LPI2C_MCFGR2_FILTSDA_SHIFT 24u
#define LPI2C_MCFGR2_FILTSDA_WIDTH 4u
#define LPI2C_MCFGR2_FILTSDA(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCFGR2_FILTSDA_SHIFT)) & LPI2C_MCFGR2_FILTSDA_MASK)
/* MCFGR3 Bit Fields */
#define LPI2C_MCFGR3_PINLOW_MASK  0xFFF00u
#define LPI2C_MCFGR3_PINLOW_SHIFT 8u
#define LPI2C_MCFGR3_PINLOW_WIDTH 12u
#define LPI2C_MCFGR3_PINLOW(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCFGR3_PINLOW_SHIFT)) & LPI2C_MCFGR3_PINLOW_MASK)
/* MDMR Bit Fields */
#define LPI2C_MDMR_MATCH0_MASK  0xFFu
#define LPI2C_MDMR_MATCH0_SHIFT 0u
#define LPI2C_MDMR_MATCH0_WIDTH 8u
#define LPI2C_MDMR_MATCH0(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MDMR_MATCH0_SHIFT)) & LPI2C_MDMR_MATCH0_MASK)
#define LPI2C_MDMR_MATCH1_MASK  0xFF0000u
#define LPI2C_MDMR_MATCH1_SHIFT 16u
#define LPI2C_MDMR_MATCH1_WIDTH 8u
#define LPI2C_MDMR_MATCH1(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MDMR_MATCH1_SHIFT)) & LPI2C_MDMR_MATCH1_MASK)
/* MCCR0 Bit Fields */
#define LPI2C_MCCR0_CLKLO_MASK  0x3Fu
#define LPI2C_MCCR0_CLKLO_SHIFT 0u
#define LPI2C_MCCR0_CLKLO_WIDTH 6u
#define LPI2C_MCCR0_CLKLO(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCCR0_CLKLO_SHIFT)) & LPI2C_MCCR0_CLKLO_MASK)
#define LPI2C_MCCR0_CLKHI_MASK  0x3F00u
#define LPI2C_MCCR0_CLKHI_SHIFT 8u
#define LPI2C_MCCR0_CLKHI_WIDTH 6u
#define LPI2C_MCCR0_CLKHI(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCCR0_CLKHI_SHIFT)) & LPI2C_MCCR0_CLKHI_MASK)
#define LPI2C_MCCR0_SETHOLD_MASK  0x3F0000u
#define LPI2C_MCCR0_SETHOLD_SHIFT 16u
#define LPI2C_MCCR0_SETHOLD_WIDTH 6u
#define LPI2C_MCCR0_SETHOLD(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCCR0_SETHOLD_SHIFT)) & LPI2C_MCCR0_SETHOLD_MASK)
#define LPI2C_MCCR0_DATAVD_MASK  0x3F000000u
#define LPI2C_MCCR0_DATAVD_SHIFT 24u
#define LPI2C_MCCR0_DATAVD_WIDTH 6u
#define LPI2C_MCCR0_DATAVD(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCCR0_DATAVD_SHIFT)) & LPI2C_MCCR0_DATAVD_MASK)
/* MCCR1 Bit Fields */
#define LPI2C_MCCR1_CLKLO_MASK  0x3Fu
#define LPI2C_MCCR1_CLKLO_SHIFT 0u
#define LPI2C_MCCR1_CLKLO_WIDTH 6u
#define LPI2C_MCCR1_CLKLO(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCCR1_CLKLO_SHIFT)) & LPI2C_MCCR1_CLKLO_MASK)
#define LPI2C_MCCR1_CLKHI_MASK  0x3F00u
#define LPI2C_MCCR1_CLKHI_SHIFT 8u
#define LPI2C_MCCR1_CLKHI_WIDTH 6u
#define LPI2C_MCCR1_CLKHI(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCCR1_CLKHI_SHIFT)) & LPI2C_MCCR1_CLKHI_MASK)
#define LPI2C_MCCR1_SETHOLD_MASK  0x3F0000u
#define LPI2C_MCCR1_SETHOLD_SHIFT 16u
#define LPI2C_MCCR1_SETHOLD_WIDTH 6u
#define LPI2C_MCCR1_SETHOLD(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCCR1_SETHOLD_SHIFT)) & LPI2C_MCCR1_SETHOLD_MASK)
#define LPI2C_MCCR1_DATAVD_MASK  0x3F000000u
#define LPI2C_MCCR1_DATAVD_SHIFT 24u
#define LPI2C_MCCR1_DATAVD_WIDTH 6u
#define LPI2C_MCCR1_DATAVD(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MCCR1_DATAVD_SHIFT)) & LPI2C_MCCR1_DATAVD_MASK)
/* MFCR Bit Fields */
#define LPI2C_MFCR_TXWATER_MASK  0x3u
#define LPI2C_MFCR_TXWATER_SHIFT 0u
#define LPI2C_MFCR_TXWATER_WIDTH 2u
#define LPI2C_MFCR_TXWATER(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MFCR_TXWATER_SHIFT)) & LPI2C_MFCR_TXWATER_MASK)
#define LPI2C_MFCR_RXWATER_MASK  0x30000u
#define LPI2C_MFCR_RXWATER_SHIFT 16u
#define LPI2C_MFCR_RXWATER_WIDTH 2u
#define LPI2C_MFCR_RXWATER(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MFCR_RXWATER_SHIFT)) & LPI2C_MFCR_RXWATER_MASK)
/* MFSR Bit Fields */
#define LPI2C_MFSR_TXCOUNT_MASK  0x7u
#define LPI2C_MFSR_TXCOUNT_SHIFT 0u
#define LPI2C_MFSR_TXCOUNT_WIDTH 3u
#define LPI2C_MFSR_TXCOUNT(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MFSR_TXCOUNT_SHIFT)) & LPI2C_MFSR_TXCOUNT_MASK)
#define LPI2C_MFSR_RXCOUNT_MASK  0x70000u
#define LPI2C_MFSR_RXCOUNT_SHIFT 16u
#define LPI2C_MFSR_RXCOUNT_WIDTH 3u
#define LPI2C_MFSR_RXCOUNT(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MFSR_RXCOUNT_SHIFT)) & LPI2C_MFSR_RXCOUNT_MASK)
/* MTDR Bit Fields */
#define LPI2C_MTDR_DATA_MASK  0xFFu
#define LPI2C_MTDR_DATA_SHIFT 0u
#define LPI2C_MTDR_DATA_WIDTH 8u
#define LPI2C_MTDR_DATA(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MTDR_DATA_SHIFT)) & LPI2C_MTDR_DATA_MASK)
#define LPI2C_MTDR_CMD_MASK  0x700u
#define LPI2C_MTDR_CMD_SHIFT 8u
#define LPI2C_MTDR_CMD_WIDTH 3u
#define LPI2C_MTDR_CMD(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MTDR_CMD_SHIFT)) & LPI2C_MTDR_CMD_MASK)
/* MRDR Bit Fields */
#define LPI2C_MRDR_DATA_MASK  0xFFu
#define LPI2C_MRDR_DATA_SHIFT 0u
#define LPI2C_MRDR_DATA_WIDTH 8u
#define LPI2C_MRDR_DATA(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MRDR_DATA_SHIFT)) & LPI2C_MRDR_DATA_MASK)
#define LPI2C_MRDR_RXEMPTY_MASK  0x4000u
#define LPI2C_MRDR_RXEMPTY_SHIFT 14u
#define LPI2C_MRDR_RXEMPTY_WIDTH 1u
#define LPI2C_MRDR_RXEMPTY(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_MRDR_RXEMPTY_SHIFT)) & LPI2C_MRDR_RXEMPTY_MASK)
/* SCR Bit Fields */
#define LPI2C_SCR_SEN_MASK     0x1u
#define LPI2C_SCR_SEN_SHIFT    0u
#define LPI2C_SCR_SEN_WIDTH    1u
#define LPI2C_SCR_SEN(x)       (((uint32_t)(((uint32_t)(x)) << LPI2C_SCR_SEN_SHIFT)) & LPI2C_SCR_SEN_MASK)
#define LPI2C_SCR_RST_MASK     0x2u
#define LPI2C_SCR_RST_SHIFT    1u
#define LPI2C_SCR_RST_WIDTH    1u
#define LPI2C_SCR_RST(x)       (((uint32_t)(((uint32_t)(x)) << LPI2C_SCR_RST_SHIFT)) & LPI2C_SCR_RST_MASK)
#define LPI2C_SCR_FILTEN_MASK  0x10u
#define LPI2C_SCR_FILTEN_SHIFT 4u
#define LPI2C_SCR_FILTEN_WIDTH 1u
#define LPI2C_SCR_FILTEN(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCR_FILTEN_SHIFT)) & LPI2C_SCR_FILTEN_MASK)
#define LPI2C_SCR_FILTDZ_MASK  0x20u
#define LPI2C_SCR_FILTDZ_SHIFT 5u
#define LPI2C_SCR_FILTDZ_WIDTH 1u
#define LPI2C_SCR_FILTDZ(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCR_FILTDZ_SHIFT)) & LPI2C_SCR_FILTDZ_MASK)
#define LPI2C_SCR_RTF_MASK  0x100u
#define LPI2C_SCR_RTF_SHIFT 8u
#define LPI2C_SCR_RTF_WIDTH 1u
#define LPI2C_SCR_RTF(x)    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCR_RTF_SHIFT)) & LPI2C_SCR_RTF_MASK)
#define LPI2C_SCR_RRF_MASK  0x200u
#define LPI2C_SCR_RRF_SHIFT 9u
#define LPI2C_SCR_RRF_WIDTH 1u
#define LPI2C_SCR_RRF(x)    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCR_RRF_SHIFT)) & LPI2C_SCR_RRF_MASK)
/* SSR Bit Fields */
#define LPI2C_SSR_TDF_MASK   0x1u
#define LPI2C_SSR_TDF_SHIFT  0u
#define LPI2C_SSR_TDF_WIDTH  1u
#define LPI2C_SSR_TDF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_SSR_TDF_SHIFT)) & LPI2C_SSR_TDF_MASK)
#define LPI2C_SSR_RDF_MASK   0x2u
#define LPI2C_SSR_RDF_SHIFT  1u
#define LPI2C_SSR_RDF_WIDTH  1u
#define LPI2C_SSR_RDF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_SSR_RDF_SHIFT)) & LPI2C_SSR_RDF_MASK)
#define LPI2C_SSR_AVF_MASK   0x4u
#define LPI2C_SSR_AVF_SHIFT  2u
#define LPI2C_SSR_AVF_WIDTH  1u
#define LPI2C_SSR_AVF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_SSR_AVF_SHIFT)) & LPI2C_SSR_AVF_MASK)
#define LPI2C_SSR_TAF_MASK   0x8u
#define LPI2C_SSR_TAF_SHIFT  3u
#define LPI2C_SSR_TAF_WIDTH  1u
#define LPI2C_SSR_TAF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_SSR_TAF_SHIFT)) & LPI2C_SSR_TAF_MASK)
#define LPI2C_SSR_RSF_MASK   0x100u
#define LPI2C_SSR_RSF_SHIFT  8u
#define LPI2C_SSR_RSF_WIDTH  1u
#define LPI2C_SSR_RSF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_SSR_RSF_SHIFT)) & LPI2C_SSR_RSF_MASK)
#define LPI2C_SSR_SDF_MASK   0x200u
#define LPI2C_SSR_SDF_SHIFT  9u
#define LPI2C_SSR_SDF_WIDTH  1u
#define LPI2C_SSR_SDF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_SSR_SDF_SHIFT)) & LPI2C_SSR_SDF_MASK)
#define LPI2C_SSR_BEF_MASK   0x400u
#define LPI2C_SSR_BEF_SHIFT  10u
#define LPI2C_SSR_BEF_WIDTH  1u
#define LPI2C_SSR_BEF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_SSR_BEF_SHIFT)) & LPI2C_SSR_BEF_MASK)
#define LPI2C_SSR_FEF_MASK   0x800u
#define LPI2C_SSR_FEF_SHIFT  11u
#define LPI2C_SSR_FEF_WIDTH  1u
#define LPI2C_SSR_FEF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_SSR_FEF_SHIFT)) & LPI2C_SSR_FEF_MASK)
#define LPI2C_SSR_AM0F_MASK  0x1000u
#define LPI2C_SSR_AM0F_SHIFT 12u
#define LPI2C_SSR_AM0F_WIDTH 1u
#define LPI2C_SSR_AM0F(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SSR_AM0F_SHIFT)) & LPI2C_SSR_AM0F_MASK)
#define LPI2C_SSR_AM1F_MASK  0x2000u
#define LPI2C_SSR_AM1F_SHIFT 13u
#define LPI2C_SSR_AM1F_WIDTH 1u
#define LPI2C_SSR_AM1F(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SSR_AM1F_SHIFT)) & LPI2C_SSR_AM1F_MASK)
#define LPI2C_SSR_GCF_MASK   0x4000u
#define LPI2C_SSR_GCF_SHIFT  14u
#define LPI2C_SSR_GCF_WIDTH  1u
#define LPI2C_SSR_GCF(x)     (((uint32_t)(((uint32_t)(x)) << LPI2C_SSR_GCF_SHIFT)) & LPI2C_SSR_GCF_MASK)
#define LPI2C_SSR_SARF_MASK  0x8000u
#define LPI2C_SSR_SARF_SHIFT 15u
#define LPI2C_SSR_SARF_WIDTH 1u
#define LPI2C_SSR_SARF(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SSR_SARF_SHIFT)) & LPI2C_SSR_SARF_MASK)
#define LPI2C_SSR_SBF_MASK  0x1000000u
#define LPI2C_SSR_SBF_SHIFT 24u
#define LPI2C_SSR_SBF_WIDTH 1u
#define LPI2C_SSR_SBF(x)    (((uint32_t)(((uint32_t)(x)) << LPI2C_SSR_SBF_SHIFT)) & LPI2C_SSR_SBF_MASK)
#define LPI2C_SSR_BBF_MASK  0x2000000u
#define LPI2C_SSR_BBF_SHIFT 25u
#define LPI2C_SSR_BBF_WIDTH 1u
#define LPI2C_SSR_BBF(x)    (((uint32_t)(((uint32_t)(x)) << LPI2C_SSR_BBF_SHIFT)) & LPI2C_SSR_BBF_MASK)
/* SIER Bit Fields */
#define LPI2C_SIER_TDIE_MASK  0x1u
#define LPI2C_SIER_TDIE_SHIFT 0u
#define LPI2C_SIER_TDIE_WIDTH 1u
#define LPI2C_SIER_TDIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SIER_TDIE_SHIFT)) & LPI2C_SIER_TDIE_MASK)
#define LPI2C_SIER_RDIE_MASK  0x2u
#define LPI2C_SIER_RDIE_SHIFT 1u
#define LPI2C_SIER_RDIE_WIDTH 1u
#define LPI2C_SIER_RDIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SIER_RDIE_SHIFT)) & LPI2C_SIER_RDIE_MASK)
#define LPI2C_SIER_AVIE_MASK  0x4u
#define LPI2C_SIER_AVIE_SHIFT 2u
#define LPI2C_SIER_AVIE_WIDTH 1u
#define LPI2C_SIER_AVIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SIER_AVIE_SHIFT)) & LPI2C_SIER_AVIE_MASK)
#define LPI2C_SIER_TAIE_MASK  0x8u
#define LPI2C_SIER_TAIE_SHIFT 3u
#define LPI2C_SIER_TAIE_WIDTH 1u
#define LPI2C_SIER_TAIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SIER_TAIE_SHIFT)) & LPI2C_SIER_TAIE_MASK)
#define LPI2C_SIER_RSIE_MASK  0x100u
#define LPI2C_SIER_RSIE_SHIFT 8u
#define LPI2C_SIER_RSIE_WIDTH 1u
#define LPI2C_SIER_RSIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SIER_RSIE_SHIFT)) & LPI2C_SIER_RSIE_MASK)
#define LPI2C_SIER_SDIE_MASK  0x200u
#define LPI2C_SIER_SDIE_SHIFT 9u
#define LPI2C_SIER_SDIE_WIDTH 1u
#define LPI2C_SIER_SDIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SIER_SDIE_SHIFT)) & LPI2C_SIER_SDIE_MASK)
#define LPI2C_SIER_BEIE_MASK  0x400u
#define LPI2C_SIER_BEIE_SHIFT 10u
#define LPI2C_SIER_BEIE_WIDTH 1u
#define LPI2C_SIER_BEIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SIER_BEIE_SHIFT)) & LPI2C_SIER_BEIE_MASK)
#define LPI2C_SIER_FEIE_MASK  0x800u
#define LPI2C_SIER_FEIE_SHIFT 11u
#define LPI2C_SIER_FEIE_WIDTH 1u
#define LPI2C_SIER_FEIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SIER_FEIE_SHIFT)) & LPI2C_SIER_FEIE_MASK)
#define LPI2C_SIER_AM0IE_MASK  0x1000u
#define LPI2C_SIER_AM0IE_SHIFT 12u
#define LPI2C_SIER_AM0IE_WIDTH 1u
#define LPI2C_SIER_AM0IE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SIER_AM0IE_SHIFT)) & LPI2C_SIER_AM0IE_MASK)
#define LPI2C_SIER_AM1F_MASK  0x2000u
#define LPI2C_SIER_AM1F_SHIFT 13u
#define LPI2C_SIER_AM1F_WIDTH 1u
#define LPI2C_SIER_AM1F(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SIER_AM1F_SHIFT)) & LPI2C_SIER_AM1F_MASK)
#define LPI2C_SIER_GCIE_MASK  0x4000u
#define LPI2C_SIER_GCIE_SHIFT 14u
#define LPI2C_SIER_GCIE_WIDTH 1u
#define LPI2C_SIER_GCIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SIER_GCIE_SHIFT)) & LPI2C_SIER_GCIE_MASK)
#define LPI2C_SIER_SARIE_MASK  0x8000u
#define LPI2C_SIER_SARIE_SHIFT 15u
#define LPI2C_SIER_SARIE_WIDTH 1u
#define LPI2C_SIER_SARIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SIER_SARIE_SHIFT)) & LPI2C_SIER_SARIE_MASK)
/* SDER Bit Fields */
#define LPI2C_SDER_TDDE_MASK  0x1u
#define LPI2C_SDER_TDDE_SHIFT 0u
#define LPI2C_SDER_TDDE_WIDTH 1u
#define LPI2C_SDER_TDDE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SDER_TDDE_SHIFT)) & LPI2C_SDER_TDDE_MASK)
#define LPI2C_SDER_RDDE_MASK  0x2u
#define LPI2C_SDER_RDDE_SHIFT 1u
#define LPI2C_SDER_RDDE_WIDTH 1u
#define LPI2C_SDER_RDDE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SDER_RDDE_SHIFT)) & LPI2C_SDER_RDDE_MASK)
#define LPI2C_SDER_AVDE_MASK  0x4u
#define LPI2C_SDER_AVDE_SHIFT 2u
#define LPI2C_SDER_AVDE_WIDTH 1u
#define LPI2C_SDER_AVDE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SDER_AVDE_SHIFT)) & LPI2C_SDER_AVDE_MASK)
/* SCFGR1 Bit Fields */
#define LPI2C_SCFGR1_ADRSTALL_MASK  0x1u
#define LPI2C_SCFGR1_ADRSTALL_SHIFT 0u
#define LPI2C_SCFGR1_ADRSTALL_WIDTH 1u
#define LPI2C_SCFGR1_ADRSTALL(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCFGR1_ADRSTALL_SHIFT)) & LPI2C_SCFGR1_ADRSTALL_MASK)
#define LPI2C_SCFGR1_RXSTALL_MASK  0x2u
#define LPI2C_SCFGR1_RXSTALL_SHIFT 1u
#define LPI2C_SCFGR1_RXSTALL_WIDTH 1u
#define LPI2C_SCFGR1_RXSTALL(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCFGR1_RXSTALL_SHIFT)) & LPI2C_SCFGR1_RXSTALL_MASK)
#define LPI2C_SCFGR1_TXDSTALL_MASK  0x4u
#define LPI2C_SCFGR1_TXDSTALL_SHIFT 2u
#define LPI2C_SCFGR1_TXDSTALL_WIDTH 1u
#define LPI2C_SCFGR1_TXDSTALL(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCFGR1_TXDSTALL_SHIFT)) & LPI2C_SCFGR1_TXDSTALL_MASK)
#define LPI2C_SCFGR1_ACKSTALL_MASK  0x8u
#define LPI2C_SCFGR1_ACKSTALL_SHIFT 3u
#define LPI2C_SCFGR1_ACKSTALL_WIDTH 1u
#define LPI2C_SCFGR1_ACKSTALL(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCFGR1_ACKSTALL_SHIFT)) & LPI2C_SCFGR1_ACKSTALL_MASK)
#define LPI2C_SCFGR1_GCEN_MASK  0x100u
#define LPI2C_SCFGR1_GCEN_SHIFT 8u
#define LPI2C_SCFGR1_GCEN_WIDTH 1u
#define LPI2C_SCFGR1_GCEN(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCFGR1_GCEN_SHIFT)) & LPI2C_SCFGR1_GCEN_MASK)
#define LPI2C_SCFGR1_SAEN_MASK  0x200u
#define LPI2C_SCFGR1_SAEN_SHIFT 9u
#define LPI2C_SCFGR1_SAEN_WIDTH 1u
#define LPI2C_SCFGR1_SAEN(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCFGR1_SAEN_SHIFT)) & LPI2C_SCFGR1_SAEN_MASK)
#define LPI2C_SCFGR1_TXCFG_MASK  0x400u
#define LPI2C_SCFGR1_TXCFG_SHIFT 10u
#define LPI2C_SCFGR1_TXCFG_WIDTH 1u
#define LPI2C_SCFGR1_TXCFG(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCFGR1_TXCFG_SHIFT)) & LPI2C_SCFGR1_TXCFG_MASK)
#define LPI2C_SCFGR1_RXCFG_MASK  0x800u
#define LPI2C_SCFGR1_RXCFG_SHIFT 11u
#define LPI2C_SCFGR1_RXCFG_WIDTH 1u
#define LPI2C_SCFGR1_RXCFG(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCFGR1_RXCFG_SHIFT)) & LPI2C_SCFGR1_RXCFG_MASK)
#define LPI2C_SCFGR1_IGNACK_MASK  0x1000u
#define LPI2C_SCFGR1_IGNACK_SHIFT 12u
#define LPI2C_SCFGR1_IGNACK_WIDTH 1u
#define LPI2C_SCFGR1_IGNACK(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCFGR1_IGNACK_SHIFT)) & LPI2C_SCFGR1_IGNACK_MASK)
#define LPI2C_SCFGR1_HSMEN_MASK  0x2000u
#define LPI2C_SCFGR1_HSMEN_SHIFT 13u
#define LPI2C_SCFGR1_HSMEN_WIDTH 1u
#define LPI2C_SCFGR1_HSMEN(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCFGR1_HSMEN_SHIFT)) & LPI2C_SCFGR1_HSMEN_MASK)
#define LPI2C_SCFGR1_ADDRCFG_MASK  0x70000u
#define LPI2C_SCFGR1_ADDRCFG_SHIFT 16u
#define LPI2C_SCFGR1_ADDRCFG_WIDTH 3u
#define LPI2C_SCFGR1_ADDRCFG(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCFGR1_ADDRCFG_SHIFT)) & LPI2C_SCFGR1_ADDRCFG_MASK)
/* SCFGR2 Bit Fields */
#define LPI2C_SCFGR2_CLKHOLD_MASK  0xFu
#define LPI2C_SCFGR2_CLKHOLD_SHIFT 0u
#define LPI2C_SCFGR2_CLKHOLD_WIDTH 4u
#define LPI2C_SCFGR2_CLKHOLD(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCFGR2_CLKHOLD_SHIFT)) & LPI2C_SCFGR2_CLKHOLD_MASK)
#define LPI2C_SCFGR2_DATAVD_MASK  0x3F00u
#define LPI2C_SCFGR2_DATAVD_SHIFT 8u
#define LPI2C_SCFGR2_DATAVD_WIDTH 6u
#define LPI2C_SCFGR2_DATAVD(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCFGR2_DATAVD_SHIFT)) & LPI2C_SCFGR2_DATAVD_MASK)
#define LPI2C_SCFGR2_FILTSCL_MASK  0xF0000u
#define LPI2C_SCFGR2_FILTSCL_SHIFT 16u
#define LPI2C_SCFGR2_FILTSCL_WIDTH 4u
#define LPI2C_SCFGR2_FILTSCL(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCFGR2_FILTSCL_SHIFT)) & LPI2C_SCFGR2_FILTSCL_MASK)
#define LPI2C_SCFGR2_FILTSDA_MASK  0xF000000u
#define LPI2C_SCFGR2_FILTSDA_SHIFT 24u
#define LPI2C_SCFGR2_FILTSDA_WIDTH 4u
#define LPI2C_SCFGR2_FILTSDA(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SCFGR2_FILTSDA_SHIFT)) & LPI2C_SCFGR2_FILTSDA_MASK)
/* SAMR Bit Fields */
#define LPI2C_SAMR_ADDR0_MASK  0x7FEu
#define LPI2C_SAMR_ADDR0_SHIFT 1u
#define LPI2C_SAMR_ADDR0_WIDTH 10u
#define LPI2C_SAMR_ADDR0(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SAMR_ADDR0_SHIFT)) & LPI2C_SAMR_ADDR0_MASK)
#define LPI2C_SAMR_ADDR1_MASK  0x7FE0000u
#define LPI2C_SAMR_ADDR1_SHIFT 17u
#define LPI2C_SAMR_ADDR1_WIDTH 10u
#define LPI2C_SAMR_ADDR1(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SAMR_ADDR1_SHIFT)) & LPI2C_SAMR_ADDR1_MASK)
/* SASR Bit Fields */
#define LPI2C_SASR_RADDR_MASK  0x7FFu
#define LPI2C_SASR_RADDR_SHIFT 0u
#define LPI2C_SASR_RADDR_WIDTH 11u
#define LPI2C_SASR_RADDR(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SASR_RADDR_SHIFT)) & LPI2C_SASR_RADDR_MASK)
#define LPI2C_SASR_ANV_MASK  0x4000u
#define LPI2C_SASR_ANV_SHIFT 14u
#define LPI2C_SASR_ANV_WIDTH 1u
#define LPI2C_SASR_ANV(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SASR_ANV_SHIFT)) & LPI2C_SASR_ANV_MASK)
/* STAR Bit Fields */
#define LPI2C_STAR_TXNACK_MASK  0x1u
#define LPI2C_STAR_TXNACK_SHIFT 0u
#define LPI2C_STAR_TXNACK_WIDTH 1u
#define LPI2C_STAR_TXNACK(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_STAR_TXNACK_SHIFT)) & LPI2C_STAR_TXNACK_MASK)
/* STDR Bit Fields */
#define LPI2C_STDR_DATA_MASK  0xFFu
#define LPI2C_STDR_DATA_SHIFT 0u
#define LPI2C_STDR_DATA_WIDTH 8u
#define LPI2C_STDR_DATA(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_STDR_DATA_SHIFT)) & LPI2C_STDR_DATA_MASK)
/* SRDR Bit Fields */
#define LPI2C_SRDR_DATA_MASK  0xFFu
#define LPI2C_SRDR_DATA_SHIFT 0u
#define LPI2C_SRDR_DATA_WIDTH 8u
#define LPI2C_SRDR_DATA(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SRDR_DATA_SHIFT)) & LPI2C_SRDR_DATA_MASK)
#define LPI2C_SRDR_RXEMPTY_MASK  0x4000u
#define LPI2C_SRDR_RXEMPTY_SHIFT 14u
#define LPI2C_SRDR_RXEMPTY_WIDTH 1u
#define LPI2C_SRDR_RXEMPTY(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SRDR_RXEMPTY_SHIFT)) & LPI2C_SRDR_RXEMPTY_MASK)
#define LPI2C_SRDR_SOF_MASK  0x8000u
#define LPI2C_SRDR_SOF_SHIFT 15u
#define LPI2C_SRDR_SOF_WIDTH 1u
#define LPI2C_SRDR_SOF(x) \
    (((uint32_t)(((uint32_t)(x)) << LPI2C_SRDR_SOF_SHIFT)) & LPI2C_SRDR_SOF_MASK)

/**
 * @}
 */ /* end of group LPI2C_Register_Masks */

/**
 * @}
 */ /* end of group LPI2C_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------
   -- LPSPI Peripheral Access Layer
   ---------------------------------------------------------------------- */

/**
 * @addtogroup LPSPI_Peripheral_Access_Layer LPSPI Peripheral Access Layer
 * @{
 */

/** LPSPI - Size of Registers Arrays */

/** LPSPI - Register Layout Typedef */
typedef struct
{
    __I uint32_t VERID; /**< offset: 0x0 */
    __I uint32_t PARAM; /**< offset: 0x4 */
    _NA uint8_t  RESERVED_0[8];
    _IO uint32_t CR;    /**< offset: 0x10 */
    _IO uint32_t SR;    /**< offset: 0x14 */
    _IO uint32_t IER;   /**< offset: 0x18 */
    _IO uint32_t DER;   /**< offset: 0x1C */
    _IO uint32_t CFGR0; /**< offset: 0x20 */
    _IO uint32_t CFGR1; /**< offset: 0x24 */
    _NA uint8_t  RESERVED_1[8];
    _IO uint32_t DMR0;  /**< offset: 0x30 */
    _IO uint32_t DMR1;  /**< offset: 0x34 */
    _NA uint8_t  RESERVED_2[8];
    _IO uint32_t CCR;   /**< offset: 0x40 */
    _NA uint8_t  RESERVED_3[20];
    _IO uint32_t FCR;   /**< offset: 0x58 */
    __I uint32_t FSR;   /**< offset: 0x5C */
    _IO uint32_t TCR;   /**< offset: 0x60 */
    __O uint32_t TDR;   /**< offset: 0x64 */
    _NA uint8_t  RESERVED_4[8];
    __I uint32_t RSR;   /**< offset: 0x70 */
    __I uint32_t RDR;   /**< offset: 0x74 */
    _IO uint32_t XCSR;  /**< offset: 0x7C */
} LPSPI_t, *LPSPI_MemMapPtr;

/** Number of instances of the LPSPI module. */
#define LPSPI_INSTANCE_COUNT (3u)

/* LPSPI - Peripheral instance base addresses */
/** Peripheral LPSPI0 base address */
#define LPSPI0_BASE (0x4002C000u)
/** Peripheral LPSPI0 base pointer */
#define LPSPI0 ((LPSPI_t *)LPSPI0_BASE)
/** Peripheral LPSPI1 base address */
#define LPSPI1_BASE (0x4002D000u)
/** Peripheral LPSPI1 base pointer */
#define LPSPI1 ((LPSPI_t *)LPSPI1_BASE)
/** Peripheral LPSPI2 base address */
#define LPSPI2_BASE (0x4002E000u)
/** Peripheral LPSPI2 base pointer */
#define LPSPI2 ((LPSPI_t *)LPSPI2_BASE)
/** Array initializer of LPSPI peripheral base addresses */
#define LPSPI_BASE_ADDRS                      \
    {                                         \
        LPSPI0_BASE, LPSPI1_BASE, LPSPI2_BASE \
    }
/** Array initializer of LPSPI peripheral base pointers */
#define LPSPI_BASE_PTRS        \
    {                          \
        LPSPI0, LPSPI1, LPSPI2 \
    }
/** Number of interrupt vector arrays for the LPSPI module. */
#define LPSPI_IRQS_ARR_COUNT (1u)
/** Number of interrupt channels for the LPSPI module. */
#define LPSPI_IRQS_CH_COUNT (1u)
/** Interrupt vectors for the LPSPI peripheral type */
#define LPSPI_IRQS                            \
    {                                         \
        LPSPI0_IRQn, LPSPI1_IRQn, LPSPI2_IRQn \
    }

/* --------------------------------------------------------------------------
   -- LPSPI Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup LPSPI_Register_Masks LPSPI Register Masks
 * @{
 */

/* VERID Bit Fields */
#define LPSPI_VERID_FEATURE_MASK  0xFFFFu
#define LPSPI_VERID_FEATURE_SHIFT 0u
#define LPSPI_VERID_FEATURE_WIDTH 16u
#define LPSPI_VERID_FEATURE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_VERID_FEATURE_SHIFT)) & LPSPI_VERID_FEATURE_MASK)
#define LPSPI_VERID_MINOR_MASK  0xFF0000u
#define LPSPI_VERID_MINOR_SHIFT 16u
#define LPSPI_VERID_MINOR_WIDTH 8u
#define LPSPI_VERID_MINOR(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_VERID_MINOR_SHIFT)) & LPSPI_VERID_MINOR_MASK)
#define LPSPI_VERID_MAJOR_MASK  0xFF000000u
#define LPSPI_VERID_MAJOR_SHIFT 24u
#define LPSPI_VERID_MAJOR_WIDTH 8u
#define LPSPI_VERID_MAJOR(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_VERID_MAJOR_SHIFT)) & LPSPI_VERID_MAJOR_MASK)
/* PARAM Bit Fields */
#define LPSPI_PARAM_TXFIFO_MASK  0xFFu
#define LPSPI_PARAM_TXFIFO_SHIFT 0u
#define LPSPI_PARAM_TXFIFO_WIDTH 8u
#define LPSPI_PARAM_TXFIFO(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_PARAM_TXFIFO_SHIFT)) & LPSPI_PARAM_TXFIFO_MASK)
#define LPSPI_PARAM_RXFIFO_MASK  0xFF00u
#define LPSPI_PARAM_RXFIFO_SHIFT 8u
#define LPSPI_PARAM_RXFIFO_WIDTH 8u
#define LPSPI_PARAM_RXFIFO(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_PARAM_RXFIFO_SHIFT)) & LPSPI_PARAM_RXFIFO_MASK)
/* CR Bit Fields */
#define LPSPI_CR_MEN_MASK    0x1u
#define LPSPI_CR_MEN_SHIFT   0u
#define LPSPI_CR_MEN_WIDTH   1u
#define LPSPI_CR_MEN(x)      (((uint32_t)(((uint32_t)(x)) << LPSPI_CR_MEN_SHIFT)) & LPSPI_CR_MEN_MASK)
#define LPSPI_CR_RST_MASK    0x2u
#define LPSPI_CR_RST_SHIFT   1u
#define LPSPI_CR_RST_WIDTH   1u
#define LPSPI_CR_RST(x)      (((uint32_t)(((uint32_t)(x)) << LPSPI_CR_RST_SHIFT)) & LPSPI_CR_RST_MASK)
#define LPSPI_CR_DOZEN_MASK  0x4u
#define LPSPI_CR_DOZEN_SHIFT 2u
#define LPSPI_CR_DOZEN_WIDTH 1u
#define LPSPI_CR_DOZEN(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CR_DOZEN_SHIFT)) & LPSPI_CR_DOZEN_MASK)
#define LPSPI_CR_DBGEN_MASK  0x8u
#define LPSPI_CR_DBGEN_SHIFT 3u
#define LPSPI_CR_DBGEN_WIDTH 1u
#define LPSPI_CR_DBGEN(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CR_DBGEN_SHIFT)) & LPSPI_CR_DBGEN_MASK)
#define LPSPI_CR_RTF_MASK  0x100u
#define LPSPI_CR_RTF_SHIFT 8u
#define LPSPI_CR_RTF_WIDTH 1u
#define LPSPI_CR_RTF(x)    (((uint32_t)(((uint32_t)(x)) << LPSPI_CR_RTF_SHIFT)) & LPSPI_CR_RTF_MASK)
#define LPSPI_CR_RRF_MASK  0x200u
#define LPSPI_CR_RRF_SHIFT 9u
#define LPSPI_CR_RRF_WIDTH 1u
#define LPSPI_CR_RRF(x)    (((uint32_t)(((uint32_t)(x)) << LPSPI_CR_RRF_SHIFT)) & LPSPI_CR_RRF_MASK)
/* SR Bit Fields */
#define LPSPI_SR_TDF_MASK  0x1u
#define LPSPI_SR_TDF_SHIFT 0u
#define LPSPI_SR_TDF_WIDTH 1u
#define LPSPI_SR_TDF(x)    (((uint32_t)(((uint32_t)(x)) << LPSPI_SR_TDF_SHIFT)) & LPSPI_SR_TDF_MASK)
#define LPSPI_SR_RDF_MASK  0x2u
#define LPSPI_SR_RDF_SHIFT 1u
#define LPSPI_SR_RDF_WIDTH 1u
#define LPSPI_SR_RDF(x)    (((uint32_t)(((uint32_t)(x)) << LPSPI_SR_RDF_SHIFT)) & LPSPI_SR_RDF_MASK)
#define LPSPI_SR_WCF_MASK  0x100u
#define LPSPI_SR_WCF_SHIFT 8u
#define LPSPI_SR_WCF_WIDTH 1u
#define LPSPI_SR_WCF(x)    (((uint32_t)(((uint32_t)(x)) << LPSPI_SR_WCF_SHIFT)) & LPSPI_SR_WCF_MASK)
#define LPSPI_SR_FCF_MASK  0x200u
#define LPSPI_SR_FCF_SHIFT 9u
#define LPSPI_SR_FCF_WIDTH 1u
#define LPSPI_SR_FCF(x)    (((uint32_t)(((uint32_t)(x)) << LPSPI_SR_FCF_SHIFT)) & LPSPI_SR_FCF_MASK)
#define LPSPI_SR_TCF_MASK  0x400u
#define LPSPI_SR_TCF_SHIFT 10u
#define LPSPI_SR_TCF_WIDTH 1u
#define LPSPI_SR_TCF(x)    (((uint32_t)(((uint32_t)(x)) << LPSPI_SR_TCF_SHIFT)) & LPSPI_SR_TCF_MASK)
#define LPSPI_SR_TEF_MASK  0x800u
#define LPSPI_SR_TEF_SHIFT 11u
#define LPSPI_SR_TEF_WIDTH 1u
#define LPSPI_SR_TEF(x)    (((uint32_t)(((uint32_t)(x)) << LPSPI_SR_TEF_SHIFT)) & LPSPI_SR_TEF_MASK)
#define LPSPI_SR_REF_MASK  0x1000u
#define LPSPI_SR_REF_SHIFT 12u
#define LPSPI_SR_REF_WIDTH 1u
#define LPSPI_SR_REF(x)    (((uint32_t)(((uint32_t)(x)) << LPSPI_SR_REF_SHIFT)) & LPSPI_SR_REF_MASK)
#define LPSPI_SR_DMF_MASK  0x2000u
#define LPSPI_SR_DMF_SHIFT 13u
#define LPSPI_SR_DMF_WIDTH 1u
#define LPSPI_SR_DMF(x)    (((uint32_t)(((uint32_t)(x)) << LPSPI_SR_DMF_SHIFT)) & LPSPI_SR_DMF_MASK)
#define LPSPI_SR_MBF_MASK  0x1000000u
#define LPSPI_SR_MBF_SHIFT 24u
#define LPSPI_SR_MBF_WIDTH 1u
#define LPSPI_SR_MBF(x)    (((uint32_t)(((uint32_t)(x)) << LPSPI_SR_MBF_SHIFT)) & LPSPI_SR_MBF_MASK)
/* IER Bit Fields */
#define LPSPI_IER_TDIE_MASK  0x1u
#define LPSPI_IER_TDIE_SHIFT 0u
#define LPSPI_IER_TDIE_WIDTH 1u
#define LPSPI_IER_TDIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_IER_TDIE_SHIFT)) & LPSPI_IER_TDIE_MASK)
#define LPSPI_IER_RDIE_MASK  0x2u
#define LPSPI_IER_RDIE_SHIFT 1u
#define LPSPI_IER_RDIE_WIDTH 1u
#define LPSPI_IER_RDIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_IER_RDIE_SHIFT)) & LPSPI_IER_RDIE_MASK)
#define LPSPI_IER_WCIE_MASK  0x100u
#define LPSPI_IER_WCIE_SHIFT 8u
#define LPSPI_IER_WCIE_WIDTH 1u
#define LPSPI_IER_WCIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_IER_WCIE_SHIFT)) & LPSPI_IER_WCIE_MASK)
#define LPSPI_IER_FCIE_MASK  0x200u
#define LPSPI_IER_FCIE_SHIFT 9u
#define LPSPI_IER_FCIE_WIDTH 1u
#define LPSPI_IER_FCIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_IER_FCIE_SHIFT)) & LPSPI_IER_FCIE_MASK)
#define LPSPI_IER_TCIE_MASK  0x400u
#define LPSPI_IER_TCIE_SHIFT 10u
#define LPSPI_IER_TCIE_WIDTH 1u
#define LPSPI_IER_TCIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_IER_TCIE_SHIFT)) & LPSPI_IER_TCIE_MASK)
#define LPSPI_IER_TEIE_MASK  0x800u
#define LPSPI_IER_TEIE_SHIFT 11u
#define LPSPI_IER_TEIE_WIDTH 1u
#define LPSPI_IER_TEIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_IER_TEIE_SHIFT)) & LPSPI_IER_TEIE_MASK)
#define LPSPI_IER_REIE_MASK  0x1000u
#define LPSPI_IER_REIE_SHIFT 12u
#define LPSPI_IER_REIE_WIDTH 1u
#define LPSPI_IER_REIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_IER_REIE_SHIFT)) & LPSPI_IER_REIE_MASK)
#define LPSPI_IER_DMIE_MASK  0x2000u
#define LPSPI_IER_DMIE_SHIFT 13u
#define LPSPI_IER_DMIE_WIDTH 1u
#define LPSPI_IER_DMIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_IER_DMIE_SHIFT)) & LPSPI_IER_DMIE_MASK)
/* DER Bit Fields */
#define LPSPI_DER_TDDE_MASK  0x1u
#define LPSPI_DER_TDDE_SHIFT 0u
#define LPSPI_DER_TDDE_WIDTH 1u
#define LPSPI_DER_TDDE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_DER_TDDE_SHIFT)) & LPSPI_DER_TDDE_MASK)
#define LPSPI_DER_RDDE_MASK  0x2u
#define LPSPI_DER_RDDE_SHIFT 1u
#define LPSPI_DER_RDDE_WIDTH 1u
#define LPSPI_DER_RDDE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_DER_RDDE_SHIFT)) & LPSPI_DER_RDDE_MASK)
/* CFGR0 Bit Fields */
#define LPSPI_CFGR0_HREN_MASK  0x1u
#define LPSPI_CFGR0_HREN_SHIFT 0u
#define LPSPI_CFGR0_HREN_WIDTH 1u
#define LPSPI_CFGR0_HREN(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CFGR0_HREN_SHIFT)) & LPSPI_CFGR0_HREN_MASK)
#define LPSPI_CFGR0_HRPOL_MASK  0x2u
#define LPSPI_CFGR0_HRPOL_SHIFT 1u
#define LPSPI_CFGR0_HRPOL_WIDTH 1u
#define LPSPI_CFGR0_HRPOL(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CFGR0_HRPOL_SHIFT)) & LPSPI_CFGR0_HRPOL_MASK)
#define LPSPI_CFGR0_HRSEL_MASK  0x4u
#define LPSPI_CFGR0_HRSEL_SHIFT 2u
#define LPSPI_CFGR0_HRSEL_WIDTH 1u
#define LPSPI_CFGR0_HRSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CFGR0_HRSEL_SHIFT)) & LPSPI_CFGR0_HRSEL_MASK)
#define LPSPI_CFGR0_CIRFIFO_MASK  0x100u
#define LPSPI_CFGR0_CIRFIFO_SHIFT 8u
#define LPSPI_CFGR0_CIRFIFO_WIDTH 1u
#define LPSPI_CFGR0_CIRFIFO(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CFGR0_CIRFIFO_SHIFT)) & LPSPI_CFGR0_CIRFIFO_MASK)
#define LPSPI_CFGR0_RDMO_MASK  0x200u
#define LPSPI_CFGR0_RDMO_SHIFT 9u
#define LPSPI_CFGR0_RDMO_WIDTH 1u
#define LPSPI_CFGR0_RDMO(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CFGR0_RDMO_SHIFT)) & LPSPI_CFGR0_RDMO_MASK)
/* CFGR1 Bit Fields */
#define LPSPI_CFGR1_MASTER_MASK  0x1u
#define LPSPI_CFGR1_MASTER_SHIFT 0u
#define LPSPI_CFGR1_MASTER_WIDTH 1u
#define LPSPI_CFGR1_MASTER(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CFGR1_MASTER_SHIFT)) & LPSPI_CFGR1_MASTER_MASK)
#define LPSPI_CFGR1_SAMPLE_MASK  0x2u
#define LPSPI_CFGR1_SAMPLE_SHIFT 1u
#define LPSPI_CFGR1_SAMPLE_WIDTH 1u
#define LPSPI_CFGR1_SAMPLE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CFGR1_SAMPLE_SHIFT)) & LPSPI_CFGR1_SAMPLE_MASK)
#define LPSPI_CFGR1_AUTOPCS_MASK  0x4u
#define LPSPI_CFGR1_AUTOPCS_SHIFT 2u
#define LPSPI_CFGR1_AUTOPCS_WIDTH 1u
#define LPSPI_CFGR1_AUTOPCS(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CFGR1_AUTOPCS_SHIFT)) & LPSPI_CFGR1_AUTOPCS_MASK)
#define LPSPI_CFGR1_NOSTALL_MASK  0x8u
#define LPSPI_CFGR1_NOSTALL_SHIFT 3u
#define LPSPI_CFGR1_NOSTALL_WIDTH 1u
#define LPSPI_CFGR1_NOSTALL(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CFGR1_NOSTALL_SHIFT)) & LPSPI_CFGR1_NOSTALL_MASK)
#define LPSPI_CFGR1_PCSPOL_MASK  0xF00u
#define LPSPI_CFGR1_PCSPOL_SHIFT 8u
#define LPSPI_CFGR1_PCSPOL_WIDTH 4u
#define LPSPI_CFGR1_PCSPOL(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CFGR1_PCSPOL_SHIFT)) & LPSPI_CFGR1_PCSPOL_MASK)
#define LPSPI_CFGR1_MATCFG_MASK  0x70000u
#define LPSPI_CFGR1_MATCFG_SHIFT 16u
#define LPSPI_CFGR1_MATCFG_WIDTH 3u
#define LPSPI_CFGR1_MATCFG(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CFGR1_MATCFG_SHIFT)) & LPSPI_CFGR1_MATCFG_MASK)
#define LPSPI_CFGR1_PINCFG_MASK  0x3000000u
#define LPSPI_CFGR1_PINCFG_SHIFT 24u
#define LPSPI_CFGR1_PINCFG_WIDTH 2u
#define LPSPI_CFGR1_PINCFG(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CFGR1_PINCFG_SHIFT)) & LPSPI_CFGR1_PINCFG_MASK)
#define LPSPI_CFGR1_OUTCFG_MASK  0x4000000u
#define LPSPI_CFGR1_OUTCFG_SHIFT 26u
#define LPSPI_CFGR1_OUTCFG_WIDTH 1u
#define LPSPI_CFGR1_OUTCFG(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CFGR1_OUTCFG_SHIFT)) & LPSPI_CFGR1_OUTCFG_MASK)
#define LPSPI_CFGR1_PCSCFG_MASK  0x8000000u
#define LPSPI_CFGR1_PCSCFG_SHIFT 27u
#define LPSPI_CFGR1_PCSCFG_WIDTH 1u
#define LPSPI_CFGR1_PCSCFG(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CFGR1_PCSCFG_SHIFT)) & LPSPI_CFGR1_PCSCFG_MASK)
/* DMR0 Bit Fields */
#define LPSPI_DMR0_MATCH0_MASK  0xFFFFFFFFu
#define LPSPI_DMR0_MATCH0_SHIFT 0u
#define LPSPI_DMR0_MATCH0_WIDTH 32u
#define LPSPI_DMR0_MATCH0(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_DMR0_MATCH0_SHIFT)) & LPSPI_DMR0_MATCH0_MASK)
/* DMR1 Bit Fields */
#define LPSPI_DMR1_MATCH1_MASK  0xFFFFFFFFu
#define LPSPI_DMR1_MATCH1_SHIFT 0u
#define LPSPI_DMR1_MATCH1_WIDTH 32u
#define LPSPI_DMR1_MATCH1(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_DMR1_MATCH1_SHIFT)) & LPSPI_DMR1_MATCH1_MASK)
/* CCR Bit Fields */
#define LPSPI_CCR_SCKDIV_MASK  0xFFu
#define LPSPI_CCR_SCKDIV_SHIFT 0u
#define LPSPI_CCR_SCKDIV_WIDTH 8u
#define LPSPI_CCR_SCKDIV(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CCR_SCKDIV_SHIFT)) & LPSPI_CCR_SCKDIV_MASK)
#define LPSPI_CCR_DBT_MASK     0xFF00u
#define LPSPI_CCR_DBT_SHIFT    8u
#define LPSPI_CCR_DBT_WIDTH    8u
#define LPSPI_CCR_DBT(x)       (((uint32_t)(((uint32_t)(x)) << LPSPI_CCR_DBT_SHIFT)) & LPSPI_CCR_DBT_MASK)
#define LPSPI_CCR_PCSSCK_MASK  0xFF0000u
#define LPSPI_CCR_PCSSCK_SHIFT 16u
#define LPSPI_CCR_PCSSCK_WIDTH 8u
#define LPSPI_CCR_PCSSCK(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CCR_PCSSCK_SHIFT)) & LPSPI_CCR_PCSSCK_MASK)
#define LPSPI_CCR_SCKPCS_MASK  0xFF000000u
#define LPSPI_CCR_SCKPCS_SHIFT 24u
#define LPSPI_CCR_SCKPCS_WIDTH 8u
#define LPSPI_CCR_SCKPCS(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_CCR_SCKPCS_SHIFT)) & LPSPI_CCR_SCKPCS_MASK)
/* FCR Bit Fields */
#define LPSPI_FCR_TXWATER_MASK  0x3u
#define LPSPI_FCR_TXWATER_SHIFT 0u
#define LPSPI_FCR_TXWATER_WIDTH 2u
#define LPSPI_FCR_TXWATER(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_FCR_TXWATER_SHIFT)) & LPSPI_FCR_TXWATER_MASK)
#define LPSPI_FCR_RXWATER_MASK  0x30000u
#define LPSPI_FCR_RXWATER_SHIFT 16u
#define LPSPI_FCR_RXWATER_WIDTH 2u
#define LPSPI_FCR_RXWATER(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_FCR_RXWATER_SHIFT)) & LPSPI_FCR_RXWATER_MASK)
/* FSR Bit Fields */
#define LPSPI_FSR_TXCOUNT_MASK  0x7u
#define LPSPI_FSR_TXCOUNT_SHIFT 0u
#define LPSPI_FSR_TXCOUNT_WIDTH 3u
#define LPSPI_FSR_TXCOUNT(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_FSR_TXCOUNT_SHIFT)) & LPSPI_FSR_TXCOUNT_MASK)
#define LPSPI_FSR_RXCOUNT_MASK  0x70000u
#define LPSPI_FSR_RXCOUNT_SHIFT 16u
#define LPSPI_FSR_RXCOUNT_WIDTH 3u
#define LPSPI_FSR_RXCOUNT(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_FSR_RXCOUNT_SHIFT)) & LPSPI_FSR_RXCOUNT_MASK)
/* TCR Bit Fields */
#define LPSPI_TCR_FRAMESZ_MASK  0xFFFu
#define LPSPI_TCR_FRAMESZ_SHIFT 0u
#define LPSPI_TCR_FRAMESZ_WIDTH 12u
#define LPSPI_TCR_FRAMESZ(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_TCR_FRAMESZ_SHIFT)) & LPSPI_TCR_FRAMESZ_MASK)
#define LPSPI_TCR_WIDTH_MASK  0x30000u
#define LPSPI_TCR_WIDTH_SHIFT 16u
#define LPSPI_TCR_WIDTH_WIDTH 2u
#define LPSPI_TCR_WIDTH(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_TCR_WIDTH_SHIFT)) & LPSPI_TCR_WIDTH_MASK)
#define LPSPI_TCR_TXMSK_MASK  0x40000u
#define LPSPI_TCR_TXMSK_SHIFT 18u
#define LPSPI_TCR_TXMSK_WIDTH 1u
#define LPSPI_TCR_TXMSK(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_TCR_TXMSK_SHIFT)) & LPSPI_TCR_TXMSK_MASK)
#define LPSPI_TCR_RXMSK_MASK  0x80000u
#define LPSPI_TCR_RXMSK_SHIFT 19u
#define LPSPI_TCR_RXMSK_WIDTH 1u
#define LPSPI_TCR_RXMSK(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_TCR_RXMSK_SHIFT)) & LPSPI_TCR_RXMSK_MASK)
#define LPSPI_TCR_CONTC_MASK  0x100000u
#define LPSPI_TCR_CONTC_SHIFT 20u
#define LPSPI_TCR_CONTC_WIDTH 1u
#define LPSPI_TCR_CONTC(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_TCR_CONTC_SHIFT)) & LPSPI_TCR_CONTC_MASK)
#define LPSPI_TCR_CONT_MASK  0x200000u
#define LPSPI_TCR_CONT_SHIFT 21u
#define LPSPI_TCR_CONT_WIDTH 1u
#define LPSPI_TCR_CONT(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_TCR_CONT_SHIFT)) & LPSPI_TCR_CONT_MASK)
#define LPSPI_TCR_BYSW_MASK  0x400000u
#define LPSPI_TCR_BYSW_SHIFT 22u
#define LPSPI_TCR_BYSW_WIDTH 1u
#define LPSPI_TCR_BYSW(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_TCR_BYSW_SHIFT)) & LPSPI_TCR_BYSW_MASK)
#define LPSPI_TCR_LSBF_MASK  0x800000u
#define LPSPI_TCR_LSBF_SHIFT 23u
#define LPSPI_TCR_LSBF_WIDTH 1u
#define LPSPI_TCR_LSBF(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_TCR_LSBF_SHIFT)) & LPSPI_TCR_LSBF_MASK)
#define LPSPI_TCR_PCS_MASK       0x3000000u
#define LPSPI_TCR_PCS_SHIFT      24u
#define LPSPI_TCR_PCS_WIDTH      2u
#define LPSPI_TCR_PCS(x)         (((uint32_t)(((uint32_t)(x)) << LPSPI_TCR_PCS_SHIFT)) & LPSPI_TCR_PCS_MASK)
#define LPSPI_TCR_PRESCALE_MASK  0x38000000u
#define LPSPI_TCR_PRESCALE_SHIFT 27u
#define LPSPI_TCR_PRESCALE_WIDTH 3u
#define LPSPI_TCR_PRESCALE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_TCR_PRESCALE_SHIFT)) & LPSPI_TCR_PRESCALE_MASK)
#define LPSPI_TCR_CPHA_MASK  0x40000000u
#define LPSPI_TCR_CPHA_SHIFT 30u
#define LPSPI_TCR_CPHA_WIDTH 1u
#define LPSPI_TCR_CPHA(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_TCR_CPHA_SHIFT)) & LPSPI_TCR_CPHA_MASK)
#define LPSPI_TCR_CPOL_MASK  0x80000000u
#define LPSPI_TCR_CPOL_SHIFT 31u
#define LPSPI_TCR_CPOL_WIDTH 1u
#define LPSPI_TCR_CPOL(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_TCR_CPOL_SHIFT)) & LPSPI_TCR_CPOL_MASK)
/* TDR Bit Fields */
#define LPSPI_TDR_DATA_MASK  0xFFFFFFFFu
#define LPSPI_TDR_DATA_SHIFT 0u
#define LPSPI_TDR_DATA_WIDTH 32u
#define LPSPI_TDR_DATA(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_TDR_DATA_SHIFT)) & LPSPI_TDR_DATA_MASK)
/* RSR Bit Fields */
#define LPSPI_RSR_SOF_MASK      0x1u
#define LPSPI_RSR_SOF_SHIFT     0u
#define LPSPI_RSR_SOF_WIDTH     1u
#define LPSPI_RSR_SOF(x)        (((uint32_t)(((uint32_t)(x)) << LPSPI_RSR_SOF_SHIFT)) & LPSPI_RSR_SOF_MASK)
#define LPSPI_RSR_RXEMPTY_MASK  0x2u
#define LPSPI_RSR_RXEMPTY_SHIFT 1u
#define LPSPI_RSR_RXEMPTY_WIDTH 1u
#define LPSPI_RSR_RXEMPTY(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_RSR_RXEMPTY_SHIFT)) & LPSPI_RSR_RXEMPTY_MASK)
/* RDR Bit Fields */
#define LPSPI_RDR_DATA_MASK  0xFFFFFFFFu
#define LPSPI_RDR_DATA_SHIFT 0u
#define LPSPI_RDR_DATA_WIDTH 32u
#define LPSPI_RDR_DATA(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_RDR_DATA_SHIFT)) & LPSPI_RDR_DATA_MASK)
/* XCSR Bit Fields */
#define LPSPI_XCSR_MCSEN_MASK  0x200u
#define LPSPI_XCSR_MCSEN_SHIFT 9u
#define LPSPI_XCSR_MCSEN_WIDTH 1u
#define LPSPI_XCSR_MCSEN_DATA(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_XCSR_MCSEN_SHIFT)) & LPSPI_XCSR_MCSEN_MASK)
#define LPSPI_XCSR_MCSO_MASK  0x400u
#define LPSPI_XCSR_MCSO_SHIFT 10u
#define LPSPI_XCSR_MCSO_WIDTH 1u
#define LPSPI_XCSR_MCSO_DATA(x) \
    (((uint32_t)(((uint32_t)(x)) << LPSPI_XCSR_MCSO_SHIFT)) & LPSPI_XCSR_MCSO_MASK)

/**
 * @}
 */ /* end of group LPSPI_Register_Masks */

/**
 * @}
 */ /* end of group LPSPI_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- HSSPI Peripheral Access Layer
   ----------------------------------------------------------------------------
 */

/**
 * @addtogroup HSSPI_Peripheral_Access_Layer HSSPI Peripheral Access Layer
 * @{
 */

/** HSSPI - Size of Registers Arrays */
#define HSSPI_RX_BUF_SIZE   (68u)
#define HSSPI_TX_BUF_SIZE   (68u)
#define HSSPI_RX_SRAM_WORDS (17u)
#define HSSPI_TX_SRAM_WORDS (17u)

/** HSSPI - Register Layout Typedef */
typedef struct
{
    __I uint32_t VERID;                      /**< Version ID Register, offset: 0x0 */
    __I uint32_t PARAM;                      /**< Parameter Register, offset: 0x4 */
    _NA uint32_t RESERVED_0[2];
    _IO uint32_t RST;                        /**< HSSPI Reset Register, offset: 0x10 */
    _IO uint32_t FUN;                        /**< Function Enable Register, offset: 0x14 */
    _IO uint32_t RSPSR;                      /**< Response Status Register, offset: 0x18 */
    _IO uint32_t OCR;                        /**< Operation Code Register, offset: 0x1C */
    _IO uint32_t SR;                         /**< Status Register, offset: 0x20 */
    _IO uint32_t INTMSK;                     /**< Interrupt Mask Register, offset: 0x24 */
    __I uint32_t PL;                         /**< Packet Length Register, offset: 0x28 */
    _IO uint32_t SOR;                        /**< Slave Opiton Register, offset: 0x2C */
    _IO uint32_t LOC;                        /**< TPM Locality Register, offset: 0x30 */
    _IO uint32_t RDFIFOCNT;                  /**< TPM RD FIFO Count Register, offset: 0x34 */
    _IO uint32_t LAST;                       /**< TPM Last status Register, offset: 0x38 */
    __I uint32_t WRFIFOCNT;                  /**< TPM WR FIFO Count Register, offset: 0x3C */
    __I uint32_t ADDRREC;                    /**< TPM Last address Register, offset: 0x40 */
    _NA uint32_t RESERVED_1[15];
    __I uint32_t RXBUF[HSSPI_RX_SRAM_WORDS]; /**< TPM RX Buffer Register, offset: 0x80 */
    _NA uint32_t RESERVED_2[15];
    __O uint32_t TXBUF[HSSPI_TX_SRAM_WORDS]; /**< TPM TX Buffer Register, offset: 0x100 */
    _NA uint32_t RESERVED_3[47];
    _IO uint32_t ACCESS;                     /**< TPM_AccessX Register, offset: 0x200 */
    _IO uint32_t INTEN;                      /**< TPM_INTEN Register, offset: 0x204 */
    _IO uint32_t VECTOR;                     /**< TPM_VECTOR Register, offset: 0x208 */
    _IO uint32_t INTSTS;                     /**< TPM_INTSTS Register, offset: 0x20C */
    _IO uint32_t INTFCAP;                    /**< TPM_INTFCAP Register, offset: 0x210 */
    _IO uint32_t STS;                        /**< TPM_STS Register, offset: 0x214 */
    _IO uint32_t DIDVID;                     /**< TPM_STS Register, offset: 0x218 */
    _IO uint32_t RID;                        /**< TPM_STS Register, offset: 0x21C */
    _IO uint32_t HASHEND;                    /**< TPM_HASH_END Register, offset: 0x220 */
    _IO uint32_t HASHSTART;                  /**< TPM_HASH_START Register, offset: 0x224 */
    _IO uint32_t INFID;                      /**< TPM_INTERFACE_ID Register, offset: 0x228 */
} HSSPI_t, *HSSPI_MemMapPtr;

/** Number of instances of the HSSPI module. */
#define HSSPI_INSTANCE_COUNT (1u)

/* HSSPI - Peripheral instance base addresses */
/** Peripheral HSSPI base address */
#define HSSPI_BASE (0x4002F000u)
/** Peripheral HSSPI base pointer */
#define HSSPI ((HSSPI_t *)HSSPI_BASE)
/** Array initializer of HSSPI peripheral base addresses */
#define HSSPI_BASE_ADDRS \
    {                    \
        HSSPI_BASE       \
    }
/** Array initializer of HSSPI peripheral base pointers */
#define HSSPI_BASE_PTRS \
    {                   \
        HSSPI           \
    }
/** Number of interrupt vector arrays for the HSSPI module. */
#define HSSPI_IRQS_ARR_COUNT (1u)
/** Number of interrupt channels for the RX_TX type of HSSPI module. */
#define HSSPI_RX_TX_IRQS_CH_COUNT (1u)
/** Interrupt vectors for the HSSPI peripheral type */
#define HSSPI_IRQS \
    {              \
        HSSPI_IRQn \
    }

/* ----------------------------------------------------------------------------
   -- HSSPI Register Masks
   ----------------------------------------------------------------------------
 */

/**
 * @addtogroup HSSPI_Register_Masks HSSPI Register Masks
 * @{
 */

/* VERID Bit Fields */
#define HSSPI_VERID_FEATURE_MASK  0xFFFFu
#define HSSPI_VERID_FEATURE_SHIFT 0u
#define HSSPI_VERID_FEATURE_WIDTH 16u
#define HSSPI_VERID_FEATURE(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_VERID_FEATURE_SHIFT)) & HSSPI_VERID_FEATURE_MASK)
#define HSSPI_VERID_MINOR_MASK  0xFF0000u
#define HSSPI_VERID_MINOR_SHIFT 16u
#define HSSPI_VERID_MINOR_WIDTH 8u
#define HSSPI_VERID_MINOR(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_VERID_MINOR_SHIFT)) & HSSPI_VERID_MINOR_MASK)
#define HSSPI_VERID_MAJOR_MASK  0xFF000000u
#define HSSPI_VERID_MAJOR_SHIFT 24u
#define HSSPI_VERID_MAJOR_WIDTH 8u
#define HSSPI_VERID_MAJOR(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_VERID_MAJOR_SHIFT)) & HSSPI_VERID_MAJOR_MASK)
/* PARAM Bit Fields */
#define HSSPI_PARAM_TXFIFO_MASK  0xFFu
#define HSSPI_PARAM_TXFIFO_SHIFT 0u
#define HSSPI_PARAM_TXFIFO_WIDTH 8u
#define HSSPI_PARAM_TXFIFO(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_PARAM_TXFIFO_SHIFT)) & HSSPI_PARAM_TXFIFO_MASK)
#define HSSPI_PARAM_RXFIFO_MASK  0xFF00u
#define HSSPI_PARAM_RXFIFO_SHIFT 8u
#define HSSPI_PARAM_RXFIFO_WIDTH 8u
#define HSSPI_PARAM_RXFIFO(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_PARAM_RXFIFO_SHIFT)) & HSSPI_PARAM_RXFIFO_MASK)
/* RST Bit Fields */
#define HSSPI_RST_ACT_MASK  0x1u
#define HSSPI_RST_ACT_SHIFT 0u
#define HSSPI_RST_ACT_WIDTH 1u
#define HSSPI_RST_ACT(x)    (((uint32_t)(((uint32_t)(x)) << HSSPI_RST_ACT_SHIFT)) & HSSPI_RST_ACT_MASK)
/* FUN Bit Fields */
#define HSSPI_FUN_RPEN_MASK  0x1u
#define HSSPI_FUN_RPEN_SHIFT 0u
#define HSSPI_FUN_RPEN_WIDTH 1u
#define HSSPI_FUN_RPEN(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_FUN_RPEN_SHIFT)) & HSSPI_FUN_RPEN_MASK)
#define HSSPI_FUN_TPMEN_MASK  0x2u
#define HSSPI_FUN_TPMEN_SHIFT 1u
#define HSSPI_FUN_TPMEN_WIDTH 1u
#define HSSPI_FUN_TPMEN(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_FUN_TPMEN_SHIFT)) & HSSPI_FUN_TPMEN_MASK)
#define HSSPI_FUN_TPMWAIT_MASK  0x4u
#define HSSPI_FUN_TPMWAIT_SHIFT 2u
#define HSSPI_FUN_TPMWAIT_WIDTH 1u
#define HSSPI_FUN_TPMWAIT(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_FUN_TPMWAIT_SHIFT)) & HSSPI_FUN_TPMWAIT_MASK)
#define HSSPI_FUN_TPMFIFOWAIT_MASK  0x8u
#define HSSPI_FUN_TPMFIFOWAIT_SHIFT 3u
#define HSSPI_FUN_TPMFIFOWAIT_WIDTH 1u
#define HSSPI_FUN_TPMFIFOWAIT(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_FUN_TPMFIFOWAIT_SHIFT)) & HSSPI_FUN_TPMFIFOWAIT_MASK)
/* RSPSR Bit Fields */
#define HSSPI_RSPSR_STS_MASK  0xFFu
#define HSSPI_RSPSR_STS_SHIFT 0u
#define HSSPI_RSPSR_STS_WIDTH 8u
#define HSSPI_RSPSR_STS(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_RSPSR_STS_SHIFT)) & HSSPI_RSPSR_STS_MASK)
/* OCR Bit Fields */
#define HSSPI_OCR_WR_MASK  0xFFu
#define HSSPI_OCR_WR_SHIFT 0u
#define HSSPI_OCR_WR_WIDTH 8u
#define HSSPI_OCR_WR(x)    (((uint32_t)(((uint32_t)(x)) << HSSPI_OCR_WR_SHIFT)) & HSSPI_OCR_WR_MASK)
#define HSSPI_OCR_RD_MASK  0xFF00u
#define HSSPI_OCR_RD_SHIFT 8u
#define HSSPI_OCR_RD_WIDTH 8u
#define HSSPI_OCR_RD(x)    (((uint32_t)(((uint32_t)(x)) << HSSPI_OCR_RD_SHIFT)) & HSSPI_OCR_RD_MASK)
/* SR Bit Fields */
#define HSSPI_SR_RWCC_MASK    0x1u
#define HSSPI_SR_RWCC_SHIFT   0u
#define HSSPI_SR_RWCC_WIDTH   1u
#define HSSPI_SR_RWCC(x)      (((uint32_t)(((uint32_t)(x)) << HSSPI_SR_RWCC_SHIFT)) & HSSPI_SR_RWCC_MASK)
#define HSSPI_SR_RRCW_MASK    0x2u
#define HSSPI_SR_RRCW_SHIFT   1u
#define HSSPI_SR_RRCW_WIDTH   1u
#define HSSPI_SR_RRCW(x)      (((uint32_t)(((uint32_t)(x)) << HSSPI_SR_RRCW_SHIFT)) & HSSPI_SR_RRCW_MASK)
#define HSSPI_SR_RRCC_MASK    0x4u
#define HSSPI_SR_RRCC_SHIFT   2u
#define HSSPI_SR_RRCC_WIDTH   1u
#define HSSPI_SR_RRCC(x)      (((uint32_t)(((uint32_t)(x)) << HSSPI_SR_RRCC_SHIFT)) & HSSPI_SR_RRCC_MASK)
#define HSSPI_SR_RTFWCC_MASK  0x8u
#define HSSPI_SR_RTFWCC_SHIFT 3u
#define HSSPI_SR_RTFWCC_WIDTH 1u
#define HSSPI_SR_RTFWCC(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_SR_RTFWCC_SHIFT)) & HSSPI_SR_RTFWCC_MASK)
#define HSSPI_SR_BSY_MASK         0x10u
#define HSSPI_SR_BSY_SHIFT        4u
#define HSSPI_SR_BSY_WIDTH        1u
#define HSSPI_SR_BSY(x)           (((uint32_t)(((uint32_t)(x)) << HSSPI_SR_BSY_SHIFT)) & HSSPI_SR_BSY_MASK)
#define HSSPI_SR_LASTADDR80_MASK  0x20u
#define HSSPI_SR_LASTADDR80_SHIFT 5u
#define HSSPI_SR_LASTADDR80_WIDTH 1u
#define HSSPI_SR_LASTADDR80(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_SR_LASTADDR80_SHIFT)) & HSSPI_SR_LASTADDR80_MASK)
/* INTMSK Bit Fields */
#define HSSPI_INTMSK_RWCC_MASK  0x1u
#define HSSPI_INTMSK_RWCC_SHIFT 0u
#define HSSPI_INTMSK_RWCC_WIDTH 1u
#define HSSPI_INTMSK_RWCC(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_INTMSK_RWCC_SHIFT)) & HSSPI_INTMSK_RWCC_MASK)
#define HSSPI_INTMSK_RRCW_MASK  0x2u
#define HSSPI_INTMSK_RRCW_SHIFT 1u
#define HSSPI_INTMSK_RRCW_WIDTH 1u
#define HSSPI_INTMSK_RRCW(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_INTMSK_RRCW_SHIFT)) & HSSPI_INTMSK_RRCW_MASK)
#define HSSPI_INTMSK_RRCC_MASK  0x4u
#define HSSPI_INTMSK_RRCC_SHIFT 2u
#define HSSPI_INTMSK_RRCC_WIDTH 1u
#define HSSPI_INTMSK_RRCC(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_INTMSK_RRCC_SHIFT)) & HSSPI_INTMSK_RRCC_MASK)
#define HSSPI_INTMSK_UPD_MASK  0x100u
#define HSSPI_INTMSK_UPD_SHIFT 8u
#define HSSPI_INTMSK_UPD_WIDTH 1u
#define HSSPI_INTMSK_UPD(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_INTMSK_UPD_SHIFT)) & HSSPI_INTMSK_UPD_MASK)
/* PL Bit Fields */
#define HSSPI_PL_RX_MASK  0xFFu
#define HSSPI_PL_RX_SHIFT 0u
#define HSSPI_PL_RX_WIDTH 8u
/* SOR Bit Fields */
#define HSSPI_SOR_BC_MASK   0x7u
#define HSSPI_SOR_BC_SHIFT  0u
#define HSSPI_SOR_BC_WIDTH  3u
#define HSSPI_SOR_BC(x)     (((uint32_t)(((uint32_t)(x)) << HSSPI_SOR_BC_SHIFT)) & HSSPI_SOR_BC_MASK)
#define HSSPI_SOR_CTS_MASK  0x8u
#define HSSPI_SOR_CTS_SHIFT 3u
#define HSSPI_SOR_CTS_WIDTH 1u
#define HSSPI_SOR_CTS(x)    (((uint32_t)(((uint32_t)(x)) << HSSPI_SOR_CTS_SHIFT)) & HSSPI_SOR_CTS_MASK)
/* TPM Locality Bit Fields */
#define HSSPI_TPM_LOCHEAD_MASK  0xFFu
#define HSSPI_TPM_LOCHEAD_SHIFT 0u
#define HSSPI_TPM_LOCHEAD_WIDTH 8u
#define HSSPI_TPM_LOCHEAD(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_LOCHEAD_SHIFT)) & HSSPI_TPM_LOCHEAD_MASK)
#define HSSPI_TPM_LOCNUM0_MASK  0x100u
#define HSSPI_TPM_LOCNUM0_SHIFT 8u
#define HSSPI_TPM_LOCNUM0_WIDTH 1u
#define HSSPI_TPM_LOCNUM0(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_LOCNUM0_SHIFT)) & HSSPI_TPM_LOCNUM0_MASK)
#define HSSPI_TPM_LOCNUM1_MASK  0x200u
#define HSSPI_TPM_LOCNUM1_SHIFT 9u
#define HSSPI_TPM_LOCNUM1_WIDTH 1u
#define HSSPI_TPM_LOCNUM1(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_LOCNUM1_SHIFT)) & HSSPI_TPM_LOCNUM1_MASK)
#define HSSPI_TPM_LOCNUM2_MASK  0x400u
#define HSSPI_TPM_LOCNUM2_SHIFT 10u
#define HSSPI_TPM_LOCNUM2_WIDTH 1u
#define HSSPI_TPM_LOCNUM2(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_LOCNUM2_SHIFT)) & HSSPI_TPM_LOCNUM2_MASK)
#define HSSPI_TPM_LOCNUM3_MASK  0x800u
#define HSSPI_TPM_LOCNUM3_SHIFT 11u
#define HSSPI_TPM_LOCNUM3_WIDTH 1u
#define HSSPI_TPM_LOCNUM3(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_LOCNUM3_SHIFT)) & HSSPI_TPM_LOCNUM3_MASK)
#define HSSPI_TPM_LOCNUM4_MASK  0x1000u
#define HSSPI_TPM_LOCNUM4_SHIFT 12u
#define HSSPI_TPM_LOCNUM4_WIDTH 1u
#define HSSPI_TPM_LOCNUM4(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_LOCNUM4_SHIFT)) & HSSPI_TPM_LOCNUM4_MASK)
#define HSSPI_TPM_BYPASSLOCCHK_MASK  0x10000u
#define HSSPI_TPM_BYPASSLOCCHK_SHIFT 16u
#define HSSPI_TPM_BYPASSLOCCHK_WIDTH 1u
#define HSSPI_TPM_BYPASSLOCCHK(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_BYPASSLOCCHK_SHIFT)) & HSSPI_TPM_BYPASSLOCCHK_MASK)
#define HSSPI_TPM_BYPASSLOCNUM_MASK  0x20000u
#define HSSPI_TPM_BYPASSLOCNUM_SHIFT 17u
#define HSSPI_TPM_BYPASSLOCNUM_WIDTH 1u
#define HSSPI_TPM_BYPASSLOCNUM(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_BYPASSLOCNUM_SHIFT)) & HSSPI_TPM_BYPASSLOCNUM_MASK)
/* TPM Read FIFO count Bit Fields */
#define HSSPI_TPM_TXLEN_MASK  0x7Fu
#define HSSPI_TPM_TXLEN_SHIFT 0u
#define HSSPI_TPM_TXLEN_WIDTH 7u
#define HSSPI_TPM_TXLEN(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_TXLEN_SHIFT)) & HSSPI_TPM_TXLEN_MASK)
#define HSSPI_TPM_RDFIFOCLR_MASK  0x100u
#define HSSPI_TPM_RDFIFOCLR_SHIFT 8u
#define HSSPI_TPM_RDFIFOCLR_WIDTH 1u
#define HSSPI_TPM_RDFIFOCLR(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_RDFIFOCLR_SHIFT)) & HSSPI_TPM_RDFIFOCLR_MASK)
#define HSSPI_TPM_RDFIFOCNT_MASK  0x7F0000u
#define HSSPI_TPM_RDFIFOCNT_SHIFT 16u
#define HSSPI_TPM_RDFIFOCNT_WIDTH 7u
#define HSSPI_TPM_RDFIFOCNT(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_RDFIFOCNT_SHIFT)) & HSSPI_TPM_RDFIFOCNT_MASK)
/* TPM Last command info Bit Fields */
#define HSSPI_TPM_LASTREAD_MASK  0x100u
#define HSSPI_TPM_LASTREAD_SHIFT 8u
#define HSSPI_TPM_LASTREAD_WIDTH 1u
#define HSSPI_TPM_LASTREAD(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_LASTREAD_SHIFT)) & HSSPI_TPM_LASTREAD_MASK)
#define HSSPI_TPM_LASTLEN_MASK  0x3Fu
#define HSSPI_TPM_LASTLEN_SHIFT 0u
#define HSSPI_TPM_LASTLEN_WIDTH 6u
#define HSSPI_TPM_LASTLEN(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_LASTLEN_SHIFT)) & HSSPI_TPM_LASTLEN_MASK)
/* TPM write FIFO receive byte count Bit Fields */
#define HSSPI_TPM_WRFIFOCNT_MASK  0xFFu
#define HSSPI_TPM_WRFIFOCNT_SHIFT 0u
#define HSSPI_TPM_WRFIFOCNT_WIDTH 8u
#define HSSPI_TPM_WRFIFOCNT(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_WRFIFOCNT_SHIFT)) & HSSPI_TPM_WRFIFOCNT_MASK)
/* TPM last transfer adress record Bit Fields */
#define HSSPI_TPM_LASTADDR_MASK  0xFFFFFFu
#define HSSPI_TPM_LASTADDR_SHIFT 0u
#define HSSPI_TPM_LASTADDR_WIDTH 24u
#define HSSPI_TPM_LASTADDR(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_LASTADDR_SHIFT)) & HSSPI_TPM_LASTADDR_MASK)
/* TPM_Access Bit Fields*/
#define HSSPI_TPM_ESTB_MASK  0x1u
#define HSSPI_TPM_ESTB_SHIFT 0u
#define HSSPI_TPM_ESTB_WIDTH 1u
#define HSSPI_TPM_ESTB(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_ESTB_SHIFT)) & HSSPI_TPM_ESTB_MASK)
#define HSSPI_TPM_REQ_MASK      0x2u
#define HSSPI_TPM_REQ_SHIFT     1u
#define HSSPI_TPM_REQ_WIDTH     1u
#define HSSPI_TPM_REQ(x)        (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_REQ_SHIFT)) & HSSPI_TPM_REQ_MASK)
#define HSSPI_TPM_PENDREQ_MASK  0x4u
#define HSSPI_TPM_PENDREQ_SHIFT 2u
#define HSSPI_TPM_PENDREQ_WIDTH 1u
#define HSSPI_TPM_PENDREQ(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_PENDREQ_SHIFT)) & HSSPI_TPM_PENDREQ_MASK)
#define HSSPI_TPM_SEIZE_MASK  0x8u
#define HSSPI_TPM_SEIZE_SHIFT 3u
#define HSSPI_TPM_SEIZE_WIDTH 1u
#define HSSPI_TPM_SEIZE(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_SEIZE_SHIFT)) & HSSPI_TPM_SEIZE_MASK)
#define HSSPI_TPM_BSEIZE_MASK  0x10u
#define HSSPI_TPM_BSEIZE_SHIFT 4u
#define HSSPI_TPM_BSEIZE_WIDTH 1u
#define HSSPI_TPM_BSEIZE(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_BSEIZE_SHIFT)) & HSSPI_TPM_BSEIZE_MASK)
#define HSSPI_TPM_ACTLOC_MASK  0x20u
#define HSSPI_TPM_ACTLOC_SHIFT 5u
#define HSSPI_TPM_ACTLOC_WIDTH 1u
#define HSSPI_TPM_ACTLOC(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_ACTLOC_SHIFT)) & HSSPI_TPM_ACTLOC_MASK)
#define HSSPI_TPM_REGVALID_MASK  0x80u
#define HSSPI_TPM_REGVALID_SHIFT 7u
#define HSSPI_TPM_REGVALID_WIDTH 1u
#define HSSPI_TPM_REGVALID(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_REGVALID_SHIFT)) & HSSPI_TPM_REGVALID_MASK)
/* TPM_INT_ENABLE Bit Fields*/
#define HSSPI_TPM_AVAINTEN_MASK  0x1u
#define HSSPI_TPM_AVAINTEN_SHIFT 0u
#define HSSPI_TPM_AVAINTEN_WIDTH 1u
#define HSSPI_TPM_AVAINTEN(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_AVAINTEN_SHIFT)) & HSSPI_TPM_AVAINTEN_MASK)
#define HSSPI_TPM_STSINTEN_MASK  0x2u
#define HSSPI_TPM_STSINTEN_SHIFT 1u
#define HSSPI_TPM_STSINTEN_WIDTH 1u
#define HSSPI_TPM_STSINTEN(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_STSINTEN_SHIFT)) & HSSPI_TPM_STSINTEN_MASK)
#define HSSPI_TPM_LOCCHGINTEN_MASK  0x4u
#define HSSPI_TPM_LOCCHGINTEN_SHIFT 2u
#define HSSPI_TPM_LOCCHGINTEN_WIDTH 1u
#define HSSPI_TPM_LOCCHGINTEN(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_LOCCHGINTEN_SHIFT)) & HSSPI_TPM_LOCCHGINTEN_MASK)
#define HSSPI_TPM_TYPPOL_MASK  0x18u
#define HSSPI_TPM_TYPPOL_SHIFT 3u
#define HSSPI_TPM_TYPPOL_WIDTH 2u
#define HSSPI_TPM_TYPPOL(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_TYPPOL_SHIFT)) & HSSPI_TPM_TYPPOL_MASK)
#define HSSPI_TPM_CMDRDYEN_MASK  0x80u
#define HSSPI_TPM_CMDRDYEN_SHIFT 7u
#define HSSPI_TPM_CMDRDYEN_WIDTH 1u
#define HSSPI_TPM_CMDRDYEN(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_CMDRDYEN_SHIFT)) & HSSPI_TPM_CMDRDYEN_MASK)
#define HSSPI_TPM_GLBINTEN_MASK  0x80000000u
#define HSSPI_TPM_GLBINTEN_SHIFT 31u
#define HSSPI_TPM_GLBINTEN_WIDTH 1u
#define HSSPI_TPM_GLBINTEN(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_GLBINTEN_SHIFT)) & HSSPI_TPM_GLBINTEN_MASK)
/* TPM_INT_VECTOR Bit Fields */
#define HSSPI_TPM_SIRQVEC_MASK  0xFu
#define HSSPI_TPM_SIRQVEC_SHIFT 0u
#define HSSPI_TPM_SIRQVEC_WIDTH 4u
#define HSSPI_TPM_SIRQVEC(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_SIRQVEC_SHIFT)) & HSSPI_TPM_SIRQVEC_MASK)
/* TPM_INT_STATUS Bit Fields */
#define HSSPI_TPM_AVAINTOC_MASK  0x1u
#define HSSPI_TPM_AVAINTOC_SHIFT 0u
#define HSSPI_TPM_AVAINTOC_WIDTH 1u
#define HSSPI_TPM_AVAINTOC(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_AVAINTOC_SHIFT)) & HSSPI_TPM_AVAINTOC_MASK)
#define HSSPI_TPM_STSINTOC_MASK  0x2u
#define HSSPI_TPM_STSINTOC_SHIFT 1u
#define HSSPI_TPM_STSINTOC_WIDTH 1u
#define HSSPI_TPM_STSINTOC(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_STSINTOC_SHIFT)) & HSSPI_TPM_STSINTOC_MASK)
#define HSSPI_TPM_LOCCHGINTOC_MASK  0x4u
#define HSSPI_TPM_LOCCHGINTOC_SHIFT 2u
#define HSSPI_TPM_LOCCHGINTOC_WIDTH 1u
#define HSSPI_TPM_LOCCHGINTOC(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_LOCCHGINTOC_SHIFT)) & HSSPI_TPM_LOCCHGINTOC_MASK)
#define HSSPI_TPM_CMDRDYOC_MASK  0x80u
#define HSSPI_TPM_CMDRDYOC_SHIFT 7u
#define HSSPI_TPM_CMDRDYOC_WIDTH 1u
#define HSSPI_TPM_CMDRDYOC(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_CMDRDYOC_SHIFT)) & HSSPI_TPM_CMDRDYOC_MASK)
/* TPM_INTF_CAPABILITY Bit Fields */
#define HSSPI_TPM_AVAINTSP_MASK  0x1u
#define HSSPI_TPM_AVAINTSP_SHIFT 0u
#define HSSPI_TPM_AVAINTSP_WIDTH 1u
#define HSSPI_TPM_AVAINTSP(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_AVAINTSP_SHIFT)) & HSSPI_TPM_AVAINTSP_MASK)
#define HSSPI_TPM_STSINTSP_MASK  0x2u
#define HSSPI_TPM_STSINTSP_SHIFT 1u
#define HSSPI_TPM_STSINTSP_WIDTH 1u
#define HSSPI_TPM_STSINTSP(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_STSINTSP_SHIFT)) & HSSPI_TPM_STSINTSP_MASK)
#define HSSPI_TPM_LOCCHGINTSP_MASK  0x4u
#define HSSPI_TPM_LOCCHGINTSP_SHIFT 2u
#define HSSPI_TPM_LOCCHGINTSP_WIDTH 1u
#define HSSPI_TPM_LOCCHGINTSP(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_LOCCHGINTSP_SHIFT)) & HSSPI_TPM_LOCCHGINTSP_MASK)
#define HSSPI_TPM_HIGHLVINT_MASK  0x8u
#define HSSPI_TPM_HIGHLVINT_SHIFT 3u
#define HSSPI_TPM_HIGHLVINT_WIDTH 1u
#define HSSPI_TPM_HIGHLVINT(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_HIGHLVINT_SHIFT)) & HSSPI_TPM_HIGHLVINT_MASK)
#define HSSPI_TPM_LOWLVINT_MASK  0x10u
#define HSSPI_TPM_LOWLVINT_SHIFT 4u
#define HSSPI_TPM_LOWLVINT_WIDTH 1u
#define HSSPI_TPM_LOWLVINT(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_LOWLVINT_SHIFT)) & HSSPI_TPM_LOWLVINT_MASK)
#define HSSPI_TPM_EDGRISEINT_MASK  0x20u
#define HSSPI_TPM_EDGRISEINT_SHIFT 5u
#define HSSPI_TPM_EDGRISEINT_WIDTH 1u
#define HSSPI_TPM_EDGRISEINT(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_EDGRISEINT_SHIFT)) & HSSPI_TPM_EDGRISEINT_MASK)
#define HSSPI_TPM_EDGFALLINT_MASK  0x40u
#define HSSPI_TPM_EDGFALLINT_SHIFT 6u
#define HSSPI_TPM_EDGFALLINT_WIDTH 1u
#define HSSPI_TPM_EDGFALLINT(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_EDGRISEINT_SHIFT)) & HSSPI_TPM_EDGRISEINT_MASK)
#define HSSPI_TPM_CMDRDYSP_MASK  0x80u
#define HSSPI_TPM_CMDRDYSP_SHIFT 7u
#define HSSPI_TPM_CMDRDYSP_WIDTH 1u
#define HSSPI_TPM_CMDRDYSP(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_CMDRDYSP_SHIFT)) & HSSPI_TPM_CMDRDYSP_MASK)
#define HSSPI_TPM_BSTCNTSP_MASK  0x100u
#define HSSPI_TPM_BSTCNTSP_SHIFT 8u
#define HSSPI_TPM_BSTCNTSP_WIDTH 1u
#define HSSPI_TPM_BSTCNTSP(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_BSTCNTSP_SHIFT)) & HSSPI_TPM_BSTCNTSP_MASK)
#define HSSPI_TPM_TRFSZSP_MASK  0x600u
#define HSSPI_TPM_TRFSZSP_SHIFT 9u
#define HSSPI_TPM_TRFSZSP_WIDTH 2u
#define HSSPI_TPM_TRFSZSP(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_TRFSZSP_SHIFT)) & HSSPI_TPM_TRFSZSP_MASK)
#define HSSPI_TPM_IFVERSP_MASK  0x70000000u
#define HSSPI_TPM_IFVERSP_SHIFT 28u
#define HSSPI_TPM_IFVERSP_WIDTH 3u
#define HSSPI_TPM_IFVERSP(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_IFVERSP_SHIFT)) & HSSPI_TPM_IFVERSP_MASK)
/* TPM_STS Bit Fields */
#define HSSPI_TPM_RESEND_MASK  0x2u
#define HSSPI_TPM_RESEND_SHIFT 1u
#define HSSPI_TPM_RESEND_WIDTH 1u
#define HSSPI_TPM_RESEND(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_RESEND_SHIFT)) & HSSPI_TPM_RESEND_MASK)
#define HSSPI_TPM_SELFTEST_MASK  0x4u
#define HSSPI_TPM_SELFTEST_SHIFT 2u
#define HSSPI_TPM_SELFTEST_WIDTH 1u
#define HSSPI_TPM_SELFTEST(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_SELFTEST_SHIFT)) & HSSPI_TPM_SELFTEST_MASK)
#define HSSPI_TPM_EXPECT_MASK  0x8u
#define HSSPI_TPM_EXPECT_SHIFT 3u
#define HSSPI_TPM_EXPECT_WIDTH 1u
#define HSSPI_TPM_EXPECT(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_EXPECT_SHIFT)) & HSSPI_TPM_EXPECT_MASK)
#define HSSPI_TPM_AVA_MASK    0x10u
#define HSSPI_TPM_AVA_SHIFT   4u
#define HSSPI_TPM_AVA_WIDTH   1u
#define HSSPI_TPM_AVA(x)      (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_AVA_SHIFT)) & HSSPI_TPM_AVA_MASK)
#define HSSPI_TPM_TPMGO_MASK  0x20u
#define HSSPI_TPM_TPMGO_SHIFT 5u
#define HSSPI_TPM_TPMGO_WIDTH 1u
#define HSSPI_TPM_TPMGO(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_TPMGO_SHIFT)) & HSSPI_TPM_TPMGO_MASK)
#define HSSPI_TPM_CMDRDY_MASK  0x40u
#define HSSPI_TPM_CMDRDY_SHIFT 6u
#define HSSPI_TPM_CMDRDY_WIDTH 1u
#define HSSPI_TPM_CMDRDY(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_CMDRDY_SHIFT)) & HSSPI_TPM_CMDRDY_MASK)
#define HSSPI_TPM_STSVLD_MASK  0x80u
#define HSSPI_TPM_STSVLD_SHIFT 7u
#define HSSPI_TPM_STSVLD_WIDTH 1u
#define HSSPI_TPM_STSVLD(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_STSVLD_SHIFT)) & HSSPI_TPM_STSVLD_MASK)
#define HSSPI_TPM_BSTCNT_MASK  0xFFFF00u
#define HSSPI_TPM_BSTCNT_SHIFT 8u
#define HSSPI_TPM_BSTCNT_WIDTH 16u
#define HSSPI_TPM_BSTCNT(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_BSTCNT_SHIFT)) & HSSPI_TPM_BSTCNT_MASK)
#define HSSPI_TPM_CMDCAN_MASK  0x1000000u
#define HSSPI_TPM_CMDCAN_SHIFT 24u
#define HSSPI_TPM_CMDCAN_WIDTH 1u
#define HSSPI_TPM_CMDCAN(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_CMDCAN_SHIFT)) & HSSPI_TPM_CMDCAN_MASK)
#define HSSPI_TPM_RSTESTB_MASK  0x2000000u
#define HSSPI_TPM_RSTESTB_SHIFT 25u
#define HSSPI_TPM_RSTESTB_WIDTH 1u
#define HSSPI_TPM_RSTESTB(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_RSTESTB_SHIFT)) & HSSPI_TPM_RSTESTB_MASK)
#define HSSPI_TPM_FAMILY_MASK  0xC000000u
#define HSSPI_TPM_FAMILY_SHIFT 26u
#define HSSPI_TPM_FAMILY_WIDTH 2u
#define HSSPI_TPM_FAMILY(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_FAMILY_SHIFT)) & HSSPI_TPM_FAMILY_MASK)
/* TPM_DID_VID Bit Fields */
#define HSSPI_TPM_VID_MASK  0xFFFFu
#define HSSPI_TPM_VID_SHIFT 0u
#define HSSPI_TPM_VID_WIDTH 16u
#define HSSPI_TPM_VID(x)    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_VID_SHIFT)) & HSSPI_TPM_VID_MASK)
#define HSSPI_TPM_DID_MASK  0xFFFF0000u
#define HSSPI_TPM_DID_SHIFT 16u
#define HSSPI_TPM_DID_WIDTH 16u
#define HSSPI_TPM_DID(x)    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_DID_SHIFT)) & HSSPI_TPM_DID_MASK)
/* TPM_RID Bit Fields */
#define HSSPI_TPM_RID_MASK  0xFFu
#define HSSPI_TPM_RID_SHIFT 0u
#define HSSPI_TPM_RID_WIDTH 8u
#define HSSPI_TPM_RID(x)    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_RID_SHIFT)) & HSSPI_TPM_RID_MASK)
/* TPM_HASH_END Bit Fields */
#define HSSPI_TPM_HASHEND_MASK  0xFFu
#define HSSPI_TPM_HASHEND_SHIFT 0u
#define HSSPI_TPM_HASHEND_WIDTH 8u
#define HSSPI_TPM_HASHEND(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_HASHEND_SHIFT)) & HSSPI_TPM_HASHEND_MASK)
/* TPM_HASH_START Bit Fields */
#define HSSPI_TPM_HASHSTRT_MASK  0xFFu
#define HSSPI_TPM_HASHSTRT_SHIFT 0u
#define HSSPI_TPM_HASHSTRT_WIDTH 8u
#define HSSPI_TPM_HASHSTRT(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_HASHSTRT_SHIFT)) & HSSPI_TPM_HASHSTRT_MASK)
/* TPM_HASH_START Bit Fields */
#define HSSPI_TPM_HASHSTRT_MASK  0xFFu
#define HSSPI_TPM_HASHSTRT_SHIFT 0u
#define HSSPI_TPM_HASHSTRT_WIDTH 8u
#define HSSPI_TPM_HASHSTRT(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_HASHSTRT_SHIFT)) & HSSPI_TPM_HASHSTRT_MASK)
/* TPM_INTERFACE_ID Bit Fields */
#define HSSPI_TPM_IFTYP_MASK  0xFu
#define HSSPI_TPM_IFTYP_SHIFT 0u
#define HSSPI_TPM_IFTYP_WIDTH 4u
#define HSSPI_TPM_IFTYP(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_IFTYP_SHIFT)) & HSSPI_TPM_IFTYP_MASK)
#define HSSPI_TPM_IFVER_MASK  0xF0u
#define HSSPI_TPM_IFVER_SHIFT 4u
#define HSSPI_TPM_IFVER_WIDTH 4u
#define HSSPI_TPM_IFVER(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_IFVER_SHIFT)) & HSSPI_TPM_IFVER_MASK)
#define HSSPI_TPM_CAPLOC_MASK  0x100u
#define HSSPI_TPM_CAPLOC_SHIFT 8u
#define HSSPI_TPM_CAPLOC_WIDTH 1u
#define HSSPI_TPM_CAPLOC(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_CAPLOC_SHIFT)) & HSSPI_TPM_CAPLOC_MASK)
#define HSSPI_TPM_CAPTIS_MASK  0x2000u
#define HSSPI_TPM_CAPTIS_SHIFT 13u
#define HSSPI_TPM_CAPTIS_WIDTH 1u
#define HSSPI_TPM_CAPTIS(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_CAPTIS_SHIFT)) & HSSPI_TPM_CAPTIS_MASK)
#define HSSPI_TPM_CAPCRB_MASK  0x4000u
#define HSSPI_TPM_CAPCRB_SHIFT 14u
#define HSSPI_TPM_CAPCRB_WIDTH 1u
#define HSSPI_TPM_CAPCRB(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_CAPCRB_SHIFT)) & HSSPI_TPM_CAPCRB_MASK)
#define HSSPI_TPM_CAPIFR_MASK  0x18000u
#define HSSPI_TPM_CAPIFR_SHIFT 15u
#define HSSPI_TPM_CAPIFR_WIDTH 2u
#define HSSPI_TPM_CAPIFR(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_CAPIFR_SHIFT)) & HSSPI_TPM_CAPIFR_MASK)
#define HSSPI_TPM_IFSEL_MASK  0x60000u
#define HSSPI_TPM_IFSEL_SHIFT 17u
#define HSSPI_TPM_IFSEL_WIDTH 2u
#define HSSPI_TPM_IFSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_IFSEL_SHIFT)) & HSSPI_TPM_IFSEL_MASK)
#define HSSPI_TPM_IFSELLK_MASK  0x80000u
#define HSSPI_TPM_IFSELLK_SHIFT 19u
#define HSSPI_TPM_IFSELLK_WIDTH 1u
#define HSSPI_TPM_IFSELLK(x) \
    (((uint32_t)(((uint32_t)(x)) << HSSPI_TPM_IFSELLK_SHIFT)) & HSSPI_TPM_IFSELLK_MASK)

/**
 * @}
 */
/* end of group HSSPI_Register_Masks */

/**
 * @}
 */
/* end of group HSSPI_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------
   -- LPTMR Peripheral Access Layer
   ---------------------------------------------------------------------- */

/**
 * @addtogroup LPTMR_Peripheral_Access_Layer LPTMR Peripheral Access Layer
 * @{
 */

/** LPTMR - Size of Registers Arrays */

/** LPTMR - Register Layout Typedef */
typedef struct
{
    _IO uint32_t CSR; /**< offset: 0x0 */
    _IO uint32_t PSR; /**< offset: 0x4 */
    _IO uint32_t CMR; /**< offset: 0x8 */
    _IO uint32_t CNR; /**< offset: 0xC */
} LPTMR_t, *LPTMR_MemMapPtr;

/** Number of instances of the LPTMR module. */
#define LPTMR_INSTANCE_COUNT (1u)

/* LPTMR - Peripheral instance base addresses */
/** Peripheral LPTMR0 base address */
#define LPTMR0_BASE (0x40040000u)
/** Peripheral LPTMR0 base pointer */
#define LPTMR0 ((LPTMR_t *)LPTMR0_BASE)
/** Array initializer of LPTMR peripheral base addresses */
#define LPTMR_BASE_ADDRS \
    {                    \
        LPTMR0_BASE      \
    }
/** Array initializer of LPTMR peripheral base pointers */
#define LPTMR_BASE_PTRS \
    {                   \
        LPTMR0          \
    }
/** Number of interrupt vector arrays for the LPTMR module. */
#define LPTMR_IRQS_ARR_COUNT (1u)
/** Number of interrupt channels for the LPTMR module. */
#define LPTMR_IRQS_CH_COUNT (1u)
/** Interrupt vectors for the LPTMR peripheral type */
#define LPTMR_IRQS  \
    {               \
        LPTMR0_IRQn \
    }

/* --------------------------------------------------------------------------
   -- LPTMR Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup LPTMR_Register_Masks LPTMR Register Masks
 * @{
 */

/* CSR Bit Fields */
#define LPTMR_CSR_TEN_MASK   0x1u
#define LPTMR_CSR_TEN_SHIFT  0u
#define LPTMR_CSR_TEN_WIDTH  1u
#define LPTMR_CSR_TEN(x)     (((uint32_t)(((uint32_t)(x)) << LPTMR_CSR_TEN_SHIFT)) & LPTMR_CSR_TEN_MASK)
#define LPTMR_CSR_TMS_MASK   0x2u
#define LPTMR_CSR_TMS_SHIFT  1u
#define LPTMR_CSR_TMS_WIDTH  1u
#define LPTMR_CSR_TMS(x)     (((uint32_t)(((uint32_t)(x)) << LPTMR_CSR_TMS_SHIFT)) & LPTMR_CSR_TMS_MASK)
#define LPTMR_CSR_TFC_MASK   0x4u
#define LPTMR_CSR_TFC_SHIFT  2u
#define LPTMR_CSR_TFC_WIDTH  1u
#define LPTMR_CSR_TFC(x)     (((uint32_t)(((uint32_t)(x)) << LPTMR_CSR_TFC_SHIFT)) & LPTMR_CSR_TFC_MASK)
#define LPTMR_CSR_TPP_MASK   0x8u
#define LPTMR_CSR_TPP_SHIFT  3u
#define LPTMR_CSR_TPP_WIDTH  1u
#define LPTMR_CSR_TPP(x)     (((uint32_t)(((uint32_t)(x)) << LPTMR_CSR_TPP_SHIFT)) & LPTMR_CSR_TPP_MASK)
#define LPTMR_CSR_TPS_MASK   0x30u
#define LPTMR_CSR_TPS_SHIFT  4u
#define LPTMR_CSR_TPS_WIDTH  2u
#define LPTMR_CSR_TPS(x)     (((uint32_t)(((uint32_t)(x)) << LPTMR_CSR_TPS_SHIFT)) & LPTMR_CSR_TPS_MASK)
#define LPTMR_CSR_TIE_MASK   0x40u
#define LPTMR_CSR_TIE_SHIFT  6u
#define LPTMR_CSR_TIE_WIDTH  1u
#define LPTMR_CSR_TIE(x)     (((uint32_t)(((uint32_t)(x)) << LPTMR_CSR_TIE_SHIFT)) & LPTMR_CSR_TIE_MASK)
#define LPTMR_CSR_TCF_MASK   0x80u
#define LPTMR_CSR_TCF_SHIFT  7u
#define LPTMR_CSR_TCF_WIDTH  1u
#define LPTMR_CSR_TCF(x)     (((uint32_t)(((uint32_t)(x)) << LPTMR_CSR_TCF_SHIFT)) & LPTMR_CSR_TCF_MASK)
#define LPTMR_CSR_TDRE_MASK  0x100u
#define LPTMR_CSR_TDRE_SHIFT 8u
#define LPTMR_CSR_TDRE_WIDTH 1u
#define LPTMR_CSR_TDRE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPTMR_CSR_TDRE_SHIFT)) & LPTMR_CSR_TDRE_MASK)
/* PSR Bit Fields */
#define LPTMR_PSR_PCS_MASK   0x3u
#define LPTMR_PSR_PCS_SHIFT  0u
#define LPTMR_PSR_PCS_WIDTH  2u
#define LPTMR_PSR_PCS(x)     (((uint32_t)(((uint32_t)(x)) << LPTMR_PSR_PCS_SHIFT)) & LPTMR_PSR_PCS_MASK)
#define LPTMR_PSR_PBYP_MASK  0x4u
#define LPTMR_PSR_PBYP_SHIFT 2u
#define LPTMR_PSR_PBYP_WIDTH 1u
#define LPTMR_PSR_PBYP(x) \
    (((uint32_t)(((uint32_t)(x)) << LPTMR_PSR_PBYP_SHIFT)) & LPTMR_PSR_PBYP_MASK)
#define LPTMR_PSR_PRESCALE_MASK  0x78u
#define LPTMR_PSR_PRESCALE_SHIFT 3u
#define LPTMR_PSR_PRESCALE_WIDTH 4u
#define LPTMR_PSR_PRESCALE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPTMR_PSR_PRESCALE_SHIFT)) & LPTMR_PSR_PRESCALE_MASK)
/* CMR Bit Fields */
#define LPTMR_CMR_COMPARE_MASK  0xFFFFu
#define LPTMR_CMR_COMPARE_SHIFT 0u
#define LPTMR_CMR_COMPARE_WIDTH 16u
#define LPTMR_CMR_COMPARE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPTMR_CMR_COMPARE_SHIFT)) & LPTMR_CMR_COMPARE_MASK)
/* CNR Bit Fields */
#define LPTMR_CNR_COUNTER_MASK  0xFFFFu
#define LPTMR_CNR_COUNTER_SHIFT 0u
#define LPTMR_CNR_COUNTER_WIDTH 16u
#define LPTMR_CNR_COUNTER(x) \
    (((uint32_t)(((uint32_t)(x)) << LPTMR_CNR_COUNTER_SHIFT)) & LPTMR_CNR_COUNTER_MASK)

/**
 * @}
 */ /* end of group LPTMR_Register_Masks */

/**
 * @}
 */ /* end of group LPTMR_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------
   -- LPUART Peripheral Access Layer
   ---------------------------------------------------------------------- */

/**
 * @addtogroup LPUART_Peripheral_Access_Layer LPUART Peripheral Access Layer
 * @{
 */

/** LPUART - Size of Registers Arrays */

/** LPUART - Register Layout Typedef */
typedef struct
{
    __I uint32_t VERID;  /**< offset: 0x0 */
    __I uint32_t PARAM;  /**< offset: 0x4 */
    _NA uint32_t RESERVED_0[1];
    _IO uint32_t PINCFG; /**< offset: 0xC */
    _IO uint32_t BAUD;   /**< offset: 0x10 */
    _IO uint32_t STAT;   /**< offset: 0x14 */
    _IO uint32_t CTRL;   /**< offset: 0x18 */
    _IO uint32_t DATA;   /**< offset: 0x1C */
    _IO uint32_t MATCH;  /**< offset: 0x20 */
    _NA uint32_t RESERVED_1[1];
    _IO uint32_t FIFO;   /**< offset: 0x28 */
    _IO uint32_t WATER;  /**< offset: 0x2C */
} LPUART_t, *LPUART_MemMapPtr;

/** Number of instances of the LPUART module. */
#define LPUART_INSTANCE_COUNT (2u)

/* LPUART - Peripheral instance base addresses */
/** Peripheral LPUART0 base address */
#define LPUART0_BASE (0x4006A000u)
/** Peripheral LPUART0 base pointer */
#define LPUART0 ((LPUART_t *)LPUART0_BASE)
/** Peripheral LPUART1 base address */
#define LPUART1_BASE (0x4006B000u)
/** Peripheral LPUART1 base pointer */
#define LPUART1 ((LPUART_t *)LPUART1_BASE)
/** Array initializer of LPUART peripheral base addresses */
#define LPUART_BASE_ADDRS          \
    {                              \
        LPUART0_BASE, LPUART1_BASE \
    }
/** Array initializer of LPUART peripheral base pointers */
#define LPUART_BASE_PTRS \
    {                    \
        LPUART0, LPUART1 \
    }
/** Number of interrupt vector arrays for the LPUART module. */
#define LPUART_IRQS_ARR_COUNT (1u)
/** Number of interrupt channels for the RX_TX type of LPUART module. */
#define LPUART_RX_TX_IRQS_CH_COUNT (1u)
/** Interrupt vectors for the LPUART peripheral type */
#define LPUART_RX_TX_IRQS                    \
    {                                        \
        LPUART0_RxTx_IRQn, LPUART1_RxTx_IRQn \
    }

/* --------------------------------------------------------------------------
   -- LPUART Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup LPUART_Register_Masks LPUART Register Masks
 * @{
 */

/* VERID Bit Fields */
#define LPUART_VERID_FEATURE_MASK  0xFFFFu
#define LPUART_VERID_FEATURE_SHIFT 0u
#define LPUART_VERID_FEATURE_WIDTH 16u
#define LPUART_VERID_FEATURE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_VERID_FEATURE_SHIFT)) & LPUART_VERID_FEATURE_MASK)
#define LPUART_VERID_MINOR_MASK  0xFF0000u
#define LPUART_VERID_MINOR_SHIFT 16u
#define LPUART_VERID_MINOR_WIDTH 8u
#define LPUART_VERID_MINOR(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_VERID_MINOR_SHIFT)) & LPUART_VERID_MINOR_MASK)
#define LPUART_VERID_MAJOR_MASK  0xFF000000u
#define LPUART_VERID_MAJOR_SHIFT 24u
#define LPUART_VERID_MAJOR_WIDTH 8u
#define LPUART_VERID_MAJOR(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_VERID_MAJOR_SHIFT)) & LPUART_VERID_MAJOR_MASK)
/* PARAM Bit Fields */
#define LPUART_PARAM_TXFIFO_MASK  0xFFu
#define LPUART_PARAM_TXFIFO_SHIFT 0u
#define LPUART_PARAM_TXFIFO_WIDTH 8u
#define LPUART_PARAM_TXFIFO(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_PARAM_TXFIFO_SHIFT)) & LPUART_PARAM_TXFIFO_MASK)
#define LPUART_PARAM_RXFIFO_MASK  0xFF00u
#define LPUART_PARAM_RXFIFO_SHIFT 8u
#define LPUART_PARAM_RXFIFO_WIDTH 8u
#define LPUART_PARAM_RXFIFO(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_PARAM_RXFIFO_SHIFT)) & LPUART_PARAM_RXFIFO_MASK)
/* PINCFG Bit Fields */
#define LPUART_PINCFG_TRGSEL_MASK  0x3u
#define LPUART_PINCFG_TRGSEL_SHIFT 0u
#define LPUART_PINCFG_TRGSEL_WIDTH 2u
#define LPUART_PINCFG_TRGSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_PINCFG_TRGSEL_SHIFT)) & LPUART_PINCFG_TRGSEL_MASK)
/* BAUD Bit Fields */
#define LPUART_BAUD_SBR_MASK  0x1FFFu
#define LPUART_BAUD_SBR_SHIFT 0u
#define LPUART_BAUD_SBR_WIDTH 13u
#define LPUART_BAUD_SBR(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_SBR_SHIFT)) & LPUART_BAUD_SBR_MASK)
#define LPUART_BAUD_SBNS_MASK  0x2000u
#define LPUART_BAUD_SBNS_SHIFT 13u
#define LPUART_BAUD_SBNS_WIDTH 1u
#define LPUART_BAUD_SBNS(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_SBNS_SHIFT)) & LPUART_BAUD_SBNS_MASK)
#define LPUART_BAUD_BOTHEDGE_MASK  0x20000u
#define LPUART_BAUD_BOTHEDGE_SHIFT 17u
#define LPUART_BAUD_BOTHEDGE_WIDTH 1u
#define LPUART_BAUD_BOTHEDGE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_BOTHEDGE_SHIFT)) & LPUART_BAUD_BOTHEDGE_MASK)
#define LPUART_BAUD_MATCFG_MASK  0xC0000u
#define LPUART_BAUD_MATCFG_SHIFT 18u
#define LPUART_BAUD_MATCFG_WIDTH 2u
#define LPUART_BAUD_MATCFG(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_MATCFG_SHIFT)) & LPUART_BAUD_MATCFG_MASK)
#define LPUART_BAUD_RDMAE_MASK  0x200000u
#define LPUART_BAUD_RDMAE_SHIFT 21u
#define LPUART_BAUD_RDMAE_WIDTH 1u
#define LPUART_BAUD_RDMAE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_RDMAE_SHIFT)) & LPUART_BAUD_RDMAE_MASK)
#define LPUART_BAUD_TDMAE_MASK  0x800000u
#define LPUART_BAUD_TDMAE_SHIFT 23u
#define LPUART_BAUD_TDMAE_WIDTH 1u
#define LPUART_BAUD_TDMAE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_TDMAE_SHIFT)) & LPUART_BAUD_TDMAE_MASK)
#define LPUART_BAUD_OSR_MASK  0x1F000000u
#define LPUART_BAUD_OSR_SHIFT 24u
#define LPUART_BAUD_OSR_WIDTH 5u
#define LPUART_BAUD_OSR(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_OSR_SHIFT)) & LPUART_BAUD_OSR_MASK)
#define LPUART_BAUD_M10_MASK  0x20000000u
#define LPUART_BAUD_M10_SHIFT 29u
#define LPUART_BAUD_M10_WIDTH 1u
#define LPUART_BAUD_M10(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_M10_SHIFT)) & LPUART_BAUD_M10_MASK)
#define LPUART_BAUD_MAEN2_MASK  0x40000000u
#define LPUART_BAUD_MAEN2_SHIFT 30u
#define LPUART_BAUD_MAEN2_WIDTH 1u
#define LPUART_BAUD_MAEN2(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_MAEN2_SHIFT)) & LPUART_BAUD_MAEN2_MASK)
#define LPUART_BAUD_MAEN1_MASK  0x80000000u
#define LPUART_BAUD_MAEN1_SHIFT 31u
#define LPUART_BAUD_MAEN1_WIDTH 1u
#define LPUART_BAUD_MAEN1(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_MAEN1_SHIFT)) & LPUART_BAUD_MAEN1_MASK)
/* STAT Bit Fields */
#define LPUART_STAT_MA2F_MASK  0x4000u
#define LPUART_STAT_MA2F_SHIFT 14u
#define LPUART_STAT_MA2F_WIDTH 1u
#define LPUART_STAT_MA2F(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_MA2F_SHIFT)) & LPUART_STAT_MA2F_MASK)
#define LPUART_STAT_MA1F_MASK  0x8000u
#define LPUART_STAT_MA1F_SHIFT 15u
#define LPUART_STAT_MA1F_WIDTH 1u
#define LPUART_STAT_MA1F(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_MA1F_SHIFT)) & LPUART_STAT_MA1F_MASK)
#define LPUART_STAT_PF_MASK  0x10000u
#define LPUART_STAT_PF_SHIFT 16u
#define LPUART_STAT_PF_WIDTH 1u
#define LPUART_STAT_PF(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_PF_SHIFT)) & LPUART_STAT_PF_MASK)
#define LPUART_STAT_FE_MASK  0x20000u
#define LPUART_STAT_FE_SHIFT 17u
#define LPUART_STAT_FE_WIDTH 1u
#define LPUART_STAT_FE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_FE_SHIFT)) & LPUART_STAT_FE_MASK)
#define LPUART_STAT_NF_MASK  0x40000u
#define LPUART_STAT_NF_SHIFT 18u
#define LPUART_STAT_NF_WIDTH 1u
#define LPUART_STAT_NF(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_NF_SHIFT)) & LPUART_STAT_NF_MASK)
#define LPUART_STAT_OR_MASK  0x80000u
#define LPUART_STAT_OR_SHIFT 19u
#define LPUART_STAT_OR_WIDTH 1u
#define LPUART_STAT_OR(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_OR_SHIFT)) & LPUART_STAT_OR_MASK)
#define LPUART_STAT_IDLE_MASK  0x100000u
#define LPUART_STAT_IDLE_SHIFT 20u
#define LPUART_STAT_IDLE_WIDTH 1u
#define LPUART_STAT_IDLE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_IDLE_SHIFT)) & LPUART_STAT_IDLE_MASK)
#define LPUART_STAT_RDRF_MASK  0x200000u
#define LPUART_STAT_RDRF_SHIFT 21u
#define LPUART_STAT_RDRF_WIDTH 1u
#define LPUART_STAT_RDRF(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_RDRF_SHIFT)) & LPUART_STAT_RDRF_MASK)
#define LPUART_STAT_TC_MASK  0x400000u
#define LPUART_STAT_TC_SHIFT 22u
#define LPUART_STAT_TC_WIDTH 1u
#define LPUART_STAT_TC(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_TC_SHIFT)) & LPUART_STAT_TC_MASK)
#define LPUART_STAT_TDRE_MASK  0x800000u
#define LPUART_STAT_TDRE_SHIFT 23u
#define LPUART_STAT_TDRE_WIDTH 1u
#define LPUART_STAT_TDRE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_TDRE_SHIFT)) & LPUART_STAT_TDRE_MASK)
#define LPUART_STAT_RXINV_MASK  0x10000000u
#define LPUART_STAT_RXINV_SHIFT 28u
#define LPUART_STAT_RXINV_WIDTH 1u
#define LPUART_STAT_RXINV(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_RXINV_SHIFT)) & LPUART_STAT_RXINV_MASK)
/* CTRL Bit Fields */
#define LPUART_CTRL_PT_MASK  0x1u
#define LPUART_CTRL_PT_SHIFT 0u
#define LPUART_CTRL_PT_WIDTH 1u
#define LPUART_CTRL_PT(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_PT_SHIFT)) & LPUART_CTRL_PT_MASK)
#define LPUART_CTRL_PE_MASK  0x2u
#define LPUART_CTRL_PE_SHIFT 1u
#define LPUART_CTRL_PE_WIDTH 1u
#define LPUART_CTRL_PE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_PE_SHIFT)) & LPUART_CTRL_PE_MASK)
#define LPUART_CTRL_ILT_MASK  0x4u
#define LPUART_CTRL_ILT_SHIFT 2u
#define LPUART_CTRL_ILT_WIDTH 1u
#define LPUART_CTRL_ILT(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_ILT_SHIFT)) & LPUART_CTRL_ILT_MASK)
#define LPUART_CTRL_M_MASK        0x10u
#define LPUART_CTRL_M_SHIFT       4u
#define LPUART_CTRL_M_WIDTH       1u
#define LPUART_CTRL_M(x)          (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_M_SHIFT)) & LPUART_CTRL_M_MASK)
#define LPUART_CTRL_IDLECFG_MASK  0x700u
#define LPUART_CTRL_IDLECFG_SHIFT 8u
#define LPUART_CTRL_IDLECFG_WIDTH 3u
#define LPUART_CTRL_IDLECFG(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_IDLECFG_SHIFT)) & LPUART_CTRL_IDLECFG_MASK)
#define LPUART_CTRL_M7_MASK  0x800u
#define LPUART_CTRL_M7_SHIFT 11u
#define LPUART_CTRL_M7_WIDTH 1u
#define LPUART_CTRL_M7(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_M7_SHIFT)) & LPUART_CTRL_M7_MASK)
#define LPUART_CTRL_MA2IE_MASK  0x4000u
#define LPUART_CTRL_MA2IE_SHIFT 14u
#define LPUART_CTRL_MA2IE_WIDTH 1u
#define LPUART_CTRL_MA2IE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_MA2IE_SHIFT)) & LPUART_CTRL_MA2IE_MASK)
#define LPUART_CTRL_MA1IE_MASK  0x8000u
#define LPUART_CTRL_MA1IE_SHIFT 15u
#define LPUART_CTRL_MA1IE_WIDTH 1u
#define LPUART_CTRL_MA1IE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_MA1IE_SHIFT)) & LPUART_CTRL_MA1IE_MASK)
#define LPUART_CTRL_RE_MASK  0x40000u
#define LPUART_CTRL_RE_SHIFT 18u
#define LPUART_CTRL_RE_WIDTH 1u
#define LPUART_CTRL_RE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_RE_SHIFT)) & LPUART_CTRL_RE_MASK)
#define LPUART_CTRL_TE_MASK  0x80000u
#define LPUART_CTRL_TE_SHIFT 19u
#define LPUART_CTRL_TE_WIDTH 1u
#define LPUART_CTRL_TE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_TE_SHIFT)) & LPUART_CTRL_TE_MASK)
#define LPUART_CTRL_ILIE_MASK  0x100000u
#define LPUART_CTRL_ILIE_SHIFT 20u
#define LPUART_CTRL_ILIE_WIDTH 1u
#define LPUART_CTRL_ILIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_ILIE_SHIFT)) & LPUART_CTRL_ILIE_MASK)
#define LPUART_CTRL_RIE_MASK  0x200000u
#define LPUART_CTRL_RIE_SHIFT 21u
#define LPUART_CTRL_RIE_WIDTH 1u
#define LPUART_CTRL_RIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_RIE_SHIFT)) & LPUART_CTRL_RIE_MASK)
#define LPUART_CTRL_TCIE_MASK  0x400000u
#define LPUART_CTRL_TCIE_SHIFT 22u
#define LPUART_CTRL_TCIE_WIDTH 1u
#define LPUART_CTRL_TCIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_TCIE_SHIFT)) & LPUART_CTRL_TCIE_MASK)
#define LPUART_CTRL_TIE_MASK  0x800000u
#define LPUART_CTRL_TIE_SHIFT 23u
#define LPUART_CTRL_TIE_WIDTH 1u
#define LPUART_CTRL_TIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_TIE_SHIFT)) & LPUART_CTRL_TIE_MASK)
#define LPUART_CTRL_PEIE_MASK  0x1000000u
#define LPUART_CTRL_PEIE_SHIFT 24u
#define LPUART_CTRL_PEIE_WIDTH 1u
#define LPUART_CTRL_PEIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_PEIE_SHIFT)) & LPUART_CTRL_PEIE_MASK)
#define LPUART_CTRL_FEIE_MASK  0x2000000u
#define LPUART_CTRL_FEIE_SHIFT 25u
#define LPUART_CTRL_FEIE_WIDTH 1u
#define LPUART_CTRL_FEIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_FEIE_SHIFT)) & LPUART_CTRL_FEIE_MASK)
#define LPUART_CTRL_NEIE_MASK  0x4000000u
#define LPUART_CTRL_NEIE_SHIFT 26u
#define LPUART_CTRL_NEIE_WIDTH 1u
#define LPUART_CTRL_NEIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_NEIE_SHIFT)) & LPUART_CTRL_NEIE_MASK)
#define LPUART_CTRL_ORIE_MASK  0x8000000u
#define LPUART_CTRL_ORIE_SHIFT 27u
#define LPUART_CTRL_ORIE_WIDTH 1u
#define LPUART_CTRL_ORIE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_ORIE_SHIFT)) & LPUART_CTRL_ORIE_MASK)
#define LPUART_CTRL_TXINV_MASK  0x10000000u
#define LPUART_CTRL_TXINV_SHIFT 28u
#define LPUART_CTRL_TXINV_WIDTH 1u
#define LPUART_CTRL_TXINV(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_TXINV_SHIFT)) & LPUART_CTRL_TXINV_MASK)
#define LPUART_CTRL_R9T8_MASK  0x40000000u
#define LPUART_CTRL_R9T8_SHIFT 30u
#define LPUART_CTRL_R9T8_WIDTH 1u
#define LPUART_CTRL_R9T8(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_R9T8_SHIFT)) & LPUART_CTRL_R9T8_MASK)
#define LPUART_CTRL_R8T9_MASK  0x80000000u
#define LPUART_CTRL_R8T9_SHIFT 31u
#define LPUART_CTRL_R8T9_WIDTH 1u
#define LPUART_CTRL_R8T9(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_R8T9_SHIFT)) & LPUART_CTRL_R8T9_MASK)
/* DATA Bit Fields */
#define LPUART_DATA_R0T0_MASK  0x1u
#define LPUART_DATA_R0T0_SHIFT 0u
#define LPUART_DATA_R0T0_WIDTH 1u
#define LPUART_DATA_R0T0(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R0T0_SHIFT)) & LPUART_DATA_R0T0_MASK)
#define LPUART_DATA_R1T1_MASK  0x2u
#define LPUART_DATA_R1T1_SHIFT 1u
#define LPUART_DATA_R1T1_WIDTH 1u
#define LPUART_DATA_R1T1(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R1T1_SHIFT)) & LPUART_DATA_R1T1_MASK)
#define LPUART_DATA_R2T2_MASK  0x4u
#define LPUART_DATA_R2T2_SHIFT 2u
#define LPUART_DATA_R2T2_WIDTH 1u
#define LPUART_DATA_R2T2(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R2T2_SHIFT)) & LPUART_DATA_R2T2_MASK)
#define LPUART_DATA_R3T3_MASK  0x8u
#define LPUART_DATA_R3T3_SHIFT 3u
#define LPUART_DATA_R3T3_WIDTH 1u
#define LPUART_DATA_R3T3(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R3T3_SHIFT)) & LPUART_DATA_R3T3_MASK)
#define LPUART_DATA_R4T4_MASK  0x10u
#define LPUART_DATA_R4T4_SHIFT 4u
#define LPUART_DATA_R4T4_WIDTH 1u
#define LPUART_DATA_R4T4(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R4T4_SHIFT)) & LPUART_DATA_R4T4_MASK)
#define LPUART_DATA_R5T5_MASK  0x20u
#define LPUART_DATA_R5T5_SHIFT 5u
#define LPUART_DATA_R5T5_WIDTH 1u
#define LPUART_DATA_R5T5(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R5T5_SHIFT)) & LPUART_DATA_R5T5_MASK)
#define LPUART_DATA_R6T6_MASK  0x40u
#define LPUART_DATA_R6T6_SHIFT 6u
#define LPUART_DATA_R6T6_WIDTH 1u
#define LPUART_DATA_R6T6(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R6T6_SHIFT)) & LPUART_DATA_R6T6_MASK)
#define LPUART_DATA_R7T7_MASK  0x80u
#define LPUART_DATA_R7T7_SHIFT 7u
#define LPUART_DATA_R7T7_WIDTH 1u
#define LPUART_DATA_R7T7(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R7T7_SHIFT)) & LPUART_DATA_R7T7_MASK)
#define LPUART_DATA_R8T8_MASK  0x100u
#define LPUART_DATA_R8T8_SHIFT 8u
#define LPUART_DATA_R8T8_WIDTH 1u
#define LPUART_DATA_R8T8(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R8T8_SHIFT)) & LPUART_DATA_R8T8_MASK)
#define LPUART_DATA_R9T9_MASK  0x200u
#define LPUART_DATA_R9T9_SHIFT 9u
#define LPUART_DATA_R9T9_WIDTH 1u
#define LPUART_DATA_R9T9(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R9T9_SHIFT)) & LPUART_DATA_R9T9_MASK)
#define LPUART_DATA_RXEMPT_MASK  0x1000u
#define LPUART_DATA_RXEMPT_SHIFT 12u
#define LPUART_DATA_RXEMPT_WIDTH 1u
#define LPUART_DATA_RXEMPT(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_RXEMPT_SHIFT)) & LPUART_DATA_RXEMPT_MASK)
/* MATCH Bit Fields */
#define LPUART_MATCH_MA1_MASK  0x3FFu
#define LPUART_MATCH_MA1_SHIFT 0u
#define LPUART_MATCH_MA1_WIDTH 10u
#define LPUART_MATCH_MA1(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_MATCH_MA1_SHIFT)) & LPUART_MATCH_MA1_MASK)
#define LPUART_MATCH_MA2_MASK  0x3FF0000u
#define LPUART_MATCH_MA2_SHIFT 16u
#define LPUART_MATCH_MA2_WIDTH 10u
#define LPUART_MATCH_MA2(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_MATCH_MA2_SHIFT)) & LPUART_MATCH_MA2_MASK)
/* FIFO Bit Fields */
#define LPUART_FIFO_RXFIFOSIZE_MASK  0x7u
#define LPUART_FIFO_RXFIFOSIZE_SHIFT 0u
#define LPUART_FIFO_RXFIFOSIZE_WIDTH 3u
#define LPUART_FIFO_RXFIFOSIZE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_FIFO_RXFIFOSIZE_SHIFT)) & LPUART_FIFO_RXFIFOSIZE_MASK)
#define LPUART_FIFO_RXFE_MASK  0x8u
#define LPUART_FIFO_RXFE_SHIFT 3u
#define LPUART_FIFO_RXFE_WIDTH 1u
#define LPUART_FIFO_RXFE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_FIFO_RXFE_SHIFT)) & LPUART_FIFO_RXFE_MASK)
#define LPUART_FIFO_TXFIFOSIZE_MASK  0x70u
#define LPUART_FIFO_TXFIFOSIZE_SHIFT 4u
#define LPUART_FIFO_TXFIFOSIZE_WIDTH 3u
#define LPUART_FIFO_TXFIFOSIZE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_FIFO_TXFIFOSIZE_SHIFT)) & LPUART_FIFO_TXFIFOSIZE_MASK)
#define LPUART_FIFO_TXFE_MASK  0x80u
#define LPUART_FIFO_TXFE_SHIFT 7u
#define LPUART_FIFO_TXFE_WIDTH 1u
#define LPUART_FIFO_TXFE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_FIFO_TXFE_SHIFT)) & LPUART_FIFO_TXFE_MASK)
#define LPUART_FIFO_RXUFE_MASK  0x100u
#define LPUART_FIFO_RXUFE_SHIFT 8u
#define LPUART_FIFO_RXUFE_WIDTH 1u
#define LPUART_FIFO_RXUFE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_FIFO_RXUFE_SHIFT)) & LPUART_FIFO_RXUFE_MASK)
#define LPUART_FIFO_TXOFE_MASK  0x200u
#define LPUART_FIFO_TXOFE_SHIFT 9u
#define LPUART_FIFO_TXOFE_WIDTH 1u
#define LPUART_FIFO_TXOFE(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_FIFO_TXOFE_SHIFT)) & LPUART_FIFO_TXOFE_MASK)
#define LPUART_FIFO_RXIDEN_MASK  0x1C00u
#define LPUART_FIFO_RXIDEN_SHIFT 10u
#define LPUART_FIFO_RXIDEN_WIDTH 3u
#define LPUART_FIFO_RXIDEN(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_FIFO_RXIDEN_SHIFT)) & LPUART_FIFO_RXIDEN_MASK)
#define LPUART_FIFO_RXFLUSH_MASK  0x4000u
#define LPUART_FIFO_RXFLUSH_SHIFT 14u
#define LPUART_FIFO_RXFLUSH_WIDTH 1u
#define LPUART_FIFO_RXFLUSH(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_FIFO_RXFLUSH_SHIFT)) & LPUART_FIFO_RXFLUSH_MASK)
#define LPUART_FIFO_TXFLUSH_MASK  0x8000u
#define LPUART_FIFO_TXFLUSH_SHIFT 15u
#define LPUART_FIFO_TXFLUSH_WIDTH 1u
#define LPUART_FIFO_TXFLUSH(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_FIFO_TXFLUSH_SHIFT)) & LPUART_FIFO_TXFLUSH_MASK)
#define LPUART_FIFO_RXUF_MASK  0x10000u
#define LPUART_FIFO_RXUF_SHIFT 16u
#define LPUART_FIFO_RXUF_WIDTH 1u
#define LPUART_FIFO_RXUF(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_FIFO_RXUF_SHIFT)) & LPUART_FIFO_RXUF_MASK)
#define LPUART_FIFO_TXOF_MASK  0x20000u
#define LPUART_FIFO_TXOF_SHIFT 17u
#define LPUART_FIFO_TXOF_WIDTH 1u
#define LPUART_FIFO_TXOF(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_FIFO_TXOF_SHIFT)) & LPUART_FIFO_TXOF_MASK)
#define LPUART_FIFO_RXEMPT_MASK  0x400000u
#define LPUART_FIFO_RXEMPT_SHIFT 22u
#define LPUART_FIFO_RXEMPT_WIDTH 1u
#define LPUART_FIFO_RXEMPT(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_FIFO_RXEMPT_SHIFT)) & LPUART_FIFO_RXEMPT_MASK)
#define LPUART_FIFO_TXEMPT_MASK  0x800000u
#define LPUART_FIFO_TXEMPT_SHIFT 23u
#define LPUART_FIFO_TXEMPT_WIDTH 1u
#define LPUART_FIFO_TXEMPT(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_FIFO_TXEMPT_SHIFT)) & LPUART_FIFO_TXEMPT_MASK)
/* WATER Bit Fields */
#define LPUART_WATER_TXWATER_MASK  0x3u
#define LPUART_WATER_TXWATER_SHIFT 0u
#define LPUART_WATER_TXWATER_WIDTH 2u
#define LPUART_WATER_TXWATER(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_WATER_TXWATER_SHIFT)) & LPUART_WATER_TXWATER_MASK)
#define LPUART_WATER_TXCOUNT_MASK  0x700u
#define LPUART_WATER_TXCOUNT_SHIFT 8u
#define LPUART_WATER_TXCOUNT_WIDTH 3u
#define LPUART_WATER_TXCOUNT(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_WATER_TXCOUNT_SHIFT)) & LPUART_WATER_TXCOUNT_MASK)
#define LPUART_WATER_RXWATER_MASK  0x30000u
#define LPUART_WATER_RXWATER_SHIFT 16u
#define LPUART_WATER_RXWATER_WIDTH 2u
#define LPUART_WATER_RXWATER(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_WATER_RXWATER_SHIFT)) & LPUART_WATER_RXWATER_MASK)
#define LPUART_WATER_RXCOUNT_MASK  0x7000000u
#define LPUART_WATER_RXCOUNT_SHIFT 24u
#define LPUART_WATER_RXCOUNT_WIDTH 3u
#define LPUART_WATER_RXCOUNT(x) \
    (((uint32_t)(((uint32_t)(x)) << LPUART_WATER_RXCOUNT_SHIFT)) & LPUART_WATER_RXCOUNT_MASK)

/**
 * @}
 */ /* end of group LPUART_Register_Masks */

/**
 * @}
 */ /* end of group LPUART_Peripheral_Access_Layer */

/* --------------------------------------------------------------------------
   -- PCC Peripheral Access Layer
   -------------------------------------------------------------------------- */

/**
 * @addtogroup PCC_Peripheral_Access_Layer PCC Peripheral Access Layer
 * @{
 */

/** PCC - Size of Registers Arrays */
#define PCC_PCCn_COUNT 125u

/** PCC - Register Layout Typedef */
typedef struct
{
    _IO uint32_t PCCn[PCC_PCCn_COUNT]; /**< array offset: 0x0 */
} PCC_t, *PCC_MemMapPtr;

/** Number of instances of the PCC module. */
#define PCC_INSTANCE_COUNT (1u)

/* PCC - Peripheral instance base addresses */
/** Peripheral PCC base address */
#define PCC_BASE (0x40065000u)
/** Peripheral PCC base pointer */
#define PCC ((PCC_t *)PCC_BASE)
/** Array initializer of PCC peripheral base addresses */
#define PCC_BASE_ADDRS \
    {                  \
        PCC_BASE       \
    }
/** Array initializer of PCC peripheral base pointers */
#define PCC_BASE_PTRS \
    {                 \
        PCC           \
    }

/* PCC index offsets */
#define PCC_LPSPI0_INDEX  44
#define PCC_LPSPI1_INDEX  45
#define PCC_LPSPI2_INDEX  46
#define PCC_FTM0_INDEX    56
#define PCC_FTM1_INDEX    57
#define PCC_LPTMR0_INDEX  64
#define PCC_LPI2C0_INDEX  102
#define PCC_LPI2C1_INDEX  103
#define PCC_LPUART0_INDEX 106
#define PCC_LPUART1_INDEX 107
#define PCC_FSUSB_INDEX   122
#define PCC_PUF_INDEX     123
#define PCC_HSSPI_INDEX   124

/* --------------------------------------------------------------------------
   -- PCC Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup PCC_Register_Masks PCC Register Masks
 * @{
 */

/* PCCn Bit Fields */
#define PCC_PCCn_PCD_MASK   0x7u
#define PCC_PCCn_PCD_SHIFT  0u
#define PCC_PCCn_PCD_WIDTH  3u
#define PCC_PCCn_PCD(x)     (((uint32_t)(((uint32_t)(x)) << PCC_PCCn_PCD_SHIFT)) & PCC_PCCn_PCD_MASK)
#define PCC_PCCn_FRAC_MASK  0x8u
#define PCC_PCCn_FRAC_SHIFT 3u
#define PCC_PCCn_FRAC_WIDTH 1u
#define PCC_PCCn_FRAC(x)    (((uint32_t)(((uint32_t)(x)) << PCC_PCCn_FRAC_SHIFT)) & PCC_PCCn_FRAC_MASK)
#define PCC_PCCn_PCS_MASK   0x7000000u
#define PCC_PCCn_PCS_SHIFT  24u
#define PCC_PCCn_PCS_WIDTH  3u
#define PCC_PCCn_PCS(x)     (((uint32_t)(((uint32_t)(x)) << PCC_PCCn_PCS_SHIFT)) & PCC_PCCn_PCS_MASK)
#define PCC_PCCn_PCGC_MASK  0x20000000u
#define PCC_PCCn_PCGC_SHIFT 29u
#define PCC_PCCn_PCGC_WIDTH 1u
#define PCC_PCCn_PCGC(x)    (((uint32_t)(((uint32_t)(x)) << PCC_PCCn_PCGC_SHIFT)) & PCC_PCCn_PCGC_MASK)
#define PCC_PCCn_CGC_MASK   0x40000000u
#define PCC_PCCn_CGC_SHIFT  30u
#define PCC_PCCn_CGC_WIDTH  1u
#define PCC_PCCn_CGC(x)     (((uint32_t)(((uint32_t)(x)) << PCC_PCCn_CGC_SHIFT)) & PCC_PCCn_CGC_MASK)
#define PCC_PCCn_PR_MASK    0x80000000u
#define PCC_PCCn_PR_SHIFT   31u
#define PCC_PCCn_PR_WIDTH   1u
#define PCC_PCCn_PR(x)      (((uint32_t)(((uint32_t)(x)) << PCC_PCCn_PR_SHIFT)) & PCC_PCCn_PR_MASK)

/**
 * @}
 */ /* end of group PCC_Register_Masks */

/**
 * @}
 */ /* end of group PCC_Peripheral_Access_Layer */

/* --------------------------------------------------------------------------
   -- PORT Peripheral Access Layer
   -------------------------------------------------------------------------- */

/**
 * @addtogroup PORT_Peripheral_Access_Layer PORT Peripheral Access Layer
 * @{
 */

/** PORT - Size of Registers Arrays */
#define PORT_PCR_COUNT 16u

/** PORT - Register Layout Typedef */
typedef struct
{
    _IO uint32_t PCR[PORT_PCR_COUNT]; /**< offset: 0x0 */
    _NA uint8_t  RESERVED_0[64];
    __O uint32_t GPCLR;               /**< offset: 0x80 */
    _NA uint8_t  RESERVED_1[4];
    __O uint32_t GICLR;               /**< offset: 0x88 */
    _NA uint8_t  RESERVED_2[20];
    _IO uint32_t ISFR;                /**< offset: 0xA0 */
    _NA uint8_t  RESERVED_3[28];
    _IO uint32_t DFER;                /**< offset: 0xC0 */
    _IO uint32_t DFCR;                /**< offset: 0xC4 */
    _IO uint32_t DFWR;                /**< offset: 0xC8 */
} PORT_t, *PORT_MemMapPtr;

/** Number of instances of the PORT module. */
#define PORT_INSTANCE_COUNT (5u)

/* PORT - Peripheral instance base addresses */
/** Peripheral PORTA base address */
#define PORTA_BASE (0x40049000u)
/** Peripheral PORTA base pointer */
#define PORTA ((PORT_t *)PORTA_BASE)
/** Peripheral PORTB base address */
#define PORTB_BASE (0x4004A000u)
/** Peripheral PORTB base pointer */
#define PORTB ((PORT_t *)PORTB_BASE)
/** Peripheral PORTC base address */
#define PORTC_BASE (0x4004B000u)
/** Peripheral PORTC base pointer */
#define PORTC ((PORT_t *)PORTC_BASE)
/** Peripheral PORTD base address */
#define PORTD_BASE (0x4004C000u)
/** Peripheral PORTD base pointer */
#define PORTD ((PORT_t *)PORTD_BASE)
/** Peripheral PORTE base address */
#define PORTE_BASE (0x4004D000u)
/** Peripheral PORTE base pointer */
#define PORTE ((PORT_t *)PORTE_BASE)
/** Array initializer of PORT peripheral base addresses */
#define PORT_BASE_ADDRS                                            \
    {                                                              \
        PORTA_BASE, PORTB_BASE, PORTC_BASE, PORTD_BASE, PORTE_BASE \
    }
/** Array initializer of PORT peripheral base pointers */
#define PORT_BASE_PTRS                    \
    {                                     \
        PORTA, PORTB, PORTC, PORTD, PORTE \
    }
/** Number of interrupt vector arrays for the PORT module. */
#define PORT_IRQS_ARR_COUNT (1u)
/** Number of interrupt channels for the PORT module. */
#define PORT_IRQS_CH_COUNT (1u)
/** Interrupt vectors for the PORT peripheral type */
#define PORT_IRQS                                             \
    {                                                         \
        PORT_IRQn, PORT_IRQn, PORT_IRQn, PORT_IRQn, PORT_IRQn \
    }

/* --------------------------------------------------------------------------
   -- PORT Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup PORT_Register_Masks PORT Register Masks
 * @{
 */

/* PCR Bit Fields */
#define PORT_PCR_PS_MASK    0x1u
#define PORT_PCR_PS_SHIFT   0u
#define PORT_PCR_PS_WIDTH   1u
#define PORT_PCR_PS(x)      (((uint32_t)(((uint32_t)(x)) << PORT_PCR_PS_SHIFT)) & PORT_PCR_PS_MASK)
#define PORT_PCR_PE_MASK    0x2u
#define PORT_PCR_PE_SHIFT   1u
#define PORT_PCR_PE_WIDTH   1u
#define PORT_PCR_PE(x)      (((uint32_t)(((uint32_t)(x)) << PORT_PCR_PE_SHIFT)) & PORT_PCR_PE_MASK)
#define PORT_PCR_DSE_MASK   0x40u
#define PORT_PCR_DSE_SHIFT  6u
#define PORT_PCR_DSE_WIDTH  1u
#define PORT_PCR_DSE(x)     (((uint32_t)(((uint32_t)(x)) << PORT_PCR_DSE_SHIFT)) & PORT_PCR_DSE_MASK)
#define PORT_PCR_MUX_MASK   0x700u
#define PORT_PCR_MUX_SHIFT  8u
#define PORT_PCR_MUX_WIDTH  3u
#define PORT_PCR_MUX(x)     (((uint32_t)(((uint32_t)(x)) << PORT_PCR_MUX_SHIFT)) & PORT_PCR_MUX_MASK)
#define PORT_PCR_LK_MASK    0x8000u
#define PORT_PCR_LK_SHIFT   15u
#define PORT_PCR_LK_WIDTH   1u
#define PORT_PCR_LK(x)      (((uint32_t)(((uint32_t)(x)) << PORT_PCR_LK_SHIFT)) & PORT_PCR_LK_MASK)
#define PORT_PCR_IRQC_MASK  0xF0000u
#define PORT_PCR_IRQC_SHIFT 16u
#define PORT_PCR_IRQC_WIDTH 4u
#define PORT_PCR_IRQC(x)    (((uint32_t)(((uint32_t)(x)) << PORT_PCR_IRQC_SHIFT)) & PORT_PCR_IRQC_MASK)
#define PORT_PCR_ISF_MASK   0x1000000u
#define PORT_PCR_ISF_SHIFT  24u
#define PORT_PCR_ISF_WIDTH  1u
#define PORT_PCR_ISF(x)     (((uint32_t)(((uint32_t)(x)) << PORT_PCR_ISF_SHIFT)) & PORT_PCR_ISF_MASK)
/* GPCLR Bit Fields */
#define PORT_GPCLR_GPWD_MASK  0xFFFFu
#define PORT_GPCLR_GPWD_SHIFT 0u
#define PORT_GPCLR_GPWD_WIDTH 16u
#define PORT_GPCLR_GPWD(x) \
    (((uint32_t)(((uint32_t)(x)) << PORT_GPCLR_GPWD_SHIFT)) & PORT_GPCLR_GPWD_MASK)
#define PORT_GPCLR_GPWE_MASK  0xFFFF0000u
#define PORT_GPCLR_GPWE_SHIFT 16u
#define PORT_GPCLR_GPWE_WIDTH 16u
#define PORT_GPCLR_GPWE(x) \
    (((uint32_t)(((uint32_t)(x)) << PORT_GPCLR_GPWE_SHIFT)) & PORT_GPCLR_GPWE_MASK)
/* GPCHR Bit Fields */
/* GICLR Bit Fields */
#define PORT_GICLR_GIWE_MASK  0xFFFFu
#define PORT_GICLR_GIWE_SHIFT 0u
#define PORT_GICLR_GIWE_WIDTH 16u
#define PORT_GICLR_GIWE(x) \
    (((uint32_t)(((uint32_t)(x)) << PORT_GICLR_GIWE_SHIFT)) & PORT_GICLR_GIWE_MASK)
#define PORT_GICLR_GIWD_MASK  0xFFFF0000u
#define PORT_GICLR_GIWD_SHIFT 16u
#define PORT_GICLR_GIWD_WIDTH 16u
#define PORT_GICLR_GIWD(x) \
    (((uint32_t)(((uint32_t)(x)) << PORT_GICLR_GIWD_SHIFT)) & PORT_GICLR_GIWD_MASK)
/* GICHR Bit Fields */
/* ISFR Bit Fields */
#define PORT_ISFR_ISF_MASK  0xFFFFu
#define PORT_ISFR_ISF_SHIFT 0u
#define PORT_ISFR_ISF_WIDTH 16u
#define PORT_ISFR_ISF(x)    (((uint32_t)(((uint32_t)(x)) << PORT_ISFR_ISF_SHIFT)) & PORT_ISFR_ISF_MASK)
/* DFER Bit Fields */
#define PORT_DFER_DFE_MASK  0xFFFFu
#define PORT_DFER_DFE_SHIFT 0u
#define PORT_DFER_DFE_WIDTH 16u
#define PORT_DFER_DFE(x)    (((uint32_t)(((uint32_t)(x)) << PORT_DFER_DFE_SHIFT)) & PORT_DFER_DFE_MASK)
/* DFCR Bit Fields */
#define PORT_DFCR_CS_MASK  0x1u
#define PORT_DFCR_CS_SHIFT 0u
#define PORT_DFCR_CS_WIDTH 1u
#define PORT_DFCR_CS(x)    (((uint32_t)(((uint32_t)(x)) << PORT_DFCR_CS_SHIFT)) & PORT_DFCR_CS_MASK)
/* DFWR Bit Fields */
#define PORT_DFWR_FILT_MASK  0x1Fu
#define PORT_DFWR_FILT_SHIFT 0u
#define PORT_DFWR_FILT_WIDTH 5u
#define PORT_DFWR_FILT(x) \
    (((uint32_t)(((uint32_t)(x)) << PORT_DFWR_FILT_SHIFT)) & PORT_DFWR_FILT_MASK)

/**
 * @}
 */ /* end of group PORT_Register_Masks */

/**
 * @}
 */ /* end of group PORT_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------
   -- RCM Peripheral Access Layer
   ---------------------------------------------------------------------- */

/**
 * @addtogroup RCM_Peripheral_Access_Layer RCM Peripheral Access Layer
 * @{
 */

/** RCM - Size of Registers Arrays */

/** RCM - Register Layout Typedef */
typedef struct
{
    __I uint32_t VERID; /**< offset: 0x0 */
    __I uint32_t PARAM; /**< offset: 0x4 */
    __I uint32_t SRS;   /**< offset: 0x8 */
    _IO uint32_t RPC;   /**< offset: 0xC */
    _NA uint8_t  RESERVED_0[8];
    _IO uint32_t SSRS;  /**< offset: 0x18 */
    _IO uint32_t SRIE;  /**< offset: 0x1C */
} RCM_t, *RCM_MemMapPtr;

/** Number of instances of the RCM module. */
#define RCM_INSTANCE_COUNT (1u)

/* RCM - Peripheral instance base addresses */
/** Peripheral RCM base address */
#define RCM_BASE (0x4007F000u)
/** Peripheral RCM base pointer */
#define RCM ((RCM_t *)RCM_BASE)
/** Array initializer of RCM peripheral base addresses */
#define RCM_BASE_ADDRS \
    {                  \
        RCM_BASE       \
    }
/** Array initializer of RCM peripheral base pointers */
#define RCM_BASE_PTRS \
    {                 \
        RCM           \
    }
/** Number of interrupt vector arrays for the RCM module. */
#define RCM_IRQS_ARR_COUNT (1u)
/** Number of interrupt channels for the RCM module. */
#define RCM_IRQS_CH_COUNT (1u)
/** Interrupt vectors for the RCM peripheral type */
#define RCM_IRQS \
    {            \
        RCM_IRQn \
    }

/* --------------------------------------------------------------------------
   -- RCM Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup RCM_Register_Masks RCM Register Masks
 * @{
 */

/* VERID Bit Fields */
#define RCM_VERID_FEATURE_MASK  0xFFFFu
#define RCM_VERID_FEATURE_SHIFT 0u
#define RCM_VERID_FEATURE_WIDTH 16u
#define RCM_VERID_FEATURE(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_VERID_FEATURE_SHIFT)) & RCM_VERID_FEATURE_MASK)
#define RCM_VERID_MINOR_MASK  0xFF0000u
#define RCM_VERID_MINOR_SHIFT 16u
#define RCM_VERID_MINOR_WIDTH 8u
#define RCM_VERID_MINOR(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_VERID_MINOR_SHIFT)) & RCM_VERID_MINOR_MASK)
#define RCM_VERID_MAJOR_MASK  0xFF000000u
#define RCM_VERID_MAJOR_SHIFT 24u
#define RCM_VERID_MAJOR_WIDTH 8u
#define RCM_VERID_MAJOR(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_VERID_MAJOR_SHIFT)) & RCM_VERID_MAJOR_MASK)
/* PARAM Bit Fields */
#define RCM_PARAM_EWAKEUP_MASK  0x1u
#define RCM_PARAM_EWAKEUP_SHIFT 0u
#define RCM_PARAM_EWAKEUP_WIDTH 1u
#define RCM_PARAM_EWAKEUP(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_PARAM_EWAKEUP_SHIFT)) & RCM_PARAM_EWAKEUP_MASK)
#define RCM_PARAM_ELVD_MASK  0x2u
#define RCM_PARAM_ELVD_SHIFT 1u
#define RCM_PARAM_ELVD_WIDTH 1u
#define RCM_PARAM_ELVD(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_PARAM_ELVD_SHIFT)) & RCM_PARAM_ELVD_MASK)
#define RCM_PARAM_ELOC_MASK  0x4u
#define RCM_PARAM_ELOC_SHIFT 2u
#define RCM_PARAM_ELOC_WIDTH 1u
#define RCM_PARAM_ELOC(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_PARAM_ELOC_SHIFT)) & RCM_PARAM_ELOC_MASK)
#define RCM_PARAM_ELOL_MASK  0x8u
#define RCM_PARAM_ELOL_SHIFT 3u
#define RCM_PARAM_ELOL_WIDTH 1u
#define RCM_PARAM_ELOL(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_PARAM_ELOL_SHIFT)) & RCM_PARAM_ELOL_MASK)
#define RCM_PARAM_ECMU_LOC_MASK  0x10u
#define RCM_PARAM_ECMU_LOC_SHIFT 4u
#define RCM_PARAM_ECMU_LOC_WIDTH 1u
#define RCM_PARAM_ECMU_LOC(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_PARAM_ECMU_LOC_SHIFT)) & RCM_PARAM_ECMU_LOC_MASK)
#define RCM_PARAM_EWDOG_MASK  0x20u
#define RCM_PARAM_EWDOG_SHIFT 5u
#define RCM_PARAM_EWDOG_WIDTH 1u
#define RCM_PARAM_EWDOG(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_PARAM_EWDOG_SHIFT)) & RCM_PARAM_EWDOG_MASK)
#define RCM_PARAM_EPIN_MASK  0x40u
#define RCM_PARAM_EPIN_SHIFT 6u
#define RCM_PARAM_EPIN_WIDTH 1u
#define RCM_PARAM_EPIN(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_PARAM_EPIN_SHIFT)) & RCM_PARAM_EPIN_MASK)
#define RCM_PARAM_EPOR_MASK  0x80u
#define RCM_PARAM_EPOR_SHIFT 7u
#define RCM_PARAM_EPOR_WIDTH 1u
#define RCM_PARAM_EPOR(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_PARAM_EPOR_SHIFT)) & RCM_PARAM_EPOR_MASK)
#define RCM_PARAM_EJTAG_MASK  0x100u
#define RCM_PARAM_EJTAG_SHIFT 8u
#define RCM_PARAM_EJTAG_WIDTH 1u
#define RCM_PARAM_EJTAG(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_PARAM_EJTAG_SHIFT)) & RCM_PARAM_EJTAG_MASK)
#define RCM_PARAM_ELOCKUP_MASK  0x200u
#define RCM_PARAM_ELOCKUP_SHIFT 9u
#define RCM_PARAM_ELOCKUP_WIDTH 1u
#define RCM_PARAM_ELOCKUP(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_PARAM_ELOCKUP_SHIFT)) & RCM_PARAM_ELOCKUP_MASK)
#define RCM_PARAM_ESW_MASK      0x400u
#define RCM_PARAM_ESW_SHIFT     10u
#define RCM_PARAM_ESW_WIDTH     1u
#define RCM_PARAM_ESW(x)        (((uint32_t)(((uint32_t)(x)) << RCM_PARAM_ESW_SHIFT)) & RCM_PARAM_ESW_MASK)
#define RCM_PARAM_EMDM_AP_MASK  0x800u
#define RCM_PARAM_EMDM_AP_SHIFT 11u
#define RCM_PARAM_EMDM_AP_WIDTH 1u
#define RCM_PARAM_EMDM_AP(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_PARAM_EMDM_AP_SHIFT)) & RCM_PARAM_EMDM_AP_MASK)
#define RCM_PARAM_ESACKERR_MASK  0x2000u
#define RCM_PARAM_ESACKERR_SHIFT 13u
#define RCM_PARAM_ESACKERR_WIDTH 1u
#define RCM_PARAM_ESACKERR(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_PARAM_ESACKERR_SHIFT)) & RCM_PARAM_ESACKERR_MASK)
#define RCM_PARAM_ETAMPER_MASK  0x8000u
#define RCM_PARAM_ETAMPER_SHIFT 15u
#define RCM_PARAM_ETAMPER_WIDTH 1u
#define RCM_PARAM_ETAMPER(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_PARAM_ETAMPER_SHIFT)) & RCM_PARAM_ETAMPER_MASK)
#define RCM_PARAM_ECORE1_MASK  0x10000u
#define RCM_PARAM_ECORE1_SHIFT 16u
#define RCM_PARAM_ECORE1_WIDTH 1u
#define RCM_PARAM_ECORE1(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_PARAM_ECORE1_SHIFT)) & RCM_PARAM_ECORE1_MASK)
/* SRS Bit Fields */
#define RCM_SRS_LVD_MASK      0x2u
#define RCM_SRS_LVD_SHIFT     1u
#define RCM_SRS_LVD_WIDTH     1u
#define RCM_SRS_LVD(x)        (((uint32_t)(((uint32_t)(x)) << RCM_SRS_LVD_SHIFT)) & RCM_SRS_LVD_MASK)
#define RCM_SRS_LOC_MASK      0x4u
#define RCM_SRS_LOC_SHIFT     2u
#define RCM_SRS_LOC_WIDTH     1u
#define RCM_SRS_LOC(x)        (((uint32_t)(((uint32_t)(x)) << RCM_SRS_LOC_SHIFT)) & RCM_SRS_LOC_MASK)
#define RCM_SRS_LOL_MASK      0x8u
#define RCM_SRS_LOL_SHIFT     3u
#define RCM_SRS_LOL_WIDTH     1u
#define RCM_SRS_LOL(x)        (((uint32_t)(((uint32_t)(x)) << RCM_SRS_LOL_SHIFT)) & RCM_SRS_LOL_MASK)
#define RCM_SRS_CMU_LOC_MASK  0x10u
#define RCM_SRS_CMU_LOC_SHIFT 4u
#define RCM_SRS_CMU_LOC_WIDTH 1u
#define RCM_SRS_CMU_LOC(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_SRS_CMU_LOC_SHIFT)) & RCM_SRS_CMU_LOC_MASK)
#define RCM_SRS_WDOG_MASK    0x20u
#define RCM_SRS_WDOG_SHIFT   5u
#define RCM_SRS_WDOG_WIDTH   1u
#define RCM_SRS_WDOG(x)      (((uint32_t)(((uint32_t)(x)) << RCM_SRS_WDOG_SHIFT)) & RCM_SRS_WDOG_MASK)
#define RCM_SRS_PIN_MASK     0x40u
#define RCM_SRS_PIN_SHIFT    6u
#define RCM_SRS_PIN_WIDTH    1u
#define RCM_SRS_PIN(x)       (((uint32_t)(((uint32_t)(x)) << RCM_SRS_PIN_SHIFT)) & RCM_SRS_PIN_MASK)
#define RCM_SRS_POR_MASK     0x80u
#define RCM_SRS_POR_SHIFT    7u
#define RCM_SRS_POR_WIDTH    1u
#define RCM_SRS_POR(x)       (((uint32_t)(((uint32_t)(x)) << RCM_SRS_POR_SHIFT)) & RCM_SRS_POR_MASK)
#define RCM_SRS_JTAG_MASK    0x100u
#define RCM_SRS_JTAG_SHIFT   8u
#define RCM_SRS_JTAG_WIDTH   1u
#define RCM_SRS_JTAG(x)      (((uint32_t)(((uint32_t)(x)) << RCM_SRS_JTAG_SHIFT)) & RCM_SRS_JTAG_MASK)
#define RCM_SRS_LOCKUP_MASK  0x200u
#define RCM_SRS_LOCKUP_SHIFT 9u
#define RCM_SRS_LOCKUP_WIDTH 1u
#define RCM_SRS_LOCKUP(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_SRS_LOCKUP_SHIFT)) & RCM_SRS_LOCKUP_MASK)
#define RCM_SRS_SW_MASK      0x400u
#define RCM_SRS_SW_SHIFT     10u
#define RCM_SRS_SW_WIDTH     1u
#define RCM_SRS_SW(x)        (((uint32_t)(((uint32_t)(x)) << RCM_SRS_SW_SHIFT)) & RCM_SRS_SW_MASK)
#define RCM_SRS_MDM_AP_MASK  0x800u
#define RCM_SRS_MDM_AP_SHIFT 11u
#define RCM_SRS_MDM_AP_WIDTH 1u
#define RCM_SRS_MDM_AP(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_SRS_MDM_AP_SHIFT)) & RCM_SRS_MDM_AP_MASK)
#define RCM_SRS_SACKERR_MASK  0x2000u
#define RCM_SRS_SACKERR_SHIFT 13u
#define RCM_SRS_SACKERR_WIDTH 1u
#define RCM_SRS_SACKERR(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_SRS_SACKERR_SHIFT)) & RCM_SRS_SACKERR_MASK)
/* RPC Bit Fields */
#define RCM_RPC_RSTFLTSRW_MASK  0x3u
#define RCM_RPC_RSTFLTSRW_SHIFT 0u
#define RCM_RPC_RSTFLTSRW_WIDTH 2u
#define RCM_RPC_RSTFLTSRW(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_RPC_RSTFLTSRW_SHIFT)) & RCM_RPC_RSTFLTSRW_MASK)
#define RCM_RPC_RSTFLTSS_MASK  0x4u
#define RCM_RPC_RSTFLTSS_SHIFT 2u
#define RCM_RPC_RSTFLTSS_WIDTH 1u
#define RCM_RPC_RSTFLTSS(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_RPC_RSTFLTSS_SHIFT)) & RCM_RPC_RSTFLTSS_MASK)
#define RCM_RPC_RSTFLTSEL_MASK  0x1F00u
#define RCM_RPC_RSTFLTSEL_SHIFT 8u
#define RCM_RPC_RSTFLTSEL_WIDTH 5u
#define RCM_RPC_RSTFLTSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_RPC_RSTFLTSEL_SHIFT)) & RCM_RPC_RSTFLTSEL_MASK)
/* SSRS Bit Fields */
#define RCM_SSRS_SLVD_MASK      0x2u
#define RCM_SSRS_SLVD_SHIFT     1u
#define RCM_SSRS_SLVD_WIDTH     1u
#define RCM_SSRS_SLVD(x)        (((uint32_t)(((uint32_t)(x)) << RCM_SSRS_SLVD_SHIFT)) & RCM_SSRS_SLVD_MASK)
#define RCM_SSRS_SLOC_MASK      0x4u
#define RCM_SSRS_SLOC_SHIFT     2u
#define RCM_SSRS_SLOC_WIDTH     1u
#define RCM_SSRS_SLOC(x)        (((uint32_t)(((uint32_t)(x)) << RCM_SSRS_SLOC_SHIFT)) & RCM_SSRS_SLOC_MASK)
#define RCM_SSRS_SLOL_MASK      0x8u
#define RCM_SSRS_SLOL_SHIFT     3u
#define RCM_SSRS_SLOL_WIDTH     1u
#define RCM_SSRS_SLOL(x)        (((uint32_t)(((uint32_t)(x)) << RCM_SSRS_SLOL_SHIFT)) & RCM_SSRS_SLOL_MASK)
#define RCM_SSRS_SCMU_LOC_MASK  0x10u
#define RCM_SSRS_SCMU_LOC_SHIFT 4u
#define RCM_SSRS_SCMU_LOC_WIDTH 1u
#define RCM_SSRS_SCMU_LOC(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_SSRS_SCMU_LOC_SHIFT)) & RCM_SSRS_SCMU_LOC_MASK)
#define RCM_SSRS_SWDOG_MASK  0x20u
#define RCM_SSRS_SWDOG_SHIFT 5u
#define RCM_SSRS_SWDOG_WIDTH 1u
#define RCM_SSRS_SWDOG(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_SSRS_SWDOG_SHIFT)) & RCM_SSRS_SWDOG_MASK)
#define RCM_SSRS_SPIN_MASK   0x40u
#define RCM_SSRS_SPIN_SHIFT  6u
#define RCM_SSRS_SPIN_WIDTH  1u
#define RCM_SSRS_SPIN(x)     (((uint32_t)(((uint32_t)(x)) << RCM_SSRS_SPIN_SHIFT)) & RCM_SSRS_SPIN_MASK)
#define RCM_SSRS_SPOR_MASK   0x80u
#define RCM_SSRS_SPOR_SHIFT  7u
#define RCM_SSRS_SPOR_WIDTH  1u
#define RCM_SSRS_SPOR(x)     (((uint32_t)(((uint32_t)(x)) << RCM_SSRS_SPOR_SHIFT)) & RCM_SSRS_SPOR_MASK)
#define RCM_SSRS_SJTAG_MASK  0x100u
#define RCM_SSRS_SJTAG_SHIFT 8u
#define RCM_SSRS_SJTAG_WIDTH 1u
#define RCM_SSRS_SJTAG(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_SSRS_SJTAG_SHIFT)) & RCM_SSRS_SJTAG_MASK)
#define RCM_SSRS_SLOCKUP_MASK  0x200u
#define RCM_SSRS_SLOCKUP_SHIFT 9u
#define RCM_SSRS_SLOCKUP_WIDTH 1u
#define RCM_SSRS_SLOCKUP(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_SSRS_SLOCKUP_SHIFT)) & RCM_SSRS_SLOCKUP_MASK)
#define RCM_SSRS_SSW_MASK      0x400u
#define RCM_SSRS_SSW_SHIFT     10u
#define RCM_SSRS_SSW_WIDTH     1u
#define RCM_SSRS_SSW(x)        (((uint32_t)(((uint32_t)(x)) << RCM_SSRS_SSW_SHIFT)) & RCM_SSRS_SSW_MASK)
#define RCM_SSRS_SMDM_AP_MASK  0x800u
#define RCM_SSRS_SMDM_AP_SHIFT 11u
#define RCM_SSRS_SMDM_AP_WIDTH 1u
#define RCM_SSRS_SMDM_AP(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_SSRS_SMDM_AP_SHIFT)) & RCM_SSRS_SMDM_AP_MASK)
#define RCM_SSRS_SSACKERR_MASK  0x2000u
#define RCM_SSRS_SSACKERR_SHIFT 13u
#define RCM_SSRS_SSACKERR_WIDTH 1u
#define RCM_SSRS_SSACKERR(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_SSRS_SSACKERR_SHIFT)) & RCM_SSRS_SSACKERR_MASK)
/* SRIE Bit Fields */
#define RCM_SRIE_DELAY_MASK  0x3u
#define RCM_SRIE_DELAY_SHIFT 0u
#define RCM_SRIE_DELAY_WIDTH 2u
#define RCM_SRIE_DELAY(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_SRIE_DELAY_SHIFT)) & RCM_SRIE_DELAY_MASK)
#define RCM_SRIE_LOC_MASK      0x4u
#define RCM_SRIE_LOC_SHIFT     2u
#define RCM_SRIE_LOC_WIDTH     1u
#define RCM_SRIE_LOC(x)        (((uint32_t)(((uint32_t)(x)) << RCM_SRIE_LOC_SHIFT)) & RCM_SRIE_LOC_MASK)
#define RCM_SRIE_LOL_MASK      0x8u
#define RCM_SRIE_LOL_SHIFT     3u
#define RCM_SRIE_LOL_WIDTH     1u
#define RCM_SRIE_LOL(x)        (((uint32_t)(((uint32_t)(x)) << RCM_SRIE_LOL_SHIFT)) & RCM_SRIE_LOL_MASK)
#define RCM_SRIE_CMU_LOC_MASK  0x10u
#define RCM_SRIE_CMU_LOC_SHIFT 4u
#define RCM_SRIE_CMU_LOC_WIDTH 1u
#define RCM_SRIE_CMU_LOC(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_SRIE_CMU_LOC_SHIFT)) & RCM_SRIE_CMU_LOC_MASK)
#define RCM_SRIE_WDOG_MASK    0x20u
#define RCM_SRIE_WDOG_SHIFT   5u
#define RCM_SRIE_WDOG_WIDTH   1u
#define RCM_SRIE_WDOG(x)      (((uint32_t)(((uint32_t)(x)) << RCM_SRIE_WDOG_SHIFT)) & RCM_SRIE_WDOG_MASK)
#define RCM_SRIE_PIN_MASK     0x40u
#define RCM_SRIE_PIN_SHIFT    6u
#define RCM_SRIE_PIN_WIDTH    1u
#define RCM_SRIE_PIN(x)       (((uint32_t)(((uint32_t)(x)) << RCM_SRIE_PIN_SHIFT)) & RCM_SRIE_PIN_MASK)
#define RCM_SRIE_GIE_MASK     0x80u
#define RCM_SRIE_GIE_SHIFT    7u
#define RCM_SRIE_GIE_WIDTH    1u
#define RCM_SRIE_GIE(x)       (((uint32_t)(((uint32_t)(x)) << RCM_SRIE_GIE_SHIFT)) & RCM_SRIE_GIE_MASK)
#define RCM_SRIE_JTAG_MASK    0x100u
#define RCM_SRIE_JTAG_SHIFT   8u
#define RCM_SRIE_JTAG_WIDTH   1u
#define RCM_SRIE_JTAG(x)      (((uint32_t)(((uint32_t)(x)) << RCM_SRIE_JTAG_SHIFT)) & RCM_SRIE_JTAG_MASK)
#define RCM_SRIE_LOCKUP_MASK  0x200u
#define RCM_SRIE_LOCKUP_SHIFT 9u
#define RCM_SRIE_LOCKUP_WIDTH 1u
#define RCM_SRIE_LOCKUP(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_SRIE_LOCKUP_SHIFT)) & RCM_SRIE_LOCKUP_MASK)
#define RCM_SRIE_SW_MASK      0x400u
#define RCM_SRIE_SW_SHIFT     10u
#define RCM_SRIE_SW_WIDTH     1u
#define RCM_SRIE_SW(x)        (((uint32_t)(((uint32_t)(x)) << RCM_SRIE_SW_SHIFT)) & RCM_SRIE_SW_MASK)
#define RCM_SRIE_MDM_AP_MASK  0x800u
#define RCM_SRIE_MDM_AP_SHIFT 11u
#define RCM_SRIE_MDM_AP_WIDTH 1u
#define RCM_SRIE_MDM_AP(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_SRIE_MDM_AP_SHIFT)) & RCM_SRIE_MDM_AP_MASK)
#define RCM_SRIE_SACKERR_MASK  0x2000u
#define RCM_SRIE_SACKERR_SHIFT 13u
#define RCM_SRIE_SACKERR_WIDTH 1u
#define RCM_SRIE_SACKERR(x) \
    (((uint32_t)(((uint32_t)(x)) << RCM_SRIE_SACKERR_SHIFT)) & RCM_SRIE_SACKERR_MASK)

/**
 * @}
 */ /* end of group RCM_Register_Masks */

/**
 * @}
 */ /* end of group RCM_Peripheral_Access_Layer */

/* --------------------------------------------------------------------------
   -- NVIC Peripheral Access Layer
   -------------------------------------------------------------------------- */

/**
 * @addtogroup NVIC_Peripheral_Access_Layer NVIC Peripheral Access Layer
 * @{
 */

/** NVIC - Size of Registers Arrays */
#define NVIC_ISER_COUNT 1u
#define NVIC_ICER_COUNT 1u
#define NVIC_ISPR_COUNT 1u
#define NVIC_ICPR_COUNT 1u
#define NVIC_IPR_COUNT  8u

/** NVIC - Register Layout Typedef */
typedef struct
{
    _IO uint32_t ISER[NVIC_ISER_COUNT]; /**< offset: 0x0, array step: 0x4 */
    _NA uint8_t  RESERVED_0[124];
    _IO uint32_t ICER[NVIC_ICER_COUNT]; /**< offset: 0x80, array step: 0x4 */
    _NA uint8_t  RESERVED_1[124];
    _IO uint32_t ISPR[NVIC_ISPR_COUNT]; /**< offset: 0x100, array step: 0x4 */
    _NA uint8_t  RESERVED_2[124];
    _IO uint32_t ICPR[NVIC_ICPR_COUNT]; /**< offset: 0x180, array step: 0x4 */
    _NA uint8_t  RESERVED_3[380];
    _IO uint32_t IPR[NVIC_IPR_COUNT];   /**< offset: 0x300, array step: 0x4 */
} NVIC_t, *NVIC_MemMapPtr;

/** Number of instances of the NVIC module. */
#define NVIC_INSTANCE_COUNT (1u)

/* NVIC - Peripheral instance base addresses */
/** Peripheral NVIC base address */
#define NVIC_BASE (0xE000E100u)
/** Peripheral NVIC base pointer */
#define NVIC ((NVIC_t *)NVIC_BASE)
/** Array initializer of NVIC peripheral base addresses */
#define NVIC_BASE_ADDRS \
    {                   \
        NVIC_BASE       \
    }
/** Array initializer of NVIC peripheral base pointers */
#define NVIC_BASE_PTRS \
    {                  \
        NVIC           \
    }

/* --------------------------------------------------------------------------
   -- NVIC Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup NVIC_Register_Masks NVIC Register Masks
 * @{
 */

/* ISER Bit Fields */
#define NVIC_ISER_SETENA_MASK  0xFFFFFFFFu
#define NVIC_ISER_SETENA_SHIFT 0u
#define NVIC_ISER_SETENA_WIDTH 32u
#define NVIC_ISER_SETENA(x) \
    (((uint32_t)(((uint32_t)(x)) << NVIC_ISER_SETENA_SHIFT)) & NVIC_ISER_SETENA_MASK)
/* ICER Bit Fields */
#define NVIC_ICER_CLRENA_MASK  0xFFFFFFFFu
#define NVIC_ICER_CLRENA_SHIFT 0u
#define NVIC_ICER_CLRENA_WIDTH 32u
#define NVIC_ICER_CLRENA(x) \
    (((uint32_t)(((uint32_t)(x)) << NVIC_ICER_CLRENA_SHIFT)) & NVIC_ICER_CLRENA_MASK)
/* ISPR Bit Fields */
#define NVIC_ISPR_SETPEND_MASK  0xFFFFFFFFu
#define NVIC_ISPR_SETPEND_SHIFT 0u
#define NVIC_ISPR_SETPEND_WIDTH 32u
#define NVIC_ISPR_SETPEND(x) \
    (((uint32_t)(((uint32_t)(x)) << NVIC_ISPR_SETPEND_SHIFT)) & NVIC_ISPR_SETPEND_MASK)
/* ICPR Bit Fields */
#define NVIC_ICPR_CLRPEND_MASK  0xFFFFFFFFu
#define NVIC_ICPR_CLRPEND_SHIFT 0u
#define NVIC_ICPR_CLRPEND_WIDTH 32u
#define NVIC_ICPR_CLRPEND(x) \
    (((uint32_t)(((uint32_t)(x)) << NVIC_ICPR_CLRPEND_SHIFT)) & NVIC_ICPR_CLRPEND_MASK)
/* IPR Bit Fields */
#define NVIC_IPR_PRI_0_MASK  0xFFu
#define NVIC_IPR_PRI_0_SHIFT 0u
#define NVIC_IPR_PRI_0_WIDTH 8u
#define NVIC_IPR_PRI_0(x) \
    (((uint32_t)(((uint32_t)(x)) << NVIC_IPR_PRI_0_SHIFT)) & NVIC_IPR_PRI_0_MASK)
#define NVIC_IPR_PRI_1_MASK  0xFF00u
#define NVIC_IPR_PRI_1_SHIFT 8u
#define NVIC_IPR_PRI_1_WIDTH 8u
#define NVIC_IPR_PRI_1(x) \
    (((uint32_t)(((uint32_t)(x)) << NVIC_IPR_PRI_1_SHIFT)) & NVIC_IPR_PRI_1_MASK)
#define NVIC_IPR_PRI_2_MASK  0xFF0000u
#define NVIC_IPR_PRI_2_SHIFT 16u
#define NVIC_IPR_PRI_2_WIDTH 8u
#define NVIC_IPR_PRI_2(x) \
    (((uint32_t)(((uint32_t)(x)) << NVIC_IPR_PRI_2_SHIFT)) & NVIC_IPR_PRI_2_MASK)
#define NVIC_IPR_PRI_3_MASK  0xFF000000u
#define NVIC_IPR_PRI_3_SHIFT 24u
#define NVIC_IPR_PRI_3_WIDTH 8u
#define NVIC_IPR_PRI_3(x) \
    (((uint32_t)(((uint32_t)(x)) << NVIC_IPR_PRI_3_SHIFT)) & NVIC_IPR_PRI_3_MASK)

/**
 * @}
 */ /* end of group NVIC_Register_Masks */

/**
 * @}
 */ /* end of group NVIC_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------
   -- SCB Peripheral Access Layer
   ---------------------------------------------------------------------- */

/**
 * @addtogroup SCB_Peripheral_Access_Layer SCB Peripheral Access
 * Layer
 * @{
 */

/** SCB - Size of Registers Arrays */

/** SCB - Register Layout Typedef */
typedef struct
{
    _NA uint8_t  RESERVED_0[8];
    __I uint32_t ACTLR; /**< offset: 0x8 */
    _NA uint8_t  RESERVED_1[3316];
    __I uint32_t CPUID; /**< offset: 0xD00 */
    _IO uint32_t ICSR;  /**< offset: 0xD04 */
    _NA uint8_t  RESERVED_2[4];
    _IO uint32_t AIRCR; /**< offset: 0xD0C */
    _IO uint32_t SCR;   /**< offset: 0xD10 */
    __I uint32_t CCR;   /**< offset: 0xD14 */
    _NA uint8_t  RESERVED_3[4];
    _IO uint32_t SHPR2; /**< offset: 0xD1C */
    _IO uint32_t SHPR3; /**< offset: 0xD20 */
    _IO uint32_t SHCSR; /**< offset: 0xD24 */
    _NA uint8_t  RESERVED_4[8];
    _IO uint32_t DFSR;  /**< offset: 0xD30 */
} SCB_t, *SCB_MemMapPtr;

/** Number of instances of the SCB module. */
#define SCB_INSTANCE_COUNT (1u)

/* SCB - Peripheral instance base addresses */
/** Peripheral SCB base address */
#define SCB_BASE (0xE000E000u)
/** Peripheral SCB base pointer */
#define SCB ((SCB_t *)SCB_BASE)
/** Array initializer of SCB peripheral base addresses */
#define SCB_BASE_ADDRS \
    {                  \
        SCB_BASE       \
    }
/** Array initializer of SCB peripheral base pointers */
#define SCB_BASE_PTRS \
    {                 \
        SCB           \
    }

/* --------------------------------------------------------------------------
   -- SCB Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup SCB_Register_Masks SCB Register Masks
 * @{
 */

/* CPUID Bit Fields */
#define SCB_CPUID_REVISION_MASK  0xFu
#define SCB_CPUID_REVISION_SHIFT 0u
#define SCB_CPUID_REVISION_WIDTH 4u
#define SCB_CPUID_REVISION(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_CPUID_REVISION_SHIFT)) & SCB_CPUID_REVISION_MASK)
#define SCB_CPUID_PARTNO_MASK  0xFFF0u
#define SCB_CPUID_PARTNO_SHIFT 4u
#define SCB_CPUID_PARTNO_WIDTH 12u
#define SCB_CPUID_PARTNO(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_CPUID_PARTNO_SHIFT)) & SCB_CPUID_PARTNO_MASK)
#define SCB_CPUID_VARIANT_MASK  0xF00000u
#define SCB_CPUID_VARIANT_SHIFT 20u
#define SCB_CPUID_VARIANT_WIDTH 4u
#define SCB_CPUID_VARIANT(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_CPUID_VARIANT_SHIFT)) & SCB_CPUID_VARIANT_MASK)
#define SCB_CPUID_IMPLEMENTER_MASK  0xFF000000u
#define SCB_CPUID_IMPLEMENTER_SHIFT 24u
#define SCB_CPUID_IMPLEMENTER_WIDTH 8u
#define SCB_CPUID_IMPLEMENTER(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_CPUID_IMPLEMENTER_SHIFT)) & SCB_CPUID_IMPLEMENTER_MASK)
/* ICSR Bit Fields */
#define SCB_ICSR_VECTACTIVE_MASK  0x3Fu
#define SCB_ICSR_VECTACTIVE_SHIFT 0u
#define SCB_ICSR_VECTACTIVE_WIDTH 6u
#define SCB_ICSR_VECTACTIVE(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_ICSR_VECTACTIVE_SHIFT)) & SCB_ICSR_VECTACTIVE_MASK)
#define SCB_ICSR_VECTPENDING_MASK  0x3F000u
#define SCB_ICSR_VECTPENDING_SHIFT 12u
#define SCB_ICSR_VECTPENDING_WIDTH 6u
#define SCB_ICSR_VECTPENDING(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_ICSR_VECTPENDING_SHIFT)) & SCB_ICSR_VECTPENDING_MASK)
#define SCB_ICSR_ISRPENDING_MASK  0x400000u
#define SCB_ICSR_ISRPENDING_SHIFT 22u
#define SCB_ICSR_ISRPENDING_WIDTH 1u
#define SCB_ICSR_ISRPENDING(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_ICSR_ISRPENDING_SHIFT)) & SCB_ICSR_ISRPENDING_MASK)
#define SCB_ICSR_PENDSTCLR_MASK  0x2000000u
#define SCB_ICSR_PENDSTCLR_SHIFT 25u
#define SCB_ICSR_PENDSTCLR_WIDTH 1u
#define SCB_ICSR_PENDSTCLR(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_ICSR_PENDSTCLR_SHIFT)) & SCB_ICSR_PENDSTCLR_MASK)
#define SCB_ICSR_PENDSTSET_MASK  0x4000000u
#define SCB_ICSR_PENDSTSET_SHIFT 26u
#define SCB_ICSR_PENDSTSET_WIDTH 1u
#define SCB_ICSR_PENDSTSET(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_ICSR_PENDSTSET_SHIFT)) & SCB_ICSR_PENDSTSET_MASK)
#define SCB_ICSR_PENDSVCLR_MASK  0x8000000u
#define SCB_ICSR_PENDSVCLR_SHIFT 27u
#define SCB_ICSR_PENDSVCLR_WIDTH 1u
#define SCB_ICSR_PENDSVCLR(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_ICSR_PENDSVCLR_SHIFT)) & SCB_ICSR_PENDSVCLR_MASK)
#define SCB_ICSR_PENDSVSET_MASK  0x10000000u
#define SCB_ICSR_PENDSVSET_SHIFT 28u
#define SCB_ICSR_PENDSVSET_WIDTH 1u
#define SCB_ICSR_PENDSVSET(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_ICSR_PENDSVSET_SHIFT)) & SCB_ICSR_PENDSVSET_MASK)
#define SCB_ICSR_NMIPENDSET_MASK  0x80000000u
#define SCB_ICSR_NMIPENDSET_SHIFT 31u
#define SCB_ICSR_NMIPENDSET_WIDTH 1u
#define SCB_ICSR_NMIPENDSET(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_ICSR_NMIPENDSET_SHIFT)) & SCB_ICSR_NMIPENDSET_MASK)
/* VTOR Bit Fields */
/* AIRCR Bit Fields */
#define SCB_AIRCR_VECTCLRACTIVE_MASK  0x2u
#define SCB_AIRCR_VECTCLRACTIVE_SHIFT 1u
#define SCB_AIRCR_VECTCLRACTIVE_WIDTH 1u
#define SCB_AIRCR_VECTCLRACTIVE(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_AIRCR_VECTCLRACTIVE_SHIFT)) & SCB_AIRCR_VECTCLRACTIVE_MASK)
#define SCB_AIRCR_SYSRESETREQ_MASK  0x4u
#define SCB_AIRCR_SYSRESETREQ_SHIFT 2u
#define SCB_AIRCR_SYSRESETREQ_WIDTH 1u
#define SCB_AIRCR_SYSRESETREQ(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_AIRCR_SYSRESETREQ_SHIFT)) & SCB_AIRCR_SYSRESETREQ_MASK)
#define SCB_AIRCR_ENDIANNESS_MASK  0x8000u
#define SCB_AIRCR_ENDIANNESS_SHIFT 15u
#define SCB_AIRCR_ENDIANNESS_WIDTH 1u
#define SCB_AIRCR_ENDIANNESS(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_AIRCR_ENDIANNESS_SHIFT)) & SCB_AIRCR_ENDIANNESS_MASK)
#define SCB_AIRCR_VECTKEY_MASK  0xFFFF0000u
#define SCB_AIRCR_VECTKEY_SHIFT 16u
#define SCB_AIRCR_VECTKEY_WIDTH 16u
#define SCB_AIRCR_VECTKEY(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_AIRCR_VECTKEY_SHIFT)) & SCB_AIRCR_VECTKEY_MASK)
/* SCR Bit Fields */
#define SCB_SCR_SLEEPONEXIT_MASK  0x2u
#define SCB_SCR_SLEEPONEXIT_SHIFT 1u
#define SCB_SCR_SLEEPONEXIT_WIDTH 1u
#define SCB_SCR_SLEEPONEXIT(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_SCR_SLEEPONEXIT_SHIFT)) & SCB_SCR_SLEEPONEXIT_MASK)
#define SCB_SCR_SLEEPDEEP_MASK  0x4u
#define SCB_SCR_SLEEPDEEP_SHIFT 2u
#define SCB_SCR_SLEEPDEEP_WIDTH 1u
#define SCB_SCR_SLEEPDEEP(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_SCR_SLEEPDEEP_SHIFT)) & SCB_SCR_SLEEPDEEP_MASK)
#define SCB_SCR_SEVONPEND_MASK  0x10u
#define SCB_SCR_SEVONPEND_SHIFT 4u
#define SCB_SCR_SEVONPEND_WIDTH 1u
#define SCB_SCR_SEVONPEND(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_SCR_SEVONPEND_SHIFT)) & SCB_SCR_SEVONPEND_MASK)
/* CCR Bit Fields */
#define SCB_CCR_UNALIGN_TRP_MASK  0x8u
#define SCB_CCR_UNALIGN_TRP_SHIFT 3u
#define SCB_CCR_UNALIGN_TRP_WIDTH 1u
#define SCB_CCR_UNALIGN_TRP(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_CCR_UNALIGN_TRP_SHIFT)) & SCB_CCR_UNALIGN_TRP_MASK)
#define SCB_CCR_STKALIGN_MASK  0x200u
#define SCB_CCR_STKALIGN_SHIFT 9u
#define SCB_CCR_STKALIGN_WIDTH 1u
#define SCB_CCR_STKALIGN(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_CCR_STKALIGN_SHIFT)) & SCB_CCR_STKALIGN_MASK)
/* SHPR2 Bit Fields */
#define SCB_SHPR2_PRI_11_MASK  0xFF000000u
#define SCB_SHPR2_PRI_11_SHIFT 24u
#define SCB_SHPR2_PRI_11_WIDTH 8u
#define SCB_SHPR2_PRI_11(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_SHPR2_PRI_11_SHIFT)) & SCB_SHPR2_PRI_11_MASK)
/* SHPR3 Bit Fields */
#define SCB_SHPR3_PRI_14_MASK  0xFF0000u
#define SCB_SHPR3_PRI_14_SHIFT 16u
#define SCB_SHPR3_PRI_14_WIDTH 8u
#define SCB_SHPR3_PRI_14(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_SHPR3_PRI_14_SHIFT)) & SCB_SHPR3_PRI_14_MASK)
#define SCB_SHPR3_PRI_15_MASK  0xFF000000u
#define SCB_SHPR3_PRI_15_SHIFT 24u
#define SCB_SHPR3_PRI_15_WIDTH 8u
#define SCB_SHPR3_PRI_15(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_SHPR3_PRI_15_SHIFT)) & SCB_SHPR3_PRI_15_MASK)
/* SHCSR Bit Fields */
#define SCB_SHCSR_SVCALLPENDED_MASK  0x8000u
#define SCB_SHCSR_SVCALLPENDED_SHIFT 15u
#define SCB_SHCSR_SVCALLPENDED_WIDTH 1u
#define SCB_SHCSR_SVCALLPENDED(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_SHCSR_SVCALLPENDED_SHIFT)) & SCB_SHCSR_SVCALLPENDED_MASK)
/* DFSR Bit Fields */
#define SCB_DFSR_HALTED_MASK  0x1u
#define SCB_DFSR_HALTED_SHIFT 0u
#define SCB_DFSR_HALTED_WIDTH 1u
#define SCB_DFSR_HALTED(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_DFSR_HALTED_SHIFT)) & SCB_DFSR_HALTED_MASK)
#define SCB_DFSR_BKPT_MASK     0x2u
#define SCB_DFSR_BKPT_SHIFT    1u
#define SCB_DFSR_BKPT_WIDTH    1u
#define SCB_DFSR_BKPT(x)       (((uint32_t)(((uint32_t)(x)) << SCB_DFSR_BKPT_SHIFT)) & SCB_DFSR_BKPT_MASK)
#define SCB_DFSR_DWTTRAP_MASK  0x4u
#define SCB_DFSR_DWTTRAP_SHIFT 2u
#define SCB_DFSR_DWTTRAP_WIDTH 1u
#define SCB_DFSR_DWTTRAP(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_DFSR_DWTTRAP_SHIFT)) & SCB_DFSR_DWTTRAP_MASK)
#define SCB_DFSR_VCATCH_MASK  0x8u
#define SCB_DFSR_VCATCH_SHIFT 3u
#define SCB_DFSR_VCATCH_WIDTH 1u
#define SCB_DFSR_VCATCH(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_DFSR_VCATCH_SHIFT)) & SCB_DFSR_VCATCH_MASK)
#define SCB_DFSR_EXTERNAL_MASK  0x10u
#define SCB_DFSR_EXTERNAL_SHIFT 4u
#define SCB_DFSR_EXTERNAL_WIDTH 1u
#define SCB_DFSR_EXTERNAL(x) \
    (((uint32_t)(((uint32_t)(x)) << SCB_DFSR_EXTERNAL_SHIFT)) & SCB_DFSR_EXTERNAL_MASK)

/**
 * @}
 */ /* end of group SCB_Register_Masks */

/**
 * @}
 */ /* end of group SCB_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------
   -- SysTick Peripheral Access Layer
   ---------------------------------------------------------------------- */

/**
 * @addtogroup SysTick_Peripheral_Access_Layer SysTick Peripheral
 * Access Layer
 * @{
 */

/** SysTick - Size of Registers Arrays */

/** SysTick - Register Layout Typedef */
typedef struct
{
    _IO uint32_t CSR;   /**< offset: 0x0 */
    _IO uint32_t RVR;   /**< offset: 0x4 */
    _IO uint32_t CVR;   /**< offset: 0x8 */
    __I uint32_t CALIB; /**< offset: 0xC */
} SysTick_t, *SysTick_MemMapPtr;

/** Number of instances of the SysTick module. */
#define SysTick_INSTANCE_COUNT (1u)

/* SysTick - Peripheral instance base addresses */
/** Peripheral SysTick base address */
#define SysTick_BASE (0xE000E010u)
/** Peripheral SysTick base pointer */
#define SysTick ((SysTick_t *)SysTick_BASE)
/** Array initializer of SysTick peripheral base addresses */
#define SysTick_BASE_ADDRS \
    {                      \
        SysTick_BASE       \
    }
/** Array initializer of SysTick peripheral base pointers */
#define SysTick_BASE_PTRS \
    {                     \
        SysTick           \
    }
/** Number of interrupt vector arrays for the SysTick module. */
#define SysTick_IRQS_ARR_COUNT (1u)
/** Number of interrupt channels for the SysTick module. */
#define SysTick_IRQS_CH_COUNT (1u)
/** Interrupt vectors for the SysTick peripheral type */
#define SysTick_IRQS \
    {                \
        SysTick_IRQn \
    }

/* --------------------------------------------------------------------------
   -- SysTick Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup SysTick_Register_Masks SysTick Register Masks
 * @{
 */

/* CSR Bit Fields */
#define SysTick_CSR_ENABLE_MASK  0x1u
#define SysTick_CSR_ENABLE_SHIFT 0u
#define SysTick_CSR_ENABLE_WIDTH 1u
#define SysTick_CSR_ENABLE(x) \
    (((uint32_t)(((uint32_t)(x)) << SysTick_CSR_ENABLE_SHIFT)) & SysTick_CSR_ENABLE_MASK)
#define SysTick_CSR_TICKINT_MASK  0x2u
#define SysTick_CSR_TICKINT_SHIFT 1u
#define SysTick_CSR_TICKINT_WIDTH 1u
#define SysTick_CSR_TICKINT(x) \
    (((uint32_t)(((uint32_t)(x)) << SysTick_CSR_TICKINT_SHIFT)) & SysTick_CSR_TICKINT_MASK)
#define SysTick_CSR_CLKSOURCE_MASK  0x4u
#define SysTick_CSR_CLKSOURCE_SHIFT 2u
#define SysTick_CSR_CLKSOURCE_WIDTH 1u
#define SysTick_CSR_CLKSOURCE(x) \
    (((uint32_t)(((uint32_t)(x)) << SysTick_CSR_CLKSOURCE_SHIFT)) & SysTick_CSR_CLKSOURCE_MASK)
#define SysTick_CSR_COUNTFLAG_MASK  0x10000u
#define SysTick_CSR_COUNTFLAG_SHIFT 16u
#define SysTick_CSR_COUNTFLAG_WIDTH 1u
#define SysTick_CSR_COUNTFLAG(x) \
    (((uint32_t)(((uint32_t)(x)) << SysTick_CSR_COUNTFLAG_SHIFT)) & SysTick_CSR_COUNTFLAG_MASK)
/* RVR Bit Fields */
#define SysTick_RVR_RELOAD_MASK  0xFFFFFFu
#define SysTick_RVR_RELOAD_SHIFT 0u
#define SysTick_RVR_RELOAD_WIDTH 24u
#define SysTick_RVR_RELOAD(x) \
    (((uint32_t)(((uint32_t)(x)) << SysTick_RVR_RELOAD_SHIFT)) & SysTick_RVR_RELOAD_MASK)
/* CVR Bit Fields */
#define SysTick_CVR_CURRENT_MASK  0xFFFFFFu
#define SysTick_CVR_CURRENT_SHIFT 0u
#define SysTick_CVR_CURRENT_WIDTH 24u
#define SysTick_CVR_CURRENT(x) \
    (((uint32_t)(((uint32_t)(x)) << SysTick_CVR_CURRENT_SHIFT)) & SysTick_CVR_CURRENT_MASK)
/* CALIB Bit Fields */
#define SysTick_CALIB_TENMS_MASK  0xFFFFFFu
#define SysTick_CALIB_TENMS_SHIFT 0u
#define SysTick_CALIB_TENMS_WIDTH 24u
#define SysTick_CALIB_TENMS(x) \
    (((uint32_t)(((uint32_t)(x)) << SysTick_CALIB_TENMS_SHIFT)) & SysTick_CALIB_TENMS_MASK)
#define SysTick_CALIB_SKEW_MASK  0x40000000u
#define SysTick_CALIB_SKEW_SHIFT 30u
#define SysTick_CALIB_SKEW_WIDTH 1u
#define SysTick_CALIB_SKEW(x) \
    (((uint32_t)(((uint32_t)(x)) << SysTick_CALIB_SKEW_SHIFT)) & SysTick_CALIB_SKEW_MASK)
#define SysTick_CALIB_NOREF_MASK  0x80000000u
#define SysTick_CALIB_NOREF_SHIFT 31u
#define SysTick_CALIB_NOREF_WIDTH 1u
#define SysTick_CALIB_NOREF(x) \
    (((uint32_t)(((uint32_t)(x)) << SysTick_CALIB_NOREF_SHIFT)) & SysTick_CALIB_NOREF_MASK)

/**
 * @}
 */ /* end of group SysTick_Register_Masks */

/**
 * @}
 */ /* end of group SysTick_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------
   -- SCG Peripheral Access Layer
   ---------------------------------------------------------------------- */

/**
 * @addtogroup SCG_Peripheral_Access_Layer SCG Peripheral Access Layer
 * @{
 */

/** SCG - Size of Registers Arrays */

/** SCG - Register Layout Typedef */
typedef struct
{
    _NA uint8_t  RESERVED_0[8];
    _IO uint32_t OSCHW;      /**< offset: 0x8 */
    _IO uint32_t RESETHW;    /**< offset: 0xC */
    __I uint32_t CSR;        /**< offset: 0x10 */
    _IO uint32_t RCCR;       /**< offset: 0x14 */
    _IO uint32_t VCCR;       /**< offset: 0x18 */
    _IO uint32_t HCCR;       /**< offset: 0x1C */
    _IO uint32_t CLKOUTCNFG; /**< offset: 0x20 */
    _NA uint8_t  RESERVED_1[220];
    _IO uint32_t SOSCCSR;    /**< offset: 0x100 */
    _IO uint32_t SOSCDIV;    /**< offset: 0x104 */
    _IO uint32_t SOSCCFG;    /**< offset: 0x108 */
    _NA uint8_t  RESERVED_2[244];
    _IO uint32_t SIRCCSR;    /**< offset: 0x200 */
    _IO uint32_t SIRCDIV;    /**< offset: 0x204 */
    _IO uint32_t SIRCCFG;    /**< offset: 0x208 */
    _NA uint8_t  RESERVED_3[244];
    _IO uint32_t FIRCCSR;    /**< offset: 0x300 */
    _IO uint32_t FIRCDIV;    /**< offset: 0x304 */
    _IO uint32_t FIRCCFG;    /**< offset: 0x308 */
} SCG_t, *SCG_MemMapPtr;

/** Number of instances of the SCG module. */
#define SCG_INSTANCE_COUNT (1u)

/* SCG - Peripheral instance base addresses */
/** Peripheral SCG base address */
#define SCG_BASE (0x40064000u)
/** Peripheral SCG base pointer */
#define SCG ((SCG_t *)SCG_BASE)
/** Array initializer of SCG peripheral base addresses */
#define SCG_BASE_ADDRS \
    {                  \
        SCG_BASE       \
    }
/** Array initializer of SCG peripheral base pointers */
#define SCG_BASE_PTRS \
    {                 \
        SCG           \
    }

/* --------------------------------------------------------------------------
   -- SCG Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup SCG_Register_Masks SCG Register Masks
 * @{
 */

/* VERID Bit Fields */
/* PARAM Bit Fields */
/* CSR Bit Fields */
#define SCG_CSR_DIVSLOW_MASK  0xFu
#define SCG_CSR_DIVSLOW_SHIFT 0u
#define SCG_CSR_DIVSLOW_WIDTH 4u
#define SCG_CSR_DIVSLOW(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_CSR_DIVSLOW_SHIFT)) & SCG_CSR_DIVSLOW_MASK)
#define SCG_CSR_DIVCORE_MASK  0xF0000u
#define SCG_CSR_DIVCORE_SHIFT 16u
#define SCG_CSR_DIVCORE_WIDTH 4u
#define SCG_CSR_DIVCORE(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_CSR_DIVCORE_SHIFT)) & SCG_CSR_DIVCORE_MASK)
#define SCG_CSR_SCS_MASK  0xF000000u
#define SCG_CSR_SCS_SHIFT 24u
#define SCG_CSR_SCS_WIDTH 4u
#define SCG_CSR_SCS(x)    (((uint32_t)(((uint32_t)(x)) << SCG_CSR_SCS_SHIFT)) & SCG_CSR_SCS_MASK)
/* RCCR Bit Fields */
#define SCG_RCCR_DIVSLOW_MASK  0xFu
#define SCG_RCCR_DIVSLOW_SHIFT 0u
#define SCG_RCCR_DIVSLOW_WIDTH 4u
#define SCG_RCCR_DIVSLOW(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_RCCR_DIVSLOW_SHIFT)) & SCG_RCCR_DIVSLOW_MASK)
#define SCG_RCCR_DIVCORE_MASK  0xF0000u
#define SCG_RCCR_DIVCORE_SHIFT 16u
#define SCG_RCCR_DIVCORE_WIDTH 4u
#define SCG_RCCR_DIVCORE(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_RCCR_DIVCORE_SHIFT)) & SCG_RCCR_DIVCORE_MASK)
#define SCG_RCCR_SCS_MASK  0xF000000u
#define SCG_RCCR_SCS_SHIFT 24u
#define SCG_RCCR_SCS_WIDTH 4u
#define SCG_RCCR_SCS(x)    (((uint32_t)(((uint32_t)(x)) << SCG_RCCR_SCS_SHIFT)) & SCG_RCCR_SCS_MASK)
/* VCCR Bit Fields */
#define SCG_VCCR_DIVSLOW_MASK  0xFu
#define SCG_VCCR_DIVSLOW_SHIFT 0u
#define SCG_VCCR_DIVSLOW_WIDTH 4u
#define SCG_VCCR_DIVSLOW(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_VCCR_DIVSLOW_SHIFT)) & SCG_VCCR_DIVSLOW_MASK)
#define SCG_VCCR_DIVCORE_MASK  0xF0000u
#define SCG_VCCR_DIVCORE_SHIFT 16u
#define SCG_VCCR_DIVCORE_WIDTH 4u
#define SCG_VCCR_DIVCORE(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_VCCR_DIVCORE_SHIFT)) & SCG_VCCR_DIVCORE_MASK)
#define SCG_VCCR_SCS_MASK  0xF000000u
#define SCG_VCCR_SCS_SHIFT 24u
#define SCG_VCCR_SCS_WIDTH 4u
#define SCG_VCCR_SCS(x)    (((uint32_t)(((uint32_t)(x)) << SCG_VCCR_SCS_SHIFT)) & SCG_VCCR_SCS_MASK)
/* HCCR Bit Fields */
#define SCG_HCCR_DIVSLOW_MASK  0xFu
#define SCG_HCCR_DIVSLOW_SHIFT 0u
#define SCG_HCCR_DIVSLOW_WIDTH 4u
#define SCG_HCCR_DIVSLOW(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_HCCR_DIVSLOW_SHIFT)) & SCG_HCCR_DIVSLOW_MASK)
#define SCG_HCCR_DIVCORE_MASK  0xF0000u
#define SCG_HCCR_DIVCORE_SHIFT 16u
#define SCG_HCCR_DIVCORE_WIDTH 4u
#define SCG_HCCR_DIVCORE(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_HCCR_DIVCORE_SHIFT)) & SCG_HCCR_DIVCORE_MASK)
#define SCG_HCCR_SCS_MASK  0xF000000u
#define SCG_HCCR_SCS_SHIFT 24u
#define SCG_HCCR_SCS_WIDTH 4u
#define SCG_HCCR_SCS(x)    (((uint32_t)(((uint32_t)(x)) << SCG_HCCR_SCS_SHIFT)) & SCG_HCCR_SCS_MASK)
/* CLKOUTCNFG Bit Fields */
#define SCG_CLKOUTCNFG_CLKOUTSEL_MASK  0xF000000u
#define SCG_CLKOUTCNFG_CLKOUTSEL_SHIFT 24u
#define SCG_CLKOUTCNFG_CLKOUTSEL_WIDTH 4u
#define SCG_CLKOUTCNFG_CLKOUTSEL(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << SCG_CLKOUTCNFG_CLKOUTSEL_SHIFT)) & \
     SCG_CLKOUTCNFG_CLKOUTSEL_MASK)
/* SOSCCSR Bit Fields */
/* SOSCDIV Bit Fields */
#define SCG_SOSCDIV_SOSCDIV1_MASK  0x7u
#define SCG_SOSCDIV_SOSCDIV1_SHIFT 0u
#define SCG_SOSCDIV_SOSCDIV1_WIDTH 3u
#define SCG_SOSCDIV_SOSCDIV1(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_SOSCDIV_SOSCDIV1_SHIFT)) & SCG_SOSCDIV_SOSCDIV1_MASK)
#define SCG_SOSCDIV_SOSCDIV2_MASK  0x700u
#define SCG_SOSCDIV_SOSCDIV2_SHIFT 8u
#define SCG_SOSCDIV_SOSCDIV2_WIDTH 3u
#define SCG_SOSCDIV_SOSCDIV2(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_SOSCDIV_SOSCDIV2_SHIFT)) & SCG_SOSCDIV_SOSCDIV2_MASK)
/* SOSCCFG Bit Fields */
/* SIRCCSR Bit Fields */
/* SIRCDIV Bit Fields */
#define SCG_SIRCDIV_SIRCDIV1_MASK  0x7u
#define SCG_SIRCDIV_SIRCDIV1_SHIFT 0u
#define SCG_SIRCDIV_SIRCDIV1_WIDTH 3u
#define SCG_SIRCDIV_SIRCDIV1(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_SIRCDIV_SIRCDIV1_SHIFT)) & SCG_SIRCDIV_SIRCDIV1_MASK)
#define SCG_SIRCDIV_SIRCDIV2_MASK  0x700u
#define SCG_SIRCDIV_SIRCDIV2_SHIFT 8u
#define SCG_SIRCDIV_SIRCDIV2_WIDTH 3u
#define SCG_SIRCDIV_SIRCDIV2(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_SIRCDIV_SIRCDIV2_SHIFT)) & SCG_SIRCDIV_SIRCDIV2_MASK)
/* SIRCCFG Bit Fields */
/* FIRCCSR Bit Fields */
/* FIRCDIV Bit Fields */
#define SCG_FIRCDIV_FIRCDIV1_MASK  0x7u
#define SCG_FIRCDIV_FIRCDIV1_SHIFT 0u
#define SCG_FIRCDIV_FIRCDIV1_WIDTH 3u
#define SCG_FIRCDIV_FIRCDIV1(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_FIRCDIV_FIRCDIV1_SHIFT)) & SCG_FIRCDIV_FIRCDIV1_MASK)
#define SCG_FIRCDIV_FIRCDIV2_MASK  0x700u
#define SCG_FIRCDIV_FIRCDIV2_SHIFT 8u
#define SCG_FIRCDIV_FIRCDIV2_WIDTH 3u
#define SCG_FIRCDIV_FIRCDIV2(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_FIRCDIV_FIRCDIV2_SHIFT)) & SCG_FIRCDIV_FIRCDIV2_MASK)
/* FIRCCFG Bit Fields */
/* SPLLDIV Bit Fields */
#define SCG_SPLLDIV_SPLLDIV1_MASK  0x7u
#define SCG_SPLLDIV_SPLLDIV1_SHIFT 0u
#define SCG_SPLLDIV_SPLLDIV1_WIDTH 3u
#define SCG_SPLLDIV_SPLLDIV1(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_SPLLDIV_SPLLDIV1_SHIFT)) & SCG_SPLLDIV_SPLLDIV1_MASK)
#define SCG_SPLLDIV_SPLLDIV2_MASK  0x700u
#define SCG_SPLLDIV_SPLLDIV2_SHIFT 8u
#define SCG_SPLLDIV_SPLLDIV2_WIDTH 3u
#define SCG_SPLLDIV_SPLLDIV2(x) \
    (((uint32_t)(((uint32_t)(x)) << SCG_SPLLDIV_SPLLDIV2_SHIFT)) & SCG_SPLLDIV_SPLLDIV2_MASK)

/**
 * @}
 */ /* end of group SCG_Register_Masks */

/**
 * @}
 */ /* end of group SCG_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------
   -- SIM Peripheral Access Layer
   ---------------------------------------------------------------------- */

/**
 * @addtogroup SIM_Peripheral_Access_Layer SIM Peripheral Access Layer
 * @{
 */

/** SIM - Size of Registers Arrays */

/** SIM - Register Layout Typedef */
typedef struct
{
    _NA uint8_t  RESERVED_0[4];
    _IO uint32_t CHIPCTL;   /**< offset: 0x4 */
    _NA uint8_t  RESERVED_1[4];
    _IO uint32_t FTMOPT0;   /**< offset: 0xC */
    _IO uint32_t LPOCLKS;   /**< offset: 0x10 */
    _NA uint8_t  RESERVED_2[16];
    __I uint32_t SDID;      /**< offset: 0x24 */
    _NA uint8_t  RESERVED_3[72];
    __I uint32_t HWID;      /**< offset: 0x70 */
    _IO uint32_t FPREG;     /**< offset: 0x74 */
    _NA uint8_t  RESERVED_4[8];
    _IO uint32_t LDOCTR;    /**< offset: 0x80 */
    _IO uint32_t HMCMR;     /**< offset: 0x84 */
    _IO uint32_t OSCCTR;    /**< offset: 0x88 */
    _IO uint32_t USBHMCR;   /**< offset: 0x8C */
    _IO uint32_t RAM0HMCR0; /**< offset: 0x90 */
    _IO uint32_t RAM0HMCR1; /**< offset: 0x94 */
} SIM_t, *SIM_MemMapPtr;

/** Number of instances of the SIM module. */
#define SIM_INSTANCE_COUNT (1u)

/* SIM - Peripheral instance base addresses */
/** Peripheral SIM base address */
#define SIM_BASE (0x40048000u)
/** Peripheral SIM base pointer */
#define SIM ((SIM_t *)SIM_BASE)
/** Array initializer of SIM peripheral base addresses */
#define SIM_BASE_ADDRS \
    {                  \
        SIM_BASE       \
    }
/** Array initializer of SIM peripheral base pointers */
#define SIM_BASE_PTRS \
    {                 \
        SIM           \
    }

/* --------------------------------------------------------------------------
   -- SIM Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup SIM_Register_Masks SIM Register Masks
 * @{
 */

/* CHIPCTL Bit Fields */
#define SIM_CHIPCTL_CLKOUTSEL_MASK  0xF0u
#define SIM_CHIPCTL_CLKOUTSEL_SHIFT 4u
#define SIM_CHIPCTL_CLKOUTSEL_WIDTH 4u
#define SIM_CHIPCTL_CLKOUTSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_CHIPCTL_CLKOUTSEL_SHIFT)) & SIM_CHIPCTL_CLKOUTSEL_MASK)
#define SIM_CHIPCTL_CLKOUTDIV_MASK  0x700u
#define SIM_CHIPCTL_CLKOUTDIV_SHIFT 8u
#define SIM_CHIPCTL_CLKOUTDIV_WIDTH 3u
#define SIM_CHIPCTL_CLKOUTDIV(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_CHIPCTL_CLKOUTDIV_SHIFT)) & SIM_CHIPCTL_CLKOUTDIV_MASK)
#define SIM_CHIPCTL_CLKOUTEN_MASK  0x800u
#define SIM_CHIPCTL_CLKOUTEN_SHIFT 11u
#define SIM_CHIPCTL_CLKOUTEN_WIDTH 1u
#define SIM_CHIPCTL_CLKOUTEN(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_CHIPCTL_CLKOUTEN_SHIFT)) & SIM_CHIPCTL_CLKOUTEN_MASK)
/* FTMOPT0 Bit Fields */
#define SIM_FTMOPT0_FTM0FLTxSEL_MASK  0x7u
#define SIM_FTMOPT0_FTM0FLTxSEL_SHIFT 0u
#define SIM_FTMOPT0_FTM0FLTxSEL_WIDTH 3u
#define SIM_FTMOPT0_FTM0FLTxSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_FTMOPT0_FTM0FLTxSEL_SHIFT)) & SIM_FTMOPT0_FTM0FLTxSEL_MASK)
#define SIM_FTMOPT0_FTM1FLTxSEL_MASK  0x70u
#define SIM_FTMOPT0_FTM1FLTxSEL_SHIFT 4u
#define SIM_FTMOPT0_FTM1FLTxSEL_WIDTH 3u
#define SIM_FTMOPT0_FTM1FLTxSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_FTMOPT0_FTM1FLTxSEL_SHIFT)) & SIM_FTMOPT0_FTM1FLTxSEL_MASK)
#define SIM_FTMOPT0_FTM2FLTxSEL_MASK  0x700u
#define SIM_FTMOPT0_FTM2FLTxSEL_SHIFT 8u
#define SIM_FTMOPT0_FTM2FLTxSEL_WIDTH 3u
#define SIM_FTMOPT0_FTM2FLTxSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_FTMOPT0_FTM2FLTxSEL_SHIFT)) & SIM_FTMOPT0_FTM2FLTxSEL_MASK)
#define SIM_FTMOPT0_FTM3FLTxSEL_MASK  0x7000u
#define SIM_FTMOPT0_FTM3FLTxSEL_SHIFT 12u
#define SIM_FTMOPT0_FTM3FLTxSEL_WIDTH 3u
#define SIM_FTMOPT0_FTM3FLTxSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_FTMOPT0_FTM3FLTxSEL_SHIFT)) & SIM_FTMOPT0_FTM3FLTxSEL_MASK)
#define SIM_FTMOPT0_FTM0CLKSEL_MASK  0x3000000u
#define SIM_FTMOPT0_FTM0CLKSEL_SHIFT 24u
#define SIM_FTMOPT0_FTM0CLKSEL_WIDTH 2u
#define SIM_FTMOPT0_FTM0CLKSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_FTMOPT0_FTM0CLKSEL_SHIFT)) & SIM_FTMOPT0_FTM0CLKSEL_MASK)
#define SIM_FTMOPT0_FTM1CLKSEL_MASK  0xC000000u
#define SIM_FTMOPT0_FTM1CLKSEL_SHIFT 26u
#define SIM_FTMOPT0_FTM1CLKSEL_WIDTH 2u
#define SIM_FTMOPT0_FTM1CLKSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_FTMOPT0_FTM1CLKSEL_SHIFT)) & SIM_FTMOPT0_FTM1CLKSEL_MASK)
#define SIM_FTMOPT0_FTM2CLKSEL_MASK  0x30000000u
#define SIM_FTMOPT0_FTM2CLKSEL_SHIFT 28u
#define SIM_FTMOPT0_FTM2CLKSEL_WIDTH 2u
#define SIM_FTMOPT0_FTM2CLKSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_FTMOPT0_FTM2CLKSEL_SHIFT)) & SIM_FTMOPT0_FTM2CLKSEL_MASK)
#define SIM_FTMOPT0_FTM3CLKSEL_MASK  0xC0000000u
#define SIM_FTMOPT0_FTM3CLKSEL_SHIFT 30u
#define SIM_FTMOPT0_FTM3CLKSEL_WIDTH 2u
#define SIM_FTMOPT0_FTM3CLKSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_FTMOPT0_FTM3CLKSEL_SHIFT)) & SIM_FTMOPT0_FTM3CLKSEL_MASK)
/* LPOCLKS Bit Fields */
#define SIM_LPOCLKS_LPO1KCLKEN_MASK  0x1u
#define SIM_LPOCLKS_LPO1KCLKEN_SHIFT 0u
#define SIM_LPOCLKS_LPO1KCLKEN_WIDTH 1u
#define SIM_LPOCLKS_LPO1KCLKEN(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_LPOCLKS_LPO1KCLKEN_SHIFT)) & SIM_LPOCLKS_LPO1KCLKEN_MASK)
#define SIM_LPOCLKS_LPO32KCLKEN_MASK  0x2u
#define SIM_LPOCLKS_LPO32KCLKEN_SHIFT 1u
#define SIM_LPOCLKS_LPO32KCLKEN_WIDTH 1u
#define SIM_LPOCLKS_LPO32KCLKEN(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_LPOCLKS_LPO32KCLKEN_SHIFT)) & SIM_LPOCLKS_LPO32KCLKEN_MASK)
#define SIM_LPOCLKS_LPOCLKSEL_MASK  0xCu
#define SIM_LPOCLKS_LPOCLKSEL_SHIFT 2u
#define SIM_LPOCLKS_LPOCLKSEL_WIDTH 2u
#define SIM_LPOCLKS_LPOCLKSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_LPOCLKS_LPOCLKSEL_SHIFT)) & SIM_LPOCLKS_LPOCLKSEL_MASK)
#define SIM_LPOCLKS_RTCCLKSEL_MASK  0x30u
#define SIM_LPOCLKS_RTCCLKSEL_SHIFT 4u
#define SIM_LPOCLKS_RTCCLKSEL_WIDTH 2u
#define SIM_LPOCLKS_RTCCLKSEL(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_LPOCLKS_RTCCLKSEL_SHIFT)) & SIM_LPOCLKS_RTCCLKSEL_MASK)
/* SDID Bit Fields */
#define SIM_SDID_FEATURES_MASK  0xFFu
#define SIM_SDID_FEATURES_SHIFT 0u
#define SIM_SDID_FEATURES_WIDTH 8u
#define SIM_SDID_FEATURES(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_SDID_FEATURES_SHIFT)) & SIM_SDID_FEATURES_MASK)
#define SIM_SDID_PACKAGE_MASK  0xF00u
#define SIM_SDID_PACKAGE_SHIFT 8u
#define SIM_SDID_PACKAGE_WIDTH 4u
#define SIM_SDID_PACKAGE(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_SDID_PACKAGE_SHIFT)) & SIM_SDID_PACKAGE_MASK)
#define SIM_SDID_REVID_MASK  0xF000u
#define SIM_SDID_REVID_SHIFT 12u
#define SIM_SDID_REVID_WIDTH 4u
#define SIM_SDID_REVID(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_SDID_REVID_SHIFT)) & SIM_SDID_REVID_MASK)
#define SIM_SDID_RAMSIZE_MASK  0xF0000u
#define SIM_SDID_RAMSIZE_SHIFT 16u
#define SIM_SDID_RAMSIZE_WIDTH 4u
#define SIM_SDID_RAMSIZE(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_SDID_RAMSIZE_SHIFT)) & SIM_SDID_RAMSIZE_MASK)
#define SIM_SDID_DERIVATE_MASK  0xF00000u
#define SIM_SDID_DERIVATE_SHIFT 20u
#define SIM_SDID_DERIVATE_WIDTH 4u
#define SIM_SDID_DERIVATE(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_SDID_DERIVATE_SHIFT)) & SIM_SDID_DERIVATE_MASK)
#define SIM_SDID_SUBSERIES_MASK  0xF000000u
#define SIM_SDID_SUBSERIES_SHIFT 24u
#define SIM_SDID_SUBSERIES_WIDTH 4u
#define SIM_SDID_SUBSERIES(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_SDID_SUBSERIES_SHIFT)) & SIM_SDID_SUBSERIES_MASK)
#define SIM_SDID_GENERATION_MASK  0xF0000000u
#define SIM_SDID_GENERATION_SHIFT 28u
#define SIM_SDID_GENERATION_WIDTH 4u
#define SIM_SDID_GENERATION(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_SDID_GENERATION_SHIFT)) & SIM_SDID_GENERATION_MASK)
/* HWID Bit Fields */
#define SIM_HWID_SERNUM_MASK  0xFFu
#define SIM_HWID_SERNUM_SHIFT 0u
#define SIM_HWID_SERNUM_WIDTH 8u
#define SIM_HWID_SERNUM(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_HWID_SERNUM_SHIFT)) & SIM_HWID_SERNUM_MASK)
#define SIM_HWID_RTLVID_MASK  0xFF00u
#define SIM_HWID_RTLVID_SHIFT 8u
#define SIM_HWID_RTLVID_WIDTH 8u
#define SIM_HWID_RTLVID(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_HWID_RTLVID_SHIFT)) & SIM_HWID_RTLVID_MASK)
#define SIM_HWID_BCID_MASK    0xF0000u
#define SIM_HWID_BCID_SHIFT   16u
#define SIM_HWID_BCID_WIDTH   4u
#define SIM_HWID_BCID(x)      (((uint32_t)(((uint32_t)(x)) << SIM_HWID_BCID_SHIFT)) & SIM_HWID_BCID_MASK)
#define SIM_HWID_FSHCID_MASK  0xF00000u
#define SIM_HWID_FSHCID_SHIFT 20u
#define SIM_HWID_FSHCID_WIDTH 4u
#define SIM_HWID_FSHCID(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_HWID_FSHCID_SHIFT)) & SIM_HWID_FSHCID_MASK)
#define SIM_HWID_HWVID_MASK  0xF000000u
#define SIM_HWID_HWVID_SHIFT 24u
#define SIM_HWID_HWVID_WIDTH 4u
#define SIM_HWID_HWVID(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_HWID_HWVID_SHIFT)) & SIM_HWID_HWVID_MASK)
#define SIM_HWID_FWVID_MASK  0xF0000000u
#define SIM_HWID_FWVID_SHIFT 28u
#define SIM_HWID_FWVID_WIDTH 4u
#define SIM_HWID_FWVID(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_HWID_FWVID_SHIFT)) & SIM_HWID_FWVID_MASK)
/* FPREG Bit Fields */
#define SIM_FPREG_FPREG_MASK  0xFFFFu
#define SIM_FPREG_FPREG_SHIFT 0u
#define SIM_FPREG_FPREG_WIDTH 16u
#define SIM_FPREG_FPREG(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_FPREG_FPREG_SHIFT)) & SIM_FPREG_FPREG_MASK)
/* LDOCTR Bit Fields */
#define SIM_LDOCTR_OLCPD_MASK  0x4000000u
#define SIM_LDOCTR_OLCPD_SHIFT 26u
#define SIM_LDOCTR_OLCPD_WIDTH 1u
#define SIM_LDOCTR_OLCPD(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_LDOCTR_OLCPD_SHIFT)) & SIM_LDOCTR_OLCPD_MASK)
#define SIM_LDOCTR_CSET_MASK  0x3000000u
#define SIM_LDOCTR_CSET_SHIFT 24u
#define SIM_LDOCTR_CSET_WIDTH 2u
#define SIM_LDOCTR_CSET(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_LDOCTR_CSET_SHIFT)) & SIM_LDOCTR_CSET_MASK)
#define SIM_LDOCTR_PLL_PD_MASK  0x100000u
#define SIM_LDOCTR_PLL_PD_SHIFT 20u
#define SIM_LDOCTR_PLL_PD_WIDTH 1u
#define SIM_LDOCTR_PLL_PD(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_LDOCTR_PLL_PD_SHIFT)) & SIM_LDOCTR_PLL_PD_MASK)
#define SIM_LDOCTR_KP_MASK       0xF0000u
#define SIM_LDOCTR_KP_SHIFT      16u
#define SIM_LDOCTR_KP_WIDTH      4u
#define SIM_LDOCTR_KP(x)         (((uint32_t)(((uint32_t)(x)) << SIM_LDOCTR_KP_SHIFT)) & SIM_LDOCTR_KP_MASK)
#define SIM_LDOCTR_V12_SEL_MASK  0x3800u
#define SIM_LDOCTR_V12_SEL_SHIFT 11u
#define SIM_LDOCTR_V12_SEL_WIDTH 3u
#define SIM_LDOCTR_V12_SEL(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_LDOCTR_V12_SEL_SHIFT)) & SIM_LDOCTR_V12_SEL_MASK)
#define SIM_LDOCTR_V12G_STB_MASK  0x400u
#define SIM_LDOCTR_V12G_STB_SHIFT 10u
#define SIM_LDOCTR_V12G_STB_WIDTH 1u
#define SIM_LDOCTR_V12G_STB(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_LDOCTR_V12G_STB_SHIFT)) & SIM_LDOCTR_V12G_STB_MASK)
#define SIM_LDOCTR_TR_MASK       0x300u
#define SIM_LDOCTR_TR_SHIFT      8u
#define SIM_LDOCTR_TR_WIDTH      2u
#define SIM_LDOCTR_TR(x)         (((uint32_t)(((uint32_t)(x)) << SIM_LDOCTR_TR_SHIFT)) & SIM_LDOCTR_TR_MASK)
#define SIM_LDOCTR_TST_SEL_MASK  0x7u
#define SIM_LDOCTR_TST_SEL_SHIFT 0u
#define SIM_LDOCTR_TST_SEL_WIDTH 3u
#define SIM_LDOCTR_TST_SEL(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_LDOCTR_TST_SEL_SHIFT)) & SIM_LDOCTR_TST_SEL_MASK)
/* HMCMR Bit Fields */
#define SIM_HMCMR_USB_SUSPEND_FMD_MASK  0x10u
#define SIM_HMCMR_USB_SUSPEND_FMD_SHIFT 4u
#define SIM_HMCMR_USB_SUSPEND_FMD_WIDTH 1u
#define SIM_HMCMR_USB_SUSPEND_FMD(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << SIM_HMCMR_USB_SUSPEND_FMD_SHIFT)) & \
     SIM_HMCMR_USB_SUSPEND_FMD_MASK)
#define SIM_HMCMR_OSC8M_PD_FMD_MASK  0x8u
#define SIM_HMCMR_OSC8M_PD_FMD_SHIFT 3u
#define SIM_HMCMR_OSC8M_PD_FMD_WIDTH 1u
#define SIM_HMCMR_OSC8M_PD_FMD(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_HMCMR_OSC8M_PD_FMD_SHIFT)) & SIM_HMCMR_OSC8M_PD_FMD_MASK)
#define SIM_HMCMR_OSCPD_FMD_MASK  0x4u
#define SIM_HMCMR_OSCPD_FMD_SHIFT 2u
#define SIM_HMCMR_OSCPD_FMD_WIDTH 1u
#define SIM_HMCMR_OSCPD_FMD(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_HMCMR_OSCPD_FMD_SHIFT)) & SIM_HMCMR_OSCPD_FMD_MASK)
#define SIM_HMCMR_PLL_PD_FMD_MASK  0x2u
#define SIM_HMCMR_PLL_PD_FMD_SHIFT 1u
#define SIM_HMCMR_PLL_PD_FMD_WIDTH 1u
#define SIM_HMCMR_PLL_PD_FMD(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_HMCMR_PLL_PD_FMD_SHIFT)) & SIM_HMCMR_PLL_PD_FMD_MASK)
/* OSCCTR Bit Fields */
#define SIM_OSCCTR_TRIM_MD_MASK  0x40000u
#define SIM_OSCCTR_TRIM_MD_SHIFT 18u
#define SIM_OSCCTR_TRIM_MD_WIDTH 1u
#define SIM_OSCCTR_TRIM_MD(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_OSCCTR_TRIM_MD_SHIFT)) & SIM_OSCCTR_TRIM_MD_MASK)
#define SIM_OSCCTR_SELC_MASK  0x30000u
#define SIM_OSCCTR_SELC_SHIFT 16u
#define SIM_OSCCTR_SELC_WIDTH 2u
#define SIM_OSCCTR_SELC(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_OSCCTR_SELC_SHIFT)) & SIM_OSCCTR_SELC_MASK)
#define SIM_OSCCTR_FIN_MASK  0xF000u
#define SIM_OSCCTR_FIN_SHIFT 12u
#define SIM_OSCCTR_FIN_WIDTH 4u
#define SIM_OSCCTR_FIN(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_OSCCTR_FIN_SHIFT)) & SIM_OSCCTR_FIN_MASK)
#define SIM_OSCCTR_OSC8M_SEL_REF_MASK  0xC00u
#define SIM_OSCCTR_OSC8M_SEL_REF_SHIFT 10u
#define SIM_OSCCTR_OSC8M_SEL_REF_WIDTH 2u
#define SIM_OSCCTR_OSC8M_SEL_REF(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << SIM_OSCCTR_OSC8M_SEL_REF_SHIFT)) & \
     SIM_OSCCTR_OSC8M_SEL_REF_MASK)
#define SIM_OSCCTR_OSC8M_SEL_EN_MASK  0x200u
#define SIM_OSCCTR_OSC8M_SEL_EN_SHIFT 9u
#define SIM_OSCCTR_OSC8M_SEL_EN_WIDTH 1u
#define SIM_OSCCTR_OSC8M_SEL_EN(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_OSCCTR_OSC8M_SEL_EN_SHIFT)) & SIM_OSCCTR_OSC8M_SEL_EN_MASK)
#define SIM_OSCCTR_OSC8M_PD_MASK  0x100u
#define SIM_OSCCTR_OSC8M_PD_SHIFT 8u
#define SIM_OSCCTR_OSC8M_PD_WIDTH 1u
#define SIM_OSCCTR_OSC8M_PD(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_OSCCTR_OSC8M_PD_SHIFT)) & SIM_OSCCTR_OSC8M_PD_MASK)
#define SIM_OSCCTR_FBIN_MASK  0xFFu
#define SIM_OSCCTR_FBIN_SHIFT 0u
#define SIM_OSCCTR_FBIN_WIDTH 8u
#define SIM_OSCCTR_FBIN(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_OSCCTR_FBIN_SHIFT)) & SIM_OSCCTR_FBIN_MASK)
/* USBHMCR Bit Fields */
#define SIM_USBHMCR_USB_SUSPEND_MASK  0x1000000u
#define SIM_USBHMCR_USB_SUSPEND_SHIFT 24u
#define SIM_USBHMCR_USB_SUSPEND_WIDTH 1u
#define SIM_USBHMCR_USB_SUSPEND(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_USBHMCR_USB_SUSPEND_SHIFT)) & SIM_USBHMCR_USB_SUSPEND_MASK)
#define SIM_USBHMCR_LSPD_MASK  0x800000u
#define SIM_USBHMCR_LSPD_SHIFT 23u
#define SIM_USBHMCR_LSPD_WIDTH 1u
#define SIM_USBHMCR_LSPD(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_USBHMCR_LSPD_SHIFT)) & SIM_USBHMCR_LSPD_MASK)
#define SIM_USBHMCR_RPU_ENM_MASK  0x400000u
#define SIM_USBHMCR_RPU_ENM_SHIFT 22u
#define SIM_USBHMCR_RPU_ENM_WIDTH 1u
#define SIM_USBHMCR_RPU_ENM(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_USBHMCR_RPU_ENM_SHIFT)) & SIM_USBHMCR_RPU_ENM_MASK)
#define SIM_USBHMCR_DPPDEN_MASK  0x200000u
#define SIM_USBHMCR_DPPDEN_SHIFT 21u
#define SIM_USBHMCR_DPPDEN_WIDTH 1u
#define SIM_USBHMCR_DPPDEN(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_USBHMCR_DPPDEN_SHIFT)) & SIM_USBHMCR_DPPDEN_MASK)
#define SIM_USBHMCR_DMPDEN_MASK  0x100000u
#define SIM_USBHMCR_DMPDEN_SHIFT 20u
#define SIM_USBHMCR_DMPDEN_WIDTH 1u
#define SIM_USBHMCR_DMPDEN(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_USBHMCR_DMPDEN_SHIFT)) & SIM_USBHMCR_DMPDEN_MASK)
#define SIM_USBHMCR_RCAL_MASK  0xF0000u
#define SIM_USBHMCR_RCAL_SHIFT 16u
#define SIM_USBHMCR_RCAL_WIDTH 4u
#define SIM_USBHMCR_RCAL(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_USBHMCR_RCAL_SHIFT)) & SIM_USBHMCR_RCAL_MASK)
#define SIM_USBHMCR_FSP_MASK  0xF000u
#define SIM_USBHMCR_FSP_SHIFT 12u
#define SIM_USBHMCR_FSP_WIDTH 4u
#define SIM_USBHMCR_FSP(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_USBHMCR_FSP_SHIFT)) & SIM_USBHMCR_FSP_MASK)
#define SIM_USBHMCR_FS_MASK  0xF00u
#define SIM_USBHMCR_FS_SHIFT 8u
#define SIM_USBHMCR_FS_WIDTH 4u
#define SIM_USBHMCR_FS(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_USBHMCR_FS_SHIFT)) & SIM_USBHMCR_FS_MASK)
#define SIM_USBHMCR_FSHP_MASK  0xF0u
#define SIM_USBHMCR_FSHP_SHIFT 4u
#define SIM_USBHMCR_FSHP_WIDTH 4u
#define SIM_USBHMCR_FSHP(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_USBHMCR_FSHP_SHIFT)) & SIM_USBHMCR_FSHP_MASK)
#define SIM_USBHMCR_FSH_MASK  0xFu
#define SIM_USBHMCR_FSH_SHIFT 0u
#define SIM_USBHMCR_FSH_WIDTH 4u
#define SIM_USBHMCR_FSH(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_USBHMCR_FSH_SHIFT)) & SIM_USBHMCR_FSH_MASK)
/* RAM0HMCR0 Bit Fields */
#define SIM_RAM0HMCR0_DVS3_MASK  0x1E000000u
#define SIM_RAM0HMCR0_DVS3_SHIFT 25u
#define SIM_RAM0HMCR0_DVS3_WIDTH 4u
#define SIM_RAM0HMCR0_DVS3(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_RAM0HMCR0_DVS3_SHIFT)) & SIM_RAM0HMCR0_DVS3_MASK)
#define SIM_RAM0HMCR0_DVSE3_MASK  0x1000000u
#define SIM_RAM0HMCR0_DVSE3_SHIFT 24u
#define SIM_RAM0HMCR0_DVSE3_WIDTH 1u
#define SIM_RAM0HMCR0_DVSE3(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_RAM0HMCR0_DVSE3_SHIFT)) & SIM_RAM0HMCR0_DVSE3_MASK)
#define SIM_RAM0HMCR0_SLP2_MASK  0x200000u
#define SIM_RAM0HMCR0_SLP2_SHIFT 21u
#define SIM_RAM0HMCR0_SLP2_WIDTH 1u
#define SIM_RAM0HMCR0_SLP2(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_RAM0HMCR0_SLP2_SHIFT)) & SIM_RAM0HMCR0_SLP2_MASK)
#define SIM_RAM0HMCR0_RET2_MASK  0x100000u
#define SIM_RAM0HMCR0_RET2_SHIFT 20u
#define SIM_RAM0HMCR0_RET2_WIDTH 1u
#define SIM_RAM0HMCR0_RET2(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_RAM0HMCR0_RET2_SHIFT)) & SIM_RAM0HMCR0_RET2_MASK)
#define SIM_RAM0HMCR0_DVS2_MASK  0xE0000u
#define SIM_RAM0HMCR0_DVS2_SHIFT 17u
#define SIM_RAM0HMCR0_DVS2_WIDTH 3u
#define SIM_RAM0HMCR0_DVS2(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_RAM0HMCR0_DVS2_SHIFT)) & SIM_RAM0HMCR0_DVS2_MASK)
#define SIM_RAM0HMCR0_DVSE2_MASK  0x10000u
#define SIM_RAM0HMCR0_DVSE2_SHIFT 16u
#define SIM_RAM0HMCR0_DVSE2_WIDTH 1u
#define SIM_RAM0HMCR0_DVSE2(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_RAM0HMCR0_DVSE2_SHIFT)) & SIM_RAM0HMCR0_DVSE2_MASK)
#define SIM_RAM0HMCR0_SLP1_MASK  0x2000u
#define SIM_RAM0HMCR0_SLP1_SHIFT 13u
#define SIM_RAM0HMCR0_SLP1_WIDTH 1u
#define SIM_RAM0HMCR0_SLP1(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_RAM0HMCR0_SLP1_SHIFT)) & SIM_RAM0HMCR0_SLP1_MASK)
#define SIM_RAM0HMCR0_RET1_MASK  0x1000u
#define SIM_RAM0HMCR0_RET1_SHIFT 12u
#define SIM_RAM0HMCR0_RET1_WIDTH 1u
#define SIM_RAM0HMCR0_RET1(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_RAM0HMCR0_RET1_SHIFT)) & SIM_RAM0HMCR0_RET1_MASK)
#define SIM_RAM0HMCR0_DVS1_MASK  0xE00u
#define SIM_RAM0HMCR0_DVS1_SHIFT 9u
#define SIM_RAM0HMCR0_DVS1_WIDTH 3u
#define SIM_RAM0HMCR0_DVS1(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_RAM0HMCR0_DVS1_SHIFT)) & SIM_RAM0HMCR0_DVS1_MASK)
#define SIM_RAM0HMCR0_DVSE1_MASK  0x100u
#define SIM_RAM0HMCR0_DVSE1_SHIFT 8u
#define SIM_RAM0HMCR0_DVSE1_WIDTH 1u
#define SIM_RAM0HMCR0_DVSE1(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_RAM0HMCR0_DVSE1_SHIFT)) & SIM_RAM0HMCR0_DVSE1_MASK)
#define SIM_RAM0HMCR0_DVS_MASK  0x1Eu
#define SIM_RAM0HMCR0_DVS_SHIFT 1u
#define SIM_RAM0HMCR0_DVS_WIDTH 4u
#define SIM_RAM0HMCR0_DVS(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_RAM0HMCR0_DVS_SHIFT)) & SIM_RAM0HMCR0_DVS_MASK)
#define SIM_RAM0HMCR0_DVSE_MASK  0x1u
#define SIM_RAM0HMCR0_DVSE_SHIFT 0u
#define SIM_RAM0HMCR0_DVSE_WIDTH 1u
#define SIM_RAM0HMCR0_DVSE(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_RAM0HMCR0_DVSE_SHIFT)) & SIM_RAM0HMCR0_DVSE_MASK)
/* RAM0HMCR1 Bit Fields */
#define SIM_RAM0HMCR1_DVS_MASK  0x1Eu
#define SIM_RAM0HMCR1_DVS_SHIFT 1u
#define SIM_RAM0HMCR1_DVS_WIDTH 4u
#define SIM_RAM0HMCR1_DVS(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_RAM0HMCR1_DVS_SHIFT)) & SIM_RAM0HMCR1_DVS_MASK)
#define SIM_RAM0HMCR1_DVSE_MASK  0x1u
#define SIM_RAM0HMCR1_DVSE_SHIFT 0u
#define SIM_RAM0HMCR1_DVSE_WIDTH 1u
#define SIM_RAM0HMCR1_DVSE(x) \
    (((uint32_t)(((uint32_t)(x)) << SIM_RAM0HMCR1_DVSE_SHIFT)) & SIM_RAM0HMCR1_DVSE_MASK)

/**
 * @}
 */ /* end of group SIM_Register_Masks */

/**
 * @}
 */ /* end of group SIM_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------
   -- SMC Peripheral Access Layer
   ---------------------------------------------------------------------- */

/**
 * @addtogroup SMC_Peripheral_Access_Layer SMC Peripheral Access Layer
 * @{
 */

/** SMC - Size of Registers Arrays */

/** SMC - Register Layout Typedef */
typedef struct
{
    _NA uint8_t  RESERVED_0[8];
    _IO uint32_t PMPROT;   /**< offset: 0x8 */
    _IO uint32_t PMCTRL;   /**< offset: 0xC */
    _IO uint32_t STOPCTRL; /**< offset: 0x10 */
    __I uint32_t PMSTAT;   /**< offset: 0x14 */
} SMC_t, *SMC_MemMapPtr;

/** Number of instances of the SMC module. */
#define SMC_INSTANCE_COUNT (1u)

/* SMC - Peripheral instance base addresses */
/** Peripheral SMC base address */
#define SMC_BASE (0x4007E000u)
/** Peripheral SMC base pointer */
#define SMC ((SMC_t *)SMC_BASE)
/** Array initializer of SMC peripheral base addresses */
#define SMC_BASE_ADDRS \
    {                  \
        SMC_BASE       \
    }
/** Array initializer of SMC peripheral base pointers */
#define SMC_BASE_PTRS \
    {                 \
        SMC           \
    }

/* --------------------------------------------------------------------------
   -- SMC Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup SMC_Register_Masks SMC Register Masks
 * @{
 */

/* VERID Bit Fields */
/* PARAM Bit Fields */
/* PMPROT Bit Fields */
#define SMC_PMPROT_AVLP_MASK  0x20u
#define SMC_PMPROT_AVLP_SHIFT 5u
#define SMC_PMPROT_AVLP_WIDTH 1u
#define SMC_PMPROT_AVLP(x) \
    (((uint32_t)(((uint32_t)(x)) << SMC_PMPROT_AVLP_SHIFT)) & SMC_PMPROT_AVLP_MASK)
#define SMC_PMPROT_AHSRUN_MASK  0x80u
#define SMC_PMPROT_AHSRUN_SHIFT 7u
#define SMC_PMPROT_AHSRUN_WIDTH 1u
#define SMC_PMPROT_AHSRUN(x) \
    (((uint32_t)(((uint32_t)(x)) << SMC_PMPROT_AHSRUN_SHIFT)) & SMC_PMPROT_AHSRUN_MASK)
#define SMC_PMPROT_PMENWUSB_MASK  0x10000u
#define SMC_PMPROT_PMENWUSB_SHIFT 16u
#define SMC_PMPROT_PMENWUSB_WIDTH 1u
#define SMC_PMPROT_PMENWUSB(x) \
    (((uint32_t)(((uint32_t)(x)) << SMC_PMPROT_PMENWUSB_SHIFT)) & SMC_PMPROT_PMENWUSB_MASK)
#define SMC_PMPROT_SIRCOFFEN_MASK  0x1000000u
#define SMC_PMPROT_SIRCOFFEN_SHIFT 24u
#define SMC_PMPROT_SIRCOFFEN_WIDTH 1u
#define SMC_PMPROT_SIRCOFFEN(x) \
    (((uint32_t)(((uint32_t)(x)) << SMC_PMPROT_SIRCOFFEN_SHIFT)) & SMC_PMPROT_SIRCOFFEN_MASK)
/* PMCTRL Bit Fields */
#define SMC_PMCTRL_STOPM_MASK  0x7u
#define SMC_PMCTRL_STOPM_SHIFT 0u
#define SMC_PMCTRL_STOPM_WIDTH 3u
#define SMC_PMCTRL_STOPM(x) \
    (((uint32_t)(((uint32_t)(x)) << SMC_PMCTRL_STOPM_SHIFT)) & SMC_PMCTRL_STOPM_MASK)
#define SMC_PMCTRL_VLPSA_MASK  0x8u
#define SMC_PMCTRL_VLPSA_SHIFT 3u
#define SMC_PMCTRL_VLPSA_WIDTH 1u
#define SMC_PMCTRL_VLPSA(x) \
    (((uint32_t)(((uint32_t)(x)) << SMC_PMCTRL_VLPSA_SHIFT)) & SMC_PMCTRL_VLPSA_MASK)
#define SMC_PMCTRL_RUNM_MASK  0x60u
#define SMC_PMCTRL_RUNM_SHIFT 5u
#define SMC_PMCTRL_RUNM_WIDTH 2u
#define SMC_PMCTRL_RUNM(x) \
    (((uint32_t)(((uint32_t)(x)) << SMC_PMCTRL_RUNM_SHIFT)) & SMC_PMCTRL_RUNM_MASK)
/* STOPCTRL Bit Fields */
#define SMC_STOPCTRL_STOPO_MASK  0xC0u
#define SMC_STOPCTRL_STOPO_SHIFT 6u
#define SMC_STOPCTRL_STOPO_WIDTH 2u
#define SMC_STOPCTRL_STOPO(x) \
    (((uint32_t)(((uint32_t)(x)) << SMC_STOPCTRL_STOPO_SHIFT)) & SMC_STOPCTRL_STOPO_MASK)
/* PMSTAT Bit Fields */
#define SMC_PMSTAT_PMSTAT_MASK  0xFFu
#define SMC_PMSTAT_PMSTAT_SHIFT 0u
#define SMC_PMSTAT_PMSTAT_WIDTH 8u
#define SMC_PMSTAT_PMSTAT(x) \
    (((uint32_t)(((uint32_t)(x)) << SMC_PMSTAT_PMSTAT_SHIFT)) & SMC_PMSTAT_PMSTAT_MASK)

/**
 * @}
 */ /* end of group SMC_Register_Masks */

/**
 * @}
 */ /* end of group SMC_Peripheral_Access_Layer */

/* --------------------------------------------------------------------------
   -- TRGMUX Peripheral Access Layer
   -------------------------------------------------------------------------- */

/**
 * @addtogroup TRGMUX_Peripheral_Access_Layer TRGMUX Peripheral Access Layer
 * @{
 */

/** TRGMUX - Size of Registers Arrays */
#define TRGMUX_TRGMUXn_COUNT 26u

/** TRGMUX - Register Layout Typedef */
typedef struct
{
    _IO uint32_t TRGMUXn[TRGMUX_TRGMUXn_COUNT]; /**< offset: 0x0, array step: 0x4 */
} TRGMUX_t, *TRGMUX_MemMapPtr;

/** Number of instances of the TRGMUX module. */
#define TRGMUX_INSTANCE_COUNT (1u)

/* TRGMUX - Peripheral instance base addresses */
/** Peripheral TRGMUX base address */
#define TRGMUX_BASE (0x40063000u)
/** Peripheral TRGMUX base pointer */
#define TRGMUX ((TRGMUX_t *)TRGMUX_BASE)
/** Array initializer of TRGMUX peripheral base addresses */
#define TRGMUX_BASE_ADDRS \
    {                     \
        TRGMUX_BASE       \
    }
/** Array initializer of TRGMUX peripheral base pointers */
#define TRGMUX_BASE_PTRS \
    {                    \
        TRGMUX           \
    }

/* TRGMUX index offsets */
#define TRGMUX_DMAMUX0_INDEX 0
#define TRGMUX_EXTOUT0_INDEX 1
#define TRGMUX_EXTOUT1_INDEX 2
#define TRGMUX_FTM0_INDEX    10
#define TRGMUX_FTM1_INDEX    11
#define TRGMUX_LPUART2_INDEX 18
#define TRGMUX_LPUART0_INDEX 19
#define TRGMUX_LPUART1_INDEX 20

/* --------------------------------------------------------------------------
   -- TRGMUX Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup TRGMUX_Register_Masks TRGMUX Register Masks
 * @{
 */

/* TRGMUXn Bit Fields */
#define TRGMUX_TRGMUXn_SEL0_MASK  0x3Fu
#define TRGMUX_TRGMUXn_SEL0_SHIFT 0u
#define TRGMUX_TRGMUXn_SEL0_WIDTH 6u
#define TRGMUX_TRGMUXn_SEL0(x) \
    (((uint32_t)(((uint32_t)(x)) << TRGMUX_TRGMUXn_SEL0_SHIFT)) & TRGMUX_TRGMUXn_SEL0_MASK)
#define TRGMUX_TRGMUXn_SEL1_MASK  0x3F00u
#define TRGMUX_TRGMUXn_SEL1_SHIFT 8u
#define TRGMUX_TRGMUXn_SEL1_WIDTH 6u
#define TRGMUX_TRGMUXn_SEL1(x) \
    (((uint32_t)(((uint32_t)(x)) << TRGMUX_TRGMUXn_SEL1_SHIFT)) & TRGMUX_TRGMUXn_SEL1_MASK)
#define TRGMUX_TRGMUXn_SEL2_MASK  0x3F0000u
#define TRGMUX_TRGMUXn_SEL2_SHIFT 16u
#define TRGMUX_TRGMUXn_SEL2_WIDTH 6u
#define TRGMUX_TRGMUXn_SEL2(x) \
    (((uint32_t)(((uint32_t)(x)) << TRGMUX_TRGMUXn_SEL2_SHIFT)) & TRGMUX_TRGMUXn_SEL2_MASK)
#define TRGMUX_TRGMUXn_SEL3_MASK  0x3F000000u
#define TRGMUX_TRGMUXn_SEL3_SHIFT 24u
#define TRGMUX_TRGMUXn_SEL3_WIDTH 6u
#define TRGMUX_TRGMUXn_SEL3(x) \
    (((uint32_t)(((uint32_t)(x)) << TRGMUX_TRGMUXn_SEL3_SHIFT)) & TRGMUX_TRGMUXn_SEL3_MASK)
#define TRGMUX_TRGMUXn_LK_MASK  0x80000000u
#define TRGMUX_TRGMUXn_LK_SHIFT 31u
#define TRGMUX_TRGMUXn_LK_WIDTH 1u
#define TRGMUX_TRGMUXn_LK(x) \
    (((uint32_t)(((uint32_t)(x)) << TRGMUX_TRGMUXn_LK_SHIFT)) & TRGMUX_TRGMUXn_LK_MASK)

/**
 * @}
 */ /* end of group TRGMUX_Register_Masks */

/**
 * @}
 */ /* end of group TRGMUX_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------
   -- WDOG Peripheral Access Layer
   ---------------------------------------------------------------------- */

/**
 * @addtogroup WDOG_Peripheral_Access_Layer WDOG Peripheral Access Layer
 * @{
 */

/** WDOG - Size of Registers Arrays */

/** WDOG - Register Layout Typedef */
typedef struct
{
    _IO uint32_t CS;    /**< offset: 0x0 */
    _IO uint32_t CNT;   /**< offset: 0x4 */
    _IO uint32_t TOVAL; /**< offset: 0x8 */
    _IO uint32_t WIN;   /**< offset: 0xC */
} WDOG_t, *WDOG_MemMapPtr;

/** Number of instances of the WDOG module. */
#define WDOG_INSTANCE_COUNT (1u)

/* WDOG - Peripheral instance base addresses */
/** Peripheral WDOG base address */
#define WDOG_BASE (0x40052000u)
/** Peripheral WDOG base pointer */
#define WDOG ((WDOG_t *)WDOG_BASE)
/** Array initializer of WDOG peripheral base addresses */
#define WDOG_BASE_ADDRS \
    {                   \
        WDOG_BASE       \
    }
/** Array initializer of WDOG peripheral base pointers */
#define WDOG_BASE_PTRS \
    {                  \
        WDOG           \
    }
/** Number of interrupt vector arrays for the WDOG module. */
#define WDOG_IRQS_ARR_COUNT (1u)
/** Number of interrupt channels for the WDOG module. */
#define WDOG_IRQS_CH_COUNT (1u)
/** Interrupt vectors for the WDOG peripheral type */
#define WDOG_IRQS \
    {             \
        WDOG_IRQn \
    }

/* --------------------------------------------------------------------------
   -- WDOG Register Masks
   -------------------------------------------------------------------------- */

/**
 * @addtogroup WDOG_Register_Masks WDOG Register Masks
 * @{
 */

/* CS Bit Fields */
#define WDOG_CS_STOP_MASK    0x1u
#define WDOG_CS_STOP_SHIFT   0u
#define WDOG_CS_STOP_WIDTH   1u
#define WDOG_CS_STOP(x)      (((uint32_t)(((uint32_t)(x)) << WDOG_CS_STOP_SHIFT)) & WDOG_CS_STOP_MASK)
#define WDOG_CS_UPDATE_MASK  0x20u
#define WDOG_CS_UPDATE_SHIFT 5u
#define WDOG_CS_UPDATE_WIDTH 1u
#define WDOG_CS_UPDATE(x) \
    (((uint32_t)(((uint32_t)(x)) << WDOG_CS_UPDATE_SHIFT)) & WDOG_CS_UPDATE_MASK)
#define WDOG_CS_INT_MASK      0x40u
#define WDOG_CS_INT_SHIFT     6u
#define WDOG_CS_INT_WIDTH     1u
#define WDOG_CS_INT(x)        (((uint32_t)(((uint32_t)(x)) << WDOG_CS_INT_SHIFT)) & WDOG_CS_INT_MASK)
#define WDOG_CS_EN_MASK       0x80u
#define WDOG_CS_EN_SHIFT      7u
#define WDOG_CS_EN_WIDTH      1u
#define WDOG_CS_EN(x)         (((uint32_t)(((uint32_t)(x)) << WDOG_CS_EN_SHIFT)) & WDOG_CS_EN_MASK)
#define WDOG_CS_CLK_MASK      0x300u
#define WDOG_CS_CLK_SHIFT     8u
#define WDOG_CS_CLK_WIDTH     2u
#define WDOG_CS_CLK(x)        (((uint32_t)(((uint32_t)(x)) << WDOG_CS_CLK_SHIFT)) & WDOG_CS_CLK_MASK)
#define WDOG_CS_RCS_MASK      0x400u
#define WDOG_CS_RCS_SHIFT     10u
#define WDOG_CS_RCS_WIDTH     1u
#define WDOG_CS_RCS(x)        (((uint32_t)(((uint32_t)(x)) << WDOG_CS_RCS_SHIFT)) & WDOG_CS_RCS_MASK)
#define WDOG_CS_ULK_MASK      0x800u
#define WDOG_CS_ULK_SHIFT     11u
#define WDOG_CS_ULK_WIDTH     1u
#define WDOG_CS_ULK(x)        (((uint32_t)(((uint32_t)(x)) << WDOG_CS_ULK_SHIFT)) & WDOG_CS_ULK_MASK)
#define WDOG_CS_PRES_MASK     0x1000u
#define WDOG_CS_PRES_SHIFT    12u
#define WDOG_CS_PRES_WIDTH    1u
#define WDOG_CS_PRES(x)       (((uint32_t)(((uint32_t)(x)) << WDOG_CS_PRES_SHIFT)) & WDOG_CS_PRES_MASK)
#define WDOG_CS_CMD32EN_MASK  0x2000u
#define WDOG_CS_CMD32EN_SHIFT 13u
#define WDOG_CS_CMD32EN_WIDTH 1u
#define WDOG_CS_CMD32EN(x) \
    (((uint32_t)(((uint32_t)(x)) << WDOG_CS_CMD32EN_SHIFT)) & WDOG_CS_CMD32EN_MASK)
#define WDOG_CS_FLG_MASK  0x4000u
#define WDOG_CS_FLG_SHIFT 14u
#define WDOG_CS_FLG_WIDTH 1u
#define WDOG_CS_FLG(x)    (((uint32_t)(((uint32_t)(x)) << WDOG_CS_FLG_SHIFT)) & WDOG_CS_FLG_MASK)
#define WDOG_CS_WIN_MASK  0x8000u
#define WDOG_CS_WIN_SHIFT 15u
#define WDOG_CS_WIN_WIDTH 1u
#define WDOG_CS_WIN(x)    (((uint32_t)(((uint32_t)(x)) << WDOG_CS_WIN_SHIFT)) & WDOG_CS_WIN_MASK)
/* CNT Bit Fields */
#define WDOG_CNT_CNTLOW_MASK  0xFFu
#define WDOG_CNT_CNTLOW_SHIFT 0u
#define WDOG_CNT_CNTLOW_WIDTH 8u
#define WDOG_CNT_CNTLOW(x) \
    (((uint32_t)(((uint32_t)(x)) << WDOG_CNT_CNTLOW_SHIFT)) & WDOG_CNT_CNTLOW_MASK)
#define WDOG_CNT_CNTHIGH_MASK  0xFF00u
#define WDOG_CNT_CNTHIGH_SHIFT 8u
#define WDOG_CNT_CNTHIGH_WIDTH 8u
#define WDOG_CNT_CNTHIGH(x) \
    (((uint32_t)(((uint32_t)(x)) << WDOG_CNT_CNTHIGH_SHIFT)) & WDOG_CNT_CNTHIGH_MASK)
/* TOVAL Bit Fields */
#define WDOG_TOVAL_TOVALLOW_MASK  0xFFu
#define WDOG_TOVAL_TOVALLOW_SHIFT 0u
#define WDOG_TOVAL_TOVALLOW_WIDTH 8u
#define WDOG_TOVAL_TOVALLOW(x) \
    (((uint32_t)(((uint32_t)(x)) << WDOG_TOVAL_TOVALLOW_SHIFT)) & WDOG_TOVAL_TOVALLOW_MASK)
#define WDOG_TOVAL_TOVALHIGH_MASK  0xFF00u
#define WDOG_TOVAL_TOVALHIGH_SHIFT 8u
#define WDOG_TOVAL_TOVALHIGH_WIDTH 8u
#define WDOG_TOVAL_TOVALHIGH(x) \
    (((uint32_t)(((uint32_t)(x)) << WDOG_TOVAL_TOVALHIGH_SHIFT)) & WDOG_TOVAL_TOVALHIGH_MASK)
/* WIN Bit Fields */
#define WDOG_WIN_WINLOW_MASK  0xFFu
#define WDOG_WIN_WINLOW_SHIFT 0u
#define WDOG_WIN_WINLOW_WIDTH 8u
#define WDOG_WIN_WINLOW(x) \
    (((uint32_t)(((uint32_t)(x)) << WDOG_WIN_WINLOW_SHIFT)) & WDOG_WIN_WINLOW_MASK)
#define WDOG_WIN_WINHIGH_MASK  0xFF00u
#define WDOG_WIN_WINHIGH_SHIFT 8u
#define WDOG_WIN_WINHIGH_WIDTH 8u
#define WDOG_WIN_WINHIGH(x) \
    (((uint32_t)(((uint32_t)(x)) << WDOG_WIN_WINHIGH_SHIFT)) & WDOG_WIN_WINHIGH_MASK)

/**
 * @}
 */ /* end of group WDOG_Register_Masks */

/**
 * @}
 */ /* end of group WDOG_Peripheral_Access_Layer */

/* ----------------------------------------------------------------------------
   -- FSUSB Peripheral Access Layer
   ----------------------------------------------------------------------------
 */

/**
 * @addtogroup FSUSB_Peripheral_Access_Layer FSUSB Peripheral Access Layer
 * @{
 */

/** FSUSB - Size of Registers Arrays */
#define FSUSB_SRAM0_BUF_SIZE (64u)
#define FSUSB_SRAM1_BUF_SIZE (64u)
#define FSUSB_SRAM3_BUF_SIZE (64u)
#define FSUSB_SRAM0_MAP_BIAS (0x100u)
#define FSUSB_SRAM1_MAP_BIAS (0x200u)
#define FSUSB_SRAM3_MAP_BIAS (0x400u)

/** FSUSB - Register Layout Typedef */
typedef struct
{
    __I uint32_t VERID;   /**< Version ID Register, offset: 0x0 */
    __I uint32_t PARAM;   /**< Parameter Register, offset: 0x4 */
    _NA uint32_t RESERVED_0[1];
    _IO uint32_t SRCSR;   /**< FSUSB SRAM CS Register, offset: 0xC */
    __I uint32_t STS;     /**< FSUSB Status Register, offset: 0x10 */
    _IO uint32_t PHY;     /**< FSUSB Phy. Register, offset: 0x14 */
    _IO uint32_t MODE;    /**< FSUSB Mode Register, offset: 0x18 */
    _IO uint32_t SPD;     /**< FSUSB Speed Control Register, offset: 0x1C */
    _IO uint32_t FFSR;    /**< FSUSB Force to FS Control Register, offset: 0x20 */
    _IO uint32_t TMEN;    /**< FSUSB Test Mode Enable Register, offset: 0x24 */
    _IO uint32_t TMSEL;   /**< FSUSB Test Mode Selection Register, offset: 0x28 */
    _IO uint32_t UADR;    /**< FSUSB USB Address Register, offset: 0x2C */
    _IO uint32_t UISR;    /**< FSUSB USB Interrupt Status Register, offset: 0x30 */
    _IO uint32_t INTMSK;  /**< FSUSB Interrupt Mask Register, offset: 0x34 */
    _IO uint32_t POSR;    /**< FSUSB Power On Suspend Register, offset: 0x38 */
    _NA uint32_t RESERVED_1[1];
    __I uint32_t EP0SR;   /**< FSUSB End-point 0 Status Register, offset: 0x40 */
    _IO uint32_t EP0CR;   /**< FSUSB End-point 0 Control Register, offset: 0x44 */
    _IO uint32_t EP0TR;   /**< FSUSB End-point 0 Toggle Register, offset: 0x48 */
    _IO uint32_t EP0TZR;  /**< FSUSB End-point 0 Transmit Zero Length Register,
                               offset: 0x4C */
    _IO uint32_t EP0TPR;  /**< FSUSB End-point 0 Transmit Payload Register,
                               offset: 0x50 */
    _IO uint32_t EP0STR;  /**< FSUSB End-point 0 Stall Register, offset: 0x54 */
    _IO uint32_t EP0TCR;  /**< FSUSB End-point 0 Control Ext. Clear Toggle
                               Register, offset: 0x58 */
    __I uint32_t EP0RPR;  /**< FSUSB End-point 0 Receive Payload Register,
                              offset: 0x5C */
    __I uint32_t EPX;     /**< FSUSB End-points 0 Special Status Register, offset: 0x60 */
    _IO uint32_t EPXSPC;  /**< FSUSB End-point 0 Special Control Ext. 2
                               Register, offset: 0x64 */
    _IO uint32_t EP1STR;  /**< FSUSB End-point 1 Stall Register, offset: 0x68 */
    _IO uint32_t EP1TCR;  /**< FSUSB End-point 1 Control Ext. Clear Toggle
                               Register, offset: 0x6C */
    __I uint32_t EP1SR;   /**< FSUSB End-point 1 Status Register, offset: 0x70 */
    _IO uint32_t EP1TPR;  /**< FSUSB End-point 1 Transmit Payload Register,
                               offset: 0x74 */
    _IO uint32_t EP2STR;  /**< FSUSB End-point 2 Stall Register, offset: 0x78 */
    _IO uint32_t EP2TCR;  /**< FSUSB End-point 2 Control Ext. Clear Toggle
                               Register, offset: 0x7C */
    __I uint32_t EP2SR;   /**< FSUSB End-point 2 Status Register, offset: 0x80 */
    _IO uint32_t EP2TPR;  /**< FSUSB End-point 2 Transmit Payload Register,
                               offset: 0x84 */
    _IO uint32_t EP3STR;  /**< FSUSB End-point 3 Stall Register, offset: 0x88 */
    _IO uint32_t EP3TCR;  /**< FSUSB End-point 3 Control Ext. Clear Toggle
                               Register, offset: 0x8C */
    __I uint32_t EP3SR;   /**< FSUSB End-point 3 Status Register, offset: 0x90 */
    _IO uint32_t EP3TPR;  /**< FSUSB End-point 3 Transmit Payload Register,
                               offset: 0x94 */
    _IO uint32_t EP4CR;   /**< FSUSB End-point 4 Control Register, offset: 0x98 */
    _IO uint32_t EP4STR;  /**< FSUSB End-point 4 Stall Register, offset: 0x9C */
    _IO uint32_t EP4TCR;  /**< FSUSB End-point 4 Control Ext. Clear Toggle
                               Register, offset: 0xA0 */
    __I uint32_t EP4RPR;  /**< FSUSB End-point 4 Receive Payload Register,
                              offset: 0xA4 */
    _NA uint32_t RESERVED_2[2];
    _IO uint32_t SD0;     /**< FSUSB Setup Data 0 Register, offset: 0xB0 */
    _IO uint32_t SD1;     /**< FSUSB Setup Data 1 Register, offset: 0xB4 */
    _IO uint32_t SD2;     /**< FSUSB Setup Data 2 Register, offset: 0xB8 */
    _IO uint32_t SD3;     /**< FSUSB Setup Data 3 Register, offset: 0xBC */
    _IO uint32_t SD4;     /**< FSUSB Setup Data 4 Register, offset: 0xC0 */
    _IO uint32_t SD5;     /**< FSUSB Setup Data 5 Register, offset: 0xC4 */
    _IO uint32_t SD6;     /**< FSUSB Setup Data 6 Register, offset: 0xC8 */
    _IO uint32_t SD7;     /**< FSUSB Setup Data 7 Register, offset: 0xCC */
    _IO uint32_t EP56STR; /**< FSUSB End-point 5-6 Stall Register, offset: 0xD0 */
    _IO uint32_t EP56TCR; /**< FSUSB End-point 5-6 Control Ext. Status
                                Register, offset: 0xD4 */
    _IO uint32_t EP56CSR; /**< FSUSB End-point 5-6 Control Status Register,
                                offset: 0xD8 */
    _IO uint32_t EP56PR;  /**< FSUSB End-point 5-6 Payload Register,
                                offset: 0xDC */
    _NA uint32_t RESERVED_3[4];
    _IO uint32_t TRIM;    /**< FSUSB RC Trim Tuning Register, offset: 0xF0 */
    _IO uint32_t TRSC;    /**< FSUSB RC Trim Scale Register, offset: 0xF4 */
    __I uint32_t TRDIF;   /**< FSUSB RC Lead/Lag Difference Register, offset: 0xF8 */
} FSUSB_t, *FSUSB_MemMapPtr;

/** Number of instances of the FSUSB module. */
#define FSUSB_INSTANCE_COUNT (1u)

/* FSUSB - Peripheral instance base addresses */
/** Peripheral FSUSB base address */
#define FSUSB_BASE (0x40079000u)
/** Peripheral FSUSB base pointer */
#define FSUSB ((FSUSB_t *)FSUSB_BASE)
/** Array initializer of FSUSB peripheral base addresses */
#define FSUSB_BASE_ADDRS \
    {                    \
        FSUSB_BASE       \
    }
/** Array initializer of FSUSB peripheral base pointers */
#define FSUSB_BASE_PTRS \
    {                   \
        FSUSB           \
    }
/** Number of interrupt vector arrays for the FSUSB module. */
#define FSUSB_IRQS_ARR_COUNT (1u)
/** Number of interrupt channels for the RX_TX type of FSUSB module. */
#define FSUSB_RX_TX_IRQS_CH_COUNT (1u)
/** Interrupt vectors for the FSUSB peripheral type */
#define FSUSB_IRQS \
    {              \
        FSUSB_IRQn \
    }

/* ----------------------------------------------------------------------------
   -- FSUSB Register Masks
   ----------------------------------------------------------------------------
 */

/**
 * @addtogroup FSUSB_Register_Masks FSUSB Register Masks
 * @{
 */

/* VERID Bit Fields */
#define FSUSB_VERID_FEATURE_MASK  0xFFFFu
#define FSUSB_VERID_FEATURE_SHIFT 0u
#define FSUSB_VERID_FEATURE_WIDTH 16u
#define FSUSB_VERID_FEATURE(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_VERID_FEATURE_SHIFT)) & FSUSB_VERID_FEATURE_MASK)
#define FSUSB_VERID_MINOR_MASK  0xFF0000u
#define FSUSB_VERID_MINOR_SHIFT 16u
#define FSUSB_VERID_MINOR_WIDTH 8u
#define FSUSB_VERID_MINOR(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_VERID_MINOR_SHIFT)) & FSUSB_VERID_MINOR_MASK)
#define FSUSB_VERID_MAJOR_MASK  0xFF000000u
#define FSUSB_VERID_MAJOR_SHIFT 24u
#define FSUSB_VERID_MAJOR_WIDTH 8u
#define FSUSB_VERID_MAJOR(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_VERID_MAJOR_SHIFT)) & FSUSB_VERID_MAJOR_MASK)
/* PARAM Bit Fields */
#define FSUSB_PARAM_TXFIFO_MASK  0xFFu
#define FSUSB_PARAM_TXFIFO_SHIFT 0u
#define FSUSB_PARAM_TXFIFO_WIDTH 8u
#define FSUSB_PARAM_TXFIFO(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_PARAM_TXFIFO_SHIFT)) & FSUSB_PARAM_TXFIFO_MASK)
#define FSUSB_PARAM_RXFIFO_MASK  0xFF00u
#define FSUSB_PARAM_RXFIFO_SHIFT 8u
#define FSUSB_PARAM_RXFIFO_WIDTH 8u
#define FSUSB_PARAM_RXFIFO(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_PARAM_RXFIFO_SHIFT)) & FSUSB_PARAM_RXFIFO_MASK)
/* SRCSR Bit Fields */
#define FSUSB_SRCSR_CS0EN_MASK  0x1u
#define FSUSB_SRCSR_CS0EN_SHIFT 0u
#define FSUSB_SRCSR_CS0EN_WIDTH 1u
#define FSUSB_SRCSR_CS0EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_SRCSR_CS0EN_SHIFT)) & FSUSB_SRCSR_CS0EN_MASK)
#define FSUSB_SRCSR_CS1EN_MASK  0x2u
#define FSUSB_SRCSR_CS1EN_SHIFT 1u
#define FSUSB_SRCSR_CS1EN_WIDTH 1u
#define FSUSB_SRCSR_CS1EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_SRCSR_CS1EN_SHIFT)) & FSUSB_SRCSR_CS1EN_MASK)
#define FSUSB_SRCSR_CS2EN_MASK  0x4u
#define FSUSB_SRCSR_CS2EN_SHIFT 2u
#define FSUSB_SRCSR_CS2EN_WIDTH 1u
#define FSUSB_SRCSR_CS2EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_SRCSR_CS2EN_SHIFT)) & FSUSB_SRCSR_CS2EN_MASK)
#define FSUSB_SRCSR_CS3EN_MASK  0x8u
#define FSUSB_SRCSR_CS3EN_SHIFT 3u
#define FSUSB_SRCSR_CS3EN_WIDTH 1u
#define FSUSB_SRCSR_CS3EN(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_SRCSR_CS3EN_SHIFT)) & FSUSB_SRCSR_CS3EN_MASK)
/* STS Bit Fields */
#define FSUSB_STS_BL_MASK  0x3u
#define FSUSB_STS_BL_SHIFT 0u
#define FSUSB_STS_BL_WIDTH 2u
/* PHY Bit Fields */
#define FSUSB_PHY_CONN_MASK  0x1u
#define FSUSB_PHY_CONN_SHIFT 0u
#define FSUSB_PHY_CONN_WIDTH 1u
#define FSUSB_PHY_CONN(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_PHY_CONN_SHIFT)) & FSUSB_PHY_CONN_MASK)
#define FSUSB_PHY_DPDMLI_MASK  0x2u
#define FSUSB_PHY_DPDMLI_SHIFT 1u
#define FSUSB_PHY_DPDMLI_WIDTH 1u
#define FSUSB_PHY_DPDMLI(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_PHY_DPDMLI_SHIFT)) & FSUSB_PHY_DPDMLI_MASK)

/* UADR Bit Fields */
#define FSUSB_UADR_MASK  0x7Fu
#define FSUSB_UADR_SHIFT 0u
#define FSUSB_UADR_WIDTH 7u
#define FSUSB_UADDR(x)   (((uint32_t)(((uint32_t)(x)) << FSUSB_UADR_SHIFT)) & FSUSB_UADR_MASK)
/* UISR Bit Fields */
#define FSUSB_UISR_RST_MASK  0x1u
#define FSUSB_UISR_RST_SHIFT 0u
#define FSUSB_UISR_RST_WIDTH 1u
#define FSUSB_UISR_RST(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_UISR_RST_SHIFT)) & FSUSB_UISR_RST_MASK)
#define FSUSB_UISR_SETUP_MASK  0x2u
#define FSUSB_UISR_SETUP_SHIFT 1u
#define FSUSB_UISR_SETUP_WIDTH 1u
#define FSUSB_UISR_SETUP(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_UISR_SETUP_SHIFT)) & FSUSB_UISR_SETUP_MASK)
#define FSUSB_UISR_EP0TXDONE_MASK  0x8u
#define FSUSB_UISR_EP0TXDONE_SHIFT 3u
#define FSUSB_UISR_EP0TXDONE_WIDTH 1u
#define FSUSB_UISR_EP0TXDONE(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_UISR_EP0TXDONE_SHIFT)) & FSUSB_UISR_EP0TXDONE_MASK)
#define FSUSB_UISR_EP1TXDONE_MASK  0x10u
#define FSUSB_UISR_EP1TXDONE_SHIFT 4u
#define FSUSB_UISR_EP1TXDONE_WIDTH 1u
#define FSUSB_UISR_EP1TXDONE(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_UISR_EP1TXDONE_SHIFT)) & FSUSB_UISR_EP1TXDONE_MASK)
#define FSUSB_UISR_EP2TXDONE_MASK  0x20u
#define FSUSB_UISR_EP2TXDONE_SHIFT 5u
#define FSUSB_UISR_EP2TXDONE_WIDTH 1u
#define FSUSB_UISR_EP2TXDONE(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_UISR_EP2TXDONE_SHIFT)) & FSUSB_UISR_EP2TXDONE_MASK)
#define FSUSB_UISR_EP3TXDONE_MASK  0x40u
#define FSUSB_UISR_EP3TXDONE_SHIFT 6u
#define FSUSB_UISR_EP3TXDONE_WIDTH 1u
#define FSUSB_UISR_EP3TXDONE(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_UISR_EP3TXDONE_SHIFT)) & FSUSB_UISR_EP3TXDONE_MASK)
#define FSUSB_UISR_EP4RXRDY_MASK  0x80u
#define FSUSB_UISR_EP4RXRDY_SHIFT 7u
#define FSUSB_UISR_EP4RXRDY_WIDTH 1u
#define FSUSB_UISR_EP4RXRDY(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_UISR_EP4RXRDY_SHIFT)) & FSUSB_UISR_EP4RXRDY_MASK)
#define FSUSB_UISR_EP5TXDONE_MASK  0x100u
#define FSUSB_UISR_EP5TXDONE_SHIFT 8u
#define FSUSB_UISR_EP5TXDONE_WIDTH 1u
#define FSUSB_UISR_EP5TXDONE(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_UISR_EP5TXDONE_SHIFT)) & FSUSB_UISR_EP5TXDONE_MASK)
#define FSUSB_UISR_EP6RXRDY_MASK  0x200u
#define FSUSB_UISR_EP6RXRDY_SHIFT 9u
#define FSUSB_UISR_EP6RXRDY_WIDTH 1u
#define FSUSB_UISR_EP6RXRDY(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_UISR_EP6RXRDY_SHIFT)) & FSUSB_UISR_EP6RXRDY_MASK)
/* INTMSK Bit Fields */
#define FSUSB_INTMSK_RST_MASK  0x1u
#define FSUSB_INTMSK_RST_SHIFT 0u
#define FSUSB_INTMSK_RST_WIDTH 1u
#define FSUSB_INTMSK_RST(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_INTMSK_RST_SHIFT)) & FSUSB_INTMSK_RST_MASK)
#define FSUSB_INTMSK_SETUP_MASK  0x2u
#define FSUSB_INTMSK_SETUP_SHIFT 1u
#define FSUSB_INTMSK_SETUP_WIDTH 1u
#define FSUSB_INTMSK_SETUP(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_INTMSK_SETUP_SHIFT)) & FSUSB_INTMSK_SETUP_MASK)
#define FSUSB_INTMSK_EP0TX_MASK  0x8u
#define FSUSB_INTMSK_EP0TX_SHIFT 3u
#define FSUSB_INTMSK_EP0TX_WIDTH 1u
#define FSUSB_INTMSK_EP0TX(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_INTMSK_EP0TX_SHIFT)) & FSUSB_INTMSK_EP0TX_MASK)
#define FSUSB_INTMSK_EP1TX_MASK  0x10u
#define FSUSB_INTMSK_EP1TX_SHIFT 4u
#define FSUSB_INTMSK_EP1TX_WIDTH 1u
#define FSUSB_INTMSK_EP1TX(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_INTMSK_EP1TX_SHIFT)) & FSUSB_INTMSK_EP1TX_MASK)
#define FSUSB_INTMSK_EP2TX_MASK  0x20u
#define FSUSB_INTMSK_EP2TX_SHIFT 5u
#define FSUSB_INTMSK_EP2TX_WIDTH 1u
#define FSUSB_INTMSK_EP2TX(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_INTMSK_EP2TX_SHIFT)) & FSUSB_INTMSK_EP2TX_MASK)
#define FSUSB_INTMSK_EP3TX_MASK  0x40u
#define FSUSB_INTMSK_EP3TX_SHIFT 6u
#define FSUSB_INTMSK_EP3TX_WIDTH 1u
#define FSUSB_INTMSK_EP3TX(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_INTMSK_EP3TX_SHIFT)) & FSUSB_INTMSK_EP3TX_MASK)
#define FSUSB_INTMSK_EP4RX_MASK  0x80u
#define FSUSB_INTMSK_EP4RX_SHIFT 7u
#define FSUSB_INTMSK_EP4RX_WIDTH 1u
#define FSUSB_INTMSK_EP4RX(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_INTMSK_EP4RX_SHIFT)) & FSUSB_INTMSK_EP4RX_MASK)
#define FSUSB_INTMSK_EP5TX_MASK  0x100u
#define FSUSB_INTMSK_EP5TX_SHIFT 8u
#define FSUSB_INTMSK_EP5TX_WIDTH 1u
#define FSUSB_INTMSK_EP5TX(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_INTMSK_EP5TX_SHIFT)) & FSUSB_INTMSK_EP5TX_MASK)
#define FSUSB_INTMSK_EP6RX_MASK  0x200u
#define FSUSB_INTMSK_EP6RX_SHIFT 9u
#define FSUSB_INTMSK_EP6RX_WIDTH 1u
#define FSUSB_INTMSK_EP6RX(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_INTMSK_EP6RX_SHIFT)) & FSUSB_INTMSK_EP6RX_MASK)
/* POSR Bit Fields */
#define FSUSB_POSR_ON_MASK  0x4u
#define FSUSB_POSR_ON_SHIFT 2u
#define FSUSB_POSR_ON_WIDTH 1u
#define FSUSB_POSR_ON(x)    (((uint32_t)(((uint32_t)(x)) << FSUSB_POSR_ON_SHIFT)) & FSUSB_POSR_ON_MASK)

/* EP0SR Bit Fields */
#define FSUSB_EP0SR_RXSETUP_MASK  0x1u
#define FSUSB_EP0SR_RXSETUP_SHIFT 0u
#define FSUSB_EP0SR_RXSETUP_WIDTH 1u
#define FSUSB_EP0SR_RXZERO_MASK   0x2u
#define FSUSB_EP0SR_RXZERO_SHIFT  1u
#define FSUSB_EP0SR_RXZERO_WIDTH  1u
#define FSUSB_EP0SR_TXBSY_MASK    0x4u
#define FSUSB_EP0SR_TXBSY_SHIFT   2u
#define FSUSB_EP0SR_TXBSY_WIDTH   1u
/* EP0CR Bit Fields */
#define FSUSB_EP0CR_LAT_MASK  0x1u
#define FSUSB_EP0CR_LAT_SHIFT 0u
#define FSUSB_EP0CR_LAT_WIDTH 1u
#define FSUSB_EP0CR_LAT(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP0CR_LAT_SHIFT)) & FSUSB_EP0CR_LAT_MASK)
/* EP0TR Bit Fields */
#define FSUSB_EP0TR_TX_MASK  0x1u
#define FSUSB_EP0TR_TX_SHIFT 0u
#define FSUSB_EP0TR_TX_WIDTH 1u
#define FSUSB_EP0TR_TX(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP0TR_TX_SHIFT)) & FSUSB_EP0TR_TX_MASK)
/* EP0TZR Bit Fields */
#define FSUSB_EP0TZR_ZERO_MASK  0x1u
#define FSUSB_EP0TZR_ZERO_SHIFT 0u
#define FSUSB_EP0TZR_ZERO_WIDTH 1u
/* EP0TPR Bit Fields */
#define FSUSB_EP0TPR_LEN_MASK  0x7Fu
#define FSUSB_EP0TPR_LEN_SHIFT 0u
#define FSUSB_EP0TPR_LEN_WIDTH 7u
#define FSUSB_EP0TPR_LEN(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP0TPR_LEN_SHIFT)) & FSUSB_EP0TPR_LEN_MASK)
/* EP0STR Bit Fields */
#define FSUSB_EP0STR_STL_MASK  0x1u
#define FSUSB_EP0STR_STL_SHIFT 0u
#define FSUSB_EP0STR_STL_WIDTH 1u
#define FSUSB_EP0STR_STL(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP0STR_STL_SHIFT)) & FSUSB_EP0STR_STL_MASK)
/* EP0TCR Bit Fields */
#define FSUSB_EP0TCR_CLR_MASK  0x1u
#define FSUSB_EP0TCR_CLR_SHIFT 0u
#define FSUSB_EP0TCR_CLR_WIDTH 1u
#define FSUSB_EP0TCR_CLR(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP0TCR_CLR_SHIFT)) & FSUSB_EP0TCR_CLR_MASK)
/* EP0RPR Bit Fields */
#define FSUSB_EP0RPR_LEN_MASK  0x7Fu
#define FSUSB_EP0RPR_LEN_SHIFT 0u
#define FSUSB_EP0RPR_LEN_WIDTH 7u
/* EPX Bit Fields */
#define FSUSB_EPX_EP0IN_MASK   0x1u
#define FSUSB_EPX_EP0IN_SHIFT  0u
#define FSUSB_EPX_EP0IN_WIDTH  1u
#define FSUSB_EPX_EP0OUT_MASK  0x2u
#define FSUSB_EPX_EP0OUT_SHIFT 1u
#define FSUSB_EPX_EP0OUT_WIDTH 1u
#define FSUSB_EPX_EP1IN_MASK   0x4u
#define FSUSB_EPX_EP1IN_SHIFT  2u
#define FSUSB_EPX_EP1IN_WIDTH  1u
#define FSUSB_EPX_EP2IN_MASK   0x8u
#define FSUSB_EPX_EP2IN_SHIFT  3u
#define FSUSB_EPX_EP2IN_WIDTH  1u
#define FSUSB_EPX_EP3IN_MASK   0x10u
#define FSUSB_EPX_EP3IN_SHIFT  4u
#define FSUSB_EPX_EP3IN_WIDTH  1u
#define FSUSB_EPX_EP4OUT_MASK  0x20u
#define FSUSB_EPX_EP4OUT_SHIFT 5u
#define FSUSB_EPX_EP4OUT_WIDTH 1u
#define FSUSB_EPX_EP5IN_MASK   0x40u
#define FSUSB_EPX_EP5IN_SHIFT  6u
#define FSUSB_EPX_EP5IN_WIDTH  1u
#define FSUSB_EPX_EP6OUT_MASK  0x80u
#define FSUSB_EPX_EP6OUT_SHIFT 7u
#define FSUSB_EPX_EP6OUT_WIDTH 1u

/* EP1STR Bit Fields */
#define FSUSB_EP1STR_STL_MASK  0x1u
#define FSUSB_EP1STR_STL_SHIFT 0u
#define FSUSB_EP1STR_STL_WIDTH 1u
#define FSUSB_EP1STR_STL(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP1STR_STL_SHIFT)) & FSUSB_EP1STR_STL_MASK)
/* EP1TCR Bit Fields */
#define FSUSB_EP1TCR_CLR_MASK  0x1u
#define FSUSB_EP1TCR_CLR_SHIFT 0u
#define FSUSB_EP1TCR_CLR_WIDTH 1u
#define FSUSB_EP1TCR_CLR(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP1TCR_CLR_SHIFT)) & FSUSB_EP1TCR_CLR_MASK)
/* EP1SR Bit Fields */
#define FSUSB_EP1SR_TXBSY_MASK  0x1u
#define FSUSB_EP1SR_TXBSY_SHIFT 0u
#define FSUSB_EP1SR_TXBSY_WIDTH 1u
/* EP1TPR Bit Fields */
#define FSUSB_EP1TPR_LEN_MASK  0x7Fu
#define FSUSB_EP1TPR_LEN_SHIFT 0u
#define FSUSB_EP1TPR_LEN_WIDTH 7u
#define FSUSB_EP1TPR_LEN(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP1TPR_LEN_SHIFT)) & FSUSB_EP1TPR_LEN_MASK)
/* EP2STR Bit Fields */
#define FSUSB_EP2STR_STL_MASK  0x1u
#define FSUSB_EP2STR_STL_SHIFT 0u
#define FSUSB_EP2STR_STL_WIDTH 1u
#define FSUSB_EP2STR_STL(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP2STR_STL_SHIFT)) & FSUSB_EP2STR_STL_MASK)
/* EP2TCR Bit Fields */
#define FSUSB_EP2TCR_CLR_MASK  0x1u
#define FSUSB_EP2TCR_CLR_SHIFT 0u
#define FSUSB_EP2TCR_CLR_WIDTH 1u
#define FSUSB_EP2TCR_CLR(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP2TCR_CLR_SHIFT)) & FSUSB_EP2TCR_CLR_MASK)
/* EP2SR Bit Fields */
#define FSUSB_EP2SR_TXBSY_MASK  0x1u
#define FSUSB_EP2SR_TXBSY_SHIFT 0u
#define FSUSB_EP2SR_TXBSY_WIDTH 1u
/* EP2TPR Bit Fields */
#define FSUSB_EP2TPR_LEN_MASK  0x7Fu
#define FSUSB_EP2TPR_LEN_SHIFT 0u
#define FSUSB_EP2TPR_LEN_WIDTH 7u
#define FSUSB_EP2TPR_LEN(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP2TPR_LEN_SHIFT)) & FSUSB_EP2TPR_LEN_MASK)
/* EP3STR Bit Fields */
#define FSUSB_EP3STR_STL_MASK  0x1u
#define FSUSB_EP3STR_STL_SHIFT 0u
#define FSUSB_EP3STR_STL_WIDTH 1u
#define FSUSB_EP3STR_STL(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP3STR_STL_SHIFT)) & FSUSB_EP3STR_STL_MASK)
/* EP3TCR Bit Fields */
#define FSUSB_EP3TCR_CLR_MASK  0x1u
#define FSUSB_EP3TCR_CLR_SHIFT 0u
#define FSUSB_EP3TCR_CLR_WIDTH 1u
#define FSUSB_EP3TCR_CLR(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP3TCR_CLR_SHIFT)) & FSUSB_EP3TCR_CLR_MASK)
/* EP3SR Bit Fields */
#define FSUSB_EP3SR_TXBSY_MASK  0x1u
#define FSUSB_EP3SR_TXBSY_SHIFT 0u
#define FSUSB_EP3SR_TXBSY_WIDTH 1u
/* EP3TPR Bit Fields */
#define FSUSB_EP3TPR_LEN_MASK  0x7Fu
#define FSUSB_EP3TPR_LEN_SHIFT 0u
#define FSUSB_EP3TPR_LEN_WIDTH 07u
#define FSUSB_EP3TPR_LEN(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP3TPR_LEN_SHIFT)) & FSUSB_EP3TPR_LEN_MASK)
/* EP4CR Bit Fields */
#define FSUSB_EP4CR_LAT_MASK  0x1u
#define FSUSB_EP4CR_LAT_SHIFT 0u
#define FSUSB_EP4CR_LAT_WIDTH 1u
#define FSUSB_EP4CR_LAT(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP4CR_LAT_SHIFT)) & FSUSB_EP4CR_LAT_MASK)
/* EP4STR Bit Fields */
#define FSUSB_EP4STR_STL_MASK  0x1u
#define FSUSB_EP4STR_STL_SHIFT 0u
#define FSUSB_EP4STR_STL_WIDTH 1u
#define FSUSB_EP4STR_STL(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP4STR_STL_SHIFT)) & FSUSB_EP4STR_STL_MASK)
/* EP4TCR Bit Fields */
#define FSUSB_EP4TCR_CLR_MASK  0x1u
#define FSUSB_EP4TCR_CLR_SHIFT 0u
#define FSUSB_EP4TCR_CLR_WIDTH 1u
#define FSUSB_EP4TCR_CLR(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP4TCR_CLR_SHIFT)) & FSUSB_EP4TCR_CLR_MASK)
/* EP4RPR Bit Fields */
#define FSUSB_EP4RPR_LEN_MASK  0x7Fu
#define FSUSB_EP4RPR_LEN_SHIFT 0u
#define FSUSB_EP4RPR_LEN_WIDTH 7u
/* SD0 Bit Fields */
#define FSUSB_SD0_BYTE_MASK  0xFFu
#define FSUSB_SD0_BYTE_SHIFT 0u
#define FSUSB_SD0_BYTE_WIDTH 8u
#define FSUSB_SD0_BYTE(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_SD0_BYTE_SHIFT)) & FSUSB_SD0_BYTE_MASK)
/* SD1 Bit Fields */
#define FSUSB_SD1_BYTE_MASK  0xFFu
#define FSUSB_SD1_BYTE_SHIFT 0u
#define FSUSB_SD1_BYTE_WIDTH 8u
#define FSUSB_SD1_BYTE(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_SD1_BYTE_SHIFT)) & FSUSB_SD1_BYTE_MASK)
/* SD2 Bit Fields */
#define FSUSB_SD2_BYTE_MASK  0xFFu
#define FSUSB_SD2_BYTE_SHIFT 0u
#define FSUSB_SD2_BYTE_WIDTH 8u
#define FSUSB_SD2_BYTE(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_SD2_BYTE_SHIFT)) & FSUSB_SD2_BYTE_MASK)
/* SD3 Bit Fields */
#define FSUSB_SD3_BYTE_MASK  0xFFu
#define FSUSB_SD3_BYTE_SHIFT 0u
#define FSUSB_SD3_BYTE_WIDTH 8u
#define FSUSB_SD3_BYTE(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_SD3_BYTE_SHIFT)) & FSUSB_SD3_BYTE_MASK)
/* SD4 Bit Fields */
#define FSUSB_SD4_BYTE_MASK  0xFFu
#define FSUSB_SD4_BYTE_SHIFT 0u
#define FSUSB_SD4_BYTE_WIDTH 8u
#define FSUSB_SD4_BYTE(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_SD4_BYTE_SHIFT)) & FSUSB_SD4_BYTE_MASK)
/* SD5 Bit Fields */
#define FSUSB_SD5_BYTE_MASK  0xFFu
#define FSUSB_SD5_BYTE_SHIFT 0u
#define FSUSB_SD5_BYTE_WIDTH 8u
#define FSUSB_SD5_BYTE(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_SD5_BYTE_SHIFT)) & FSUSB_SD5_BYTE_MASK)
/* SD6 Bit Fields */
#define FSUSB_SD6_BYTE_MASK  0xFFu
#define FSUSB_SD6_BYTE_SHIFT 0u
#define FSUSB_SD6_BYTE_WIDTH 8u
#define FSUSB_SD6_BYTE(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_SD6_BYTE_SHIFT)) & FSUSB_SD6_BYTE_MASK)
/* SD7 Bit Fields */
#define FSUSB_SD7_BYTE_MASK  0xFFu
#define FSUSB_SD7_BYTE_SHIFT 0u
#define FSUSB_SD7_BYTE_WIDTH 8u
#define FSUSB_SD7_BYTE(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_SD7_BYTE_SHIFT)) & FSUSB_SD7_BYTE_MASK)
/* EP56STR Bit Fields */
#define FSUSB_EP5STR_STL_MASK  0x1u
#define FSUSB_EP5STR_STL_SHIFT 0u
#define FSUSB_EP5STR_STL_WIDTH 1u
#define FSUSB_EP5STR_STL(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP5STR_STL_SHIFT)) & FSUSB_EP5STR_STL_MASK)
#define FSUSB_EP6STR_STL_MASK  0x100u
#define FSUSB_EP6STR_STL_SHIFT 8u
#define FSUSB_EP6STR_STL_WIDTH 1u
#define FSUSB_EP6STR_STL(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP6STR_STL_SHIFT)) & FSUSB_EP6STR_STL_MASK)
/* EP56TCR Bit Fields */
#define FSUSB_EP5TCR_CLR_MASK  0x1u
#define FSUSB_EP5TCR_CLR_SHIFT 0u
#define FSUSB_EP5TCR_CLR_WIDTH 1u
#define FSUSB_EP5TCR_CLR(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP5TCR_CLR_SHIFT)) & FSUSB_EP5TCR_CLR_MASK)
#define FSUSB_EP6TCR_CLR_MASK  0x100u
#define FSUSB_EP6TCR_CLR_SHIFT 8u
#define FSUSB_EP6TCR_CLR_WIDTH 1u
#define FSUSB_EP6TCR_CLR(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP6TCR_CLR_SHIFT)) & FSUSB_EP6TCR_CLR_MASK)
/* EP56CSR Bit Fields */
#define FSUSB_EP5SR_TXBSY_MASK  0x1u
#define FSUSB_EP5SR_TXBSY_SHIFT 0u
#define FSUSB_EP5SR_TXBSY_WIDTH 1u
#define FSUSB_EP6CR_LAT_MASK    0x100u
#define FSUSB_EP6CR_LAT_SHIFT   8u
#define FSUSB_EP6CR_LAT_WIDTH   1u
#define FSUSB_EP6CR_LAT(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP6CR_LAT_SHIFT)) & FSUSB_EP6CR_LAT_MASK)
/* EP56PR Bit Fields */
#define FSUSB_EP5PR_LEN_MASK  0x7Fu
#define FSUSB_EP5PR_LEN_SHIFT 0u
#define FSUSB_EP5PR_LEN_WIDTH 7u
#define FSUSB_EP5PR_LEN(x) \
    (((uint32_t)(((uint32_t)(x)) << FSUSB_EP5PR_LEN_SHIFT)) & FSUSB_EP5PR_LEN_MASK)
#define FSUSB_EP6PR_LEN_MASK  0x7F00u
#define FSUSB_EP6PR_LEN_SHIFT 8u
#define FSUSB_EP6PR_LEN_WIDTH 7u

/**
 * @}
 */ /* end of group FSUSB_Register_Masks */

/**
 * @}
 */ /* end of group FSUSB_Peripheral_Access_Layer */

/**
 * @}
 */ /* end of group Peripheral_access_layer_$(CMSIS_name) */

#ifdef __cplusplus
}
#endif

#endif /* $(CMSIS_nameUppercase)_H */

/*** end of file ***/
