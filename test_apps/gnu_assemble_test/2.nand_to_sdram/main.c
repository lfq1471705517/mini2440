
#define	GPBCON		(*(volatile unsigned long *)0x56000010)
#define	GPBDAT		(*(volatile unsigned long *)0x56000014)

#define	GPB5_out	(1<<(5*2))
#define	GPB6_out	(1<<(6*2))
#define	GPB7_out	(1<<(7*2))

void  wait(volatile unsigned long dly)
{
	for(; dly > 0; dly--);
}

int main(void)
{
	unsigned long i = 0;
	
	GPBCON = GPB5_out|GPB6_out|GPB7_out;		// ��LED1,2,4��Ӧ��GPF4/5/6����������Ϊ���

	while(1){
		wait(30000);
		GPBDAT = (~(i<<5));	 	// ����i��ֵ������LED1,2,4
		if(++i == 8)
			i = 0;
	}

	return 0;
}
