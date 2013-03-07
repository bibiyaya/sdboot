//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:堆管理模块
//作者：lst
//版本：V1.0.0
//文件描述: 内核配置头文件
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef _PORT_CONFIG_H_
#define _PORT_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//与硬件相关的配置
#define M   1000000
#define cn_mclk         (266*M)         //m时钟
#define cn_hclkx2       (266*M)         //DDR时钟
#define cn_hclk         (133*M)         //高速外设总线(ahb)时钟=133M
#define cn_pclk         66500000        //低速外设总线(apb)时钟=66.5M
#define cn_eclk         84666667        //外部时钟=84.67M
#define cn_timer_clk    cn_pclk         //定时器时钟源

//存储器大小端的配置
#define cn_little_endian       0
#define cn_big_endian          1
#define cn_cpu_endian          cn_little_endian

//与CPU字长相关的配置
#define cn_cpu_bits     32  //处理器字长
#define cn_point_bit    32  //CPU地址位数，移植关键
#define cn_cpu_bits_suffix_zero 5
#define cn_byte_wide    8   //字节位宽

typedef unsigned  long long uint64_t;
typedef signed    long long sint64_t;
typedef unsigned  int       uint32_t;
typedef unsigned  short     uint16_t;
typedef           int       sint32_t;
typedef short     int       sint16_t;
typedef unsigned  char      uint8_t;
typedef signed    char      sint8_t;
//ptu32_t类型与cpu的寻址范围有关，也与CPU的结构有关，如果该系统中指针长度小于或
//等于32位，则ptu32_t被定义为32位，否则与指针等长，它的用途有二:
//1.用于内存分配的指针限定,或者用于描述内存长度。
//2.用于可能需要转换成指针使用的整形数据，比如struct pan_device结构中的
//  ptu32_t  private_tag
//一般来说ptu32_t与ucpu_t相同,但也有意外,尤其是8位机和16位机中,例如51机的ucpu_t
//是8位的,但寻址范围是16位的,ptu32_t=16位.80c296单片机的ucpu_t是16位的，寻址范围
//是24位的，ptu32_t=32位。
typedef uint32_t            ptu32_t;

//以下是CPU可以用一条指令处理的数据类型，多用于需要保持数据完整性的操作，以及
//需要快速的操作。
//数据完整性和原子操作的异同:
//数据完整性是指在任何时候都能读到正确值的数据，比如下列语句:
//  uint32_t    b32;
//  b32++;  //b32的原值是0x0000ffff
//在32位risc机上，执行过程为：
//取b32地址到寄存器0-->取b32的数值到寄存器1-->寄存器1加1-->把寄存器1写入b32.
//上述过程无论在哪里被中断或者被高优先级线程打断，在中断或高优先级线程中读b32，
//要么得到0x0000ffff，要么得到0x00010000,这两都可以认为是正确数据.
//而在16位机上，执行过程是
//1、取0x0000到寄存器r0，取0xffff到寄存器r1
//2、执行加操作，使r0=0x0001,r1=0x0000
//3、把r0写入b32的高16位。
//4、把r1写入b32的低16位。
//如果在第3和第4步骤之间被中断打断，在中断里读b32，将得到错误的0x00001ffff。
//那么原子操作呢?就是整个执行过程不被任何中断或者高优先级线程打断，还看上面代码，
//如果b32被定义为原子变量，则无论在16位还是32位机上，b32都可以得到正确的值。目
//前，djyos并不支持原子变量，只能使用互斥量实现原子变量的效果。
typedef uint32_t        ucpu_t;
    #define cn_limit_ucpu   cn_limit_uint32
    #define cn_min_ucpu     (0)
typedef sint32_t        scpu_t;
    #define cn_limit_scpu   cn_limit_sint32
    #define cn_min_scpu     cn_min_sint32
//以下是快速数据类型，一般来说，CPU类型是快速数据类型，但有些系统存储器宽度小于
//CPU字长，导致从存储器读一个cpu字长的数需要多个存储器周期，另外,16位和32位cpu
//中,字节操作也并不一定最快.对于有些对字长没有要求的数据类型来说，使用与存储器
//宽度等长的变量也许是最快速的。配置16位SDRAM的44b0x快速类型是16位的.
typedef uint16_t        ufast_t;
    #define cn_limit_ufast  cn_limit_uint16
    #define cn_min_ufast    (0)
typedef sint16_t        sfast_t;
    #define cn_limit_sfast  cn_limit_sint16
    #define cn_min_sfast    cn_min_sint16

typedef ufast_t    bool_t;
#define true    1
#define false   0

//以下定义tick参数
#define cn_tick_ms      1       //操作系统内核时钟脉冲长度，以毫秒为单位。
#define cn_tick_hz      1000    //内核时钟频率，单位为hz。
#define cn_fine_us      1       //操作系统内核精密时钟脉冲长度，以微秒为单位。
                                //它表示从上一次tick脉冲开始至今流逝的时间量。
#define cn_fine_hz      1000000 //内核精密时钟频率，是cn_fine_us的倒数。

//下面定义cpu系统的栈类型
#define cn_full_down_stack  0   //向下生长的满栈
#define cn_empty_down_stack 1   //向下生长的空栈
#define cn_full_up_stack    2   //向上生长的满栈
#define cn_empty_up_stack   3   //向上生长的空栈
#define cn_stack_type       cn_full_down_stack

//设置是否支持mmu，在不支持mmu的cpu上，必须设为false，在支持mmu的机器上，则看
//软件是否打算使用mmu进行动态内存映射，是则设为true，否则设为false。
//此常数与动态内存分配算法有关。
#define cn_mmu          false       //44b0x不支持mmu
#define cn_phy_mem      0x800000    //物理内存总量，支持mmu才有意义。

//cpu运行模式定义
#define cn_mode_si          0   //单映像模式
#define cn_mode_dlsp        1   //动态加载单进程模式
#define cn_mode_mp          2   //多进程模式，只有mmu=true才可选择此模式
#define cn_run_mode         cn_mode_si

#define cn_device_limit     20     //定义设备数量。
#define cn_handle_limit     20     //定义设备句柄数量。
#define cn_locks_limit      20     //定义程序中可用的自由锁的数量。自由锁指用户
                //调用semp_creat和mutex_create创建的锁，不包括嵌入到设备中的锁。
#define cn_events_limit     20     //事件数量
#define cn_evtts_limit      16     //总事件类型数
#define cn_wdt_limit        5      //允许养狗数量

//与内存管理相关的常量
#define cn_page_size        0x1000      //基页尺寸为4Kbytes
#define cn_page_size_suffix_zero    12  //页尺寸后缀0的个数
#define cn_block_limit      0x400000    //最大块尺寸
#define cn_mem_recycle      true        //支持内存回收----配置工具
#define cn_mem_pools        (10)        //允许建立10个内存池

//可同时打开的文件数量，注意多级路径的每一级都算一个文件，但相同的路径不重复算
#define cn_opened_fsnode_limit   16

//代理内核栈的深度计算方法:栈需求最深的那个系统服务+sizeof(struct thread_vm)
//djyos for 44b0x移植版本中固定设为8Kbytes，
#define cn_kernel_stack     0x2000 //代理内核栈，线程需要为系统服务额外增加的栈

//定义一些与对齐相关的宏
#define mem_align           1           //如果目标系统没有对齐要求，改为0

#define align_down_2(x)     ((x)&(~1))        //2字节,向下对齐,
#define align_down_4(x)     ((x)&(~3))        //4字节,向下对齐,
#define align_down_8(x)     ((x)&(~7))        //8字节,向下对齐,
#define align_down_16(x)    ((x)&(~15))       //16字节,向下对齐,
#define align_up_2(x)       (((x)+1)&(~1))    //2字节,向上对齐,
#define align_up_4(x)       (((x)+3)&(~3))    //4字节,向上对齐,
#define align_up_8(x)       (((x)+7)&(~7))    //8字节,向上对齐,
#define align_up_16(x)      (((x)+15)&(~15))  //16字节,向上对齐,

//以下声明本系统的数据对齐方式，移植关键
#define align_down(x)       align_down_8(x)   //gcc-arm要求8字节对齐
#define align_up(x)         align_up_8(x)     //gcc-arm要求8字节对齐

//跟中断相关的配置
#define cn_int_num  26  //44b0x有26个中断源.

//组件配置
#define cfg_djyfs           1   //是否包含文件系统
#define cfg_djyfs_flash     1   //是否包含flash文件系统的驱动
#define cfg_djyfs_ram       0   //是否包含ram文件系统

#ifdef __cplusplus
}
#endif
#endif // _PORT_CONFIG_H_


