objs := head.o init.o nand.o main.o

nand.bin : $(objs)
	/arm-mini2440-linux-gnueabi_sdk-buildroot/bin/arm-mini2440-linux-gnueabi-ld -Tnand.lds	-o nand_elf $^
	/arm-mini2440-linux-gnueabi_sdk-buildroot/bin/arm-mini2440-linux-gnueabi-objcopy -O binary -S nand_elf $@
	/arm-mini2440-linux-gnueabi_sdk-buildroot/bin/arm-mini2440-linux-gnueabi-objdump -D -m arm  nand_elf > nand.dis

%.o:%.c
	/arm-mini2440-linux-gnueabi_sdk-buildroot/bin/arm-mini2440-linux-gnueabi-gcc -Wall -fno-stack-protector -c -O2 -o $@ $<

%.o:%.S
	/arm-mini2440-linux-gnueabi_sdk-buildroot/bin/arm-mini2440-linux-gnueabi-gcc -Wall -fno-stack-protector -c -O2 -o $@ $<

clean:
	rm -f  nand.dis nand.bin nand_elf *.o
