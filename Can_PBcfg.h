/*
 * Can_PBcfg.h
 *
 *  Created on: Mar 8, 2020
 *      Author: Sherif_Khourshed
 */

#ifndef CAN_PBCFG_H_
#define CAN_PBCFG_H_

/* Id for our team in the AUTOSAR, for example ID = 88) */
//#define CAN_VENDOR_ID                  	(88U)

/* Module Id */
//#define CAN_MODULE_ID                  	(80U)

/* Can Instance Id */
//#define CAN_INSTANCE_ID  (0U)

/* Module Version 1.0.0 */
#define CAN_PBCFG_SW_MAJOR_VERSION				(1U)
#define CAN_PBCFG_SW_MINOR_VERSION           	(0U)
#define CAN_PBCFG_SW_PATCH_VERSION           	(0U)


/* AUTOSAR Version 4.3.1 */
#define CAN_PBCFG_AR_RELEASE_MAJOR_VERSION		(4U)
#define CAN_PBCFG_AR_RELEASE_MINOR_VERSION   	(3U)
#define CAN_PBCFG_AR_RELEASE_REVISION_VERSION 	(1U)

#include "Std_Types.h"
/* AUTOSAR Version checking between Std Types and Can_PBcfg Module */
#if(CAN_PBCFG_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION)\
		||(CAN_PBCFG_AR_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION)\
		||(CAN_PBCFG_AR_RELEASE_REVISION_VERSION != STD_TYPES_AR_RELEASE_REVISION_VERSION)
#error "The AR version of Std_Types.h does not match the expected version"
#endif



/*******************************************************************
                      PBcfg_Can_General_Container
 ******************************************************************/
/********************END_PBcfg_Can_General_Container*****************/


/*******************************************************************
                      PBcfg_Can_Controller_Container
 ******************************************************************/
/* Enables / disables API Can_MainFunction_BusOff() for handling busoff events in polling mode */
typedef enum
{
	INTERRUPT, POLLING
}CanBusoffProcessing;

/* Enables / disables API Can_MainFunction_Read() for handling PDU reception events in polling mode */
typedef enum
{
	INTERRUPT, POLLING, MIXED
}CanRxProcessing;

/* Enables / disables API Can_MainFunction_Write() for handling PDU transmission events in polling mode */
typedef enum
{
	INTERRUPT, POLLING, MIXED
}CanTxProcessing;

/* Enables / disables API Can_MainFunction_Wakeup() for handling wakeup events in polling mode */
typedef enum
{
	INTERRUPT, POLLING
}CanWakeupProcessing;

typedef struct
{
	/* Specifies the CAN controller base address */
	uint32 CanControllerBaseAddress;
	/* This parameter provides the controller ID which is unique in a given CAN Driver */
	/* The value for this parameter starts with 0 and continue without any gaps */
	uint8 CanControllerId;
	/*Reference to baudrate configuration container configured for the Can Controller. Reference to [ CanControllerBaudrateConfig ]*/
	CanControllerBaudrateConfig* CanControllerDefaultBaudrate;

	/* MCU Driver is missing */
	/* Reference to the CPU clock configuration, which is set in the MCU driver configuration */
	//ptr* CanCpuClockRef;
}CanController;
/********************END_PBcfg_Can_Controller_Container********************/


/*******************************************************************
                 PBcfg_Can_Controller_Baudrate_Config
 ******************************************************************/
typedef struct
{
	/* Specifies the baudrate of the controller in kbps */
	uint16 CanControllerBaudRate; /* Range min=0 to max=2000 */
	/* Uniquely identifies a specific baud rate configuration. This ID is used by SetBaudrate API */
	uint16 CanControllerBaudRateConfigID; /* Range min=0 to max=65535 */
	/* Specifies propagation delay in time quantas */
	uint8 CanControllerPropSeg; /* Range min=0 to max=255 */
	/* Specifies phase segment 1 in time quantas */
	uint8 CanControllerSeg1; /* Range min=0 to max=255 */
	/* Specifies phase segment 2 in time quantas */
	uint8 CanControllerSeg2; /* Range min=0 to max=255 */
	/* Specifies the synchronization jump width for the controller in time quantas */
	uint8 CanControllerSyncJumpWidth; /* Range min=0 to max=255 */
}CanControllerBaudrateConfig;
/********************END_PBcfg_Can_Controller_Baudrate_Config********************/


/*******************************************************************
                  PBcfg_Can_Hardware_Object
 ******************************************************************/
/* Specifies the type (Full-CAN or Basic-CAN) of a hardware object */
typedef enum
{
	/* BASIC: For several L-PDUs are hadled by the hardware object */
	/* FULL: For only one L-PDU (identifier) is handled by the hardware object */
	BASIC, FULL
}Can_HandleType;


/* Specifies whether the IdValue is of type */
typedef enum
{
	EXTENDED, MIXED, STANDARD
}CanIdType;


/* Specifies if the HardwareObject is used as Transmit or as Receive object */
typedef enum
{
	RECEIVE, TRANSMIT
}CanObjectType;

typedef struct
{
	/* Enables polling of this hardware object */
	boolean CanHardwareObjectUsesPolling;
	/* Number of hardware objects used to implement one HOH */
	/* HRH this parameter defines the number of elements in the hardware FIFO or the number of shadow buffers */
	/* HTH it defines the number of hardware objects used for multiplexed transmission or for a hardware FIFO used by a FullCAN HTH */
	uint16 CanHWObjectCount;
	/* Holds the handle ID of HRH or HTH. The value of this parameter is unique in a given CAN Driver */
	/* and it should start with 0 and continue without any gaps. The HRH and HTH Ids share a common ID range */
	/* Example: HRH0-0, HRH1-1, HTH0-2, HTH1-3 */
	Can_ObjectId CanIdType;
	/* This parameter defines if or if not Can supports the trigger-transmit API for this handle */
	boolean CanTriggerTransmitEnable;
	/* Reference to CAN Controller to which the HOH is associated to */
	CanController* CanControllerRef;
	/* Reference to CAN Controller to which the HOH is associated to */
	CanMainFunctionRWPeriods* CanMainFunctionRWPeriodRef;
}CanHardwareObject;
/********************END_PBcfg_Can_Hardware_Object********************/



/*******************************************************************
                     PBcfg_Can_Hardware_Filter
 ******************************************************************/
/* Specifies (together with the filter mask) the identifiers range that passes the hardware filter */
uint32 CanHwFilterCode;
/* Describes a mask for hardware-based filtering of CAN identifiers */
/* The CAN identifiers of incoming messages are masked with the appropriate CanFilterMaskValue */
/* Bits holding a 0 mean don't care, i.e. do not compare the message's identifier in the respective bit position */
/* The mask shall be build by filling with leading 0 */
/* In case of CanIdType  EXTENDED or MIXED a 29 bit mask shall be build */
/* In case of CanIdType  STANDARD a 11 bit mask shall be buil*/
/* dependency: The filter mask settings must be known by the CanIf configuration for optimization of the SW filters */
uint32 CanHwFilterMask;
/********************END_PBcfg_Can_Hardware_Filter********************/

/*******************************************************************
                     PBcfg_Can_Config_Set
 ******************************************************************/
typedef struct
{
	/*Reference to CAN Controller to which the HOH is associated to.*/
	CanController *CanControllerRef;
	/*This container contains the configuration (parameters) of CAN Hardware Objects. */
	CanHardwareObject *CanHardwareObjectRef;
} Can_ConfigType;
/********************END_PBcfg_Can_Config_Set********************/



/*******************************************************************
                  PBcfg_Can_Main_Function_RW_Periods
 ******************************************************************/
typedef struct
{
	float32 CanMainFunctionPeriod;
}CanMainFunctionRWPeriods;
/****************END_PBcfg_Can_Main_Function_RW_Periods****************/


#endif /* CAN_PBCFG_H_ */
