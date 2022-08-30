@******************************************************************************
@ File��head.s
@ ���ܣ�����SDRAM���������Ƶ�SDRAM��Ȼ������SDRAM����ִ��
@******************************************************************************       
  
.text
.global _start
_start:
                                            @����disable_watch_dog, memsetup, init_nand, nand_read_ll��init.c�ж���
            ldr     sp, =4096               @���ö�ջ 
            bl      disable_watch_dog       @��WATCH DOG
            bl      memsetup                @��ʼ��SDRAM
            bl      nand_init               @��ʼ��NAND Flash

                                            @��NAND Flash�е�ַ4096��ʼ��1024�ֽڴ���(main.c����õ�)���Ƶ�SDRAM��
                                            @nand_read_ll������Ҫ3��������
            ldr     r0,     =0x30000000     @1. Ŀ���ַ=0x30000000������SDRAM����ʼ��ַ
            mov     r1,     #4096           @2.  Դ��ַ   = 4096�����ӵ�ʱ��main.c�еĴ��붼����NAND Flash��ַ4096��ʼ��
            mov     r2,     #4096           @3.  ���Ƴ���= 2048(bytes)�����ڱ�ʵ���main.c�������㹻��
            bl      nand_read               @����C����nand_read

            ldr     sp, =0x34000000         @����ջ
            ldr     lr, =halt_loop          @���÷��ص�ַ
            ldr     pc, =main               @bָ���blָ��ֻ��ǰ����ת32M�ķ�Χ����������ʹ����pc��ֵ�ķ���������ת
halt_loop:
            b       halt_loop
