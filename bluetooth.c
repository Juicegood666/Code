#include "bluetooth.h"
u8 BLE_MAC_BUFFER[15];  //保存使蓝牙的MAC地址

/*
函数功能: 检测蓝牙硬件是否正常
函数返回值: 1表示有问题 0表示成功
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
函数功能: 获取蓝牙的MAC地址
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
函数功能: 复位蓝牙模块
函数返回值: 1表示有问题 0表示成功
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
函数功能: 设置蓝牙名称
函数返回值: 1表示有问题 0表示成功
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
                 BLE_Reset(); //复位
                 delay_ms(1000);
                 return 0;
              }   
            }
        }
    }
    return 1;
}

/*
函数功能: 获取蓝牙连接状态
函数返回值: 1表示未连接 0表示连接成功
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
