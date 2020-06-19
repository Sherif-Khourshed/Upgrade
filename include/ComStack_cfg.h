/*
 * ComStack_cfg.h
 *
 *  Created on: Mar 8, 2020
 *      Author: Sherif_Khourshed
 */

#ifndef COMSTACK_CFG_H_
#define COMSTACK_CFG_H_

/* Id for our team in the AUTOSAR, for example ID = 88) */
//#define COMSTACK_CFG_VENDOR_ID                  	(88U)

/* Module Id */
//#define COMSTACK_CFG_MODULE_ID                  	(196U)

/* Module Version 1.0.0 */
#define COMSTACK_CFG_SW_MAJOR_VERSION				(1U)
#define COMSTACK_CFG_SW_MINOR_VERSION           	(0U)
#define COMSTACK_CFG_SW_PATCH_VERSION           	(0U)


/* AUTOSAR Version 4.3.1 */
#define COMSTACK_CFG_AR_RELEASE_MAJOR_VERSION		(4U)
#define COMSTACK_CFG_AR_RELEASE_MINOR_VERSION   	(3U)
#define COMSTACK_CFG_AR_RELEASE_REVISION_VERSION    (1U)


/* The size of this global type depends on the maximum number of PDUs used */
/* typedef uint8 PduIdType; */
typedef uint16 PduIdType;

/* The size of this global type depends on the maximum length of PDUs to be sent by an ECU */
/*
If no segmentation is used the length depends on the maximum payload size of a frame
of the underlying communication system (for FlexRay maximum size is 255, therefore uint8).
If segmentation is used it depends on the maximum length of a segmented N-PDU (in general uint16 is used)
*/
/* typedef uint8 PduLengthType; */
/* typedef uint32 PduLengthType; */
typedef uint16 PduLengthType;

#endif /* COMSTACK_CFG_H_ */
