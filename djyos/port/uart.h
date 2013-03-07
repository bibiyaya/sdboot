//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��: uart��������
//���ߣ�lst
//�汾��V1.0.1
//�ļ�����: ����44b0��uart
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef _UART_H_
#define _UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"

//UTRSTAT״̬λ����
#define UartTXShiftEmpty    (1<<2)  //1:������λ�Ĵ�����,0:�ǿ�
#define UartTxBufEmpty      (1<<1)  //1:���ͻ�������,0:�ǿ�
#define UartRxBufNotEmpty   (1<<0)  //1:���ܻ�����������,0:������
//UFCON FIFO���ƼĴ���
#define cn_tx_fifo_reset    (1<<2)  //д1��λ����fifo,������0
#define cn_rx_fifo_reset    (1<<1)  //д1��λ����fifo,������0
//UFSTAT FIFO״̬�Ĵ���
#define cn_rx_over          (1<<6)  //1:����fifo���,0:����

//ͨ��״̬������λ����
#define cn_ser_tx_busy         (1<<0)
#define cn_ser_rxbuf_empty     (1<<1)
#define cn_ser_receive_over    (1<<2)

//�жϿ���λ����2410���жϿ������У�ÿ������ֻ������һ���жϣ������ж�ԭ����ͨ��
//��ѯ SUBSRCPND �Ĵ�����Ӧ��λ���õ��ģ����� INTSUBMSK ����Ӧλ�ɶ����ؽ�ֹ
//����/�����жϡ�
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
//����״̬���ƽṹ
struct uart_UCB
{
    struct ring_buf send_ring_buf;           //���η��ͻ�����.
    struct ring_buf recv_ring_buf;           //���ν��ջ�����.

    struct semaphore_LCB *send_buf_semp;     //���ͻ�������
    struct semaphore_LCB *recv_buf_semp;     //���ջ�������

    uint16_t evtt_right_write;      //�豸���ֱ�д�󵯳����¼�����
    uint16_t evtt_left_write;       //�豸���ֱ�д�󵯳����¼�����
    uint16_t recv_trigger_level;    //���ڽ��մ���ˮƽ,���յ���������ʱ�����¼�
    uint16_t timeout;               //δ������ˮƽ,�����趨ʱ����û���յ�����,
                                    //Ҳ�����¼�.
    uint16_t send_trigger_level;    //����ͬ������ˮƽ�������ͻ��λ�����������
                                    //��send_buf_semp������ǰ�̣߳�ֱ�����໺��
                                    //������������send_trigger_levelʱ�ͷ�֮��
    struct hard_reg_uart volatile *my_reg;
    uint32_t baud;
    uint32_t rx_int_line,tx_int_line;    //���ͺͽ����жϺ�
    ufast_t status;
    ufast_t serial_no;
};
*/
//���ڴ�������
enum uart_error_no
{
    enum_recv_fifo_over,         //����Ӳ��fifo���
    enum_recv_buf_over,          //���ջ��������
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
    uint8_t  reserve;   //UTXH��URXH���ֽڷ��ʼĴ���,reserve��ռλ����.
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


//�����豸���������������uart_ctrl������
enum uart_ctrl_no
{
    enum_uart_connect_recv_evtt,  //�豸��������д������ݳ����������ˮƽʱ����
                                  //�������¼�
    enum_uart_disconnect_recv_evtt, //�Ͽ������¼�
    enum_uart_connect_send_evtt,  //�豸��������д�������ʱ�����������¼�
    enum_uart_disconnect_send_evtt, //�Ͽ������¼�
    enum_uart_start,              //��������
    enum_uart_stop,               //ֹͣ����
    enum_uart_set_speed,          //����uartBaud.
    enum_uart_send_data,          //��������
    enum_uart_recv_data,          //��������
    enum_uart_completed_send,     //��ɷ��͹���
    enum_uart_rx_pause,           //��ͣ�������ݵ�������
    enum_uart_rx_resume,          //���¿�ʼ�������ݵ�������
    enum_uart_overtime,           //���ý��ճ�ʱʱ�䣬������
    enum_uart_recv_soft_level,    //���ý����������������ˮƽ
    enum_uart_send_soft_level,    //���ý����������������ˮƽ
    enum_uart_recv_hard_level,    //���ý���fifo����ˮƽ
    enum_uart_send_hard_level,    //���÷���fifo����ˮƽ
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

