#include "at24c08.h"

#if IIC_MODE

/*
��������: дһ���ֽ�
��������:
	u8 addr ���ݵ�λ��(0~1023)
	u8 data ���ݷ�Χ(0~255)
*/
void AT24C08_WriteOneByte(u16 addr,u8 data)
{
		IIC_SendStart();//��ʼ�ź�
		IIC_SendOneByte(AT24C08_WRITE_ADDR);//�����豸��ַ
		IIC_GetAck();//��ȡӦ��
		IIC_SendOneByte(addr); //���ݴ�ŵĵ�ַ
		IIC_GetAck();//��ȡӦ��
		IIC_SendOneByte(data); //���ͽ�Ҫ��ŵ�����
		IIC_GetAck();//��ȡӦ��
		IIC_SendStop(); //ֹͣ�ź�
		DelayMs(10); //�ȴ�д
}


/*
��������: ��һ���ֽ�
��������:
	u8 addr ���ݵ�λ��(0~1023)
����ֵ: ����������
*/
u8 AT24C08_ReadOneByte(u16 addr)
{
		u8 data=0;
		IIC_SendStart();//��ʼ�ź�
		IIC_SendOneByte(AT24C08_WRITE_ADDR);//�����豸��ַ
		IIC_GetAck();//��ȡӦ��
		IIC_SendOneByte(addr); //��Ҫ��ȡ���ݵĵ�ַ
		IIC_GetAck();//��ȡӦ��
		
		IIC_SendStart();//��ʼ�ź�
		IIC_SendOneByte(AT24C08_READ_ADDR);//�����豸��ַ
		IIC_GetAck();//��ȡӦ��
		data=IIC_RecvOneByte();//��ȡ����
		IIC_SendAck(1); //���ͷ�Ӧ��
		IIC_SendStop(); //ֹͣ�ź�
		return data;
}

/*
��������: ��ָ��λ�ö�ȡָ�����ȵ�����
��������:
	u16 addr ���ݵ�λ��(0~1023)
	u16 len ��ȡ�ĳ���
	u8 *buffer ��Ŷ�ȡ������
����ֵ: ����������
*/
void AT24C08_ReadByte(u16 addr,u16 len,u8 *buffer)
{
		u16 i=0;
		IIC_SendStart();//��ʼ�ź�
		IIC_SendOneByte(AT24C08_WRITE_ADDR);//�����豸��ַ
		IIC_GetAck();//��ȡӦ��
		IIC_SendOneByte(addr); //��Ҫ��ȡ���ݵĵ�ַ
		IIC_GetAck();//��ȡӦ��
		
		IIC_SendStart();//��ʼ�ź�
		IIC_SendOneByte(AT24C08_READ_ADDR);//�����豸��ַ
		IIC_GetAck();//��ȡӦ��
		for(i=0;i<len;i++)
		{
				buffer[i]=IIC_RecvOneByte();//��ȡ����
				if(i<len-1)IIC_SendAck(0); //����Ӧ��
				else IIC_SendAck(1); //���ͷ�Ӧ��
		}
		IIC_SendStop(); //ֹͣ�ź�
}

/*
��������: AT24C08ҳд����
��������:
	u16 addr д���λ��(0~1023)
	u8 len д��ĳ���(ÿҳ16�ֽ�)
	u8 *buffer ��Ŷ�ȡ������
*/
void AT24C08_PageWrite(u16 addr,u16 len,u8 *buffer)
{
		u16 i=0;
		IIC_SendStart();//��ʼ�ź�
		IIC_SendOneByte(AT24C08_WRITE_ADDR);//�����豸��ַ
		IIC_GetAck();//��ȡӦ��
		IIC_SendOneByte(addr); //���ݴ�ŵĵ�ַ
		IIC_GetAck();//��ȡӦ��
		
		for(i=0;i<len;i++)
		{
			IIC_SendOneByte(buffer[i]); //���ͽ�Ҫ��ŵ�����
			IIC_GetAck();//��ȡӦ��	
		}
		IIC_SendStop(); //ֹͣ�ź�
		DelayMs(10); //�ȴ�д
}


/*
��������: ��ָ��λ��д��ָ�����ȵ�����
��������:
	u16 addr ���ݵ�λ��(0~1023)
	u16 len д��ĳ���
	u8 *buffer ��ż���д�������
����ֵ: ����������
*/
void AT24C08_WriteByte(u16 addr,u16 len,u8 *buffer)
{
		u8 page_byte=16-addr%16; //�õ���ǰҳʣ����ֽ�����
		if(page_byte>len) //�жϵ�ǰҳʣ����ֽڿռ��Ƿ�д
		{
				page_byte=len; //��ʾһ���Կ���д��
		}
		while(1)
		{
				AT24C08_PageWrite(addr,page_byte,buffer); //дһҳ
				if(page_byte==len)break; //д����
				buffer+=page_byte; //ָ��ƫ��
				addr+=page_byte;//��ַƫ��
				len-=page_byte;//�õ�ʣ��û��д��ĳ���
				if(len>16)page_byte=16;
				else page_byte=len; //һ�ο���д��
		}
}

#else
/*
��������: дһ���ֽ�
��������:
	u8 addr ���ݵ�λ��(0~1023)
	u8 data ���ݷ�Χ(0~255)
*/
void AT24C08_WriteOneByte(u16 addr,u8 data)
{
		IIC_SendStart();//��ʼ�ź�
		IIC_SendAddr(AT24C08_WRITE_ADDR);//�����豸��ַ
		IIC_SendOneByte(addr); //���ݴ�ŵĵ�ַ
		IIC_SendOneByte(data); //���ͽ�Ҫ��ŵ�����
		IIC_SendStop(); //ֹͣ�ź�
		DelayMs(10); //�ȴ�д
}


/*
��������: ��һ���ֽ�
��������:
	u8 addr ���ݵ�λ��(0~1023)
����ֵ: ����������
*/
u8 AT24C08_ReadOneByte(u16 addr)
{
		u8 data=0;
		IIC_SendStart();//��ʼ�ź�
		IIC_SendAddr(AT24C08_WRITE_ADDR);//�����豸��ַ
		IIC_SendOneByte(addr); //��Ҫ��ȡ���ݵĵ�ַ
		IIC_SendStart();//��ʼ�ź�
		IIC_SendAddr(AT24C08_READ_ADDR);//�����豸��ַ
		data=IIC_RecvOneByte();//��ȡ����
		IIC_SendStop(); //ֹͣ�ź�
		return data;
}


/*
��������: ��ָ��λ�ö�ȡָ�����ȵ����� (˳���)
��������:
	u16 addr ���ݵ�λ��(0~1023)
	u16 len ��ȡ�ĳ���
	u8 *buffer ��Ŷ�ȡ������
����ֵ: ����������
*/
void AT24C08_ReadByte(u16 addr,u16 len,u8 *buffer)
{
		u32 i=0;
	  IIC_SendStart();//��ʼ�ź�
		IIC_SendAddr(AT24C08_WRITE_ADDR);//�����豸��ַ
		IIC_SendOneByte(addr); //��Ҫ��ȡ���ݵĵ�ַ
		IIC_SendStart();//��ʼ�ź�
		IIC_SendAddr(AT24C08_READ_ADDR);//�����豸��ַ
		
		for(i=0;i<len;i++)
		{
				buffer[i]=IIC_RecvOneByte();//��ȡ����
		}
		IIC_SendStop(); //ֹͣ�ź�
}

/*
��������: AT24C08ҳд����
��������:
	u16 addr д���λ��(0~1023)
	u8 len д��ĳ���(ÿҳ16�ֽ�)
	u8 *buffer ��Ŷ�ȡ������
*/
void AT24C08_PageWrite(u16 addr,u16 len,u8 *buffer)
{
		u16 i=0;
		IIC_SendStart();//��ʼ�ź�
		IIC_SendAddr(AT24C08_WRITE_ADDR);//�����豸��ַ
		IIC_SendOneByte(addr); //���ݴ�ŵĵ�ַ
		for(i=0;i<len;i++)
		{
			IIC_SendOneByte(buffer[i]); //���ͽ�Ҫ��ŵ�����
		}
		IIC_SendStop(); //ֹͣ�ź�
		DelayMs(10); //�ȴ�д
}


/*
��������: ��ָ��λ��д��ָ�����ȵ�����
��������:
	u16 addr ���ݵ�λ��(0~1023)
	u16 len д��ĳ���
	u8 *buffer ��ż���д�������
����ֵ: ����������
*/
void AT24C08_WriteByte(u16 addr,u16 len,u8 *buffer)
{
		u8 page_byte=16-addr%16; //�õ���ǰҳʣ����ֽ�����
		if(page_byte>len) //�жϵ�ǰҳʣ����ֽڿռ��Ƿ�д
		{
				page_byte=len; //��ʾһ���Կ���д��
		}
		while(1)
		{
				AT24C08_PageWrite(addr,page_byte,buffer); //дһҳ
				if(page_byte==len)break; //д����
				buffer+=page_byte; //ָ��ƫ��
				addr+=page_byte;//��ַƫ��
				len-=page_byte;//�õ�ʣ��û��д��ĳ���
				if(len>16)page_byte=16;
				else page_byte=len; //һ�ο���д��
		}
}

#endif
