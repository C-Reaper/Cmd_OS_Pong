CC ?= gcc
LD ?= ld
OBJCOPY ?= objcopy
CFLAGS ?= -m32 -ffreestanding -O2 -Wall -Wextra -fno-pic -fno-pie -nostdlib -Ikernel
LDFLAGS ?= -m elf_i386 -T linker.ld

OBJDIR ?= build
BOOT_OBJDIR := $(OBJDIR)/boot
KERNEL_OBJDIR := $(OBJDIR)/kernel

KERNEL_ELF = kernel.elf
OS_IMAGE = os-image.bin
ISO_IMAGE = pongos.iso
ISO_ROOT = iso-root
KERNEL_OBJS = \
	$(BOOT_OBJDIR)/boot.o \
	$(KERNEL_OBJDIR)/kernel.o \
	$(KERNEL_OBJDIR)/framebuffer.o \
	$(KERNEL_OBJDIR)/renderer.o \
	$(KERNEL_OBJDIR)/timer.o \
	$(KERNEL_OBJDIR)/font.o \
	$(KERNEL_OBJDIR)/keyboard.o \
	$(KERNEL_OBJDIR)/mouse.o \
	$(KERNEL_OBJDIR)/memcpy.o \
	$(KERNEL_OBJDIR)/pong.o

all: $(ISO_IMAGE)

$(BOOT_OBJDIR)/boot.o: boot/boot.asm
	mkdir -p $(BOOT_OBJDIR)
	@nasm -f elf32 -o $@ $<

$(KERNEL_OBJDIR)/%.o: kernel/%.c
	mkdir -p $(KERNEL_OBJDIR)
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
	rm -rf $(ISO_ROOT) $(OBJDIR)

.PHONY: all clean iso run
