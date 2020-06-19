/*
 * Can_cfg.h
 *
 *  Created on: Mar 8, 2020
 *      Author: Sherif_Khourshed
 */

#ifndef CAN_CFG_H_
#define CAN_CFG_H_

/* Id for our team in the AUTOSAR, for example ID = 88) */
#define CAN_VENDOR_ID                  	(88U)

/* Module Id */
//#define CAN_MODULE_ID                  	(80U)

/* Can Instance Id */
//#define CAN_INSTANCE_ID  (0U)

/* Module Version 1.0.0 */
#define CAN_CFG_SW_MAJOR_VERSION			(1U)
#define CAN_CFG_SW_MINOR_VERSION           	(0U)
#define CAN_CFG_SW_PATCH_VERSION           	(0U)


/* AUTOSAR Version 4.3.1 */
#define CAN_CFG_AR_RELEASE_MAJOR_VERSION	(4U)
#define CAN_CFG_AR_RELEASE_MINOR_VERSION   	(3U)
#define CAN_CFG_AR_RELEASE_REVISION_VERSION (1U)

#include "Can_PBcfg.h"
#include "Std_Types.h"
/* AUTOSAR Version checking between Std Types and Can_cfg Module */
#if(CAN_CFG_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION)\
	||(CAN_CFG_AR_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION)\
	||(CAN_CFG_AR_RELEASE_REVISION_VERSION != STD_TYPES_AR_RELEASE_REVISION_VERSION)
#error "The AR version of Std_Types.h does not match the expected version"
#endif

#define CAN_0_BASE_ADDRESS  ((uint32)0x40024000)
#define CAN_1_BASE_ADDRESS  ((uint32)0x40025000)
#define CAN_2_BASE_ADDRESS  ((uint32)0x4002B000)
#define CAN_0_ID            ((uint8)0x00)
#define CAN_1_ID            ((uint8)0x01)
#define CAN_2_ID            ((uint8)0x02)
#define CONTROLLERS_NUMBER  ((uint8)0x01)
#define CONTROLLER_0_BDR_ID ((uint8)0x00)
#define INTERRUPT           ((uint8)0x00)
#define MIXED               ((uint8)0x01)
#define POLLING             ((uint8)0x02)
#define CLOCK               (80000000)            /* Can Clock in Hz */

typedef float64 McuClockReferencePoint;
/*******************************************************************
                      cfg_Can_General_Container
 ******************************************************************/
/* Switches the development error detection and notification on or off */
#define CAN_DEV_ERROR_DETECT				(STD_ON)

/* Specifies the InstanceId of this module instance. If only one instance is present it shall have the Id 0 */
#define CAN_INDEX							((uint8)0x00)

/* This parameter defines the existence and the name of a callout function that is called after a successful */
/* This parameter defines the existence and the name of a callout function that is called after a successful */
#define CAN_LPDU_RECEIVE_CALLOUT_FUNCTION 	(/* Function_Name */)

/* This parameter describes the period for cyclic call to Can_MainFunction_Busoff. Unit is seconds */
#define CAN_MAIN_FUNCTION_BUSOFF_PERIOD	  	((float32)/* BusOff_Period */)

/* This parameter describes the period for cyclic call to Can_MainFunction_Mode. Unit is seconds */
#define CAN_MAIN_FUNCTION_MODE_PERIOD	  	((float32)/* Mode_Period */)

/* This parameter describes the period for cyclic call to Can_MainFunction_Wakeup. Unit is seconds */
#define CAN_MAIN_FUNCTION_WAKEUP_PERIOD	  	((float32)/* Wakeup_Period */)

/* Specifies if multiplexed transmission shall be supported.ON or OFF */
#define CAN_MULTIPLEXED_TRNSMISSION			(STD_OFF)

/* Selects support of Pretended Network features in Can driver */
#define CAN_PUBLIC_ICOM_SUPPORT 			(STD_OFF)

/* The support of the Can_SetBaudrate API is optional */
/* If this parameter is set to true the Can_SetBaudrate API shall be supported. Otherwise the API is not supported */
#define CAN_SET_BAUDRATE_API				(STD_ON)

/* Specifies the maximum time for blocking function until a timeout is detected. Unit is seconds */
#define CAN_TIMEOUT_DURATION				((float32)/* Timeout_Duration */)

/* Switches the Can_GetVersionInfo() API ON or OFF */
#define CAN_VERSION_INFO_API 				(STD_ON)

/* This parameter contains a reference to the OsCounter, which is used by the CAN driver */
#define CAN_OS_COUNTER_REF					(/* Reference_To_OS_Counter */)

/* The parameter refers to CanIfSupportTTCAN parameter in the CAN Interface Module configuration */
/* The CanIfSupportTTCAN parameter defines whether TTCAN is supported */
#define CAN_SUPPORT_TTCAN_REF				(/* Reference_to_[CanIfPrivateCfg] */)
/********************END_cfg_Can_General_Container********************/


/*******************************************************************
                      cfg_Can_Controller_Container
 ******************************************************************/
/* Defines if a CAN controller is used in the configuration */
#define CAN_CONTROLLER_ACTIVATION			(STD_ON)
/* Enables / disables API Can_MainFunction_BusOff() for handling busoff events in polling mode */
#define CanBusoffProcessing 				(INTERRUPT)
/* Enables / disables API Can_MainFunction_Read() for handling PDU reception events in polling mode */
#define CanRxProcessing 					(INTERRUPT)
/* Enables / disables API Can_MainFunction_Write() for handling PDU transmission events in polling mode */
#define CanTxProcessing 					(INTERRUPT)
/* Enables / disables API Can_MainFunction_Wakeup() for handling wakeup events in polling mode */
#define CanWakeupProcessing 				(INTERRUPT)

/* Adds / removes the service Can_CheckWakeup() from the code*/
/* True: Can_CheckWakeup can be used, False: Can_CheckWakeup cannot be used*/
#define CAN_WAKEUP_FUNCTIONALITY_API		(STD_OFF)

/* CAN driver support for wakeup over CAN Bus */
#define CAN_WAKEUP_SUPPORT					(STD_OFF)
/********************END_cfg_Can_Controller_Container********************/



/*******************************************************************
                  cfg_Can_Hardware_Object
 ******************************************************************/
/* Symbolic Names generated for this parameters */
typedef uint16 Can_ObjectId;
#define HRH0_0 ((Can_ObjectId)0x00U)
#define HTH0_0 ((Can_ObjectId)0x01U)
#define HRH0_1 ((Can_ObjectId)0x02U)
#define HTH0_1 ((Can_ObjectId)0x03U)
/********************END_cfg_Can_Hardware_Object********************/



#endif /* CAN_CFG_H_ */
