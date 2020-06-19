#ifndef CAN_PBCFG_H
#define CAN_PBCFG_H

/* [SWS_Can_00034] */
#include "Can_Cfg.h"

/* Published information */
#define CAN_PB_CFG_MODULE_ID (80U)
#define CAN_PB_CFG_VENDOR_ID (88U)   

#define CAN_PB_CFG_AR_RELEASE_MAJOR_VERSION (4U)
#define CAN_PB_CFG_AR_RELEASE_MINOR_VERSION (3U)
#define CAN_PB_CFG_AR_RELEASE_REVISION_VERSION (1U)

#define CAN_PB_CFG_SW_MAJOR_VERSION (1U)
#define CAN_PB_CFG_SW_MINOR_VERSION (0U)
#define CAN_PB_CFG_SW_PATCH_VERSION (0U)

/* AUTOSAR checking between Std Types and Can_Cfg Module */
#if(CAN_PB_CFG_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION)\
	||(CAN_PB_CFG_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION)\
	||(CAN_PB_CFG_AR_RELEASE_REVISION_VERSION != STD_AR_RELEASE_REVISION_VERSION)
#error "The AR version of Std_Types.h does not match the expected version"
#endif

typedef struct
{
    /*Specifies the baudrate of the controller in kbps*/
    uint16 CanControllerBaudRate;
    /*Uniquely identifies a specific baud rate configuration. This ID is used by SetBaudrate API.*/
    uint16 CanControllerBaudRateConfigID;
    /*Specifies propagation delay in time quantas.*/
    uint8 CanControllerPropSeg;
    /*Specifies phase segment 1 in time quantas.*/
     uint8 CanControllerSeg1;
     /*Specifies phase segment 2 in time quantas.*/
     uint8 CanControllerSeg2;
     /*Specifies the synchronization jump width for the controller in time quantas*/
     uint8 CanControllerSyncJumpWidth;
}CanControllerBaudrateConfig;


typedef struct
{
    /* Defines if a CAN controller is used in the configuration */
    boolean CanControllerActivation;
    /*Specifies the CAN controller base address*/
    uint32 CanControllerBaseAddress;
    /*This parameter provides the controller ID which is unique in a given CAN Driver.
     The value for this parameter starts with 0 and continue without any gaps*/
    uint8 CanControllerId;
    /*Reference to baudrate configuration container configured for the Can Controller. Reference to [ CanControllerBaudrateConfig ]*/
    CanControllerBaudrateConfig* CanControllerDefaultBaudrate;

	/*Reference to the clock source from MCU Driver is missing, Will be added when finishing MCU driver */
}CanController;



/* Container "CanHWFilter" */
typedef struct
{
    /*Specifies (together with the filter mask) the identifiers range that passes the hardware filter.*/
    uint32 CanHwFilterCode;

    /*Describes a mask for hardware-based filtering of CAN identifiers. The CAN identifiers of incoming
     messages are masked with the appropriate CanFilterMaskValue. Bits holding a 0 mean don't care,
     i.e. do not compare the message's identifier in the respective bit position.The mask shall be
     build by filling with leading 0. In case of CanIdType EXTENDED or MIXED a 29 bit mask shall
     be build. In case of CanIdType STANDARD a 11 bit mask shall be build*/
    uint32 CanHwFilterMask;

}CanHwFilter;
/* End of Container "CanHWFilter" */


/* Container "CanHardwareObject" */

/*  This configuration element is used as information for the CAN Interface only. The relevant CAN driver configuration is done with the filter mask and identifier*/
typedef enum {BASIC,FULL}Can_HandleType;

typedef enum {RECEIVE,TRANSMIT}Can_ObjectType;
typedef struct
{
	Can_HandleType CanHandleType; /* Specifies the type (Full-CAN or Basic-CAN) of a hardware object */
	boolean CanHardwareObjectUsesPolling; /* Enables polling of this hardware object */
	uint16 CanHWObjectCount; /* Number of hardware objects used to implement one HOH. In case of a HRH this parameter defines the number of elements in the hardware FIFO or the number of shadow buffers, in case of a HTH it defines the number of hardware objects used for multiplexed transmission or for a hardware FIFO used by a FullCAN HTH */
	Can_IdType CanIdType; /* Standard, extended or mixed mode */
	Can_ObjectId CanObjectId;
	Can_ObjectType CanObjectType;
	boolean CanTriggerTransmitEnable; /* This parameter defines if or if not Can supports the trigger-transmit API for this handle */
	CanController* CanControllerRef;
	CanHwFilter* CanHWFilterRef;
}CanHardwareObject;
/* End of container "CanHardwareObject" */



typedef struct
{
    CanController* CanController_ptr;
    CanHardwareObject* CanHardwareObject_ptr;
    CanControllerBaudrateConfig* CanControllerBaudrateConfig_ptr;
}Can_ConfigType;



#endif      /* CAN_PBCFG_H */
