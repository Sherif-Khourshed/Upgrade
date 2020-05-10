/*
 * Std_Types.h
 *
 *  Created on: Feb 22, 2020
 *      Author: Sherif_Khourshed
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

#include "Platform_Types.h"
#include "Compiler.h"

/* Id for our team in the AUTOSAR, for example ID = 88) */
#define STDTYPES_VENDOR_ID                  (88U)

/* Module Version 1.0.0 */
#define STD_TYPES_SW_MAJOR_VERSION			(1U)
#define STD_TYPES_SW_MINOR_VERSION           (0U)
#define STD_TYPES_SW_PATCH_VERSION           (0U)


/* AUTOSAR Version 4.3.1 */
#define STD_TYPES_AR_MAJOR_VERSION	(4U)
#define STD_TYPES_AR_MINOR_VERSION   (3U)
#define STD_TYPES_AR_PATCH_VERSION   (1U)

/* Standard API return type which is shared between the RTE and the BSW modules */
typedef uint8 Std_ReturnType;

/* Structure for version of a BSW module,
 request by calling the <Module name>_GetVersionInfo() function */
typedef struct
{
	uint16 vendorID;
	uint16 moduleID;
	uint8 sw_major_version;
	uint8 sw_minor_version;
	uint8 sw_patch_version;
}Std_VersionInfoType;

#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED
#define E_OK 		((Std_ReturnType)0x00U) 	 /* Function returns OK */
typedef unsigned char StatusType;				 /* OSEK compliance */
#endif
#define E_NOT_OK	((Std_ReturnType)0x01U)		 /* Function returns NOT OK */

#define STD_HIGH	 0x01U						 /* Physical state 5V or 3.3V */
#define STD_LOW 	 0x00U						 /* Physical state 0V */

#define STD_ACTIVE 	 0x01U						 /* Logical state active */
#define STD_IDLE 	 0x00U						 /* Logical state idle */

#define STD_ON 		 0x01U 						 /* Standard ON */
#define STD_OFF 	 0x00U						 /* Standard OFF */

#endif /* STD_TYPES_H_ */
