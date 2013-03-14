/*
  由于所有的命令、地址和数据全部从8位IO口传输，所以nandflash定义了一个命令集来
  完成各种操作。有的操作只需要一个命令（即一个周期）即可，而有的操作则需要两个命令
  （即两个周期）来实现。下面的宏定义为K9F2G08U0A的常用命令：
*/
#define CMD_READ1                0x00              //页读命令周期1
#define CMD_READ2                0x30              //页读命令周期2
#define CMD_READID               0x90              //读ID命令
#define CMD_WRITE1               0x80              //页写命令周期1
#define CMD_WRITE2               0x10              //页写命令周期2
#define CMD_ERASE1               0x60              //块擦除命令周期1
#define CMD_ERASE2               0xd0              //块擦除命令周期2
#define CMD_STATUS               0x70              //读状态命令
#define CMD_RESET                0xff              //复位
#define CMD_RANDOMREAD1          0x05              //随意读命令周期1
#define CMD_RANDOMREAD2          0xE0              //随意读命令周期2
#define CMD_RANDOMWRITE          0x85              //随意写命令

/*
  在这里，随意读命令和随意写命令可以实现在一页内任意地址地读写。读状态命令可以实现读
  取设备内的状态寄存器，通过该命令可以获知写操作或擦除操作是否完成（判断第6位），以
  及是否成功完成（判断第0位）。
  引脚配置：
  OM[1:0] = 00: Enable NAND flash memory boot
  NCON : NAND flash memory selection(Normal / Advance)
      0: Normal NAND flash(256Words/512Bytes page size, 3/4 address cycle)
      1: Advance NAND flash(1KWords/2KBytes page size, 4/5 address cycle)
  GPG13 : NAND flash memory page capacitance selection
      0: Page=256Words(NCON = 0) or Page=1KWords(NCON = 1)
      1: Page=512Bytes(NCON = 0) or Page=2KBytes(NCON = 1)
  GPG14: NAND flash memory address cycle selection
      0: 3 address cycle(NCON = 0) or 4 address cycle(NCON = 1)
      1: 4 address cycle(NCON = 0) or 5 address cycle(NCON = 1)
  （注：GPG13 GPG14 read only）
  GPG15 : NAND flash memory bus width selection
      0: 8-bit bus width
      1: 16-bit bus width
  与s3c2440的硬件连接：
  下面介绍s3c2440的nandflash控制器。s3c2440支持8位或16位的每页大小为256字，512
  字节，1K字和2K字节的nandflash，这些配置是通过系统上电后相应引脚的高低电平来实现
  的。s3c2440还可以硬件产生ECC校验码，这为准确及时发现nandflash的坏块带来了方便。
  nandflash控制器的主要寄存器有NFCONF（nandflash配置寄存器），NFCONT（nandflash
  控制寄存器），NFCMMD（nandflash命令集寄存器），NFADDR（nandflash地址集寄存
  器），NFDATA（nandflash数据寄存器），NFMECCD0/1（nandflash的main区ECC寄存
  器），NFSECCD（nandflash的spare区ECC寄存器），NFSTAT（nandflash操作状态寄存
  器），NFESTAT0/1（nandflash的ECC状态寄存器），NFMECC0/1（nandflash用于数据的
  ECC寄存器），以及NFSECC（nandflash用于IO的ECC寄存器）。
  NFCMMD，NFADDR和NFDATA分别用于传输命令，地址和数据，为了方便起见，我们
  可以定义一些宏定义用于完成上述操作：
*/
#define NF_CMD(data)               {rNFCMD  = (data); }          //传输命令
#define NF_ADDR(addr)              {rNFADDR = (addr); }          //传输地址
#define NF_RDDATA()                (rNFDATA)                     //读32位数据
#define NF_RDDATA8()               (rNFDATA8)                    //读8位数据
#define NF_WRDATA(data)            {rNFDATA = (data); }          //写32位数据
#define NF_WRDATA8(data)           {rNFDATA8 = (data); }         //写8位数据

/*
  其中rNFDATA8的定义为 
  (*(volatile unsigned char *)0x4E000010)         //0x4E000010此地址是NFDATA寄存器的地址
  NFCONF主要用到了TACLS、TWRPH0、TWRPH1，这三个变量用于配置nandflash的时
  序。s3c2440的数据手册没有详细说明这三个变量的具体含义，但通过它所给出的时序图，我
  们可以看出，TACLS为CLE/ALE有效到nWE有效之间的持续时间，TWRPH0为nWE的有效持
  续时间，TWRPH1为nWE无效到CLE/ALE无效之间的持续时间，这些时间都是以HCLK为单位
  的（本文程序中的HCLK=100MHz）。通过查阅K9F2G08U0A的数据手册，我们可以找到并计
  算该nandflash与s3c2440相对应的时序：K9F2G08U0A中的tWP与TWRPH0相对应，tCLH与
  TWRPH1相对应，（tCLS－tWP）与TACLS相对应。K9F2G08U0A给出的都是最小时
  间，s3c2440只要满足它的最小时间即可，因此TACLS、TWRPH0、TWRPH1这三个变量取
  值大一些会更保险。在这里，这三个值分别取1，2和0。
  NFCONF的第0位表示的是外接的nandflash是8位IO还是16位IO，这里当然要选择8位的IO。
  NFCONT寄存器是另一个需要事先初始化的寄存器。它的第13位和第12位用于锁定配置，第8
  位到第10位用于nandflash的中断，第4位到第6位用于ECC的配置，第1位用于nandflash芯片
  的选取，第0位用于nandflash控制器的使能。另外，为了初始化nandflash，还需要配置
  GPACON寄存器，使它的第17位到第22位与nandflash芯片的控制引脚相对应。下面的程序实
  现了初始化nandflash控制器：
*/
void NF_Init ( void )
{
    rGPACON = (rGPACON &~(0x3f<<17)) | (0x3f<<17);            //配置芯片引脚
    //TACLS=1、TWRPH0=2、TWRPH1=0，8位IO，
    rNFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4)|(0<<0);
    //非锁定，屏蔽nandflash中断，初始化ECC及锁定main区和spare区ECC，使能nandflash片选及控制器
    rNFCONT = (0<<13)|(0<<12)|(0<<10)|(0<<9)|(0<<8)|(1<<6)|(1<<5)|(1<<4)|(1<<1)|(1<<0);
}

//为了更好地应用ECC和使能nandflash片选，我们还需要一些宏定义：
#define NF_nFCE_L()                  {rNFCONT &= ~(1<<1); }
#define NF_CE_L()                     NF_nFCE_L()                          //打开nandflash片选
#define NF_nFCE_H()                  {rNFCONT |= (1<<1); }
#define NF_CE_H()                     NF_nFCE_H()                          //关闭nandflash片选
#define NF_RSTECC()                  {rNFCONT |= (1<<4); }                 //复位ECC
#define NF_MECC_UnLock()             {rNFCONT &= ~(1<<5); }                //解锁main区ECC
#define NF_MECC_Lock()               {rNFCONT |= (1<<5); }                 //锁定main区ECC
#define NF_SECC_UnLock()             {rNFCONT &= ~(1<<6); }                //解锁spare区ECC
#define NF_SECC_Lock()               {rNFCONT |= (1<<6); }                 //锁定spare区ECC
//NFSTAT是另一个比较重要的寄存器，它的第0位可以用于判断nandflash是否在忙，第2位用于检测RnB引脚信号：
#define NF_WAITRB()                  {while(!(rNFSTAT&(1<<0)));}           //等待nandflash不忙
#define NF_CLEAR_RB()                {rNFSTAT |= (1<<2); }                 //清除RnB信号
#define NF_DETECT_RB()               {while(!(rNFSTAT&(1<<2)));}           //等待RnB信号变高，即不忙

//下面就详细介绍K9F2G08U0A的基本操作，包括复位，读ID，页读、写数据，随意读、写数据，块擦除等。
//复位操作最简单，只需写入复位命令即可：
static void rNF_Reset()
{
    NF_CE_L();                   //打开nandflash片选
    NF_CLEAR_RB();               //清除RnB信号
    NF_CMD(CMD_RESET);           //写入复位命令
    NF_DETECT_RB();              //等待RnB信号变高，即不忙
    NF_CE_H();                   //关闭nandflash片选
}

/*
  读取K9F2G08U0A芯片ID操作首先需要写入读ID命令，然后再写入0x00地址，就可以读
  取到一共五个周期的芯片ID，第一个周期为厂商ID，第二个周期为设备ID，第三个周期至第五
  个周期包括了一些具体的该芯片信息，这里就不多介绍：
*/
static char rNF_ReadID()
{
    char pMID;
    char pDID;
    char cyc3, cyc4, cyc5;
    NF_nFCE_L();                            //打开nandflash片选
    NF_CLEAR_RB();                          //清RnB信号
    NF_CMD(CMD_READID);                     //读ID命令
    NF_ADDR(0x0);                           //写0x00地址
    //读五个周期的ID
    pMID = NF_RDDATA8();                    //厂商ID：0xEC
    pDID = NF_RDDATA8();                    //设备ID：0xDA
    cyc3 = NF_RDDATA8();                    //0x10
    cyc4 = NF_RDDATA8();                    //0x95
    cyc5 = NF_RDDATA8();                    //0x44
    NF_nFCE_H();                            //关闭nandflash片选
    return (pDID);
}

/*
  下面介绍读操作，读操作是以页为单位进行的。如果在读取数据的过程中不进行ECC校验
  判断，则读操作比较简单，在写入读命令的两个周期之间写入要读取的页地址，然后读取数据
  即可。如果为了更准确地读取数据，则在读取完数据之后还要进行ECC校验判断，以确定所读
  取的数据是否正确。
  在上文中我们已经介绍过，nandflash的每一页有两区：main区和spare区，main区用于存储
  正常的数据，spare区用于存储其他附加信息，其中就包括ECC校验码。当我们在写入数据的
  时候，我们就计算这一页数据的ECC校验码，然后把校验码存储到spare区的特定位置中，在
  下次读取这一页数据的时候，同样我们也计算ECC校验码，然后与spare区中的ECC校验码比
  较，如果一致则说明读取的数据正确，如果不一致则不正确。ECC的算法较为复杂，好在
  s3c2440能够硬件产生ECC校验码，这样就省去了不少的麻烦事。s3c2440即可以产生main区
  的ECC校验码，也可以产生spare区的ECC校验码。因为K9F2G08U0A是8位IO口，因此
  s3c2440共产生4个字节的main区ECC码和2个字节的spare区ECC码。在这里我们规定，在每
  一页的spare区的第0个地址到第3个地址存储main区ECC，第4个地址和第5个地址存储spare
  区ECC。产生ECC校验码的过程为：在读取或写入哪个区的数据之前，先解锁该区的ECC，
  以便产生该区的ECC。在读取或写入完数据之后，再锁定该区的ECC，这样系统就会把产生
  的ECC码保存到相应的寄存器中。main区的ECC保存到NFMECC0/1中（因为K9F2G08U0A是
  8位IO口，因此这里只用到了NFMECC0），spare区的ECC保存到NFSECC中。对于读操作来
  说，我们还要继续读取spare区的相应地址内容，已得到上次写操作时所存储的main区和
  spare区的ECC，并把这些数据分别放入NFMECCD0/1和NFSECCD的相应位置中。最后我们
  就可以通过读取NFESTAT0/1（因为K9F2G08U0A是8位IO口，因此这里只用到了
  NFESTAT0）中的低4位来判断读取的数据是否正确，其中第0位和第1位为main区指示错误，
  第2位和第3位为spare区指示错误。
  下面就给出一段具体的页读操作程序：
*/
U8 rNF_ReadPage(U32 page_number)
{
    U32 i, mecc0, secc;
    NF_RSTECC();                                 //复位ECC
    NF_MECC_UnLock();                            //解锁main区ECC
    NF_nFCE_L();                                 //打开nandflash片选
    NF_CLEAR_RB();                               //清RnB信号
    NF_CMD(CMD_READ1);                           //页读命令周期1
    //写入5个地址周期
    NF_ADDR(0x00);                               //列地址A0~A7
    NF_ADDR(0x00);                               //列地址A8~A11
    NF_ADDR((page_number) & 0xff);               //行地址A12~A19
    NF_ADDR((page_number >> 8) & 0xff);          //行地址A20~A27
    NF_ADDR((page_number >> 16) & 0xff);         //行地址A28
    NF_CMD(CMD_READ2);                           //页读命令周期2
    NF_DETECT_RB();                              //等待RnB信号变高，即不忙
      
    //读取一页数据内容
    for (i = 0; i < 2048; i++)
    {
        buffer[i] =  NF_RDDATA8();
    }
      
    NF_MECC_Lock();                    //锁定main区ECC值
      
    NF_SECC_UnLock();                  //解锁spare区ECC
    mecc0=NF_RDDATA();                 //读spare区的前4个地址内容，即第2048~2051地址，这4个字节为main区的ECC
    //把读取到的main区的ECC校验码放入NFMECCD0/1的相应位置内
    rNFMECCD0=((mecc0&0xff00)<<8)|(mecc0&0xff);
    rNFMECCD1=((mecc0&0xff000000)>>8)|((mecc0&0xff0000)>>16);
            
    NF_SECC_Lock();                    //锁定spare区的ECC值
    secc=NF_RDDATA();                  //继续读spare区的4个地址内容，即第2052~2055地址，其中前2个字节为spare区的ECC值
    //把读取到的spare区的ECC校验码放入NFSECCD的相应位置内
    rNFSECCD=((secc&0xff00)<<8)|(secc&0xff);
    NF_nFCE_H();                       //关闭nandflash片选
      
    //判断所读取到的数据是否正确
    if ((rNFESTAT0&0xf) == 0x0)
        return 0x66;                  //正确
    else
        return 0x44;                  //错误
}

/*
  这段程序是把某一页的内容读取到全局变量数组buffer中。该程序的输入参数直接就为
  K9F2G08U0A的第几页，例如我们要读取第128064页中的内容，可以调用该程序
  为：rNF_ReadPage(128064);。由于第128064页是第2001块中的第0页
  （128064＝2001×64＋0），所以为了更清楚地表示页与块之间的关系，也可以写
  为：rNF_ReadPage(2001*64);。
*/
/*
  页写操作的大致流程为：在两个写命令周期之间分别写入页地址和数据，当然如果为了保
  证下次读取该数据时的正确性，还需要把main区的ECC值和spare区的ECC值写入到该页的
  spare区内。然后我们还需要读取状态寄存器，以判断这次写操作是否正确。下面就给出一段
  具体的页写操作程序，其中输入参数也是要写入数据到第几页：
*/
U8 rNF_WritePage(U32 page_number)
{
    U32 i, mecc0, secc;
    U8 stat, temp;
      
    temp = rNF_IsBadBlock(page_number>>6);              //判断该块是否为坏块
    if(temp == 0x33)
        return 0x42;               //是坏块，返回
    NF_RSTECC();                   //复位ECC
    NF_MECC_UnLock();              //解锁main区的ECC
    NF_nFCE_L();                   //打开nandflash片选
    NF_CLEAR_RB();                 //清RnB信号
    NF_CMD(CMD_WRITE1);            //页写命令周期1
    //写入5个地址周期
    NF_ADDR(0x00);                                     //列地址A0~A7
    NF_ADDR(0x00);                                     //列地址A8~A11
    NF_ADDR((page_number) & 0xff);                     //行地址A12~A19
    NF_ADDR((page_number >> 8) & 0xff);                //行地址A20~A27
    NF_ADDR((page_number >> 16) & 0xff);               //行地址A28
      
    //写入一页数据
    for (i = 0; i < 2048; i++)
    {
        NF_WRDATA8((char)(i+6));
    }
      
    NF_MECC_Lock();                      //锁定main区的ECC值
      
    mecc0=rNFMECC0;                      //读取main区的ECC校验码
    //把ECC校验码由字型转换为字节型，并保存到全局变量数组ECCBuf中
    ECCBuf[0]=(U8)(mecc0&0xff);
    ECCBuf[1]=(U8)((mecc0>>8) & 0xff);
    ECCBuf[2]=(U8)((mecc0>>16) & 0xff);
    ECCBuf[3]=(U8)((mecc0>>24) & 0xff);
    NF_SECC_UnLock();                    //解锁spare区的ECC
    //把main区的ECC值写入到spare区的前4个字节地址内，即第2048~2051地址
    for(i=0;i<4;i++)
    {
        NF_WRDATA8(ECCBuf[i]);
    }
    NF_SECC_Lock();                      //锁定spare区的ECC值
    secc=rNFSECC;                        //读取spare区的ECC校验码
    //把ECC校验码保存到全局变量数组ECCBuf中
    ECCBuf[4]=(U8)(secc&0xff);
    ECCBuf[5]=(U8)((secc>>8) & 0xff);
    //把spare区的ECC值继续写入到spare区的第2052~2053地址内
    for(i=4;i<6;i++)
    {
        NF_WRDATA8(ECCBuf[i]);
    }
    NF_CMD(CMD_WRITE2);                //页写命令周期2
    delay(1000);                       //延时一段时间，以等待写操作完成
   
    NF_CMD(CMD_STATUS);                //读状态命令
   
    //判断状态值的第6位是否为1，即是否在忙，该语句的作用与NF_DETECT_RB();相同
    do{
        stat = NF_RDDATA8();
    }while(!(stat&0x40));
   
    NF_nFCE_H();                       //关闭nandflash片选
    //判断状态值的第0位是否为0，为0则写操作正确，否则错误
    if (stat & 0x1)
    {
        temp = rNF_MarkBadBlock(page_number>>6);         //标注该页所在的块为坏块
        if (temp == 0x21)
            return 0x43;               //标注坏块失败
        else
            return 0x44;               //写操作失败
    }
    else
        return 0x66;                   //写操作成功
}

/*
  该段程序先判断该页所在的坏是否为坏块，如果是则退出。在最后写操作失败后，还要标注该
  页所在的块为坏块，其中所用到的函数rNF_IsBadBlock和rNF_MarkBadBlock，我们在后面介
  绍。我们再总结一下该程序所返回数值的含义，0x42：表示该页所在的块为坏块；0x43：表
  示写操作失败，并且在标注该页所在的块为坏块时也失败；0x44：表示写操作失败，但是标注
  坏块成功；0x66：写操作成功
*/

/*
  擦除是以块为单位进行的，因此在写地址周期是，只需写三个行周期，并且要从A18开始写
  起。与写操作一样，在擦除结束前还要判断是否擦除操作成功，另外同样也存在需要判断是否
  为坏块以及要标注坏块的问题。下面就给出一段具体的块擦除操作程序：
*/
U8 rNF_EraseBlock(U32 block_number)
{
    char stat, temp;
      
    temp = rNF_IsBadBlock(block_number);     //判断该块是否为坏块
    if(temp == 0x33)
        return 0x42;                         //是坏块，返回
      
    NF_nFCE_L();                             //打开片选
    NF_CLEAR_RB();                           //清RnB信号
    NF_CMD(CMD_ERASE1);                      //擦除命令周期1
    //写入3个地址周期，从A18开始写起
    NF_ADDR((block_number << 6) & 0xff);     //行地址A18~A19
    NF_ADDR((block_number >> 2) & 0xff);     //行地址A20~A27
    NF_ADDR((block_number >> 10) & 0xff);    //行地址A28
      
    NF_CMD(CMD_ERASE2);                      //擦除命令周期2
    delay(1000);                             //延时一段时间
   
    NF_CMD(CMD_STATUS);                      //读状态命令
      
    //判断状态值的第6位是否为1，即是否在忙，该语句的作用与NF_DETECT_RB();相同
    do{
        stat = NF_RDDATA8();
    }while(!(stat&0x40));
   
    NF_nFCE_H();                             //关闭nandflash片选

    //判断状态值的第0位是否为0，为0则擦除操作正确，否则错误
    if (stat & 0x1)
    {
        temp = rNF_MarkBadBlock(page_number>>6);         //标注该块为坏块
        if (temp == 0x21)
            return 0x43;            //标注坏块失败
        else
            return 0x44;            //擦除操作失败
    }
    else
        return 0x66;                //擦除操作成功
}

/*
  该程序的输入参数为K9F2G08U0A的第几块，例如我们要擦除第2001块，则调用该函数
  为：rNF_EraseBlock(2001)。
*/
/*
  K9F2G08U0A除了提供了页读和页写功能外，还提供了页内地址随意读、写功能。页读
  和页写是从页的首地址开始读、写，而随意读、写实现了在一页范围内任意地址的读、写。随
  意读操作是在页读操作后输入随意读命令和页内列地址，这样就可以读取到列地址所指定地址
  的数据。随意写操作是在页写操作的第二个页写命令周期前，输入随意写命令和页内列地址，
  以及要写入的数据，这样就可以把数据写入到列地址所指定的地址内。下面两段程序实现了随
  意读和随意写功能，其中随意读程序的输入参数分别为页地址和页内地址，输出参数为所读取
  到的数据，随意写程序的输入参数分别为页地址，页内地址，以及要写入的数据。
*/
U8 rNF_RamdomRead(U32 page_number, U32 add)
{
    NF_nFCE_L();                                //打开nandflash片选
    NF_CLEAR_RB();                              //清RnB信号
    NF_CMD(CMD_READ1);                          //页读命令周期1
    //写入5个地址周期
    NF_ADDR(0x00);                              //列地址A0~A7
    NF_ADDR(0x00);                              //列地址A8~A11
    NF_ADDR((page_number) & 0xff);              //行地址A12~A19
    NF_ADDR((page_number >> 8) & 0xff);         //行地址A20~A27
    NF_ADDR((page_number >> 16) & 0xff);        //行地址A28
    NF_CMD(CMD_READ2);                          //页读命令周期2
    NF_DETECT_RB();                             //等待RnB信号变高，即不忙
    NF_CMD(CMD_RANDOMREAD1);                    //随意读命令周期1
    //页内地址
    NF_ADDR((char)(add&0xff));                  //列地址A0~A7
    NF_ADDR((char)((add>>8)&0x0f));             //列地址A8~A11
    NF_CMD(CMD_RANDOMREAD2);                    //随意读命令周期2
      
    return NF_RDDATA8();                        //读取数据
}

U8 rNF_RamdomWrite(U32 page_number, U32 add, U8 dat)
{
    U8 temp,stat;
    NF_nFCE_L();                                //打开nandflash片选
    NF_CLEAR_RB();                              //清RnB信号
    NF_CMD(CMD_WRITE1);                         //页写命令周期1
    //写入5个地址周期
    NF_ADDR(0x00);                              //列地址A0~A7
    NF_ADDR(0x00);                              //列地址A8~A11
    NF_ADDR((page_number) & 0xff);              //行地址A12~A19
    NF_ADDR((page_number >> 8) & 0xff);         //行地址A20~A27
    NF_ADDR((page_number >> 16) & 0xff);        //行地址A28
    NF_CMD(CMD_RANDOMWRITE);                    //随意写命令
    //页内地址
    NF_ADDR((char)(add&0xff));                  //列地址A0~A7
    NF_ADDR((char)((add>>8)&0x0f));             //列地址A8~A11
    NF_WRDATA8(dat);                            //写入数据
    NF_CMD(CMD_WRITE2);                         //页写命令周期2
      
    delay(1000);                                //延时一段时间
    
    NF_CMD(CMD_STATUS);                         //读状态命令
   
    //判断状态值的第6位是否为1，即是否在忙，该语句的作用与NF_DETECT_RB();相同
    do{
        stat =  NF_RDDATA8();
    }while(!(stat&0x40));
   
    NF_nFCE_H();                                //关闭nandflash片选
    
    //判断状态值的第0位是否为0，为0则写操作正确，否则错误
    if (stat & 0x1)
        return 0x44;                  //失败
    else
        return 0x66;                  //成功
}

/*
  下面介绍上文中提到的判断坏块以及标注坏块的那两个程序：rNF_IsBadBlock和
  rNF_MarkBadBlock。在这里，我们定义在spare区的第6个地址（即每页的第2054地址）用来
  标注坏块，0x44表示该块为坏块。要判断坏块时，利用随意读命令来读取2054地址的内容是
  否为0x44，要标注坏块时，利用随意写命令来向2054地址写0x33。下面就给出这两个程序，
  它们的输入参数都为块地址，也就是即使仅仅一页出现问题，我们也标注整个块为坏块。
*/
U8 rNF_IsBadBlock(U32 block)
{
    return rNF_RamdomRead(block*64, 2054);
}

U8 rNF_MarkBadBlock(U32 block)
{
    U8 result;
   
    result = rNF_RamdomWrite(block*64, 2054, 0x33);
    if(result == 0x44)
        return 0x21;                  //写坏块标注失败
    else
        return 0x60;                  //写坏块标注成功
}

/*
  关于nandflash的基本操作就讲解到这里，当然nandflash还有一些其他复杂的操作，如逻
  辑地址与物理地址的转换，坏块的替代等，这些内容本文就不再介绍了。
*/
