/*
  �������е������ַ������ȫ����8λIO�ڴ��䣬����nandflash������һ�������
  ��ɸ��ֲ������еĲ���ֻ��Ҫһ�������һ�����ڣ����ɣ����еĲ�������Ҫ��������
  �����������ڣ���ʵ�֡�����ĺ궨��ΪK9F2G08U0A�ĳ������
*/
#define CMD_READ1                0x00              //ҳ����������1
#define CMD_READ2                0x30              //ҳ����������2
#define CMD_READID               0x90              //��ID����
#define CMD_WRITE1               0x80              //ҳд��������1
#define CMD_WRITE2               0x10              //ҳд��������2
#define CMD_ERASE1               0x60              //�������������1
#define CMD_ERASE2               0xd0              //�������������2
#define CMD_STATUS               0x70              //��״̬����
#define CMD_RESET                0xff              //��λ
#define CMD_RANDOMREAD1          0x05              //�������������1
#define CMD_RANDOMREAD2          0xE0              //�������������2
#define CMD_RANDOMWRITE          0x85              //����д����

/*
  �������������������д�������ʵ����һҳ�������ַ�ض�д����״̬�������ʵ�ֶ�
  ȡ�豸�ڵ�״̬�Ĵ�����ͨ����������Ի�֪д��������������Ƿ���ɣ��жϵ�6λ������
  ���Ƿ�ɹ���ɣ��жϵ�0λ����
  �������ã�
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
  ��ע��GPG13 GPG14 read only��
  GPG15 : NAND flash memory bus width selection
      0: 8-bit bus width
      1: 16-bit bus width
  ��s3c2440��Ӳ�����ӣ�
  �������s3c2440��nandflash��������s3c2440֧��8λ��16λ��ÿҳ��СΪ256�֣�512
  �ֽڣ�1K�ֺ�2K�ֽڵ�nandflash����Щ������ͨ��ϵͳ�ϵ����Ӧ���ŵĸߵ͵�ƽ��ʵ��
  �ġ�s3c2440������Ӳ������ECCУ���룬��Ϊ׼ȷ��ʱ����nandflash�Ļ�������˷��㡣
  nandflash����������Ҫ�Ĵ�����NFCONF��nandflash���üĴ�������NFCONT��nandflash
  ���ƼĴ�������NFCMMD��nandflash����Ĵ�������NFADDR��nandflash��ַ���Ĵ�
  ������NFDATA��nandflash���ݼĴ�������NFMECCD0/1��nandflash��main��ECC�Ĵ�
  ������NFSECCD��nandflash��spare��ECC�Ĵ�������NFSTAT��nandflash����״̬�Ĵ�
  ������NFESTAT0/1��nandflash��ECC״̬�Ĵ�������NFMECC0/1��nandflash�������ݵ�
  ECC�Ĵ��������Լ�NFSECC��nandflash����IO��ECC�Ĵ�������
  NFCMMD��NFADDR��NFDATA�ֱ����ڴ��������ַ�����ݣ�Ϊ�˷������������
  ���Զ���һЩ�궨�������������������
*/
#define NF_CMD(data)               {rNFCMD  = (data); }          //��������
#define NF_ADDR(addr)              {rNFADDR = (addr); }          //�����ַ
#define NF_RDDATA()                (rNFDATA)                     //��32λ����
#define NF_RDDATA8()               (rNFDATA8)                    //��8λ����
#define NF_WRDATA(data)            {rNFDATA = (data); }          //д32λ����
#define NF_WRDATA8(data)           {rNFDATA8 = (data); }         //д8λ����

/*
  ����rNFDATA8�Ķ���Ϊ 
  (*(volatile unsigned char *)0x4E000010)         //0x4E000010�˵�ַ��NFDATA�Ĵ����ĵ�ַ
  NFCONF��Ҫ�õ���TACLS��TWRPH0��TWRPH1��������������������nandflash��ʱ
  ��s3c2440�������ֲ�û����ϸ˵�������������ľ��庬�壬��ͨ������������ʱ��ͼ����
  �ǿ��Կ�����TACLSΪCLE/ALE��Ч��nWE��Ч֮��ĳ���ʱ�䣬TWRPH0ΪnWE����Ч��
  ��ʱ�䣬TWRPH1ΪnWE��Ч��CLE/ALE��Ч֮��ĳ���ʱ�䣬��Щʱ�䶼����HCLKΪ��λ
  �ģ����ĳ����е�HCLK=100MHz����ͨ������K9F2G08U0A�������ֲᣬ���ǿ����ҵ�����
  ���nandflash��s3c2440���Ӧ��ʱ��K9F2G08U0A�е�tWP��TWRPH0���Ӧ��tCLH��
  TWRPH1���Ӧ����tCLS��tWP����TACLS���Ӧ��K9F2G08U0A�����Ķ�����Сʱ
  �䣬s3c2440ֻҪ����������Сʱ�伴�ɣ����TACLS��TWRPH0��TWRPH1����������ȡ
  ֵ��һЩ������ա������������ֵ�ֱ�ȡ1��2��0��
  NFCONF�ĵ�0λ��ʾ������ӵ�nandflash��8λIO����16λIO�����ﵱȻҪѡ��8λ��IO��
  NFCONT�Ĵ�������һ����Ҫ���ȳ�ʼ���ļĴ��������ĵ�13λ�͵�12λ�����������ã���8
  λ����10λ����nandflash���жϣ���4λ����6λ����ECC�����ã���1λ����nandflashоƬ
  ��ѡȡ����0λ����nandflash��������ʹ�ܡ����⣬Ϊ�˳�ʼ��nandflash������Ҫ����
  GPACON�Ĵ�����ʹ���ĵ�17λ����22λ��nandflashоƬ�Ŀ����������Ӧ������ĳ���ʵ
  ���˳�ʼ��nandflash��������
*/
void NF_Init ( void )
{
    rGPACON = (rGPACON &~(0x3f<<17)) | (0x3f<<17);            //����оƬ����
    //TACLS=1��TWRPH0=2��TWRPH1=0��8λIO��
    rNFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4)|(0<<0);
    //������������nandflash�жϣ���ʼ��ECC������main����spare��ECC��ʹ��nandflashƬѡ��������
    rNFCONT = (0<<13)|(0<<12)|(0<<10)|(0<<9)|(0<<8)|(1<<6)|(1<<5)|(1<<4)|(1<<1)|(1<<0);
}

//Ϊ�˸��õ�Ӧ��ECC��ʹ��nandflashƬѡ�����ǻ���ҪһЩ�궨�壺
#define NF_nFCE_L()                  {rNFCONT &= ~(1<<1); }
#define NF_CE_L()                     NF_nFCE_L()                          //��nandflashƬѡ
#define NF_nFCE_H()                  {rNFCONT |= (1<<1); }
#define NF_CE_H()                     NF_nFCE_H()                          //�ر�nandflashƬѡ
#define NF_RSTECC()                  {rNFCONT |= (1<<4); }                 //��λECC
#define NF_MECC_UnLock()             {rNFCONT &= ~(1<<5); }                //����main��ECC
#define NF_MECC_Lock()               {rNFCONT |= (1<<5); }                 //����main��ECC
#define NF_SECC_UnLock()             {rNFCONT &= ~(1<<6); }                //����spare��ECC
#define NF_SECC_Lock()               {rNFCONT |= (1<<6); }                 //����spare��ECC
//NFSTAT����һ���Ƚ���Ҫ�ļĴ��������ĵ�0λ���������ж�nandflash�Ƿ���æ����2λ���ڼ��RnB�����źţ�
#define NF_WAITRB()                  {while(!(rNFSTAT&(1<<0)));}           //�ȴ�nandflash��æ
#define NF_CLEAR_RB()                {rNFSTAT |= (1<<2); }                 //���RnB�ź�
#define NF_DETECT_RB()               {while(!(rNFSTAT&(1<<2)));}           //�ȴ�RnB�źű�ߣ�����æ

//�������ϸ����K9F2G08U0A�Ļ���������������λ����ID��ҳ����д���ݣ��������д���ݣ�������ȡ�
//��λ������򵥣�ֻ��д�븴λ����ɣ�
static void rNF_Reset()
{
    NF_CE_L();                   //��nandflashƬѡ
    NF_CLEAR_RB();               //���RnB�ź�
    NF_CMD(CMD_RESET);           //д�븴λ����
    NF_DETECT_RB();              //�ȴ�RnB�źű�ߣ�����æ
    NF_CE_H();                   //�ر�nandflashƬѡ
}

/*
  ��ȡK9F2G08U0AоƬID����������Ҫд���ID���Ȼ����д��0x00��ַ���Ϳ��Զ�
  ȡ��һ��������ڵ�оƬID����һ������Ϊ����ID���ڶ�������Ϊ�豸ID������������������
  �����ڰ�����һЩ����ĸ�оƬ��Ϣ������Ͳ�����ܣ�
*/
static char rNF_ReadID()
{
    char pMID;
    char pDID;
    char cyc3, cyc4, cyc5;
    NF_nFCE_L();                            //��nandflashƬѡ
    NF_CLEAR_RB();                          //��RnB�ź�
    NF_CMD(CMD_READID);                     //��ID����
    NF_ADDR(0x0);                           //д0x00��ַ
    //��������ڵ�ID
    pMID = NF_RDDATA8();                    //����ID��0xEC
    pDID = NF_RDDATA8();                    //�豸ID��0xDA
    cyc3 = NF_RDDATA8();                    //0x10
    cyc4 = NF_RDDATA8();                    //0x95
    cyc5 = NF_RDDATA8();                    //0x44
    NF_nFCE_H();                            //�ر�nandflashƬѡ
    return (pDID);
}

/*
  ������ܶ�����������������ҳΪ��λ���еġ�����ڶ�ȡ���ݵĹ����в�����ECCУ��
  �жϣ���������Ƚϼ򵥣���д����������������֮��д��Ҫ��ȡ��ҳ��ַ��Ȼ���ȡ����
  ���ɡ����Ϊ�˸�׼ȷ�ض�ȡ���ݣ����ڶ�ȡ������֮��Ҫ����ECCУ���жϣ���ȷ������
  ȡ�������Ƿ���ȷ��
  �������������Ѿ����ܹ���nandflash��ÿһҳ��������main����spare����main�����ڴ洢
  ���������ݣ�spare�����ڴ洢����������Ϣ�����оͰ���ECCУ���롣��������д�����ݵ�
  ʱ�����Ǿͼ�����һҳ���ݵ�ECCУ���룬Ȼ���У����洢��spare�����ض�λ���У���
  �´ζ�ȡ��һҳ���ݵ�ʱ��ͬ������Ҳ����ECCУ���룬Ȼ����spare���е�ECCУ�����
  �ϣ����һ����˵����ȡ��������ȷ�������һ������ȷ��ECC���㷨��Ϊ���ӣ�����
  s3c2440�ܹ�Ӳ������ECCУ���룬������ʡȥ�˲��ٵ��鷳�¡�s3c2440�����Բ���main��
  ��ECCУ���룬Ҳ���Բ���spare����ECCУ���롣��ΪK9F2G08U0A��8λIO�ڣ����
  s3c2440������4���ֽڵ�main��ECC���2���ֽڵ�spare��ECC�롣���������ǹ涨����ÿ
  һҳ��spare���ĵ�0����ַ����3����ַ�洢main��ECC����4����ַ�͵�5����ַ�洢spare
  ��ECC������ECCУ����Ĺ���Ϊ���ڶ�ȡ��д���ĸ���������֮ǰ���Ƚ���������ECC��
  �Ա����������ECC���ڶ�ȡ��д��������֮��������������ECC������ϵͳ�ͻ�Ѳ���
  ��ECC�뱣�浽��Ӧ�ļĴ����С�main����ECC���浽NFMECC0/1�У���ΪK9F2G08U0A��
  8λIO�ڣ��������ֻ�õ���NFMECC0����spare����ECC���浽NFSECC�С����ڶ�������
  ˵�����ǻ�Ҫ������ȡspare������Ӧ��ַ���ݣ��ѵõ��ϴ�д����ʱ���洢��main����
  spare����ECC��������Щ���ݷֱ����NFMECCD0/1��NFSECCD����Ӧλ���С��������
  �Ϳ���ͨ����ȡNFESTAT0/1����ΪK9F2G08U0A��8λIO�ڣ��������ֻ�õ���
  NFESTAT0���еĵ�4λ���ж϶�ȡ�������Ƿ���ȷ�����е�0λ�͵�1λΪmain��ָʾ����
  ��2λ�͵�3λΪspare��ָʾ����
  ����͸���һ�ξ����ҳ����������
*/
U8 rNF_ReadPage(U32 page_number)
{
    U32 i, mecc0, secc;
    NF_RSTECC();                                 //��λECC
    NF_MECC_UnLock();                            //����main��ECC
    NF_nFCE_L();                                 //��nandflashƬѡ
    NF_CLEAR_RB();                               //��RnB�ź�
    NF_CMD(CMD_READ1);                           //ҳ����������1
    //д��5����ַ����
    NF_ADDR(0x00);                               //�е�ַA0~A7
    NF_ADDR(0x00);                               //�е�ַA8~A11
    NF_ADDR((page_number) & 0xff);               //�е�ַA12~A19
    NF_ADDR((page_number >> 8) & 0xff);          //�е�ַA20~A27
    NF_ADDR((page_number >> 16) & 0xff);         //�е�ַA28
    NF_CMD(CMD_READ2);                           //ҳ����������2
    NF_DETECT_RB();                              //�ȴ�RnB�źű�ߣ�����æ
      
    //��ȡһҳ��������
    for (i = 0; i < 2048; i++)
    {
        buffer[i] =  NF_RDDATA8();
    }
      
    NF_MECC_Lock();                    //����main��ECCֵ
      
    NF_SECC_UnLock();                  //����spare��ECC
    mecc0=NF_RDDATA();                 //��spare����ǰ4����ַ���ݣ�����2048~2051��ַ����4���ֽ�Ϊmain����ECC
    //�Ѷ�ȡ����main����ECCУ�������NFMECCD0/1����Ӧλ����
    rNFMECCD0=((mecc0&0xff00)<<8)|(mecc0&0xff);
    rNFMECCD1=((mecc0&0xff000000)>>8)|((mecc0&0xff0000)>>16);
            
    NF_SECC_Lock();                    //����spare����ECCֵ
    secc=NF_RDDATA();                  //������spare����4����ַ���ݣ�����2052~2055��ַ������ǰ2���ֽ�Ϊspare����ECCֵ
    //�Ѷ�ȡ����spare����ECCУ�������NFSECCD����Ӧλ����
    rNFSECCD=((secc&0xff00)<<8)|(secc&0xff);
    NF_nFCE_H();                       //�ر�nandflashƬѡ
      
    //�ж�����ȡ���������Ƿ���ȷ
    if ((rNFESTAT0&0xf) == 0x0)
        return 0x66;                  //��ȷ
    else
        return 0x44;                  //����
}

/*
  ��γ����ǰ�ĳһҳ�����ݶ�ȡ��ȫ�ֱ�������buffer�С��ó�����������ֱ�Ӿ�Ϊ
  K9F2G08U0A�ĵڼ�ҳ����������Ҫ��ȡ��128064ҳ�е����ݣ����Ե��øó���
  Ϊ��rNF_ReadPage(128064);�����ڵ�128064ҳ�ǵ�2001���еĵ�0ҳ
  ��128064��2001��64��0��������Ϊ�˸�����ر�ʾҳ���֮��Ĺ�ϵ��Ҳ����д
  Ϊ��rNF_ReadPage(2001*64);��
*/
/*
  ҳд�����Ĵ�������Ϊ��������д��������֮��ֱ�д��ҳ��ַ�����ݣ���Ȼ���Ϊ�˱�
  ֤�´ζ�ȡ������ʱ����ȷ�ԣ�����Ҫ��main����ECCֵ��spare����ECCֵд�뵽��ҳ��
  spare���ڡ�Ȼ�����ǻ���Ҫ��ȡ״̬�Ĵ��������ж����д�����Ƿ���ȷ������͸���һ��
  �����ҳд�������������������Ҳ��Ҫд�����ݵ��ڼ�ҳ��
*/
U8 rNF_WritePage(U32 page_number)
{
    U32 i, mecc0, secc;
    U8 stat, temp;
      
    temp = rNF_IsBadBlock(page_number>>6);              //�жϸÿ��Ƿ�Ϊ����
    if(temp == 0x33)
        return 0x42;               //�ǻ��飬����
    NF_RSTECC();                   //��λECC
    NF_MECC_UnLock();              //����main����ECC
    NF_nFCE_L();                   //��nandflashƬѡ
    NF_CLEAR_RB();                 //��RnB�ź�
    NF_CMD(CMD_WRITE1);            //ҳд��������1
    //д��5����ַ����
    NF_ADDR(0x00);                                     //�е�ַA0~A7
    NF_ADDR(0x00);                                     //�е�ַA8~A11
    NF_ADDR((page_number) & 0xff);                     //�е�ַA12~A19
    NF_ADDR((page_number >> 8) & 0xff);                //�е�ַA20~A27
    NF_ADDR((page_number >> 16) & 0xff);               //�е�ַA28
      
    //д��һҳ����
    for (i = 0; i < 2048; i++)
    {
        NF_WRDATA8((char)(i+6));
    }
      
    NF_MECC_Lock();                      //����main����ECCֵ
      
    mecc0=rNFMECC0;                      //��ȡmain����ECCУ����
    //��ECCУ����������ת��Ϊ�ֽ��ͣ������浽ȫ�ֱ�������ECCBuf��
    ECCBuf[0]=(U8)(mecc0&0xff);
    ECCBuf[1]=(U8)((mecc0>>8) & 0xff);
    ECCBuf[2]=(U8)((mecc0>>16) & 0xff);
    ECCBuf[3]=(U8)((mecc0>>24) & 0xff);
    NF_SECC_UnLock();                    //����spare����ECC
    //��main����ECCֵд�뵽spare����ǰ4���ֽڵ�ַ�ڣ�����2048~2051��ַ
    for(i=0;i<4;i++)
    {
        NF_WRDATA8(ECCBuf[i]);
    }
    NF_SECC_Lock();                      //����spare����ECCֵ
    secc=rNFSECC;                        //��ȡspare����ECCУ����
    //��ECCУ���뱣�浽ȫ�ֱ�������ECCBuf��
    ECCBuf[4]=(U8)(secc&0xff);
    ECCBuf[5]=(U8)((secc>>8) & 0xff);
    //��spare����ECCֵ����д�뵽spare���ĵ�2052~2053��ַ��
    for(i=4;i<6;i++)
    {
        NF_WRDATA8(ECCBuf[i]);
    }
    NF_CMD(CMD_WRITE2);                //ҳд��������2
    delay(1000);                       //��ʱһ��ʱ�䣬�Եȴ�д�������
   
    NF_CMD(CMD_STATUS);                //��״̬����
   
    //�ж�״ֵ̬�ĵ�6λ�Ƿ�Ϊ1�����Ƿ���æ��������������NF_DETECT_RB();��ͬ
    do{
        stat = NF_RDDATA8();
    }while(!(stat&0x40));
   
    NF_nFCE_H();                       //�ر�nandflashƬѡ
    //�ж�״ֵ̬�ĵ�0λ�Ƿ�Ϊ0��Ϊ0��д������ȷ���������
    if (stat & 0x1)
    {
        temp = rNF_MarkBadBlock(page_number>>6);         //��ע��ҳ���ڵĿ�Ϊ����
        if (temp == 0x21)
            return 0x43;               //��ע����ʧ��
        else
            return 0x44;               //д����ʧ��
    }
    else
        return 0x66;                   //д�����ɹ�
}

/*
  �öγ������жϸ�ҳ���ڵĻ��Ƿ�Ϊ���飬��������˳��������д����ʧ�ܺ󣬻�Ҫ��ע��
  ҳ���ڵĿ�Ϊ���飬�������õ��ĺ���rNF_IsBadBlock��rNF_MarkBadBlock�������ں����
  �ܡ��������ܽ�һ�¸ó�����������ֵ�ĺ��壬0x42����ʾ��ҳ���ڵĿ�Ϊ���飻0x43����
  ʾд����ʧ�ܣ������ڱ�ע��ҳ���ڵĿ�Ϊ����ʱҲʧ�ܣ�0x44����ʾд����ʧ�ܣ����Ǳ�ע
  ����ɹ���0x66��д�����ɹ�
*/

/*
  �������Կ�Ϊ��λ���еģ������д��ַ�����ǣ�ֻ��д���������ڣ�����Ҫ��A18��ʼд
  ����д����һ�����ڲ�������ǰ��Ҫ�ж��Ƿ���������ɹ�������ͬ��Ҳ������Ҫ�ж��Ƿ�
  Ϊ�����Լ�Ҫ��ע��������⡣����͸���һ�ξ���Ŀ������������
*/
U8 rNF_EraseBlock(U32 block_number)
{
    char stat, temp;
      
    temp = rNF_IsBadBlock(block_number);     //�жϸÿ��Ƿ�Ϊ����
    if(temp == 0x33)
        return 0x42;                         //�ǻ��飬����
      
    NF_nFCE_L();                             //��Ƭѡ
    NF_CLEAR_RB();                           //��RnB�ź�
    NF_CMD(CMD_ERASE1);                      //������������1
    //д��3����ַ���ڣ���A18��ʼд��
    NF_ADDR((block_number << 6) & 0xff);     //�е�ַA18~A19
    NF_ADDR((block_number >> 2) & 0xff);     //�е�ַA20~A27
    NF_ADDR((block_number >> 10) & 0xff);    //�е�ַA28
      
    NF_CMD(CMD_ERASE2);                      //������������2
    delay(1000);                             //��ʱһ��ʱ��
   
    NF_CMD(CMD_STATUS);                      //��״̬����
      
    //�ж�״ֵ̬�ĵ�6λ�Ƿ�Ϊ1�����Ƿ���æ��������������NF_DETECT_RB();��ͬ
    do{
        stat = NF_RDDATA8();
    }while(!(stat&0x40));
   
    NF_nFCE_H();                             //�ر�nandflashƬѡ

    //�ж�״ֵ̬�ĵ�0λ�Ƿ�Ϊ0��Ϊ0�����������ȷ���������
    if (stat & 0x1)
    {
        temp = rNF_MarkBadBlock(page_number>>6);         //��ע�ÿ�Ϊ����
        if (temp == 0x21)
            return 0x43;            //��ע����ʧ��
        else
            return 0x44;            //��������ʧ��
    }
    else
        return 0x66;                //���������ɹ�
}

/*
  �ó�����������ΪK9F2G08U0A�ĵڼ��飬��������Ҫ������2001�飬����øú���
  Ϊ��rNF_EraseBlock(2001)��
*/
/*
  K9F2G08U0A�����ṩ��ҳ����ҳд�����⣬���ṩ��ҳ�ڵ�ַ�������д���ܡ�ҳ��
  ��ҳд�Ǵ�ҳ���׵�ַ��ʼ����д�����������дʵ������һҳ��Χ�������ַ�Ķ���д����
  �����������ҳ����������������������ҳ���е�ַ�������Ϳ��Զ�ȡ���е�ַ��ָ����ַ
  �����ݡ�����д��������ҳд�����ĵڶ���ҳд��������ǰ����������д�����ҳ���е�ַ��
  �Լ�Ҫд������ݣ������Ϳ��԰�����д�뵽�е�ַ��ָ���ĵ�ַ�ڡ��������γ���ʵ������
  ���������д���ܣ�����������������������ֱ�Ϊҳ��ַ��ҳ�ڵ�ַ���������Ϊ����ȡ
  �������ݣ�����д�������������ֱ�Ϊҳ��ַ��ҳ�ڵ�ַ���Լ�Ҫд������ݡ�
*/
U8 rNF_RamdomRead(U32 page_number, U32 add)
{
    NF_nFCE_L();                                //��nandflashƬѡ
    NF_CLEAR_RB();                              //��RnB�ź�
    NF_CMD(CMD_READ1);                          //ҳ����������1
    //д��5����ַ����
    NF_ADDR(0x00);                              //�е�ַA0~A7
    NF_ADDR(0x00);                              //�е�ַA8~A11
    NF_ADDR((page_number) & 0xff);              //�е�ַA12~A19
    NF_ADDR((page_number >> 8) & 0xff);         //�е�ַA20~A27
    NF_ADDR((page_number >> 16) & 0xff);        //�е�ַA28
    NF_CMD(CMD_READ2);                          //ҳ����������2
    NF_DETECT_RB();                             //�ȴ�RnB�źű�ߣ�����æ
    NF_CMD(CMD_RANDOMREAD1);                    //�������������1
    //ҳ�ڵ�ַ
    NF_ADDR((char)(add&0xff));                  //�е�ַA0~A7
    NF_ADDR((char)((add>>8)&0x0f));             //�е�ַA8~A11
    NF_CMD(CMD_RANDOMREAD2);                    //�������������2
      
    return NF_RDDATA8();                        //��ȡ����
}

U8 rNF_RamdomWrite(U32 page_number, U32 add, U8 dat)
{
    U8 temp,stat;
    NF_nFCE_L();                                //��nandflashƬѡ
    NF_CLEAR_RB();                              //��RnB�ź�
    NF_CMD(CMD_WRITE1);                         //ҳд��������1
    //д��5����ַ����
    NF_ADDR(0x00);                              //�е�ַA0~A7
    NF_ADDR(0x00);                              //�е�ַA8~A11
    NF_ADDR((page_number) & 0xff);              //�е�ַA12~A19
    NF_ADDR((page_number >> 8) & 0xff);         //�е�ַA20~A27
    NF_ADDR((page_number >> 16) & 0xff);        //�е�ַA28
    NF_CMD(CMD_RANDOMWRITE);                    //����д����
    //ҳ�ڵ�ַ
    NF_ADDR((char)(add&0xff));                  //�е�ַA0~A7
    NF_ADDR((char)((add>>8)&0x0f));             //�е�ַA8~A11
    NF_WRDATA8(dat);                            //д������
    NF_CMD(CMD_WRITE2);                         //ҳд��������2
      
    delay(1000);                                //��ʱһ��ʱ��
    
    NF_CMD(CMD_STATUS);                         //��״̬����
   
    //�ж�״ֵ̬�ĵ�6λ�Ƿ�Ϊ1�����Ƿ���æ��������������NF_DETECT_RB();��ͬ
    do{
        stat =  NF_RDDATA8();
    }while(!(stat&0x40));
   
    NF_nFCE_H();                                //�ر�nandflashƬѡ
    
    //�ж�״ֵ̬�ĵ�0λ�Ƿ�Ϊ0��Ϊ0��д������ȷ���������
    if (stat & 0x1)
        return 0x44;                  //ʧ��
    else
        return 0x66;                  //�ɹ�
}

/*
  ��������������ᵽ���жϻ����Լ���ע���������������rNF_IsBadBlock��
  rNF_MarkBadBlock����������Ƕ�����spare���ĵ�6����ַ����ÿҳ�ĵ�2054��ַ������
  ��ע���飬0x44��ʾ�ÿ�Ϊ���顣Ҫ�жϻ���ʱ�������������������ȡ2054��ַ��������
  ��Ϊ0x44��Ҫ��ע����ʱ����������д��������2054��ַд0x33������͸�������������
  ���ǵ����������Ϊ���ַ��Ҳ���Ǽ�ʹ����һҳ�������⣬����Ҳ��ע������Ϊ���顣
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
        return 0x21;                  //д�����עʧ��
    else
        return 0x60;                  //д�����ע�ɹ�
}

/*
  ����nandflash�Ļ��������ͽ��⵽�����Ȼnandflash����һЩ�������ӵĲ���������
  ����ַ�������ַ��ת�������������ȣ���Щ���ݱ��ľͲ��ٽ����ˡ�
*/
