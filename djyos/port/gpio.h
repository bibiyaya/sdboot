#ifndef __S3C6410_GPIO_H
#define __S3C6410_GPIO_H

#if __cplusplus
extern "C" {
#endif

//base address = 0x7f008000
struct hard_reg_gpio
{
    uint32_t GPACON;        // 000
    uint32_t GPADAT;        // 004
    uint32_t GPAPUD;        // 008
    uint32_t GPACONSLP;    // 00c

    uint32_t GPAPUDSLP;    // 010
    uint32_t PAD1[3];        // 014~01f

    uint32_t GPBCON;        // 020
    uint32_t GPBDAT;        // 024
    uint32_t GPBPUD;        // 028
    uint32_t GPBCONSLP;    // 02c

    uint32_t GPBPUDSLP;    // 030
    uint32_t PAD2[3];        // 034~03f

    uint32_t GPCCON;        // 040
    uint32_t GPCDAT;        // 044
    uint32_t GPCPUD;        // 048
    uint32_t GPCCONSLP;    // 04c

    uint32_t GPCPUDSLP;    // 050
    uint32_t PAD3[3];        // 054~05f

    uint32_t GPDCON;        // 060
    uint32_t GPDDAT;        // 064
    uint32_t GPDPUD;        // 068
    uint32_t GPDCONSLP;    // 06c

    uint32_t GPDPUDSLP;    // 070
    uint32_t PAD4[3];        // 074~07f

    uint32_t GPECON;        // 080
    uint32_t GPEDAT;        // 084
    uint32_t GPEPUD;        // 088
    uint32_t GPECONSLP;    // 08c

    uint32_t GPEPUDSLP;    // 090
    uint32_t PAD5[3];        // 094~09f

    uint32_t GPFCON;        // 0a0
    uint32_t GPFDAT;        // 0a4
    uint32_t GPFPUD;        // 0a8
    uint32_t GPFCONSLP;    // 0ac

    uint32_t GPFPUDSLP;    // 0b0
    uint32_t PAD6[3];        // 0b4~0bf

    uint32_t GPGCON;        // 0c0
    uint32_t GPGDAT;        // 0c4
    uint32_t GPGPUD;        // 0c8
    uint32_t GPGCONSLP;    // 0cc

    uint32_t GPGPUDSLP;    // 0d0
    uint32_t PAD7[3];        // 0d4~0df

    uint32_t GPHCON0;        // 0e0
    uint32_t GPHCON1;        // 0e4
    uint32_t GPHDAT;        // 0e8
    uint32_t GPHPUD;        // 0ec

    uint32_t GPHCONSLP;    // 0f0
    uint32_t GPHPUDSLP;    // 0f4
    uint32_t PAD8[2];        // 0f8~0ff

    uint32_t GPICON;        // 100
    uint32_t GPIDAT;        // 104
    uint32_t GPIPUD;        // 108
    uint32_t GPICONSLP;    // 10c

    uint32_t GPIPUDSLP;    // 110
    uint32_t PAD9[3];        // 114~11f

    uint32_t GPJCON;        // 120
    uint32_t GPJDAT;        // 124
    uint32_t GPJPUD;        // 128
    uint32_t GPJCONSLP;    // 12c

    uint32_t GPJPUDSLP;    // 130
    uint32_t PAD10[3];        // 134~13f

    // GPK, GPL, GPM, GPN are Below

    uint32_t GPOCON;        // 140
    uint32_t GPODAT;        // 144
    uint32_t GPOPUD;        // 148
    uint32_t GPOCONSLP;    // 14c

    uint32_t GPOPUDSLP;    // 150
    uint32_t PAD11[3];        // 154~15f

    uint32_t GPPCON;        // 160
    uint32_t GPPDAT;        // 164
    uint32_t GPPPUD;        // 168
    uint32_t GPPCONSLP;    // 16c

    uint32_t GPPPUDSLP;    // 170
    uint32_t PAD12[3];        // 174~17f

    uint32_t GPQCON;        // 180
    uint32_t GPQDAT;        // 184
    uint32_t GPQPUD;        // 188
    uint32_t GPQCONSLP;    // 18c

    uint32_t GPQPUDSLP;    // 190
    uint32_t PAD13[3];        // 194~19f

    uint32_t SPCON;        // 1a0
    uint32_t PAD14[3];        // 1a4~1af

    uint32_t MEM0CONSTOP;    // 1b0
    uint32_t MEM1CONSTOP;    // 1b4
    uint32_t PAD15[2];            // 1b8~1bf

    uint32_t MEM0CONSLP0;    // 1c0
    uint32_t MEM0CONSLP1;    // 1c4
    uint32_t MEM1CONSLP;        // 1c8
    uint32_t PAD16;            // 1cc

    uint32_t MEM0DRVCON;        // 1d0
    uint32_t MEM1DRVCON;        // 1d4
    uint32_t PAD17[2];            // 1d8~1df

    uint32_t PAD18[8];            // 1e0~1ff

    uint32_t EINT12CON;        // 200
    uint32_t EINT34CON;        // 204
    uint32_t EINT56CON;        // 208
    uint32_t EINT78CON;        // 20c

    uint32_t EINT9CON;        // 210
    uint32_t PAD19[3];            // 214~21f

    uint32_t EINT12FLTCON;    // 220
    uint32_t EINT34FLTCON;    // 224
    uint32_t EINT56FLTCON;    // 228
    uint32_t EINT78FLTCON;    // 22c

    uint32_t EINT9FLTCON;        // 230
    uint32_t PAD20[3];            // 234~23f

    uint32_t EINT12MASK;        // 240
    uint32_t EINT34MASK;        // 244
    uint32_t EINT56MASK;        // 248
    uint32_t EINT78MASK;        // 24c

    uint32_t EINT9MASK;        // 250
    uint32_t PAD21[3];            // 254~25f

    uint32_t EINT12PEND;        // 260
    uint32_t EINT34PEND;        // 264
    uint32_t EINT56PEND;        // 268
    uint32_t EINT78PEND;        // 26c

    uint32_t EINT9PEND;        // 270
    uint32_t PAD22[3];            // 274~27f

    uint32_t PRIORITY;        // 280
    uint32_t SERVICE;            // 284
    uint32_t SERVICEPEND;        // 288
    uint32_t PAD23;            // 28f

    uint32_t PAD24[348];        // 290~7ff

    uint32_t GPKCON0;            // 800
    uint32_t GPKCON1;            // 804
    uint32_t GPKDAT;            // 808
    uint32_t GPKPUD;            // 80c

    uint32_t GPLCON0;            // 810
    uint32_t GPLCON1;            // 814
    uint32_t GPLDAT;            // 818
    uint32_t GPLPUD;            // 81c

    uint32_t GPMCON;            // 820
    uint32_t GPMDAT;            // 824
    uint32_t GPMPUD;            // 828
    uint32_t PAD25;            // 82f

    uint32_t GPNCON;            // 830
    uint32_t GPNDAT;            // 834
    uint32_t GPNPUD;            // 838
    uint32_t PAD26;            // 83f

    uint32_t PAD27[16];        // 840~87f

    uint32_t SPCONSLP;        // 880
    uint32_t PAD28[3];            // 884~88f

    uint32_t PAD29[28];        // 890~8ff

    uint32_t EINT0CON0;        // 900
    uint32_t EINT0CON1;        // 904
    uint32_t PAD30[2];            // 908~90f

    uint32_t EINT0FLTCON0;    // 910
    uint32_t EINT0FLTCON1;    // 914
    uint32_t EINT0FLTCON2;    // 918
    uint32_t EINT0FLTCON3;    // 91c

    uint32_t EINT0MASK;        // 920
    uint32_t EINT0PEND;        // 924
    uint32_t PAD31[2];            // 928~92f

    uint32_t SLPEN;            // 930
    uint32_t PAD32[3];            // 934~93f
} ;

void gpio_init(void);
//extern struct hard_reg_gpio volatile * const pg_gpio_reg;

#if __cplusplus
}
#endif

#endif // __S3C6410_GPIO_H

