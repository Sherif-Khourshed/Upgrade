#ifndef CANIF_CBK_H_
#define CANIF_CBK_H_

/* Software version */
#define CANIF_CBK_SW_MAJOR_VERSION   (1U)
#define CANIF_CBK_SW_MINOR_VERSION   (0U)
#define CANIF_CBK_SW_PATCH_VERSION   (0U)

/* AUTOSAR version */
#define CANIF_CBK_AR_MAJOR_VERSION   (4U)
#define CANIF_CBK_AR_MINOR_VERSION   (3U)
#define CANIF_CBK_AR_PATCH_VERSION   (1U)

/********************************* Callback Notifications *********************************/

Std_ReturnType CanIf_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr);

void CanIf_TxConfirmation(PduIdType CanTxPduId);

void CanIf_RxIndication(const Can_HwType* Mailbox, const PduInfoType* PduInfoPtr);

void CanIf_ControllerBusOff(uint8 ControllerId);

void CanIf_ConfirmPnAvailability(uint8 TransceiverId);

void CanIf_ClearTrcvWufFlagIndication(uint8 TransceiverId);

void CanIf_CheckTrcvWakeFlagIndication(uint8 TransceiverId);

void CanIf_ControllerModeIndication(uint8 ControllerId, Can_ControllerStateType ControllerMode);

void CanIf_TrcvModeIndication(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode);

void CanIf_CurrentIcomConfiguration(uint8 ControllerId, IcomConfigIdType ConfigurationId, IcomSwitch_ErrorType Error);

#endif