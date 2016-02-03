BOARD_DIR = board-$(BOARD)
ifeq ($(wildcard $(BOARD_DIR)/.),)
$(error Invalid BOARD specified)
endif
include $(BOARD_DIR)/config.mk

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
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE = $(CROSS_COMPILE)size

INC =  -I.
INC += -Icmsis/inc
INC += -Icmsis/devinc
INC += -Ihal/f4
INC += -I$(BOARD_DIR)

CFLAGS_CORTEX_M4 = -mthumb -mtune=cortex-m4 -mabi=aapcs-linux -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fsingle-precision-constant -Wdouble-promotion
CFLAGS = $(INC) -Wall -ansi -std=gnu99 -nostdlib $(CFLAGS_CORTEX_M4) $(COPT)

#Debugging/Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -g
COPT = -O0
else
COPT += -Os -DNDEBUG
endif

LDFLAGS = --nostdlib -T $(BOARD_DIR)/$(LDSCRIPT) -Map=$(@:.elf=.map) --cref

OBJ = $(addprefix $(BUILD)/,\
	$(STARTUP_S:.s=.o) \
	system_stm32f4xx.o \
	clock.o \
	$(TARGET).o \
	stm32f4xx_it.o \
	stm32f4xx_hal.o \
	stm32f4xx_hal_cortex.o \
	stm32f4xx_hal_gpio.o \
	stm32f4xx_hal_rcc.o \
	)

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

vpath %.c hal/f4 $(BOARD_DIR)
$(BUILD)/%.o: %.c
	$(call compile_c)

pgm: $(BUILD)/$(TARGET).bin
	dfu-util -a 0 -D $^ -s 0x8000000:leave

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $^ $@

$(BUILD)/$(TARGET).elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $(OBJ) $(LIBS)
	$(Q)$(SIZE) $@

stlink: $(BUILD)/$(TARGET).bin
	$(Q)st-flash --reset write $(BUILD)/$(TARGET).bin 0x08000000

clean:
	$(RM) -rf $(BUILD)
.PHONY: clean

-include $(OBJ:.o=.P)

