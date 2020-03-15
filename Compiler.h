/*
 * Compiler.h
 *
 *  Created on: Feb 22, 2020
 *      Author: Sherif_Khourshed
 */

#ifndef COMPILER_H_
#define COMPILER_H_

/* Id for our team in the AUTOSAR, for example ID = 88) */
#define COMPILER_VENDOR_ID                  	(88U)

/* Module Id */
#define COMPILER_MODULE_ID                  	(198U)

/* Module Version 1.0.0 */
#define COMPILER_SW_MAJOR_VERSION				(1U)
#define COMPILER_SW_MINOR_VERSION           	(0U)
#define COMPILER_SW_PATCH_VERSION           	(0U)


/* AUTOSAR Version 4.3.1 */
#define COMPILER_AR_RELEASE_MAJOR_VERSION		(4U)
#define COMPILER_AR_RELEASE_MINOR_VERSION  		(3U)
#define COMPILER_AR_RELEASE_REVISION_VERSION  	(1U)

/* The memory class AUTOMATIC shall be provided as empty definition, used for the declaration of local pointers */
#define AUTOMATIC

/* The memory class is used within type definitions, where no memory qualifier can be specified */
#define TYPEDEF

/* This is used to define the void pointer to zero definition */
#define NULL_PTR		((void *)0)

/* This is used to define the abstraction of compiler keyword inline */
//#define INLINE
#define INLINE			inline

/* This is used to define the local inline function */
//#define LOCAL_INLINE
#define LOCAL_INLINE	static inline


// This is used to define the abstraction of compiler keyword static
#define STATIC			static


/*
 * Functions definitions supported for old compiler for ptr (near and far)
 * not used know(Compiler Dependent)
 */

/* FUNC */
#define FUNC(rettype, memclass)						TYPEDEF

/* FUNC_P2CONST */
#define FUNC_P2CONST(rettype, ptrclass, memclass)	TYPEDEF

/* FUNC_P2VAR */
#define FUNC_P2VAR(rettype, ptrclass, memclass)		TYPEDEF

/* P2VAR */
#define P2VAR(ptrtype, memclass, ptrclass)			TYPEDEF

/* P2CONST */
#define P2CONST(ptrtype, memclass, ptrclass)		TYPEDEF

/* CONSTP2VAR */
#define CONSTP2VAR(ptrtype, memclass, ptrclass)		TYPEDEF

/* CONSTP2CONST */
#define CONSTP2CONST(ptrtype, memclass, ptrclass)	TYPEDEF

/* P2FUNC */
#define P2FUNC(rettype, ptrclass, fctname)			TYPEDEF

/* CONSTP2FUNC */
#define CONSTP2FUNC(rettype, ptrclass, fctname)		TYPEDEF

/* CONST */
#ifndef CONST	// As it usually uses
#define CONST(consttype, memclass)					TYPEDEF
#endif

/* VAR */
#define VAR(vartype, memclass)						TYPEDEF

#endif /* COMPILER_H_ */
