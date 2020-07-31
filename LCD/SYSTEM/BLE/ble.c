#include "ble.h"
struct Bluetooth bluetooth; //蓝牙信息
#define BLE_MAXCHECK_CNT 5

/*
函数功能: BLE蓝牙初始化检测
返 回 值: 0表示正常 1表示错误
*/
u8 BLE_InitCheck(void)
{
   u8 i,j; 
   for(i=0;i<BLE_MAXCHECK_CNT;i++) //测试总次数
   {
      //清除串口相关标志位
     USART2_RX_CNT=0;
     USART2_RX_FLAG=0;
     //发送指令
     USART_X_SendString(USART2,"AT");
     //等待蓝牙反应的时间ms   
     for(j=0;j<200;j++) 
     {
         if(USART2_RX_FLAG)
         {
            if(USART2_RX_BUFF[0]=='O'
               &&USART2_RX_BUFF[1]=='K'
               &&USART2_RX_CNT==4)
            {
               USART2_RX_CNT=0;
               USART2_RX_FLAG=0; 
               return 0; //检测成功
            }
            else
            {
                break;
            }
         }
         delay_ms(5);
     }
   }
   USART2_RX_CNT=0;
   USART2_RX_FLAG=0;
   return 1;
}

/*
函数功能: 获取BLE蓝牙的MAC地址
返 回 值: 0表示正常 1表示错误
格式示例: 312796BE7C08\r\n,14
*/
u8 BLE_GetMacAddr(void)
{
   u8 i,j; 
   for(i=0;i<BLE_MAXCHECK_CNT;i++) //测试总次数
   {
      //清除串口相关标志位
     USART2_RX_CNT=0;
     USART2_RX_FLAG=0;
     //发送指令
     USART_X_SendString(USART2,"AT+MAC");
     //等待蓝牙反应的时间ms   
     for(j=0;j<200;j++) 
     {
         if(USART2_RX_FLAG)
         {
            if(USART2_RX_CNT==14)
            {
               strncpy(bluetooth.mac,(const char*)USART2_RX_BUFF,12);
               USART2_RX_CNT=0;
               USART2_RX_FLAG=0; 
               return 0; //获取成功
            }
            else
            {
                break;
            }
         }
         delay_ms(5);
     }
   }
   USART2_RX_CNT=0;
   USART2_RX_FLAG=0;
   return 1;
}

/*
函数功能: 设置BLE蓝牙名称
返 回 值: 0表示正常 1表示错误
*/
u8 BLE_SetName(const char *name)
{
   u8 i,j;
   char cmd[30];
   //拼接传入的指令
   sprintf(cmd,"AT+NAME=%s",name);
   for(i=0;i<BLE_MAXCHECK_CNT;i++) //测试总次数
   {
      //清除串口相关标志位
     USART2_RX_CNT=0;
     USART2_RX_FLAG=0;
     //发送指令
     USART_X_SendString(USART2,cmd);
     //等待蓝牙反应的时间ms   
     for(j=0;j<200;j++) 
     {
         if(USART2_RX_FLAG)
         {
            if(USART2_RX_BUFF[0]=='O'
               &&USART2_RX_BUFF[1]=='K'
               &&USART2_RX_CNT==4)
            {
               USART2_RX_CNT=0;
               USART2_RX_FLAG=0;
               delay_ms(1000); //间隔时间
               BLE_RST();      //重启蓝牙
               return 0;//检测成功
            }
            else
            {
                break;
            }
         }
         delay_ms(5);
     }
   }
   USART2_RX_CNT=0;
   USART2_RX_FLAG=0;
   return 1;
}

/*
函数功能: 复位蓝牙
返 回 值: 0表示正常 1表示错误
*/
u8 BLE_RST(void)
{
   u8 i,j; 
   for(i=0;i<BLE_MAXCHECK_CNT;i++) //测试总次数
   {
      //清除串口相关标志位
     USART2_RX_CNT=0;
     USART2_RX_FLAG=0;
     //发送指令
     USART_X_SendString(USART2,"AT+RST");
     //等待蓝牙反应的时间ms   
     for(j=0;j<200;j++) 
     {
         if(USART2_RX_FLAG)
         {
            if(USART2_RX_BUFF[0]=='O'
               &&USART2_RX_BUFF[1]=='K'
               &&USART2_RX_CNT==5)
            {
               //等待蓝牙重启成功
               delay_ms(1000);
               delay_ms(1000);
               delay_ms(1000);
              
               USART2_RX_CNT=0;
               USART2_RX_FLAG=0;
               return 0; //检测成功
            }
            else
            {
                break;
            }
         }
         delay_ms(5);
     }
   }
   USART2_RX_CNT=0;
   USART2_RX_FLAG=0;
   return 1;
}


/*
函数功能: 获取BLE蓝牙的名称
返 回 值: 0表示正常 1表示错误
*/
u8 BLE_GetName(void)
{
   u8 i,j,n; 
   for(i=0;i<BLE_MAXCHECK_CNT;i++) //测试总次数
   {
      //清除串口相关标志位
     USART2_RX_CNT=0;
     USART2_RX_FLAG=0;
     //发送指令
     USART_X_SendString(USART2,"AT+NAME");
     //等待蓝牙反应的时间ms   
     for(j=0;j<200;j++) 
     {
         if(USART2_RX_FLAG)
         {
           for(n=0;n<18;n++)
           {
              if(USART2_RX_BUFF[n]=='\r'||USART2_RX_BUFF[n]=='\n')break;
              bluetooth.name[n]=USART2_RX_BUFF[n]; //拷贝数据
           }
           USART2_RX_CNT=0;
           USART2_RX_FLAG=0; 
           return 0; //获取成功
         }
         delay_ms(5);
     }
   }
   USART2_RX_CNT=0;
   USART2_RX_FLAG=0;
   return 1;
}

/*
函数功能: 获取BLE蓝牙的连接状态
返 回 值: 0表示正常 1表示错误
示 例:
advertising,13
connected,11
*/
u8 BLE_GetConnectState(void)
{
   u8 i,j; 
   for(i=0;i<BLE_MAXCHECK_CNT;i++) //测试总次数
   {
      //清除串口相关标志位
     USART2_RX_CNT=0;
     USART2_RX_FLAG=0;
     //发送指令
     USART_X_SendString(USART2,"AT+STATE");
     //等待蓝牙反应的时间ms   
     for(j=0;j<200;j++) 
     {
         if(USART2_RX_FLAG)
         {
           if(strstr((char*)USART2_RX_BUFF,"connected")
             &&USART2_RX_CNT==11)
           {
              USART2_RX_CNT=0;
              USART2_RX_FLAG=0; 
              return 0; //获取成功  
           }
           else break;
         }
         delay_ms(5);
     }
   }
   USART2_RX_CNT=0;
   USART2_RX_FLAG=0;
   return 1;
}

