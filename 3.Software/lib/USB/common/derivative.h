/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */

/* Include the derivative-specific header file */

/*
 * Include the platform specific header file
 */
#if (defined(TWR_K40X256))
  #include <MK40N512VMD100.h>
#elif (defined(TWR_K53N512))
  #include <MK53N512CMD100.h>
#elif (defined(LPLD_K60))
  #include "common.h" //lpld modify
#else
  #error "No valid platform defined"
#endif

#define __MK_xxx_H__
#define LITTLE_ENDIAN
#define __NO_SETJMP //lpld modify
//#define printf printf_kinetis
//#define sprintf sprintf_kinetis
 
//define current version of Freescal USB stack
#define FREESCALE_USB_STACK_VERSION (v4.1.1) //lpld modify

