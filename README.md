# Iceskate USB firmware

The microcontroller firmware for the rinklink bridge. (see [rinklink-js](https://github.com/icerink/rinklink-js) for the computer side)
![We are building a gardena to mains adapter](https://etel-tuning.eu/479-thickbox_default/adapter-drehstrom-auf-gardena.jpg)

## Architecture & Message encoding

See [rinklink-js](https://github.com/icerink/rinklink-js).

## Build & Flash

1. Install `make`, `arm-none-eabi-gcc`, and `dfu-util`
2. run `make all`
3. shorten the left and the middle pin of JP1 of the iceskate board and plug in the usb cable
4. run `make flash`
