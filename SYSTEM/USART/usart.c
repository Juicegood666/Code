#include "usart.h"
/*
函数功能: 串口1的初始化
硬件连接: PA9(TX)  和 PA10(RX)
*/
void USART1_Init(u32 baud)
{
    /*1. 开时钟*/
    RCC->APB2ENR|=1<<14; //USART1时钟
    RCC->APB2ENR|=1<<2;  //PA
    RCC->APB2RSTR|=1<<14; //开启复位时钟
    RCC->APB2RSTR&=~(1<<14);//停止复位
    /*2. 配置GPIO口的模式*/
    GPIOA->CRH&=0xFFFFF00F;
    GPIOA->CRH|=0x000008B0;
    /*3. 配置波特率*/
    USART1->BRR=72000000/baud;
    /*4. 配置核心寄存器*/
    USART1->CR1|=1<<5; //开启接收中断
    STM32_SetPriority(USART1_IRQn,1,1); //设置中断优先级
    USART1->CR1|=1<<2; //开启接收
    USART1->CR1|=1<<3; //开启发送
    USART1->CR1|=1<<13;//开启串口功能
}

/*
函数功能: 串口2的初始化
硬件连接: PA2(TX)  和 PA3(RX)
*/
void USART2_Init(u32 baud)
{
    /*1. 开时钟*/
    RCC->APB1ENR|=1<<17; //USART2时钟
    RCC->APB2ENR|=1<<2;  //PA
    RCC->APB1RSTR|=1<<17; //开启复位时钟
    RCC->APB1RSTR&=~(1<<17);//停止复位
    
    /*2. 配置GPIO口的模式*/
    GPIOA->CRL&=0xFFFF00FF;
    GPIOA->CRL|=0x00008B00;
    /*3. 配置波特率*/
    USART2->BRR=36000000/baud;
    /*4. 配置核心寄存器*/
    USART2->CR1|=1<<5; //开启接收中断
    STM32_SetPriority(USART2_IRQn,1,1); //设置中断优先级
    USART2->CR1|=1<<2; //开启接收
    USART2->CR1|=1<<3; //开启发送
    USART2->CR1|=1<<13;//开启串口功能
}

/*
函数功能: 串口3的初始化
硬件连接: PB10(TX)  和 PB11(RX)
*/
void USART3_Init(u32 baud)
{
    /*1. 开时钟*/
    RCC->APB1ENR|=1<<18; //USART3时钟
    RCC->APB2ENR|=1<<3;  //PB
    RCC->APB1RSTR|=1<<18; //开启复位时钟
    RCC->APB1RSTR&=~(1<<18);//停止复位
    
    /*2. 配置GPIO口的模式*/
    GPIOB->CRH&=0xFFFF00FF;
    GPIOB->CRH|=0x00008B00;
    /*3. 配置波特率*/
    USART3->BRR=36000000/baud;
    /*4. 配置核心寄存器*/
    USART3->CR1|=1<<5; //开启接收中断
    STM32_SetPriority(USART3_IRQn,1,1); //设置中断优先级
    USART3->CR1|=1<<2; //开启接收
    USART3->CR1|=1<<3; //开启发送
    USART3->CR1|=1<<13;//开启串口功能
}

u8 USART1_RX_BUFFER[USART1_RX_LENGTH]; //保存接收数据的缓冲区
u32 USART1_RX_CNT=0;  //当前接收到的数据长度
u8 USART1_RX_FLAG=0; //1表示数据接收完毕 0表示没有接收完毕

//串口1的中断服务函数
void USART1_IRQHandler(void)
{
    u8 data;
    //接收中断
    if(USART1->SR&1<<5)
    {
        TIM1->CNT=0; //清除计数器
        TIM1->CR1|=1<<0; //开启定时器1
        data=USART1->DR; //读取串口数据
      //  if(USART1_RX_FLAG==0) //判断上一次的数据是否已经处理完毕
        {
            //判断是否可以继续接收
            if(USART1_RX_CNT<USART1_RX_LENGTH)
            {
               USART1_RX_BUFFER[USART1_RX_CNT++]=data;
            }
            else  //不能接收，超出存储范围，强制表示接收完毕
            {
                USART1_RX_FLAG=1;
            }
        } 
    }
}


u8 USART2_RX_BUFFER[USART2_RX_LENGTH]; //保存接收数据的缓冲区
u32 USART2_RX_CNT=0;  //当前接收到的数据长度
u8 USART2_RX_FLAG=0; //1表示数据接收完毕 0表示没有接收完毕

//串口2的中断服务函数
void USART2_IRQHandler(void)
{
    u8 data;
    //接收中断
    if(USART2->SR&1<<5)
    {
        TIM2->CNT=0; //清除计数器
        TIM2->CR1|=1<<0; //开启定时器2
        data=USART2->DR; //读取串口数据
      //  if(USART2_RX_FLAG==0) //判断上一次的数据是否已经处理完毕
        {
            //判断是否可以继续接收
            if(USART2_RX_CNT<USART2_RX_LENGTH)
            {
               USART2_RX_BUFFER[USART2_RX_CNT++]=data;
            }
            else  //不能接收，超出存储范围，强制表示接收完毕
            {
                USART2_RX_FLAG=1;
            }
        } 
    }
}

u8 USART3_RX_BUFFER[USART3_RX_LENGTH]; //保存接收数据的缓冲区
u32 USART3_RX_CNT=0;  //当前接收到的数据长度
u8 USART3_RX_FLAG=0; //1表示数据接收完毕 0表示没有接收完毕

//串口3的中断服务函数
void USART3_IRQHandler(void)
{
    u8 data;
    //接收中断
    if(USART3->SR&1<<5)
    {
        TIM3->CNT=0; //清除计数器
        TIM3->CR1|=1<<0; //开启定时器3
        data=USART3->DR; //读取串口数据
      //  if(USART3_RX_FLAG==0) //判断上一次的数据是否已经处理完毕
        {
            //判断是否可以继续接收
            if(USART3_RX_CNT<USART3_RX_LENGTH)
            {
               USART3_RX_BUFFER[USART3_RX_CNT++]=data;
            }
            else  //不能接收，超出存储范围，强制表示接收完毕
            {
                USART3_RX_FLAG=1;
            }
        } 
    }
}


/*
函数功能: 字符串发送
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
函数功能: 数据发送
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

//printf函数底层函数接口
int fputc(int c, FILE* stream)
{
    USART1->DR=c;
    while(!(USART1->SR&1<<7)){}
    return c;
}


/*
函数功能: 格式化打印函数
*/
char USART1_PRINTF_BUFF[1024];
void USART1_Printf(char *fmt,...)
{
   va_list ap;
   /*1. 初始化形参列表*/
   va_start(ap,fmt);
   /*2. 提取可变形参数据*/
    vsprintf(USART1_PRINTF_BUFF,fmt,ap);
   /*3. 结束，释放空间*/
    va_end(ap);
   /*4. 输出数据到串口1*/
   USARTx_StringSend(USART1,USART1_PRINTF_BUFF);
   
   //USART1_Printf("%d%s",123,454656); 
   //int data=va_arg(ap,int);
}
