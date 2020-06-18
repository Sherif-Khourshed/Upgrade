#include "Can.h"
#include "Det.h"
#include "S32_core_cm4.h"
static Can_ModuleStateType CAN_MODULE_STATE = CAN_UNINIT;    /* Very important variable used to hold the current state of the can driver */
static Can_ControllerStateType CAN_CONTROLLERS_STATES[] = {CAN_CS_UNINIT,CAN_CS_UNINIT,CAN_CS_UNINIT};
static uint32 CAN_BASE_ADDRESSES[] = {CAN_0_BASE_ADDRESS, CAN_1_BASE_ADDRESS, CAN_2_BASE_ADDRESS};
static Can_ConfigType* TempConfig = NULL_PTR;   /* Temporary configuration pointer to store the configuration for Can_SetControllerMode(CAN_CS_STARTED) usage */
static uint32 PCC_CAN_OFFSETS[3] = {0x90,0x94,0xAC};
static uint8 TotalNumOfBuffers[NUM_OF_CONTROLLERS] = {NUM_OF_TX_BUFFERS_0 + NUM_OF_RX_BUFFERS_0, NUM_OF_TX_BUFFERS_1 + NUM_OF_RX_BUFFERS_1, NUM_OF_TX_BUFFERS_2 + NUM_OF_RX_BUFFERS_2};
static uint8 TotalTxSysBuf = NUM_OF_TX_BUFFERS_0 + NUM_OF_TX_BUFFERS_1 + NUM_OF_TX_BUFFERS_2;
static uint8 TotalRxSysBuf = NUM_OF_RX_BUFFERS_0 + NUM_OF_RX_BUFFERS_1 + NUM_OF_RX_BUFFERS_2;
static uint8 MAXMB[] = {128, 64, 64};
static uint8 NumOfIntDis[NUM_OF_CONTROLLERS] = {1,1,1};   /* Number of calls of the Can_DisableControllerInterrupts() Function for each controller (1 because the interrupts are initially disabled) */
static uint8 BufferCounter = 0;   /* Counter to count inside the hardware object array */
static uint8 TxBufferCounter = 0;
static uint8 RxBufferCounter = 0;
static uint8 RegCounter = 0;
/* The following two types are implying that when the user wants to write any message on the can bus,
   he will choose the buffer from the Tx array, These 2 arrays are filled from the initialization function
   according to the PBcfg.h file.
*/
static CanHardwareObject Can_HTH_Array[TotalTxSysBuf];    /* Array that will hold the Tx buffers informations */
static CanHardwareObject Can_HRH_Array[TotalRxSysBuf];    /* Array that will hold the Rx buffers informations */

void Can_Init( const Can_ConfigType* Config )
{
	DISABLE_INTERRUPTS()      /* Macro in S32_core_cm4.h */
	uint8 Can_ControllerIndex = 0;
	uint8 BufferConfigCounter = 0;
	uint8 RAMCounter = 0;
	/* [SWS_Can_00174] If development error detection for the Can module is enabled:
	The function Can_Init shall raise the error CAN_E_TRANSITION if the driver is not in state CAN_UNINIT */
	/* [SWS_Can_00408] If development error detection for the Can module is enabled:
	The function Can_Init shall raise the error CAN_E_TRANSITION if the CAN controllers are not in state UNINIT */
	#if (CAN_DEV_ERROR_DETECT == STD_ON)
		if (NULL_PTR == Config)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x00U, CAN_E_PARAM_POINTER);
		}
		else if ( (CAN_UNINIT != CAN_MODULE_STATE) || (CAN_CS_UNINIT != CAN_CONTROLLERS_STATES[0]) || (CAN_CS_UNINIT != CAN_CONTROLLERS_STATES[1]) || (CAN_CS_UNINIT != CAN_CONTROLLERS_STATES[2]) )
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x00U, CAN_E_TRANSITION);
		}
		else
		{
			/* MISRA */
		}
	#endif
	*TempConfig = *Config;     /* Storing the configurations for future usage */
	/* [SWS_Can_00259] The function Can_Init shall set all CAN controllers in the state STOPPED */
	/* It will call the function Can_SetControllerMode(CAN_CS_STOPPED) which is not implemented yet */
	for (Can_ControllerIndex = 0 ; Can_ControllerIndex < NUM_OF_CONTROLLERS ; Can_ControllerIndex++)
	{
		if (TRUE == (Config -> (CanController_ptr[Can_ControllerIndex].CanControllerActivation)) )
		{
			/* We must enable the clock for the CAN controller through the PCC module */
			/* This must be done by the MCU driver */
			/* Until implementing the MCU driver, the Clock will be enabled manually */
			HW_Register(PCC_BASE_ADDRESS + PCC_CAN_OFFSETS[Can_ControllerIndex]) |= (1 << 30);

			/* Set the controller in the freeze mode for initialization */
			Can_SetControllerMode(Can_ControllerIndex, CAN_CS_STOPPED);
			/* The next line shall be implemented in the function Can_SetControllerMode */
			// CAN_CONTROLLERS_STATES[Can_ControllerIndex] = CAN_CS_STOPPED;

			RAMn = (Config -> (CanController_ptr[Can_ControllerIndex].CanControllerBaseAddress + FIRSTOFFSET);       /* First element in the array is the first message buffer */
			RXIMRn = (Config -> (CanController_ptr[Can_ControllerIndex].CanControllerBaseAddress + MASKOFFSET);
			/* MCR Initialization */
			HW_Register((Config -> (CanController_ptr[Can_ControllerIndex].CanControllerBaseAddress + MCR)) &= ~(1 << MDIS);  /* Enable the Can Controller */
			HW_Register((Config -> (CanController_ptr[Can_ControllerIndex].CanControllerBaseAddress + MCR)) |= ( (1 << AEN) | (1 << IRMQ) );   /* Enable Abort enable feature */

			/* CTRL1 Initialization */
			#if (CAN_SET_BAUDRATE_API == STD_OFF)
			HW_Register((Config->CanController_ptr[Can_ControllerIndex].CanControllerBaseAddress)+CTRL1) |= ((Config->CanController_ptr[Can_ControllerIndex].CanControllerDefaultBaudrate[0].CanControllerPropSeg)<<PROPSEG) | ((Config->CanController_ptr[Can_ControllerIndex].CanControllerDefaultBaudrate[0].CanControllerSeg1)<<PSEG1) | ((Config->CanController_ptr[Can_ControllerIndex].CanControllerDefaultBaudrate[0].CanControllerSeg2)<<PSEG2) | ((Config->CanController_ptr[Can_ControllerIndex].CanControllerDefaultBaudrate[0].CanControllerSyncJumpWidth)<<RJW) | (1 << ERRMSK) | (1 << BOFFREC) ;
			#endif

			/* Message Buffers Initialization :
			a. The control and status word of all message buffers must be initialized.
			b. If Rx FIFO was enabled, the ID filter table must be initialized.
			c. Other entries in each message buffer should be initialized as required
			*/
			for (RAMCounter = 0; RAMCounter < MAXMB[Can_ControllerIndex]; ++RAMCounter)  /* FlexCAN0 32 MB, FlexCAN1,2 = 16 MB */
			{
				RAMn[RAMCounter] = 0;    /* Clearing all message buffers */
			}

			/* The FlexCan0 MAXMB bits (located in MCR register) were initialized as the reset value = 16 buffers */
			/* But we will be using 2 buffers for each module */
			/* The user can configure it through NUM_OF_TX_BUFFERS, NUM_OF_RX_BUFFERS located in Can.h */
			/* The number configured by the user must be consistent with the Can_PBcfg.c file */   /*Very Important*/
			/* The user must define the hardware objects in order of 0, 1, 2 */
			for (BufferConfigCounter = 0 ; BufferConfigCounter < (TotalNumOfBuffers[Can_ControllerIndex]) ; BufferConfigCounter+=4)
			{
				if (STANDARD == Config->CanHardwareObject_ptr[(BufferCounter) ].CanIdType)
				{
					/* Writing to MB IDE bit (21) */
					RAMn[BufferConfigCounter] |= (1 << 21);

					if (TRANSMIT == Config->CanHardwareObject_ptr[BufferCounter].CanObjectType)
					{
						/* Fill the Tx buffers array */
						Can_HTH_Array[TxBufferCounter++] = CanHardwareObject_arr[BufferCounter];

						/* Writing the MB code as TX INACTIVE */
						/* It is important to write the buffer as inactive as that will help checking if the buffer busy or not */
						RAMn[BufferConfigCounter] |= (8 << 24);	/* Code = 0b1000 in bits (24 - 27) */

					}
					else
					{
						/* Fill the Rx buffers array */
						Can_HRH_Array[RxBufferCounter++] = CanHardwareObject_arr[BufferCounter];
						/* Writing the MB code as RX EMPTY */
						RAMn[BufferConfigCounter] |= (4 << 24);	/* Code = 0b0100 in bits (24 - 27) */

						/* Writing the corresponding MASK */
						RXIMRn[RegCounter] = Config->CanHardwareObject_ptr[BufferCounter].CanHWFilterRef->CanHwFilterMask;
					}
				}

				else /* The ID is extended */
				{
					/* Writing to MB IDE bit (21) */
					RAMn[BufferConfigCounter] &= ~(1 << 21);

					if (TRANSMIT == Config->CanHardwareObject_ptr[BufferCounter].CanObjectType)
					{
						/* Fill the Tx buffers array */
						Can_HTH_Array[TxBufferCounter++] = CanHardwareObject_arr[BufferCounter];

						/* Writing the MB code as TX INACTIVE */
						/* It is important to write the buffer as inactive as that will help checking if the buffer busy or not */
						RAMn[BufferConfigCounter] |= (8 << 24);	/* Code = 0b1000 in bits (24 - 27) */
					}
					else
					{
						/* Fill the Rx buffers array */
						Can_HRH_Array[RxBufferCounter++] = CanHardwareObject_arr[BufferCounter];

						/* Writing the MB code as RX EMPTY */
						RAMn[BufferConfigCounter] |= (4 << 24);	/* Code = 0b0100 in bits (24 - 27) */

						/* Writing the corresponding MASK */
						RXIMRn[RegCounter] = Config->CanHardwareObject_ptr[BufferCounter].CanHWFilterRef->CanHwFilterMask;
					}
				}
				BufferCounter++;
				RegCounter++;
			}
		}

		else
		{
			/* Disable the Controller */
			HW_Register((Config -> (CanController_ptr[Can_ControllerIndex].CanControllerBaseAddress + MCR)) |= (1 << MDIS);
		}
	}

	CAN_MODULE_STATE = CAN_READY;

	ENABLE_INTERRUPTS()
}

void Can_GetVersionInfo( Std_VersionInfoType* versioninfo )
{
	#if (CAN_DEV_ERROR_DETECT == STD_ON)
		if (NULL_PTR == versioninfo)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x07U, CAN_E_PARAM_POINTER);
		}
		else
		{
			/* MISRA */
		}
	#endif      /* CAN_DEV_ERROR_DETECT */
	versioninfo->vendorID = CAN_VENDOR_ID;
	versioninfo->moduleID = CAN_MODULE_ID;
	versioninfo->sw_major_version = CAN_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = CAN_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = CAN_SW_PATCH_VERSION;

	return;
}

void Can_DeInit( void )
{
	DISABLE_INTERRUPTS()         /* Non-reentrant function */
	#if (CAN_DEV_ERROR_DETECT == STD_ON)
		if (CAN_READY != CAN_MODULE_STATE)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x10U,  CAN_E_TRANSITION );
		}
		else if ( (CAN_CS_STARTED == CAN_CONTROLLERS_STATES[0]) || (CAN_CS_STARTED == CAN_CONTROLLERS_STATES[1]) || (CAN_CS_STARTED == CAN_CONTROLLERS_STATES[2]) )
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x10U,  CAN_E_TRANSITION );
		}
		else
		{
			/* MISRA */
		}
	#endif      /* CAN_DEV_ERROR_DETECT */
	uint8 Can_ControllerIndex = 0;
	for (Can_ControllerIndex = 0 ; Can_ControllerIndex < NUM_OF_CONTROLLERS ; Can_ControllerIndex++)
	{
		HW_Register(CAN_BASE_ADDRESSES[Can_ControllerIndex] + MCR) |= (1 << MDIS); /* Disable the controller */
		CAN_CONTROLLERS_STATES[Can_ControllerIndex] = CAN_CS_UNINIT;
	}
	CAN_MODULE_STATE = CAN_UNINIT;
	ENABLE_INTERRUPTS()
	return;
}



#if (CAN_BUSOFF_PROCESSING == POLLING)
/* Can_MainFunction_BusOff() implementation here */
#endif /*CAN_BUSOFF_PROCESSING*/

#if (CAN_TX_PROCESSING == POLLING)
/* Can_MainFunction_Write() implementation here */
#endif /*CAN_TX_PROCESSING*/

#if (CAN_RX_PROCESSING == POLLING)
/* Can_MainFunction_Read() implementation here */
#endif /*CAN_RX_PROCESSING*/


/* The user can select one of the baud rates from the baud rates array located in the PBcfg.c file */
/* I think the user supposed to initialize the CAN controller without writing the bit timing When this API is enabled */
/* When this API is enabled, after initialization the user supposed to Call this service to configure bit timing */
/* When this function is disabled, the initialization function will write bit timing as the element number zero (BaudrateConfigID=0) */
/* The Caller of this function must call first Can_SetControllerMode(CAN_CS_STOPPED) before calling this function or call it after initialization before starting the controller */
#if (CAN_SET_BAUDRATE_API == STD_ON)
Std_ReturnType Can_SetBaudrate( uint8 Controller, uint16 BaudRateConfigID )
{
	#if (CAN_DEV_ERROR_DETECT == STD_ON)
		if (CAN_READY != CAN_MODULE_STATE)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x0fU, CAN_E_UNINIT );
			return E_NOT_OK;
		}
		else if (BaudRateConfigID > 65535)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x0fU,  CAN_E_PARAM_BAUDRATE  );
			return E_NOT_OK;
		}
		else if (Controller > NUM_OF_CONTROLLERS)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x0fU, CAN_E_PARAM_CONTROLLER );
			return E_NOT_OK;

		}
		/* Need Discussion : [SWS_CAN_00500] If the requested baud rate change can not performed without a re-initialization of the CAN Controller E_NO_OK shall be returned */
		else if (CAN_CS_STOPPED != CAN_CONTROLLERS_STATES[Controller])
		{
			return E_NOT_OK;
		}
		else
		{
			/* MISRA */
		}
	#endif /* CAN_DEV_ERROR_DETECT */
	/* Selecting Baudrate configuration from the array of configurations according to ConfigID */
	HW_Register((CAN_BASE_ADDRESSES[Controller]+CTRL1) |= (CanControllerBaudrateConfig_arr[BaudRateConfigID].CanControllerPropSeg)<<PROPSEG) | (CanControllerBaudrateConfig_arr[BaudRateConfigID].CanControllerSeg1)<<PSEG1) | ((CanControllerBaudrateConfig_arr[BaudRateConfigID].CanControllerSeg2)<<PSEG2) | ((CanControllerBaudrateConfig_arr[BaudRateConfigID].CanControllerSyncJumpWidth)<<RJW) | (1 << ERRMSK) | (1 << BOFFREC) ;
	return E_OK;
}
#endif   /* CAN_SET_BAUDRATE_API */

/* Only Can_DeInit() function can set the controller in the state CAN_CS_UNINIT */
/* The request of Can_SetControllerMode(CAN_CS_UNINIT) is an invalid request */

Std_ReturnType Can_SetControllerMode( uint8 Controller, Can_ControllerStateType Transition )
{
	DISABLE_INTERRUPTS()       /* This function is non-reentrant */
	RegCounter = 0;
	uint8 RAMCounter = 0;
	uint8 BufferConfigCounter = 0;
    #if (CAN_DEV_ERROR_DETECT==STD_ON)
		if (CAN_READY != CAN_MODULE_STATE)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x03U, CAN_E_UNINIT );
			return E_NOT_OK;
		}
		else if (Controller > NUM_OF_CONTROLLERS)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x03U, CAN_E_PARAM_CONTROLLER );
			return E_NOT_OK;

		}
		else if (CAN_CS_STARTED != Transition || CAN_CS_STOPPED != Transition || CAN_CS_SLEEP != Transition || CAN_CS_UNINIT == Transition)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x03U,  CAN_E_TRANSITION  );
			return E_NOT_OK;
		}
		else
		{
			/* MISRA */
		}

    #endif    /* CAN_DEV_ERROR_DETECT */
	switch ( Transition )
	{
		/* [SWS_Can_00384]: Each time the CAN controller state machine is triggered with the state transition value
		CAN_CS_STARTED, the function Can_SetControllerMode shall re-initialize the CAN controller with the same
		controller configuration set previously used by functions Can_SetBaudrate or Can_Init */
		/* [SWS_Can_00196]: The function Can_SetControllerMode shall enable / disable interrupts that are needed in the new state */
		/* Interrupts needed in the state STARTED: message buffer interrupts, busoff, error so we will call Can_EnableControllerInterrupts() */
		/* Interrupts needed in the states SLEEP, STOPPED : none so we will call Can_DisableControllerInterrupts() */
		case CAN_CS_STARTED:
			if (TRUE == (TempConfig -> (CanController_ptr[Controller].CanControllerActivation)) )
			{
				RAMn = (TempConfig -> (CanController_ptr[Controller].CanControllerBaseAddress + FIRSTOFFSET);       /* First element in the array is the first message buffer */
				RXIMRn = (TempConfig -> (CanController_ptr[Controller].CanControllerBaseAddress + MASKOFFSET);
				/* Re-initialize the selected controller according to the original configuration */
				/* will use the configurations of Can_init() to re-initialize */

				/* Enabling of The Clock using the PCC Module */
				HW_Register(PCC_BASE_ADDRESS + PCC_CAN_OFFSETS[Controller]) |= (1 << 30);

				/* Entering Freeze mode for re-initialization */
				HW_Register((TempConfig -> (CanController_ptr[Controller].CanControllerBaseAddress + MCR)) |= ( (1 << MDIS) | (1 << HALT) );
				while(! ((HW_Register((TempConfig -> (CanController_ptr[Controller].CanControllerBaseAddress + MCR))) & (1 << FRZACK) )));

				/* re-initialization of the MCR */
				HW_Register((TempConfig -> (CanController_ptr[Controller].CanControllerBaseAddress + MCR)) &= ~(1 << MDIS);  /* Enable the Can Controller */
				HW_Register((TempConfig -> (CanController_ptr[Controller].CanControllerBaseAddress + MCR)) |= ( (1 << AEN) | (1 << IRMQ) );   /* Enable Abort enable feature */

				/* re-initialization of the CTRL1 */
				#if (CAN_SET_BAUDRATE_API == STD_OFF)
				HW_Register((TempConfig->CanController_ptr[Controller].CanControllerBaseAddress)+CTRL1) |= ((TempConfig->CanController_ptr[Controller].CanControllerDefaultBaudrate[0].CanControllerPropSeg)<<PROPSEG) | ((TempConfig->CanController_ptr[Controller].CanControllerDefaultBaudrate[0].CanControllerSeg1)<<PSEG1) | ((TempConfig->CanController_ptr[Controller].CanControllerDefaultBaudrate[0].CanControllerSeg2)<<PSEG2) | ((TempConfig->CanController_ptr[Controller].CanControllerDefaultBaudrate[0].CanControllerSyncJumpWidth)<<RJW) | (1 << ERRMSK) | (1 << BOFFREC) ;
				#endif

				/* Enabling Controller Interrupts */
				Can_EnableControllerInterrupts(Controller);

				/* re-initialization of the message buffers */
				for (RAMCounter = 0; RAMCounter < MAXMB[Controller]; ++RAMCounter)  /* FlexCAN0 32 MB, FlexCAN1,2 = 16 MB */
				{
					RAMn[RAMCounter] = 0;    /* Clearing all message buffers */
				}

				/* Setting a counter for accessing the configuration array correctly */
				if (0 == Controller)
				{
					BufferCounter = 0;
				}
				else if (1 == Controller)
				{
					BufferCounter = TotalNumOfBuffers[0];
				}
				else
				{
					BufferCounter = TotalNumOfBuffers[0] + TotalNumOfBuffers[1];
				}

				for (BufferConfigCounter = 0 ; BufferConfigCounter < (TotalNumOfBuffers[Controller]) ; BufferConfigCounter+=4)
				{
					if (STANDARD == TempConfig->CanHardwareObject_ptr[(BufferCounter) ].CanIdType)
					{
						/* Writing to MB IDE bit (21) */
						RAMn[BufferConfigCounter] |= (1 << 21);

						if (TRANSMIT == TempConfig->CanHardwareObject_ptr[BufferCounter].CanObjectType)
						{
							/* Writing the MB code as TX INACTIVE */
							RAMn[BufferConfigCounter] |= (8 << 24);	/* Code = 0b1000 in bits (24 - 27) */
						}
						else
						{
							/* Writing the MB code as RX EMPTY */
							RAMn[BufferConfigCounter] |= (4 << 24);	/* Code = 0b0100 in bits (24 - 27) */

							/* Writing the corresponding MASK */
							RXIMRn[RegCounter] = TempConfig->CanHardwareObject_ptr[BufferCounter].CanHWFilterRef->CanHwFilterMask;
						}
					}

					else /* The ID is extended */
					{
						/* Writing to MB IDE bit (21) */
						RAMn[BufferConfigCounter] &= ~(1 << 21);

						if (TRANSMIT == TempConfig->CanHardwareObject_ptr[BufferCounter].CanObjectType)
						{
							/* Writing the MB code as TX INACTIVE */
							RAMn[BufferConfigCounter] |= (8 << 24);	/* Code = 0b1000 in bits (24 - 27) */
						}
						else
						{
							/* Writing the MB code as RX EMPTY */
							RAMn[BufferConfigCounter] |= (4 << 24);	/* Code = 0b0100 in bits (24 - 27) */

							/* Writing the corresponding MASK */
							RXIMRn[RegCounter] = TempConfig->CanHardwareObject_ptr[BufferCounter].CanHWFilterRef->CanHwFilterMask;
						}
					}
					BufferCounter++;
					RegCounter++;
				}
			}
			else
			{
				/* Disable the Controller */
				HW_Register((TempConfig -> (CanController_ptr[Controller].CanControllerBaseAddress + MCR)) |= (1 << MDIS);
			}


			/* Negate MCR(HALT) To Exit freeze mode */
			HW_Register((TempConfig -> (CanController_ptr[Controller].CanControllerBaseAddress + MCR)) &= ~(1 << HALT);


			/* wait for module to exit freeze mode */
			while(((HW_Register((TempConfig -> (CanController_ptr[Controller].CanControllerBaseAddress + MCR))) & (1 << FRZACK) )));

			/* wait for module to be ready */
			while(((HW_Register((TempConfig -> (CanController_ptr[Controller].CanControllerBaseAddress + MCR))) & (1 << NOTRDY) )));

			CAN_CONTROLLERS_STATES[Controller] = CAN_CS_STARTED;
			break;

		case CAN_CS_STOPPED:      /* Freeze mode */
			HW_Register((CAN_BASE_ADDRESSES[Controller] + MCR)) |= ( (1 << MDIS) | (1 << HALT) );
			while(! ((HW_Register((CAN_BASE_ADDRESSES[Controller] + MCR))) & (1 << FRZACK) )));

			/* Disabling Controller interrupts */
			Can_DisableControllerInterrupts(Controller);

			CAN_CONTROLLERS_STATES[Controller] = CAN_CS_STOPPED;
			break;

		case CAN_CS_SLEEP:     /* Freeze mode */
			HW_Register((CAN_BASE_ADDRESSES[Controller] + MCR)) |= ( (1 << MDIS) | (1 << HALT) );
			while(! ((HW_Register((CAN_BASE_ADDRESSES[Controller] + MCR))) & (1 << FRZACK) )));
			/* Disabling controller interrupts */
			Can_DisableControllerInterrupts(Controller);

			CAN_CONTROLLERS_STATES[Controller] = CAN_CS_SLEEP;

			break;
	}

	ENABLE_INTERRUPTS()
	return E_OK;
}


/* This function shall disable all interrupts of a CAN controller */
/* The default by the init function is that all the interrupts of the can controller are disabled if the Tx and Rx processing are set to be polling */
/* The default by the init function is that all the message buffer interrupts are disabled */
/* The user need to call Can_EnableControllerInterrupts to enable message buffers interrupts */
/* Types of interrupts in each Can Controller :
1) Message buffer interrupts (IMASK1 Register)
2) Warning interrupts (MCR_21, ESR1)
3) BusOff interrupt (CTRL1_15, ESR1_2)
4) Error interrupt (CTRL1_14, ESR1)
5) Tx warning interrupt (MCR_21, CTRL1_11, ESR1_17)
6) Rx warning interrupt (MCR_21, CTRL1_10, ESR1_16)
*/

void Can_DisableControllerInterrupts( uint8 Controller )
{
    #if (CAN_DEV_ERROR_DETECT==STD_ON)
		if (CAN_READY != CAN_MODULE_STATE)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x04U, CAN_E_UNINIT );
		}
		else if (Controller > NUM_OF_CONTROLLERS)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x04U, CAN_E_PARAM_CONTROLLER );
		}
		else
		{
			/* MISRA */
		}

    #endif    /* CAN_DEV_ERROR_DETECT */

	/* [SWS_Can_00049]: The function Can_DisableControllerInterrupts shall access the CAN controller registers to
	disable all interrupts for that CAN controller only, if interrupts for that CAN Controller are enabled */
	if ((NumOfIntDis[Controller]++) == 0)      /* This function has not been called before = interrupts enabled */
	{
		/* Disabling the controller interrupts */
		HW_Register((CAN_BASE_ADDRESSES[Controller] + IMASK1)) &= (0x0);   /* Clear all register bits */
		HW_Register((CAN_BASE_ADDRESSES[Controller]) + MCR) &= ~(1 << WRNEN);
		HW_Register((CAN_BASE_ADDRESSES[Controller]) + CTRL1) &= ~( (1 << BOFFMSK) | (1 << RWRNMSK) | (1 << TWRNMSK) ) ;
	}
	else
	{
		/* MISRA */
	}

	return;
}


void Can_EnableControllerInterrupts( uint8 Controller )
{
    #if (CAN_DEV_ERROR_DETECT==STD_ON)
		if (CAN_READY != CAN_MODULE_STATE)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x04U, CAN_E_UNINIT );
		}
		else if (Controller > NUM_OF_CONTROLLERS)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x04U, CAN_E_PARAM_CONTROLLER );
		}
		else
		{
			/* MISRA */
		}
    #endif    /* CAN_DEV_ERROR_DETECT */
	if ( 0 == (NumOfIntDis[Controller]) )
	{
		/* [SWS_Can_00208]: The function Can_EnableControllerInterrupts shall perform no action when
		Can_DisableControllerInterrupts has not been called before */
	}
	else if ( --(NumOfIntDis[Controller]) == 0)   /* The disable function has been called before */
	{
		/* Enabling the interrupts */
		HW_Register((CAN_BASE_ADDRESSES[Controller] + IMASK1)) |= 0xffffffff;   /* Set all register bits */
		/* Enable warning interrupts and the busoff interrupt */
		HW_Register((CAN_BASE_ADDRESSES[Controller])+MCR) |= (1 << WRNEN);
		HW_Register((CAN_BASE_ADDRESSES[Controller])+CTRL1) |= ( (1 << BOFFMSK) | (1 << RWRNMSK) | (1 << TWRNMSK) ) ;
	}
	else
	{
		/* MISRA */
	}
	return;
}

/* To get error state for each controller we must read the fault confinement bits in the ESR register */

Std_ReturnType Can_GetControllerErrorState( uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr )
{
    #if (CAN_DEV_ERROR_DETECT==STD_ON)
		if (CAN_READY != CAN_MODULE_STATE)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x11U, CAN_E_UNINIT );
			return E_NOT_OK;
		}
		else if (Controller > NUM_OF_CONTROLLERS)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x11U, CAN_E_PARAM_CONTROLLER );
			return E_NOT_OK;
		}
		else if (NULL_PTR == ErrorStatePtr)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x11U, CAN_E_PARAM_POINTER );
			return E_NOT_OK;
		}
		else
		{
			/* MISRA */
		}
    #endif    /* CAN_DEV_ERROR_DETECT */
	if ( !(HW_Register((CAN_BASE_ADDRESSES[Controller]) + ESR1) & (48)) )    /* (4-5) bits in ESR1 */
	{
		*ErrorStatePtr = CAN_ERRORSTATE_ACTIVE;
	}
	else if ( 1 == (HW_Register((CAN_BASE_ADDRESSES[Controller]) + ESR1) & (48)) )
	{
		*ErrorStatePtr = CAN_ERRORSTATE_PASSIVE;
	}
	else
	{
		*ErrorStatePtr = CAN_ERRORSTATE_BUSOFF;
	}
	return E_OK;
}

Std_ReturnType Can_GetControllerMode( uint8 Controller, Can_ControllerStateType* ControllerModePtr )
{
    #if (CAN_DEV_ERROR_DETECT==STD_ON)
		if (CAN_READY != CAN_MODULE_STATE)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x12U, CAN_E_UNINIT );
			return E_NOT_OK;
		}
		else if (Controller > NUM_OF_CONTROLLERS)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x12U, CAN_E_PARAM_CONTROLLER );
			return E_NOT_OK;
		}
		else if (NULL_PTR == ControllerModePtr)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x12U, CAN_E_PARAM_POINTER );
			return E_NOT_OK;
		}
		else
		{
			/* MISRA */
		}
    #endif    /* CAN_DEV_ERROR_DETECT */

	*ControllerModePtr = CAN_CONTROLLERS_STATES[Controller];
	return E_OK;
}

/* We are gonna check the availability of the message buffer by checking the MB code */
/* If the buffer code is DATA, that means the buffer is BUSY */
/* This function will access the Can_HTH_Array according to the argument Hth */
/* We will know the address of the message buffer based on its position in Can_HTH_Array and the fact that they are
   sorted as : Can0 MBs, Can1 MBs, Can2 MBs */
/* The design of this function is based on the previous comment, so the user configurations must be consistent with
   this design before attempting to use this function */

/* [SWS_Can_00212] ⌈ The function Can_Write shall perform following actions if the hardware transmit object is free:
1) The mutex for that HTH is set to ‘signaled’
2) The ID, Data Length and SDU are put in a format appropriate for the hardware (if necessary) and copied in the appropriate hardware registers/buffers.
3) All necessary control operations to initiate the transmit are done
4) The mutex for that HTH is released
5) The function returns with E_OK (SRS_Can_01049)
*/
Std_ReturnType Can_Write( Can_HwHandleType Hth, const Can_PduType* PduInfo )
{
    #if (CAN_DEV_ERROR_DETECT==STD_ON)
		if (CAN_READY != CAN_MODULE_STATE)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x06U, CAN_E_UNINIT );
			return E_NOT_OK;
		}
		else if (Hth < 0 || Hth > (NUM_OF_TX_BUFFERS_0 + NUM_OF_TX_BUFFERS_1 + NUM_OF_TX_BUFFERS_2 - 1) ) /* Hth is out of range */
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x06U, CAN_E_PARAM_HANDLE );
			return E_NOT_OK;
		}
		else if (PduInfo->length > 8)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x06U, CAN_E_PARAM_DATA_LENGTH );
			return E_NOT_OK;
		}
		else if (NULL_PTR == PduInfo)
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x06U, CAN_E_PARAM_POINTER );
			return E_NOT_OK;
		}
		else
		{
			/* MISRA */
		}
	#endif    /* CAN_DEV_ERROR_DETECT */

	IFLAG1 = HW_Register(Can_HTH_Array[Hth].CanControllerRef->CanControllerBaseAddress + IFLAGOFFSET);
	/* Calculating the address of the requested message buffer */
	uint32 BufferAddress;
	uint8 DistanceFromBeggining;
	uint8* TempDataPtr = NULL_PTR;
	uint8 length = PduInfo->length;
	if ( (0 == Hth) || (NUM_OF_TX_BUFFERS_0 == Hth) || ( (NUM_OF_TX_BUFFERS_0 + NUM_OF_TX_BUFFERS_1) == Hth ) )      /* The buffer is at the begining of the Can Controller */
	{
		BufferAddress = ( Can_HTH_Array[Hth].CanControllerRef->CanControllerBaseAddress + FIRSTOFFSET );
		DistanceFromBeggining = 0;
	}
	else
	{
		if (Hth > (NUM_OF_TX_BUFFERS_0 + NUM_OF_TX_BUFFERS_1) )
		{
			DistanceFromBeggining = (Hth - (NUM_OF_TX_BUFFERS_0 + NUM_OF_TX_BUFFERS_1) );
		}
		else if (Hth > NUM_OF_TX_BUFFERS_0)
		{
			DistanceFromBeggining = (Hth - (NUM_OF_TX_BUFFERS_0) );
		}
		else
		{
			DistanceFromBeggining = Hth;
		}
		BufferAddress = ( Can_HTH_Array[Hth].CanControllerRef->CanControllerBaseAddress + (FIRSTOFFSET + DistanceFromBeggining * 4) );
	}

	/* Checking if the message buffer is free (The code of the message buffer is INACTIVE)*/
	if (MB_CS_INACTIVE == ( HW_Register( BufferOffset ) & (0x0f000000) ) )
	{
		TempDataPtr = (uint32*)BufferAddress + 2;         /* Pointing to the first byte of the Data */
		/* Proceed in the transmission process */

		/* Clear the corresponding IFLAG bit */
		IFLAG1 |= (1 << DistanceFromBeggining);
		/* Write the payload data */
		while (length--)
		{
			*(TempDataPtr++) = *( (PduInfo->sdu)++ );
		}

		/* Write the ID */
		/* The Can_IdType is 32 bit which the two most significant bits discriminate between StdCan and Canfd */
		/* Since we are not using CanFD we will ignore the two most significant bits */
		HW_Register ( (uint32*)(BufferAddress) + 1 ) |= (uint32)(PduInfo->id) ;

		/* Activate MB for transmitting and writing the DLC */
		/* SRR = 1 (Tx frame) */
		/* EDL,BRS,ESI=0: CANFD not used */
		/* RTR = 0: data, not remote tx request frame */
		HW_Register(BufferAddress) |= ( ( (PduInfo->length) << 16 ) | (MB_CS_DATA << 24) | (1 << 22));
		HW_Register(BufferAddress) &= ~( 7 << 29 ) | (1 << 20));
	}
	else if ( MB_CS_DATA == ( HW_Register( BufferOffset ) & (0x0f000000) ) )
	{
		/* [SWS_Can_00213]: The function Can_Write shall perform no actions if the hardware transmit object is busy with another transmit request for an L-PDU:
		1. The transmission of the other L-PDU shall not be cancelled and the function Can_Write is left without any actions.
		2. The function Can_Write shall return CAN_BUSY */
		return CAN_BUSY;
	}
	else
	{
		/* MISRA */
	}
	return E_OK;
}
