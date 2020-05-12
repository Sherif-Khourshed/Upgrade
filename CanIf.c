/*
 * CanIf.c
 *
 *  Created on: Mar 18, 2020
 *      Author: Ahmed Nasr
 */


#include "CanIf.h"
#include "CanIf_Cbk.h"

#include "MemMap.h"

#if (CANIF_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"

#if ((CANIF_AR_MAJOR_VERSION != DET_AR_MAJOR_VERSION)\
  || (CANIF_AR_MINOR_VERSION != DET_AR_MINOR_VERSION)\
  || (CANIF_AR_PATCH_VERSION != DET_AR_PATCH_VERSION))
	#error "The AR version of Det.h does not match the expected version"
#endif

/* Validation methods initialization */
#if  (CANIF_DEV_ERROR_DETECT == STD_ON)
#define VALIDATE(_exp,_api,_err) \
        if( !(_exp) ){ \
          Det_ReportError(CANIF_MODULE_ID, 0, _api, _err); \
        }
#else
#define VALIDATE(_exp,_api,_err)
#endif

#endif

// Helper to get the Can Controller refered to by a CanIf Channel
#define ARC_GET_CHANNEL_CONTROLLER(_channel) \
		CanIf_ConfigPtr->Arc_ChannelToControllerMap[_channel]

/* Global configure */
static const CanIf_ConfigType* CanIf_ConfigPtr;

/* Struct of controller private data. */
typedef struct
{
  CanIf_ControllerModeType ControllerMode;
  CanIf_ChannelGetModeType PduMode;
} CanIf_ChannelPrivateType;

/* Global configuration struct */
typedef struct
{
	/* Is CanIf initialized */
	boolean initRun;

	/* Channels private data */
	CanIf_ChannelPrivateType channelData[CANIF_CHANNEL_CNT];
} CanIf_GlobalType;

// Global config
static CanIf_GlobalType CanIf_Global;

/***************************** Prototypes ******************************/

void CanIf_PreInit_InitController(uint8 Controller, uint8 ConfigurationIndex);
static CanIf_Arc_ChannelIdType CanIf_Arc_FindHrhChannel( Can_Arc_HRHType hrh );
const CanIfHrhCfg* CanIf_Arc_GetReceiveHandler(CanIf_Arc_ChannelIdType Channel);
const CanIfHthCfg* CanIf_Arc_GetTransmitHandler(CanIf_Arc_ChannelIdType Channel);
uint8 CanIf_Arc_GetChannelDefaultConfIndex(CanIf_Arc_ChannelIdType Channel);

/*---------------------------------------------------------------------*/

static CanIf_Arc_ChannelIdType CanIf_Arc_FindHrhChannel( Can_Arc_HRHType hrh )
{
  const CanIfInitHohCfg *hohConfig;
  const CanIfHrhCfg *hrhConfig;

  // foreach(hoh){ foreach(hrh in hoh) {} }
  hohConfig = CanIf_ConfigPtr->InitCfg->CanIfHohCfgPtr;
  hohConfig--;
  do
  {
    hohConfig++;

    hrhConfig = hohConfig->HrhCfg;
    hrhConfig--;
    do
    {
      hrhConfig++;
      if (hrhConfig->CanIfHrhIdSymRef == hrh){
        return hrhConfig->CanIfCanControllerHrhIdRef;
      }
    } while(!hrhConfig->CanIf_Arc_EOL);
  } while(!hohConfig->CanIf_Arc_EOL);

  Det_ReportError(CANIF_MODULE_ID, 0, CANIF_RXINDICATION_SID, CANIF_E_PARAM_HOH);

  return (CanIf_Arc_ChannelIdType) -1;
}

/*---------------------------------------------------------------------*/

/*
 * Service name: CanIf_Init
 * Syntax: void CanIf_Init(const CanIf_ConfigType* ConfigPtr)
 * Service ID[hex]: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): ConfigPtr Pointer to configuration parameter set, used e.g. for post build parameters
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: This service Initializes internal and external interfaces of the CAN Interface for the further processing.
 */
void CanIf_Init(const CanIf_ConfigType* ConfigPtr)
{
	/* Validate the configuration pointer does not point to null */
	VALIDATE(ConfigPtr != 0, CANIF_INIT_SID, CANIF_E_PARAM_POINTER);

	CanIf_ConfigPtr = ConfigPtr;

	/* Counter for channels */
	uint8 channel = 0;

	for (channel = 0; channel < CANIF_CHANNEL_CNT; channel++)
	{
		CanIf_Global.channelData[channel].ControllerMode = CANIF_CS_STOPPED;
		CanIf_Global.channelData[channel].PduMode = CANIF_GET_OFFLINE;
		CanIf_PreInit_InitController(channel, CanIf_ConfigPtr->Arc_ChannelDefaultConfIndex[channel]);
	}

	CanIf_Global.initRun = TRUE;
}

/*---------------------------------------------------------------------*/

void CanIf_PreInit_InitController(uint8 Controller, uint8 ConfigurationIndex)
{
	/* We call this a CanIf channel. Hopefully makes it easier to follow. */
	CanIf_Arc_ChannelIdType channel = (CanIf_Arc_ChannelIdType) Controller;

	VALIDATE(channel < CANIF_CHANNEL_CNT, CANIF_INIT_SID, CANIF_E_PARAM_CONTROLLERID);
	VALIDATE(ConfigurationIndex < CANIF_CHANNEL_CONFIGURATION_CNT, CANIF_INIT_SID, CANIF_E_PARAM_POINTER);

	const CanControllerIdType canControllerId = ARC_GET_CHANNEL_CONTROLLER(channel);
	/* Validate that the configuration at the index match the right channel */
	VALIDATE(CanIf_ConfigPtr->CtrlCfg[ConfigurationIndex].CanIfCtrlId == channel, CANIF_INIT_SID, CANIF_E_PARAM_CONTROLLERID);

	/* TODO: remove comment after adding can.c */
	//const Can_ConfigType *canConfig = CanIf_ConfigPtr->CtrlCfg[ConfigurationIndex].CanIfCtrlCanCtrlRef;
	/* Validate that the CanIfControllerConfig points to configuration for the right Can Controller */
	//VALIDATE(canConfig->CanController_ptr->CanControllerId == canControllerId, CANIF_INIT_SID, CANIF_E_PARAM_CONTROLLERID);

	//Can_InitController(canControllerId, canConfig);
}

/*---------------------------------------------------------------------*/

/*
 * Service ID[hex]: 0x03
 * Sync/Async: Asynchronous
 * Reentrancy: Reentrant (Not for the same controller)
 * Parameters (in): ControllerId Abstracted CanIf ControllerId which is assigned to a CAN controller, which is requested for mode transition.
					ControllerMode Requested mode transition
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: Std_ReturnType E_OK: Controller mode request has been accepted
								E_NOT_OK: Controller mode request has not been accepted
 * Description: This service calls the corresponding CAN Driver service for changing of the CAN controller mode.
 */
Std_ReturnType CanIf_SetControllerMode(uint8 Controller, Can_ControllerStateType ControllerMode)
{
  // We call this a CanIf channel. Hopefully makes it easier to follow.
  CanIf_Arc_ChannelIdType channel = (CanIf_Arc_ChannelIdType) Controller;


  CanIf_ControllerModeType oldMode;

  VALIDATE( CanIf_Global.initRun, CANIF_SETCONTROLLERMODE_SID, CANIF_E_UNINIT );
  VALIDATE( channel < CANIF_CHANNEL_CNT, CANIF_SETCONTROLLERMODE_SID, CANIF_E_PARAM_CONTROLLERID );

  oldMode = CanIf_Global.channelData[channel].ControllerMode;

  if (oldMode == CANIF_CS_UNINIT)
  {
    VALIDATE(FALSE, CANIF_SETCONTROLLERMODE_SID, CANIF_E_UNINIT); // See figure 32, 33
    return E_NOT_OK;
  }

  CanControllerIdType canControllerId = ARC_GET_CHANNEL_CONTROLLER(Controller);

  switch (ControllerMode)
  {
  case CAN_CS_STARTED:
  {
    switch (oldMode)
    {
      case CANIF_CS_SLEEP:
    	  /* TODO: remove comment after adding can.h */
        /*if (Can_SetControllerMode(canControllerId, CAN_T_STOP) == CAN_NOT_OK){
          return E_NOT_OK;
        }*/
        CanIf_Global.channelData[channel].ControllerMode = CANIF_CS_STOPPED;
        break;
      default:
        // Just fall through
        break;
    }

    CanIf_SetPduMode(channel, CANIF_SET_ONLINE);
    /* TODO: remove comment after adding can.h */
    /*if (Can_SetControllerMode(canControllerId, CAN_T_START) == CAN_NOT_OK){
      return E_NOT_OK;
    }*/
    CanIf_Global.channelData[channel].ControllerMode = CANIF_CS_STARTED;
  }
  break;

  case CAN_CS_SLEEP: // Figure 33
  {
    switch (oldMode) {
      case CANIF_CS_STARTED:
    	  /* TODO: remove comment after adding can.h */
        /*if (Can_SetControllerMode(canControllerId, CAN_T_STOP) == CAN_NOT_OK){
          return E_NOT_OK;
        }*/
        CanIf_Global.channelData[channel].ControllerMode = CANIF_CS_STOPPED;
        break;
      default:
        // Just fall through for other cases
        break;
    }

    /*if (Can_SetControllerMode(canControllerId, CAN_T_SLEEP) == CAN_NOT_OK){
      return E_NOT_OK;
    }*/
    CanIf_Global.channelData[channel].ControllerMode = CANIF_CS_SLEEP;
  }
  break;

  case CAN_CS_STOPPED:
  {
    switch (oldMode)
    {
      case CANIF_CS_SLEEP:
    	  /* TODO: remove comment after adding can.h */
        /*if (Can_SetControllerMode(canControllerId, CAN_T_WAKEUP) == CAN_NOT_OK){
          return E_NOT_OK;
        }*/
        break;
      default:
        // Just fall through for other cases
        break;
    }

    CanIf_SetPduMode(channel, CANIF_SET_OFFLINE);
    /* TODO: remove comment after adding can.h */
    /*if (Can_SetControllerMode(canControllerId, CAN_T_STOP) == CAN_NOT_OK){
      return E_NOT_OK;
    }*/
    CanIf_Global.channelData[channel].ControllerMode = CANIF_CS_STOPPED;
    break;
  }
  break;

  case CAN_CS_UNINIT:
    // Just fall through
    break;
  }
  return E_OK;
}

/*---------------------------------------------------------------------*/

/*
 * Service ID[hex]: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): ControllerId Abstracted CanIf ControllerId which is assigned to a CAN controller, which is requested for current operation mode.
 * Parameters (inout): None
 * Parameters (out): ControllerModePtr Pointer to a memory location, where the current mode of the CAN controller will be stored.
 * Return value: Std_ReturnType E_OK: Controller mode request has been accepted.
								E_NOT_OK: Controller mode request has not been accepted.
 * Description: This service calls the corresponding CAN Driver service for obtaining the current status of the CAN controller.
 */
Std_ReturnType CanIf_GetControllerMode(uint8 Controller, CanIf_ControllerModeType *ControllerModePtr)
{
  // We call this a CanIf channel. Hopefully makes it easier to follow.
  CanIf_Arc_ChannelIdType channel = (CanIf_Arc_ChannelIdType) Controller;

  VALIDATE(CanIf_Global.initRun, CANIF_GETCONTROLLERMODE_SID, CANIF_E_UNINIT );
  VALIDATE(channel < CANIF_CHANNEL_CNT, CANIF_GETCONTROLLERMODE_SID, CANIF_E_PARAM_CONTROLLERID );
  VALIDATE(ControllerModePtr != NULL, CANIF_GETCONTROLLERMODE_SID, CANIF_E_PARAM_POINTER );

  *ControllerModePtr = CanIf_Global.channelData[channel].ControllerMode;

  return E_OK;
}

/*---------------------------------------------------------------------*/

/**
 * Matches a Tx PDU id agaist the ones that are in the database.
 *
 * @returns Ptr a TxPdu
 */
#if ( CANIF_ARC_RUNTIME_PDU_CONFIGURATION == STD_ON )
CanIfTxPduCfg * CanIf_FindTxPduEntry(PduIdType id)
#else
static const CanIfTxPduCfg * CanIf_FindTxPduEntry(PduIdType id)
#endif
{
	uint64 index = 0;
	for (index = 0; index < CanIf_ConfigPtr->InitCfg->CanIfMaxTxPduCfg; index++)
	{
		if (id == CanIf_ConfigPtr->InitCfg->CanIfTxPduCfgPtr[index].CanIfTxPduId)
		{
			return &CanIf_ConfigPtr->InitCfg->CanIfTxPduCfgPtr[index];
		}
	}

	return NULL;
	/*if (id >= CanIf_ConfigPtr->InitCfg->CanIfMaxTxPduCfg) {
		return NULL;
	} else {
		return &CanIf_ConfigPtr->InitCfg->CanIfTxPduCfgPtr[id];

	}*/
}

/*---------------------------------------------------------------------*/

#if ( CANIF_ARC_RUNTIME_PDU_CONFIGURATION == STD_ON )
CanIfRxPduCfg * CanIf_FindRxPduEntry(PduIdType id) {
	if (id >= CanIf_ConfigPtr->InitCfg->CanIfMaxRxPduCfg) {
		return NULL;
	} else {
		return &CanIf_ConfigPtr->InitCfg->CanIfRxPduCfgPtr[id];
	}
}

const CanIfHrhCfg* CanIf_Arc_GetReceiveHandler(CanIf_Arc_ChannelIdType Channel) {
  const CanIfInitHohCfg *hohConfig;
  const CanIfHrhCfg *hrhConfig;

  // foreach(hoh){ foreach(hrh in hoh) {} }
  hohConfig = CanIf_ConfigPtr->InitCfg->CanIfHohCfgPtr;
  hohConfig--;
  do
  {
	hohConfig++;

	hrhConfig = hohConfig->HrhCfg;
    hrhConfig--;

    /*uint32 i;
    for (i = hrhConfig->HrhRangeCfg->CanIfHrhRangeRxPduLowerCanId;
    	 i < hrhConfig->HrhRangeCfg->CanIfHrhRangeRxPduUpperCanId;
    	 i++)
    {
    	hrhConfig++;
    	if (hrhConfig->CanIfHrhCanCtrlIdRef->CanIfCtrlId == Channel)
    		return hrhConfig;
    }*/

    do
    {
      hrhConfig++;
      if (hrhConfig->CanIfHrhCanCtrlIdRef->CanIfCtrlId == Channel)
        return hrhConfig;
	} while(!hrhConfig->CanIf_Arc_EOL);

  } while(!hohConfig->CanIf_Arc_EOL);

  Det_ReportError(CANIF_MODULE_ID, 0, 0xFF, CANIF_E_PARAM_HOH);

  return NULL;
}

const CanIfHthCfg* CanIf_Arc_GetTransmitHandler(CanIf_Arc_ChannelIdType Channel) {
  const CanIfInitHohCfg *hohConfig;
  const CanIfHthCfg *hthConfig;

  // foreach(hoh){ foreach(hrh in hoh) {} }
  hohConfig = CanIf_ConfigPtr->InitCfg->CanIfHohCfgPtr;
  hohConfig--;
  do
  {
	hohConfig++;

	hthConfig = hohConfig->HthCfg;
	hthConfig--;
    do
    {
    	hthConfig++;
      if (hthConfig->CanIfHthCanCtrlIdRef->CanIfCtrlId == Channel)
        return hthConfig;
	} while(!hthConfig->CanIf_Arc_EOL);

  } while(!hohConfig->CanIf_Arc_EOL);

  Det_ReportError(CANIF_MODULE_ID, 0, 0xFF, CANIF_E_PARAM_HOH);

  return NULL;
}
#endif

/*---------------------------------------------------------------------*/

/*
 * Service ID[hex]: 0x49
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
 * Parameters (in): TxPduId Identifier of the PDU to be transmitted
					PduInfoPtr Length of and pointer to the PDU data and pointer to MetaData.
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: Std_ReturnType E_OK: Transmit request has been accepted.
								E_NOT_OK: Transmit request has not been accepted.
 * Description: Requests transmission of a PDU.
 */
Std_ReturnType CanIf_Transmit(PduIdType CanTxPduId, const PduInfoType *PduInfoPtr)
{
  Can_PduType canPdu;
  const CanIfTxPduCfg *txEntry;
  CanIf_ControllerModeType csMode;
  CanIf_ChannelGetModeType pduMode;

  VALIDATE(CanIf_Global.initRun, CANIF_TRANSMIT_SID, CANIF_E_UNINIT );
  VALIDATE((PduInfoPtr != NULL), CANIF_TRANSMIT_SID, CANIF_E_PARAM_POINTER );

  // Get the controller from L-PDU handle
  txEntry = CanIf_FindTxPduEntry(CanTxPduId);

  if (txEntry == 0)
  {
    VALIDATE(FALSE, CANIF_TRANSMIT_SID, CANIF_E_INVALID_TXPDUID);
    return E_NOT_OK;
  }

  CanIf_Arc_ChannelIdType channel = txEntry->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->CanIfCtrlId;

  // Get and verify the controller mode
  if (CanIf_GetControllerMode(channel, &csMode) == E_NOT_OK){
    return E_NOT_OK;
  }

  if (csMode != CANIF_CS_STARTED){  // CANIF_161
    return E_NOT_OK;
  }

  // Get and verify the PDU channel mode control
  if (CanIf_GetPduMode(channel, &pduMode) == E_NOT_OK){
    return E_NOT_OK;
  }

  if ((pduMode != CANIF_GET_TX_ONLINE) && (pduMode != CANIF_GET_ONLINE)){
    return E_NOT_OK;
  }

  canPdu.id = txEntry->CanIfTxPduCanId;
  canPdu.length = PduInfoPtr->SduLength;
  canPdu.sdu = PduInfoPtr->SduDataPtr;
  canPdu.swPduHandle = CanTxPduId;

  /* TODO: remove comment after adding can.c */
  /*Can_ReturnType rVal = Can_Write(txEntry->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthIdSymRef, &canPdu);

  if (rVal == CAN_NOT_OK){
    return E_NOT_OK;
  }

  if (rVal == CAN_BUSY)  // CANIF 082, CANIF 161
  {
    // Tx buffering not supported so just return.
    return E_NOT_OK;
  }*/

  return E_OK;
}

/*---------------------------------------------------------------------*/

/*
 * Service ID[hex]: 0x09
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): ControllerId All PDUs of the own ECU connected to the corresponding CanIf ControllerId, which is assigned to a physical CAN controller are addressed.
					PduModeRequest Requested PDU mode change
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: Std_ReturnType E_OK: Request for mode transition has been accepted.
								E_NOT_OK: Request for mode transition has not been accepted.
 * Description: This service sets the requested mode at the L-PDUs of a predefined logical PDU channel.
 */
Std_ReturnType CanIf_SetPduMode(uint8 Controller, CanIf_ChannelSetModeType PduModeRequest)
{
  // We call this a CanIf channel. Hopefully makes it easier to follow.
  CanIf_Arc_ChannelIdType channel = (CanIf_Arc_ChannelIdType) Controller;

  VALIDATE( CanIf_Global.initRun, CANIF_SETPDUMODE_SID, CANIF_E_UNINIT );
  VALIDATE( channel < CANIF_CHANNEL_CNT, CANIF_SETPDUMODE_SID, CANIF_E_PARAM_CONTROLLERID );

  CanIf_ChannelGetModeType oldMode = CanIf_Global.channelData[channel].PduMode;

  switch(PduModeRequest)
  {
  case CANIF_SET_OFFLINE:
    CanIf_Global.channelData[channel].PduMode = CANIF_GET_OFFLINE;
    break;
  case CANIF_SET_RX_OFFLINE:
    if (oldMode == CANIF_GET_RX_ONLINE){
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_OFFLINE;
    } else if (oldMode == CANIF_GET_ONLINE){
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_TX_ONLINE;
    } else if (oldMode == CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE){
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_OFFLINE_ACTIVE;
    }

    // Other oldmodes don't care
    break;
  case CANIF_SET_RX_ONLINE:
    if (oldMode == CANIF_GET_OFFLINE){
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_RX_ONLINE;
    } else if (oldMode == CANIF_GET_TX_ONLINE) {
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_ONLINE;
    } else if (oldMode == CANIF_GET_OFFLINE_ACTIVE){
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE;
    }

    // Other oldmodes don't care
    break;
  case CANIF_SET_TX_OFFLINE:
    if (oldMode == CANIF_GET_TX_ONLINE){
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_OFFLINE;
    } else if (oldMode == CANIF_GET_ONLINE) {
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_RX_ONLINE;
    } else if (oldMode == CANIF_GET_OFFLINE_ACTIVE) {
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_OFFLINE;
    } else if (oldMode == CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE) {
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_RX_ONLINE;
    }

    // Other oldmodes don't care
    break;
  case CANIF_SET_TX_ONLINE:
    if (oldMode == CANIF_GET_OFFLINE){
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_TX_ONLINE;
    } else if (oldMode == CANIF_GET_RX_ONLINE) {
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_ONLINE;
    } else if (oldMode == CANIF_GET_OFFLINE_ACTIVE) {
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_TX_ONLINE;
    } else if (oldMode == CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE) {
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_ONLINE;
    }

    // Other oldmodes don't care
    break;
  case CANIF_SET_ONLINE:
    CanIf_Global.channelData[channel].PduMode = CANIF_GET_ONLINE;
    break;

  case CANIF_SET_TX_OFFLINE_ACTIVE:
    if (oldMode == CANIF_GET_OFFLINE) {
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_OFFLINE_ACTIVE;
    } else if (oldMode == CANIF_GET_RX_ONLINE) {
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE;
    } else if (oldMode == CANIF_GET_TX_ONLINE) {
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_OFFLINE_ACTIVE;
    } else if (oldMode == CANIF_GET_ONLINE) {
      CanIf_Global.channelData[channel].PduMode = CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE;
    }

    // Other old modes don't care
    break;
  }

  return E_OK;
}

/*---------------------------------------------------------------------*/

/*
 * Service ID[hex]: 0x0a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant (Not for the same channel)
 * Parameters (in): ControllerId All PDUs of the own ECU connected to the corresponding CanIf ControllerId, which is assigned to a physical CAN controller are addressed.
 * Parameters (inout): None
 * Parameters (out): PduModePtr Pointer to a memory location, where the current mode of the logical PDU channel will be stored.
 * Return value: Std_ReturnType E_OK: PDU mode request has been accepted
								E_NOT_OK: PDU mode request has not been accepted
 * Description: This service reports the current mode of a requested PDU channel.
 */
Std_ReturnType CanIf_GetPduMode(uint8 Controller, CanIf_ChannelGetModeType *PduModePtr)
{
  // We call this a CanIf channel. Hopefully makes it easier to follow.
  CanIf_Arc_ChannelIdType channel = (CanIf_Arc_ChannelIdType) Controller;

  VALIDATE( CanIf_Global.initRun, CANIF_GETPDUMODE_SID, CANIF_E_UNINIT );
  VALIDATE( channel < CANIF_CHANNEL_CNT, CANIF_GETPDUMODE_SID, CANIF_E_PARAM_CONTROLLERID );

  *PduModePtr = CanIf_Global.channelData[channel].PduMode;

  return E_OK;
}

/*---------------------------------------------------------------------*/

/*
 * Service ID[hex]: 0x0c
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): CanIfTxSduId L-SDU handle to be transmitted. This handle specifies the corresponding CAN LSDU ID and implicitly the CAN Driver instance as well as the corresponding CAN controller device.
					CanId Standard/Extended CAN ID of CAN L-SDU that shall be transmitted as FD or conventional CAN frame.
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: This service reconfigures the corresponding CAN identifier of the requested CAN L-PDU.
 */
#if ( (CANIF_PUBLIC_SET_DYNAMIC_TX_ID_API == STD_ON) && (CANIF_ARC_RUNTIME_PDU_CONFIGURATION == STD_ON) )
void CanIf_SetDynamicTxId(PduIdType CanTxPduId, Can_IdType CanId)
{
  CanIfTxPduCfg *txEntry;
  VALIDATE(CanIf_Global.initRun, CANIF_SETDYNAMICTXID_SID, CANIF_E_UNINIT);

  // Get the controller from L-PDU handle
  txEntry = CanIf_FindTxPduEntry(CanTxPduId);

  if (txEntry == 0)
  {
    VALIDATE(FALSE, CANIF_SETDYNAMICTXID_SID, CANIF_E_INVALID_TXPDUID);
    return;
  }

  // Check that this is a dynamic PDU
  if (txEntry->CanIfTxPduType != DYNAMIC)
  {
    VALIDATE(FALSE, CANIF_SETDYNAMICTXID_SID, CANIF_E_INVALID_TXPDUID);
    return;
  }

  // Check that this is an extended or standard id
  if (((CanId & 0x80000000U) && (txEntry->CanIfTxPduCanIdType == CANIF_PDUCANID_EXTENDED_CAN)) ||
	  ((CanId & 0x80000000U) && (txEntry->CanIfTxPduCanIdType == CANIF_PDUCANID_EXTENDED_FD_CAN)) ||
      (((CanId & 0x80000000U) == 0) && (txEntry->CanIfTxPduCanIdType == CANIF_PDUCANID_STANDARD_CAN)) ||
	  (((CanId & 0x80000000U) == 0) && (txEntry->CanIfTxPduCanIdType == CANIF_PDUCANID_STANDARD_FD_CAN)))
  {
    // Update the CanID
    txEntry->CanIfTxPduCanId = CanId & 0x7FFFFFFFU;
  }
  else
  {
    // Inavlid Canid to configuration
    VALIDATE(FALSE, CANIF_SETDYNAMICTXID_SID, CANIF_E_PARAM_CANID);
  }
}
#endif

/*---------------------------------------------------------------------*/

/*
 * Callback interface from driver
 */

/*
 * Service ID[hex]: 0x13
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): CanTxPduId L-PDU handle of CAN L-PDU successfully transmitted. This ID specifies the corresponding CAN L-PDU ID and implicitly the CAN Driver instance as well as the corresponding CAN controller device.
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: This service confirms a previously successfully processed transmission of a CAN TxPDU.
 */
void CanIf_TxConfirmation(PduIdType canTxPduId)
{
  VALIDATE(CanIf_Global.initRun, CANIF_TXCONFIRMATION_SID, CANIF_E_UNINIT)
  VALIDATE(canTxPduId < CanIf_ConfigPtr->InitCfg->CanIfMaxTxPduCfg, CANIF_TXCONFIRMATION_SID, CANIF_E_PARAM_LPDU);

  const CanIfTxPduCfg* entry = &CanIf_ConfigPtr->InitCfg->CanIfTxPduCfgPtr[canTxPduId];

      if (entry->CanIfTxPduUserTxConfirmation != NULL)
      {
        CanIf_ChannelGetModeType mode;
        CanIf_GetPduMode(entry->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->CanIfCtrlId, &mode);
        if ((mode == CANIF_GET_TX_ONLINE) || (mode == CANIF_GET_ONLINE)
            || (mode == CANIF_GET_OFFLINE_ACTIVE) || (mode == CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE) )
        {
          entry->CanIfTxPduUserTxConfirmation(entry->CanIfTxPduId);  /* CANIF053 */
        }
      }
      return;
}

/*---------------------------------------------------------------------*/

/*
 * Service ID[hex]: 0x14
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): Mailbox Identifies the HRH and its corresponding CAN Controller
					PduInfoPtr Pointer to the received L-PDU
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: This service indicates a successful reception of a received CAN Rx LPDU to the CanIf after passing all filters and validation checks.
 */
void CanIf_RxIndication(const Can_HwType* Mailbox, const PduInfoType* PduInfoPtr)
{
  VALIDATE(CanIf_Global.initRun, CANIF_RXINDICATION_SID, CANIF_E_UNINIT);
  VALIDATE(PduInfoPtr->SduDataPtr != NULL, CANIF_RXINDICATION_SID, CANIF_E_PARAM_POINTER);
  /* Check PDU mode before continue processing */
  CanIf_ChannelGetModeType mode;
  CanIf_Arc_ChannelIdType channel = (Can_Arc_HRHType)Mailbox->ControllerId;
  if (channel == -1)  // Invalid HRH
  {
    return;
  }

  if (CanIf_GetPduMode(channel, &mode) == E_OK)
  {
    if ( (mode == CANIF_GET_OFFLINE) || (mode == CANIF_GET_TX_ONLINE) ||
        (mode == CANIF_GET_OFFLINE_ACTIVE) )
    {
      // Receiver path is disabled so just drop it
      return;
    }
  }
  else
  {
    return;  // No mode so just return
  }

  const CanIfRxPduCfg *entry = CanIf_ConfigPtr->InitCfg->CanIfRxPduCfgPtr;

  /* Find the CAN id in the RxPduList */
  for (uint16 i = 0; i < CanIf_ConfigPtr->InitCfg->CanIfMaxRxPduCfg; i++)
  {
    if (entry->CanIfRxPduHrhIdRef->CanIfHrhIdSymRef == (Can_Arc_HRHType)Mailbox->ControllerId)
    {
      // Software filtering
      if (entry->CanIfRxPduHrhIdRef->CanIfHrhType == CAN_ARC_HANDLE_TYPE_BASIC)
      {
        if (entry->CanIfRxPduHrhIdRef->CanIfHrhSoftwareFilter)
        {
          if (entry->CanIfRxPduHrhIdRef->CanIfSoftwareFilterType == MASK)
          {
            if ((Mailbox->CanId & entry->CanIfRxPduCanIdMask ) ==
                ( entry->CanIfRxPduCanId & entry->CanIfRxPduCanIdMask))
            {
              // We found a pdu so call higher layers
            }
            else
            {
              entry++;
              continue; // Go to next entry
            }
          }
          else
          {
            Det_ReportError(CANIF_MODULE_ID, 0, CANIF_RXINDICATION_SID, CANIF_E_PARAM_HOH);
            continue; // Not a supported filter type, so just drop the frame
          }
        }
      }

      switch (entry->CanIfRxPduUserRxIndicationUL)
      {
        /*case CANIF_USER_TYPE_CAN_SPECIAL:
        {
          ( (CanIf_FuncTypeCanSpecial)(entry->CanIfUserRxIndication) )(
            entry->CanIfCanRxPduHrhRef->CanIfCanControllerHrhIdRef,
            entry->CanIfCanRxPduId,
            CanSduPtr,
            CanDlc,
            CanId);

            return;
        }
        break;*/

        case CANIF_RXPDUUSERRXINDICATIONUL_CAN_NM:
#if defined(USE_CANNM)
        	CanNm_RxIndication(entry->CanIfCanRxPduId,CanSduPtr);
        	return;
#endif
        	break;

        case CANIF_RXPDUUSERRXINDICATIONUL_PDUR:
            // Send Can frame to PDU router
#if defined(USE_PDUR)
        	{
        		PduInfoType pduInfo;
        		pduInfo.SduLength = CanDlc;
        		pduInfo.SduDataPtr = (uint8 *)CanSduPtr;
            	PduR_CanIfRxIndication(entry->CanIfCanRxPduId,&pduInfo);
        	}
            return;
#endif
            break;

        case CANIF_RXPDUUSERRXINDICATIONUL_CAN_TP:
          // Send Can frame to CAN TP
#if defined(USE_CANTP)
            {
        	    PduInfoType CanTpRxPdu;
        	    CanTpRxPdu.SduLength = CanDlc;
        	    CanTpRxPdu.SduDataPtr = (uint8 *)CanSduPtr;
                CanTp_RxIndication(entry->CanIfCanRxPduId, &CanTpRxPdu);
            }
            return;
#endif
            break;
        case CANIF_RXPDUUSERRXINDICATIONUL_J1939TP:
          // Send Can frame to CAN TP
#if defined(USE_J1939TP)
            {
        	    PduInfoType J1939TpRxPdu;
        	    J1939TpRxPdu.SduLength = CanDlc;
        	    J1939TpRxPdu.SduDataPtr = (uint8 *)CanSduPtr;
        	    J1939Tp_RxIndication(entry->CanIfCanRxPduId, &J1939TpRxPdu);
            }
            return;
#endif
            break;
      }
    }

    entry++;
  }

  // Did not find the PDU, something is wrong
  VALIDATE(FALSE, CANIF_RXINDICATION_SID, CANIF_E_PARAM_LPDU);
}

/*---------------------------------------------------------------------*/

/*
 * Service ID[hex]: 0x14
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): Mailbox Identifies the HRH and its corresponding CAN Controller
					PduInfoPtr Pointer to the received L-PDU
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: This service indicates a successful reception of a received CAN Rx LPDU to the CanIf after passing all filters and validation checks.
 */
void CanIf_ControllerBusOff(uint8 Controller)
{
  CanIf_Arc_ChannelIdType channel = 0xff;

  VALIDATE( CanIf_Global.initRun, CANIF_CONTROLLERBUSOFF_SID, CANIF_E_UNINIT );

  for(int i = 0; i < CANIF_CHANNEL_CNT; i++)
  {
	  if(CanIf_ConfigPtr->Arc_ChannelToControllerMap[i] == Controller)
	  {
		  channel = i;
	  }
  }

  VALIDATE( Controller < CANIF_CHANNEL_CNT, CANIF_CONTROLLERBUSOFF_SID, CANIF_E_PARAM_CONTROLLERID );

  // According to figure 35 in canif spec this should be done in
  // Can driver but it is better to do it here
  CanIf_SetControllerMode(channel, CANIF_CS_STOPPED);

  if (CanIf_ConfigPtr->DispatchCfg->CanIfDispatchUserCtrlBusOff != NULL)
  {
    CanIf_ConfigPtr->DispatchCfg->CanIfDispatchUserCtrlBusOff(channel);
  }
}


/* TODO: add this function after adding can.h */
/*void CanIf_Arc_Error(uint8 Controller, Can_Arc_ErrorType Error)
{
  CanIf_Arc_ChannelIdType channel = 0xff;

  VALIDATE_NO_RV( CanIf_Global.initRun, CANIF_ARCERROR_ID, CANIF_E_UNINIT );

  for(int i = 0; i < CANIF_CHANNEL_CNT; i++)
  {
	  if(CanIf_ConfigPtr->Arc_ChannelToControllerMap[i] == Controller)
	  {
		  channel = i;
	  }
  }

  VALIDATE_NO_RV( channel < CANIF_CHANNEL_CNT, CANIF_ARCERROR_ID, CANIF_E_PARAM_CONTROLLER );

  if (CanIf_ConfigPtr->DispatchConfig->CanIfErrorNotificaton != NULL)
  {
    CanIf_ConfigPtr->DispatchConfig->CanIfErrorNotificaton(Controller, Error);
  }

  // Special fix for restart of bus in case of general can error i.e. connection to CanSM
  if (CanIf_ConfigPtr->DispatchConfig->CanIfBusOffNotification != NULL)
  {
    CanIf_ConfigPtr->DispatchConfig->CanIfBusOffNotification(channel);
  }
}*/

uint8 CanIf_Arc_GetChannelDefaultConfIndex(CanIf_Arc_ChannelIdType Channel)
{
	return CanIf_ConfigPtr->Arc_ChannelDefaultConfIndex[Channel];
}
