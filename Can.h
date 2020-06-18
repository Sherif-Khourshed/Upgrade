/*
 * Can.h
 *
 *  Created on: Mar 8, 2020
 *      Author: Sherif_Khourshed
 */

#ifndef CAN_H_
#define CAN_H_

/* Id for our team in the AUTOSAR, for example ID = 88) */
#define CAN_VENDOR_ID                  	(88U)

/* Module Id */
#define CAN_MODULE_ID                  	(80U)

/* Can Instance Id */
#define CAN_INSTANCE_ID  (0U)

/* Module Version 1.0.0 */
#define CAN_SW_MAJOR_VERSION			(1U)
#define CAN_SW_MINOR_VERSION           	(0U)
#define CAN_SW_PATCH_VERSION           	(0U)


/* AUTOSAR Version 4.3.1 */
#define CAN_AR_RELEASE_MAJOR_VERSION	(4U)
#define CAN_AR_RELEASE_MINOR_VERSION   	(3U)
#define CAN_AR_RELEASE_REVISION_VERSION (1U)

#include "Can_GeneralTypes.h"
/* AUTOSAR Version checking between Can_GeneralTypes and Can Module */
#if(CAN_GENERALTYPES_AR_RELEASE_MAJOR_VERSION != CAN_AR_RELEASE_MAJOR_VERSION)\
	||(CAN_GENERALTYPES_AR_RELEASE_MINOR_VERSION != CAN_AR_RELEASE_MINOR_VERSION)\
	||(CAN_GENERALTYPES_AR_RELEASE_REVISION_VERSION != CAN_AR_RELEASE_REVISION_VERSION)
#error "The AR version of Std_Types.h does not match the expected version"
#endif

#include "ComStack_types.h"
/* AUTOSAR Version checking between ComStack_types and Can Module */
#if(COMSTACK_TYPES_AR_RELEASE_MAJOR_VERSION != CAN_AR_RELEASE_MAJOR_VERSION)\
	||(COMSTACK_TYPES_AR_RELEASE_MINOR_VERSION != CAN_AR_RELEASE_MINOR_VERSION)\
	||(COMSTACK_TYPES_AR_RELEASE_REVISION_VERSION != CAN_AR_RELEASE_REVISION_VERSION)
#error "The AR version of Std_Types.h does not match the expected version"
#endif

#include "Can_cfg.h"
/* AUTOSAR Version checking between Can_cfg and Can Module */
#if(CAN_CFG_AR_RELEASE_MAJOR_VERSION != CAN_AR_RELEASE_MAJOR_VERSION)\
	||(CAN_CFG_AR_RELEASE_MINOR_VERSION != CAN_AR_RELEASE_MINOR_VERSION)\
	||(CAN_CFG_AR_RELEASE_REVISION_VERSION != CAN_AR_RELEASE_REVISION_VERSION)
#error "The AR version of Std_Types.h does not match the expected version"
#endif

#include "Can_PBcfg.h"
/* AUTOSAR Version checking between Can_PBcfg and Can Module */
#if(CAN_PBCFG_AR_RELEASE_MAJOR_VERSION != CAN_AR_RELEASE_MAJOR_VERSION)\
	||(CAN_PBCFG_AR_RELEASE_MINOR_VERSION != CAN_AR_RELEASE_MINOR_VERSION)\
	||(CAN_PBCFG_AR_RELEASE_REVISION_VERSION != CAN_AR_RELEASE_REVISION_VERSION)
#error "The AR version of Std_Types.h does not match the expected version"
#endif

#define CAN_BUSY 							(Std_ReturnType)0x02U
/*******************************************************************
                       API_Service_Id_Macros
 ******************************************************************/
typedef uint8 Can_ServiceId;

/* Service ID for Can_Init */
#define Can_Init_SID						((Can_ServiceId)0x00)
/* Service ID for Can_GetVersionInfo */
#define Can_GetVersionInfo_SID				((Can_ServiceId)0x07)
/* Service ID for Can_DeInit */
#define Can_DeInit_SID						((Can_ServiceId)0x10)
/* Service ID for Can_SetBaudrate */
#define Can_SetBaudrate_SID					((Can_ServiceId)0x0f)
/* Service ID for Can_SetControllerMode */
#define Can_SetControllerMode_SID			((Can_ServiceId)0x03)
/* Service ID for Can_DisableControllerInterrupts */
#define Can_DisableControllerInterrupts_SID	((Can_ServiceId)0x04)
/* Service ID for Can_EnableControllerInterrupts */
#define Can_EnableControllerInterrupts_SID	((Can_ServiceId)0x05)
/* Service ID for Can_CheckWakeup */
#define Can_CheckWakeup_SID					((Can_ServiceId)0x05)
/* Service ID for Can_GetControllerErrorState */
#define Can_GetControllerErrorState_SID		((Can_ServiceId)0x11)
/* Service ID for Can_GetControllerMode */
#define Can_GetControllerMode_SID			((Can_ServiceId)0x12)
/* Service ID for Can_Write */
#define Can_Write_SID						((Can_ServiceId)0x06)
/* Service ID for Can_MainFunction_Write */
#define Can_MainFunction_Write_SID			((Can_ServiceId)0x01)
/* Service ID for Can_MainFunction_Read */
#define Can_MainFunction_Read_SID			((Can_ServiceId)0x08)
/* Service ID for Can_MainFunction_BusOff */
#define Can_MainFunction_BusOff_SID			((Can_ServiceId)0x09)
/* Service ID for Can_MainFunction_Wakeup  */
#define Can_MainFunction_Wakeup_SID			((Can_ServiceId)0x0a)
/* Service ID for Can_MainFunction_Mode  */
#define Can_MainFunction_Mode_SID			((Can_ServiceId)0x0c)
/********************END_API_Service_Id_Macros********************/


/*******************************************************************
                       API_Can_DetError_Id_Macros
 ******************************************************************/
typedef uint8 Can_DetErrorType;

#define CAN_E_PARAM_POINTER              ((Can_DetErrorType)0x01)
#define CAN_E_PARAM_HANDLE               ((Can_DetErrorType)0x02)
#define CAN_E_PARAM_DATA_LENGTH          ((Can_DetErrorType)0x03)
#define CAN_E_PARAM_CONTROLLER           ((Can_DetErrorType)0x04)
#define CAN_E_UNINIT                     ((Can_DetErrorType)0x05)
#define CAN_E_TRANSITION                 ((Can_DetErrorType)0x06)
#define CAN_E_PARAM_BAUDRATE             ((Can_DetErrorType)0x07)
#define CAN_E_ICOM_CONFIG_INVALID        ((Can_DetErrorType)0x08)
#define CAN_E_INIT_FAILED                ((Can_DetErrorType)0x09)
/********************END_API_Can_DetError_Id_Macros********************/


/******************************************************************
                      Function Prototypes
 ******************************************************************/
/********* Services affecting the complete hardware unit *********/

/* This function initializes the module */
void Can_Init(const Can_ConfigType* Config);

/* This function returns the version information of this module */
void Can_GetVersionInfo(Std_VersionInfoType* versioninfo);

/* This function de-initializes the module */
void Can_DeInit(void);
/******************************************************************/

/********* Services affecting one single CAN Controller *********/

/* This service shall set the baud rate configuration of the CAN controller */
/* Depending on necessary baud rate modifications the controller might have to reset */
Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigI );

/* This function performs software triggered state transitions of the CAN controller State machine */
Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition);

/* This function disables all interrupts for this CAN controller */
void Can_DisableControllerInterrupts(uint8 Controller);

/* This function enables all allowed interrupts */
void Can_EnableControllerInterrupts(uint8 Controller);

/* This function checks if a wakeup has occurred for the given controller */
Std_ReturnType Can_CheckWakeup(uint8 Controller);

/* This service obtains the error state of the CAN controller */
Std_ReturnType Can_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr);

/* This service reports about the current status of the requested CAN controller */
Std_ReturnType Can_GetControllerMode(uint8 Controller, Can_ControllerStateType* ControllerModePtr);
/******************************************************************/

/********* Services affecting a Hardware Handle *********/

/* This function is called by CanIf to pass a CAN message to CanDrv for transmission */
Std_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo);
/******************************************************************/

/********* Scheduled functions *********/

/* This function performs the polling of TX confirmation when CAN_TX_PROCESSING is set to POLLING */
/*  The Can module may implement the function Can_MainFunction_Write as empty define in case no polling at all is used */
void Can_MainFunction_Write(void);

/* This function performs the polling of RX indications when CAN_RX_PROCESSING is set to POLLING */
/* The Can module may implement the function Can_MainFunction_Read as empty define in case no polling at all is used */
void Can_MainFunction_Read(void);

/* This function performs the polling of bus-off events that are configured statically as 'to be polled' */
/* The Can module may implement the function Can_MainFunction_BusOff as empty define in case no polling at all is used */
void Can_MainFunction_BusOff(void);

/* This function performs the polling of wake-up events that are configured statically as 'to be polled' */
/* The Can module may implement the function Can_MainFunction_Wakeup as empty define in case no polling at all is used */
void Can_MainFunction_Wakeup(void);

/* This function performs the polling of CAN controller mode transitions */
void Can_MainFunction_Mode(void);

#endif /* CAN_H_ */
