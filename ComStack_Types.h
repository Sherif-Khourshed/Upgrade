/*
 * ComStack_Types.h
 *
 *  Created on: Mar 1, 2020
 *      Author: Sherif_Khourshed
 */

#ifndef COMSTACK_TYPES_H_
#define COMSTACK_TYPES_H_

/* Id for our team in the AUTOSAR, for example ID = 88) */
#define COMSTACK_TYPES_VENDOR_ID                  	(88U)

/* Module Id */
#define COMSTACK_TYPES_MODULE_ID                  	(196U)

/* Module Version 1.0.0 */
#define COMSTACK_TYPES_SW_MAJOR_VERSION				(1U)
#define COMSTACK_TYPES_SW_MINOR_VERSION           	(0U)
#define COMSTACK_TYPES_SW_PATCH_VERSION           	(0U)


/* AUTOSAR Version 4.3.1 */
#define COMSTACK_TYPES_AR_RELEASE_MAJOR_VERSION		(4U)
#define COMSTACK_TYPES_AR_RELEASE_MINOR_VERSION   	(3U)
#define COMSTACK_TYPES_AR_RELEASE_REVISION_VERSION  (1U)

#include "Std_Types.h"
/* AUTOSAR Version checking between Std Types and ComStackTypes Module */
#if(COMSTACK_TYPES_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION)\
	||(COMSTACK_TYPES_AR_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION)\
	||(COMSTACK_TYPES_AR_RELEASE_REVISION_VERSION != STD_TYPES_AR_RELEASE_REVISION_VERSION)
#error "The AR version of Std_Types.h does not match the expected version"
#endif

#include "ComStack_cfg.h"
/* AUTOSAR Version checking between ComStack_cfg.h and ComStackTypes Module */
#if(COMSTACK_TYPES_AR_RELEASE_MAJOR_VERSION != COMSTACK_CFG_AR_RELEASE_MAJOR_VERSION)\
	||(COMSTACK_TYPES_AR_RELEASE_MINOR_VERSION != COMSTACK_CFG_AR_RELEASE_MINOR_VERSION)\
	||(COMSTACK_TYPES_AR_RELEASE_REVISION_VERSION != COMSTACK_CFG_AR_RELEASE_REVISION_VERSION)
#error "The AR version of ComStack_cfg.h does not match the expected version"
#endif

/* Software Version checking between Dio_Cfg.h and Dio.h files */
#if(COMSTACK_TYPES_SW_MAJOR_VERSION != COMSTACK_CFG_SW_MAJOR_VERSION)\
	||(COMSTACK_TYPES_SW_MINOR_VERSION != COMSTACK_CFG_SW_MINOR_VERSION)\
	||(COMSTACK_TYPES_SW_PATCH_VERSION != COMSTACK_CFG_SW_PATCH_VERSION)
#error "The AR version of ComStack_cfg.h does not match the expected version"
#endif

/* store the basic information about a PDU of any type */
typedef struct
{
	uint8*			SduDataPtr;
	uint8*			MetaDataPtr;
	PduLengthType	SduLength;
}PduInfoType;

/* parameter to which the value has to be changed (BS or STmin) */
typedef enum
{
	TP_STMIN = 0x00,	/* Separation Time */
	TP_BS = 0x01,		/* Block Size */
	TP_BC = 0x02,		/* The Band width control parameter used in FlexRay transport protocol module */
}TPParameterType;

/* Variables of this type shall be used to store the result of a buffer request */
typedef enum
{
	BUFREQ_OK = 0x00,		/* Buffer request accomplished successful */
	BUFREQ_E_NOT_OK = 0x01,	/* Buffer request not successful. Buffer cannot be accessed */
	BUFREQ_E_BUSY = 0x02,	/* Temporarily no buffer available. It's up the requester to retry request for a certain time */
	BUFREQ_E_OVFL = 0x03,	/* No Buffer of the required length can be provided */
}BufReq_ReturnType;

/* Variables of this type shall be used to store the state of TP buffer */
typedef enum
{
	/* data, that have been copied so far, are confirmed and can be removed from the TP buffer.
	 Data copied by this API call are excluded and will be confirmed later*/
	TP_DATACONF = 0x00,
	/* this API call shall copy already copied data in order to recover from an error.
	 In this case TxTpDataCnt specifies the offset of the first byte to be copied by the API call */
	TP_DATARETRY = 0x01,
	/* The previously copied data must remain in the TP */
	TP_CONFPENDING = 0x02,
}TpDataStateType;

/* Variables of this type shall be used to store the information about Tp buffer handling */
typedef struct
{
	TpDataStateType	TpDataState;	/* The enum type to be used to store the state of Tp buffer */
	PduLengthType	TxTpDataCnt;	/* Offset from the current position which identifies the number of bytes to be retransmitted */
}RetryInfoType;

/* Variables of the type NetworkHandleType shall be used to store the identifier of a communication channel */
typedef uint8 NetworkHandleType;

/* defines the configuration ID. An ID of 0 is the default configuration.
 An ID greater than 0 shall identify a configuration for Pretended Networking.
 There is more than 1 configuration possible */
typedef uint8 IcomConfigIdType;

/* defines the errors which can occur when activating or deactivating Pretended Networking */
typedef enum
{
	ICOM_SWITCH_E_OK = 0x00,		/* The activation of Pretended Networking was successful */
	ICOM_SWITCH_E_FAILED = 0x01,	/* The activation of Pretended Networking was not successful */
}IcomSwitch_ErrorType;

#endif /* COMSTACK_TYPES_H_ */
