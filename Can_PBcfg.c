#include "Can_PBcfg.h"
/* Contains Initial values for the containers */

/* All the Nodes participating in the network will have the same baud rate configuration */
CanController CanController_arr[]
{
    {
		TRUE,
		CAN_0_BASE_ADDRESS,
		CAN_0_ID,
		CanControllerBaudrateConfig_arr
		/* Reference to system clock */
    },
	{
		TRUE,
		CAN_1_BASE_ADDRESS,
		CAN_1_ID,
		CanControllerBaudrateConfig_arr	
		/* Reference to system clock */
	},
	{
		TRUE,
		CAN_2_BASE_ADDRESS,
		CAN_2_ID,
		CanControllerBaudrateConfig_arr
        /* Reference to system clock */		
	}
};


CanControllerBaudrateConfig CanControllerBaudrateConfig_arr[]
{
	{
		/* Baud rate configuration */

	}
};

CanHwFilter CanHwFilterHTH[]{

    {
		/* There is no hardware filter for transmission, It needed to be exist for standard purposes */

    }
};

CanHwFilter CanHwFilterHRH[]{
	{
		/* ID */
		/* Mask for controller 0 */
	},
	{
		/* ID */
		/* Mask for controller 1 */
	},
	{
		/* ID */
		/* Mask for controller 2 */	
	}
};


CanHardwareObject CanHardwareObject_arr[]
{
  /* Transmitting buffer for each controller */
  {
	  FULL,
	  FALSE,
	  1,
	  STANDARD,
	  HTH0_0,
	  TRANSMIT,
	  FALSE,
	  &CanController_arr[0],
	  &CanHwFilterHTH[0]
  },
  {
	  FULL,
	  FALSE,
	  1,
	  STANDARD,
	  HTH1_0,
	  TRANSMIT,
	  FALSE,
	  &CanController_arr[1],
	  &CanHwFilterHTH[0]
  },
  {
	  FULL,
	  FALSE,
	  1,
	  STANDARD,
	  HTH2_0,
	  TRANSMIT,
	  FALSE,
	  &CanController_arr[2],
	  &CanHwFilterHTH[0]
  },
  /* Receiving buffer for each controller */
  {
	  FULL,
	  FALSE,
	  1,
	  STANDARD,
	  HRH0_0,
	  RECEIVE,
	  FALSE,
	  &CanController_arr[0],
	  &CanHwFilterHRH[0]	   
  },
  {
	  FULL,
	  FALSE,
	  1,
	  STANDARD,
	  HRH1_0,
	  RECEIVE,
	  FALSE,
	  &CanController_arr[1],
	  &CanHwFilterHRH[1]	   
  },
  {
	  FULL,
	  FALSE,
	  1,
	  STANDARD,
	  HRH2_0,
	  RECEIVE,
	  FALSE,
	  &CanController_arr[2],
	  &CanHwFilterHRH[2]	   
  },
};



Can_ConfigType Can_ConfigType_Handle
{
	CanController_arr,
	CanHardwareObject_arr
};
