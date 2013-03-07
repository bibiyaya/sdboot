//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块: uart驱动程序
//作者：lst
//版本：V1.0.0
//文件描述: 驱动44b0的uart
//其他说明:
//修订历史:
//1. 日期:2009-03-10
//   作者:lst
//   新版本号：1.0.0
//   修改说明: 移植字44b0的1.0.1版
//------------------------------------------------------
#include "config.h"
#include "gpio.h"
#include "syscon.h"
#include "uart.h"

volatile struct syscon_reg *pg_syscon_reg;
volatile struct hard_reg_uart *pg_uart_reg;
volatile struct hard_reg_gpio *pg_gpio_reg;

uint16_t u16g_evtt_uart0_error;
uint16_t u16g_evtt_uart1_error;
uint16_t u16g_evtt_uart2_error;

uint8_t scan_byte(void)
{
    uint32_t fifo_reg;
    for(;;)
    {
        fifo_reg = pg_uart_reg->UFSTAT;
        if((fifo_reg & 0x3f) != 0)
            return pg_uart_reg->URXH;
    }
}
bool_t read_byte(uint8_t *byte)
{
    uint32_t fifo_reg;
    fifo_reg = pg_uart_reg->UFSTAT;
    if((fifo_reg & 0x3f) != 0)
    {
        *byte = pg_uart_reg->URXH;
        return true;
    }else
        return false;
}

void printf_byte(uint8_t byte)
{
    uint32_t fifo_reg;
    for(;;)
    {
        fifo_reg = pg_uart_reg->UFSTAT;
        if(((fifo_reg>>8) & 0x3f) < 0x3f)
        {
            pg_uart_reg->UTXH = byte;
            return ;
        }
    }
}

void printf_str(uint8_t *str)
{
    uint32_t i=0;
    while(str[i] != 0)
    {
        printf_byte(str[i]);
        i++;
    }
}
//----初始化uart0模块----------------------------------------------------------
//功能：初始化uart0模块
//参数：模块初始化函数没有参数
//返回：true = 成功初始化，false = 初始化失败
//-----------------------------------------------------------------------------
bool_t module_init_uart0(void)
{
    uint32_t temp;
    pg_syscon_reg = (void*)0x7e00f000;
    pg_uart_reg = (void*)0x7f005000;
    pg_gpio_reg = (void*)0x7f008000;
    //初始化IO端口位uart功能
    //Ports  :  GPA10 GPA9 GPA8 GPA7 GPA6 GPA5 GPA4 GPA3 GPA2 GPA1  GPA0
    //Signal :   xx    xx   xx   xx   xx   xx   xx   xx   xx  TXD0  RXD0
    //Binary :   xx    xx   xx   xx   xx   xx   xx   xx   xx  0010  0010
    temp = pg_gpio_reg->GPACON;
    temp &= ~0xff;
    temp |= 0x22;
    pg_gpio_reg->GPACON = temp;
    pg_gpio_reg->GPAPUD &= ~0xf;   // The pull u/d function is disabled GPA[1:0]
    pg_gpio_reg->GPACONSLP |= 0xf; // keep GPA[1:0] Previous state when sleep
    pg_gpio_reg->GPAPUDSLP &= ~0xf;   // disabled pull u/d GPA[1:0] when sleep
    //select mclk as clk source of uartclk ，但不知这个时钟是用来干什么的，
    //不是用来产生baud，手册也没讲
    pg_syscon_reg->CLK_SRC |= 0x2000;
    temp = pg_syscon_reg->CLK_DIV2;
    temp &= 0xfff0ffff;
    temp |= 0x00010000;
    pg_syscon_reg->CLK_DIV2 = temp; //uartclk = mclk/2
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
    //初始化uart硬件控制数据结构
    pg_uart_reg->ULCON = 0x3;
    pg_uart_reg->UCON =  0x805;
    pg_uart_reg->UFCON = 0x57;   //bit0=1使能fifo.
    pg_uart_reg->UMCON = 0x0;
    pg_uart_reg->UBRDIV = ((cn_pclk<<2)/115200 -32)>>6;
    pg_uart_reg->UDIVSLOT0 = 0x0080;    //微调baud，正式版本中，此值需要计算。
    temp = pg_uart_reg->UERSTAT;     //读一下清除错误状态
    return true;
}

