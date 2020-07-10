# nrf52_vesc

This is the code used one my nRF52 VESC receiver, for communicating between the VESC and VESC Tool (linux and mobile) over BLE. After uploading the firmware, the NRF can be connected to the VESC using the RX and TX pins chosen in main.c, and the BLE scanner in VESC Tool should be able to find it and connect. Note that the UART port on the VESC must be enabled with a baud rate of 115200 for this to work. The NRF can also communicate with the VESC Remote at the same time as it runs BLE.  

The code can be build using Visual Studio Code with the NRF52 SDK by specifying the path in the Makefile. 

This is how to connect an J-LINK EDU MINI or STLINK V2 to the NRF52 for uploading the code:

| NRF52         | J-LINK EDU MINI|
| ------------- |----------------|
| GND           | GND            |
| VDD           | 3.3V           |
| SDO           | SWDIO          |
| SCL           | SWCLK          |

After the code is uploaded, the NRF52 can be connected to the VESC in the following way:

| NRF52         | VESC          |
| ------------- |---------------|
| GND           | GND           |
| VDD           | VCC (3.3V)    |
| Px.y (TX)     | RX            |
| Px2.y2 (RX)   | TX            |
