#include "ble.h"
struct Bluetooth bluetooth; //������Ϣ
#define BLE_MAXCHECK_CNT 5

/*
��������: BLE������ʼ�����
�� �� ֵ: 0��ʾ���� 1��ʾ����
*/
u8 BLE_InitCheck(void)
{
   u8 i,j; 
   for(i=0;i<BLE_MAXCHECK_CNT;i++) //�����ܴ���
   {
      //���������ر�־λ
     USART2_RX_CNT=0;
     USART2_RX_FLAG=0;
     //����ָ��
     USART_X_SendString(USART2,"AT");
     //�ȴ�������Ӧ��ʱ��ms   
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
               return 0; //���ɹ�
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
��������: ��ȡBLE������MAC��ַ
�� �� ֵ: 0��ʾ���� 1��ʾ����
��ʽʾ��: 312796BE7C08\r\n,14
*/
u8 BLE_GetMacAddr(void)
{
   u8 i,j; 
   for(i=0;i<BLE_MAXCHECK_CNT;i++) //�����ܴ���
   {
      //���������ر�־λ
     USART2_RX_CNT=0;
     USART2_RX_FLAG=0;
     //����ָ��
     USART_X_SendString(USART2,"AT+MAC");
     //�ȴ�������Ӧ��ʱ��ms   
     for(j=0;j<200;j++) 
     {
         if(USART2_RX_FLAG)
         {
            if(USART2_RX_CNT==14)
            {
               strncpy(bluetooth.mac,(const char*)USART2_RX_BUFF,12);
               USART2_RX_CNT=0;
               USART2_RX_FLAG=0; 
               return 0; //��ȡ�ɹ�
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
��������: ����BLE��������
�� �� ֵ: 0��ʾ���� 1��ʾ����
*/
u8 BLE_SetName(const char *name)
{
   u8 i,j;
   char cmd[30];
   //ƴ�Ӵ����ָ��
   sprintf(cmd,"AT+NAME=%s",name);
   for(i=0;i<BLE_MAXCHECK_CNT;i++) //�����ܴ���
   {
      //���������ر�־λ
     USART2_RX_CNT=0;
     USART2_RX_FLAG=0;
     //����ָ��
     USART_X_SendString(USART2,cmd);
     //�ȴ�������Ӧ��ʱ��ms   
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
               delay_ms(1000); //���ʱ��
               BLE_RST();      //��������
               return 0;//���ɹ�
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
��������: ��λ����
�� �� ֵ: 0��ʾ���� 1��ʾ����
*/
u8 BLE_RST(void)
{
   u8 i,j; 
   for(i=0;i<BLE_MAXCHECK_CNT;i++) //�����ܴ���
   {
      //���������ر�־λ
     USART2_RX_CNT=0;
     USART2_RX_FLAG=0;
     //����ָ��
     USART_X_SendString(USART2,"AT+RST");
     //�ȴ�������Ӧ��ʱ��ms   
     for(j=0;j<200;j++) 
     {
         if(USART2_RX_FLAG)
         {
            if(USART2_RX_BUFF[0]=='O'
               &&USART2_RX_BUFF[1]=='K'
               &&USART2_RX_CNT==5)
            {
               //�ȴ����������ɹ�
               delay_ms(1000);
               delay_ms(1000);
               delay_ms(1000);
              
               USART2_RX_CNT=0;
               USART2_RX_FLAG=0;
               return 0; //���ɹ�
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
��������: ��ȡBLE����������
�� �� ֵ: 0��ʾ���� 1��ʾ����
*/
u8 BLE_GetName(void)
{
   u8 i,j,n; 
   for(i=0;i<BLE_MAXCHECK_CNT;i++) //�����ܴ���
   {
      //���������ر�־λ
     USART2_RX_CNT=0;
     USART2_RX_FLAG=0;
     //����ָ��
     USART_X_SendString(USART2,"AT+NAME");
     //�ȴ�������Ӧ��ʱ��ms   
     for(j=0;j<200;j++) 
     {
         if(USART2_RX_FLAG)
         {
           for(n=0;n<18;n++)
           {
              if(USART2_RX_BUFF[n]=='\r'||USART2_RX_BUFF[n]=='\n')break;
              bluetooth.name[n]=USART2_RX_BUFF[n]; //��������
           }
           USART2_RX_CNT=0;
           USART2_RX_FLAG=0; 
           return 0; //��ȡ�ɹ�
         }
         delay_ms(5);
     }
   }
   USART2_RX_CNT=0;
   USART2_RX_FLAG=0;
   return 1;
}

/*
��������: ��ȡBLE����������״̬
�� �� ֵ: 0��ʾ���� 1��ʾ����
ʾ ��:
advertising,13
connected,11
*/
u8 BLE_GetConnectState(void)
{
   u8 i,j; 
   for(i=0;i<BLE_MAXCHECK_CNT;i++) //�����ܴ���
   {
      //���������ر�־λ
     USART2_RX_CNT=0;
     USART2_RX_FLAG=0;
     //����ָ��
     USART_X_SendString(USART2,"AT+STATE");
     //�ȴ�������Ӧ��ʱ��ms   
     for(j=0;j<200;j++) 
     {
         if(USART2_RX_FLAG)
         {
           if(strstr((char*)USART2_RX_BUFF,"connected")
             &&USART2_RX_CNT==11)
           {
              USART2_RX_CNT=0;
              USART2_RX_FLAG=0; 
              return 0; //��ȡ�ɹ�  
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

