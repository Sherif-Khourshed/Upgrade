/* On L-PDU transmission, the Can module writes the L-PDU in an appropriate buffer inside the CAN controller hardware */
/* On L-PDU reception, the Can module calls the RX indication callback function with ID, Data Length and pointer to L-SDU as parameter */
/* The Can module provides an interface that serves as periodical processing function, 
and which must be called by the Basic Software Scheduler module periodically. 
Furthermore, the Can module provides services to control the state of the CAN controllers. 
Bus-off and Wake-up events are notified by means of callback functions. 
The Can module is a Basic Software Module that accesses hardware resources */
/* [SWS_Can_00033] The Can module shall implement the interrupt service routines for all CAN Hardware Unit interrupts that are needed */
/* [SWS_Can_00419] The Can module shall disable all unused interrupts in the CAN controller */
/* [SWS_Can_00420] The Can module shall reset the interrupt flag at the end of the ISR (if not done automatically by hardware) */
/* The Can module has a very simple state machine, with the two states CAN_UNINIT and CAN_READY */
/* [SWS_Can_00103] After power-up/reset, the Can module shall be in the state CAN_UNINIT */
/* The ECU State Manager module shall call Can_Init at most once during runtime */
/* Can controller states are :  UNINIT, STOPPED, STARTED and SLEEP */
/* In case development errors are enabled and there is a not allowed transition requested by the upper layer, 
the Can module shall rise the development error CAN_E_TRANSITION */
/* The Can module does not check the actual state before it performs Can_Write or raises callbacks */
/* CAN controller state UNINIT :
The CAN controller is not initialized. All registers belonging to the CAN module are in reset state, 
CAN interrupts are disabled. The CAN Controller is not participating on the CAN bus */
/* CAN controller state STOPPED :
In this state the CAN Controller is initialized but does not participate on the bus. In addition, 
error frames and acknowledges must not be sent */
/* CAN controller state STARTED 
The controller is in a normal operation mode with complete functionality, which means it participates in the network. 
For many controllers leaving the initialization-mode causes the controller to be started */
/* [SWS_Can_00257] When the CAN hardware supports sleep mode and is triggered to transition into SLEEP state, 
the Can module shall set the controller to the SLEEP state from which the hardware can be woken over CAN Bus */
/* [SWS_Can_00258] When the CAN hardware does not support sleep mode and is triggered to transition into SLEEP state, 
the Can module shall emulate a logical SLEEP state from which it returns only, when it is triggered by software to 
transition into STOPPED state */
/* [SWS_Can_00404] The CAN hardware shall remain in state STOPPED, while the logical SLEEP state is active */

#include "Can.h"
#include "Det.h"
static Can_ModuleStateType CAN_MODULE_STATE = CAN_UNINIT;
static Can_ControllerStateType CAN_CONTROLLERS_STATES[] = {CAN_CS_UNINIT,CAN_CS_UNINIT,CAN_CS_UNINIT}; 

/* When the function Can_Init is entered and the Can module is not in state CAN_UNINIT or the CAN controllers 
are not in state UNINIT, it shall raise the error CAN_E_TRANSITION */
/* [SWS_Can_00056] Post-Build configuration elements that are marked as multiple (‘M’ or ‘x’) in chapter 10 can 
be selected by passing the pointer ‘Config’ to the init function of the module */
/* [SWS_Can_00250] The function Can_Init shall initialize: 
static variables, including flags, Common setting for the complete CAN HW unit, CAN controller specific 
settings for each CAN controller */
/* [SWS_Can_00407] If the hardware allows for only one usage of the register, 
the Can module implementing that functionality is responsible initializing the register. 
If the register can affect several hardware modules and if it is an I/O register it shall be initialized by the PORT driver. 
If the register can affect several hardware modules and if it is not an I/O register it shall be initialized by the MCU driver. 
One-time writable registers that require initialization directly after reset shall be initialized by the startup code */


void Can_Init( const Can_ConfigType* Config )
{
	uint8 Can_ControllerIndex = 0;
	/* [SWS_Can_00174] If development error detection for the Can module is enabled: 
	The function Can_Init shall raise the error CAN_E_TRANSITION if the driver is not in state CAN_UNINIT */
	/* [SWS_Can_00408] If development error detection for the Can module is enabled: 
	The function Can_Init shall raise the error CAN_E_TRANSITION if the CAN controllers are not in state UNINIT */
	#if (CAN_DEV_ERROR_DETECT == STD_ON)
		if ( (CAN_UNINIT != CAN_MODULE_STATE) || (CAN_CS_UNINIT != CAN_CONTROLLERS_STATES[0]) || (CAN_CS_UNINIT != CAN_CONTROLLERS_STATES[1]) || (CAN_CS_UNINIT != CAN_CONTROLLERS_STATES[2]) )
		{
			Det_ReportError(CAN_MODULE_ID, CAN_INDEX, 0x00U, CAN_E_TRANSITION);
		}
		else
		{
			/* MISRA */ 
		}
	#endif
	
	/* [SWS_Can_00259] The function Can_Init shall set all CAN controllers in the state STOPPED */
	/* It will call the function Can_SetControllerMode(CAN_CS_STOPPED) which is not implemented yet */
	for (Can_ControllerIndex = 0 ; Can_ControllerIndex < NUM_OF_CONTROLLERS ; Can_ControllerIndex++)
	{
		/* Set all the controllers in the freeze mode for initialization */
		Can_SetControllerMode(Can_ControllerIndex, CAN_CS_STOPPED); 
		/* The next line shall be implemented in the function Can_SetControllerMode */
		// CAN_CONTROLLERS_STATES[Can_ControllerIndex] = CAN_CS_STOPPED;
	}
	
	/* [SWS_Can_00245] The function Can_Init shall initialize all CAN controllers according to their configuration. 
	Each CAN controller must then be started separately by calling the function Can_SetControllerMode(CAN_CS_STARTED) */
	/* From The S32_RM :
	For any configuration change/initialization it is required that FlexCAN be put into Freeze mode */ 
	/* We will design the module that the STOPPED Mode of operation is the freeze mode, so the previous call 
	of Can_SetControllerMode(CAN_CS_STOPPED) will put all Can Controllers in the freeze mode of operation */
	
	/* Initializing the FlexCan0 registers according to the configuration pointer */
	/* 31) MDIS (Module Disable / Enable) = 0b To Enable
	30) FRZ (Freeze mode Enable / Disable) = 1b To Enable (Already enabled from the Call of Can_SetControllerMode)
	29) RFEN (Rx FIFO Enable) = 1b To Enable. This cannot be enabled while in CANFD mode. (Need Discussion)
	28) HALT (Halt FlexCan) = 1b To enter the freeze mode if FRZ is asserted (Already 1b)
	27) NOTRDY: Read-Only. 
	26) Reserved: Write the reset value.
	25) SoftReset: 1b reset the regs.
	24) FRZACK (Freeze acknowledgment)(read-only) : 1b: the controller is in the freeze mode
	23) SUPV (Supervisor Mode) : 1b (Supervisor mode)   (Need Discussion)
	22) Reserved.
	21) WRNEN (Warning Interrupt Enable): When asserted, this bit enables the generation of the TWRNINT and RWRNINT flags in the Error and Status Register 1 (ESR1).
	20) LPMACK (Low-Power mode acknowledgment)(Read-Only): 1b - FlexCAN is in a low-power mode.
	19) Reserved.
	18) Reserved.
	17) SRXDIS (Self Reception Disable): 0b - Self-reception enabled.   
	16) IRMQ (Individual Rx Masking And Queue Enable) : This bit indicates whether Rx matching process will be based either on individual masking and queue or on masking scheme with RXMGMASK, RX14MASK, RX15MASK, and RXFGMASK     (Need Discussion)
	15) DMA (DMA for Rx FIFO Enable): 1b (Enabled).      
	14) PNET_EN (Pretended Networking Enable) : 1b (Enabled).
	13) LPRIOEN (Local priority enable) : 0b (Disabled).
	12) AEN (Abort Enable) : 0b (Disabled).
	11) FDEN (CAN FD operation enable) : 0b (Disabled).
	10) Reserved
	9-8) IDAM: ID Acceptance Mode: This 2-bit field identifies the format of the Rx FIFO ID filter table elements   (Need Discussion)
		00b - Format A: One full ID (standard and extended) per ID filter table element. 
		01b - Format B: Two full standard IDs or two partial 14-bit (standard and extended) IDs per ID filter table element. 
		10b - Format C: Four partial 8-bit standard IDs per ID filter table element. 
		11b - Format D: All frames rejected.
	7) Reserved
	6-0) MAXMB (Number Of The Last Message Buffer) (Reset Value) = 0x0f	      (Need Discussion)
	
	/* CTRL1 Register */ 
	/* 31-24) PRESDIV (Prescaler Division Factor): This 8-bit field defines the ratio between the PE clock frequency 
	and the serial clock (Sclock) frequency. The Sclock period defines the time quantum of the CAN protocol. 
	For the reset value, the Sclock frequency is equal to the PE clock frequency. The maximum value of this 
	field is 0xFF, which gives a minimum Sclock frequency equal to the PE clock frequency divided by 256. 
	This field can be written only in Freeze mode because it is blocked by hardware in other modes. 
	Sclock frequency = PE clock frequency / (PRESDIV + 1).
	
	23-22) RJW (Resync Jump Width): This 2-bit field defines the maximum number of time quanta that a bit time can be 
	changed by one resynchronization. One time quantum is equal to the Sclock period.
	Resync Jump Width = RJW + 1.
	
	21-19) PSEG1: Phase Segment 1: This 3-bit field defines the length of phase segment 1 in the bit time. 
	The valid programmable values are 0–7. This field can be written only in Freeze mode because it is 
	blocked by hardware in other modes. 
	Phase Buffer Segment 1 = (PSEG1 + 1) × Time-Quanta
	
	18-16) PSEG2: Phase Segment 2: This 3-bit field defines the length of phase segment 2 in the bit time. 
	The valid programmable values are 1–7. This field can be written only in Freeze mode because it is blocked 
	by hardware in other modes. 
	Phase Buffer Segment 2 = (PSEG2 + 1) × Time-Quanta
	
	15) BOFFMSK: Bus-off interrupt mask: This bit provides a mask for the Bus Off interrupt ESR1[BOFFINT]. 
	0b - Bus Off interrupt disabled. 
	1b - Bus Off interrupt enabled.

	14) ERRMSK: Error Interrupt Mask: This bit provides a mask for the Error interrupt ESR1[ERRINT]. 
	0b - Error interrupt disabled. 
	1b - Error interrupt enabled.
	
	13) CLKSRC: CAN Engine Clock Source: This bit selects the clock source to the CAN Protocol Engine (PE) to be 
	either the peripheral clock or the oscillator clock. The selected clock is the one fed to the prescaler to 
	generate the serial clock (Sclock). In order to guarantee reliable operation, this bit can be written only 
	in Disable mode.
	
	12) LPB: Loop Back Mode (Disabled) (0b)
	
	11) TWRNMSK: Tx Warning Interrupt Mask: This bit provides a mask for the Tx Warning interrupt associated with 
	the TWRNINT flag in the Error and Status Register 1 (ESR1). This bit is read as zero when MCR[WRNEN] is negated. 
	This bit can be written only if MCR[WRNEN] is asserted. 
	0b - Tx Warning interrupt disabled. 
	1b - Tx Warning interrupt enabled.

	10) RWRNMSK: Rx Warning Interrupt Mask This bit provides a mask for the Rx Warning interrupt associated with 
	the RWRNINT flag in the Error and Status Register 1 (ESR1). This bit is read as zero when MCR[WRNEN] bit is negated. 
	This bit can be written only if MCR[WRNEN] bit is asserted. 
	0b - Rx Warning interrupt disabled. 
	1b - Rx Warning interrupt enabled.
	
	9-8) Reserved
	
	7) SMP: CAN Bit Sampling: This bit defines the sampling mode of CAN bits at the Rx input. It can be written in 
	Freeze mode only, because it is blocked by hardware in other modes
	0b - Just one sample is used to determine the bit value. 
	1b - Three samples are used to determine the value of the received bit: the regular one (sample point) and two preceding samples; a majority rule is used.
	
	6) BOFFREC: Bus Off Recovery: 
	0b - Automatic recovering from Bus Off state enabled. 
	1b - Automatic recovering from Bus Off state disabled.
	[SRS_Can_01060] The CAN driver shall not recover from bus-off automatically.
	
	5) TSYN: Timer Sync: This bit enables a mechanism that resets the free running timer each time a message is 
	received in message buffer 0.
	0b - Timer sync feature disabled 
	1b - Timer sync feature enabled

	4) LBUF: Lowest Buffer Transmitted First: 
	0b - Buffer with highest priority is transmitted first. 
	1b - Lowest number buffer is transmitted first.

	3) LOM: Listen Only mode: 0b (Disabled).
	
	2-0) PROPSEG: Propagation Segment This 3-bit field defines the length of the propagation segment in the bit time. 
	The valid programmable values are 0–7. This field can be written only in Freeze mode because it is blocked by 
	hardware in other modes. 
	Propagation segment time = (PROPSEG + 1) × time-quanta. Time-quantum = one Sclock period.

	CTRL1 Initialization 
	Prescaler = reset value (0x00) (The PE clock = Sclock). 
	RJW = From configuration pointer. 
	PSEG1, PSEG2 from configuration pointer.
	15, 16 (High) Enable Bus-off, Error Interrupts.
	13 0b (reset Value) (oscillator clock).
	12 (0b).
	11, 10 (High) (Enable Tx, Rx Warning Interrupts).
	9-8 (0b) (Reset value).
	7 (0b) One Sample determines the bit value.
	6 (1b) Automatic Bus-off Recovery disabled. 
	5 (0b) disabled.
	4 (0b) Buffer with highest priority is transmitted first.
	3 (0b). 
	2-0 = From configuration pointer.		
	*/

	/* Checking whether the controller is used in the configuration or not */
	if (TRUE == (Config -> (CanController_ptr[CAN_0_ID].CanControllerActivation)) ) 
	{
		/* MCR Initialization */ 
		HW_Register((Config -> (CanController_ptr[CAN_0_ID].CanControllerBaseAddress + MCR)) &= ~(1 << MDIS);  /* Enable the Can Controller */
		HW_Register((Config -> (CanController_ptr[CAN_0_ID].CanControllerBaseAddress + MCR)) |= ( (1 << RFEN) | (1 << AEN) );   /* Enable Rx FIFO */
		
		/* CTRL1 Initialization */
        HW_Register((Config->CanController_ptr[index1].CanControllerBaseAddress)+CTRL1) |= ((Config->CanController_ptr[index1].CanControllerDefaultBaudrate->CanControllerPropSeg)<<PROPSEG) | ((Config->CanController_ptr[index1].CanControllerDefaultBaudrate->CanControllerSeg1)<<PSEG1) | ((Config->CanController_ptr[index1].CanControllerDefaultBaudrate->CanControllerSeg2)<<PSEG2) | ((Config->CanController_ptr[index1].CanControllerDefaultBaudrate->CanControllerSyncJumpWidth)<<RJW) | (1 << ERRMSK) | (1 << BOFFREC) ;
		#if (CAN_BUSOFF_PROCESSING == INTERRUPT) 
			HW_Register((Config->CanController_ptr[index1].CanControllerBaseAddress)+CTRL1) |= (1 << BOFFMSK);
		#endif /*CAN_BUSOFF_PROCESSING*/
		#if (CAN_RX_PROCESSING == INTERRUPT) 
			HW_Register((Config->CanController_ptr[index1].CanControllerBaseAddress)+CTRL1) |= (1 << RWRNMSK);
		#endif /*CAN_RX_PROCESSING*/
		#if (CAN_TX_PROCESSING == INTERRUPT) 
			HW_Register((Config->CanController_ptr[index1].CanControllerBaseAddress)+CTRL1) |= (1 << TWRNMSK);
		#endif /*CAN_TX_PROCESSING*/
		
		

	}
	else 
	{
		/* Disable the Controller */ 
		HW_Register((Config -> (CanController_ptr[CAN_0_ID].CanControllerBaseAddress + MCR)) |= (1 << MDIS);
	}

	
	
	
	
	
	
	
	/* Initializing the FlexCan1 registers according to the configuration pointer */































	/* Initializing the FlexCan2 registers according to the configuration pointer */



























	
	/* [SWS_Can_00246] The function Can_Init shall change the module state to CAN_READY, 
	after initializing all controllers inside the HW Unit */
	CAN_MODULE_STATE = CAN_READY;
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

#if (CAN_SET_BAUDRATE_API == STD_ON)
/* Can_SetBaudrate() implementation here */
#endif