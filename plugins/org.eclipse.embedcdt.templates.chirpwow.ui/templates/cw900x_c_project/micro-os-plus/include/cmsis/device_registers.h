/**
 * @file device_registers.h
 * @brief Include related header files about this MCU device/platform/board,
 * defined from CMSIS-Core Device Templates.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef DEVICE_REGISTERS_H
#define DEVICE_REGISTERS_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
/* Specific core definitions */
#include "core_cm0.h"
/* MCU device/platform/board specific feature definitions */
#include "cw900x_features.h"
/* Register definitions */
#include "cw900x.h"
/* Debug assertion */
#include "devassert.h"

#ifdef __cplusplus
}
#endif

#endif /* DEVICE_REGISTERS_H */

/*** end of file ***/
