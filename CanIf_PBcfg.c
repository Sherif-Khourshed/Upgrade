/*
 * CanIf_PBcfg.c
 *
 *  Created on: Mar 12, 2020
 *      Author: Ahmed Nasr
 */

#include "CanIf.h"
#include "CanIf_Types.h"
#include "main.h"

// Imported structs from Can_PBcfg.c
//extern const Can_ControllerConfigType Can_ControllerCfgData[];
//extern const Can_ConfigSetType Can_ConfigSetData;

// Contains the mapping from CanIf-specific Channels to Can Controllers
const CanControllerIdType CanIf_Arc_ChannelToControllerMap[CANIF_CHANNEL_CNT] = {
	CAN_CTRL_0                      ,/* CANIF_CHL_LS                     */
	CAN_CTRL_1                      ,/* CANIF_CHL_HS                     */

};

const uint8 CanIf_Arc_ChannelDefaultConfIndex[CANIF_CHANNEL_CNT] = {
	CANIF_CHL_0,
	CANIF_CHL_1,

};

// Container that gets slamed into CanIf_InitController()
// Inits ALL controllers
// Multiplicity 1..*
CanIfCtrlCfg CanIfControllerConfig[] = {

    {
        .CanIfCtrlWakeupSupport = CANIF_WAKEUP_SUPPORT_NO_WAKEUP,
        .CanIfCtrlId = CANIF_CHL_0,
        //.CanIfCtrlCanCtrlRef = &Can_ControllerCfgData[INDEX_OF_CAN_CTRL_0],
    },

    {
        .CanIfCtrlWakeupSupport = CANIF_WAKEUP_SUPPORT_NO_WAKEUP,
        .CanIfCtrlId = CANIF_CHL_1,
        //.CanIfCtrlCanCtrlRef = &Can_ControllerCfgData[INDEX_OF_CAN_CTRL_2],
    },

};

// Function callbacks for higher layers
CanIfDispatchCfg CanIfDispatchConfig =
{
    .CanIfDispatchUserCtrlBusOff = NULL,
    .CanIfDispatchUserCtrlModeIndication = NULL,        // Not used
    .CanIfDispatchUserTrcvModeIndication = NULL,   // Not used
    .CanIfDispatchUserValidateWakeupEvent = NULL,
};

CanIfHthCfg CanIfHthConfigData_CANIF_CHL_LS[]=
{

    {
        .CanIfHthType = CAN_ARC_HANDLE_TYPE_BASIC, // TODO
        .CanIfHthCanCtrlIdRef = &CanIfControllerConfig[0],
        .CanIfHthIdSymRef = CAN_CTRL_0_HTH,
        .CanIf_Arc_EOL = TRUE
    },
};

CanIfHthCfg CanIfHthConfigData_CANIF_CHL_HS[]=
{

    {
        .CanIfHthType = CAN_ARC_HANDLE_TYPE_BASIC, // TODO
        .CanIfHthCanCtrlIdRef = &CanIfControllerConfig[1],
        .CanIfHthIdSymRef = CAN_CTRL_1_HTH,
        .CanIf_Arc_EOL = TRUE
    },
};

CanIfHrhCfg CanIfHrhConfigData_CANIF_CHL_LS[]=
{

    {
        .CanIfHrhType = CAN_ARC_HANDLE_TYPE_BASIC,
        .CanIfHrhSoftwareFilter = TRUE, // Must Be True
		.CanIfSoftwareFilterType = MASK,
        .CanIfCanControllerHrhIdRef = CANIF_CHL_0,
        .CanIfHrhIdSymRef = CAN_CTRL_0_HRH,
		.CanIfHrhCanCtrlIdRef = NULL,
        .HrhRangeCfg = NULL,
        .CanIf_Arc_EOL = TRUE
    },
};

CanIfHrhCfg CanIfHrhConfigData_CANIF_CHL_HS[]=
{

    {
        .CanIfHrhType = CAN_ARC_HANDLE_TYPE_BASIC,
        .CanIfHrhSoftwareFilter = TRUE, // Must Be True
		.CanIfSoftwareFilterType = MASK,
        .CanIfCanControllerHrhIdRef = CANIF_CHL_1,
        .CanIfHrhIdSymRef = CAN_CTRL_1_HRH,
		.CanIfHrhCanCtrlIdRef = NULL,
        .HrhRangeCfg = NULL,
        .CanIf_Arc_EOL = TRUE
    },
};

CanIfInitHohCfg CanIfHohConfigData[] =
{

    {
        //.CanConfigSet = &Can_ConfigSetData,
        .HrhCfg = CanIfHrhConfigData_CANIF_CHL_LS,
        .HthCfg = CanIfHthConfigData_CANIF_CHL_LS,
        .CanIf_Arc_EOL = FALSE
    },

    {
        //.CanConfigSet = &Can_ConfigSetData,
        .HrhCfg = CanIfHrhConfigData_CANIF_CHL_HS,
        .HthCfg = CanIfHthConfigData_CANIF_CHL_HS,
        .CanIf_Arc_EOL = TRUE
    },
};

CanIfTxPduCfg CanIfTxPduConfigData[] =
{

    {
        .CanIfTxPduId = 15,
        .CanIfTxPduCanId = 0x732,
        .CanIfCanTxPduIdDlc = 8,
        .CanIfTxPduType = STATIC,
#if ( CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API == STD_ON )
        .CanIfTxPduReadNotifyStatus = TRUE,
#endif
        .CanIfTxPduCanIdType = CANIF_PDUCANID_STANDARD_CAN,
        .CanIfTxPduUserTxConfirmation = address15TxCanIf,
		.CanIfBufferHthRef = &CanIfHthConfigData_CANIF_CHL_LS[0]
    },

    {
        .CanIfTxPduId = 20,
        .CanIfTxPduCanId = 0x742,
        .CanIfCanTxPduIdDlc = 8,
        .CanIfTxPduType = STATIC,
#if ( CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API == STD_ON )
        .CanIfTxPduReadNotifyStatus = TRUE,
#endif
        .CanIfTxPduCanIdType = CANIF_PDUCANID_STANDARD_CAN,
        .CanIfTxPduUserTxConfirmation = address20TxCanIf,
        .CanIfBufferHthRef = &CanIfHthConfigData_CANIF_CHL_HS[0]
    },

};

CanIfRxPduCfg CanIfRxPduConfigData[] =
{

    {
        .CanIfRxPduId = 16,
        .CanIfRxPduCanId = 0x731,
        .CanIfCanRxPduDlc = 8,
#if ( CANIF_PUBLIC_READ_RX_PDU_DATA_API == STD_ON )
        .CanIfRxPduReadData = TRUE,
#endif
#if ( CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API == STD_ON )
        .CanIfRxPduReadNotifyStatus = TRUE,
#endif
        .CanIfRxPduCanIdType = CANIF_RXPDUCANIDTYPE_STANDARD_CAN,
        .CanIfRxPduUserRxIndicationUL = CANIF_RXPDUUSERRXINDICATIONUL_PDUR,
        .CanIfRxPduUserRxIndication = NULL,
        .CanIfRxPduHrhIdRef = &CanIfHrhConfigData_CANIF_CHL_LS[0],
        .CanIfRxPduCanIdMask = 0xFFF
    },

    {
        .CanIfRxPduId = 21,
        .CanIfRxPduCanId = 0x741,
        .CanIfCanRxPduDlc = 8,
#if ( CANIF_PUBLIC_READ_RX_PDU_DATA_API == STD_ON )
        .CanIfRxPduReadData = TRUE,
#endif
#if ( CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API == STD_ON )
        .CanIfRxPduReadNotifyStatus = TRUE,
#endif
        .CanIfRxPduCanIdType = CANIF_RXPDUCANIDTYPE_STANDARD_CAN,
        .CanIfRxPduUserRxIndicationUL = CANIF_RXPDUUSERRXINDICATIONUL_PDUR,
        .CanIfRxPduUserRxIndication = NULL,
        .CanIfRxPduHrhIdRef = &CanIfHrhConfigData_CANIF_CHL_HS[0],
        .CanIfRxPduCanIdMask = 0xFFF
    }

};

// This container contains the init parameters of the CAN
// Multiplicity 1..*
CanIfInitCfg CanIfInitConfig =
{
    .CanIfInitCfgSet = 0, // Not used
    .CanIfMaxRxPduCfg = sizeof(CanIfRxPduConfigData)/sizeof(CanIfRxPduCfg),
    .CanIfMaxTxPduCfg = sizeof(CanIfTxPduConfigData)/sizeof(CanIfTxPduCfg),
    .CanIfMaxBufferSize = 0, // Not used
    // Containers
    .CanIfHohCfgPtr = CanIfHohConfigData,
    .CanIfRxPduCfgPtr = CanIfRxPduConfigData,
    .CanIfTxPduCfgPtr = CanIfTxPduConfigData,
};

// This container includes all necessary configuration sub-containers
// according the CAN Interface configuration structure
const CanIf_ConfigType CanIf_Config =
{
    .CtrlCfg = CanIfControllerConfig,
    .DispatchCfg = &CanIfDispatchConfig,
    .InitCfg = &CanIfInitConfig,
    .TrcvDrvCfg = NULL, // Not used
    .Arc_ChannelToControllerMap = CanIf_Arc_ChannelToControllerMap,
    .Arc_ChannelDefaultConfIndex = CanIf_Arc_ChannelDefaultConfIndex,
};
