


#include "dht11.h"
#include "mysystick.h" 

/*
	DHT11 �����ڵ�Ƭ����GPA3 ������
	��������������
	
*/


void DHT11_init(void)
{	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.ʹ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

		//����������Ϊ�ߵ�ƽ
	GPIO_SetBits(GPIOA, GPIO_Pin_3);
	
	//2.��ʼ��Ϊ���ģʽ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;   //��©���ģʽ
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	//��·�������������裬�ҾͲ�������
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}





//��DHT11��ȡ����λ��ÿ�ε��ö�ȡ8��λ��������
static uint8_t DHT11_read_bitdat(void)
{
	uint8_t i;
	uint8_t dat;
	
	
	for(i=0;i<8;i++)
	{	
		dat <<= 1;   //����һλ
		
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)) ; //�ȴ����ű�Ϊ�͵�ƽ
		
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0);  // �ȴ����ű�Ϊ�ߵ�ƽ
		
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))   //�����ߵ�ƽ��ʱ��
		{
			delay_us(30);
		}
		
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))  //30us֮���Ǹߵ�ƽ��DHT11���ݸ��ҵ����ݾ�������λ1
			dat |= 1;   //�������޸����λ
				
	}
	
	return dat;
}






/*
	ͨ���������¶Ⱥ�ʪ�ȷ���
	����ֵ��
           0����ʾ�ɹ���ȡ���¶Ⱥ�ʪ��
		   ����ֵ����ʾ��ȡʧ��
*/
uint8_t DHT11_read(uint8_t * humi,uint8_t *temp)
{
	uint8_t retry = 0;
	uint8_t i;
	uint8_t buf[5];
	
	
	//1.�ɵ�Ƭ������ʼ�ź�
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	delay_ms(20);   //����20ms
	//תΪ�ߵ�ƽ
	GPIO_SetBits(GPIOA, GPIO_Pin_3);
	delay_us(30);   //����30us
	
		
	//�ȵ�DHT11����Ӧ������
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))  //�ȴ����ű�Ϊ�͵�ƽ
	{
		delay_us(2);
		retry++;		
		if(retry > 10)
		{	
			return 1;      //����
		}
	}
	retry = 0;
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)  //�ȴ������ɵ͵�ƽ��Ϊ�ߵ�ƽ
	{
		delay_us(10);
		retry++;		
		if(retry > 10)
		{	
			return 1;      //����
		}
	}

	//��ȡ����
	for(i=0;i<5;i++)
	{
		buf[i] = DHT11_read_bitdat();
	}
	
	
	if(buf[0]+buf[1]+buf[2]+buf[3] == buf[4]) //����ɹ�
	{
		*humi = buf[0];
		*temp = buf[2];
		
		return 0;   //�ɹ�
	}
	
	return 1;   //����
}




