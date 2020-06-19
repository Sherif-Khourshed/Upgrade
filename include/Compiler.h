/*
 * Compiler.h
 *
 *  Created on: Feb 23, 2020
 *      Author: Sherif_Khourshed
 */
 
#ifndef COMPILER_H
#define COMPILER_H

/* Id for our team in the AUTOSAR, for example ID = 88) */
#define COMPILER_VENDOR_ID                  (88U)
#define COMPILER_MODULE_ID                  (198U)
/* Module Version 1.0.0 */
#define COMPILER_SW_MAJOR_VERSION			(1U)
#define COMPILER_SW_MINOR_VERSION           (0U)
#define COMPILER_SW_PATCH_VERSION           (0U)


/* AUTOSAR Version 4.3.1 */
#define COMPILER_AR_RELEASE_MAJOR_VERSION	(4U)
#define COMPILER_AR_RELEASE_MINOR_VERSION   (3U)
#define COMPILER_AR_RELEASE_REVISION_VERSION   (1U)

/* AUTOSAR checking between Std Types and Compiler Module */
#if(COMPILER_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION)\
	||(COMPILER_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION)\
	||(COMPILER_RELEASE_REVISION_VERSION != STD_TYPES_AR_RELEASE_REVISION_VERSION)
#error "The AR version of Std_Types.h does not match the expected version"
#endif


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

#define FUNC(rettype,memclass) rettype

#define FUNC_P2VAR(rettype, ptrclass, memclass) rettype *(memclass)

#define FUNC_P2CONST(rettype, ptrclass, memclass)const rettype *(memclass)

#define P2VAR(ptrtype, memclass, ptrclass) ptrtype *

#define P2CONST(ptrtype, memclass, ptrclass) const ptrtype *

#define CONSTP2VAR(ptrtype,memclass,ptrclass) ptrtype * const

#define CONSTP2CONST(ptrtype, memclass, ptrclass) const ptrtype * const

#define P2FUNC(rettype,ptrclass,fctname) rettype (*fctname)

#ifndef CONST
#define CONST(consttype,memclass) const consttype
#endif

#define VAR(vartype,memclass) vartype

/*
 * Functions definitions supported for old compiler for ptr (near and far)
 * not used know
 */


#endif /* COMPILER_H */
