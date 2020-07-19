#include "usart.h"
/*
��������: ����1�ĳ�ʼ��
Ӳ������: PA9(TX)  �� PA10(RX)
*/
void USART1_Init(u32 baud)
{
    /*1. ��ʱ��*/
    RCC->APB2ENR|=1<<14; //USART1ʱ��
    RCC->APB2ENR|=1<<2;  //PA
    RCC->APB2RSTR|=1<<14; //������λʱ��
    RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ
    /*2. ����GPIO�ڵ�ģʽ*/
    GPIOA->CRH&=0xFFFFF00F;
    GPIOA->CRH|=0x000008B0;
    /*3. ���ò�����*/
    USART1->BRR=72000000/baud;
    /*4. ���ú��ļĴ���*/
    USART1->CR1|=1<<5; //���������ж�
    STM32_SetPriority(USART1_IRQn,1,1); //�����ж����ȼ�
    USART1->CR1|=1<<2; //��������
    USART1->CR1|=1<<3; //��������
    USART1->CR1|=1<<13;//�������ڹ���
}

/*
��������: ����2�ĳ�ʼ��
Ӳ������: PA2(TX)  �� PA3(RX)
*/
void USART2_Init(u32 baud)
{
    /*1. ��ʱ��*/
    RCC->APB1ENR|=1<<17; //USART2ʱ��
    RCC->APB2ENR|=1<<2;  //PA
    RCC->APB1RSTR|=1<<17; //������λʱ��
    RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ
    
    /*2. ����GPIO�ڵ�ģʽ*/
    GPIOA->CRL&=0xFFFF00FF;
    GPIOA->CRL|=0x00008B00;
    /*3. ���ò�����*/
    USART2->BRR=36000000/baud;
    /*4. ���ú��ļĴ���*/
    USART2->CR1|=1<<5; //���������ж�
    STM32_SetPriority(USART2_IRQn,1,1); //�����ж����ȼ�
    USART2->CR1|=1<<2; //��������
    USART2->CR1|=1<<3; //��������
    USART2->CR1|=1<<13;//�������ڹ���
}

/*
��������: ����3�ĳ�ʼ��
Ӳ������: PB10(TX)  �� PB11(RX)
*/
void USART3_Init(u32 baud)
{
    /*1. ��ʱ��*/
    RCC->APB1ENR|=1<<18; //USART3ʱ��
    RCC->APB2ENR|=1<<3;  //PB
    RCC->APB1RSTR|=1<<18; //������λʱ��
    RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ
    
    /*2. ����GPIO�ڵ�ģʽ*/
    GPIOB->CRH&=0xFFFF00FF;
    GPIOB->CRH|=0x00008B00;
    /*3. ���ò�����*/
    USART3->BRR=36000000/baud;
    /*4. ���ú��ļĴ���*/
    USART3->CR1|=1<<5; //���������ж�
    STM32_SetPriority(USART3_IRQn,1,1); //�����ж����ȼ�
    USART3->CR1|=1<<2; //��������
    USART3->CR1|=1<<3; //��������
    USART3->CR1|=1<<13;//�������ڹ���
}

u8 USART1_RX_BUFFER[USART1_RX_LENGTH]; //����������ݵĻ�����
u32 USART1_RX_CNT=0;  //��ǰ���յ������ݳ���
u8 USART1_RX_FLAG=0; //1��ʾ���ݽ������ 0��ʾû�н������

//����1���жϷ�����
void USART1_IRQHandler(void)
{
    u8 data;
    //�����ж�
    if(USART1->SR&1<<5)
    {
        TIM1->CNT=0; //���������
        TIM1->CR1|=1<<0; //������ʱ��1
        data=USART1->DR; //��ȡ��������
      //  if(USART1_RX_FLAG==0) //�ж���һ�ε������Ƿ��Ѿ��������
        {
            //�ж��Ƿ���Լ�������
            if(USART1_RX_CNT<USART1_RX_LENGTH)
            {
               USART1_RX_BUFFER[USART1_RX_CNT++]=data;
            }
            else  //���ܽ��գ������洢��Χ��ǿ�Ʊ�ʾ�������
            {
                USART1_RX_FLAG=1;
            }
        } 
    }
}


u8 USART2_RX_BUFFER[USART2_RX_LENGTH]; //����������ݵĻ�����
u32 USART2_RX_CNT=0;  //��ǰ���յ������ݳ���
u8 USART2_RX_FLAG=0; //1��ʾ���ݽ������ 0��ʾû�н������

//����2���жϷ�����
void USART2_IRQHandler(void)
{
    u8 data;
    //�����ж�
    if(USART2->SR&1<<5)
    {
        TIM2->CNT=0; //���������
        TIM2->CR1|=1<<0; //������ʱ��2
        data=USART2->DR; //��ȡ��������
      //  if(USART2_RX_FLAG==0) //�ж���һ�ε������Ƿ��Ѿ��������
        {
            //�ж��Ƿ���Լ�������
            if(USART2_RX_CNT<USART2_RX_LENGTH)
            {
               USART2_RX_BUFFER[USART2_RX_CNT++]=data;
            }
            else  //���ܽ��գ������洢��Χ��ǿ�Ʊ�ʾ�������
            {
                USART2_RX_FLAG=1;
            }
        } 
    }
}

u8 USART3_RX_BUFFER[USART3_RX_LENGTH]; //����������ݵĻ�����
u32 USART3_RX_CNT=0;  //��ǰ���յ������ݳ���
u8 USART3_RX_FLAG=0; //1��ʾ���ݽ������ 0��ʾû�н������

//����3���жϷ�����
void USART3_IRQHandler(void)
{
    u8 data;
    //�����ж�
    if(USART3->SR&1<<5)
    {
        TIM3->CNT=0; //���������
        TIM3->CR1|=1<<0; //������ʱ��3
        data=USART3->DR; //��ȡ��������
      //  if(USART3_RX_FLAG==0) //�ж���һ�ε������Ƿ��Ѿ��������
        {
            //�ж��Ƿ���Լ�������
            if(USART3_RX_CNT<USART3_RX_LENGTH)
            {
               USART3_RX_BUFFER[USART3_RX_CNT++]=data;
            }
            else  //���ܽ��գ������洢��Χ��ǿ�Ʊ�ʾ�������
            {
                USART3_RX_FLAG=1;
            }
        } 
    }
}


/*
��������: �ַ�������
*/
void USARTx_StringSend(USART_TypeDef *USARTx,char *str)
{
   while(*str!='\0')
   {
       USARTx->DR=*str++;
       while(!(USARTx->SR&1<<7)){}
   }
}

/*
��������: ���ݷ���
*/
void USARTx_DataSend(USART_TypeDef *USARTx,u8 *data,u32 len)
{
   u32 i;
   for(i=0;i<len;i++)
   {
       USARTx->DR=*data++;
       while(!(USARTx->SR&1<<7)){}
   }
}

//printf�����ײ㺯���ӿ�
int fputc(int c, FILE* stream)
{
    USART1->DR=c;
    while(!(USART1->SR&1<<7)){}
    return c;
}


/*
��������: ��ʽ����ӡ����
*/
char USART1_PRINTF_BUFF[1024];
void USART1_Printf(char *fmt,...)
{
   va_list ap;
   /*1. ��ʼ���β��б�*/
   va_start(ap,fmt);
   /*2. ��ȡ�ɱ��β�����*/
    vsprintf(USART1_PRINTF_BUFF,fmt,ap);
   /*3. �������ͷſռ�*/
    va_end(ap);
   /*4. ������ݵ�����1*/
   USARTx_StringSend(USART1,USART1_PRINTF_BUFF);
   
   //USART1_Printf("%d%s",123,454656); 
   //int data=va_arg(ap,int);
}
