#ifndef __USB_DRIVER_H__
#define __USB_DRIVER_H__

#include "usb_dci_kinetis.h"

#if(USB_DEVICE_CLASS == USB_DEVICE_CLASS_CDC)
#include "usb_cdc.h"
#include "virtual_com.h"
#elif(USB_DEVICE_CLASS == USB_DEVICE_CLASS_HID)
#include "usb_hid.h"
#include "mouse_button.h"
#endif

#endif