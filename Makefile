BOARD_DIR = board-$(BOARD)
ifeq ($(wildcard $(BOARD_DIR)/.),)
$(error Invalid BOARD specified)
endif
include $(BOARD_DIR)/config.mk

HAL_FAMILY ?= f4
FLASH_ADDR ?= 0x8000000

ifeq ($(findstring CYGWIN_NT,$(shell uname -s)),CYGWIN_NT)
USE_PYDFU = 0
else
USE_PYDFU = 1
endif

TARGET ?= blinky

# Turn on increased build verbosity by defining BUILD_VERBOSE in your main
# Makefile or in your environment. You can also use V=1 on the make command
# line.

ifeq ("$(origin V)", "command line")
BUILD_VERBOSE=$(V)
endif
ifndef BUILD_VERBOSE
BUILD_VERBOSE = 0
endif
ifeq ($(BUILD_VERBOSE),0)
Q = @
else
Q =
endif
# Since this is a new feature, advertise it
ifeq ($(BUILD_VERBOSE),0)
$(info Use make V=1 or set BUILD_VERBOSE in your environment to increase build verbosity.)
endif

BUILD ?= build-$(BOARD)

RM = rm
ECHO = @echo

CROSS_COMPILE = arm-none-eabi-

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
GDB = $(CROSS_COMPILE)gdb
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE = $(CROSS_COMPILE)size

INC =  -I.
INC += -Icmsis/inc
INC += -Icmsis/devinc
INC += -Ihal/$(HAL_FAMILY)
INC += -I$(BOARD_DIR)

CFLAGS_CORTEX_M3 = -mthumb -mtune=cortex-m3 -mabi=aapcs-linux -mcpu=cortex-m3 -fsingle-precision-constant -Wdouble-promotion
CFLAGS_CORTEX_M4 = -mthumb -mtune=cortex-m4 -mabi=aapcs-linux -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fsingle-precision-constant -Wdouble-promotion

CFLAGS_HAL_FAMILY_f1 = $(CFLAGS_CORTEX_M3)
CFLAGS_HAL_FAMILY_f4 = $(CFLAGS_CORTEX_M4)
CFLAGS_HAL_FAMILY_l4 = $(CFLAGS_CORTEX_M4)

CFLAGS = $(INC) -Wall -ansi -std=gnu99 -nostdlib $(CFLAGS_HAL_FAMILY_$(HAL_FAMILY)) $(COPT)

#Debugging/Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -g
COPT = -O0
else
COPT += -Os -DNDEBUG
endif

LDFLAGS = --nostdlib -T $(BOARD_DIR)/$(LDSCRIPT) -Map=$(@:.elf=.map) --cref

ifeq ($(TARGET),boot-stub)
OBJ = $(addprefix $(BUILD)/,\
	$(STARTUP_S:.s=.o) \
	$(TARGET).o \
	)
else
OBJ = $(addprefix $(BUILD)/,\
	$(STARTUP_S:.s=.o) \
	system_stm32$(HAL_FAMILY)xx.o \
	clock.o \
	gpio.o \
	$(TARGET).o \
	stm32$(HAL_FAMILY)xx_it.o \
	stm32$(HAL_FAMILY)xx_hal.o \
	stm32$(HAL_FAMILY)xx_hal_cortex.o \
	stm32$(HAL_FAMILY)xx_hal_gpio.o \
	stm32$(HAL_FAMILY)xx_hal_rcc.o \
	string0.o \
	)
endif
#	stm32$(HAL_FAMILY)xx_hal_pwr.o \
#	stm32$(HAL_FAMILY)xx_hal_pwr_ex.o \


all: $(BUILD)/$(TARGET).elf

define compile_c
$(ECHO) "CC $<"
$(Q)$(CC) $(CFLAGS) -c -MD -o $@ $<
@# The following fixes the dependency file.
@# See http://make.paulandlesley.org/autodep.html for details.
@cp $(@:.o=.d) $(@:.o=.P); \
  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
      -e '/^$$/ d' -e 's/$$/ :/' < $(@:.o=.d) >> $(@:.o=.P); \
  rm -f $(@:.o=.d)
endef

$(OBJ): | $(BUILD)
$(BUILD):
	mkdir -p $@

vpath %.s $(BOARD_DIR) cmsis/src
$(BUILD)/%.o: %.s
	$(ECHO) "AS $<"
	$(Q)$(AS) -o $@ $<

vpath %.c hal/$(HAL_FAMILY) $(BOARD_DIR)
$(BUILD)/%.o: %.c
	$(call compile_c)

pgm: $(BUILD)/$(TARGET).dfu
ifeq ($(USE_PYDFU),1)
	$(Q)./pydfu.py -u $^
else
	$(Q)dfu-util -a 0 -D $^ -s:leave
endif

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $^ $@

$(BUILD)/$(TARGET).dfu: $(BUILD)/$(TARGET).bin
	$(Q)./dfu.py -b $(FLASH_ADDR):$^ $@

$(BUILD)/$(TARGET).elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $(OBJ) $(LIBS)
	$(Q)$(SIZE) $@

stlink: $(BUILD)/$(TARGET).bin
	$(Q)st-flash --reset write $^ $(FLASH_ADDR)

uart: $(BUILD)/$(TARGET).bin
	$(Q)./stm32loader.py -p /dev/ttyUSB0 -evw $^

GDB_PORT_Linux = /dev/ttyACM0
GDB_PORT_Darwin = /dev/cu.usbmodemD5DFBBF1
run: $(BUILD)/$(TARGET).elf
	$(GDB) -ex 'target extended-remote ${GDB_PORT_$(shell uname)}' -x gdbinit $<


# Unprotect does a MASS erase, so it shouldn't try to flash as well.
# And on the STM32F103, the ACK never gets received
uart-unprotect:
	$(Q)./stm32loader.py -p /dev/ttyUSB0 -uV

clean:
	$(RM) -rf $(BUILD)
.PHONY: clean

-include $(OBJ:.o=.P)

