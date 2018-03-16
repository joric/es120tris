es120tris
=========

Tetris for ES120 screwdriver

[![Tetris for ES120 screwdriver](http://img.youtube.com/vi/Q2bpTqUKbqA/0.jpg)](https://www.youtube.com/watch?v=Q2bpTqUKbqA)

Installation
------------
Download .hex file in the [releases section](https://github.com/joric/es120tris/releases), upload via USB.


How to play
-----------
Triple press to start the game, accelerometer to move, short press to rotate, medium press to drop, long press to exit.


Hardware
--------

* STM32F103T8U6 (ARM Cortex M3, clock frequency 72 MHz)
* SSD1306 (White 96x16 OLED Display)
* MMA8652FC (3-Axis, 12-bit Digital Accelerometer)
* L3G4200D (Angular Rate Sensor)


Specifications
--------------

* Work Voltage: DC3-5V
* Material: 304 stainless steel
* No-load Speed:  About 270rpm
* Electronic Torque: 2.8kgf.cm
* Maximum Torque: 30kgf.cm (manual mode)
* Screwdriver Bit: 4M hexagon
* Charging Time: 45min
* Running Time: 30min (full power)
* Charging Mode:  Micro USB 5V Input
* Screen: 16*96 OLED
* Battery: 300mAh 10440 Lithium-ion battery
* Rotor:  Bidirectional overrunning clutch
* Gearbox: 1: 144 Integrated planetary gearbox
* Motor: 25,000 rpm brush motor
* Main Chip: STM32
* Sensor:  Angular velocity sensor

The only difference between ES121 and ES120 is the gearbox, 1:64 vs 1:144.


References
----------

* https://hackaday.com/2017/10/01/a-screwdriver-for-the-lazy/
* [Tetris for TS100 Soldering Iron](https://github.com/joric/ts100tris)
