/*
 * File: CanIf_Types.h
 * Created by: Ahmed Nasr
 * Date: 28/2/2020
 */

#ifndef CANIF_TYPES_H
#define CANIF_TYPES_H

/* Id for our team in the AUTOSAR, for example ID = 88) */
#define CANIF_TYPES_VENDOR_ID          (88U)

#define CANIF_TYPES_SW_MAJOR_VERSION   (1U)
#define CANIF_TYPES_SW_MINOR_VERSION   (0U)
#define CANIF_TYPES_SW_PATCH_VERSION   (0U)

#define CANIF_TYPES_AR_MAJOR_VERSION   (4U)
#define CANIF_TYPES_AR_MINOR_VERSION   (3U)
#define CANIF_TYPES_AR_PATCH_VERSION   (1U)

/* Types of Communication Stack */
#include "ComStack_Types.h"

/* AUTOSAR checking between Can IF Types and Com Stack Types Modules */
#if ((CANIF_TYPES_AR_MAJOR_VERSION != COMSTACK_TYPES_AR_MAJOR_VERSION)\
 ||  (CANIF_TYPES_AR_MINOR_VERSION != COMSTACK_TYPES_AR_MINOR_VERSION)\
 ||  (CANIF_TYPES_AR_PATCH_VERSION != COMSTACK_TYPES_AR_PATCH_VERSION))
  #error "The AR version of ComStack_Types.h does not match the expected version"
#endif

/* Types of Communication Stack */
#include "Can_GeneralTypes.h"

/* AUTOSAR checking between Can IF Types and Com Stack Types Modules */
#if ((CANIF_TYPES_AR_MAJOR_VERSION != CAN_GENERALTYPES_AR_MAJOR_VERSION)\
 ||  (CANIF_TYPES_AR_MINOR_VERSION != CAN_GENERALTYPES_AR_MINOR_VERSION)\
 ||  (CANIF_TYPES_AR_PATCH_VERSION != CAN_GENERALTYPES_AR_PATCH_VERSION))
  #error "The AR version of Can_GeneralTypes.h does not match the expected version"
#endif

/* --------------------------------------------------------------- */

/* 
 * The PduMode of a channel defines its transmit or receive activity.
 * Communication direction (transmission and/or reception) of the channel can
 * be controlled separately or together by upper layers.
 */
typedef enum
{
	/* 
	 * Transmit and receive path of the corresponding channel are
	 * disabled => no communication mode
	 */
	CANIF_OFFLINE = 0x00,
	
	/*
	 * Transmit path of the corresponding channel is disabled.
	 * The receive path is enabled.
	 */
	CANIF_TX_OFFLINE = 0x01,
	
	/*
	 * Transmit path of the corresponding channel is in offline active mode
	 * (see SWS_CANIF_00072).
	 * The receive path is disabled.
	 * This mode requires CanIfTxOfflineActiveSupport = TRUE.
	 */
	CANIF_TX_OFFLINE_ACTIVE = 0x02,
	
	/* 
	 * Transmit and receive path of the corresponding channel are
	 * enabled => full operation mode
	 */
	CANIF_ONLINE = 0x03
} CanIf_PduModeType;

/* Return value of CAN L-PDU notification status. */
typedef enum
{
	/* No transmit or receive event occurred for the requested L-PDU. */
	CANIF_NO_NOTIFICATION = 0x00,
	
	/* The requested Rx/Tx CAN L-PDU was successfully transmitted or received. */
	CANIF_TX_RX_NOTIFICATION
} CanIf_NotifStatusType;

/*
 * Selects the desired software filter mechanism for reception only.
 * Each implemented software filtering method is identified by this enumeration number.
 *
 * dependency: BasicCAN reception must be enabled by referenced
 * parameter CAN_HANDLE_TYPE of the CAN Driver module via
 * CANIF_HRH_HANDLETYPE_REF for at least one HRH.
 */
typedef enum
{
	BINARY,		/* Selects Binary Filter method. */
	INDEX,		/* Selects Index Filter method. */
	LINEAR,		/* Selects Linear Filter method. */
	TABLE		/* Selects Table Filter method. */
} CanIfPrivateSoftwareFilterType;

typedef struct
{
	CanIfPrivateSoftwareFilterType CanIfPrivateSoftwareFilter;
} CanIfPrivateCfg;

/*--------------------------------------------------------------------------------*/

/*
 * This parameter is used to configure the Can_HwHandleType.
 * TheCan_HwHandleType represents the hardware object handles of a CAN
 * hardware unit. For CAN hardware units with more than 255 HW objects
 * the extended range shall be used (UINT16).
 *
 * dependency: Can_HwHandleType.
 */
typedef enum
{
	UINT16,
	UINT8
} CanIfPublicHandleTypeEnum;

/*
 * This container contains the public configuration (parameters) of the
 * CAN Interface.
 */
typedef struct
{
	/*
	 * Defines header files for callback functions which shall be included in
	 * case of CDDs.
	 */
	uint8 CanIfPublicCddHeaderFile[32];
} CanIfPublicCfg;

/*--------------------------------------------------------------------------------*/

/*
 * Type of CAN Identifier of the transmit CAN L-PDU used by the CAN
 * Driver module for CAN L-PDU transmission.
 */
typedef enum
{
	EXTENDED_CAN,		/* CAN frame with extended identifier (29 bits) */
	EXTENDED_FD_CAN,	/* CAN FD frame with extended identifier (29 bits) */
	STANDARD_CAN,		/* CAN frame with standard identifier (11 bits) */
	STANDARD_FD_CAN		/* CAN FD frame with standard identifier (11 bits)*/
} CanIfTxPduCanIdTypeType;

/* Defines the type of each transmit CAN L-PDU. */
typedef enum
{
	DYNAMIC,			/* CAN ID is defined at runtime. */
	STATIC				/* CAN ID is defined at compile-time. */
} CanIfTxPduTypeType;

typedef enum
{
	CAN_NM,				/* CAN NM */
	CAN_TP,				/* CAN TP */
	CAN_TSYN,			/* Global Time Synchronization over CAN */
	CDD,				/* Complex Driver */
	J1939NM,			/* J1939Nm */
	J1939TP,			/* J1939Tp */
	PDUR,				/* PDU Router */
	XCP					/* Extended Calibration Protocol */
} CanIfTxPduUserTxConfirmationULType;

/*
 * This container contains the Txbuffer configuration. Multiple buffers with
 * different sizes could be configured. If CanIfBufferSize
 * (ECUC_CanIf_00834) equals 0, the CanIf Tx L-PDU only refers via this
 * CanIfBufferCfg the corresponding CanIfHthCfg.
 */
typedef struct
{
	/*
	 * This parameter defines the number of CanIf Tx L-PDUs which can be
	 * buffered in one Txbuffer. If this value equals 0, the CanIf does not
	 * perform Txbuffering for the CanIf Tx L-PDUs which are assigned to this
	 * Txbuffer. If CanIfPublicTxBuffering equals False, this parameter equals
	 * 0 for all TxBuffer. If the CanHandleType of the referred HTH equals
	 * FULL, this parameter equals 0 for this TxBuffer.
	 *
	 * dependency: CanIfPublicTxBuffering, CanHandleType.
	 */
#if (CANIF_PUBLIC_TX_BUFFERING == STD_ON)
	uint8 CanIfBufferSize;
#endif

	/*
	 * Reference to HTH, that defines the hardware object or the pool of
	 * hardware objects configured for transmission. All the CanIf Tx L-PDUs
	 * refer via the CanIfBufferCfg and this parameter to the HTHs if
	 * TxBuffering is enabled, or not.
	 *
	 * Each HTH shall not be assigned to more than one buffer.
	 */
	CanIfHthCfg* CanIfBufferHthRef;
} CanIfBufferCfg;

/*
 * This container contains the configuration (parameters) of a transmit
 * CAN L-PDU. It has to be configured as often as a transmit CAN L-PDU
 * is needed.
 *
 * The SHORT-NAME of "CanIfTxPduConfig" container represents the
 * symbolic name of Transmit L-PDU.
 *
 * This L-SDU consumes a meta data item of type CAN_ID_32.
 */
typedef struct
{
	/*
	 * CAN Identifier of transmit CAN L-PDUs used by the CAN Driver for
	 * CAN L-PDU transmission. Range: 11 Bit For Standard CAN Identifier
	 * ... 29 Bit For Extended CAN identifier
	 *
	 * The CAN Identifier may be omitted for dynamic transmit L-PDUs.
	 */
	uint32 CanIfTxPduCanId;

	/*
	 * Identifier mask which denotes relevant bits in the CAN Identifier. This
	 * parameter may be used to keep parts of the CAN Identifier of dynamic
	 * transmit L-PDUs static. Range: 11 bits for Standard CAN Identifier, 29
	 * bits for Extended CAN Identifier.
	 */
	uint32 CanIfTxPduCanIdMask;

	/*
	 * Type of CAN Identifier of the transmit CAN L-PDU used by the CAN
	 * Driver module for CAN L-PDU transmission.
	 */
	CanIfTxPduCanIdTypeType CanIfTxPduCanIdType;

	/*
	 * ECU wide unique, symbolic handle for transmit CAN L-SDU.
	 *
	 * Range: 0..max. number of CantTxPduIds
	 */
	PduIdType CanIfTxPduId;

	/*
	 * If CanIfPublicPnFilterSupport is enabled, by this parameter PDUs
	 * could be configured which will pass the CanIfPnFilter.
	 *
	 * If there is no CanIfTxPduPnFilterPdu configured per controller,
	 * the corresponding controller applies no CanIfPnFilter.
	 *
	 * dependency: This parameter shall only be configurable if
	 * CanIfPublicPnSupport equals True.
	 */
#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
	boolean CanIfTxPduPnFilterPdu;
#endif

	/*
	 * Enables and disables transmit confirmation for each transmit CAN
	 * L-SDU for reading its notification status.
	 *
	 * dependency: CANIF_READTXPDU_NOTIFY_STATUS_API must be enabled.
	 */
#if (CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API == STD_ON)
	boolean CanIfTxPduReadNotifyStatus;
#endif

	/* Defines the type of each transmit CAN L-PDU. */
	CanIfTxPduTypeType CanIfTxPduType;

	/*
	 * This parameter defines the upper layer (UL) module to which the
	 * confirmation of the successfully transmitted CANTXPDUID has to be
	 * routed via the <User_TxConfirmation>. This <User_TxConfirmation>
	 * has to be invoked when the confirmation of the configured
	 * CANTXPDUID will be received by a Tx confirmation event from the
	 * CAN Driver module. If no upper layer (UL) module is configured, no
	 * <User_TxConfirmation> has to be called in case of a Tx confirmation
	 * event of the CANTXPDUID from the CAN Driver module.
	 */
	CanIfTxPduUserTxConfirmationULType CanIfTxPduUserTxConfirmationUL;

	/*
	 * This parameter defines the name of the <User_TriggerTransmit>. This
	 * parameter depends on the parameter
	 * CanIfTxPduUserTxConfirmationUL. If
	 * CanIfTxPduUserTxConfirmationUL equals CAN_TP, CAN_NM, PDUR,
	 * XCP, CAN_TSYN, J1939NM or J1939TP, the name of the
	 * <User_TriggerTransmit> is fixed. If CanIfTxPduUserTxConfirmationUL
	 * equals CDD, the name of the <User_TxConfirmation> is selectable.
	 * Please be aware that this parameter depends on the same parameter
	 * as CanIfTxPduUserTxConfirmationName. It shall be clear which upper
	 * layer is responsible for that PDU.
	 *
	 * dependency: CanIfTxPduUserTriggerTransmitName requires
	 * CanIfTxPduUserTxConfirmationUL to be either PDUR or CDD.
	 */
	void (*CanIfTxPduUserTriggerTransmit)(PduIdType);

	/*
	 * This parameter defines the name of the <User_TxConfirmation>. This
	 * parameter depends on the parameter
	 * CanIfTxPduUserTxConfirmationUL. If
	 * CanIfTxPduUserTxConfirmationUL equals CAN_TP, CAN_NM, PDUR,
	 * XCP, CAN_TSYN, J1939NM or J1939TP, the name of the
	 * <User_TxConfirmation> is fixed. If CanIfTxPduUserTxConfirmationUL
	 * equals CDD, the name of the <User_TxConfirmation> is selectable.
	 */
	void (*CanIfTxPduUserTxConfirmation)(PduIdType);

	/* Configurable reference to a CanIf buffer configuration. */
	CanIfBufferCfg* CanIfTxPduBufferRef;
} CanIfTxPduCfg;

/*
 * CAN Identifier of receive CAN L-PDUs used by the CAN Driver for
 * CAN L-PDU reception.
 */
typedef enum
{
	EXTENDED_CAN,				/* CAN 2.0 or CAN FD frame with extended identifier (29 bits)*/
	EXTENDED_FD_CAN,			/* CAN FD frame with extended identifier (29 bits) */
	EXTENDED_NO_FD_CAN,			/* CAN 2.0 frame with extended identifier (29 bits) */
	STANDARD_CAN,				/* CAN 2.0 or CAN FD frame with standard identifier (11 bits) */
	STANDARD_FD_CAN,			/* CAN FD frame with standard identifier (11 bits) */
	STANDARD_NO_FD_CAN			/* CAN 2.0 frame with standard identifier (11 bits) */
} CanIfRxPduCanIdTypeType;

/*
 * This parameter defines the upper layer (UL) module to which the
 * indication of the successfully received CANRXPDUID has to be routed
 * via <User_RxIndication>. This <User_RxIndication> has to be invoked
 * when the indication of the configured CANRXPDUID will be received
 * by an Rx indication event from the CAN Driver module. If no upper
 * layer (UL) module is configured, no <User_RxIndication> has to be
 * called in case of an Rx indication event of the CANRXPDUID from the
 * CAN Driver module.
 */
typedef enum
{
	CAN_NM,						/* CAN NM */
	CAN_TP,						/* CAN TP */
	CAN_TSYN,					/* Global Time Synchronization over CAN */
	CDD,						/* Complex Driver */
	J1939NM,					/* J1939Nm */
	J1939TP,					/* J1939Tp */
	PDUR,						/* PDU Router */
	XCP							/* Extended Calibration Protocol */
} CanIfRxPduUserRxIndicationULType;

/* Optional container that allows to map a range of CAN Ids to one PduId. */
typedef struct
{
	/*
	 * Lower CAN Identifier of a receive CAN L-PDU for identifier range
	 * definition, in which all CAN Ids are mapped to one PduId.
	 */
	uint32 CanIfRxPduCanIdRangeLowerCanId;

	/*
	 * Upper CAN Identifier of a receive CAN L-PDU for identifier range
	 * definition, in which all CAN Ids are mapped to one PduId.
	 */
	uint32 CanIfRxPduCanIdRangeUpperCanId;
} CanIfRxPduCanIdRange;

/*
 * This container contains the configuration (parameters) of each receive
 * CAN L-PDU.
 *
 * The SHORT-NAME of "CanIfRxPduConfig" container itself represents
 * the symbolic name of Receive L-PDU.
 *
 * This L-SDU produces a meta data item of type CAN_ID_32.
 */
typedef struct
{
	/*
	 * CAN Identifier of Receive CAN L-PDUs used by the CAN Interface.
	 * Exa: Software Filtering. This parameter is used if exactly one Can
	 * Identifier is assigned to the Pdu. If a range is assigned then the
	 * CanIfRxPduCanIdRange parameter shall be used.
	 * Range: 11 Bit For Standard CAN Identifier ... 29 Bit For Extended CAN identifier
	 */
	uint32 CanIfRxPduCanId;

	/*
	 * Identifier mask which denotes relevant bits in the CAN Identifier. This
	 * parameter defines a CAN Identifier range in an alternative way to
	 * CanIfRxPduCanIdRange. It identifies the bits of the configured CAN
	 * Identifier that must match the received CAN Identifier. Range: 11 bits
	 * for Standard CAN Identifier, 29 bits for Extended CAN Identifier.
	 */
	uint32 CanIfRxPduCanIdMask;

	/*
	 * CAN Identifier of receive CAN L-PDUs used by the CAN Driver for
	 * CAN L-PDU reception.
	 */
	CanIfRxPduCanIdTypeType CanIfRxPduCanIdType;

	/*
	 * Data length of the received CAN L-PDUs used by the CAN Interface.
	 * This information is used for Data Length Check. Additionally it might
	 * specify the valid bits in case of the discrete DLC for CAN FD L-PDUs > 8 bytes.
	 *
	 * The data area size of a CAN L-PDU can have a range from 0 to 64 bytes.
	 *
	 * dependency: If CanIfRxPduDataLength > 8 then
	 * CanIfRxPduCanIdType must not be STANDARD_NO_FD_CAN or EXTENDED_NO_FD_CAN.
	 */
	uint8 CanIfRxPduDataLength;

	/*
	 * ECU wide unique, symbolic handle for receive CAN L-SDU. It shall
	 * fulfill ANSI/AUTOSAR definitions for constant defines.
	 *
	 * Range: 0..max. number of defined CanRxPduIds
	 */
	PduIdType CanIfRxPduId;

	/*
	 * Enables and disables the Rx buffering for reading of received L-SDU data.
	 *
	 * dependency: CANIF_CANPDUID_READDATA_API must be enabled.
	 */
#if (CANIF_PUBLIC_READ_RX_PDU_DATA_API == STD_ON)
	boolean CanIfRxPduReadData;
#endif

	/* Enables and disables receive indication for each receive CAN L-SDU
	 * for reading its notification status.
	 *
	 * dependency: CANIF_READRXPDU_NOTIFY_STATUS_API must be enabled.
	 */
#if (CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API == STD_ON)
	boolean CanIfRxPduReadNotifyStatus;
#endif

	/*
	 * This parameter defines the name of the <User_RxIndication>. This
	 * parameter depends on the parameter
	 * CANIF_RXPDU_USERRXINDICATION_UL. If
	 * CANIF_RXPDU_USERRXINDICATION_UL equals CAN_TP,
	 * CAN_NM, PDUR, XCP, CAN_TSYN, J1939NM or J1939TP, the name
	 * of the <User_RxIndication> is fixed. If
	 * CANIF_RXPDU_USERRXINDICATION_UL equals CDD, the name of
	 * the <User_RxIndication> is selectable.
	 */
	void (*CanIfRxPduUserRxIndication)(PduIdType);

	/*
	 * This parameter defines the upper layer (UL) module to which the
	 * indication of the successfully received CANRXPDUID has to be routed
	 * via <User_RxIndication>. This <User_RxIndication> has to be invoked
	 * when the indication of the configured CANRXPDUID will be received
	 * by an Rx indication event from the CAN Driver module. If no upper
	 * layer (UL) module is configured, no <User_RxIndication> has to be
	 * called in case of an Rx indication event of the CANRXPDUID from the
	 * CAN Driver module.
	 */
	CanIfRxPduUserRxIndicationULType CanIfRxPduUserRxIndicationUL;

	/*
	 * The HRH to which Rx L-PDU belongs to, is referred through this parameter.
	 *
	 * dependency: This information has to be derived from the CAN Driver configuration.
	 */
	CanIfHrhCfg* CanIfRxPduHrhIdRef;

	/* Optional container that allows to map a range of CAN Ids to one PduId. */
	CanIfRxPduCanIdRange* CanIfRxPduCanIdRangePtr;
} CanIfRxPduCfg;

/*
 * Specifies whether a configured Range of CAN Ids shall only consider
 * standard CAN Ids or extended CAN Ids.
 */
typedef enum
{
	EXTENDED,				/* All the CANIDs are of type extended only (29 bit). */
	STANDARD				/* All the CANIDs are of type standard only (11 bit). */
} CanIfHrhRangeRxPduRangeCanIdTypeType;

/*
 * Defines the parameters required for configuring multiple CANID
 * ranges for a given same HRH.
 */
typedef struct
{
	/*
	 * CAN Identifier used as base value in combination with
	 * CanIfHrhRangeMask for a masked ID range in which all CAN Ids shall
	 * pass the software filtering. The size of this parameter is limited by
	 * CanIfHrhRangeRxPduRangeCanIdType.
	 */
	uint32 CanIfHrhRangeBaseId;

	/*
	 * Used as mask value in combination with CanIfHrhRangeBaseId for a
	 * masked ID range in which all CAN Ids shall pass the software filtering.
	 * The size of this parameter is limited by
	 * CanIfHrhRangeRxPduRangeCanIdType.
	 */
	uint32 CanIfHrhRangeMask;

	/*
	 * Lower CAN Identifier of a receive CAN L-PDU for identifier range
	 * definition, in which all CAN Ids shall pass the software filtering.
	 */
	uint32 CanIfHrhRangeRxPduLowerCanId;

	/*
	 * Upper CAN Identifier of a receive CAN L-PDU for identifier range
	 * definition, in which all CAN Ids shall pass the software filtering.
	 */
	uint32 CanIfHrhRangeRxPduUpperCanId;

	/*
	 * Specifies whether a configured Range of CAN Ids shall only consider
	 * standard CAN Ids or extended CAN Ids.
	 */
	CanIfHrhRangeRxPduRangeCanIdTypeType CanIfHrhRangeRxPduRangeCanIdType;
} CanIfHrhRangeCfg;

/*
 * This container contains configuration parameters for each hardware
 * receive object (HRH).
 */
typedef struct
{
	/*
	 * Selects the hardware receive objects by using the HRH range/list from
	 * CAN Driver configuration to define, for which HRH a software filtering
	 * has to be performed at during receive processing.
	 */
	boolean CanIfHrhSoftwareFilter;

	/*
	 * Reference to controller Id to which the HRH belongs to. A controller
	 * can contain one or more HRHs.
	 */
	CanIfCtrlCfg* CanIfHrhCanCtrlIdRef;

	/*
	 * The parameter refers to a particular HRH object in the CanDrv
	 * configuration (see CanHardwareObject ECUC_Can_00324).
	 * CanIf receives the following information of the CanDrv module by
	 * this reference:
	 * CanHandleType (see ECUC_Can_00323)
	 * CanObjectId (see ECUC_Can_00326)
	 */
	/*CanHardwareObject* CanIfHrhIdSymRef;*/

	/*
	 * Defines the parameters required for configuring
	 * multiple CANID ranges for a given same HRH.
	 */
	CanIfHrhRangeCfg* HrhRangeCfg;
} CanIfHrhCfg;

/* This container contains parameters related to each HTH. */
typedef struct
{
	/*
	 * Reference to controller Id to which the HTH belongs to. A controller
	 * can contain one or more HTHs.
	 */
	CanIfCtrlCfg* CanIfHthCanCtrlIdRef;

	/*
	 * The parameter refers to a particular HTH object in the CanDrv
	 * configuration (see CanHardwareObject ECUC_Can_00324).
	 * CanIf receives the following information of the CanDrv module by
	 * this reference:
	 * CanHandleType (see ECUC_Can_00323)
	 * CanObjectId (see ECUC_Can_00326)
	 */
	/*CanHardwareObject* CanIfHthIdSymRef;*/
} CanIfHthCfg;

/*
 * This container contains the references to the configuration setup of
 * each underlying CAN Driver.
 */
typedef struct
{
	/*
	 * This container contains configuration parameters for
	 * each hardware receive object (HRH).
	 */
	CanIfHrhCfg* HrhCfg;

	/* This container contains parameters related to each HTH. */
	CanIfHthCfg* HthCfg;
} CanIfInitHohCfg;

typedef struct
{
	/*
	 * Selects the CAN Interface specific configuration setup. This type of the
	 * external data structure shall contain the post build initialization data for
	 * the CAN Interface for all underlying CAN Drivers.
	 *
	 * constant to CanIf_ConfigType
	 */
    uint8 CanIfInitCfgSet[32];

    /*
     * Maximum total size of all Tx buffers. This parameter is needed only in
     * case of post-build loadable implementation using static memory allocation.
     */
    uint64 CanIfMaxBufferSize;

    /*
     * Maximum number of Pdus. This parameter is needed only in case of
     * post-build loadable implementation using static memory allocation.
     */
    uint64 CanIfMaxRxPduCfg;

    /*
     * Maximum number of Pdus. This parameter is needed only in case of
     * post-build loadable implementation using static memory allocation.
     */
    uint64 CanIfMaxTxPduCfg;

    /*
     * This container contains the configuration (parameters) of a transmit
     * CAN L-PDU. It has to be configured as often as a transmit CAN L-PDU
     * is needed.
     *
     * The SHORT-NAME of "CanIfTxPduConfig" container represents the
     * symbolic name of Transmit L-PDU.
     *
     * This L-SDU consumes a meta data item of type CAN_ID_32.
     *
     * It may be an array.
     */
    CanIfTxPduCfg* CanIfTxPduCfgPtr[];

    /*
     * This container contains the configuration (parameters) of each receive
     * CAN L-PDU.
     * The SHORT-NAME of "CanIfRxPduConfig" container itself represents
     * the symbolic name of Receive L-PDU.
     * This L-SDU produces a meta data item of type CAN_ID_32.
     *
     * It may be an array.
     */
    CanIfRxPduCfg* CanIfRxPduCfgPtr[];

    /*
     * This container contains the references to the configuration setup of
     * each underlying CAN Driver.
     */
    CanIfInitHohCfg* CanIfHohCfgPtr;
} CanIfInitCfg;

/*--------------------------------------------------------------------------------*/

#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
/*
 * This parameter defines the upper layer module to which the
 * CheckTrcvWakeFlagIndication from the Driver modules have to be
 * routed. If CANIF_PUBLIC_PN_SUPPORT equals False, this
 * parameter shall not be configurable.
 *
 * dependency: CANIF_PUBLIC_PN_SUPPORT.
 */
typedef enum
{
	CAN_SM,						/* CAN State Manager */
	CDD							/* Complex Driver */
} CanIfDispatchUserCheckTrcvWakeFlagIndicationULType;

/*
 * This parameter defines the upper layer module to which the
 * ClearTrcvWufFlagIndication from the Driver modules have to be routed.
 * If CANIF_PUBLIC_PN_SUPPORT equals False, this parameter shall
 * not be configurable.
 *
 * dependency: CANIF_PUBLIC_PN_SUPPORT.
 */
typedef enum
{
	CAN_SM,						/* CAN State Manager */
	CDD							/* Complex Driver */
} CanIfDispatchUserClearTrcvWufFlagIndicationULType;

/*
 * This parameter defines the upper layer module to which the
 * ConfirmPnAvailability notification from the Driver modules have to be
 * routed. If CANIF_PUBLIC_PN_SUPPORT equals False, this
 * parameter shall not be configurable.
 *
 * dependency: CANIF_PUBLIC_PN_SUPPORT.
 */
typedef enum
{
	CAN_SM,						/* CAN State Manager */
	CDD							/* Complex Driver */
} CanIfDispatchUserConfirmPnAvailabilityULType;
#endif

/*
 * This parameter defines the upper layer (UL) module to which the
 * notifications of all ControllerBusOff events from the CAN Driver
 * modules have to be routed via <User_ControllerBusOff>. There is no
 * possibility to configure no upper layer (UL) module as the provider of
 * <User_ControllerBusOff>.
 */
typedef enum
{
	CAN_SM,						/* CAN State Manager */
	CDD							/* Complex Driver */
} CanIfDispatchUserCtrlBusOffULType;

/*
 * This parameter defines the upper layer (UL) module to which the
 * notifications of all ControllerTransition events from the CAN Driver
 * modules have to be routed via <User_ControllerModeIndication>.
 */
typedef enum
{
	CAN_SM,						/* CAN State Manager */
	CDD							/* Complex Driver */
} CanIfDispatchUserCtrlModeIndicationULType;

/*
 * This parameter defines the upper layer (UL) module to which the
 * notifications of all TransceiverTransition events from the CAN
 * Transceiver Driver modules have to be routed via
 * <User_TrcvModeIndication>. If no UL module is configured, no upper
 * layer callback function will be called.
 */
typedef enum
{
	CAN_SM,						/* CAN State Manager */
	CDD							/* Complex Driver */
} CanIfDispatchUserTrcvModeIndicationULType;

/*
 * This parameter defines the upper layer (UL) module to which the
 * notifications about positive former requested wake up sources have to
 * be routed via <User_ValidateWakeupEvent>. If parameter
 * CANIF_WAKEUP_CHECK_VALIDATION_API is disabled, this
 * parameter cannot be configured.
 *
 * dependency: CANIF_WAKEUP_CHECK_VALIDATION_API.
 */
#if (CANIF_WAKEUP_SUPPORT == STD_ON)
typedef enum
{
	CDD,						/* Complex Driver */
	ECUM						/* ECU State Manager */
} CanIfDispatchUserValidateWakeupEventULType;
#endif

/*
 * Callback functions provided by upper layer modules of the CanIf. The
 * callback functions defined in this container are common to all
 * configured CAN Driver / CAN Transceiver Driver modules.
 */
typedef struct
{
#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
	/*
	 * This parameter defines the name of
	 * This <User_CheckTrcvWakeFlagIndication>. If
	 * This CanIfDispatchUserCheckTrcvWakeFlagIndicationUL equals CAN_SM
	 * This the name of <User_CheckTrcvWakeFlagIndication> is fixed. If it equals
	 * This CDD, the name is selectable. If CanIfPublicPnSupport equals False,
	 * This this parameter shall not be configurable.
	 *
	 * dependency:
	 * CANIF_DISPATCH_USERCHECKTRCVWAKEFLAGINDICATION_UL,
	 * CANIF_PUBLIC_PN_SUPPORT
	 */
	void (*CanIfDispatchUserCheckTrcvWakeFlagIndication)(void);

	/*
	 * This parameter defines the upper layer module to which the
	 * CheckTrcvWakeFlagIndication from the Driver modules have to be
	 * routed. If CANIF_PUBLIC_PN_SUPPORT equals False, this
	 * parameter shall not be configurable.
	 */
	CanIfDispatchUserCheckTrcvWakeFlagIndicationULType CanIfDispatchUserCheckTrcvWakeFlagIndicationUL;

	/*
	 * This parameter defines the name of
	 * <User_ClearTrcvWufFlagIndication>. If
	 * CanIfDispatchUserClearTrcvWufFlagIndicationUL equals CAN_SM the
	 * name of <User_ClearTrcvWufFlagIndication> is fixed. If it equals CDD,
	 * the name is selectable. If CANIF_PUBLIC_PN_SUPPORT equals
	 * False, this parameter shall not be configurable.
	 *
	 * dependency: CanIfDispatchUserClearTrcvWufFlagIndicationUL,
	 * CANIF_PUBLIC_PN_SUPPORT
	 */
	void (*CanIfDispatchUserClearTrcvWufFlagIndication)(void);

	/*
	 * This parameter defines the upper layer module to which the
	 * ClearTrcvWufFlagIndication from the Driver modules have to be routed.
	 * If CANIF_PUBLIC_PN_SUPPORT equals False, this parameter shall
	 * not be configurable.
	 *
	 * dependency: CANIF_PUBLIC_PN_SUPPORT.
	 */
	CanIfDispatchUserClearTrcvWufFlagIndicationULType CanIfDispatchUserClearTrcvWufFlagIndicationUL;

	/*
	 * This parameter defines the name of <User_ConfirmPnAvailability>. If
	 * CanIfDispatchUserConfirmPnAvailabilityUL equals CAN_SM the name
	 * of <User_ConfirmPnAvailability> is fixed. If it equals CDD, the name is
	 * selectable. If CANIF_PUBLIC_PN_SUPPORT equals False, this
	 * parameter shall not be configurable.
	 *
	 * dependency: CanIfDispatchUserConfirmPnAvailabilityUL,
	 * CANIF_PUBLIC_PN_SUPPORT
	 */
	void (*CanIfDispatchUserConfirmPnAvailability)(void);

	/*
	 * This parameter defines the upper layer module to which the
	 * ConfirmPnAvailability notification from the Driver modules have to be
	 * routed. If CANIF_PUBLIC_PN_SUPPORT equals False, this
	 * parameter shall not be configurable.
	 *
	 * dependency: CANIF_PUBLIC_PN_SUPPORT.
	 */
	CanIfDispatchUserConfirmPnAvailabilityULType CanIfDispatchUserConfirmPnAvailabilityUL;
#endif

	/*
	 * This parameter defines the name of <User_ControllerBusOff>. This
	 * parameter depends on the parameter
	 * CANIF_USERCTRLBUSOFF_UL. If CANIF_USERCTRLBUSOFF_UL
	 * equals CAN_SM the name of <User_ControllerBusOff> is fixed. If
	 * CANIF_USERCTRLBUSOFF_UL equals CDD, the name of
	 * <User_ControllerBusOff> is selectable.
	 *
	 * dependency: CanIfDispatchUserCtrlBusOffUL.
	 */
	void (*CanIfDispatchUserCtrlBusOff)(uint8);

	/*
	 * This parameter defines the upper layer (UL) module to which the
	 * notifications of all ControllerBusOff events from the CAN Driver
	 * modules have to be routed via <User_ControllerBusOff>. There is no
	 * possibility to configure no upper layer (UL) module as the provider of
	 * <User_ControllerBusOff>.
	 */
	CanIfDispatchUserCtrlBusOffULType CanIfDispatchUserCtrlBusOffUL;

	/*
	 * This parameter defines the name of <User_ControllerModeIndication>.
	 * This parameter depends on the parameter
	 * CANIF_USERCTRLMODEINDICATION_UL. If
	 * CANIF_USERCTRLMODEINDICATION_UL equals CAN_SM the
	 * name of <User_ControllerModeIndication> is fixed. If
	 * CANIF_USERCTRLMODEINDICATION_UL equals CDD, the name of
	 * <User_ControllerModeIndication> is selectable.
	 *
	 * dependency: CanIfDispatchUserCtrlModeIndicationUL.
	 */
	void (*CanIfDispatchUserCtrlModeIndication)(Can_ControllerStateType);

	/*
	 * This parameter defines the upper layer (UL) module to which the
	 * notifications of all ControllerTransition events from the CAN Driver
	 * modules have to be routed via <User_ControllerModeIndication>.
	 */
	CanIfDispatchUserCtrlModeIndicationULType CanIfDispatchUserCtrlModeIndicationUL;

	/*
	 * This parameter defines the name of <User_TrcvModeIndication>. This
	 * parameter depends on the parameter
	 * CANIF_USERTRCVMODEINDICATION_UL. If
	 * CANIF_USERTRCVMODEINDICATION_UL equals CAN_SM the
	 * name of <User_TrcvModeIndication> is fixed. If
	 * CANIF_USERTRCVMODEINDICATION_UL equals CDD, the name of
	 * <User_TrcvModeIndication> is selectable.
	 *
	 * dependency: CanIfDispatchUserTrcvModeIndicationUL.
	 */
	void (*CanIfDispatchUserTrcvModeIndication)(void);

	/*
	 * This parameter defines the upper layer (UL) module to which the
	 * notifications of all TransceiverTransition events from the CAN
	 * Transceiver Driver modules have to be routed via
	 * <User_TrcvModeIndication>. If no UL module is configured, no upper
	 * layer callback function will be called.
	 */
	CanIfDispatchUserTrcvModeIndicationULType CanIfDispatchUserTrcvModeIndicationUL;

#if (CANIF_WAKEUP_SUPPORT == STD_ON)
	/*
	 * This parameter defines the name of <User_ValidateWakeupEvent>.
	 * This parameter depends on the parameter
	 * CANIF_USERVALIDATEWAKEUPEVENT_UL.
	 * CANIF_USERVALIDATEWAKEUPEVENT_UL equals ECUM the name
	 * of <User_ValidateWakeupEvent> is fixed.
	 * CANIF_USERVALIDATEWAKEUPEVENT_UL equals CDD, the name
	 * of <User_ValidateWakeupEvent> is selectable. If parameter
	 * CANIF_WAKEUP_CHECK_VALIDATION_API is disabled, no
	 * <User_ValidateWakeupEvent> API can be configured.
	 *
	 * dependency: CANIF_WAKEUP_CHECK_VALIDATION_API,
	 * CanIfDispatchUserValidateWakeupEventUL
	 */
	void (*CanIfDispatchUserValidateWakeupEvent)(void);

	/*
	 * This parameter defines the upper layer (UL) module to which the
	 * notifications about positive former requested wake up sources have to
	 * be routed via <User_ValidateWakeupEvent>. If parameter
	 * CANIF_WAKEUP_CHECK_VALIDATION_API is disabled, this
	 * parameter cannot be configured.
	 *
	 * dependency: CANIF_WAKEUP_CHECK_VALIDATION_API.
	 */
	CanIfDispatchUserValidateWakeupEventULType CanIfDispatchUserValidateWakeupEventUL;
#endif
} CanIfDispatchCfg;

/*--------------------------------------------------------------------------------*/

/*
 * This container contains the configuration (parameters) of an addressed
 * CAN controller by an underlying CAN Driver module. This container is
 * configurable per CAN controller.
 */
typedef struct
{
	/*
	 * This parameter abstracts from the CAN Driver specific parameter
	 * Controller. Each controller of all connected CAN Driver modules shall
	 * be assigned to one specific ControllerId of the CanIf. Range:
	 * 0..number of configured controllers of all CAN Driver modules.
	 */
	uint8 CanIfCtrlId;

	/*
	 * This parameter defines if a respective controller of the referenced CAN
	 * Driver modules is queriable for wake up events.
	 */
	boolean CanIfCtrlWakeupSupport;

	/*
	 * This parameter references to the logical handle of the underlying CAN
	 * controller from the CAN Driver module to be served by the CAN
	 * Interface module. The following parameters of CanController config
	 * container shall be referenced by this link: CanControllerId,
	 * CanWakeupSourceRef
	 * Range: 0..max. number of underlying supported CAN controllers
	 *
	 * dependency: amount of CAN controllers.
	 */
	Can_ConfigType* CanIfCtrlCanCtrlRef;
} CanIfCtrlCfg;

/*
 * Configuration parameters for all the underlying CAN Driver modules
 * are aggregated under this container. For each CAN Driver module a
 * separate instance of this container has to be provided.
 */
typedef struct
{
	/* Reference to the Init Hoh Configuration */
	CanIfInitHohCfg* CanIfCtrlDrvInitHohConfigRef;

	/*
	 * CAN Interface Driver Reference.
	 * This reference can be used to get any information (Ex. Driver Name,
	 * Vendor ID) from the CAN driver.
	 *
	 * The CAN Driver name can be derived from the ShortName of the CAN
	 * driver module.
	 */
	uint8 CanIfCtrlDrvNameRef[32];

	/*
	* This container contains the configuration (parameters)
	* of an addressed CAN controller by an underlying CAN
	* Driver module. This container is configurable per CAN controller.
	*/
	CanIfCtrlCfg CtrlCfg[NUM_OF_CAN_CONTROLLERS];
} CanIfCtrlDrvCfg;

/*--------------------------------------------------------------------------------*/

/*
 * This container contains the configuration (parameters) of one
 * addressed CAN transceiver by the underlying CAN Transceiver Driver
 * module. For each CAN transceiver a separate instance of this
 * container has to be provided.
 */
typedef struct
{
	/*
	 * This parameter abstracts from the CAN Transceiver Driver specific
	 * parameter Transceiver. Each transceiver of all connected CAN
	 * Transceiver Driver modules shall be assigned to one specific
	 * TransceiverId of the CanIf.
	 *
	 * Range: 0..number of configured transceivers of all CAN Transceiver
	 * Driver modules
	 */
	uint8 CanIfTrcvId;

	/*
	 * This parameter defines if a respective transceiver of the referenced
	 * CAN Transceiver Driver modules is queriable for wake up events.
	 */
	boolean CanIfTrcvWakeupSupport;


} CanIfTrcvCfg;

/*
 * This container contains the configuration (parameters) of all addressed
 * CAN transceivers by each underlying CAN Transceiver Driver module.
 * For each CAN transceiver Driver a separate instance of this container
 * shall be provided.
 */
typedef struct
{
	/*
	 * This container contains the configuration (parameters) of
	 * one addressed CAN transceiver by the underlying CAN
	 * Transceiver Driver module. For each CAN transceiver a
	 * separate instance of this container has to be provided.
	 */
	CanIfTrcvCfg TrcvCfg[NUM_OF_CAN_TRCVS];
} CanIfTrcvDrvCfg;

/*
 * This type defines a data structure for the post build parameters of the CAN
 * interface for all underlying CAN drivers.
 * At initialization the CanIf gets a
 * pointer to a structure of this type to get access to its configuration data,
 * which is necessary for initialization.
 */
typedef struct
{
	/*
	 * This container contains the private configuration
	 * (parameters) of the CAN Interface.
	 */
	const CanIfPrivateCfg* PrivateCfg;

	/*
	 * This container contains the public configuration
	 * (parameters) of the CAN Interface.
	 */
	const CanIfPublicCfg* PublicCfg;

	/* This container contains the init parameters of
	 * the CAN Interface.
	 */
	const CanIfInitCfg* InitCfg;

	/*
	 * Callback functions provided by upper layer modules of
	 * the CanIf. The callback functions defined in this
	 * container are common to all configured CAN Driver /
	 * CAN Transceiver Driver modules.
	 */
	const CanIfDispatchCfg* DispatchCfg;

	/*
	 * Configuration parameters for all the underlying CAN
	 * Driver modules are aggregated under this container.
	 * For each CAN Driver module a separate instance of
	 * this container has to be provided.
	 */
	const CanIfCtrlDrvCfg* CtrlDrvCfg[NUM_OF_CAN_CONTROLLERS];

	/*
	 * This container contains the configuration (parameters)
	 * of all addressed CAN transceivers by each underlying
	 * CAN Transceiver Driver module. For each CAN
	 * transceiver Driver a separate instance of this container
	 * shall be provided.
	 */
	const CanIfTrcvDrvCfg* TrcvDrvCfg[NUM_OF_CAN_TRCVS];
} CanIf_ConfigType;

#endif
