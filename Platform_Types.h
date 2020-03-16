/*
 * Platform_Types.h
 *
 *  Created on: Feb 22, 2020
 *      Author: Sherif_Khourshed
 */

#ifndef PLATFORM_TYPES_H_
#define PLATFORM_TYPES_H_

/* Id for our team in the AUTOSAR, for example ID = 88) */
#define PLATFORM_VENDOR_ID                  (88U)

/* Module Version 1.0.0 */
#define PLATFORM_SW_MAJOR_VERSION			(1U)
#define PLATFORM_SW_MINOR_VERSION           (0U)
#define PLATFORM_SW_PATCH_VERSION           (0U)


/* AUTOSAR Version 4.3.1 */
#define COMPILER_AR_RELEASE_MAJOR_VERSION	(4U)
#define COMPILER_AR_RELEASE_MINOR_VERSION   (3U)
#define COMPILER_AR_RELEASE_PATCH_VERSION   (1U)


/* CPU Register Type Width */
#define CPU_TYPE_8		(8U)
#define CPU_TYPE_16		(16U)
#define CPU_TYPE_32		(32U)
#define CPU_TYPE_64		(64U)

/* CPU Bit Order Definition */
#define MSB_FIRST		(0U)			/* Big Endian Bit Ordering */
#define LSB_FIRST		(1U)			/* Little Endian Bit Ordering */

/* CPU Byte Order Definition */
#define HIGH_BYTE_FIRST	(0U)		/* Big Endian Byte Ordering */
#define LOW_BYTE_FIRST	(1U)		/* Little Endian Byte Ordering */

/* Platform type and endianess definitions, specific for S32k144 */
#define CPU_TYPE		CPU_TYPE_32
#define CPU_BIT_OREDER	MSB_FIRST
#define CPU_BYTE_ORDER	HIGH_BYTE_FIRST


/* Boolean Values */
#ifndef TRUE
#define TRUE	(1U)
#endif
#ifndef FALSE
#define FALSE	(0U)
#endif

/* AUTOSAR Integer Data Types */
typedef unsigned char		boolean;				/* 			Boolean Data Type 0 or 1 		 */
typedef unsigned char 		uint8;					/*           		 0 .. 255                */
typedef signed char 		sint8;					/*        		   -128 .. +127              */
typedef unsigned short 		uint16;					/*           		0 .. 65535  	         */
typedef signed short 		sint16;					/*      		-32768 .. +32767			 */
typedef unsigned long 		uint32;					/*               0 .. 4294967295 		     */
typedef signed long 		sint32;					/* 		   -2147483648 .. +2147483647 	     */
typedef unsigned long long 	uint64;					/*      	 0..18446744073709551615 		 */
typedef signed long long 	sint64;					/* -9223372036854775808..9223372036854775807 */
typedef float				float32;				/* 		-3.4028235e+38..+3.4028235e+38	     */
typedef double				float64;				/* 		-3.4028235e+38..+3.4028235e+38	     */
/* For Optimization because of ALignnment and Padding */
typedef unsigned long 		uint8_least;			/*               At least 0..255 		     */
typedef unsigned long 		uint16_least;			/*              At least 0..65535 		     */
typedef unsigned long 		uint32_least;			/*            At least 0..4294967295 		 */
typedef signed long 		sint8_least;			/* 		   		At least -128..+127 	     */
typedef signed long 		sint16_least;			/* 		  	  At least -32768..+32767 	     */
typedef signed long 		sint32_least;			/* 		At least -2147483648..+2147483647    */

#endif /* PLATFORM_TYPES_H_ */
