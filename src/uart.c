#include "uart.h"

volatile bool uart_is_enabled = true;
volatile bool m_uart_error = false;

app_uart_comm_params_t m_uart_comm_params =
{
		.rx_pin_no    = UART_RX,
		.tx_pin_no    = UART_TX,
		.rts_pin_no   = 0,
		.cts_pin_no   = 0,
		.flow_control = APP_UART_FLOW_CONTROL_DISABLED,
		.use_parity   = false,
		.baud_rate    = NRF_UART_BAUDRATE_115200
};

static void uart_event_handle(app_uart_evt_t * p_event) {
	switch (p_event->evt_type) {
	case APP_UART_DATA_READY: {
//		uint8_t byte;
//		while (app_uart_get(&byte) == NRF_SUCCESS) {
//			packet_process_byte(byte, PACKET_VESC);
//		}
	} break;

	case APP_UART_COMMUNICATION_ERROR:
//		m_uart_error = true;
		break;

	case APP_UART_FIFO_ERROR:
//		m_uart_error = true;
		break;

	default:
		break;
	}
}

void uart_set_enabled(bool state) {
	uart_is_enabled = state;

	if (uart_is_enabled) {
		app_uart_close();
		m_uart_comm_params.tx_pin_no = UART_TX;
		uart_init();
		nrf_gpio_cfg_default(UART_TX_DISABLED);
	} else {
		app_uart_close();
		m_uart_comm_params.tx_pin_no = UART_TX_DISABLED;
		uart_init();
		nrf_gpio_cfg_default(UART_TX);
	}
}

void uart_init(void) {
	uint32_t err_code;
	APP_UART_FIFO_INIT(&m_uart_comm_params,
			UART_RX_BUF_SIZE,
			UART_TX_BUF_SIZE,
			uart_event_handle,
			APP_IRQ_PRIORITY_LOW,
			err_code);
	APP_ERROR_CHECK(err_code);
}

void uart_send_buffer(unsigned char *data, unsigned int len) {
	for (int i = 0;i < len;i++) {
		app_uart_put(data[i]);
	}
}


void uart_handle( void ){

    if (m_uart_error) {
        app_uart_close();
        uart_init();
        packet_reset(PACKET_VESC);
        m_uart_error = false;
    }

    uint8_t byte;
    while (app_uart_get(&byte) == NRF_SUCCESS) {
        packet_process_byte(byte, PACKET_VESC);
    }

}