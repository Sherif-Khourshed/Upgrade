#ifndef CAN_GENERAL_TYPES_H
#define CAN_GENERAL_TYPES_H

#include "ComStack_cfg.h"
/* Published information */
#define CAN_GENERAL_TYPES_MODULE_ID (80U)
#define CAN_GENERAL_TYPES_VENDOR_ID (88U)   

#define CAN_GENERAL_TYPES_AR_RELEASE_MAJOR_VERSION (4U)
#define CAN_GENERAL_TYPES_AR_RELEASE_MINOR_VERSION (3U)
#define CAN_GENERAL_TYPES_AR_RELEASE_REVISION_VERSION (1U)

#define CAN_GENERAL_TYPES_SW_MAJOR_VERSION (1U)
#define CAN_GENERAL_TYPES_SW_MINOR_VERSION (0U)
#define CAN_GENERAL_TYPES_SW_PATCH_VERSION (0U)

/* AUTOSAR checking between Std Types and Can_GeneralTypes Module */
#if(CAN_GENERAL_TYPES_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION)\
	||(CAN_GENERAL_TYPES_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION)\
	||(CAN_GENERAL_TYPES_AR_RELEASE_REVISION_VERSION != STD_AR_RELEASE_REVISION_VERSION)
#error "The AR version of Std_Types.h does not match the expected version"
#endif

/* This File Shall be included by the Can.h file according to [SWS_Can_00034] */


/* [SWS_Can_00416]Can_IdType: Represents the Identifier of an L-PDU. The two most significant bits specify the frame type*/
typedef uint32 Can_IdType;



/* [SWS_Can_00415]Can_PduType: This type unites PduId (swPduHandle), SduLength (length), 
SduData (sdu), and CanId (id) for any CAN L-SDU*/
typedef struct
{
	PduIdType swPduHandle;
	uint8 length;
	Can_IdType id;
	uint8* sdu;
}Can_PduType;


/* [SWS_Can_00429]Can_HwHandleType: Represents the hardware object handles of a CAN hardware unit. 
For CAN hardware units with more than 255 HW objects use extended range*/
typedef uint8 Can_HwHandleType;

/* [SWS_CAN_00496]Can_HwType: This type defines a data structure which clearly provides an 
Hardware Object Handle including its corresponding CAN Controller and therefore CanDrv as 
well as the specific CanId*/
typedef struct
{
	Can_IdType CanId;           /*Standard/Extended CAN ID of CAN LPDU*/
	Can_HwHandleType Hoh;       /*ID of the corresponding Hardware Object Range*/
	uint8 ControllerId;         /*ControllerId provided by CanIf clearly identify the corresponding controller*/	
}Can_HwType;

/* [SWS_Can_91003]Can_ErrorStateType: Error states of a CAN controller*/ 
typedef enum 
{
	CAN_ERRORSTATE_ACTIVE,
	CAN_ERRORSTATE_PASSIVE,
	CAN_ERRORSTATE_BUSOFF
}Can_ErrorStateType;

/* [SWS_Can_91013]Can_ControllerStateType: States that are used by the several ControllerMode functions*/
typedef enum
{
	CAN_CS_UNINIT,
	CAN_CS_STARTED,
	CAN_CS_STOPPED,
	CAN_CS_SLEEP
}Can_ControllerStateType;

/* Operating modes of the CAN Transceiver Driver */
typedef enum
{
	CANTRCV_TRCVMODE_NORMAL = 0x00,	/* Transceiver mode NORMAL */
	CANTRCV_TRCVMODE_SLEEP,			/* Transceiver mode SLEEP */
	CANTRCV_TRCVMODE_STANDBY,		/* Transceiver mode STANDBY */
}CanTrcv_TrcvModeType;

/* This type shall be used to control the CAN transceiver concerning wake up events and wake up notifications */
typedef enum
{
	CANTRCV_WUMODE_ENABLE = 0x00,	/* The notification for wakeup events is enabled on the addressed transceiver */
	CANTRCV_WUMODE_DISABLE = 0x01,	/* The notification for wakeup events is disabled on the addressed transceiver */
	CANTRCV_WUMODE_CLEAR = 0x02,	/* A stored wakeup event is cleared on the ad-dressed transceiver */
}CanTrcv_TrcvWakeupModeType;


typedef enum
{
	/* Due to an error wake up reason was not de-tected. This value may only be reported when error was reported to DEM before */
	CANTRCV_WU_ERROR = 0x00,
	CANTRCV_WU_NOT_SUPPORTED = 0x01, /* The transceiver does not support any infor-mation for the wake up reason */
	CANTRCV_WU_BY_BUS = 0x02,	   	 /* The transceiver has detected, that the network has caused the wake up of the ECU */
	CANTRCV_WU_INTERNALLY = 0x03,	 /* The transceiver has detected, thatnetwork has woken up by the ECU via a request to NORMAL mode*/
	CANTRCV_WU_RESET = 0x04,		 /* The transceiver has detected, that the "wake up" is due to an ECU reset */
	CANTRCV_WU_POWER_ON = 0x05,		 /* The transceiver has detected, that the "wake up" is due to an ECU reset after power on */
	CANTRCV_WU_BY_PIN = 0x06,		 /* The transceiver has detected a wake-up event at one of the transceiver's pins */
	CANTRCV_WU_BY_SYSERR = 0x07,	 /* The transceiver has detected, that the wake up of the ECU was caused by a HW related device failure*/
}CanTrcv_TrcvWakeupReasonType;

#endif      /* CAN_GENERAL_TYPES_H */
