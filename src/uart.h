#ifndef FIREFLY_UART_
#define FIREFLY_UART_

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>

#include "def.h"
#include "app_uart.h"
#include "nrf_uart.h"
#include "nrf_gpio.h"
#include "packet.h"

extern volatile bool uart_is_enabled;

#define UART_TX_BUF_SIZE                16384
#define UART_RX_BUF_SIZE                16384

// Should probably be moved to board file
#define UART_RX							NRF_GPIO_PIN_MAP(0,03)
#define UART_TX							NRF_GPIO_PIN_MAP(0,02)
#define UART_TX_DISABLED				NRF_GPIO_PIN_MAP(0,28)

void uart_init(void);

void uart_handle( void ); 

void uart_set_enabled(bool state);

void uart_send_buffer(unsigned char *data, unsigned int len);

#endif // FIREFLY_UART_