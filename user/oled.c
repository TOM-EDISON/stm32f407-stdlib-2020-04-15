

#include "oled.h"
#include <stdio.h>

//#include "oledfont.h"


/*
	0.96 oled ����  iic�ķ�ʽ
	ssd1306 ������

*/

static uint8_t OLED_GRAM[8][128];  //�Դ��ڵ�Ƭ��������


//����һ���ֽ�
//��SSD1306д��һ���ֽڡ�
//mode:����/�����־ 0,��ʾ����;1,��ʾ����;
uint8_t OLED_WR_Data(u8 dat[],uint16_t len,u8 mode)
{
	uint16_t i;

	//1.������ʼ�ź�
	if(i2c1_start() != 0)
	{
		printf("write i2c1_start()  failed\n");
		return 254;
	}

	//2.�����豸��ַ
	if(i2c1_put_devaddr(OLED_DEV_ADDR)!= 0)   //���λ��0����ʾд����
	{
		printf("write i2c1_put_devaddr failed\n");
		return 253;
	}

	//4.����ģʽ��mode���������Ҳ����������
	i2c1_put_byte_data(mode);  
	
	//3.��ack
	if(i2c1_get_ack())  //���û��Ӧ��ֱ���˳�
	{
		printf("mode error \n");
		i2c1_stop();     //����ֹͣ�źţ����߾Ϳ�����
		return 2;
	}

	for(i=0;i<len;i++)
	{			
		//4.��������
		i2c1_put_byte_data(dat[i]); 

		//3.��ack
		if(i2c1_get_ack())  //���û��Ӧ��ֱ���˳�
		{
			i2c1_stop();     //����ֹͣ�źţ����߾Ϳ�����
			return 3;
		}
	}

	//5.�������������ߵ�ռ��
	i2c1_stop();
	
	delay_ms(100);	

	return 0;
}




//�����Դ浽OLED	
void OLED_Refresh(void)
{
	OLED_WR_Data((void*)OLED_GRAM,sizeof OLED_GRAM,OLED_DATA);

}


//��������
void OLED_Clear(void)
{
	u8 i,n;

	for(i=0;i<8;i++)
	{
		for(n=0;n<128;n++)
		{
			 OLED_GRAM[i][n]=0x3;//�����������
		}
	}
	OLED_Refresh();//������ʾ
}





void oled_init(void)
{
	uint8_t cmd_arr[] = {0xAE,0x00,0x10,0x40,0x81,0xCF,0xA1,0xC8,0xA6,0xA8,
						0x3f,0xD3,0x00,0xd5,0x80,0xD9,0xF1,0xDA,0x12,0xDB,
						0x40,0x20,0x00,0x8D,0x14,0xA4,0xA6,0xAF};
	
	//i2c�ӿڵĳ�ʼ��
	i2c1_init();
		
	OLED_WR_Data(cmd_arr,sizeof cmd_arr,OLED_CMD);

	OLED_Clear();
}






//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//size:ѡ������ 12/16/24
//ȡģ��ʽ ����ʽ
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1)
{
	u8 i,j,chr1;
	u8 y0=y/8;

	chr1=chr-' ';  //����ƫ�ƺ��ֵ

	for(j=0;j<2;j++)
	{
		for(i=0;i<8;i++)
		{
			OLED_GRAM[y0+j][x+i] = asc2_1608[chr1][i+j*8];
		}
	}
	
}


//��ʾ�ַ���
//x,y:�������  
//size1:�����С 
//*chr:�ַ�����ʼ��ַ 
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1)
{
	while((*chr>=' ')&&(*chr<='~'))//�ж��ǲ��ǷǷ��ַ�!
	{
		OLED_ShowChar(x,y,*chr,size1);

		x+=size1/2;
		if(x>128-size1/2)  //����
		{
			x=0;
			y+=8;
		}
		chr++;
	}
	OLED_Refresh();//������ʾ
}
