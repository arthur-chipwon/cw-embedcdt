/**
 * @file $(CMSIS_name)_features.h
 * @brief Chip specific module features.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef $(CMSIS_nameUppercase)_FEATURES_H
#define $(CMSIS_nameUppercase)_FEATURES_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/* SOC module features */
/* @brief PORT availability on the SoC. */
#define FEATURE_SOC_PORT_COUNT (5)
#define FEATURE_SOC_SCG_COUNT  (1)
/* @brief Slow IRC high range clock frequency. */
#define FEATURE_SCG_SIRC_HIGH_RANGE_FREQ (8000000U)
/* @brief Fast IRC trimmed clock frequency(48MHz). */
#define FEATURE_SCG_FIRC_FREQ0 (48000000U)
/* @brief Fast IRC trimmed clock frequency(60MHz). */
#define FEATURE_SCG_FIRC_FREQ1 (60000000U)
/* @brief VECTKEY value so that AIRCR register write is not ignored. */
#define FEATURE_SCB_VECTKEY (0x05FAU)

/* FLASH module features */
/* @brief P-Flash block count. */
#define FEATURE_FLS_PF_BLOCK_COUNT (1u)
/* @brief P-Flash block size. */
#define FEATURE_FLS_PF_BLOCK_SIZE (0x40000U)
/* @brief P-Flash sector size. */
#define FEATURE_FLS_PF_BLOCK_SECTOR_SIZE (1024u)
/* @brief P-Flash write unit size. */
#define FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE (8u)
/* @brief Has FlexRAM memory. */
#define FEATURE_FLS_HAS_FLEX_RAM (1u)
/* @brief FlexRAM size. */
#define FEATURE_FLS_FLEX_RAM_SIZE (512u)
/* @brief FlexRAM start address. (Valid only if FlexRAM is available.) */
#define FEATURE_FLS_FLEX_RAM_START_ADDRESS (0x14000000u)

/* @brief P-Flash Erase/Read 1st all block command address alignment. */
#define FEATURE_FLS_PF_BLOCK_CMD_ADDRESS_ALIGMENT (8u)
/* @brief P-Flash Erase sector command address alignment. */
#define FEATURE_FLS_PF_SECTOR_CMD_ADDRESS_ALIGMENT (8u)
/* @brief P-Flash Program/Verify section command address alignment. */
#define FEATURE_FLS_PF_SECTION_CMD_ADDRESS_ALIGMENT (8u)
/* @brief P-Flash Program check command address alignment. */
#define FEATURE_FLS_PF_CHECK_CMD_ADDRESS_ALIGMENT (4u)

/* SMC module features */

/* RCM module feature */
/* @brief Has existence of CMU loss of clock as reset source */
#define FEATURE_RCM_HAS_EXISTENCE_CMU_LOSS_OF_CLOCK (1)
/* @brief Has CMU loss of clock as reset source */
#define FEATURE_RCM_HAS_CMU_LOSS_OF_CLOCK (1)
/* @brief Has sticky CMU loss of clock as reset source */
#define FEATURE_RCM_HAS_STICKY_CMU_LOSS_OF_CLOCK (1)

/* WDOG module features */
/* @brief The 32-bit value used for unlocking the WDOG. */
#define FEATURE_WDOG_UNLOCK_VALUE (0xD928C520U)
/* @brief The 32-bit value used for resetting the WDOG counter. */
#define FEATURE_WDOG_TRIGGER_VALUE (0xB480A602U)
/* @brief The reset value of the timeout register. */
#define FEATURE_WDOG_TO_RESET_VALUE (0x400U)
/* @brief The value minimum of the timeout register. */
#define FEATURE_WDOG_MINIMUM_TIMEOUT_VALUE (0x0U)
/* @brief The reset value of the window register. */
#define FEATURE_WDOG_WIN_RESET_VALUE (0x0U)
/* @brief The mask of the reserved bit in the CS register. */
#define FEATURE_WDOG_CS_RESERVED_MASK (0x2000U)
/* @brief The value used to set WDOG source clock from LPO. */
#define FEATURE_WDOG_CLK_FROM_LPO (0x1UL)
/* @brief The first 16-bit value used for unlocking the WDOG. */
#define FEATURE_WDOG_UNLOCK16_FIRST_VALUE (0xC520U)
/* @brief The second 16-bit value used for unlocking the WDOG. */
#define FEATURE_WDOG_UNLOCK16_SECOND_VALUE (0xD928U)
/* @brief The first 16-bit value used for resetting the WDOG counter. */
#define FEATURE_WDOG_TRIGGER16_FIRST_VALUE (0xA602U)
/* @brief The second 16-bit value used for resetting the WDOG counter. */
#define FEATURE_WDOG_TRIGGER16_SECOND_VALUE (0xB480U)
/* @brief Default reset value of the CS register. */
#define FEATURE_WDOG_CS_RESET_VALUE (0x2520U)

/* Interrupt module features */
/* @brief Lowest interrupt request number. */
#define FEATURE_INTERRUPT_IRQ_MIN (NonMaskableInt_IRQn)
/* @brief Highest interrupt request number. */
#define FEATURE_INTERRUPT_IRQ_MAX (LPUART0_RxTx_IRQn)
/**< Number of priority bits implemented in the NVIC */
#define FEATURE_NVIC_PRIO_BITS (2U)
/* @brief Has pending interrupt state. */
#define FEATURE_INTERRUPT_HAS_PENDING_STATE (1u)

/* FTM module features */
/* @brief Number of PWM channels */
#define FEATURE_FTM_CHANNEL_COUNT (8U)
/* @brief Number of fault channels */
#define FTM_FEATURE_FAULT_CHANNELS (4U)
/* @brief Width of control channel */
#define FTM_FEATURE_COMBINE_CHAN_CTRL_WIDTH (8U)
/* @brief Output channel offset */
#define FTM_FEATURE_OUTPUT_CHANNEL_OFFSET (16U)
/* @brief Max counter value */
#define FTM_FEATURE_CNT_MAX_VALUE_U32 (0x0000FFFFU)
/* @brief Input capture for single shot */
#define FTM_FEATURE_INPUT_CAPTURE_SINGLE_SHOT (2U)
/* @brief Dithering has supported on the generated PWM signals */
#define FEATURE_FTM_HAS_SUPPORTED_DITHERING (1U)
/* @brief Number of interrupt vector for channels of the FTM module. */
#define FEATURE_FTM_HAS_NUM_IRQS_CHANS (1U)

/* LPIT module features */

/* LPI2C module features */
/* @brief DMA instance used for LPI2C module */
#define LPI2C_DMA_INSTANCE 0U
/* @brief EDMA requests for LPI2C module. */
#define LPI2C_EDMA_REQ                                                \
    {                                                                 \
        {                                                             \
            (uint8_t) EDMA_REQ_LPI2C0_TX, (uint8_t)EDMA_REQ_LPI2C0_RX \
        },                                                            \
        {                                                             \
            (uint8_t) EDMA_REQ_LPI2C1_TX, (uint8_t)EDMA_REQ_LPI2C1_RX \
        }                                                             \
    }
/* @brief Initial value for state structure */
#define FEATURE_LPI2C_STATE_STRUCTURES_NULL \
    {                                       \
        NULL, NULL                          \
    }

/* @brief Clock names for LPI2C. */
#define LPI2C_CLOCK_NAMES      \
    {                          \
        LPI2C0_CLK, LPI2C1_CLK \
    }
/* @brief Disable high-speed and ultra-fast operating modes. */
#define LPI2C_HAS_FAST_PLUS_MODE  (0U)
#define LPI2C_HAS_HIGH_SPEED_MODE (0U)
#define LPI2C_HAS_ULTRA_FAST_MODE (0U)

/* MSCM module features */

/* CRC module features */
/* @brief CRC module. */
#define FEATURE_CRC_DRIVER_SOFT_POLYNOMIAL
/* @brief Default CRC bit width */
#define FEATURE_CRC_DEFAULT_WIDTH CRC_BITS_16
/* @brief Default CRC read transpose */
#define FEATURE_CRC_DEFAULT_READ_TRANSPOSE CRC_TRANSPOSE_NONE
/* @brief Default CRC write transpose */
#define FEATURE_CRC_DEFAULT_WRITE_TRANSPOSE CRC_TRANSPOSE_NONE
/* @brief Default polynomial 0x1021U */
#define FEATURE_CRC_DEFAULT_POLYNOMIAL (0x1021U)
/* @brief Default seed value is 0xFFFFU */
#define FEATURE_CRC_DEFAULT_SEED (0xFFFFU)

/* PORT module features */
/* @brief Supports LPO peripheral clock source. */
#define FEATURE_HAS_LPO_PERIPHERAL_CLOCK_SOURCE (0U)

typedef enum
{
    /* Main clocks */
    CORE_CLK   = 0u, /**< Core clock                     */
    BUS_CLK    = 1u, /**< Bus clock                      */
    SLOW_CLK   = 2u, /**< Slow clock                     */
    CLKOUT_CLK = 3u, /**< CLKOUT clock                   */

    /* Other internal clocks used by peripherals. */
    SIRC_CLK          = 4u,  /**< SIRC clock                     */
    FIRC_CLK          = 5u,  /**< FIRC clock                     */
    SOSC_CLK          = 6u,  /**< SOSC clock                     */
    RTC_CLKIN_CLK     = 8u,  /**< RTC_CLKIN clock                */
    SCG_CLKOUT_CLK    = 9u,  /**< SCG CLK_OUT clock              */
    SIRCDIV1_CLK      = 10u, /**< SIRCDIV1 functional clock      */
    SIRCDIV2_CLK      = 11u, /**< SIRCDIV2 functional clock      */
    FIRCDIV1_CLK      = 12u, /**< FIRCDIV1 functional clock      */
    FIRCDIV2_CLK      = 13u, /**< FIRCDIV2 functional clock      */
    SOSCDIV1_CLK      = 14u, /**< SOSCDIV1 functional clock      */
    SOSCDIV2_CLK      = 15u, /**< SOSCDIV2 functional clock      */
    SCG_END_OF_CLOCKS = 18u, /**< End of SCG clocks              */

    /* SIM clocks */
    SIM_FTM0_CLOCKSEL = 21u, /**< FTM0 External Clock Pin Select */
    SIM_FTM1_CLOCKSEL = 22u, /**< FTM1 External Clock Pin Select */
    SIM_CLKOUTSELL    = 23u, /**< CLKOUT Select                  */
    SIM_RTCCLK_CLK    = 24u, /**< RTCCLK clock                   */
    SIM_LPO_CLK       = 25u, /**< LPO clock                      */
    SIM_LPO_1K_CLK    = 26u, /**< LPO 1KHz clock                 */
    SIM_LPO_32K_CLK   = 27u, /**< LPO 32KHz clock                */
    SIM_LPO_128K_CLK  = 28u, /**< LPO 128KHz clock               */
    SIM_EIM_CLK       = 29u, /**< EIM clock source               */
    SIM_ERM_CLK       = 30u, /**< ERM clock source               */
    SIM_DMA_CLK       = 31u, /**< DMA clock source               */
    SIM_MPU_CLK       = 32u, /**< MPU clock source               */
    SIM_MSCM_CLK      = 33u, /**< MSCM clock source              */
    SIM_END_OF_CLOCKS = 34u, /**< End of SIM clocks              */

    /* PCC clocks */
    CMP0_CLK                      = 41u, /**< CMP0 clock source           */
    CRC0_CLK                      = 42u, /**< CRC0 clock source           */
    DMAMUX0_CLK                   = 43u, /**< DMAMUX0 clock source        */
    PORTA_CLK                     = 44u, /**< PORTA clock source          */
    PORTB_CLK                     = 45u, /**< PORTB clock source          */
    PORTC_CLK                     = 46u, /**< PORTC clock source          */
    PORTD_CLK                     = 47u, /**< PORTD clock source          */
    PORTE_CLK                     = 48u, /**< PORTE clock source          */
    RTC0_CLK                      = 49u, /**< RTC0 clock source           */
    PCC_END_OF_BUS_CLOCKS         = 50u, /**< End of BUS clocks           */
    FlexCAN0_CLK                  = 51u, /**< FlexCAN0 clock source       */
    PDB0_CLK                      = 52u, /**< PDB0 clock source           */
    PCC_END_OF_SYS_CLOCKS         = 53u, /**< End of SYS clocks           */
    FTFC0_CLK                     = 54u, /**< FTFC0 clock source          */
    PCC_END_OF_SLOW_CLOCKS        = 55u, /**< End of SLOW clocks          */
    FTM0_CLK                      = 56u, /**< FTM0 clock source           */
    FTM1_CLK                      = 57u, /**< FTM1 clock source           */
    PCC_END_OF_ASYNCH_DIV1_CLOCKS = 58u, /**< End of ASYNCH DIV1 clocks   */
    ADC0_CLK                      = 59u, /**< ADC0 clock source           */
    FLEXIO0_CLK                   = 60u, /**< FLEXIO0 clock source        */
    LPI2C0_CLK                    = 61u, /**< LPI2C0 clock source         */
    LPI2C1_CLK                    = 62u, /**< LPI2C1 clock source         */
    LPIT0_CLK                     = 63u, /**< LPIT0 clock source          */
    LPSPI0_CLK                    = 64u, /**< LPSPI0 clock source         */
    LPSPI1_CLK                    = 65u, /**< LPSPI1 clock source         */
    LPSPI2_CLK                    = 66u, /**< LPSPI2 clock source         */
    LPTMR0_CLK                    = 67u, /**< LPTMR0 clock source         */
    LPUART0_CLK                   = 68u, /**< LPUART0 clock source        */
    LPUART1_CLK                   = 69u, /**< LPUART1 clock source        */
    FSUSB_CLK                     = 70u, /**< FSUSB clock source          */
    PUF_CLK                       = 71u, /**< PUF clock source            */
    HSSPI_CLK                     = 72u, /**< HSSPI clock source          */
    PCC_END_OF_ASYNCH_DIV2_CLOCKS = 73u, /**< End of ASYNCH DIV2 clocks   */
    PCC_END_OF_CLOCKS             = 74u, /**< End of PCC clocks           */
    CLOCK_NAME_COUNT              = 75u, /**< The total number of entries */
} clock_names_t;

#define PCC_INVALID_INDEX 0

#define PCC_CLOCK_NAME_MAPPINGS                                           \
    {                                                                     \
        PCC_INVALID_INDEX,     /**< Core clock                      0  */ \
            PCC_INVALID_INDEX, /**< Bus clock                       1  */ \
            PCC_INVALID_INDEX, /**< Slow clock                      2  */ \
            PCC_INVALID_INDEX, /**< CLKOUT clock                    3  */ \
            PCC_INVALID_INDEX, /**< SIRC clock                      4  */ \
            PCC_INVALID_INDEX, /**< FIRC clock                      5  */ \
            PCC_INVALID_INDEX, /**< SOSC clock                      6  */ \
            PCC_INVALID_INDEX, /**< No clock entry in clock_names_t 7  */ \
            PCC_INVALID_INDEX, /**< RTC_CLKIN clock                 8  */ \
            PCC_INVALID_INDEX, /**< SCG CLK_OUT clock               9  */ \
            PCC_INVALID_INDEX, /**< SIRCDIV1 functional clock       10 */ \
            PCC_INVALID_INDEX, /**< SIRCDIV2 functional clock       11 */ \
            PCC_INVALID_INDEX, /**< FIRCDIV1 functional clock       12 */ \
            PCC_INVALID_INDEX, /**< FIRCDIV2 functional clock       13 */ \
            PCC_INVALID_INDEX, /**< SOSCDIV1 functional clock       14 */ \
            PCC_INVALID_INDEX, /**< SOSCDIV2 functional clock       15 */ \
            PCC_INVALID_INDEX, /**< No clock entry in clock_names_t 16 */ \
            PCC_INVALID_INDEX, /**< No clock entry in clock_names_t 17 */ \
            PCC_INVALID_INDEX, /**< End of SCG clocks               18 */ \
            PCC_INVALID_INDEX, /**< No clock entry in clock_names_t 19 */ \
            PCC_INVALID_INDEX, /**< No clock entry in clock_names_t 20 */ \
            PCC_INVALID_INDEX, /**< FTM0 External Clock Pin Select  21 */ \
            PCC_INVALID_INDEX, /**< FTM1 External Clock Pin Select  22 */ \
            PCC_INVALID_INDEX, /**< CLKOUT Select                   23 */ \
            PCC_INVALID_INDEX, /**< CLK32K clock                    24 */ \
            PCC_INVALID_INDEX, /**< LPO clock                       25 */ \
            PCC_INVALID_INDEX, /**< LPO 1KHz clock                  26 */ \
            PCC_INVALID_INDEX, /**< LPO 32KHz clock                 27 */ \
            PCC_INVALID_INDEX, /**< LPO 128KHz clock                28 */ \
            PCC_INVALID_INDEX, /**< EIM clock source                29 */ \
            PCC_INVALID_INDEX, /**< ERM clock source                30 */ \
            PCC_INVALID_INDEX, /**< DMA clock source                31 */ \
            PCC_INVALID_INDEX, /**< MPU clock source                32 */ \
            PCC_INVALID_INDEX, /**< MSCM clock source               33 */ \
            PCC_INVALID_INDEX, /**< No clock entry in clock_names_t 34 */ \
            PCC_INVALID_INDEX, /**< No clock entry in clock_names_t 35 */ \
            PCC_INVALID_INDEX, /**< No clock entry in clock_names_t 36 */ \
            PCC_INVALID_INDEX, /**< No clock entry in clock_names_t 37 */ \
            PCC_INVALID_INDEX, /**< No clock entry in clock_names_t 38 */ \
            PCC_INVALID_INDEX, /**< No clock entry in clock_names_t 39 */ \
            PCC_INVALID_INDEX, /**< No clock entry in clock_names_t 40 */ \
            PCC_INVALID_INDEX, /**< CMP0 clock source               41 */ \
            PCC_INVALID_INDEX, /**< CRC clock source                42 */ \
            PCC_INVALID_INDEX, /**< DMAMUX clock source             43 */ \
            PCC_INVALID_INDEX, /**< PORTA clock source              44 */ \
            PCC_INVALID_INDEX, /**< PORTB clock source              45 */ \
            PCC_INVALID_INDEX, /**< PORTC clock source              46 */ \
            PCC_INVALID_INDEX, /**< PORTD clock source              47 */ \
            PCC_INVALID_INDEX, /**< PORTE clock source              48 */ \
            PCC_INVALID_INDEX, /**< RTC clock source                49 */ \
            PCC_INVALID_INDEX, /**< End of BUS clocks               50 */ \
            PCC_INVALID_INDEX, /**< FlexCAN0 clock source           51 */ \
            PCC_INVALID_INDEX, /**< PDB0 clock source               52 */ \
            PCC_INVALID_INDEX, /**< End of SYS clocks               53 */ \
            PCC_INVALID_INDEX, /**< FTFC clock source               54 */ \
            PCC_INVALID_INDEX, /**< End of SLOW clocks              55 */ \
            PCC_FTM0_INDEX,    /**< FTM0 clock source               56 */ \
            PCC_FTM1_INDEX,    /**< FTM1 clock source               57 */ \
            PCC_INVALID_INDEX, /**< End of ASYNCH DIV1 clocks       58 */ \
            PCC_INVALID_INDEX, /**< ADC0 clock source               59 */ \
            PCC_INVALID_INDEX, /**< FlexIO clock source             60 */ \
            PCC_LPI2C0_INDEX,  /**< LPI2C0 clock source             61 */ \
            PCC_LPI2C1_INDEX,  /**< LPI2C1 clock source             62 */ \
            PCC_INVALID_INDEX, /**< LPIT clock source               63 */ \
            PCC_LPSPI0_INDEX,  /**< LPSPI0 clock source             64 */ \
            PCC_LPSPI1_INDEX,  /**< LPSPI1 clock source             65 */ \
            PCC_LPSPI2_INDEX,  /**< LPSPI2 clock source             66 */ \
            PCC_LPTMR0_INDEX,  /**< LPTMR0 clock source             67 */ \
            PCC_LPUART0_INDEX, /**< LPUART0 clock source            68 */ \
            PCC_LPUART1_INDEX, /**< LPUART1 clock source            69 */ \
            PCC_FSUSB_INDEX,   /**< USB clock source                70 */ \
            PCC_PUF_INDEX,     /**< PUF clock source                71 */ \
            PCC_HSSPI_INDEX,   /**< HSSPI clock source              72 */ \
            PCC_INVALID_INDEX, /**< End of ASYNCH DIV2 clocks       73 */ \
            PCC_INVALID_INDEX, /**< End of PCC clocks               74 */ \
    }

/* It's not a peripheral instance, there is no peripheral feature. */
#define NO_PERIPHERAL_FEATURE          (0U)
#define HAS_CLOCK_GATING_IN_SIM        (1U << 0U) /* Clock gating is implemented in SIM */
#define HAS_MULTIPLIER                 (1U << 1U) /* Multiplier is implemented in PCC */
#define HAS_DIVIDER                    (1U << 2U) /* Divider is implemented in PCC */
#define HAS_PROTOCOL_CLOCK_FROM_ASYNC1 (1U << 3U) /* Provided by the first asynchronous clock. */
#define HAS_PROTOCOL_CLOCK_FROM_ASYNC2 (1U << 4U) /* Provided by the second asynchronous clock. */
#define HAS_INT_CLOCK_FROM_BUS_CLOCK   (1U << 5U) /* Interface is provided by the bus clock. */
#define HAS_INT_CLOCK_FROM_SYS_CLOCK   (1U << 6U) /* Interface is provided by the sys clock. */
#define HAS_INT_CLOCK_FROM_SLOW_CLOCK  (1U << 7U) /* Interface is provided by the slow clock. */

#define PERIPHERAL_FEATURES                                                           \
    {                                                                                 \
        (NO_PERIPHERAL_FEATURE),            /**< Core clock        0  */              \
            (NO_PERIPHERAL_FEATURE),        /**< Bus clock         1  */              \
            (NO_PERIPHERAL_FEATURE),        /**< Slow clock        2  */              \
            (NO_PERIPHERAL_FEATURE),        /**< CLKOUT clock      3  */              \
            (NO_PERIPHERAL_FEATURE),        /**< SIRC clock        4  */              \
            (NO_PERIPHERAL_FEATURE),        /**< FIRC clock        5  */              \
            (NO_PERIPHERAL_FEATURE),        /**< SOSC clock        6  */              \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 7 */ \
            (NO_PERIPHERAL_FEATURE),        /**< RTC_CLKIN clock                 8 */ \
            (NO_PERIPHERAL_FEATURE),        /**< SCG CLK_OUT clock               9 */ \
            (NO_PERIPHERAL_FEATURE),        /**< End of SCG clocks 10 */              \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 11*/ \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 12*/ \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 13*/ \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 14*/ \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 15*/ \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 16*/ \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 17*/ \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 18*/ \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 19*/ \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 20*/ \
            (NO_PERIPHERAL_FEATURE),        /**< FTM0 External Clock Pin Select  21*/ \
            (NO_PERIPHERAL_FEATURE),        /**< FTM1 External Clock Pin Select  22*/ \
            (NO_PERIPHERAL_FEATURE),        /**< CLKOUT Select 23 */                  \
            (NO_PERIPHERAL_FEATURE),        /**< CLK32K clock 24 */                   \
            (NO_PERIPHERAL_FEATURE),        /**< LPO clock 25 */                      \
            (NO_PERIPHERAL_FEATURE),        /**< LPO 1KHz clock 26 */                 \
            (NO_PERIPHERAL_FEATURE),        /**< LPO 32KHz clock 27 */                \
            (NO_PERIPHERAL_FEATURE),        /**< LPO 128KHz clock 28 */               \
            (NO_PERIPHERAL_FEATURE),        /**< EIM clock source 29 */               \
            (NO_PERIPHERAL_FEATURE),        /**< ERM clock source 30 */               \
            (NO_PERIPHERAL_FEATURE),        /**< DMA clock source 31 */               \
            (NO_PERIPHERAL_FEATURE),        /**< MPU clock source 32 */               \
            (NO_PERIPHERAL_FEATURE),        /**< MSCM clock source 33 */              \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 34*/ \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 35*/ \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 36*/ \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 37*/ \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 38*/ \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 39*/ \
            (NO_PERIPHERAL_FEATURE),        /**< No clock entry in clock_names_t 40*/ \
            (NO_PERIPHERAL_FEATURE),        /**< CMP0 clock source 41 */              \
            (NO_PERIPHERAL_FEATURE),        /**< CRC clock source 42 */               \
            (NO_PERIPHERAL_FEATURE),        /**< DMAMUX clock source 43 */            \
            (NO_PERIPHERAL_FEATURE),        /**< PORTA clock source 44 */             \
            (NO_PERIPHERAL_FEATURE),        /**< PORTB clock source 45 */             \
            (NO_PERIPHERAL_FEATURE),        /**< PORTC clock source 46 */             \
            (NO_PERIPHERAL_FEATURE),        /**< PORTD clock source 47 */             \
            (NO_PERIPHERAL_FEATURE),        /**< PORTE clock source 48 */             \
            (NO_PERIPHERAL_FEATURE),        /**< RTC clock source 49 */               \
            (NO_PERIPHERAL_FEATURE),        /**< End of BUS clocks 50 */              \
            (NO_PERIPHERAL_FEATURE),        /**< FlexCAN0 clock source 51 */          \
            (NO_PERIPHERAL_FEATURE),        /**< PDB0 clock source 52 */              \
            (NO_PERIPHERAL_FEATURE),        /**< End of SYS clocks 53 */              \
            (NO_PERIPHERAL_FEATURE),        /**< FTFC clock source 54 */              \
            (NO_PERIPHERAL_FEATURE),        /**< End of SLOW clocks 55 */             \
            (HAS_PROTOCOL_CLOCK_FROM_ASYNC1 |                                         \
             HAS_INT_CLOCK_FROM_SYS_CLOCK), /**< FTM0 clock source 56 */              \
            (HAS_PROTOCOL_CLOCK_FROM_ASYNC1 |                                         \
             HAS_INT_CLOCK_FROM_SYS_CLOCK), /**< FTM1 clock source 57 */              \
            (NO_PERIPHERAL_FEATURE),        /**< End of ASYNCH DIV1 clocks 58 */      \
            (NO_PERIPHERAL_FEATURE),        /**< ADC0 clock source 59 */              \
            (NO_PERIPHERAL_FEATURE),        /**< FLEXIO clock source 60 */            \
            (HAS_PROTOCOL_CLOCK_FROM_ASYNC2 |                                         \
             HAS_INT_CLOCK_FROM_BUS_CLOCK), /**< LPI2C0 clock source 61 */            \
            (HAS_PROTOCOL_CLOCK_FROM_ASYNC2 |                                         \
             HAS_INT_CLOCK_FROM_BUS_CLOCK), /**< LPI2C1 clock source 62 */            \
            (NO_PERIPHERAL_FEATURE),        /**< LPIT clock source 63 */              \
            (HAS_PROTOCOL_CLOCK_FROM_ASYNC2 |                                         \
             HAS_INT_CLOCK_FROM_BUS_CLOCK), /**< LPSPI0 clock source 64 */            \
            (HAS_PROTOCOL_CLOCK_FROM_ASYNC2 |                                         \
             HAS_INT_CLOCK_FROM_BUS_CLOCK), /**< LPSPI1 clock source 65 */            \
            (HAS_PROTOCOL_CLOCK_FROM_ASYNC2 |                                         \
             HAS_INT_CLOCK_FROM_BUS_CLOCK), /**< LPSPI2 clock source 66 */            \
            (HAS_MULTIPLIER | HAS_DIVIDER | HAS_PROTOCOL_CLOCK_FROM_ASYNC2 |          \
             HAS_INT_CLOCK_FROM_BUS_CLOCK), /**< LPTMR0 clock source 67 */            \
            (HAS_PROTOCOL_CLOCK_FROM_ASYNC2 |                                         \
             HAS_INT_CLOCK_FROM_BUS_CLOCK), /**< LPUART0 clock source 68*/            \
            (HAS_PROTOCOL_CLOCK_FROM_ASYNC2 |                                         \
             HAS_INT_CLOCK_FROM_BUS_CLOCK), /**< LPUART1 clock source 69*/            \
            (HAS_INT_CLOCK_FROM_SYS_CLOCK), /**< FSUSB clock source 70*/              \
            (HAS_INT_CLOCK_FROM_SYS_CLOCK), /**< PUF clock source 71*/                \
            (HAS_INT_CLOCK_FROM_SYS_CLOCK), /**< HSSPI clock source 72*/              \
            (NO_PERIPHERAL_FEATURE),        /**< End of ASYNCH DIV2 clocks 73 */      \
            (NO_PERIPHERAL_FEATURE),        /**< End of PCC clocks 74 */              \
    }

/* Time to wait for SIRC to stabilize */
#define SIRC_STABILIZATION_TIMEOUT 100U
/* Time to wait for FIRC to stabilize */
#define FIRC_STABILIZATION_TIMEOUT 100U
/* Time to wait for SOSC to stabilize */
#define SOSC_STABILIZATION_TIMEOUT 3205000U;
/* Time to wait for SPLL to stabilize */
#define SPLL_STABILIZATION_TIMEOUT 1000U;

#define TMP_SIRC_CLK 0U
#define TMP_FIRC_CLK 1U
#define TMP_SOSC_CLK 2U
#define TMP_SPLL_CLK 3U

#define TMP_SYS_DIV  0U
#define TMP_SLOW_DIV 1U

#define TMP_SYS_DIV_NO 2U
#define TMP_SYS_CLK_NO 4U

#define TMP_SYSTEM_CLOCK_CONFIGS                                                              \
    { /*       SYS_CLK           SLOW_CLK */                                                  \
        {SCG_SYSTEM_CLOCK_DIV_BY_1, SCG_SYSTEM_CLOCK_DIV_BY_2},     /**< Dividers for SIRC */ \
            {SCG_SYSTEM_CLOCK_DIV_BY_1, SCG_SYSTEM_CLOCK_DIV_BY_4}, /**< Dividers for FIRC */ \
            {SCG_SYSTEM_CLOCK_DIV_BY_1, SCG_SYSTEM_CLOCK_DIV_BY_2}, /**< Dividers for SOSC */ \
            {SCG_SYSTEM_CLOCK_DIV_BY_3, SCG_SYSTEM_CLOCK_DIV_BY_2}, /**< Dividers for SPLL */ \
    }

/* @brief template system clock configuration in VLPR mode*/
#define FEATURE_VLPR_SYS_CLK  SCG_SYSTEM_CLOCK_DIV_BY_2
#define FEATURE_VLPR_SLOW_CLK SCG_SYSTEM_CLOCK_DIV_BY_4

/* DMA module features */
/* @brief Number of DMA channels. */
#define FEATURE_DMA_CHANNELS (4U)
/* @brief Number of DMA virtual channels. */
#define FEATURE_DMA_VIRTUAL_CHANNELS (FEATURE_DMA_CHANNELS * DMA_INSTANCE_COUNT)
/* @brief Number of DMA interrupt lines. */
#define FEATURE_DMA_CHANNELS_INTERRUPT_LINES (4U)
/* @brief Number of DMA virtual interrupt lines. */
#define FEATURE_DMA_VIRTUAL_CHANNELS_INTERRUPT_LINES \
    ((uint32_t)FEATURE_DMA_CHANNELS_INTERRUPT_LINES * (uint32_t)DMA_INSTANCE_COUNT)
/* @brief Number of DMA error interrupt lines. */
#define FEATURE_DMA_ERROR_INTERRUPT_LINES (1U)
/* @brief Number of DMA virtual error interrupt lines. */
#define FEATURE_DMA_VIRTUAL_ERROR_INTERRUPT_LINES \
    ((uint32_t)FEATURE_DMA_ERROR_INTERRUPT_LINES * (uint32_t)DMA_INSTANCE_COUNT)
/* @brief Conversion from channel index to DCHPRI index. */
#define FEATURE_DMA_CHN_TO_DCHPRI_INDEX(x) ((x) ^ 3U)
/* @brief DMA channel groups count. */
#define FEATURE_DMA_CHANNEL_GROUP_COUNT (1U)
/* @brief Clock name for DMA */
#define FEATURE_DMA_CLOCK_NAMES \
    {                           \
        SIM_DMA_CLK             \
    }
/* @brief DMA channel width based on number of TCDs: 2^N, N=4,5,... */
#define FEATURE_DMA_CH_WIDTH (4U)
/* @brief DMA channel to instance */
#define FEATURE_DMA_VCH_TO_INSTANCE(x) ((x) >> (uint32_t)FEATURE_DMA_CH_WIDTH)
/* @brief DMA virtual channel to channel */
#define FEATURE_DMA_VCH_TO_CH(x) ((x) & ((uint32_t)FEATURE_DMA_CHANNELS - 1U))
/* @brief DMA supports the following particular channel priorities: */
#define FEATURE_DMA_4_CH_PRIORITIES
/* @brief DMA supports bus bandwidth control. */
#define FEATURE_DMA_ENGINE_STALL

/* DMAMUX module features */
/* @brief Number of DMA channels. */
#define FEATURE_DMAMUX_CHANNELS (4U)
/* @brief Conversion from request source to the actual DMAMUX channel */
#define FEATURE_DMAMUX_REQ_SRC_TO_CH(x) (x)
/* @brief Mapping between request source and DMAMUX instance */
#define FEATURE_DMAMUX_REQ_SRC_TO_INSTANCE(x) (0U)
/* @brief Conversion from eDMA channel index to DMAMUX channel. */
#define FEATURE_DMAMUX_DMA_CH_TO_CH(x) (x)
/* @brief Conversion from DMAMUX channel DMAMUX register index. */
#define FEATURE_DMAMUX_CHN_REG_INDEX(x) (x)
/* @brief Clock names for DMAMUX. */
#define FEATURE_DMAMUX_CLOCK_NAMES \
    {                              \
        DMAMUX0_CLK                \
    }

typedef enum
{
    EDMA_REQ_DISABLED               = 0U,
    EDMA_REQ_LPUART0_RX             = 2U,
    EDMA_REQ_LPUART0_TX             = 3U,
    EDMA_REQ_LPUART1_RX             = 4U,
    EDMA_REQ_LPUART1_TX             = 5U,
    EDMA_REQ_LPUART2_RX             = 6U,
    EDMA_REQ_LPUART2_TX             = 7U,
    EDMA_REQ_LPI2C1_RX              = 8U,
    EDMA_REQ_LPI2C1_TX              = 9U,
    EDMA_REQ_LPSPI0_RX              = 14U,
    EDMA_REQ_LPSPI0_TX              = 15U,
    EDMA_REQ_LPSPI1_RX              = 16U,
    EDMA_REQ_LPSPI1_TX              = 17U,
    EDMA_REQ_LPSPI2_RX              = 18U,
    EDMA_REQ_LPSPI2_TX              = 19U,
    EDMA_REQ_FTM1_CHANNEL_0         = 20U,
    EDMA_REQ_FTM1_CHANNEL_1         = 21U,
    EDMA_REQ_FTM1_CHANNEL_2         = 22U,
    EDMA_REQ_FTM1_CHANNEL_3         = 23U,
    EDMA_REQ_FTM1_CHANNEL_4         = 24U,
    EDMA_REQ_FTM1_CHANNEL_5         = 25U,
    EDMA_REQ_FTM1_CHANNEL_6         = 26U,
    EDMA_REQ_FTM1_CHANNEL_7         = 27U,
    EDMA_REQ_FTM0_OR_CH0_CH7        = 36U,
    EDMA_REQ_LPI2C0_RX              = 44U,
    EDMA_REQ_LPI2C0_TX              = 45U,
    EDMA_REQ_PORTA                  = 49U,
    EDMA_REQ_PORTB                  = 50U,
    EDMA_REQ_PORTC                  = 51U,
    EDMA_REQ_PORTD                  = 52U,
    EDMA_REQ_PORTE                  = 53U,
    EDMA_REQ_LPTMR0                 = 59U,
    EDMA_REQ_DMAMUX_ALWAYS_ENABLED0 = 62U,
    EDMA_REQ_DMAMUX_ALWAYS_ENABLED1 = 63U
} dma_request_source_t;

/* TRGMUX module features */
enum trgmux_trigger_source_e
{
    TRGMUX_TRIG_SOURCE_DISABLED       = 0U,
    TRGMUX_TRIG_SOURCE_VDD            = 1U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN0     = 2U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN1     = 3U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN2     = 4U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN3     = 5U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN4     = 6U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN5     = 7U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN6     = 8U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN7     = 9U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN8     = 10U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN9     = 11U,
    TRGMUX_TRIG_SOURCE_LPTMR0         = 21U,
    TRGMUX_TRIG_SOURCE_FTM0_INIT_TRIG = 22U,
    TRGMUX_TRIG_SOURCE_FTM0_EXT_TRIG  = 23U,
    TRGMUX_TRIG_SOURCE_FTM1_INIT_TRIG = 24U,
    TRGMUX_TRIG_SOURCE_FTM1_EXT_TRIG  = 25U,
};

enum trgmux_target_module_e
{
    TRGMUX_TARGET_MODULE_DMA_CH0      = 0U,
    TRGMUX_TARGET_MODULE_DMA_CH1      = 1U,
    TRGMUX_TARGET_MODULE_DMA_CH2      = 2U,
    TRGMUX_TARGET_MODULE_DMA_CH3      = 3U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT0  = 4U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT1  = 5U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT2  = 6U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT3  = 7U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT4  = 8U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT5  = 9U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT6  = 10U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT7  = 11U,
    TRGMUX_TARGET_MODULE_FTM0_HWTRIG0 = 40U,
    TRGMUX_TARGET_MODULE_FTM0_FAULT0  = 41U,
    TRGMUX_TARGET_MODULE_FTM0_FAULT1  = 42U,
    TRGMUX_TARGET_MODULE_FTM0_FAULT2  = 43U,
    TRGMUX_TARGET_MODULE_FTM1_HWTRIG0 = 44U,
    TRGMUX_TARGET_MODULE_FTM1_FAULT0  = 45U,
    TRGMUX_TARGET_MODULE_FTM1_FAULT1  = 46U,
    TRGMUX_TARGET_MODULE_FTM1_FAULT2  = 47U,
    TRGMUX_TARGET_MODULE_LPUART2_TRG  = 72U,
    TRGMUX_TARGET_MODULE_LPUART0_TRG  = 76U,
    TRGMUX_TARGET_MODULE_LPUART1_TRG  = 80U,
};

#define FEATURE_TRGMUX_TARGET_MODULE                                                              \
    {                                                                                             \
        TRGMUX_TARGET_MODULE_DMA_CH0, TRGMUX_TARGET_MODULE_DMA_CH1, TRGMUX_TARGET_MODULE_DMA_CH2, \
            TRGMUX_TARGET_MODULE_DMA_CH3, TRGMUX_TARGET_MODULE_TRGMUX_OUT0,                       \
            TRGMUX_TARGET_MODULE_TRGMUX_OUT1, TRGMUX_TARGET_MODULE_TRGMUX_OUT2,                   \
            TRGMUX_TARGET_MODULE_TRGMUX_OUT3, TRGMUX_TARGET_MODULE_TRGMUX_OUT4,                   \
            TRGMUX_TARGET_MODULE_TRGMUX_OUT5, TRGMUX_TARGET_MODULE_TRGMUX_OUT6,                   \
            TRGMUX_TARGET_MODULE_TRGMUX_OUT7, TRGMUX_TARGET_MODULE_FTM0_HWTRIG0,                  \
            TRGMUX_TARGET_MODULE_FTM0_FAULT0, TRGMUX_TARGET_MODULE_FTM0_FAULT1,                   \
            TRGMUX_TARGET_MODULE_FTM0_FAULT2, TRGMUX_TARGET_MODULE_FTM1_HWTRIG0,                  \
            TRGMUX_TARGET_MODULE_FTM1_FAULT0, TRGMUX_TARGET_MODULE_FTM1_FAULT1,                   \
            TRGMUX_TARGET_MODULE_FTM1_FAULT2, TRGMUX_TARGET_MODULE_LPUART2_TRG,                   \
            TRGMUX_TARGET_MODULE_LPUART0_TRG, TRGMUX_TARGET_MODULE_LPUART1_TRG                    \
    }

/* LPSPI module features */
/* @brief Initial value for state structure */
#define FEATURE_LPSPI_STATE_STRUCTURES_NULL \
    {                                       \
        NULL, NULL, NULL                    \
    }
/* @brief Clock indexes for LPSPI clock */
#define FEATURE_LPSPI_CLOCKS_NAMES {LPSPI0_CLK, LPSPI1_CLK, LPSPI2_CLK};
/* @brief Clock names for LPUART. */
#define LPSPI_CLOCK_NAMES                  \
    {                                      \
        LPSPI0_CLK, LPSPI1_CLK, LPSPI2_CLK \
    }

/* @brief Flag clearance mask for STAT register. */
#define FEATURE_LPUART_STAT_REG_FLAGS_MASK (0xC01FC000U)
/* @brief Flag clearance mask for FIFO register. */
#define FEATURE_LPUART_FIFO_REG_FLAGS_MASK (0x00030000U)
/* @brief Reset mask for FIFO register. */
#define FEATURE_LPUART_FIFO_RESET_MASK (0x0003C000U)
/* @brief Default oversampling ratio. */
#define FEATURE_LPUART_DEFAULT_OSR (0x0FUL)
/* @brief Default baud rate modulo divisor. */
#define FEATURE_LPUART_DEFAULT_SBR (0x04UL)
/* @brief Clock names for LPUART. */
#define LPUART_CLOCK_NAMES       \
    {                            \
        LPUART0_CLK, LPUART1_CLK \
    }

/* LPTMR module features */
/* @brief LPTMR pulse counter input options */
#define FEATURE_LPTMR_HAS_INPUT_ALT1_SELECTION (1U)

/* OSIF module features */
/* PDB module features */

/* FSUSB module features */
/* @brief Clock names for FSUSB. */
#define FSUSB_CLOCK_NAMES \
    {                     \
        FSUSB_CLK         \
    }

/* HSSPI module features */
/* @brief Clock names for FSUSB. */
#define HSSPI_CLOCK_NAMES \
    {                     \
        HSSPI_CLK         \
    }

#ifdef __cplusplus
}
#endif

#endif /* $(CMSIS_nameUppercase)_FEATURES_H */

/*** end of file ***/
