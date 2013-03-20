/*
**********************************************************************
*                          Micrium, Inc.
*                      949 Crestview Circle
*                     Weston,  FL 33327-1848
*
*                            uC/FS
*
*             (c) Copyright 2001 - 2006, Micrium, Inc.
*                      All rights reserved.
*
***********************************************************************

----------------------------------------------------------------------
File        : fs_dev.h
Purpose     : Define structures for Device Drivers
---------------------------END-OF-HEADER------------------------------
*/

#ifndef _FS_DEV_H_
#define _FS_DEV_H_

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif


/*********************************************************************
*
*             Global data types
*
**********************************************************************
*/

typedef struct FS_DEVICE_TYPE {
  const char *      name;
  int               (*dev_status)    (FS_u32 Unit);
  int               (*dev_read)         (FS_u32 Unit, FS_u32 SectorNo, void *pBuffer);
  int               (*dev_write)        (FS_u32 Unit, FS_u32 SectorNo, void *pBuffer);
  int               (*dev_ioctl)        (FS_u32 Unit, FS_i32 Cmd, FS_i32 Aux, void *pBuffer);
  int               (*dev_ReadBurst)    (FS_u32 Unit, FS_u32 SectorNo, FS_u32 NumSectors,       void *pBuffer);
  int               (*dev_WriteBurst)   (FS_u32 Unit, FS_u32 SectorNo, FS_u32 NumSectors, const void *pBuffer);
  int               (*dev_InitDevice)   (FS_u32 Unit);
  void              (*dev_InitMedium)   (FS_u32 Unit);
  int               (*dev_WriteMultiple)(FS_u32 Unit, FS_u32 SectorNo, FS_u32 NumSectors, const void *pBuffer);
} FS__device_type;




/*********************************************************************
*
*             Device Driver Function Tables
*
*  If you add an own device driver,
*  for its function table here.
*/


extern const FS__device_type    FS__ramdevice_driver;     /* RAMDISK_DRIVER function table */
extern const FS__device_type    FS__windrive_driver;      /* WINDRIVE_DRIVER function table */
extern const FS__device_type    FS__smcdevice_driver;     /* SMC_DRIVER function table */
extern const FS__device_type    FS__mmcdevice_driver;     /* MMC_DRIVER function table */
extern const FS__device_type    FS__idedevice_driver;     /* IDE_CF_DRIVER function table */
extern const FS__device_type    FS__flashdevice_driver;   /* FLASH_DRIVER function table */
extern const FS__device_type    FS__SFlash_Driver;        /* Serial FLASH_DRIVER function table */
extern const FS__device_type    FS__genericdevice_driver; /* GENERIC_DRIVER function table */
extern const FS__device_type    FS__NAND2K_Driver;        /* Nand2k driver function table */


#if defined(__cplusplus)
 }               /* Make sure we have C-declarations in C++ programs */
#endif

#endif  /* _FS_DEV_H_ */

/*************************** End of file ****************************/
