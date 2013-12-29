TOOLCHAIN	?= /opt/toolchains/arm
ARMGNU		?= $(TOOLCHAIN)/bin/arm-none-eabi

BUILD_DIR	:= build
KERN_BASE	:= $(BUILD_DIR)/kernel
KERN_ELF 	:= $(KERN_BASE).elf
KERN_IMG	:= $(KERN_BASE).img

CPU		:= arm926ej-s
QEMU_CPU	:= arm926

CC		:= $(ARMGNU)-gcc
LD		:= $(ARMGNU)-ld
AS 		:= $(ARMGNU)-as
OBJCOPY		:= $(ARMGNU)-objcopy
NM		:= $(ARMGNU)-nm

SRC_ASM		:= $(shell find src -name '*.s')
SRC_C		:= $(shell find src -name '*.c')

OBJS		:= $(patsubst %.s,%.o,$(SRC_ASM))
OBJS		+= $(patsubst %.c,%.o,$(SRC_C))

INCLUDE		:= -I include
BASEFLAGS	:= -O2 -fpic -nostdlib
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

font/font_8x16.ld: font/font_8x16.png
	./scripts/mkfont font/font_8x16.png > font/font_8x16.ld

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

emu: $(KERN_ELF)
	qemu-system-arm \
		-machine integratorcp \
		-cpu $(QEMU_CPU) \
		-kernel $(KERN_ELF) \
		-serial stdio \
		-m 256

symbols:
	$(NM) -n $(KERN_ELF)

stats:
	@echo "Source (C)   =>" $(SRC_C)
	@echo "Source (ASM) =>" $(SRC_ASM)
	@echo "Objects      =>" $(OBJS)

todo:
	@egrep "(TODO|HACK|FIXME)" $$(find . -name '*.c' -or -name '*.h' -or -name '*.s')
