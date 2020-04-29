# WeAct STM32F411CEU6

Micropython board definition files can be found [here](https://github.com/mcauser/WEACT_F411CEU6).

![board](docs/STM32F411CEU6.jpg)

This is a fairly simple breakout for the STM32F411CEU6 processor.

There is an LED connected to PC13 (Bring low to turn on LED).
There is a Pushbutton connected between GND and PA0 (no external pullup).

There is a 25 MHz crystal attached to HSE, and a 32.768kHz crystal attached to LSE.

There is a footprint on the back for an external 64 Mbit SPI chip, like the WinBond W25Q64FVSIG.

* [Schematics](docs/STM32F411CEU6_schematics.pdf)
* [PCB](STM32F411CEU6_pcb.pdf)
* [Data Sheet](docs/stm32f411ce-ds.pdf)
* [Reference Manual](docs/STM32F411xCE-rm.pdf)
