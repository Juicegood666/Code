#include "bluetooth.h"
u8 BLE_MAC_BUFFER[15];  //����ʹ������MAC��ַ

/*
��������: �������Ӳ���Ƿ�����
��������ֵ: 1��ʾ������ 0��ʾ�ɹ�
*/
u8 BLE_Init(void)
{
    u8 i,j;
    for(i=0;i<10;i++)
    {
        USARTx_StringSend(USART2,"AT");
        for(j=0;j<10;j++)
        {
            delay_ms(50);
            if(USART2_RX_FLAG)
            {
              USART2_RX_FLAG=0;
              USART2_RX_CNT=0;
              if(USART2_RX_BUFFER[0]=='O'&&USART2_RX_BUFFER[1]=='K')
              {
                 return 0;
              }   
            }
        }
    }
    return 1;
}

/*
��������: ��ȡ������MAC��ַ
*/
u8 BLE_GetMacAddr(void)
{
    u8 i,j,cnt;
    for(i=0;i<10;i++)
    {
        USARTx_StringSend(USART2,"AT+MAC");
        for(j=0;j<10;j++)
        {
            delay_ms(50);
            if(USART2_RX_FLAG)
            {
              cnt=USART2_RX_CNT;
              USART2_RX_FLAG=0;
              USART2_RX_CNT=0;
              if(cnt==14)
              {
                 //DB2796BE7C08
                 memcpy(BLE_MAC_BUFFER,USART2_RX_BUFFER,12);
                 BLE_MAC_BUFFER[12]='\0';
                 return 0;
              }
            }
        }
    }
    return 1;
}


/*
��������: ��λ����ģ��
��������ֵ: 1��ʾ������ 0��ʾ�ɹ�
*/
u8 BLE_Reset(void)
{
    u8 i,j;
    for(i=0;i<10;i++)
    {
        USARTx_StringSend(USART2,"AT+RST");
        for(j=0;j<10;j++)
        {
            delay_ms(50);
            if(USART2_RX_FLAG)
            {
              USART2_RX_FLAG=0;
              USART2_RX_CNT=0;
              if(USART2_RX_BUFFER[0]=='O'&&USART2_RX_BUFFER[1]=='K')
              {
                 return 0;
              }   
            }
        }
    }
    return 1;
}

/*
��������: ������������
��������ֵ: 1��ʾ������ 0��ʾ�ɹ�
*/
u8 BLE_SetName(char *name)
{
    u8 i,j;
    char ble_name[20];
    strcpy(ble_name,"AT+NAME=");
    strcat(ble_name,name);
    for(i=0;i<10;i++)
    {
        USARTx_StringSend(USART2,ble_name);
        for(j=0;j<10;j++)
        {
            delay_ms(50);
            if(USART2_RX_FLAG)
            {
              USART2_RX_FLAG=0;
              USART2_RX_CNT=0;
              if(USART2_RX_BUFFER[0]=='O'&&USART2_RX_BUFFER[1]=='K')
              {
                 BLE_Reset(); //��λ
                 delay_ms(1000);
                 return 0;
              }   
            }
        }
    }
    return 1;
}

/*
��������: ��ȡ��������״̬
��������ֵ: 1��ʾδ���� 0��ʾ���ӳɹ�
*/
u8 BLE_GetConnectState(void)
{
    u8 i,j;
    for(i=0;i<10;i++)
    {
        USARTx_StringSend(USART2,"AT+STATE");
        for(j=0;j<10;j++)
        {
            delay_ms(50);
            if(USART2_RX_FLAG)
            {
              USART2_RX_BUFFER[USART2_RX_CNT]='\0';
              USART2_RX_FLAG=0;
              USART2_RX_CNT=0;
              if(strstr((char*)USART2_RX_BUFFER,"connected"))
              {
                 return 0;
              }                  
            }
        }
    }
    return 1;
}
