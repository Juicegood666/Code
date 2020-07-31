#include "iic.h"

#if IIC_MODE
/*
��������: ��ʼ��IIC����
Ӳ������: 
				SCL---PB6
				SDA---PB7
*/
void IIC_Init(void)
{
	 RCC->APB2ENR|=1<<3;
	 GPIOB->CRL&=0x00FFFFFF;
	 GPIOB->CRL|=0x33000000;
	 GPIOB->ODR|=0x3<<6;
}

/*
��������: ������ʼ�ź�
��ʱ����Ϊ�ߵ�ƽ��ʱ���������ɸߵ�ƽ��Ϊ�͵�ƽ�Ĺ���
*/
void IIC_SendStart(void)
{
		IIC_OUTPUT_MODE(); //���ģʽ
		IIC_SCL=1;
		IIC_SDA_OUT=1;
		DelayUs(2);
		IIC_SDA_OUT=0;
		DelayUs(2);
		IIC_SCL=0; //���߿�ʼ��������ʼ��д����
}

/*
��������: ֹͣ�ź�
��ʱ����Ϊ�ߵ�ƽ��ʱ���������ɵ͵�ƽ��Ϊ�ߵ�ƽ�Ĺ���
*/
void IIC_SendStop(void)
{
		IIC_OUTPUT_MODE(); //���ģʽ
		IIC_SCL=0;
		IIC_SDA_OUT=0;
	  DelayUs(2);
		IIC_SCL=1;
		DelayUs(2);
		IIC_SDA_OUT=1;
}

/*
��������: ��ȡӦ���ź�
�� �� ֵ: 0��ʾ��ȡ��Ӧ�� 1��ʾû�л�ȡ��Ӧ��
*/
u8 IIC_GetAck(void)
{
		u8 cnt;
		IIC_INPUT_MODE();//����ģʽ
		IIC_SDA_OUT=1; //����
		IIC_SCL=0;
		DelayUs(2);
	  IIC_SCL=1;
		while(IIC_SDA_IN)
		{
				cnt++;
				if(cnt>250)return 1; //��ȡ����Ӧ��
		}
		IIC_SCL=0;
		return 0;
}


/*
��������: ����Ӧ���ź�
����������0��ʾӦ�� 1��ʾ��Ӧ��
*/
void IIC_SendAck(u8 ack)
{
		IIC_OUTPUT_MODE(); //���ģʽ
		IIC_SCL=0;
	  DelayUs(2);
		if(ack)IIC_SDA_OUT=1; //���ͷ�Ӧ��
		else IIC_SDA_OUT=0; //����Ӧ��
		DelayUs(2);
		IIC_SCL=1;
		DelayUs(2);
		IIC_SCL=0;
}

/*
��������: ����һ���ֽ�����
*/
void IIC_SendOneByte(u8 data)
{
		u8 i=0;
		IIC_OUTPUT_MODE(); //���ģʽ
		for(i=0;i<8;i++)
		{
				IIC_SCL=0;
				if(data&0x80)IIC_SDA_OUT=1;
				else IIC_SDA_OUT=0;
				DelayUs(2);
				IIC_SCL=1;
				DelayUs(2);
				data<<=1;
		}
		IIC_SCL=0;
}

/*
��������: ����һ���ֽ�����
*/
u8 IIC_RecvOneByte(void)
{
	u8 i=0;
	u8 data=0;
	IIC_INPUT_MODE();
	for(i=0;i<8;i++)
	{
			IIC_SCL=0;
			DelayUs(2);
			IIC_SCL=1;
			data<<=1; //��ʾĬ���յ�0
			if(IIC_SDA_IN)data|=0x01;
			DelayUs(2);
	}
	IIC_SCL=0;
	return data;
}

#else  //Ӳ��ʱ��

/*
��������: ��ʼ��IIC����
Ӳ������: 
				SCL---PB6
				SDA---PB7
*/
void IIC_Init(void)
{
	 /*1. ʱ������*/
	 RCC->APB2ENR|=1<<3; //PB

 	 /*2. GPIO��ģʽ����*/
	 GPIOB->CRL&=0x00FFFFFF;
	 GPIOB->CRL|=0xFF000000; //���ÿ�©���
	 GPIOB->ODR|=0x3<<6;
		
	 /*3. GPIO��ʱ������(˳���ܴ�)*/
	 RCC->APB1ENR|=1<<21; //I2C1ʱ��
	 RCC->APB1RSTR|=1<<21; //������λʱ��
	 RCC->APB1RSTR&=~(1<<21);//�رո�λʱ��
	
	 /*4. ����IIC�ĺ��ļĴ���*/
	 I2C1->CR2=0x24<<0;  //��������Ƶ��Ϊ36MHZ
	 I2C1->CCR|=0x2D<<0; //��������Ƶ����400KHZ
	 I2C1->CR1|=1<<0;    //����IICģ��
	
	/*
	CCR=����ʱ��Ƶ��/2/IIC���ߵ�Ƶ��
	45=36MHZ/2/400KHZ  ---0x2D
	*/
}


/*
��������: ������ʼ�ź�
��ʱ����Ϊ�ߵ�ƽ��ʱ���������ɸߵ�ƽ��Ϊ�͵�ƽ�Ĺ���
*/
void IIC_SendStart(void)
{
	I2C1->CR1|=1<<8; //������ʼ�ź�
	while(!(I2C1->SR1&1<<0)){} //�ȴ���ʼ�ź����
	I2C1->SR1=0; //���״̬λ
}

/*
��������: ֹͣ�ź�
��ʱ����Ϊ�ߵ�ƽ��ʱ���������ɵ͵�ƽ��Ϊ�ߵ�ƽ�Ĺ���
*/
void IIC_SendStop(void)
{
		I2C1->CR1|=1<<9;
}

/*
��������: ���͵�ַ����
*/
void IIC_SendAddr(u8 addr)
{
	u32 s1,s2;
	I2C1->DR=addr; //��������
	while(1)
	{
			s1=I2C1->SR1;
			s2=I2C1->SR2;
		  if(s1&1<<1) //�жϵ�ַ��û�з��ͳɹ�
			{
					break;
			}
	}
}

/*
��������: ��������
*/
void IIC_SendOneByte(u8 addr)
{
	u32 s1,s2;
	I2C1->DR=addr; //��������
	while(1)
	{
			s1=I2C1->SR1;
			s2=I2C1->SR2;
			if(s1&1<<2) //�ж�������û�з��ͳɹ�
			{
					break;
			}
	}
}

/*
��������: ����һ���ֽ�����
*/
u8 IIC_RecvOneByte(void)
{
	u8 data=0;
	I2C1->CR1|=1<<10; //ʹ��Ӧ��
	while(!(I2C1->SR1&1<<6)){} //�ȴ�����
	data=I2C1->DR;
	I2C1->CR1&=~(1<<10); //�ر�Ӧ��ʹ��
	return data;
}
#endif
