#include "w25q64.h"

//оƬ��Ƭѡ��
#define W25Q64_CS PBout(12)

/*
��������: W25Q64��ʼ��
Ƭѡ����: CS---PB12
*/
void W25Q64_Init(void)
{
		RCC->APB2ENR|=1<<3; //PB
		GPIOB->CRH&=0xFFF0FFFF;
		GPIOB->CRH|=0x00030000;
		W25Q64_CS=1; //Ƭѡ����
		SPI2_Init(); //SPI��ʼ��
}

/*
��������: ���豸ID
*/
u16 W25Q64_ReadDeviceID(void)
{
		u16 device_id=0;
		W25Q64_CS=0; //ѡ��оƬ
		SPI2_ReadWriteOneByte(0x90); //��оƬID��ָ��
		SPI2_ReadWriteOneByte(0);    //���͵�ַ16->23
		SPI2_ReadWriteOneByte(0);    //���͵�ַ8->15
		SPI2_ReadWriteOneByte(0);    //���͵�ַ0->7
		
		device_id|=SPI2_ReadWriteOneByte(0xFF)<<8;//��ȡ���� ������ID
		device_id|=SPI2_ReadWriteOneByte(0xFF);//��ȡ���� �豸ID
		return device_id; //0xEF16
}


/*
��������: дʹ��ָ��
*/
void W25Q64_WriteEnabled(void)
{
		W25Q64_CS=0; //ѡ��оƬ
		SPI2_ReadWriteOneByte(0x06); //дʹ��ָ��
		W25Q64_CS=1; //ȡ��ѡ��оƬ
}

/*
��������: �ȴ�æ״̬����
*/
void W25Q64_WaitBusyState(void)
{
		u8 state=1;
		while(state&0x01)
		{
				W25Q64_CS=0; //ѡ��оƬ
				SPI2_ReadWriteOneByte(0x05); //��״ָ̬��
				state=SPI2_ReadWriteOneByte(0xFF); //��ȡ״̬
				W25Q64_CS=1; //ȡ��ѡ��оƬ
		}
}

/*
��������: ҳ���
����������
u32 addr  д�����ʼλ��
u8 *buff  д��Ļ���������
u32 len   д��ĳ���(ÿһҳ���ֻ��д256�ֽ�)
*/
void W25Q64_PageWrite(u32 addr,u32 len,u8 *buff)
{
		u16 i;
		W25Q64_WriteEnabled(); //дʹ��
		W25Q64_CS=0; //ѡ��оƬ
		SPI2_ReadWriteOneByte(0x02); //ҳ���ָ��
		SPI2_ReadWriteOneByte(addr>>16); //���͵�ַ
		SPI2_ReadWriteOneByte(addr>>8);
		SPI2_ReadWriteOneByte(addr>>0);
		
		for(i=0;i<len;i++)
		{
				SPI2_ReadWriteOneByte(buff[i]); //д������
		}
		W25Q64_CS=1; //ȡ��ѡ��оƬ
		W25Q64_WaitBusyState(); //�ȴ�д�����
}


/*
��������: ��������ָ��
һ��������С: 4096�ֽ�
0~4095 ��ʾ��һ������ .........
*/
void W25Q64_SectorErase(u32 addr)
{
		W25Q64_WriteEnabled(); //дʹ��
		W25Q64_CS=0; //ѡ��оƬ
		SPI2_ReadWriteOneByte(0x20); 			//��������ָ��
		SPI2_ReadWriteOneByte(addr>>16);  //���͵�ַ
		SPI2_ReadWriteOneByte(addr>>8);
		SPI2_ReadWriteOneByte(addr>>0);
		W25Q64_CS=1; //ȡ��ѡ��оƬ
		W25Q64_WaitBusyState(); //�ȴ�д�����
}

/*
��������: ��ָ��λ�ö�ȡָ������������
����������
u32 addr  ��ȡ����ʼλ��
u8 *buff  ��Ŷ�ȡ������
u32 len   ��ȡ�ĳ���
*/
void W25Q64_ReadData(u32 addr,u32 len,u8 *buff)
{
		u32 i=0;
		W25Q64_CS=0; //ѡ��оƬ
		SPI2_ReadWriteOneByte(0x03); 			//������ָ��
		SPI2_ReadWriteOneByte(addr>>16);  //���͵�ַ
		SPI2_ReadWriteOneByte(addr>>8);
		SPI2_ReadWriteOneByte(addr>>0);
		for(i=0;i<len;i++)
		{
				buff[i]=SPI2_ReadWriteOneByte(0xFF);
		}
		W25Q64_CS=1; //ȡ��ѡ��оƬ
}


/*
��������: д���ݣ���ָ��λ��д��ָ�����ȵ����ݡ�(�����ǲ����������Ǹ���)
��������: 
			u32 addr  д�����ʼλ��
			u8 *buff  д��Ļ���������
			u32 len   д��ĳ���
*/
void W25Q64_WriteDataNoCheck(u32 addr,u32 len,u8 *buff)
{
		u16 page_byte=256-addr%256; //�õ���ǰҳʣ����ֽ�����
		if(page_byte>len) //�жϵ�ǰҳʣ����ֽڿռ��Ƿ�д
		{
				page_byte=len; //��ʾһ���Կ���д��
		}
		while(1)
		{
				W25Q64_PageWrite(addr,page_byte,buff);//дһҳ
				if(page_byte==len)break; //д����
				buff+=page_byte; //ָ��ƫ��
				addr+=page_byte;//��ַƫ��
				len-=page_byte;//�õ�ʣ��û��д��ĳ���
				if(len>256)page_byte=256;
				else page_byte=len; //һ�ο���д��
		}
}


/*
��������: д���ݣ���ָ��λ��д��ָ�����ȵ����ݡ�(�����Ǹ���)
��������: 
			u32 addr  д�����ʼλ��
			u8 *buff  д��Ļ���������
			u32 len   д��ĳ���
*/
u8 W25Q64_CHECK_BUFF[4096]; //һ�������Ŀռ�
void W25Q64_WriteData(u32 addr,u32 len,u8 *buff)
{
		u32 i=0;
		u16 p_byte=4096-addr%4096; //�õ���ǰ����ʣ����ֽ�����
		if(p_byte>len) //�жϵ�ǰ����ʣ����ֽڿռ��Ƿ�д
		{
				p_byte=len; //��ʾһ���Կ���д��
		}
		while(1)
		{
				//��ȡ��Ҫд��ռ������
				W25Q64_ReadData(addr,p_byte,W25Q64_CHECK_BUFF);
				//�����ݽ����ж�
				for(i=0;i<p_byte;i++)
				{
						if(W25Q64_CHECK_BUFF[i]!=0xFF)break;
				}
				if(i!=p_byte) //��Ҫ����
				{
						W25Q64_SectorErase(addr);  //���в���
				}
				//д������
				W25Q64_WriteDataNoCheck(addr,p_byte,buff);
				if(p_byte==len)break; //д����
				
				addr+=p_byte; //��ַ���ƫ��
				buff+=p_byte; //����ָ��ƫ��
				len-=p_byte;  //�����Լ�
				
				 //�ж�ʣ������һ�����Ƿ����д��
				if(len>4096)p_byte=4096;
				else p_byte=len;
		}
}


