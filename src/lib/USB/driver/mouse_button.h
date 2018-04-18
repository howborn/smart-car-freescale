/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2009 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 **************************************************************************//*!
 *
 * @file mouse_button.h
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file contains Macro's and functions needed by the Mouse
 *        Application
 *
 *****************************************************************************/


#ifndef _MOUSE_BUTTON_H
#define _MOUSE_BUTTON_H

#include "types.h"

/******************************************************************************
 * Constants - None
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/
#define  CONTROLLER_ID   (0)   /* ID to identify USB CONTROLLER */

/* #define UP_LEFT   This macro enables up and left mouse movement via PTG2-3*/
#define UP_LEFT
#ifdef UP_LEFT
#define SHIFT_VAL  (0xF8)    /* send -8 for up and left movement */
#else
#define SHIFT_VAL  (0x08)    /* send +8 for down and right movement */
#endif

#define  MOUSE_BUFF_SIZE   (4)   /* report buffer size */
#define  REQ_DATA_SIZE     (1)
#define  KBI_STAT_MASK     (0x0F)
   
#define  LEFT_CLICK        (0x01) /* Left click */
#define  RIGHT_CLICK       (0x02) /* Right click */
#define  LEFT_MOVE         (0xF8) /* Left movement */
#define  RIGHT_MOVE        (0x08) /* Right movement */
#define  UP_MOVE           (0xF8) /* up movement */
#define  DOWN_MOVE         (0x08) /* down movement */
   
#define  MOVE_LEFT_RIGHT   (0x04) /* Left-Right movement*/
#define  MOVE_UP_DOWN      (0x08) /* Up-Down movement*/

/*****************************************************************************
 * Global variables
 *****************************************************************************/
extern uint_8 kbi_stat; /*records the status of the buttons (PTG0-PTG3)
                                   used to emulate mouse */

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
void USB_HID_Mouse_Init(void);
uint8 USB_HID_Mouse_Task(void);
void Emulate_Mouse_WithButton(void);
void USB_HID_MouseControl(uint8 *cmd_buf);
#endif
