/**
 * @file rcm_access.h
 * @brief Header file for the RCM.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef RCM_ACCESS_H
#define RCM_ACCESS_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "device_registers.h"
#include "status.h"

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
/**
 * @brief System Reset Source Name definitions
 *
 * Implements    : rcm_source_names_t_Class
 */
typedef enum
{
    RCM_LOW_VOLT_DETECT = 1U,  /**< Low voltage detect reset */
    RCM_WATCH_DOG       = 5U,  /**< Watch dog reset */
    RCM_EXTERNAL_PIN    = 6U,  /**< External pin reset */
    RCM_POWER_ON        = 7U,  /**< Power on reset */
    RCM_CORE_LOCKUP     = 9U,  /**< core lockup reset */
    RCM_SOFTWARE        = 10U, /**< Software reset */
    RCM_SRC_NAME_MAX
} rcm_source_names_t;

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
/**
 * @brief Gets the reset source status
 *
 * This function gets the current reset source status for a specified source.
 *
 * @param[in] baseAddr     Register base address of RCM
 * @param[in] srcName      reset source name
 * @return status          True or false for specified reset source
 */
bool rcm_get_reset_src_status(const RCM_t *const baseAddr, const rcm_source_names_t srcName);

/*******************************************************************************
 * Static functions
 ******************************************************************************/
/**
 * @brief Gets the reset source status
 *
 * This function will get the current reset source status for specified source.
 *
 * @param[in] baseAddr     Register base address of RCM
 * @param[in] srcName      reset source name
 * @return status          True or false for specified reset source
 */
static inline bool
rcm_get_src_status(const RCM_t *const baseAddr, const rcm_source_names_t srcName)
{
    bool     retValue;
    uint32_t regValue = (uint32_t)baseAddr->SRS;

    DEV_ASSERT(srcName < RCM_SRC_NAME_MAX);

    switch (srcName)
    {
        case RCM_LOW_VOLT_DETECT: /* low voltage detect reset */
            regValue = (regValue & RCM_SRS_LVD_MASK) >> RCM_SRS_LVD_SHIFT;
            break;
        case RCM_WATCH_DOG: /* watch dog reset */
            regValue = (regValue & RCM_SRS_WDOG_MASK) >> RCM_SRS_WDOG_SHIFT;
            break;
        case RCM_EXTERNAL_PIN: /* external pin reset */
            regValue = (regValue & RCM_SRS_PIN_MASK) >> RCM_SRS_PIN_SHIFT;
            break;
        case RCM_POWER_ON: /* power on reset */
            regValue = (regValue & RCM_SRS_POR_MASK) >> RCM_SRS_POR_SHIFT;
            break;
        case RCM_CORE_LOCKUP: /* core lockup reset */
            regValue = (regValue & RCM_SRS_LOCKUP_MASK) >> RCM_SRS_LOCKUP_SHIFT;
            break;
        case RCM_SOFTWARE: /* software reset */
            regValue = (regValue & RCM_SRS_SW_MASK) >> RCM_SRS_SW_SHIFT;
            break;
        default:
            /* invalid command */
            regValue = 0U;
            break;
    }

    retValue = (regValue == 0UL) ? false : true;

    return retValue;
}


#ifdef __cplusplus
}
#endif

#endif /* RCM_ACCESS_H */

/*** end of file ***/
