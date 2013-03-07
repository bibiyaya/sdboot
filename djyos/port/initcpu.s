@----------------------------------------------------
@Copyright (C), 2005-2008,  lst.
@��Ȩ����  (C), 2005-2008,   lst.
@����ģ��:  CPU��ʼ��
@���ߣ�     lst
@�汾��     V1.0.0
@��ʼ�汾������ڣ�2009-02-05
@�ļ�����:  CPU��ʼ�������û������ʵ�ֵĲ���
@����˵��:  ��
@�޶���ʷ:
@2. ...
@1. ����: 2009-06-10
@   ����: lst
@   �°汾�ţ�V1.0.0
@   �޸�˵��: s3c6410��ԭʼ�汾
@------------------------------------------------------
#include "s3c6410.h"
    .include "Option.s"

    .equ    USERMODE,           0x10
    .equ    SYSMODE,            0x1f
    .equ    FIQMODE,            0x11
    .equ    IRQMODE,            0x12
    .equ    SVCMODE,            0x13
    .equ    ABORTMODE,          0x17
    .equ    UNDEFMODE,          0x1b
    .equ    MODEMASK,           0x1f
    .equ    NOINT,              0xc0

    .equ    R1_iA,              (1<<31)
    .equ    R1_nF,              (1<<30)
    .equ    R1_VE,              (1<<24)
    .equ    R1_I,               (1<<12)
    .equ    R1_BP,              (1<<11)    @ Z bit
    .equ    R1_C,               (1<<2)
    .equ    R1_A,               (1<<1)
    .equ    R1_M,               (1<<0)

    .equ    Peripheral_base,    0x70000000
    .equ    vic0_intenclear,    0x71200014
    .equ    vic1_intenclear,    0x71200014
    .equ    wtcon,              0x7e004000
    .equ    others,             0x7e00f900

    .equ    apll_lock,          0x7e00f000
    .equ    mpll_lock,          0x7e00f004
    .equ    apll_con,           0x7e00f00c
    .equ    mpll_con,           0x7e00f010
    .equ    epll_con0,          0x7e00f014
    .equ    epll_con1,          0x7e00f018

    .equ    clk_src,            0x7e00f01c
    .equ    clk_div0,           0x7e00f020
    .equ    clk_out,            0x7e00f02c
    .equ    mem_sys_cfg,        0x7e00f120
    .equ    others,             0x7e00f900
    .equ    rst_stat,           0x7E00F904
    .equ    inform0,            0x7E00FA00
    .equ    inform1,            0x7E00FA04
    .equ    inform2,            0x7E00FA08
    .equ    inform3,            0x7E00FA0C

    .equ    vpwr_cfg,           0xB2A0F804
    .equ    vsleep_cfg,         0xB2A0F818
    .equ    vosc_stable,        0xB2A0F824
    .equ    vpwr_stable,        0xB2A0F828
    .equ    vrst_stat,          0xB2A0F904
    .equ    vinform0,           0xB2A0FA00
    .equ    vinform1,           0xB2A0FA04
    .equ    vinform2,           0xB2A0FA08
    .equ    vinform3,           0xB2A0FA0C

    .global  _start

_start:
    mrs     r0,cpsr                 @ȡCPSR
    bic     r0,r0,#MODEMASK         @��ģʽλ
    orr     r1,r0,#SVCMODE|NOINT    @����Ϊ����̬������ֹ�ж�
    msr     cpsr_cxsf,r1            @�л�������̬,�ɷ�ֹ���ⷵ��0��ַʱ����.
    ldr     sp,=0xc002000           @������ʱջָ�룬���ڴ�����úú󣬽��������ڴ�

    mov		r0, #0
    mcr		p15, 0, r0, c7, c7, 0   @ Invalidate Entire I&D Cache

    mrc		p15, 0, r0, c1, c0, 0
    orr		r0, r0, #R1_I
    mcr		p15, 0, r0, c1, c0, 0    @Enable I Cache

@ Peripheral Port Setup,Base Addres : 0x70000000, Size : 256 MB (0x13)
    ldr     r0, =(Peripheral_base+0x13)
    mcr     p15,0,r0,c15,c2,4

@ Interrupt Disable
    ldr     r0, =vic0_intenclear
    ldr     r1, =0xFFFFFFFF;
    str     r1, [r0]

    ldr     r0, =vic0_intenclear
    ldr     r1, =0xFFFFFFFF;
    str     r1, [r0]

    ldr     r0, =wtcon      @ Disable WatchDog Timer
    ldr     r1, =0x0
    str     r1, [r0]

@ Set to Synchronous Mode
    ldr     r0, =others
    ldr     r1, [r0]
    orr     r1, r1, #0x40    @ SyncMUX = Sync
    str     r1, [r0]

    nop
    nop
    nop
    nop
    nop

    ldr     r1, [r0]
    orr     r1, r1, #0x80    @ SyncReq = Sync
    str     r1, [r0]

WaitForSync:
    ldr     r1, [r0]         @ Read OTHERS
    and     r1, r1, #0xF00   @ Wait SYNCMODEACK = 0xF
    cmp     r1, #0xF00
    bne     WaitForSync

@ Check PLL and CLKDIV
    ldr     r3, =0x83FF3F07          @ Mask for APLL_CON/MPLL_CON
    ldr     r4, =0x80FF3F07          @ Mask for EPLL_CON0
    ldr     r5, =0x0000FFFF          @ Mask for EPLL_CON1
    ldr     r6, =0x0003FF17          @ Mask for CLKDIV0

    ldr     r0, =apll_con          @ Check APLL
    ldr     r1, [r0]
    and     r1, r1, r3
    ldr     r2, =((1<<31)+(apll_mdiv<<16)+(apll_pdiv<<8)+(apll_sdiv))  @ APLL_CON value to configure
    cmp     r1, r2
    bne     PLL_NeedToConfigure

    ldr     r0, =mpll_con          @ Check MPLL
    ldr     r1, [r0]
    and     r1, r1, r3
    ldr     r2, =((1<<31)+(mpll_mdiv<<16)+(mpll_pdiv<<8)+(mpll_sdiv))  @ MPLL_CON value to configure
    cmp     r1, r2
    bne     PLL_NeedToConfigure

    ldr     r0, =epll_con0          @ Check EPLL_CON0
    ldr     r1, [r0]
    and     r1, r1, r4
    ldr     r2, =((1<<31)+(epll_mdiv<<16)+(epll_pdiv<<8)+(epll_sdiv))  @ EPLL_CON0 value to configure
    cmp     r1, r2
    bne     PLL_NeedToConfigure

    ldr     r0, =epll_con1          @ Check EPLL_CON1
    ldr     r1, [r0]
    and     r1, r1, r5
    ldr     r2, =epll_kdiv          @ EPLL_CON1 value to configure
    cmp     r1, r2
    bne     PLL_NeedToConfigure

    ldr     r0, =clk_div0          @ Check CLKDIV0
    ldr     r1, [r0]
    and     r1, r1, r6

@ CLKDIV0 value to configure
    ldr     r2, =((pclk_div<<12)+(hclkx2_div<<9)+(hclk_div<<8)+(mpll_div<<4)+(apll_div<<0))

    cmp     r1, r2
    bne     CLKDIV_NeedToConfigure

    b       PLL_CLKDIV_AlreadyConfigured  @ APLL/MPLL/EPLL and CLKDIV0 is already configured

PLL_NeedToConfigure:
    ldr     r0, =clk_src
    ldr     r1, [r0]
    bic     r1, r1, #0x7       @ select FIN out,Disable PLL Clock Out
    str     r1, [r0]

    ldr     r0, =clk_div0
    ldr     r1, [r0]
    bic     r1, r1, #0xff00
    bic     r1, r1, #0xff
    ldr     r2, = ((pclk_div<<12)+(hclkx2_div<<9)+(hclk_div<<8)+(mpll_div<<4)+(apll_div<<0))
    orr     r1, r1, r2
    str     r1, [r0]

@ change PLL value
    ldr     r1, =0x4B1      @ Lock Time : 0x4b1 (100us @Fin12MHz) for APLL/MPLL
    ldr     r2, =0xE13      @ Lock Time : 0xe13 (300us @Fin12MHz) for EPLL

    ldr     r0, =apll_lock
    str     r1, [r0]        @ APLL Lock Time
    str     r1, [r0, #0x4]  @ MPLL Lock Time
    str     r2, [r0, #0x8]  @ EPLL Lock Time

    ldr     r0, =apll_con
    ldr     r1, =((1<<31)+(apll_mdiv<<16)+(apll_pdiv<<8)+(apll_sdiv))
    str     r1, [r0]

    ldr     r0, =mpll_con
    ldr     r1, =((1<<31)+(mpll_mdiv<<16)+(mpll_pdiv<<8)+(mpll_sdiv))
    str     r1, [r0]

    ldr     r0, =epll_con1
    ldr     r1, =epll_kdiv
    str     r1, [r0]

    ldr     r0, =epll_con0
    ldr     r1, =((1<<31)+(epll_mdiv<<16)+(epll_pdiv<<8)+(epll_sdiv))
    str     r1, [r0]

@ Set System Clock Divider
CLKDIV_NeedToConfigure:
    ldr     r0, =clk_div0
    ldr     r1, [r0]
    bic     r1, r1, #0x30000
    bic     r1, r1, #0xff00
    bic     r1, r1, #0xff
    ldr     r2, =((pclk_div<<12)+(hclkx2_div<<9)+(hclk_div<<8)+(mpll_div<<4)+(apll_div<<0))
    orr     r1, r1, r2
    str     r1, [r0]

@ Enable PLL Clock Out
    ldr     r0, =clk_src
    ldr     r1, [r0]
    orr     r1, r1, #0x7    @ PLL  Clockout
    str     r1, [r0]        @ System will be waiting for PLL unlocked after this instruction

PLL_CLKDIV_AlreadyConfigured: @��ʼ��norflash�ͺ�sdramm

    @.extern srom_cs0_init
    .extern DRAM_Init
    .extern nand_init
    @bl      srom_cs0_init
    bl      dram_init
    bl      nand_init

    ldr     r0, =mem_sys_cfg
    ldr     r1, [r0]
    orr     r1,r1,#0x1000       @set norflash bus_width = 16
    bic     r1, r1, #0xbf		@DDR Port1:32bit,cs0��1��4��5:sromc��cs2��3:nfcon
    str     r1, [r0]

    mrc     p15,0,r0,c1,c0,0
    bic     r0,r0,#R1_VE			@ Disable VIC
    mcr     p15,0,r0,c1,c0,0

@���³�ʼ��L1ҳ�?ƽ��ʽȫӳ��
    ldr     r0,=0x50000000  @L1ҳ���ַ
    mov     r1,#0
    ldr     r3,=0xc12   @����cache
loopnommu:
    mov     r2,r1,lsl #20   @Ŀ��α��д��L1��Ŀ�ĸ�12λ
    add     r2,r2,r3
    str     r2,[r0],#4
    add     r1,r1,#1
    cmp     r1,#0x500  @0~2fffffff��cache������д����
    bne     loopnommu

    ldr     r3,=0xc1e   @��cache
loopmmu:
    mov     r2,r1,lsl #20   @Ŀ��α��д��L1��Ŀ�ĸ�12λ
    add     r2,r2,r3
    str     r2,[r0],#4
    add     r1,r1,#1
    cmp     r1,#0x580   @128M sdram��
    bne     loopmmu

    ldr     r3,=0xc12   @����cache
loopnommu1:
    mov     r2,r1,lsl #20   @Ŀ��α��д��L1��Ŀ�ĸ�12λ
    add     r2,r2,r3
    str     r2,[r0],#4
    add     r1,r1,#1
    cmp     r1,#0x1000  @�߶˵�ַ������cache
    bne     loopnommu1
    ldr     r0,=0x50000000
    mcr     p15,0,r0,c2,c0,0    @ҳ����ַ
    ldr     r0,=0xffffffff      @ȫ������й�����Ȩ��
    mcr     p15,0,r0,c3,c0,0    @д��Ĵ���

@flash led
    ldr     r0,=0x7f008820
    ldr     r1,=0x1111
    str     r1,[r0]

    .extern main
    bl      main

    ldr     r0,=0x10000000
    mov     pc,r0
@    ldr     r0,=0x7f008824
@    ldr     r1,[r0]
@    bic     r2,r1,#3
@    orr     r2,r2,#0xc
@    bic     r3,r1,#0xc
@    orr     r3,r3,#3
@nn:
@    str     r2,[r0]
@    ldr     r4,=10000000
@delay1:
@    sub     r4,r4,#1
@    cmp     r4,#0
@    bne     delay1
@    str     r3,[r0]
@    ldr     r4,=40000000
@delay2:
@    sub     r4,r4,#1
@    cmp     r4,#0
@    bne     delay2
@    b       nn


