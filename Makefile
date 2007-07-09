CC=gcc
CFLAGS  = -Wall -nostdlib -nostdinc -ffreestanding
LDFLAGS = --warn-common
OBJECTS = boot/multiboot.o \
		drivers/videomem.o \
		core/libc.o core/akernel.o\
		arch/gdt.o arch/idt.o

KERNEL_OBJ = akernel.elf
MULTIBOOT_IMAGE = fd.img
PWD := $(shell pwd)

all: $(MULTIBOOT_IMAGE)

$(MULTIBOOT_IMAGE): $(KERNEL_OBJ)
	sh ./tools/mkdisk.sh $@ $<


$(KERNEL_OBJ): $(OBJECTS)
	$(LD) $(LDFLAGS) -T ./boot/akernel.lds -o $@ $^
	nm -C $@ | cut -d ' ' -f 1,3 > akernel.map


# Create objects from C source code
%.o: %.c
	$(CC) -I$(PWD) -c $< $(CFLAGS) -o $@

# Create objects from assembler (.S) source code
%.o: %.S
	$(CC) -I$(PWD) -c $< $(CFLAGS) -DASM_SOURCE=1 -o $@

# Clean directory
clean:
	$(RM) $(MULTIBOOT_IMAGE) menu.lst mtoolsrc $(KERNEL_OBJ) akernel.map
	$(RM) boot/*.o
	$(RM) drivers/*.o
	$(RM) arch/*.o
	$(RM) core/*.o
