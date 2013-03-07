//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块: uart驱动程序
//作者：lst
//版本：V1.0.1
//文件描述: 驱动44b0的uart
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-03-10
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef _UART_H_
#define _UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"

//UTRSTAT状态位定义
#define UartTXShiftEmpty    (1<<2)  //1:发送移位寄存器空,0:非空
#define UartTxBufEmpty      (1<<1)  //1:发送缓冲区空,0:非空
#define UartRxBufNotEmpty   (1<<0)  //1:接受缓冲区有数据,0:无数据
//UFCON FIFO控制寄存器
#define cn_tx_fifo_reset    (1<<2)  //写1复位发送fifo,读总是0
#define cn_rx_fifo_reset    (1<<1)  //写1复位发送fifo,读总是0
//UFSTAT FIFO状态寄存器
#define cn_rx_over          (1<<6)  //1:接收fifo溢出,0:正常

//通信状态变量各位定义
#define cn_ser_tx_busy         (1<<0)
#define cn_ser_rxbuf_empty     (1<<1)
#define cn_ser_receive_over    (1<<2)

//中断控制位，在2410的中断控制器中，每个串口只分配了一个中断，具体中断原因是通过
//查询 SUBSRCPND 寄存器相应的位来得到的，控制 INTSUBMSK 中相应位可独立地禁止
//接收/发送中断。
#define BIT_SUB_ERR2    (0x1<<8)
#define BIT_SUB_TXD2    (0x1<<7)
#define BIT_SUB_RXD2    (0x1<<6)
#define BIT_SUB_ERR1    (0x1<<5)
#define BIT_SUB_TXD1    (0x1<<4)
#define BIT_SUB_RXD1    (0x1<<3)
#define BIT_SUB_ERR0    (0x1<<2)
#define BIT_SUB_TXD0    (0x1<<1)
#define BIT_SUB_RXD0    (0x1<<0)
#define BIT_ALL_UART    (0x1ff)

/*
//串口状态控制结构
struct uart_UCB
{
    struct ring_buf send_ring_buf;           //环形发送缓冲区.
    struct ring_buf recv_ring_buf;           //环形接收缓冲区.

    struct semaphore_LCB *send_buf_semp;     //发送缓冲区锁
    struct semaphore_LCB *recv_buf_semp;     //接收缓冲区锁

    uint16_t evtt_right_write;      //设备右手被写后弹出的事件类型
    uint16_t evtt_left_write;       //设备左手被写后弹出的事件类型
    uint16_t recv_trigger_level;    //串口接收触发水平,即收到多少数据时弹出事件
    uint16_t timeout;               //未到触发水平,持续设定时间内没有收到数据,
                                    //也弹出事件.
    uint16_t send_trigger_level;    //发送同步触发水平，当发送环形缓冲区满，将
                                    //用send_buf_semp阻塞当前线程，直到互相缓冲
                                    //区数据量降至send_trigger_level时释放之。
    struct hard_reg_uart volatile *my_reg;
    uint32_t baud;
    uint32_t rx_int_line,tx_int_line;    //发送和接收中断号
    ufast_t status;
    ufast_t serial_no;
};
*/
//串口错误类型
enum uart_error_no
{
    enum_recv_fifo_over,         //接收硬件fifo溢出
    enum_recv_buf_over,          //接收缓冲区溢出
};

//uart0 base address:0x7f005000
//uart1 base address:0x7f005400
//uart2 base address:0x7f005800
//uart3 base address:0x7f005c00
struct hard_reg_uart
{
    uint32_t ULCON;     //offset 00 Line control
    uint32_t UCON;      //offset 04 Control
    uint32_t UFCON;     //offset 08 FIFO control
    uint32_t UMCON;     //offset 0c Modem control
    uint32_t UTRSTAT;   //offset 10 Tx/Rx status
    uint32_t UERSTAT;   //offset 14 Rx error status
    uint32_t UFSTAT;    //offset 18 FIFO status
    uint32_t UMSTAT;    //offset 1c Modem status

#ifdef __BIG_ENDIAN
    uint8_t  reserve;
    uint8_t  reserve1;
    uint8_t  reserve2;
    uint8_t  UTXH;      //offset 23
    uint8_t  reserve3;
    uint8_t  reserve4;
    uint8_t  reserve5;
    uint8_t  URXH;      //offset 27

#else //Little Endian
    uint8_t  UTXH;      //offset 20
    uint8_t  reserve;   //UTXH和URXH是字节访问寄存器,reserve是占位变量.
    uint8_t  reserve1;
    uint8_t  reserve2;
    uint8_t  URXH;      //offset 24
    uint8_t  reserve3;
    uint8_t  reserve4;
    uint8_t  reserve5;
 #endif
    uint32_t UBRDIV;    //offset 28 Baud rate divisor
    uint32_t UDIVSLOT0; //offset 2c UART channel 0 Dividing slot register
    uint32_t UINTP0;    //offset 30 UART channel 0 Interrupt Pending Register
    uint32_t UINTSP0;   //offset 34 UART channel 0 Interrupt Source Pending Register
    uint32_t UINTM0;    //offset 38 UART channel 0 Interrupt Mask Register
};

#define uart0_buf_len  512
#define uart1_buf_len  512
#define uart2_buf_len  512


//串口设备控制命令常数，用于uart_ctrl函数。
enum uart_ctrl_no
{
    enum_uart_connect_recv_evtt,  //设备被从右手写入的数据超过软件触发水平时弹出
                                  //本类型事件
    enum_uart_disconnect_recv_evtt, //断开右手事件
    enum_uart_connect_send_evtt,  //设备被从左手写入的数据时弹出本类型事件
    enum_uart_disconnect_send_evtt, //断开左手事件
    enum_uart_start,              //启动串口
    enum_uart_stop,               //停止串口
    enum_uart_set_speed,          //设置uartBaud.
    enum_uart_send_data,          //发送数据
    enum_uart_recv_data,          //接收数据
    enum_uart_completed_send,     //完成发送工作
    enum_uart_rx_pause,           //暂停接收数据到缓冲区
    enum_uart_rx_resume,          //重新开始接收数据到缓冲区
    enum_uart_overtime,           //设置接收超时时间，毫秒数
    enum_uart_recv_soft_level,    //设置接收软件缓冲区触发水平
    enum_uart_send_soft_level,    //设置接收软件缓冲区触发水平
    enum_uart_recv_hard_level,    //设置接收fifo触发水平
    enum_uart_send_hard_level,    //设置发送fifo触发水平
};

bool_t module_init_uart0(void);
uint8_t scan_byte(void);
bool_t read_byte(uint8_t *byte);
void printf_byte( uint8_t byte);
void printf_str(uint8_t *str);

#ifdef __cplusplus
}
#endif

#endif // _UART_H_

