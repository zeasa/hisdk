CROSS_PREFIX = riscv32-unknown-elf
CC = $(CROSS_PREFIX)-gcc
G++ = $(CROSS_PREFIX)-g++
AS = $(CROSS_PREFIX)-as
LD = $(CROSS_PREFIX)-ld
NM = $(CROSS_PREFIX)-nm
OBJCOPY = $(CROSS_PREFIX)-objcopy
OBJDUMP = $(CROSS_PREFIX)-objdump
STRIP = $(CROSS_PREFIX)-strip

output_name = kernel_vadd_1c
config = ./link.lds

elf = $(output_name).elf
bin = $(output_name).bin
data = $(output_name).dat
hex = $(output_name).hex
dump = $(output_name).asm
BIN = kernel_vadd_1c.bin

obj = output/start.o output/hpu_api.o output/kernel_main.co

all: dir $(bin)

dir:
	mkdir -p output

$(bin): $(obj)
	$(CC) $(obj) -o output/$(elf) -T $(config) -nostartfiles -nostdlib
	# -static -lgcc -Wl,--nmagic -Wl,--gc-sections
	$(OBJDUMP) -DSxtd output/$(elf) > output/$(dump)
	$(OBJCOPY) -O binary output/$(elf) output/$(bin)
	#$(OBJCOPY) -O verilog output/$(elf) output/$(data)
	#$(OBJCOPY) -O ihex output/$(elf) output/$(hex)

output/%.o: src/%.s
	$(AS) -c $< -o $@ -march=rv32ima -mabi=ilp32

output/%.co: src/%.c
	$(CC) -g -c $< -o $@ -march=rv32ima -mabi=ilp32
	# -mcmodel=medany -ffunction-sections -fdata-sections

clean:
	rm -rf $(obj) output/*

