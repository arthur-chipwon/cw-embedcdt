/**
 * @file rcm_driver.c
 * @brief This file provides access to the power manager.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stddef.h>
#include "rcm_access.h"

/*******************************************************************************
 * Global functions
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : rcm_get_reset_src_status
 * Description   : This function will get the current reset source status for
 *                  specified source
 *
 * Implements    : rcm_get_reset_src_status_activity
 *END**************************************************************************/
bool
rcm_get_reset_src_status(const RCM_t *const baseAddr, const rcm_source_names_t srcName)
{
    return rcm_get_src_status(baseAddr, srcName);
}

/*** end of file ***/
