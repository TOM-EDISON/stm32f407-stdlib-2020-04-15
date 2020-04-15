
#include "spi1.h"


/*
	Ӳ��SPI ��ʼ��
	
	���Ƭѡ
	
	spi��3��Ӳ���ӿ���GPIOB 3��4��5������
		
*/

void spi1_init(void)
{
	static uint8_t ready = 0;	   //��0��ʾ�Ѿ���ʼ�����
	SPI_InitTypeDef SPI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	if(ready == 0)
	{
		//1.ʱ�ӳ�ʼ��
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
			
		//2.io���ù�������
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;  //����ģʽ��ͨ�ŵ�Ƶ�ʴﵽ����
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   //ע��һ�����Ƶ��	
		GPIO_Init(GPIOB, &GPIO_InitStruct);	
		
		//2.1����ĸ��ù���
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
		
		//SPI��������ʼ��
		SPI_StructInit(&SPI_InitStruct);
		SPI_InitStruct.SPI_Mode = SPI_Mode_Master;  //����ģʽ
		SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;     //��ʹ��Ӳ��Ƭѡ�ź�
		SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
		SPI_Init(SPI1, &SPI_InitStruct);
		
		SPI_Cmd(SPI1, ENABLE);
		
		ready = 1;   //�Ѿ���ɳ�ʼ����������һ���ٵ�������������Ͳ��ٳ�ʼ����
	}
}


uint8_t spi1_write_read(uint8_t dat)
{
	//�ǲ��ǿ��У���������ݾ͵ȴ�
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, dat);
	
	//�ǲ��ǽ������ˣ�û�о͵ȴ�
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);	
}


