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

/*******************************************************************
                       API Service Id Macros
 ******************************************************************/
/* Service ID for Can_Init */
#define Can_Init_SID						(uint8)0x00
/* Service ID for Can_GetVersionInfo */
#define Can_GetVersionInfo_SID				(uint8)0x07
/* Service ID for Can_DeInit */
#define Can_DeInit_SID						(uint8)0x10
/* Service ID for Can_SetBaudrate */
#define Can_SetBaudrate_SID					(uint8)0x0f
/* Service ID for Can_SetControllerMode */
#define Can_SetControllerMode_SID			(uint8)0x03
/* Service ID for Can_DisableControllerInterrupts */
#define Can_DisableControllerInterrupts_SID	(uint8)0x04
/* Service ID for Can_EnableControllerInterrupts */
#define Can_EnableControllerInterrupts_SID	(uint8)0x05
/* Service ID for Can_CheckWakeup */
#define Can_CheckWakeup_SID					(uint8)0x05
/* Service ID for Can_GetControllerErrorState */
#define Can_GetControllerErrorState_SID		(uint8)0x11
/* Service ID for Can_GetControllerMode */
#define Can_GetControllerMode_SID			(uint8)0x12
/* Service ID for Can_Write */
#define Can_Write_SID						(uint8)0x06

/******************************************************************
                 Some Definitions related to HW
 ******************************************************************/
#define HW_Register(Add) (*(volatile uint32*)(Add))

#define MCR     0x00                     /*Module Configuration register*/
#define CTRL1   0x04
#define TIMER   0x08                     /*free running timer */
#define ECR     0x1C                     /*Error Counter*/
#define ESR1    0x20                     /*Error status register*/
#define IMASK1  0x28                     /*interrupt flag 1*/
#define IFLAG1  0x30                     /*interrupt flag 1*/
#define CTRL2   0x34                     /*control register 2*/
#define RXFIR   0x4C                     /*RX FIFO INFORMATION */
#define CBT     0x50                     /*CAN BIT TIMMING*/

/******************************************************************
                 	   Some bits in MCR
 ******************************************************************/
/* (Reset Value =  D890_000Fh = 1101 1000 1001 0000 0000 0000 0000 1111b) */
#define MDIS      31                  /* Module Disable */
#define FRZ       30
#define RFEN      29                  /*RX fifo enable*/
#define HALT      28
#define NOTRDY    27
#define SoftReset 25
#define FRZACK    24
#define SUPV      23
#define WRNEN     21
#define LPMACK    20
#define SRXDIS    17
#define IRMQ      16
#define DMA       15
#define PNET_EN   14
#define LPRIOEN   13
#define AEN       12                  /*abort enable*/
#define FDEN      11
#define IDAM      8                   /* 8-9 Pins */
#define MAXMB     0                   /* 0-6 Pins */

/******************************************************************
                 	   Some bits in CTRL1
 ******************************************************************/
/* (Reset Value = 0000_0000h) */
#define PRESDIV    24   /*Prescaler Division Factor from 24 to 31*/
#define RJW        22    /*from 22 to 23*/
#define PSEG1      19
#define PSEG2      16
#define BOFFMSK    15
#define ERRMSK     14
#define TWRNMSK    11
#define RWRNMSK    10
#define BOFFREC    6
#define PROPSEG    0

/******************************************************************
                 Development Errors Definitions
 ******************************************************************/
#define CAN_E_PARAM_POINTER            (uint8)(0x01U)
#define CAN_E_PARAM_HANDLE 		       (uint8)(0x02U)
#define CAN_E_PARAM_DATA_LENGTH        (uint8)(0x03U)
#define CAN_E_PARAM_CONTROLLER         (uint8)(0x04U)
#define CAN_E_UNINIT                   (uint8)(0x05U)
#define CAN_E_TRANSITION               (uint8)(0x06U)
#define CAN_E_PARAM_BAUDRATE           (uint8)(0x07U)
#define CAN_E_ICOM_CONFIG_INVALID      (uint8)(0x08U)
#define CAN_E_INIT_FAILED              (uint8)(0x09U)

/* Runtime errors definitions */
/* Only needed with the function Det_ReportRuntimeError which will not be implemented */
//  #define CAN_E_DATALOST                 (uint8)(0x01U)


typedef enum
{
	CAN_UNINIT,
	CAN_READY
}Can_ModuleStateType;

#define NUM_OF_CONTROLLERS             (uint8)(0x03U)


/******************************************************************
                      Function Prototypes
 ******************************************************************/
/* This function initializes the module */
void Can_Init(const Can_ConfigType* Config);
/* This function returns the version information of this module */
void Can_GetVersionInfo(Std_VersionInfoType* versioninfo);
/* This function de-initializes the module */
void Can_DeInit(void);
/*This service shall set the baud rate configuration of the CAN controller.
 Depending on necessary baud rate modifications the controller might have to reset */
Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigI );
/*This function performs software triggered state transitions of the CAN controller State machine*/
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
/* This function is called by CanIf to pass a CAN message to CanDrv for transmission */
Std_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo);
#endif /* CAN_H_ */
