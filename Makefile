CC ?= gcc
LD ?= ld
OBJCOPY ?= objcopy
CFLAGS ?= -m32 -ffreestanding -O2 -Wall -Wextra -fno-pic -fno-pie -nostdlib -Ikernel
LDFLAGS ?= -m elf_i386 -T linker.ld

KERNEL_ELF = kernel.elf
OS_IMAGE = os-image.bin
ISO_IMAGE = pongos.iso
ISO_ROOT = iso-root
KERNEL_OBJS = \
	boot/boot.o \
	kernel/kernel.o \
	kernel/framebuffer.o \
	kernel/renderer.o \
	kernel/timer.o \
	kernel/keyboard.o \
	kernel/memcpy.o \
	kernel/pong.o

all: $(ISO_IMAGE)

boot/boot.o: boot/boot.asm
	@nasm -f elf32 -o $@ $<

kernel/%.o: kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_ELF): $(KERNEL_OBJS)
	$(LD) $(LDFLAGS) -o $@ $(KERNEL_OBJS)

$(OS_IMAGE): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@

$(ISO_ROOT)/boot/kernel.elf: $(KERNEL_ELF)
	mkdir -p $(ISO_ROOT)/boot
	cp $< $@

$(ISO_ROOT)/boot/grub/grub.cfg: grub.cfg
	mkdir -p $(ISO_ROOT)/boot/grub
	cp $< $@

$(ISO_IMAGE): $(ISO_ROOT)/boot/kernel.elf $(ISO_ROOT)/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(ISO_ROOT) >/dev/null

run: $(ISO_IMAGE)
	@if [ -n "$$DISPLAY" ]; then \
		qemu-system-i386 -cdrom $(ISO_IMAGE) -boot d -m 64M -display gtk -no-reboot -no-shutdown; \
	else \
		qemu-system-i386 -cdrom $(ISO_IMAGE) -boot d -m 64M -nographic -serial stdio -no-reboot -no-shutdown; \
	fi

iso: $(ISO_IMAGE)

clean:
	rm -f $(OS_IMAGE) $(KERNEL_ELF) $(ISO_IMAGE) $(KERNEL_OBJS)
	rm -rf $(ISO_ROOT)

.PHONY: all clean iso run
