#ifndef CANIF_H_
#define CANIF_H_

/* Id for our team in the AUTOSAR, for example ID = 88) */
#define CANIF_VENDOR_ID          (88U)

/* Can If module ID */
#define CANIF_MODULE_ID			 (60U)

/* Software version */
#define CANIF_SW_MAJOR_VERSION   (1U)
#define CANIF_SW_MINOR_VERSION   (0U)
#define CANIF_SW_PATCH_VERSION   (0U)

/* AUTOSAR version */
#define CANIF_AR_MAJOR_VERSION   (4U)
#define CANIF_AR_MINOR_VERSION   (3U)
#define CANIF_AR_PATCH_VERSION   (1U)

/* Include Can If configuration header */
#include "CanIf_Cfg.h"

/* AUTOSAR checking between Can If and Can If Cfg Modules */
#if ((CANIF_AR_MAJOR_VERSION != CANIF_CFG_AR_MAJOR_VERSION)\
 ||  (CANIF_AR_MINOR_VERSION != CANIF_CFG_AR_MINOR_VERSION)\
 ||  (CANIF_AR_PATCH_VERSION != CANIF_CFG_AR_PATCH_VERSION))
  #error "The AR version of CanIf_Cfg.h does not match the expected version"
#endif

/* Software version checking between Can If and Can If Cfg Modules */
#if ((CANIF_SW_MAJOR_VERSION != CANIF_CFG_SW_MAJOR_VERSION)\
 ||  (CANIF_SW_MINOR_VERSION != CANIF_CFG_SW_MINOR_VERSION)\
 ||  (CANIF_SW_PATCH_VERSION != CANIF_CFG_SW_PATCH_VERSION))
  #error "The SW version of CanIf_Cfg.h does not match the expected version"
#endif

/* Include Can If configuration header */
#include "CanIf_Types.h"

/* AUTOSAR checking between Can If and Can If Cfg Modules */
#if ((CANIF_AR_MAJOR_VERSION != CANIF_TYPES_AR_MAJOR_VERSION)\
 ||  (CANIF_AR_MINOR_VERSION != CANIF_TYPES_AR_MINOR_VERSION)\
 ||  (CANIF_AR_PATCH_VERSION != CANIF_TYPES_AR_PATCH_VERSION))
  #error "The AR version of CanIf_Types.h does not match the expected version"
#endif

/* Software version checking between Can If and Can If Cfg Modules */
#if ((CANIF_SW_MAJOR_VERSION != CANIF_TYPES_SW_MAJOR_VERSION)\
 ||  (CANIF_SW_MINOR_VERSION != CANIF_TYPES_SW_MINOR_VERSION)\
 ||  (CANIF_SW_PATCH_VERSION != CANIF_TYPES_SW_PATCH_VERSION))
  #error "The SW version of CanIf_Types.h does not match the expected version"
#endif

/********************************* Error Codes ************************************/

/**********************
 * Development Errors *
 **********************/

/* API service called with invalid parameter */
#define CANIF_E_PARAM_CANID 					10
#define CANIF_E_PARAM_HOH 						12
#define CANIF_E_PARAM_LPDU 						13
#define CANIF_E_PARAM_CONTROLLERID 				15
#define CANIF_E_PARAM_WAKEUPSOURCE 				16
#define CANIF_E_PARAM_TRCV 						17
#define CANIF_E_PARAM_TRCVMODE 					18
#define CANIF_E_PARAM_TRCVWAKEUPMODE 			19
#define CANIF_E_PARAM_CTRLMODE 					21
#define CANIF_E_PARAM_PDU_MODE 					22

/* API service called with invalid pointer */
#define CANIF_E_PARAM_POINTER 					20

/* API service used without module initialization */
#define CANIF_E_UNINIT							30

/* Transmit PDU ID invalid */
#define CANIF_E_INVALID_TXPDUID					50

/* Receive PDU ID invalid */
#define CANIF_E_INVALID_RXPDUID					60

/* CAN Interface initialization failed */
#define CANIF_E_INIT_FAILED						80

/******************
 * Runtime Errors *
 ******************/

/* Failed Data Length Check */
#define CANIF_E_INVALID_DATA_LENGTH				61

/* Data Length */
#define CANIF_E_DATA_LENGTH_MISMATCH			62

/* Transmit requested on offline PDU channel */
#define CANIF_E_STOPPED							70

/* Message length was exceeding the maximum length */
#define CANIF_E_TXPDU_LENGTH_EXCEEDED			90

/********************************* Service IDs ***********************************/

#define CANIF_INIT_SID 							0x01
#define CANIF_DEINIT_SID 						0x02
#define CANIF_SETCONTROLLERMODE_SID				0x03
#define CANIF_GETCONTROLLERMODE_SID				0x04
#define CANIF_GETCONTROLLERERRORSTATE_SID		0x4b
#define CANIF_TRANSMIT_SID						0x49
#define CANIF_CANCELTRANSMIT_SID				0x4a
#define CANIF_READRXPDUDATA_SID					0x06
#define CANIF_READTXNOTIFSTATUS_SID				0x07
#define CANIF_READRXNOTIFSTATUS_SID				0x08
#define CANIF_SETPDUMODE_SID					0x09
#define CANIF_GETPDUMODE_SID					0x0a
#define CANIF_GETVERSIONINFO_SID				0x0b
#define CANIF_SETDYNAMICTXID_SID				0x0c
#define CANIF_SETTRCVMODE_SID					0x0d
#define CANIF_GETTRCVMODE_SID					0x0e
#define CANIF_GETTRCVWAKEUPREASON_SID			0x0f
#define CANIF_SETTRCVWAKEUPMODE_SID				0x10
#define CANIF_CHECKWAKEUP_SID					0x11
#define CANIF_CHECKVALIDATION_SID				0x12
#define CANIF_GETTXCONFIRMATIONSTATE_SID		0x19
#define CANIF_CLEARTRCVWUFFLAG_SID				0x1e
#define CANIF_CHECKTRCVWAKEFLAG_SID				0x1f
#define CANIF_SETBAUDRATE_SID					0x27
#define CANIF_SETICOMCONFIGURATION_SID			0x25
#define CANIF_TRIGGERTRANSMIT_SID				0x41
#define CANIF_TXCONFIRMATION_SID				0x13
#define CANIF_RXINDICATION_SID					0x14
#define CANIF_CONTROLLERBUSOFF_SID				0x16
#define CANIF_CONFIRMPNAVAILABILITY_SID			0x1a
#define CANIF_CLEARTRCVWUFFLAGINDICATION_SID	0x20
#define CANIF_CHECKTRCVWAKEFLAGINDICATION_SID	0x21
#define CANIF_CONTROLLERMODEINDICATION_SID		0x17
#define CANIF_TRCVMODEINDICATION_SID			0x22
#define CANIF_CURRENTICOMCONFIGURATION_SID		0x26

/********************************* Prototypes ************************************/

void CanIf_Init(const CanIf_ConfigType* ConfigPtr);

void CanIf_DeInit(void);

Std_ReturnType CanIf_SetControllerMode(uint8 ControllerId, Can_ControllerStateType ControllerMode);

Std_ReturnType CanIf_GetControllerMode(uint8 ControllerId, CanIf_ControllerModeType* ControllerModePtr);

Std_ReturnType CanIf_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr);

Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);

Std_ReturnType CanIf_CancelTransmit(PduIdType TxPduId);

//Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId, PduInfoType* CanIfRxInfoPtr);

//CanIf_NotifStatusType CanIf_ReadTxNotifStatus(PduIdType CanIfTxSduId);

//CanIf_NotifStatusType CanIf_ReadRxNotifStatus(PduIdType CanIfRxSduId);

Std_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_ChannelSetModeType PduModeRequest);

Std_ReturnType CanIf_GetPduMode(uint8 ControllerId, CanIf_ChannelGetModeType* PduModePtr);

void CanIf_GetVersionInfo(Std_VersionInfoType* VersionInfo);

void CanIf_SetDynamicTxId(PduIdType CanIfTxSduId, Can_IdType CanId);

//Std_ReturnType CanIf_SetTrcvMode(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode);

//Std_ReturnType CanIf_GetTrcvMode(uint8 TransceiverId, CanTrcv_TrcvModeType* TransceiverModePtr);

//Std_ReturnType CanIf_GetTrcvWakeupReason(uint8 TransceiverId, CanTrcv_TrcvWakeupReasonType* TrcvWuReasonPtr);

//Std_ReturnType CanIf_SetTrcvWakeupMode(uint8 TransceiverId, CanTrcv_TrcvWakeupModeType TrcvWakeupMode);

/*Std_ReturnType CanIf_CheckWakeup(EcuM_WakeupSourceType WakeupSource);*/

/*Std_ReturnType CanIf_CheckValidation(EcuM_WakeupSourceType WakeupSource);*/

CanIf_NotifStatusType CanIf_GetTxConfirmationState(uint8 ControllerId);

Std_ReturnType CanIf_ClearTrcvWufFlag(uint8 TransceiverId);

Std_ReturnType CanIf_CheckTrcvWakeFlag(uint8 TransceiverId);

Std_ReturnType CanIf_SetBaudrate(uint8 ControllerId, uint16 BaudRateConfigID);

Std_ReturnType CanIf_SetIcomConfiguration(uint8 ControllerId, IcomConfigIdType ConfigurationId);

#endif
