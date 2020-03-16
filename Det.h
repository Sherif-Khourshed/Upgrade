 /******************************************************************************
 *
 * Module: Det
 *
 * File Name: Det.h
 *
 * Description:  Det stores the development errors reported by other modules.
 *
 * Author: Ahmed Nasr
 ******************************************************************************/

#ifndef DET_H
#define DET_H

/* Id for our team in the AUTOSAR, for example ID = 88) */
#define DET_VENDOR_ID                 (88U)

/* Det Module Id */
#define DET_MODULE_ID                 (15U)

/* Instance Id */
#define DET_INSTANCE_ID               (0U)

/*
 * Det Software Module Version 1.0.0
 */
#define DET_SW_MAJOR_VERSION          (1U)
#define DET_SW_MINOR_VERSION          (0U)
#define DET_SW_PATCH_VERSION          (0U)

/*
 * AUTOSAR Version 4.3.1
 */
#define DET_AR_MAJOR_VERSION          (4U)
#define DET_AR_MINOR_VERSION          (3U)
#define DET_AR_PATCH_VERSION          (1U)

/* Standard AUTOSAR types */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and Det Modules */
#if ((DET_AR_MAJOR_VERSION != STD_TYPES_AR_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != STD_TYPES_AR_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != STD_TYPES_AR_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
void Det_ReportError( uint16 ModuleId,
                      uint8 InstanceId,
                      uint8 ApiId,
					  uint8 ErrorId );

#endif /* DET_H */
