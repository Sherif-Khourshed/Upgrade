/*
 * Can_PBcfg.c
 *
 *  Created on: Mar 8, 2020
 *      Author: Sherif_Khourshed
 */

#include "Can.h"

McuClockReferencePoint Clk = CLOCK;

/*******************************************************************
                 PBcfg_Can_Controller_Baudrate_Config
 ******************************************************************/
CanControllerBaudrateConfig	BaudrateConfig[] =
{
		{
				500,				 /* CanControllerBaudRate */
				CONTROLLER_0_BDR_ID, /* CanControllerBaudRateConfigID */
				3,					 /* Propagation segment time = (PROPSEG + 1) × time-quanta*/
				10,					 /* Phase Buffer Segment 1 = (PSEG1 + 1) × Time-Quanta */
				2,					 /* Phase Buffer Segment 2 = (PSEG2 + 1) × Time-Quanta */
				2					 /* Resync Jump Width = RJW + 1 */
		}

};


/*******************************************************************
                      PBcfg_Can_Controller_Container
 ******************************************************************/
CanController CanControllerConfig[] =
{
		/* CAN_ 0 */
		{
				CAN_0_BASE_ADDRESS,	/* CanControllerBaseAddress */
				CAN_0_ID,			/* CanControllerId */
				&BaudrateConfig[0]	/* Reference to baud rate configuration */
		},
		/* CAN_1 */
		{
				CAN_1_BASE_ADDRESS,	/* CanControllerBaseAddress */
				CAN_1_ID,			/* CanControllerId */
				&BaudrateConfig[0]	/* Reference to baud rate configuration */
		},
		/* CAN_2 */
		{
				CAN_2_BASE_ADDRESS,	/* CanControllerBaseAddress */
				CAN_2_ID,			/* CanControllerId */
				&BaudrateConfig[0]	/* Reference to baud rate configuration */
		}
};

/*******************************************************************
                  PBcfg_Can_Hardware_Object
 ******************************************************************/
CanHardwareObject CanHardwareObjectConfig[] =
{
		{
				FULL,					/* Can_HandleType */
				STANDARD,				/* CanIdType */
				TRANSMIT,				/* CanObjectType */
				FALSE,					/* CanHardwareObjectUsesPolling */
				CONTROLLERS_NUMBER,		/* CanHWObjectCount */
				HTH0_0,					/* CanIdType */
				FALSE,					/* CanTriggerTransmitEnable */
				&CanControllerConfig[0]	/* CanControllerRef */
		},
		{
				FULL,					/* Can_HandleType */
				STANDARD,				/* CanIdType */
				RECEIVE,				/* CanObjectType */
				FALSE,					/* CanHardwareObjectUsesPolling */
				CONTROLLERS_NUMBER,		/* CanHWObjectCount */
				HRH0_0,					/* CanIdType */
				FALSE,					/* CanTriggerTransmitEnable */
				&CanControllerConfig[0]	/* CanControllerRef */
		}
};


/*******************************************************************
                     PBcfg_Can_Hardware_Filter
 ******************************************************************/
CanHwFilter CanHwFilterConfig[] =
{

};


/*******************************************************************
                     PBcfg_Can_Config_Set
 ******************************************************************/
const Can_ConfigType Can_Config = {CanControllerConfig,CanHardwareObjectConfig};
