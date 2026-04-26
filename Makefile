BUILD=build
ISO=iso

all: iso

$(BUILD)/kernel.bin:
	mkdir -p $(BUILD)
	nasm -f elf32 src/boot/boot.asm -o $(BUILD)/boot.o
	gcc -m32 -ffreestanding -Iinclude -nostdlib -c src/kernel/kernel.c -o $(BUILD)/kernel.o
	gcc -m32 -ffreestanding -Iinclude -nostdlib -c src/drivers/vga.c -o $(BUILD)/vga.o
	gcc -m32 -ffreestanding -Iinclude -nostdlib -c src/drivers/keyboard.c -o $(BUILD)/keyboard.o
	ld -m elf_i386 -T linker.ld -o $(BUILD)/kernel.bin -nostdlib \
	$(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/vga.o $(BUILD)/keyboard.o

iso: $(BUILD)/kernel.bin
	mkdir -p $(ISO)/boot
	cp $(BUILD)/kernel.bin $(ISO)/boot/
	grub-mkrescue -o katela.iso $(ISO)

run: iso
	qemu-system-i386 -cdrom katela.iso

clean:
	rm -rf build katela.iso
