CC=gcc
CFLAGS  = -Wall -nostdlib -nostdinc -ffreestanding
LDFLAGS = --warn-common
OBJECTS = boot/multiboot.o \
		drivers/videomem.o \
		core/libc.o core/akernel.o\
		arch/gdt.o


KERNEL_OBJ   = akernel.elf
MULTIBOOT_IMAGE = fd.img
PWD := $(shell pwd)

# Main target
all: $(MULTIBOOT_IMAGE)

$(MULTIBOOT_IMAGE): $(KERNEL_OBJ)
	./tools/build_image.sh $@ $<

$(KERNEL_OBJ): $(OBJECTS)
	$(LD) $(LDFLAGS) -T ./tools/akernel.lds -o $@ $^
	-nm -C $@ | cut -d ' ' -f 1,3 > akernel.map

-include .mkvars

# Create objects from C source code
%.o: %.c
	$(CC) -I$(PWD) -c $< $(CFLAGS) -o $@

# Create objects from assembler (.S) source code
%.o: %.S
	$(CC) -I$(PWD) -c $< $(CFLAGS) -DASM_SOURCE=1 -o $@

# Clean directory
clean:
	$(RM) *.img *.o mtoolsrc *~ menu.txt *.img *.elf *.bin *.map
	$(RM) *.log *.out bochs*
	$(RM) boot/*.o boot/*~
	$(RM) drivers/*.o drivers/*~
	$(RM) carch/*.o arch/*~
	$(RM) core/*.o core/*~
	$(RM) tools/*~
