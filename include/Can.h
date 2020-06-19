#ifndef CAN_H
#define CAN_H
/* [SWS_Can_00034] */
#include "ComStack_Types.h"
#include "Can_GeneralTypes.h"
#include "Can_Cfg.h"
#include "Can_PBcfg.h"

/* Published information */
#define CAN_MODULE_ID (80U)
#define CAN_VENDOR_ID (88U)   

#define CAN_AR_RELEASE_MAJOR_VERSION (4U)
#define CAN_AR_RELEASE_MINOR_VERSION (3U)
#define CAN_AR_RELEASE_REVISION_VERSION (1U)

#define CAN_SW_MAJOR_VERSION (1U)
#define CAN_SW_MINOR_VERSION (0U)
#define CAN_SW_PATCH_VERSION (0U)

/* AUTOSAR checking between Std Types and Can Module */
#if(CAN_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION)\
	||(CAN_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION)\
	||(CAN_AR_RELEASE_REVISION_VERSION != STD_AR_RELEASE_REVISION_VERSION)
#error "The AR version of Std_Types.h does not match the expected version"
#endif


/* [SWS_Can_00039]Can_ReturnType: Overlayed return value of Std_ReturnType for CAN driver API Can_Write() */
#define CAN_BUSY ( (Std_ReturnType) 0x02U)

#define HW_Register(Add) (*(volatile uint32*)(Add))

/*Some Definitions related to HW*/

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

/*some bits in MCR (Reset Value =  D890_000Fh = 1101 1000 1001 0000 0000 0000 0000 1111b)*/    
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

/*some bits in CTRL1 (Reset Value = 0000_0000h) */
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
#define PCC_BASE_ADDRESS (uint32)(0x40065000)

/* Message buffers related informations */ 
#define FIRSTOFFSET                    (uint32)(0x0800U)
#define MASKOFFSET                     (uint32)(0x880U)
#define IFLAGOFFSET     			   (uint32)(0x30U)
#define NUM_OF_TX_BUFFERS_0            (uint8)(0x01U)
#define NUM_OF_RX_BUFFERS_0            (uint8)(0x01U)
#define MB_CS_INACTIVE                 (0x8U)
#define MB_CS_DATA                     (0xCU)



#define NUM_OF_TX_BUFFERS_1              (uint8)(0x01U)
#define NUM_OF_RX_BUFFERS_1              (uint8)(0x01U)

#define NUM_OF_TX_BUFFERS_2              (uint8)(0x01U)
#define NUM_OF_RX_BUFFERS_2              (uint8)(0x01U)
/* Development Errors Definitions */ 
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

/* This type is a software flag to indicate the change in the interrupt state by the functions */

#define NUM_OF_CONTROLLERS             (uint8)(0x03U)     
uint32* RAMn;        /* Message buffers array */
uint32* RXIMRn;          /* RX individual mask registers */
uint32 IFLAG1_Reg;
/*Functions Declaration*/
void Can_Init( const Can_ConfigType* Config );
void Can_GetVersionInfo( Std_VersionInfoType* versioninfo );
void Can_DeInit( void );
#if (CAN_SET_BAUDRATE_API == STD_ON)
Std_ReturnType Can_SetBaudrate( uint8 Controller, uint16 BaudRateConfigID );
#endif /* CAN_SET_BAUDRATE_API */
Std_ReturnType Can_SetControllerMode( uint8 Controller, Can_ControllerStateType Transition );
void Can_DisableControllerInterrupts( uint8 Controller );
void Can_EnableControllerInterrupts( uint8 Controller );
Std_ReturnType Can_CheckWakeup( uint8 Controller );
Std_ReturnType Can_GetControllerErrorState( uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr );
Std_ReturnType Can_GetControllerMode( uint8 Controller, Can_ControllerStateType* ControllerModePtr );
Std_ReturnType Can_Write( Can_HwHandleType Hth, const Can_PduType* PduInfo );

#endif     /* CAN_H */
