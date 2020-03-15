/*
 * Det.h
 *
 *  Created on: Feb 23, 2020
 *      Author: Sherif_Khourshed
 */

#ifndef DET_H_
#define DET_H_


/* Id for our team in the AUTOSAR, for example ID = 88) */
#define DET_VENDOR_ID 		              	(88U)

/* Module Id */
#define DET_MODULE_ID          	        	(15U)

/* Instance Id */
#define DET_INSTANCE_ID		                (0U)

/* Module Version 1.0.0 */
#define DET_SW_MAJOR_VERSION				(1U)
#define DET_SW_MINOR_VERSION 	          	(0U)
#define DET_SW_PATCH_VERSION    	       	(0U)


/* AUTOSAR Version 4.3.1 */
#define DET_AR_RELEASE_MAJOR_VERSION		(4U)
#define DET_AR_RELEASE_MINOR_VERSION 	  	(3U)
#define DET_AR_RELEASE_REVISION_VERSION   	(1U)

#include "Std_Types.h"
/* AUTOSAR Version checking between Std Types and Det Module */
#if(DET_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION)\
	||(DET_AR_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION)\
	||(DET_AR_RELEASE_REVISION_VERSION != STD_TYPES_AR_RELEASE_REVISION_VERSION)
#error "The AR version of Std_Types.h does not match the expected version"
#endif


/******************************************************************
                      Function Prototypes
 ******************************************************************/
/* To initialize the Development Error Tracer */
void Det_Init( void);

/* To report development errors */
Std_ReturnType Det_ReportError( uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId);

/* To start the Development Error Tracer */
void Det_Start( void);
#endif /* DET_H_ */
