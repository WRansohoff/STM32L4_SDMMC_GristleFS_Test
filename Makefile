TARGET = main

# Default target chip.
MCU ?= STM32L496RG

ifeq ($(MCU), STM32L496RG)
	MCU_FILES = STM32L496RGT6
	ST_MCU_DEF = STM32L496xx
	MCU_CLASS = L4
	MCU_SPEC = cortex-m4
endif

# Define the linker script location and chip architecture.
LD_SCRIPT = $(MCU_FILES).ld

# Toolchain definitions (ARM bare metal defaults)
TOOLCHAIN = /usr
CC = $(TOOLCHAIN)/bin/arm-none-eabi-gcc
AS = $(TOOLCHAIN)/bin/arm-none-eabi-as
LD = $(TOOLCHAIN)/bin/arm-none-eabi-ld
OC = $(TOOLCHAIN)/bin/arm-none-eabi-objcopy
OD = $(TOOLCHAIN)/bin/arm-none-eabi-objdump
OS = $(TOOLCHAIN)/bin/arm-none-eabi-size

# Assembly directives.
ASFLAGS += -c
ASFLAGS += -O0
ASFLAGS += -mcpu=$(MCU_SPEC)
ASFLAGS += -mthumb
ASFLAGS += -Wall
# (Set error messages to appear on a single line.)
ASFLAGS += -fmessage-length=0
ASFLAGS += -DVVC_$(MCU_CLASS)

# C compilation directives
CFLAGS += -mcpu=$(MCU_SPEC)
CFLAGS += -mthumb
ifeq ($(MCU_CLASS), F0)
	CFLAGS += -msoft-float
	CFLAGS += -mfloat-abi=soft
else
	CFLAGS += -mhard-float
	CFLAGS += -mfloat-abi=hard
	CFLAGS += -mfpu=fpv4-sp-d16
endif
CFLAGS += -Wall
CFLAGS += -g
CFLAGS += -Os
# (Disable strict aliasing for variable-length registers like SPI->DR)
CFLAGS += -fno-strict-aliasing
# (Set error messages to appear on a single line)
CFLAGS += -fmessage-length=0
# (Set system to ignore semihosted junk)
CFLAGS += --specs=nosys.specs
CFLAGS += -D$(ST_MCU_DEF)
CFLAGS += -DVVC_$(MCU_CLASS)

# Linker directives.
LSCRIPT = ./ld/$(LD_SCRIPT)
LFLAGS += -mcpu=$(MCU_SPEC)
LFLAGS += -mthumb
ifeq ($(MCU_CLASS), F0)
	LFLAGS += -msoft-float
	LFLAGS += -mfloat-abi=soft
else
	LFLAGS += -mhard-float
	LFLAGS += -mfloat-abi=hard
	LFLAGS += -mfpu=fpv4-sp-d16
endif
LFLAGS += -Wall
LFLAGS += --specs=nosys.specs
LFLAGS += -nostdlib
LFLAGS += -lgcc
LFLAGS += -T$(LSCRIPT)

AS_SRC    = ./boot_code/$(MCU_FILES)_core.S
AS_SRC   += ./vector_tables/$(MCU_FILES)_vt.S
C_SRC     = ./main.c
C_SRC    += ./global.c
C_SRC    += ./port/gpio.c
C_SRC    += ./port/rcc.c
C_SRC    += ./port/sdmmc.c
C_SRC    += ./port/tim.c

INCLUDE   = -I./
INCLUDE  += -I./src
INCLUDE  += -I./fs/src
INCLUDE  += -I./fs/src/block_drivers
INCLUDE  += -I./device_headers

OBJS  = $(AS_SRC:.S=.o)
OBJS += $(C_SRC:.c=.o)

.PHONY: all
all: $(TARGET).bin

%.o: %.S
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(TARGET).elf
	rm -f $(TARGET).bin
