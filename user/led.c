
#include "led.h"
//#include <stm32f4xx.h>






void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;  //����ṹ�壬����Ҫ���õ�������д���ṹ����

	//1.ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//2.����Ϊ�����ʽ������ߵ�ƽ��Ϩ��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;  //	
	GPIO_Init(GPIOE, &GPIO_InitStruct);  //����������ýṹ����������õ��Ĵ�����
	
	//3.����ߵ�ƽ
	GPIO_SetBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);	
}




void led_set_status(ledn_t ledn,led_sta_t status)
{
	const uint16_t pin[] = {GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10,GPIO_Pin_8 ,GPIO_Pin_9,GPIO_Pin_10};
	
	
	if(ledn > LED3)
	{
		return ;
	}
	
	
	if(status == LEDOFF)
	{			
		GPIO_SetBits(GPIOE, pin[ledn]);   //�����������Ϊ�ߵ�ƽ
	}
	else //if(status == LEDON)
	{
		GPIO_ResetBits(GPIOE, pin[ledn]); //�����������Ϊ�͵�ƽ
	}
}


/*
	�ܹ���ʾ�ľ���0-7�Ķ����Ʊ�ʾ
	Ϩ���ʾ0��������ʾ1��led1�����λ
*/
void led_show_num(uint8_t num)
{
	num = ~num; //0,��ʾҪϨ�����еĵ�	
	
	GPIO_WriteBit(GPIOE, GPIO_Pin_8, (BitAction)(num&1));
	GPIO_WriteBit(GPIOE, GPIO_Pin_9, (BitAction)((num>>1) & 1));
	GPIO_WriteBit(GPIOE, GPIO_Pin_10, (BitAction)((num>>2) & 1));
}


