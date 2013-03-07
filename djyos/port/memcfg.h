//----------------------------------------------------
//Copyright (C), 2009-2009,  lst.
//版权所有 (C), 2009-2009,   lst.
//所属模块: cpu的boot_rom模块
//作者：lst
//版本：V1.0.0
//文件描述:s3c6410的cpu及总线控制器的硬件参数定义
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:
//       作者:
//       新版本号：
//       修改说明: 原始版本
//------------------------------------------------------
#ifndef __DMC_H__
#define __DMC_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"

struct dram_reg
{
    uint32_t MEMSTAT;       // 0x7E001000 (DMC1 Case)
    uint32_t MEMCCMD;       // 0x7E001004
    uint32_t DIRECTCMD;     // 0x7E001008
    uint32_t MEMCFG;        // 0x7E00100C
    uint32_t REFRESH;       // 0x7E001010
    uint32_t CASLAT;        // 0x7E001014
    uint32_t T_DQSS;        // 0x7E001018
    uint32_t T_MRD;         // 0x7E00101C
    uint32_t T_RAS;         // 0x7E001020
    uint32_t T_RC;          // 0x7E001024
    uint32_t T_RCD;         // 0x7E001028
    uint32_t T_RFC;         // 0x7E00102C
    uint32_t T_RP;          // 0x7E001030
    uint32_t T_RRD;         // 0x7E001034
    uint32_t T_WR;          // 0x7E001038
    uint32_t T_WTR;         // 0x7E00103C
    uint32_t T_XP;          // 0x7E001040
    uint32_t T_XSR;         // 0x7E001044
    uint32_t T_ESR;         // 0x7E001048
    uint32_t MEMCFG2;       // 0x7E00104C
    uint32_t reserved1[44];
    uint32_t ID_0_CFG;      // 0x7E001100
    uint32_t ID_1_CFG;      // 0x7E001104
    uint32_t ID_2_CFG;      // 0x7E001108
    uint32_t ID_3_CFG;      // 0x7E00110C
    uint32_t ID_4_CFG;      // 0x7E001110
    uint32_t ID_5_CFG;      // 0x7E001104
    uint32_t ID_6_CFG;      // 0x7E001108
    uint32_t ID_7_CFG;      // 0x7E00110C
    uint32_t ID_8_CFG;      // 0x7E001120
    uint32_t ID_9_CFG;      // 0x7E001124
    uint32_t ID_10_CFG;     // 0x7E001128
    uint32_t ID_11_CFG;     // 0x7E00112C
    uint32_t ID_12_CFG;     // 0x7E001130
    uint32_t ID_13_CFG;     // 0x7E001134
    uint32_t ID_14_CFG;     // 0x7E001138
    uint32_t ID_15_CFG;     // 0x7E00113C
    uint32_t reserved2[48];
    uint32_t CHIP_0_CFG;    // 0x7E001200
    uint32_t CHIP_1_CFG;    // 0x7E001204
    uint32_t reserved3[62];
    uint32_t USER_STAT;     // 0x7E001300
    uint32_t USER_CFG;      // 0x7E001304
};

struct srom_reg
{
	uint32_t SROM_BW;		// 0x70000000
	uint32_t SROM_BC0;
	uint32_t SROM_BC1;
	uint32_t SROM_BC2;
	uint32_t SROM_BC3;
	uint32_t SROM_BC4;
	uint32_t SROM_BC5;

};

void srom_cs0_init(void);
void    dram_init(void );


#ifdef __cplusplus
}
#endif

#endif //__DMC_H__



