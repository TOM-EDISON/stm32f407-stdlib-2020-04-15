

#include "mysystick.h"
#include "i2c1.h"

#define SDA_PIN GPIO_Pin_7
#define SCL_PIN GPIO_Pin_6



/*
	i2c1 ���ģ��ʱ��ʵ��
	
	��ʼ������
	
	�������ŵ�����
	
	
	PB6  SCL OD���
	PB7  SDA �������������OD���ģʽ

*/
void i2c1_init(void)
{	
	GPIO_InitTypeDef GPIO_InitStruct;  //����ṹ�壬����Ҫ���õ�������д���ṹ����

	//1.ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		
	//2.����ߵ�ƽ
	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 );	
	
	//3.����Ϊ�����ʽ������ߵ�ƽ��Ϩ��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;   //��©���ģʽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;  //	
	GPIO_Init(GPIOB, &GPIO_InitStruct);  //����������ýṹ����������õ��Ĵ�����

}


void i2c1_start(void)
{
	//1.
	GPIO_SetBits(GPIOB, SDA_PIN | SCL_PIN );    //�������Ŷ��Ǹߵ�ƽ
	delay_us(2);
	
	//SCLΪ�ߵ�ƽ��ʱ��SDA�ɸ߱��
	GPIO_ResetBits(GPIOB,  SDA_PIN );   //SDA��͵�ƽ
	delay_us(2);
	
	//������Ҫ��Ҫ��SCL����
	GPIO_ResetBits(GPIOB,  SCL_PIN );   //SCL��͵�ƽ
}



void i2c1_stop(void)
{
	//SCLΪ�͵�ƽ
	GPIO_ResetBits(GPIOB,  SCL_PIN );   //SCL��͵�ƽ
	//SDAΪ�͵�ƽ
	GPIO_ResetBits(GPIOB,  SDA_PIN );   //SDA��͵�ƽ
	
	delay_us(2);
	
	GPIO_SetBits(GPIOB, SCL_PIN );   //SCLΪ��
	delay_us(2);
	
	//SDA�ɵͱ�Ϊ�ߵ�ƽ ����stop�ź�
	GPIO_SetBits(GPIOB, SDA_PIN );   //SDAΪ��
	delay_us(2);
	
	//stop֮��SDA��SCL��Ϊ�ߵ�ƽ
}



//i2c ����һ���ֽ�
//���������������û��Ӧ��
void i2c1_put_byte_data(uint8_t dat)
{
	uint8_t i;
		
	for(i=0;i<8;i++)
	{
		//1.SCLΪ��
		GPIO_ResetBits(GPIOB,  SCL_PIN );   //SCL��͵�ƽ
		delay_us(1);
		
		//2.��SCLΪ�͵�ƽ��ʱ�򣬿�����SDA��׼������
		if((dat >> 7-i) & 1)  //�ߵ�ƽ
		{
			GPIO_SetBits(GPIOB, SDA_PIN );   //SDAΪ��
		}
		else  //�͵�ƽ
		{
			GPIO_ResetBits(GPIOB,  SDA_PIN );   //SDA��͵�ƽ
		}
		delay_us(1);
		
		//SCLΪ�ߵ�ƽ�����ݾͷ���ȥ��
		GPIO_SetBits(GPIOB, SCL_PIN);   //SCLΪ��
		delay_us(4);
	}
	//��������˳���ʱ��SCL�ǵ͵�ƽ
	GPIO_ResetBits(GPIOB,  SCL_PIN );   //SCL��͵�ƽ
	delay_us(1);
}

//���մ��豸��Ӧ���ź�
//��Ӧ�𣬷���0
//��Ӧ�𣬷���1
uint8_t i2c1_get_ack(void)
{
	uint8_t ret = 0;
	//��ȡ�Է���Ӧ��	
//	GPIO_ResetBits(GPIOB,  SCL_PIN );   //SCL��͵�ƽ
//	delay_us(1);

	//1.����SDA	��Ҫ��ԭ����OD���ģʽ
	GPIO_SetBits(GPIOB, SDA_PIN );   //SDAΪ��
	delay_us(1);
	
	GPIO_SetBits(GPIOB, SCL_PIN);   //SCLΪ��
	delay_us(2);
	
	//��SCLΪ�ߵ�ʱ�� ����ȡSDA�����ŵ�ƽ
	if(GPIO_ReadInputDataBit(GPIOB, SDA_PIN ))
	{
		ret = 1;
	}
	//��������˳���ʱ��SCL�ǵ͵�ƽ
	GPIO_ResetBits(GPIOB,  SCL_PIN );   //SCL��͵�ƽ
	delay_us(1);
	
	return ret;
}


/*
	�� ���豸 ��ȡһ���ֽڵ�����

*/
uint8_t i2c1_get_byte_data(void)
{
	uint8_t i;
	uint8_t dat;

	//1.SCLΪ��
	GPIO_ResetBits(GPIOB,  SCL_PIN );   //SCL��͵�ƽ
	delay_us(1);
	
	//2.����SDA	
	GPIO_SetBits(GPIOB, SDA_PIN );   //SDAΪ��
	delay_us(1);
	
	
	for(i=0;i<8;i++)
	{
		dat <<= 1;
		
		//3.����SCL	
		GPIO_SetBits(GPIOB, SCL_PIN );   //SCLΪ��
		delay_us(1);
		
		//4.SCLΪ�ߵ�ʱ�򣬲�������������
		if(GPIO_ReadInputDataBit(GPIOB, SDA_PIN ))
			dat |= 1;    //�������Ǹߵ�ƽ����ô���λ��һ
		
		//5.SCLΪ��,��24c02׼����һ������
		GPIO_ResetBits(GPIOB,  SCL_PIN );   //SCL��͵�ƽ
		delay_us(4);		
	}
	
	return dat;
}



void i2c1_send_ack(uint8_t ack)
{
	//1.SCLΪ��
	GPIO_ResetBits(GPIOB,  SCL_PIN );   //SCL��͵�ƽ
	delay_us(1);
	
	if(ack == NO_ACK)  //���ͷ�Ӧ���ź�
	{
		GPIO_SetBits(GPIOB, SDA_PIN );   //SDAΪ��	
	}
	else
	{
		GPIO_ResetBits(GPIOB, SDA_PIN );   //SDAΪ��
	}
	delay_us(1);
	
	GPIO_SetBits(GPIOB,  SCL_PIN );   //SCL��ߵ�ƽ�����ǣ�Ӧ�����ڲŷ��ͳ�ȥ
	delay_us(2);
	
	GPIO_ResetBits(GPIOB,  SCL_PIN );   //SCL��͵�ƽ
	delay_us(1);
	
}




