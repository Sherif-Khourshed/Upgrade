/*
 * CanIf_Cfg.h
 *
 *  Created on: Mar 18, 2020
 *      Author: Ahmed Nasr
 */

#ifndef CANIF_CFG_H_
#define CANIF_CFG_H_

/* Software version */
#define CANIF_CFG_SW_MAJOR_VERSION   (1U)
#define CANIF_CFG_SW_MINOR_VERSION   (0U)
#define CANIF_CFG_SW_PATCH_VERSION   (0U)

/* AUTOSAR version */
#define CANIF_CFG_AR_MAJOR_VERSION   (4U)
#define CANIF_CFG_AR_MINOR_VERSION   (3U)
#define CANIF_CFG_AR_PATCH_VERSION   (1U)

/* Identifiers for the elements in CanIfControllerConfig[] */
/* This is the ConfigurationIndex in CanIf_InitController() */
typedef enum {
	CANIF_CHL_0_CONFIG,					/* Configuration of CH0 */
    CANIF_CHANNEL_CONFIGURATION_CNT		/* Number of configured channels */
}CanIf_Arc_ConfigurationIndexType;

typedef enum {
	CANIF_CHL_0,						/* CAN_CTRL_0 */
    CANIF_CHANNEL_CNT					/* Number of channels */
}CanIf_Arc_ChannelIdType;

typedef enum {
	CAN_CTRL_0 = 0,						/* Can controller 0 */
	CAN_CONTROLLER_CNT					/* Number of Can controllers */
}CanControllerIdType;

typedef enum {
	CAN_CTRL_0_HTH,						/* Can controller 0 HTH */
	CAN_HTH_CNT							/* Number of HTHs */
} Can_Arc_HTHType;

typedef enum {
	RxHWObject,							/* Can controller 0 HRH */
	CAN_HRH_CNT							/* Number of HRHs */
} Can_Arc_HRHType;

typedef enum {
    CAN_ARC_HANDLE_TYPE_BASIC,
    CAN_ARC_HANDLE_TYPE_FULL
} Can_Arc_HohType;

/********************
 * CanIf Public Cfg *
 ********************/

/* Switches the development error detection and notification on or off. */
#define CANIF_DEV_ERROR_DETECT 							(STD_ON)

/* Enable support for dynamic ID handling using L-SDU MetaData. */
#define CANIF_META_DATA_SUPPORT							(STD_OFF)

/*
 * Configuration parameter to enable/disable dummy API for upper layer
 * modules which allows to request the cancellation of an I-PDU.
 */
#define CANIF_PUBLIC_CANCEL_TRANSMIT_SUPPORT			(STD_ON)

/*
 * Defines header files for callback functions which shall be included in
 * case of CDDs. Range of characters is 1.. 32.
 */
#define CANIF_PUBLIC_CDD_HEADER_FILE					("")

/* Selects support of Pretended Network features in CanIf. */
#define CANIF_PUBLIC_ICOM_SUPPORT						(STD_OFF)

/* Selects support for multiple CAN Drivers. */
#define CANIF_PUBLIC_MULTIPLE_DRV_SUPPORT				(STD_OFF)

/* Selects support of Partial Network features in CanIf. */
#define CANIF_PUBLIC_PN_SUPPORT							(STD_OFF)

/*
 * Enables / Disables the API CanIf_ReadRxPduData() for
 * reading received L-SDU data.
 */
#define CANIF_PUBLIC_READ_RX_PDU_DATA_API				(STD_OFF)		// Not supported

/*
 * Enables and disables the API for reading the notification
 * status of receive L-PDUs.
 */
#define CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API		(STD_OFF)		// Not supported

/*
 * Enables and disables the API for reading the notification
 * status of transmit L-PDUs.
 */
#define CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API		(STD_OFF)		// Not supported

/*
 * Enables and disables the API for reconfiguration of the CAN
 * Identifier for each Transmit L-PDU.
 */
#define CANIF_PUBLIC_SET_DYNAMIC_TX_ID_API				(STD_ON)

/*
 * Enables and disables the buffering of transmit L-PDUs
 * (rejected by the CanDrv) within the CAN Interface module.
 */
#define CANIF_PUBLIC_TX_BUFFERING						(STD_OFF)

/*
 * Configuration parameter to enable/disable the API to poll
 * for Tx Confirmation state.
 *
 * dependency: CAN State Manager module.
 */
#define CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT			(STD_OFF)

/* Selects support for wake up validation. */
#define CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT			(STD_OFF)

/*
 * If enabled, only NM messages shall validate a detected wake-up event
 * in CanIf. If disabled, all received messages corresponding to a
 * configured Rx PDU shall validate such a wake-up event. This
 * parameter depends on CanIfPublicWakeupCheckValidSupport and
 * shall only be configurable, if it is enabled.
 *
 * dependency: CanIfPublicWakeupCheckValidSupport.
 */
#if (CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT == STD_ON)
	#define CANIF_PUBLIC_WAKEUP_CHECK_VALID_BY_NM		(STD_OFF)
#endif

/*
 * Configuration parameter to enable/disable the CanIf_SetBaudrate API
 * to change the baud rate of a CAN Controller.
 */
#define CANIF_SET_BAUDRATE_API							(STD_OFF)

/*
 * Enables the CanIf_TriggerTransmit API at Pre-Compile-Time.
 * Therefore, this parameter defines if there shall be support for trigger
 * transmit transmissions.
 */
#define CANIF_TRIGGER_TRANSMIT_SUPPORT					(STD_ON)

/*
 * Determines wether TxOffLineActive feature (see SWS_CANIF_00072)
 * is supported by CanIf.
 */
#define CANIF_TX_OFFLINE_ACTIVE_SUPPORT					(STD_OFF)

/*
 * Enables and disables the API for reading the version information about
 * the CAN Interface.
 */
#define CANIF_VERSION_INFO_API							(STD_OFF)

/*
 * Enables the CanIf_CheckWakeup API at Pre-Compile-Time.
 * Therefore, this parameter defines if there shall be support for wake-up.
 */
#define CANIF_WAKEUP_SUPPORT							(STD_ON)

/*********************
 * CanIf Private Cfg *
 *********************/

/*
 * This parameter defines if the buffer element length shall be
 * fixed to 8 Bytes for buffers to which only PDUs < 8 Bytes are assigned.
 */
#define CANIF_FIXED_BUFFER								(STD_OFF)

/* Selects whether Data Length Check is supported. */
#define CANIF_PRIVATE_DATA_LENGTH_CHECK					(STD_ON)

/* Defines whether TTCAN is supported. */
#define CANIF_SUPPORT_TT_CAN							(STD_OFF)

/******************
 * CanIf Init Cfg *
 ******************/

/*
 * Determines if or if not CanIf shall use the trigger transmit API for this
 * PDU.
 *
 * dependency: If CanIfTxPduTriggerTransmit is TRUE then
 * CanIfTxPduUserTxConfirmationUL has to be either PDUR or CDD and
 * CanIfTxPduUserTriggerTransmitName has to be specified accordingly.
 */
//#define CANIF_TX_PDU_TRIGGER_TRANSMIT					(STD_OFF)

/* Enables/disables truncation of PDUs that exceed the configured size. */
//#define CANIF_TX_PDU_TRUNCATION							(STD_ON)

/******************
 * CanIf Arch Cfg *
 ******************/

#define CANIF_ARC_RUNTIME_PDU_CONFIGURATION 			(STD_ON)

#endif
