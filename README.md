# pulse-controller

## Devices

* `controller`
  * Arduino Mega 2560 (or compatible)
  * I²C controller
* `peripheral`
  * Arduino Nano (or compatible)
  * I²C peripheral

## Programming boards
`pinout.h` defines which devices are connected to which pins. It automatically selects a different pinout for AVR-based boards (e.g. Arduino Nano) and ESP32 boards. Other boards are not supported out of the box.

### ESP32 (WROOM-32, ESP32 Devkit V1 package)
* Select `DOIT ESP32 DEVKIT V1`
* Serial always seems to run at 115200 baud

### Arduino Nano (official board)
* Serial runs at 9600 baud (defined in software)

#### Official Nano
* Select `Arduino Nano`

#### 3rd-party Nano (using CH340 USB chip)
* For Elegoo boards, select `Arduino Uno`.
* For some other boards, select `Arduino Nano`. Under `Processor`, choose `ATMega 328p (Old bootloader)`
