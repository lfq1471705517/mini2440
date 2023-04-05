# mini2440
  Linux bsp about mini2440 board, including u-boot, linux, rootfs. They just implement basic functions. The following are boards and software 
information.

## 1. hardware introducitons

```
board:			friendlyarm mini2440
soc:			s3c2440
sdram:			64MB(em63a165ts-6g)
nand flash:		256MB(k9f2g08u0c)
sd card:		16GB
serial wire:		usb to 232 converter
```

## 2. how to build

```
  And after this, you can burn released_images directory's images with "1.3 how to burn".
  In the top directory, you only need to execute command "./build_mini2440.sh". And then you will get all images in the output folder. As for the 
compiling environments, for me, I used Ubuntu 20.04 LTS version as debugging. If you meet any compiling problems, please use search engine to find the right solving ways and install dependent packages.
```

## 3. how to burn

```
  First, you need to burn Superboot2440.bin into the nor flash through jlink. And second, use MiniTools software to burn u-boot.bin into nand flash. 
Third, plug in the net wire, reset the board and make uboot running, use the following command to burn zImage.
	tftp 0x30008000 zImage
	nand erase 0x200000 0x500000
	nand write 0x30008000 0x200000 $(zImage size)
  Fourth, you need to find a sd card. I'm using a sd card to u disk device to store rootfs because my nand flash seems to be going wrong. So I have 
no choice. In Ubuntu, execute "sudo fdisk /dev/sdb", and then delete all partitions. At the same time, in order to create one partition, execute command:
	n
	p
	1
	w
	sudo mkfs.ext4 /dev/sdb1
	sudo mount /dev/sdb1 /mnt
	sudo tar jxvf rootfs.tar.bz2 -C /mnt
	sudo sync
	sudo umount /mnt
  Finally, we finished the rootfs burning. Turn off the board, insert the sd card and power on. Through the serial port, you will see the boot log 
information. To enter to the system, you must know the user name is "root" and no password.
```

