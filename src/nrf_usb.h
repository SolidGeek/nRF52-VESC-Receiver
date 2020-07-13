#ifndef NRF_USB_
#define NRF_USB_

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>

// nRF52 generel dependencies
#include "nrf.h"
#include "boards.h"
#include "nrf_drv_clock.h"

// Include USB library
#include "app_usbd_cdc_acm.h"
#include "app_usbd_serial_num.h"
#include "nrf_dfu_trigger_usb.h"
#include "app_timer.h"


void usb_init( void );

void usb_dfu_init( void );

void usb_printf(const char* format, ...);






#endif