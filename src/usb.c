#include "usb.h"

// As DFU Trigger uses interface 0, CDC must use 1 and 2
#define CDC_ACM_COMM_INTERFACE  1
#define CDC_ACM_DATA_INTERFACE  2

// Define instance "m_app_cdc_acm" for USB communication
APP_USBD_CDC_ACM_GLOBAL_DEF(m_app_cdc_acm,
                            NULL,
                            CDC_ACM_COMM_INTERFACE, 
                            CDC_ACM_DATA_INTERFACE,
                            NRF_DRV_USBD_EPIN2, // CDC_ACM_COMM_EPIN
                            NRF_DRV_USBD_EPIN1, // CDC_ACM_DATA_EPIN
                            NRF_DRV_USBD_EPOUT1, //CDC_ACM_DATA_EPOUT
                            APP_USBD_CDC_COMM_PROTOCOL_AT_V250);


static void usbd_user_ev_handler(app_usbd_event_type_t event) {
	switch (event) {
	case APP_USBD_EVT_DRV_SUSPEND:
		break;
	case APP_USBD_EVT_DRV_RESUME:
		break;
	case APP_USBD_EVT_STARTED:
		break;
	case APP_USBD_EVT_STOPPED:
		app_usbd_disable();
		break;
	case APP_USBD_EVT_POWER_DETECTED:
		if (!nrf_drv_usbd_is_enabled()) {
			app_usbd_enable();
		}
		break;
	case APP_USBD_EVT_POWER_REMOVED:
		app_usbd_stop();
		break;
	case APP_USBD_EVT_POWER_READY:
		app_usbd_start();
		break;
	default:
		break;
	}
}


void usb_printf(const char* format, ...) {

	va_list arg;
	va_start (arg, format);
	int len;
	static char print_buffer[255];

	len = vsnprintf(print_buffer, sizeof(print_buffer), format, arg);
	va_end (arg);

	if(len > 0) {
		app_usbd_cdc_acm_write(&m_app_cdc_acm, print_buffer, len < sizeof(print_buffer) ? len : sizeof(print_buffer));
	}

}

void usb_dfu_init( void ){

    nrf_drv_clock_lfclk_request(NULL);
    while(!nrf_drv_clock_lfclk_is_running()){}

    app_timer_init();

    // Enable DFU Trigger library over USB (should happen right after usbd_init)
    nrf_dfu_trigger_usb_init();
}


void usb_init( void ){

    static const app_usbd_config_t usbd_config = {
			.ev_state_proc = usbd_user_ev_handler
	};

	app_usbd_serial_num_generate();
	app_usbd_init(&usbd_config);

    // Initialize the USB DFU Trigger Interface
    usb_dfu_init();

	app_usbd_class_inst_t const * class_cdc_acm = app_usbd_cdc_acm_class_inst_get(&m_app_cdc_acm);
	app_usbd_class_append(class_cdc_acm);

    // Only enable USB, if power is supplied to the USB Bus
    app_usbd_power_events_enable();

}