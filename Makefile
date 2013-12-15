TOOLCHAIN	?= /opt/toolchains/arm
ARMGNU		?= $(TOOLCHAIN)/bin/arm-none-eabi

BUILD_DIR	:= build
KERN_BASE	:= $(BUILD_DIR)/kernel
KERN_ELF 	:= $(KERN_BASE).elf
KERN_IMG	:= $(KERN_BASE).img

CPU			:= arm926ej-s
QEMU_CPU	:= arm926

CC			:= $(ARMGNU)-gcc
LD			:= $(ARMGNU)-ld
AS 			:= $(ARMGNU)-as
OBJCOPY		:= $(ARMGNU)-objcopy

SRC_ASM		:= $(wildcard **/*.s)
SRC_C		:= $(wildcard **/*.c)

OBJS		:= $(patsubst %.s,%.o,$(SRC_ASM))
OBJS		+= $(patsubst %.c,%.o,$(SRC_C))

INCLUDE		:= -I include
BASEFLAGS	:= -O2 -fpic -pedantic -pedantic-errors -nostdlib
BASEFLAGS	+= -nostartfiles -ffreestanding -nodefaultlibs
BASEFLAGS	+= -fno-builtin -fomit-frame-pointer -mcpu=$(CPU)
WARNFLAGS	:= -Wall -Wextra
#WARNFLAGS	+= -Werror
ASFLAGS		:= $(INCLUDE)
CFLAGS		:= $(INCLUDE) $(BASEFLAGS) $(WARNFLAGS)
CFLAGS		+= -std=gnu99

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s Makefile
	$(AS) $(ASFLAGS) -c $< -o $@

all: $(KERN_IMG)

$(BUILD_DIR):
	mkdir -p $@

$(KERN_ELF): $(BUILD_DIR) $(OBJS) linker.ld
	$(LD) $(OBJS) -T linker.ld -o $@

$(KERN_IMG): $(KERN_ELF)
	$(OBJCOPY) $(KERN_ELF) -O binary $@

obj: $(OBJS)

clean:
	rm -f $(OBJS)
	rm -rf $(BUILD_DIR)

emu: $(KERN_IMG)
	qemu-system-arm \
		-machine realview-eb \
		-cpu $(QEMU_CPU) \
		-kernel $(KERN_IMG) \
		-m 256

stats:
	@echo "Source (C)   =>" $(SRC_C)
	@echo "Source (ASM) =>" $(SRC_ASM)
	@echo "Objects      =>" $(OBJS)
