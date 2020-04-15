

#include "ADC1_0_gm.h"


/*
	���������ѹ��ADCת��

	ADC1 ͨ��0
	
	���ţ� GPA 0������
*/


void adc1_gm_init(void)
{
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	
	//2.IO�˿ڵ�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;     //����Ϊģ�⹦��
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;   //��©���ģʽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;  //���������費�ܿ���
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;  //	
	GPIO_Init(GPIOA, &GPIO_InitStruct);  //����������ýṹ����������õ��Ĵ�����
		
	//3.ADC���Ƶ�����
	//ADC����������
	ADC_CommonStructInit(&ADC_CommonInitStruct);
	ADC_CommonInit(&ADC_CommonInitStruct);

	//ADC1����س�ʼ��	
	ADC_StructInit(&ADC_InitStruct);   //��������Ĭ������
	ADC_Init(ADC1, &ADC_InitStruct);

	//����ͨ������,��һ�βɼ�ͨ��0
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1, ADC_SampleTime_480Cycles);
	
	//����ADC
	ADC_Cmd(ADC1, ENABLE); 
	
}



//���ADC������
uint16_t get_gm_val(void)
{
	//1.��Ϊ�������ADCת��
	ADC_SoftwareStartConv(ADC1);
	
	//2.�ȴ�ת������ EOC
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	
	//3.��ת���Ľ��������
	return ADC_GetConversionValue(ADC1);
	
}



