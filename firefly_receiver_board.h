#ifndef FIREFLY_RECEIVER_BOARD_H
#define FIREFLY_RECEIVER_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

// --- Board defintions for Firefly nRF52480 receiver ---

/* LED definitions */
#define LEDS_NUMBER         1
#define LED_1               NRF_GPIO_PIN_MAP(1,10)
#define LEDS_ACTIVE_STATE   1
#define LEDS_LIST           { LED_1 }
#define LEDS_INV_MASK       LEDS_MASK
#define BSP_LED_0           LED_1

/* DFU reset pin
   Self reset pin used for DFU upgrade over USB UART 
   DFU is configured to be enabled by reset
   */
#define BSP_SELF_PINRESET_PIN NRF_GPIO_PIN_MAP(0,24) // Pin 10 on Adafruit nRF52480 board (connect RST to pin 10)

#define HWFC           true

#ifdef __cplusplus
}
#endif

#endif // FIREFLY_RECEIVER_BOARD_H