*DEPRECATED. I've now moved to using [ESPHome](https://esphome.io/) for my fan controller. See [here](https://github.com/johnboiles/homeassistant-config/tree/master/esphome) for my ESPHome configs (specifically look at [`basement_fan.yaml`](https://github.com/johnboiles/homeassistant-config/blob/master/esphome/basement_fan.yaml).*

# esp-fan-controller
![TravisCI Build Status](https://travis-ci.org/johnboiles/esp-fan-controller.svg?branch=master)

Control a PWM'd fan over MQTT with an ESP12 and a mostfet.

Uses:
* [Wemos D1 Mini](https://www.amazon.com/Makerfocus-NodeMcu-Development-ESP8266-ESP-12F/dp/B01N3P763C)
* [Mosfet Driver Board](https://www.amazon.com/gp/product/B01J78FX9S) to drive the fan
* [Adjustable Regulator](https://www.amazon.com/gp/product/B01MQGMOKI) to power the ESP.
* MQTT protocol for communication

## Compiling the code

First off you'll need to create a `src/secrets.h`. This file is `.gitignore`'d so you don't put your passwords on Github.

    cp src/secrets.example.h src/secrets.h

Then edit your `src/secrets.h` file to reflect your wifi ssid/password and mqtt server/username/password.

The easiest way to build and upload the code is with the [PlatformIO IDE](http://platformio.org/platformio-ide).

The first time you program your board you'll want to do it over USB. After that, programming can be done over wifi. To program over USB, change the `upload_port` in the `platformio.ini` file to point to the USB-serial device for your board. Probably something like the following will work if you're on a Mac.

    upload_port = /dev/tty.cu*

If you're not using the NodeMCU board, you'll also want to update the `board` line with your board. See [here](http://docs.platformio.org/en/latest/platforms/espressif8266.html) for other PlatformIO supported ESP8266 board. For example, for the Wemos D1 Mini:

    board = d1_mini

After that, from the PlatformIO Atom IDE, you should be able to go to PlatformIO->Upload in the menu.
