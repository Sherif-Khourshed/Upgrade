#ifndef CAN_PBCFG_H
#define CAN_PBCFG_H

#define CAN_PB_CFG_AR_MAJOR_VERSION (4U)
#define CAN_PB_CFG_AR_MINOR_VERSION (3U)
#define CAN_PB_CFG_AR_PATCH_VERSION (1U)

#define CAN_PB_CFG_SW_MAJOR_VERSION (1U)
#define CAN_PB_CFG_SW_MINOR_VERSION (0U)
#define CAN_PB_CFG_SW_PATCH_VERSION (0U)

#include "Std_Types.h"

/* AUTOSAR checking between Std Types and Can_Cfg Module */
#if ((CAN_PB_CFG_AR_MAJOR_VERSION != STD_TYPES_AR_MAJOR_VERSION)\
 ||  (CAN_PB_CFG_AR_MINOR_VERSION != STD_TYPES_AR_MINOR_VERSION)\
 ||  (CAN_PB_CFG_AR_PATCH_VERSION != STD_TYPES_AR_PATCH_VERSION))
#error "The AR version of Std_Types.h does not match the expected version"
#endif


#endif      /* CAN_PBCFG_H */
