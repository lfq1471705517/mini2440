sdram.bin : head.S leds.c
	/arm-mini2440-linux-gnueabi_sdk-buildroot/bin/arm-mini2440-linux-gnueabi-gcc  -c -o head.o head.S
	/arm-mini2440-linux-gnueabi_sdk-buildroot/bin/arm-mini2440-linux-gnueabi-gcc -c -o leds.o leds.c
	/arm-mini2440-linux-gnueabi_sdk-buildroot/bin/arm-mini2440-linux-gnueabi-ld -Ttext 0x30000000 head.o leds.o -o sdram_elf
	/arm-mini2440-linux-gnueabi_sdk-buildroot/bin/arm-mini2440-linux-gnueabi-objcopy -O binary -S sdram_elf sdram.bin
	/arm-mini2440-linux-gnueabi_sdk-buildroot/bin/arm-mini2440-linux-gnueabi-objdump -D -m arm  sdram_elf > sdram.dis
clean:
	rm -f   sdram.dis sdram.bin sdram_elf *.o
