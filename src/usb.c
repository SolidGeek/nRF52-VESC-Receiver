#include "usb.h"

// As DFU Trigger uses interface 0, CDC must use 1 and 2
#define CDC_ACM_COMM_INTERFACE  1
#define CDC_ACM_DATA_INTERFACE  2

#define CDC_ACM_COMM_EPIN       NRF_DRV_USBD_EPIN2
#define CDC_ACM_DATA_EPIN       NRF_DRV_USBD_EPIN1
#define CDC_ACM_DATA_EPOUT      NRF_DRV_USBD_EPOUT1

// Predefinition
static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst, app_usbd_cdc_acm_user_event_t event);

// Define instance "m_app_cdc_acm" for USB communication
APP_USBD_CDC_ACM_GLOBAL_DEF(m_app_cdc_acm,
    cdc_acm_user_ev_handler,
    CDC_ACM_COMM_INTERFACE,
    CDC_ACM_DATA_INTERFACE,
    CDC_ACM_COMM_EPIN,
    CDC_ACM_DATA_EPIN,
    CDC_ACM_DATA_EPOUT,
    APP_USBD_CDC_COMM_PROTOCOL_NONE
);

// Handles events coming over USB (could be done using interrupts, however this can ruin the timing)
static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst, app_usbd_cdc_acm_user_event_t event) {
	switch (event) {
	case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN: {
        // nrf_gpio_pin_set(LED_PIN);
		// Setup first transfer
		char rx;
		app_usbd_cdc_acm_read(&m_app_cdc_acm, &rx, 1);
		break;
	}
	case APP_USBD_CDC_ACM_USER_EVT_PORT_CLOSE:
        // nrf_gpio_pin_clear(LED_PIN);
		break;
	case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
		break;
	case APP_USBD_CDC_ACM_USER_EVT_RX_DONE: {
		ret_code_t ret;
		char rx;

		do {
			ret = app_usbd_cdc_acm_read(&m_app_cdc_acm, &rx, 1);
		} while (ret == NRF_SUCCESS);
		break;
	}
	default:
		break;
	}
}

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
        nrf_delay_ms(1);
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