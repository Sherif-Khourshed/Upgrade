/*
 * Can_GeneralTypes.h
 *
 *  Created on: Mar 7, 2020
 *      Author: Sherif_Khourshed
 */

#ifndef CAN_GENERALTYPES_H_
#define CAN_GENERALTYPES_H_

/* Id for our team in the AUTOSAR, for example ID = 88) */
#define CAN_GENERALTYPES_VENDOR_ID                  	(88U)

/* Module Id */
//#define CAN_GENERALTYPES_MODULE_ID                  	(80U)

/* Module Version 1.0.0 */
#define CAN_GENERALTYPES_SW_MAJOR_VERSION				(1U)
#define CAN_GENERALTYPES_SW_MINOR_VERSION           	(0U)
#define CAN_GENERALTYPES_SW_PATCH_VERSION           	(0U)


/* AUTOSAR Version 4.3.1 */
#define CAN_GENERALTYPES_AR_MAJOR_VERSION		(4U)
#define CAN_GENERALTYPES_AR_MINOR_VERSION   	(3U)
#define CAN_GENERALTYPES_AR_PATCH_VERSION   	(1U)

#include "Std_Types.h"
/* AUTOSAR Version checking between Std Types and CanGeneralTypes */
#if(CAN_GENERALTYPES_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION)\
	||(CAN_GENERALTYPES_AR_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION)\
	||(CAN_GENERALTYPES_AR_RELEASE_REVISION_VERSION != STD_TYPES_AR_RELEASE_REVISION_VERSION)
#error "The AR version of Std_Types.h does not match the expected version"
#endif

/*
Standard32Bit--0..0x400007FF
Extended32Bit--0..0xDFFFFFFF
The two most significant bits specify the frame type:
00 CAN message with Standard CAN ID
01 CAN FD frame with Standard CAN ID
10 CAN message with Extended CAN ID
11 CAN FD frame with Extended CAN ID
*/
typedef uint32 Can_IdType;

/*
Standard--0..0x0FF
Extended--0..0xFFFF
Represents the hardware object handles of a CAN hardware unit.
For CAN hardware units with more than 255 HW objects use extended range.
*/
/* typedef uint8 Can_HwHandleType; */ /* For CAN hardware units with less than 255 HW objects  */
typedef uint16 Can_HwHandleType;  /* For CAN hardware units with more than 255 HW objects use extended range */


/* PduId (swPduHandle), SduLength (length), SduData (sdu), and CanId (id) for any CAN L-SDU */
typedef struct
{
	PduIdType	swPduHandle;
	uint8		length;
	Can_IdType	id;
	uint8*		sdu;
}Can_PduType;


/* Hardware Object Handle including its corresponding CAN Controller, CanDrv and the specific CanId */
typedef struct
{
	Can_IdType			CanId;			/* Standard/Extended CAN ID of CAN L-PDU */
	Can_HwHandleType	Hoh;			/* ID of the corresponding Hardware Object Range */
	uint8				ControllerId;	/* ControllerId provided by CanIf clearly identify the corresponding controller */

}Can_HwType;


/* Error states of a CAN controller */
typedef enum
{
	CAN_ERRORSTATE_ACTIVE,		/* The CAN controller takes fully part in communication */
	CAN_ERRORSTATE_PASSIVE,		/* The CAN controller takes part in communication, but does not send active error frames */
	CAN_ERRORSTATE_BUSOFF,		/* The CAN controller does not take part in communication */
}Can_ErrorStateType;

/* States that are used by the several ControllerMode functions */
typedef enum
{
	CAN_CS_UNINIT = 0x00,	/* CAN controller state UNINIT */
	CAN_CS_STARTED = 0x01,	/* CAN controller state STARTED */
	CAN_CS_STOPPED = 0x02,	/* CAN controller state STOPPED */
	CAN_CS_SLEEP = 0x03,	/* CAN controller state SLEEP */
}Can_ControllerStateType;

typedef enum
{
	CAN_BUSY = 0x02,
}Can_ReturnType;

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
#endif /* CAN_GENERALTYPES_H_ */
