

#include "w25q64.h"

/*
	PB2 ��Ƭѡ����

*/

void w25q64_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//0.ʱ�ӳ�ʼ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//1.spi�ӿڳ�ʼ��
	spi1_init();
	
	//2.Ƭѡ		
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;   //����Ƶ�ʱȽϸߵ�ʱ������Ϊppģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
	
	//2.0 ����������loraģ���Ƭѡ���ţ���ʼ��Ϊ���ģʽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOE, &GPIO_InitStruct);	
	
	//2.1 ��Ƭѡ���ߵ�ƽ
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	
	//2.2 ��Ƭѡlora���ߵ�ƽ��Ч
	GPIO_SetBits(GPIOE, GPIO_Pin_1);
	
}




uint16_t w25q64_readID(void)
{
	uint16_t id = 0;
	
	//1.Ƭѡ,�͵�ƽ��Ч
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	
	//2.��ID
	spi1_write_read(CMD_W25Q_READ_ID);   //д
	spi1_write_read(0x00);
	spi1_write_read(0x00);
	spi1_write_read(0x00);
	
	id = spi1_write_read(0x00);    //����ֵ
	id <<= 8;
	id |= spi1_write_read(0x00);

	//1.ȡ��Ƭѡ,
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	
	return id;
}

//����ǲ���æ
void w25q64_check_busy(void)
{
	uint8_t stat;

	do
	{
		//1.Ƭѡ,�͵�ƽ��Ч
		GPIO_ResetBits(GPIOB, GPIO_Pin_2);
		
		//2.��״̬�Ĵ���1
		spi1_write_read(CMD_W25Q_READ_SR1);
		
		stat = spi1_write_read(0);

		//3.ȡ��Ƭѡ		
		GPIO_SetBits(GPIOB, GPIO_Pin_2);
	}
	while(stat & 1);   //stat�����λΪ1����ʾоƬ����æ
	
}

static void w25q64_write_enable(void)
{	
	//1.Ƭѡ,�͵�ƽ��Ч
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	
	//2.����дʹ��
	spi1_write_read(CMD_W25Q_WRITE_EN);
	
	//3.ȡ��Ƭѡ		
	GPIO_SetBits(GPIOB, GPIO_Pin_2);

	//4. �ȴ�����
	w25q64_check_busy();
	
}




void w25q64_erase_chip(void)
{
	//0.дʹ��
	w25q64_write_enable();
		
	//1.Ƭѡ,�͵�ƽ��Ч
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	
	//2.���Ͳ���ָ���Ƭ������
	spi1_write_read(CMD_W25Q_ERASE_CHIP);
	
	//3.ȡ��Ƭѡ		
	GPIO_SetBits(GPIOB, GPIO_Pin_2);

	//4. �ȴ�����
	w25q64_check_busy();	
}

/*
	������
		addr ��ʾ��ʼ��ȡ�ĵ�ַ
		buf ָ��������Ҫ�洢�Ŀռ���׵�ַ
		len ��ʾ��Ҫ��ȡ���ݵĳ���

	����ֵ�Ƕ����˶��ٸ�����
*/
int32_t w25q64_read_data(uint32_t addr, uint8_t buf[],uint32_t len)
{
	uint32_t i;
	
	//1.Ƭѡ,�͵�ƽ��Ч
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	
	//2.���Ͳ���ָ���Ƭ������
	spi1_write_read(CMD_W25Q_READ_DATA);
	spi1_write_read((addr>>16) & 0xff);
	spi1_write_read((addr>>8) & 0xff);
	spi1_write_read(addr & 0xff);
	
	for(i=0;i<len;i++)
	{
		buf[i] = spi1_write_read(0);
	}
	
	//3.ȡ��Ƭѡ		
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	
	
	return i;
}


/*
	ҳ��д�����ݣ�û�п��ǳ���ҳ����������ֻ��д��256���ֽ�
	������
		addr ��ʾ��ʼд��ĵ�ַ
		buf ָ�����ݵ��׵�ַ
		len ��ʾ��Ҫд�����ݵĳ���

	����ֵ��д�˶��ٸ�����
*/
uint8_t w25q64_write_data(uint32_t addr, uint8_t buf[],uint8_t len)
{
	uint8_t i;
		
	//0.дʹ��
	w25q64_write_enable();
	
	//1.Ƭѡ,�͵�ƽ��Ч
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	
	//2.д������
	spi1_write_read(CMD_W25Q_WRITE_DATA);
	spi1_write_read((addr>>16) & 0xff);
	spi1_write_read((addr>>8) & 0xff);
	spi1_write_read(addr & 0xff);
	
	
	for(i=0;i<len;i++)
	{
		spi1_write_read(buf[i]);
	}
		
	//3.ȡ��Ƭѡ		
	GPIO_SetBits(GPIOB, GPIO_Pin_2);

	//4. �ȴ�����
	w25q64_check_busy();
	
	return i;
}
