#include "config.h"
#include "uart.h"
#include "gpio.h"
#include "nand.h"

#define SOH     1
#define EOT     4
#define ACK     6
#define NAK     0x15
#define CAN     0x18
#define ERR     0xff

//extern volatile struct hard_reg_uart *pg_uart_reg;
//extern volatile uint16_t *chip_addr;       // = (uint16_t *)flash_base_addr;
volatile struct hard_reg_gpio *pg_gpio_reg;
uint8_t get_packet( uint8_t *buf)
{
    uint32_t i;
    uint8_t pp;
    for(i = 0; i<1000000; i++)
    {
        if(read_byte(&pp))
            break;              // receive frame head
    }
    if(i == 1000000)            // not receive frame head
        return ERR;
    switch(pp)
    {
        case SOH:
        {
            scan_byte( );
            scan_byte( );
            for(i=0; i<128; i++)
                buf[i] = scan_byte( );
            scan_byte( );
            return SOH;
        }break;
        case CAN:
        {
            return CAN;
        }break;
        case EOT:
        {
            return EOT;
        }break;
        case 's':
            return 's';
        break;
        default:return ERR;
    }
}
int main(void)
{
    uint32_t i = 0;
    //bool_t eot=0;
    //uint8_t *dbuf = 0x50004000;  // address of ddr ram
    //uint8_t *ddr_write = 0x57e00000;
    //uint8_t *ddr_read = 0x50004000;
    //uint8_t pp = 0,cmd = NAK;
    //unsigned char buf[128];
    //unsigned char buf_w[2048];
    unsigned char buf_r[2048];
    uint8_t errorcode = 0;
    //pg_gpio_reg = (void*)0x7f008000;
    //chip_addr = (uint16_t *)flash_base_addr;

    module_init_uart0();
    printf_str("\n\rwaiting...");
/*
    while(1)
    {
        printf_byte(cmd);
        pp = get_packet(buf);
        if(pp == SOH)
        {
            for(i=0;i<128;i++)
            {
                dbuf[offset] = buf[i];
                offset ++;
            }
            cmd = ACK;
        }else if(pp == EOT)
        {
            printf_byte(ACK);
            break;
            cmd = ACK;
            if(eot >= 2)
                break;
            eot++;
        }else if(pp == 's')
        {
            printf_str("\n\rturn to norflash");
            return 0;
        }
    }
*/
    //printf_str("\n\rread date form sdram, address 0x50004000, len=128\n\r");

    for(i=0;i<2048;i++)
    {
        //ddr_write[i] = 'a';
        //buf_w[i] = 'k';
        buf_r[i] = 'b';
        if ( i < 10)
        {
            printf_byte(buf_r[i]);
        }
    }

    for(i=0;i<1000;i++);

    /* nand flash erase */
    // erase block 0
    printf_str("\r\nnand erase...");
    //nand_erase(0);
    for(i=0;i<1000;i++);

    /* nand flash write */
    printf_str("\r\nnand write...");
    //nand_write(0,buf_w,2048);
    for(i=0;i<1000;i++);

    /* nand flash read */
    printf_str("\r\nnand read...");
    nand_read(0,buf_r,2048);
    for(i=0;i<1000;i++);

    printf_str("\r\nread date from ddr");
    for(i=0;i<128;i++)
    {
        if( buf_r[i] != 'k')
        {
            errorcode += 1;            
            //break;
            printf_byte(buf_r[i]);
        }
    }

    if (errorcode != 0)
    {
        printf_str("\r\nnand test error...");
    }
    else
    {
        printf_str("\r\nnand test OK...");
    }

    while(1);

    /*
   //定义一个函数指针
   uint32_t gEntryPoint = 0x57E00000;
   static void (*APPEntry)(void);
   //将制定地址强制转换为不带参数不带返回值的值函数指针。
   APPEntry = (void (*)(void)) gEntryPoint;
   (*APPEntry)();
   */
   ((void(*)(void))0x57e00000)();
    return 0;
}
