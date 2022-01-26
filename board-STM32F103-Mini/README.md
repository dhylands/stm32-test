A modified version of usbdfu.bin was built from the generic-stm32f103 branch
of https://github.com/dhylands/libopencm3-examples
which was forked from https://github.com/libopencm3/libopencm3-examples

The source can be found in the examples/stm32/f1/stm32-h103/usb_dfu directory.

The bootloader runs at 0x08000000 and expects the application program to run
at 0x08002000.

You can use dfu-util or pydfu.py to upload.

Use stm32loader.py using the UART bootloader to flash usbdfu.bin

## Flashing the USB DFU bootloader

### Connect up a tty-level serial adapter

| Pin  | Signal    | Serial Adapter |
| ---- | --------- | -------------- |
| PA9  | USART1_TX |  RXI           |
| PA10 | USART1_RX |  TXO           |
| GND  | Ground    |  Ground        |

Connect a UART to these pins, B0 jumper (closest to the edge of the board) to 1 and the B1 jumper (closest to the RESET signal) to 0.

### Unlock the chip

The board I got was locked. I made a few tweaks to stm32loader.py to add a -u
command to do the appropriate unlock:
```
../stm32loader.py -p /dev/ttyUSB0 -uV
```

### Flash usbdfu.bin
A precompiled version of usbdfu.bin can be found in the board-STM32F103-Mini directory.

```
../stm32loader.py -p /dev/ttyUSB0 -evw usbdfu.bin
```

Unplug and replug and you should see a USB device with 0483:df11 show up using
```dfu-util -l```

Use ```dfu-util``` to flash new firmware (or `make BOARD=STM32F103-Mini pgm` to flash using the Makefile from this repository).

### to boot into the Bootloader

Connect BOOT1 (aka PB2) to 1
Press RESET

Note: BOOT1 is closest to the RESET button.

### to boot into the Application

Connect BOOT1 (aka PB2) to 0
Press RESET

