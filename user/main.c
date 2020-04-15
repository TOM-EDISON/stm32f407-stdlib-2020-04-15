

#include <stm32f4xx.h>
#include "led.h"
#include "key.h"
#include "uart1.h"
#include "mysystick.h"
#include "dht11.h"
#include "24c02.h"
#include "ADC1_0_gm.h"
#include "w25q64.h"
#include "oled.h"

#include <stdio.h>
/*
	ʹ�ñ�׼��ķ���������led1

*/


#if 0
void my_systeminit(void)
{
	//1.���ⲿ�����·
	RCC->CR |= 1<<16;     
	//2.�����ⲿ�����·���ȶ�
	while(!(RCC->CR & (1<<17)));
	
	//3.PLL��֮ǰ����������һ��PLL�Ĳ���
	RCC->PLLCFGR &= 0xff000000;    //��24λȫ������
	
	//n= 336,p = 2,m = 8,PLLʱ��Դѡ��HSE
	RCC->PLLCFGR |= 1<<22 | 336<<6 | 8;
	
	//4.����PLL
	RCC->CR |= 1<<24; 
	while(!(RCC->CR & (1<<25)));  //�ȴ�PLLʱ���ȶ�
	
	//5.APB2 ���ܳ���84M APB1���ܳ���42M
	RCC->CFGR |= 4<<13 | 5<<10;
		
	//6.****����flash�Ķ�ȡ�ٶȣ�cpu��Ƶ�ʼ��٣�����flash���ٶ�û��cpu��ô�죬��ʱ��ϵͳϵͳ��������
	FLASH->ACR = 7<<8 | 5;
	
	
	//7.��ϵͳʱ���л���PLLʱ��
	RCC->CFGR |= 2;
	
	while((RCC->CFGR & 0xc) != 0x8);
	
}
#endif








void delay(unsigned int n)
{
	uint32_t i;
	
	for(i=0;i<n;i++)
		;
}


void serial_cmd_handle(void)
{
	uint8_t i;
	uint8_t check_sum = 0; //У���
	
	//У��ʹ���
	for(i=0;i<recv_buf[2]+2;i++)
	{
		check_sum += recv_buf[i];
	}
	
	printf("serial_cmd_handle\n");
	if(check_sum == recv_buf[recv_buf[2]+2] )  //����ͳɹ�
	{
		printf("check ok\n");

		if((recv_buf[3] == 1) || (recv_buf[3] == 2) || (recv_buf[3] == 3) || (recv_buf[3] == 0xf))
		{
			if(recv_buf[3] == 0xf)
			{
				recv_buf[3] = 0;
			}
			led_set_status((ledn_t)recv_buf[3],(led_sta_t)recv_buf[4]);
		
			printf("len handle\n");
		}
	}
}






int main(void)
{		
	uint8_t key_val;
//	uint8_t temp_val;   //�¶�
//	uint8_t humi_val;   //ʪ��
	
	uint8_t buf[50];
	uint8_t i;

	
	//0.�����ж����ȼ����飬һ������֮�󣬾Ͳ�Ҫ�޸���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	
	my_systick_int_init(168);   //168��ʾ���ڵ�ʱ��Ƶ��

	
	//1.led��ʼ����Ϩ��
	led_init();
	
	//2.��ʼ������
	key_init();
	
	//flash ��ʼ��
	w25q64_init();
	
	//3.����1�ĳ�ʼ��,����ָ��������,ʹ���жϽ���
  
	uart1_int_init(115200);


	//��������ADC��ʼ��
	adc1_gm_init();

	//��ʪ�ȴ�������ʼ��
	DHT11_init();

	//eeprom��ʼ��
//	AT24c02_init();
	
	
	printf("w25q64 id = 0x%x\n",w25q64_readID());

	

//	for(i=0;i<50;i++)
//	{
//		buf[i] = i*2+20;
//		
//	}
//	w25q64_write_data(100, buf,50);


	w25q64_read_data(50, buf,50);
	for(i=0;i<50;i++)
	{
		printf("%x ",buf[i]);
		if(i%10 == 9)
		{
			printf("\n");
		}
	}
	printf("\n");
	
	
	
	w25q64_read_data(100, buf,50);
	for(i=0;i<50;i++)
	{
		printf("%d ",buf[i]);
		if(i%10 == 9)
		{
			printf("\n");
		}
	}
	printf("\n");
	

	printf("hello arm %d-%d-%d\n",2020,4,9);
	oled_init();

//	OLED_ShowChar(0,0,'c',16);
	OLED_ShowString(0,0,"2020-04-10",16);


	//���°�����led2�������ɿ�led2Ϩ��	
	while(1)
	{
		//�ǲ����յ���������
		if(recv_flag)
		{
			recv_flag = 0;  //���ڸոս��ܵ����ݣ��Ѿ������˴���
	
			serial_cmd_handle();		
		}
		
		if(key_flag)   //��⵽������
		{
			key_val = key_flag;
			key_flag = 0;
			printf("key is %d\n",key_val);
		}
		
//		if(DHT11_read(&humi_val,&temp_val) == 0)
//		{
//			printf("humi = %d temp = %d\n",humi_val,temp_val);
//		}
//		else
//		{
//			printf("humi temp read error\n");
//		}
		
//		printf("guang min val = %d\n",get_gm_val());

		
		
		led_set_status(LED2,LEDON);	
		delay_ms(700);	
		led_set_status(LED2,LEDOFF);
		delay_ms(700);
	
	}
	
}

