usbdfu.bin was built from https://github.com/libopencm3/libopencm3-examples
in the examples/stm32/f1/stm32-h103/usb_dfu directory.

It runs at 0x08000000 and expects the application program to run at 0x08002000

You can use dfu-util to upload.

Use stm32loader.py using the UART to flash usbdfu.bin

## Flashing the USB DFU bootloader

### Connect up a tty-level serial adapter

| Pin  | Signal    |
| ---- | --------- |
| PA9  | USART1_TX |
| PA10 | USART1_RX |

Connect a UART to these pins and set the B0 jumper to 1.

### Unlock the chip

The board I got was locked. I made a few tweaks to stm32loader.py to add a -u
command to do the appropriate unlock:
```
./stm32loader.py -p /dev/ttyUSB0 -uV
```

### Flash usbdfu.bin
```
./stm32loader.py -p /dev/ttyUSB0 -evw usbdfu.bin
```

Unplug and replug and you should see a USB device with 0483:df11 show up using
```dfu-util -l```

Use ```dfu-util``` to flash new firmware.
