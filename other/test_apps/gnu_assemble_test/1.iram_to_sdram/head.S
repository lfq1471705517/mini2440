@*************************************************************************
@ File��head.S
@ ���ܣ�����SDRAM���������Ƶ�SDRAM��Ȼ������SDRAM����ִ��
@*************************************************************************       

.equ        MEM_CTL_BASE,       0x48000000
.equ        SDRAM_BASE,         0x30000000

.text
.global _start
_start:
    bl  disable_watch_dog               @ �ر�WATCHDOG������CPU�᲻������
    bl  memsetup                        @ ���ô洢������
    bl  copy_steppingstone_to_sdram     @ ���ƴ��뵽SDRAM��
                      @ ����SDRAM�м���ִ��
#if 0
    mov	r0, #0x32000000
	mov	r1, #0xa5
	str	r1, [r0]
	mov	r1, #0x00
	ldr	r1, [r0]
	cmp	r1, #0xa5
	bne	open_leds

loop:
    b loop

open_leds:
#endif
     ldr pc, =on_sdram
on_sdram:
    ldr sp, =0x34000000                 @ ���ö�ջ
    bl  main
halt_loop:
    b   halt_loop

disable_watch_dog:
    @ ��WATCHDOG�Ĵ���д0����
    mov r1,     #0x53000000
    mov r2,     #0x0
    str r2,     [r1]
    mov pc,     lr      @ ����

copy_steppingstone_to_sdram:
    @ ��Steppingstone��4K����ȫ�����Ƶ�SDRAM��ȥ
    @ Steppingstone��ʼ��ַΪ0x00000000��SDRAM����ʼ��ַΪ0x30000000
    
    mov r1, #0
    ldr r2, =SDRAM_BASE
    mov r3, #4*1024
1:  
    ldr r4, [r1],#4     @ ��Steppingstone��ȡ4�ֽڵ����ݣ�����Դ��ַ��4
    str r4, [r2],#4     @ ����4�ֽڵ����ݸ��Ƶ�SDRAM�У�����Ŀ�ص�ַ��4
    cmp r1, r3          @ �ж��Ƿ���ɣ�Դ��ַ����Steppingstone��δ��ַ��
    bne 1b              @ ��û�и����꣬����
    mov pc,     lr      @ ����

memsetup:
    @ ���ô洢�������Ա�ʹ��SDRAM������

    mov r1,     #MEM_CTL_BASE       @ �洢��������13���Ĵ����Ŀ�ʼ��ַ
    adrl    r2, mem_cfg_val         @ ��13��ֵ����ʼ�洢��ַ
    add r3,     r1, #52             @ 13*4 = 54
1:  
    ldr r4,     [r2], #4            @ ��ȡ����ֵ������r2��4
    str r4,     [r1], #4            @ ����ֵд��Ĵ���������r1��4
    cmp r1,     r3                  @ �ж��Ƿ�����������13���Ĵ���
    bne 1b                          @ ��û��д�ɣ�����
    mov pc,     lr                  @ ����


.align 4
mem_cfg_val:
    @ �洢������13���Ĵ���������ֵ
    .long   0x22011110      @ BWSCON
    .long   0x00000700      @ BANKCON0
    .long   0x00000700      @ BANKCON1
    .long   0x00000700      @ BANKCON2
    .long   0x00000700      @ BANKCON3  
    .long   0x00000700      @ BANKCON4
    .long   0x00000700      @ BANKCON5
    .long   0x00018005      @ BANKCON6
    .long   0x00018005      @ BANKCON7
    .long   0x008C07A3      @ REFRESH
    .long   0x000000B1      @ BANKSIZE
    .long   0x00000030      @ MRSRB6
    .long   0x00000030      @ MRSRB7
