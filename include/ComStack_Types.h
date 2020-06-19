/*[SWS_Comtype_00015] Because many of the communication 
stack type are depending on the appropriate ECU, 
this file shall be generated dependent on the specific 
ECU configuration for each ECU independently*/

#ifndef COM_STACK_TYPES_H
#define COM_STACK_TYPES_H

#include "Std_Types.h"
#include "ComStack_cfg.h"
#define COM_TYPE_VENDOR_ID                  (88U)
#define COM_TYPE_MODULE_ID                   (196U)
/* Module Version 1.0.0 */
#define COM_TYPE_SW_MAJOR_VERSION			(1U)
#define COM_TYPE_SW_MINOR_VERSION           (0U)
#define COM_TYPE_SW_PATCH_VERSION           (0U)


/* AUTOSAR Version 4.3.1 */
#define COM_TYPE_AR_RELEASE_MAJOR_VERSION	(4U)
#define COM_TYPE_AR_RELEASE_MINOR_VERSION   (3U)
#define COM_TYPE_AR_RELEASE_REVISION_VERSION   (1U)

/* AUTOSAR checking between Std Types and ComStack_Types Module */
#if(COM_TYPE_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION)\
	||(COM_TYPE_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION)\
	||(COM_TYPE_AR_RELEASE_REVISION_VERSION != STD_AR_RELEASE_REVISION_VERSION)
#error "The AR version of Std_Types.h does not match the expected version"
#endif

/* 3)[SWS_COMTYPE_00011]PduInfoType: Variables of this type shall
be used to store the basic information about a PDU of any type, 
namely a pointer variable pointing to its SDU (payload), 
a pointer to Meta Data of the PDU, and the corresponding length of the SDU in bytes*/
typedef struct  
{
	/*Pointer to the SDU (i.e. payload data) of the PDU*/
	uint8* SduDataPtr;
	/*Pointer to the meta data (e.g. CAN ID, socket ID, diagnostic addresses) of the PDU, 
	consisting of a sequence of meta data items. The length and type of the meta data items 
	is statically configured for each PDU. Meta data items with more than 8 bits use platform byte order*/ 
	uint8* MetaDataPtr;
	/*Length of the SDU in bytes*/
	PduLengthType SduLength;
}PduInfoType;

/* 4)[SWS_COMTYPE_00036]PNCHandleType: Used to store the identifier of a partial network cluster*/
/*We will not use this type but we will define it anyway*/
typedef uint8 PNCHandleType;

/* 5)[SWS_COMTYPE_00031]TPParameterType: (Enumeration) Specify the parameter to which the value has to be changed (BS or STmin)*/
typedef enum 
{
	TP_STMIN,  /*Separation Time*/
	TP_BS,     /*Block Time*/
	TP_BC     /*The Band width control parameter used in FlexRay transport protocol module*/
}TPParameterType;

/* 6)[SWS_COMTYPE_00012]BufReq_ReturnType: (Enumeration) Variables of this type shall be used to store the result of a buffer request*/
typedef enum
{
	BUFREQ_OK,        /*Buffer request accomplished successful*/
	BUFREQ_E_NOT_OK,  /*Buffer request not successful. Buffer cannot be accessed*/
	BUFREQ_E_BUSY,    /*Temporarily no buffer available. It's up the requester to retry request for a certain time*/
	BUFREQ_E_OVFL    /* No Buffer of the required length can be provided*/
}BufReq_ReturnType;

/* 7)[SWS_COMTYPE_00027]TpDataStateType: (Enumeration) Variables of this type shall be used to store the state of TP buffer*/
typedef enum 
{
	TP_DATACONF,       /*indicates that all data, that have been copied so far, are confirmed and can be removed from the TP buffer. Data copied by this API call are excluded and will be confirmed later*/  
	TP_DATARETRY,      /*indicates that this API call shall copy already copied data in order to recover from an error. In this case TxTpDataCnt specifies the offset of the first byte to be copied by the API call*/
	TP_CONFPENDING     /*indicates that the previously copied data must remain in the TP*/	
}TpDataStateType;

/* 8)[SWS_COMTYPE_00037]RetryInfoType: Variables of this type shall be used to store the information about Tp buffer handling*/ 
typedef struct
{
	TpDataStateType TpDataState;
	PduLengthType TxTpDataCnt;
}RetryInfoType;

/* 9)[SWS_COMTYPE_00038]NetworkHandleType: Variables of the type NetworkHandleType shall be used to store the identifier of a communication channel*/
typedef uint8 NetworkHandleType;

/* 10)[SWS_COMTYPE_00039]IcomConfigIdType: defines the configuration ID. An ID of 0 is the default configuration. 
An ID greater than 0 shall identify a configuration for Pretended Networking. There is more than 1 configuration possible*/
typedef uint8 IcomConfigIdType;

/* 11)[SWS_COMTYPE_00040]IcomSwitch_ErrorType: (Enumeration) defines the errors which can occur when activating or deactivating Pretended Networking*/
typedef enum 
{
	ICOM_SWITCH_E_OK,       /*The activation of Pretended Networking was successful*/
	ICOM_SWITCH_E_FAILED    /* The activation of Pretended Networking was not successful*/	
}IcomSwitch_ErrorType;

#endif      /*COM_STACK_TYPES_H*/
