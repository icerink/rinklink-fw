# Iceskate USB firmware

The microcontroller firmware for tunneling the SPI over midi USB.
![We are building a gardena to mains adapter](https://etel-tuning.eu/479-thickbox_default/adapter-drehstrom-auf-gardena.jpg)

## Message encoding

For staying inside the midi spec, and being able to send only valid midi messages, we comunicate only through `note off` messages.
We can pack two bytes in one note off command by using 7bit velocity, 7bit note number and the 2lsb of the channel number.

| spi message (2 bytes) | midi message (3 bytes / one `note off`) |
| --------------------- | --------------------------------------- |
| 0bABCDEFGH 0bIJKLMNOP | 0b1001xxAI 0b0BCDEFGH 0b0JKLMNOP        |

## Build & Flash

1. Install `make`, `arm-none-eabi-gcc`, and `dfu-util`
2. run `make all`
3. shorten the left and the middle pin of JP1 of the iceskate board and plug in the usb cable
4. run `make flash`
