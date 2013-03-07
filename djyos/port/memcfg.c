//----------------------------------------------------
//Copyright (C), 2009-2009,  lst.
//版权所有 (C), 2009-2009,   lst.
//所属模块: cpu的boot_rom模块
//作者：lst
//版本：V1.0.0
//文件描述:s3c6410的dram控制器相关代码
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:
//       作者:
//       新版本号：
//       修改说明: 原始版本
//------------------------------------------------------
#include "memcfg.h"

//片选0(norflash)参数定义
#define cn_cs0_pmc      0
#define cn_cs0_tacp     0//2
#define cn_cs0_tcah     0//2
#define cn_cs0_tcoh     0//2
#define cn_cs0_tacc     15
#define cn_cs0_tcos     0//3
#define cn_cs0_tcas     0//2

#define cn_cs0_bwth     1
#define cn_cs0_wait     0
#define cn_cs0_ublb     0

//mobile dram 参数定义
#define cn_DDR_REF      7800   // 7800ns
#define cn_DDR_RAS      45     // min. 45ns
#define cn_DDR_RC       68     // min. 67.5ns
#define cn_DDR_RCD      23     // min. 22.5ns
#define cn_DDR_RFC      80     // min. 80ns
#define cn_DDR_RP       23     // min. 22.5ns
#define cn_DDR_RRD      15     // min. 15ns
#define cn_DDR_WR       15     // min. 15ns
#define cn_DDR_XSR      120    // min 120ns
#define cn_AP_bit       0
#define cn_Row_bit      2      // 13bit
#define cn_Col_bit      2      // 10bit

#define cn_ALLEN        0
#define cn_StopEN       1      // Mem Clock is dynamically stopped

#define cn_DIS_AutoPD   0
#define cn_EN_AutoPD    1

static  volatile struct dram_reg *const pg_dram_reg = (void*)0x7E001000;
static  volatile struct srom_reg *const pg_srom_reg = (void*)0x70000000;

void srom_cs0_init(void)
{
    uint32_t temp;
    temp = pg_srom_reg->SROM_BW;
    temp &= 0xfffffff0;
    temp |= (cn_cs0_bwth<<0)+(cn_cs0_wait<<2)+(cn_cs0_ublb<<3);
    pg_srom_reg->SROM_BW = temp;
    pg_srom_reg->SROM_BC0 = (cn_cs0_tcas<<28) + (cn_cs0_tcos<<24)
                            +(cn_cs0_tacc<<16) + (cn_cs0_tcoh<<12)
                            +(cn_cs0_tcah<<8) + (cn_cs0_tacp<<4)
                            +(cn_cs0_pmc<<0);
}

void dram_init(void)
{
    uint32_t  Para_RCD, Scheduled_Para,uPara_RFC, uPara_RP,uPara_XSR;

    //Enter the Configuration Mode
    pg_dram_reg->MEMCCMD = 0x4<<0;  // Memc_cmd = 0x4

    // Timing Parameter Setup
    pg_dram_reg->REFRESH = (((cn_hclk /1000 *cn_DDR_REF) -1)/1000000 + 1);

    pg_dram_reg->CASLAT = 0x3<<1;   // CAS Latency = 3
    pg_dram_reg->T_DQSS = 0x1;          // mDDR = 0x1 (0.75 ~ 1.25)
    pg_dram_reg->T_MRD = 0x2;           // Mode Register Cmd Time
    // RAS(Row Active time) to Precharge delay , (min 45ns,  7@133MHz, margin:1 clock)
    pg_dram_reg->T_RAS = ((cn_hclk /1000 *cn_DDR_RAS) -1)/1000000 + 1;
    // RC(Row Cycle time) : Active bank x to Active bank x delay (min. 67.5ns, 10@133MHz, margin: 1clock)
    pg_dram_reg->T_RC = ((cn_hclk /1000 *cn_DDR_RC) -1)/1000000 + 1;
    Para_RCD = (((cn_hclk /1000 *cn_DDR_RCD) -1)/1000000 + 1);
    if (Para_RCD <4)
    {
        Scheduled_Para = 3;
    }
    else
    {
        Scheduled_Para = Para_RCD;
    }
    Para_RCD = ((Scheduled_Para-3)<<3) | (Para_RCD);
    pg_dram_reg->T_RCD = Para_RCD; // RAS to CAS Delay, (min 22.5ns, 4@133MHz, margin:1clock)
    uPara_RFC = (((cn_hclk /1000 *cn_DDR_RFC) -1)/1000000 + 1);
    if (uPara_RFC <4)
    {
         Scheduled_Para = 3;
    }
    else
    {
        Scheduled_Para = uPara_RFC;
    }
    uPara_RFC = ((Scheduled_Para-3)<<3) | (uPara_RFC);     // Auto Refresh to cmd time(>=t_RC), (min 80ns, 11@133MHz)
    pg_dram_reg->T_RFC = uPara_RFC;
    uPara_RP = (((cn_hclk /1000 *cn_DDR_RP) -1)/1000000 + 1);
    if (uPara_RP <4)
    {
         Scheduled_Para = 3;
    }
    else
    {
        Scheduled_Para = uPara_RP;
    }
    uPara_RP = ((Scheduled_Para-3)<<3) | (uPara_RP);
    pg_dram_reg->T_RP = uPara_RP;        // Precharge to RAS delay (Row Pre-charge time) (min 22.5ns, 4@133MHz, margin:1clock)
    //  Active bank x to Active bank y delay (min 15ns, 3@133MHz, margin:1clock)
    pg_dram_reg->T_RRD = ((cn_hclk /1000 *cn_DDR_RRD) -1)/1000000 + 1;
    //  write to precharge delay (min 15ns, 3@133MHz, margin:1clock)
    pg_dram_reg->T_WR = ((cn_hclk /1000 *cn_DDR_WR) -1)/1000000 + 1;
    pg_dram_reg->T_WTR = 2;     //  write to read delay (tDAL?) (2clk + tRP) // 犬牢 鞘夸
    pg_dram_reg->T_XP = 2;              //  exit power down cmd time ( 1clk + 1.5ns)
    uPara_XSR= (((cn_hclk /1000 *cn_DDR_XSR) -1)/1000000 + 1) ;
    pg_dram_reg->T_XSR = uPara_XSR;      // exit self refresh cmd time,
    pg_dram_reg->T_ESR = uPara_XSR;      // self refresh cmd time // 犬牢 鞘夸

    // Memory Configuration Register
    pg_dram_reg->MEMCFG =
        (0<<31)         |       // Disable Individul CKE Control
        (0<<21)         |       // 1chip
        (0<<18)         |       // Qos master selection by ARID[3:0]
        (2<<15)         |       // Burst 4
        (0<<14)         |       // Disable Stop Mem Clock ( 0)
        (0<<13)         |       // Disable Auto Power Down (0)
        (0<<7)          |       // Auto Power Down Period
        (cn_AP_bit<<6)    |       // Auto Precharge bit in bit 10
        (cn_Row_bit<<3)   |       // 13bit Row bits
        (cn_Col_bit<<0);          // 10bit Column bits

    // Memory Configuration Register 2
    pg_dram_reg->MEMCFG2 =
        (1<<11)     |      // Read delay 1 cycle from the Pad I/F ( 0x1 => mDDR)
        (3<<8)      |      // Memory Type (mDDR)
        (1<<6)      |      // Width ( 32bit)
        (0<<4)      |      // Bank bits = 2bit
        (0<<2)      |      // DQM state
        (1<<0);            // Sync. clock scheme

    // CHIP0 Configure
    pg_dram_reg->CHIP_0_CFG = 0x150f8; // Bank-Row-Column, 0x5000_0000 ~ 0x57ff_ffff ( 128MB)

    // Direct Command - External Memory Initialize

    // NOP
    pg_dram_reg->DIRECTCMD =
        (0<<20) |       // Chip Address  - Chip 0
        (3<<18);        // Command - NOP

    // Precharge All
    pg_dram_reg->DIRECTCMD =
        (0<<20) |       // Chip Address  - Chip 0
        (0<<18) ;      // Command - PALL

    // AutoRefresh 2 times
    pg_dram_reg->DIRECTCMD =
        (0<<20) |       // Chip Address  - Chip 0
        (1<<18);      // Command - Autorefresh

    pg_dram_reg->DIRECTCMD =
        (0<<20) |       // Chip Address  - Chip 0
        (1<<18);      // Command - Autorefresh

    // MRS
    pg_dram_reg->DIRECTCMD =
        (0<<20) |       // Chip Address  - Chip 0
        (2<<18) |       // Command - MRS/EMRS
        (0<<16) |       // [17:16] - MRS ( 0 )
        (0x32<<0);     //  [6:4]- CAS Latency - 3,
                        // [3] - Burst Type (Sequential), [2:0] - Burst Length(4) -> 8

    // EMRS
    pg_dram_reg->DIRECTCMD =
        (0<<20) |       // Chip Address  - Chip 0
        (2<<18) |       // Command - MRS/EMRS
        (2<<16) |       // [17:16] - EMRS (2)
        (0x0<<0);      // [6:5]- Full Strength,
                        // [2:0] - PASR - Full Array


    //'GO'  Mode
    pg_dram_reg->MEMCCMD = (0x0<<0);    // Go Command

    // Check Controller State
    while((pg_dram_reg->MEMSTAT&0x3) != 1  );
}

