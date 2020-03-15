/* Can Pre-compile Configuration File */ 
#ifndef CAN_CFG_H
#define CAN_CFG_H

/* Published information */
#define CAN_CFG_MODULE_ID (80U)
#define CAN_CFG_VENDOR_ID (88U)   

#define CAN_CFG_AR_RELEASE_MAJOR_VERSION    (4U)
#define CAN_CFG_AR_RELEASE_MINOR_VERSION    (3U)
#define CAN_CFG_AR_RELEASE_REVISION_VERSION (1U)

#define CAN_CFG_SW_MAJOR_VERSION (1U)
#define CAN_CFG_SW_MINOR_VERSION (0U)
#define CAN_CFG_SW_PATCH_VERSION (0U)

/* AUTOSAR checking between Std Types and Can_Cfg Module */
#if(CAN_CFG_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION)\
	||(CAN_CFG_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION)\
	||(CAN_CFG_RELEASE_REVISION_VERSION != STD_TYPES_AR_RELEASE_REVISION_VERSION)
#error "The AR version of Std_Types.h does not match the expected version"
#endif


#define EXTENDED   ( (Can_IdType) 0x00U)
#define MIXED      ( (Can_IdType) 0x01U)
#define STANDARD   ( (Can_IdType) 0x02U)

#define INTERRUPT           (0x00U)
#define MIXED               (0x01U)
#define POLLING             (0x02U)
#define CAN_0_BASE_ADDRESS  (uint32)(0x40024000)
#define CAN_1_BASE_ADDRESS  (uint32)(0x40025000)
#define CAN_2_BASE_ADDRESS  (uint32)(0x4002B000)
#define CAN_0_ID            (uint8)(0x00U)
#define CAN_1_ID            (uint8)(0x01U)
#define CAN_2_ID            (uint8)(0x02U)

/* Container "CanGeneral */
#define CAN_DEV_ERROR_DETECT                  (STD_ON)    /* Switches the development error detection and notification on or off */
#define CAN_INDEX                             ( (uint8) 0x00U) /* Specifies the InstanceId of this module instance. If only one instance is present it shall have the Id 0*/
#define CAN_LPDU_RECEIVE_CALLOUT_FUNCTION     ( /* Callout function name that will be called after a successfull reception. This parameter can be omitted */
#define CAN_MAIN_FUNCTION_BUSOFF_PERIOD       ( (float32) (/*busOff period */) )  /*This parameter describes the period for cyclic call to Can_MainFunction_Busoff. Unit is seconds*/
#define CAN_MAIN_FUNCTION_MODE_PERIOD         ( (float32) (/*mode period */) )    /*This parameter describes the period for cyclic call to Can_MainFunction_Mode. Unit is seconds*/
#define CAN_MAIN_FUNCTION_WAKEUP_PERIOD       ( (float32) (/*wakeup period */) )  /*This parameter describes the period for cyclic call to Can_MainFunction_Wakeup. Unit is seconds*/
#define CAN_MULTIPLEXED_TRNSMISSION           (STD_OFF)   /*Specifies if multiplexed transmission shall be supported.ON or OFF*/
#define CAN_PUBLIC_ICOM_SUPPORT               (STD_OFF)   /*Selects support of Pretended Network features in Can driver*/
#define CAN_SET_BAUDRATE_API                  (STD_ON)
#define CAN_TIMEOUT_DURATION                  ( (float32) (/*timeout*/)
#define CAN_VERSION_INFO_API                  (STD_ON)
#define CAN_OS_COUNTER_REF                    (/*reference to os counter*/)
/* End of container "CanGeneral" */

/* Container "CanController" */
/* The following parameters are #defines because they are general for the whole software module */
/* They enable / disable some APIs */ 
#define CAN_BUSOFF_PROCESSING                 (INTERRUPT)    /*Enables / disables API Can_MainFunction_BusOff() for handling busoff events in polling mode*/
#define CAN_RX_PROCESSING                     (INTERRUPT)    /*Enables / disables API Can_MainFunction_Read() for handling PDU reception events in polling mode*/
#define CAN_TX_PROCESSING                     (INTERRUPT)
// Is CAN_WAKEUP_SUPPORT means that all wakeup based functions on or off or something else??
#define CAN_WAKEUP_SUPPORT                    (STD_OFF)
#define CAN_WAKEUP_FUNCTIONALITY_API      	  (STD_OFF)      /*Adds / removes the service Can_CheckWakeup() from the code*/
#define CAN_WAKEUP_PROCESSING                 (INTERRUPT) /*Enables / disables API Can_MainFunction_Wakeup() for handling wakeup events in polling mode*/
/* End of container "CanController" */

/* Container CanHardwareObject */
typedef uint16 Can_ObjectId;
#define HRH0_0 ( (Can_ObjectId) (0x00U))      /* Symbolic Names generated for this parameters */
#define HRH1_0 ( (Can_ObjectId) (0x01U))
#define HRH2_0 ( (Can_ObjectId) (0x02U))
#define HTH0_0 ( (Can_ObjectId) (0x00U))
#define HTH1_0 ( (Can_ObjectId) (0x01U))
#define HTH2_0 ( (Can_ObjectId) (0x02U))

/* End of container "CanHardwareObject" */


#endif      /* CAN_CFG_H */ 