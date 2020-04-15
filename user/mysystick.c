

#include "mysystick.h"


/*
	systick��ʱ����ʼ��
	�������ڶ�ʱ
*/
void my_systick_init(void)
{
	SysTick->LOAD = 1000;   //����ֵ
	SysTick->VAL = 0;       //��ǰֵ
	SysTick->CTRL = 0;     //ʹ���ⲿϵͳʱ�ӣ���ʹ�ܶ�ʱ�����������ж�
}


/*
	us ����ʱʱ��798915us 
	//ȡһ���������ü���

	us����ʱ���ܳ���70,0000us

*/
void delay_us(uint32_t dus)
{
	if(dus == 0)
	{
		return;
	}	
	else if(dus > 700000)
	{
		return;
	}
		
	SysTick->LOAD = dus * 21 -1;
	SysTick->VAL = 0; 
	SysTick->CTRL |= 1;    //������ʱ��
	
	while((SysTick->CTRL & 1<<16) == 0);  //���Ŷ�ʱ��ʱ�䵽
	
	SysTick->CTRL = 0;    //�رն�ʱ��
	
}


/*
	ע�ⳬ��700ms�Ĵ������и�������

	�ܹ���ʱ������ms 65536 ms

*/
void delay_ms(uint16_t dms)
{
	while(dms >= 700)
	{
		delay_us(700000);
		dms -= 700;
	}
		
	if(dms < 700)
	{
		delay_us(dms*1000);
	}
}



/*
	SysTick_Config(uint32_t ticks)



	�����ʱ���������Ƕ���ʱ���ж�һ�� 1ms

	ÿ��1ms����������ͻᱻ����һ��

*/
void SysTick_Handler(void)
{

}


