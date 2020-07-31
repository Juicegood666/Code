#include "spi.h"

#if SPI2_MODE

//ģ��ʱ�����Ŷ���
#define SPI2_SCK PBout(13)
#define SPI2_MOSI PBout(15)
#define SPI2_MISO PBin(14)

/*
��������:(���ģ��ʱ��)��ʼ��SPI2
Ӳ������:
				MISO:PB14---����
				MOSI:PB15---���
				SCK :PB13
*/
void SPI2_Init(void)
{
	 RCC->APB2ENR|=1<<3; //PB
	 GPIOB->CRH&=0x000FFFFF;
	 GPIOB->CRH|=0x38300000;
	 SPI2_SCK=1;
}

/*
��������: SPI�ײ��дһ���ֽ�
����:
1. ʱ���߿��е�ƽΪ�ߵ�ƽ��
2. ������ʱ����Ϊ�͵�ƽ��ʱ�򷢳�
3. ���������ڸߵ�ƽ
4. �����ȷ���λ
*/
u8 SPI2_ReadWriteOneByte(u8 tx_data)
{
		u8 rx_data=0;
	  u8 i;
		for(i=0;i<8;i++)
		{
				SPI2_SCK=0;
				if(tx_data&0x80)SPI2_MOSI=1;
			  else SPI2_MOSI=0;
				SPI2_SCK=1;
			  tx_data<<=1; //����������һλ
			
				rx_data<<=1;
				if(SPI2_MISO)rx_data|=0x01;
		}
		return rx_data;
}

#else   //������Ӳ��ʱ��

/*
��������:(Ӳ��ʱ��)��ʼ��SPI2
Ӳ������:
				MISO:PB14---����
				MOSI:PB15---���
				SCK :PB13---ʱ����
*/
void SPI2_Init(void)
{
	 RCC->APB2ENR|=1<<3; //PB
	 GPIOB->CRH&=0x000FFFFF;
	 GPIOB->CRH|=0xB8B00000;
	
	 RCC->APB1ENR|=1<<14; //SPI2
	 RCC->APB1RSTR|=1<<14;
	 RCC->APB1RSTR&=~(1<<14);
	
	 //����SPI���ļĴ���
	 SPI2->CR1&=~(1<<15); //0��ѡ��˫��˫��ģʽ
	 SPI2->CR1&=~(1<<11); //0��ʹ��8λ����֡��ʽ���з���/����
	 SPI2->CR1|=1<<9;     //1������������豸����
	 SPI2->CR1|=1<<8;     //��������NSS�ϵĵ�ƽ
	 SPI2->CR1&=~(1<<7);  //0���ȷ���MSB
	 SPI2->CR1&=~(0x7<<3); //ѡ������ clk/2
	 SPI2->CR1|=1<<2; 		//1������Ϊ���豸
	 SPI2->CR1|=1<<1; 		//1�� ����״̬ʱ�� SCK���ָߵ�ƽ
	 SPI2->CR1|=1<<0; 		//1�� ���ݲ����ӵڶ���ʱ�ӱ��ؿ�ʼ
	 SPI2->CR1|=1<<6; 		//1������SPI�豸
}
/*
��������: SPI�ײ��дһ���ֽ�
*/
u8 SPI2_ReadWriteOneByte(u8 tx_data)
{
	u16 cnt=0;
	while(!(SPI2->SR&1<<1)) //�ȴ����ͻ�����Ϊ��
	{
			cnt++;
			if(cnt>=65530)return 0xFF;
	}
	
	cnt=0;
	SPI2->DR=tx_data; //��������
	while(!(SPI2->SR&1<<0)) //�ȴ����ݽ���
	{
			cnt++;
			if(cnt>=65530)return 0xFF;
	}
	return SPI2->DR; //���ض���������
}
#endif
