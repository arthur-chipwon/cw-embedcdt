/**
 * @file devassert.h
 * @brief Header file for the DEV_ASSERT macro.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

#ifndef DEVASSERT_H
#define DEVASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdbool.h>

/*******************************************************************************
 * Macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
#if defined(CUSTOM_DEVASSERT)
/* If the CUSTOM_DEVASSERT symbol is defined, then add the custom implementation
 */
#include CUSTOM_DEVASSERT
#elif defined(DEV_ERROR_DETECT)
/* Implement default assert macro */
static inline void
DevAssert(volatile bool x)
{
    if (x)
    {
    }
    else
    {
        BKPT_ASM;
        for (;;)
        {
        }
    }
}
#define DEV_ASSERT(x) DevAssert(x)
#else
/** Assert macro does nothing */
#define DEV_ASSERT(x) ((void)0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* DEVASSERT_H */

/*** end of file ***/
