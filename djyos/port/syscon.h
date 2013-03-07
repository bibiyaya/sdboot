#ifndef __S3C6410_SYSCON_H
#define __S3C6410_SYSCON_H

#if __cplusplus
extern "C" {
#endif

//base address = 0x7e00f000
struct syscon_reg
{
    uint32_t APLL_LOCK;        // 0x00
    uint32_t MPLL_LOCK;        // 0x04
    uint32_t EPLL_LOCK;        // 0x08
    uint32_t APLL_CON;         // 0x0c

    uint32_t MPLL_CON;         // 0x10
    uint32_t EPLL_CON0;        // 0x14
    uint32_t EPLL_CON1;        // 0x18
    uint32_t CLK_SRC;          // 0x1c

    uint32_t CLK_DIV0;        // 0x20
    uint32_t CLK_DIV1;        // 0x24
    uint32_t CLK_DIV2;        // 0x28
    uint32_t CLK_OUT;            // 0x2c

    uint32_t HCLK_GATE;        // 0x30
    uint32_t PCLK_GATE;        // 0x34
    uint32_t SCLK_GATE;        // 0x38
    uint32_t PAD0;            // 0x3c

    uint32_t PAD1[48];            // 0x40~0xff

    uint32_t AHB_CON0;        // 0x100
    uint32_t AHB_CON1;        // 0x104
    uint32_t AHB_CON2;        // 0x108
    uint32_t PAD2;            // 0x10c

    uint32_t SDMA_SEL;        // 0x110
    uint32_t SW_RST;            // 0x114
    uint32_t SYS_ID;            // 0x118
    uint32_t PAD3;            // 0x11c

    uint32_t MEM_SYS_CFG;    // 0x120
    uint32_t QOS_OVERRIDE0;    // 0x124
    uint32_t QOS_OVERRIDE1;    // 0x128
    uint32_t MEM_CFG_STAT;    // 0x12c

    uint32_t PAD4[436];        // 0x130~0x7ff

    uint32_t PAD5;            // 0x800
    uint32_t PWR_CFG;        // 0x804
    uint32_t EINT_MASK;        // 0x808
    uint32_t PAD6;            // 0x80c

    uint32_t NORMAL_CFG;      // 0x810
    uint32_t STOP_CFG;        // 0x814
    uint32_t SLEEP_CFG;        // 0x818
    uint32_t PAD7;            // 0x81c

    uint32_t OSC_FREQ;        // 0x820
    uint32_t OSC_STABLE;        // 0x824
    uint32_t PWR_STABLE;        // 0x828
    uint32_t FPC_STABLE;        // 0x82c

    uint32_t MTC_STABLE;        // 0x830
    uint32_t PAD8[3];            // 0x834~0x83f

    uint32_t PAD9[48];            // 0x840~0x8ff

    uint32_t OTHERS;            // 0x900
    uint32_t RST_STAT;        // 0x904
    uint32_t WAKEUP_STAT;    // 0x908
    uint32_t BLK_PWR_STAT;    // 0x90c

    uint32_t PAD10[60];        // 0x910~0x9ff

    uint32_t INFORM0;            // 0xa00
    uint32_t INFORM1;            // 0xa04
    uint32_t INFORM2;            // 0xa08
    uint32_t INFORM3;            // 0xa0c

    uint32_t INFORM4;            // 0xa10
    uint32_t INFORM5;            // 0xa14
    uint32_t INFORM6;            // 0xa18
    uint32_t INFORM7;            // 0xa1c
};

//------------------------------------------------------------------------------

#if __cplusplus
    }
#endif

#endif    // __S3C6410_SYSCON_H

