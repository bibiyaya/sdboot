//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��: uart��������
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ����44b0��uart
//����˵��:
//�޶���ʷ:
//1. ����:2009-03-10
//   ����:lst
//   �°汾�ţ�1.0.0
//   �޸�˵��: ��ֲ��44b0��1.0.1��
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
//----��ʼ��uart0ģ��----------------------------------------------------------
//���ܣ���ʼ��uart0ģ��
//������ģ���ʼ������û�в���
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//-----------------------------------------------------------------------------
bool_t module_init_uart0(void)
{
    uint32_t temp;
    pg_syscon_reg = (void*)0x7e00f000;
    pg_uart_reg = (void*)0x7f005000;
    pg_gpio_reg = (void*)0x7f008000;
    //��ʼ��IO�˿�λuart����
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
    //select mclk as clk source of uartclk ������֪���ʱ����������ʲô�ģ�
    //������������baud���ֲ�Ҳû��
    pg_syscon_reg->CLK_SRC |= 0x2000;
    temp = pg_syscon_reg->CLK_DIV2;
    temp &= 0xfff0ffff;
    temp |= 0x00010000;
    pg_syscon_reg->CLK_DIV2 = temp; //uartclk = mclk/2
    //ϵͳ��ʼ��ʱ�Ѿ�ʹ�жϴ��ڽ�ֹ״̬�������ٽ�ֹ������жϡ�
    //��ʼ��uartӲ���������ݽṹ
    pg_uart_reg->ULCON = 0x3;
    pg_uart_reg->UCON =  0x805;
    pg_uart_reg->UFCON = 0x57;   //bit0=1ʹ��fifo.
    pg_uart_reg->UMCON = 0x0;
    pg_uart_reg->UBRDIV = ((cn_pclk<<2)/115200 -32)>>6;
    pg_uart_reg->UDIVSLOT0 = 0x0080;    //΢��baud����ʽ�汾�У���ֵ��Ҫ���㡣
    temp = pg_uart_reg->UERSTAT;     //��һ���������״̬
    return true;
}

