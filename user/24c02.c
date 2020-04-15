


#include "24c02.h"
#include <stdio.h>

/*
	��Ҫ�Ƕ�д24c02
	
	

*/

void AT24c02_init(void)
{
	i2c1_init();
}




/*
	�ӵ�ǰ��ַ��һ���ֽ�
	���ṩ����Ľӿ�
*/	
static uint8_t AT24c02_read_byte_cur(uint8_t *dat,uint8_t len)
{
	uint8_t i;
	
	//1.������ʼ�ź�
	if(i2c1_start() != 0)
	{
		printf("i2c1_start() failed\n");
		return 254;
	}
	
	//2.�����豸��ַ
	if(i2c1_put_devaddr(SLAVER_ADDR | 1)!= 0)   //���λ��1����ʾ������
	{
		printf("i2c1_put_devaddr failed\n");
		return 253;
	}

		
	for(i=0;i<len;i++)
	{			
		//4.ֻ�����һ�����ݷ��ͷ�Ӧ��
		if(i == len -1)
		{
			i2c1_send_ack(NO_ACK);
		}
		else
		{
			i2c1_send_ack(ACK);
		}
		
		//3.���һ���ֽڵ�����
		dat[i] = i2c1_get_byte_data();
	}
	
	//.����stopʱ��
	i2c1_stop();     //����ֹͣ�źţ����߾Ϳ�����

	return 0;
}





/*
	24c02 ��Ƭ��д�������
	������
		word_addr �ռ��ַ������Ҫ���ֽ�datд��24c02����һ���洢��Ԫ��ȥ
		dat       ʵ�����ݵ��׵�ַ��
		len       ʵ����Ҫд�����ݵĸ���
	����ֵ��
		0  ��     �ɹ�
		��0��     ʧ��
*/
uint8_t AT24c02_write_byte(uint8_t word_addr,uint8_t *dat,uint8_t len)
{
	uint8_t i;


	//1.������ʼ�ź�
	if(i2c1_start() != 0)
	{
		printf("write i2c1_start()  failed\n");
		return 254;
	}
	
	//2.�����豸��ַ
	if(i2c1_put_devaddr(SLAVER_ADDR )!= 0)   //���λ��0����ʾд����
	{
		printf("write i2c1_put_devaddr failed\n");
		return 253;
	}
	
	//4.���Ϳռ��ַ
	i2c1_put_byte_data(word_addr);  
	
	//3.��24c02��ack
	if(i2c1_get_ack())  //���û��Ӧ��ֱ���˳�
	{
		i2c1_stop();     //����ֹͣ�źţ����߾Ϳ�����
		return 2;
	}
	
	//len����0��ʱ������Ϊ���������һ����Ч����
	if(len == 0)
	{
		return 5;
	}
	
	for(i=0;i<len;i++)
	{		
		//4.��������
		i2c1_put_byte_data(dat[i]); 
		
		//3.��24c02��ack
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



/*
	���������
	���� �� word_addr ָ����Ҫ��ȡ��λ��
			dat ��ʾ�洢���ݻ�����׵�ַ
			len ��ʾ��Ҫ��ȡ���ݵĸ���
	����ֵ�� 0 ��ʾ�ɹ�
*/
uint8_t AT24c02_read_byte(uint8_t word_addr,uint8_t *dat,uint8_t len)
{
	AT24c02_write_byte(word_addr,(void*)0,0);
		
	AT24c02_read_byte_cur(dat,len);
	
	return 0;
}





/*
	24c02 ��ҳ���

	��ʼ��ַ�����16������������ô������д����Ч��16���ֽڣ�������ֽڣ��ͻỷ��
	
*/
#define PAGE_SIZE 16    //ҳ�ֽ���
#define PAGE_SHIFT 4    //



uint8_t AT24c02_write_data(uint8_t word_addr,uint8_t *dat,uint8_t len)
{
	uint8_t n;  //д��Ĵ���
	uint8_t m;  //��һ��д����ֽ���
	
	if((word_addr & (PAGE_SIZE-1)) != 0)   //��ʾword_addr���ܹ���16����
	{
		m = PAGE_SIZE - (word_addr& (PAGE_SIZE-1));   //��һ��д����ֽ���
		n = (len-m+PAGE_SIZE-1) >> PAGE_SHIFT;
	//	n += 1;
		AT24c02_write_byte(word_addr,dat,m);
		word_addr += m;
		dat += m;
		len -= m;
		
	}
	else //��ʾword_addr�ܹ���16����
	{
		n = (len+PAGE_SIZE-1)>>PAGE_SHIFT;
	}
	
	while(n>1)
	{
//		printf("word_addr = %d len = %d\n",word_addr,len);
		AT24c02_write_byte(word_addr,dat,PAGE_SIZE);
		word_addr += PAGE_SIZE;
		dat += PAGE_SIZE;
		len -= PAGE_SIZE;
		n--;
	}
	
	AT24c02_write_byte(word_addr,dat,len);
	
	return 0;
}


