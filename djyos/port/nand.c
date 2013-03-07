/********************NAND****************************/
#define NFCONF *((volatile unsigned long*)0x70200000)
#define NFCONT *((volatile unsigned long*)0x70200004)
#define NFCMMD *((volatile unsigned long*)0x70200008)
#define NFADDR *((volatile unsigned long*)0x7020000C)
#define NFDATA *((volatile unsigned char*)0x70200010)
#define NFSTAT *((volatile unsigned long*)0x70200028)

#define MEM_SYS_CFG *((volatile unsigned long*)0x7E00F120)

#define TACLS  2  //7
#define TWRPH0 1  //7
#define TWRPH1 1  //7
#define PAGE_SIZE 2048
#define BLOCK_SIZE (PAGE_SIZE*64)

void nand_select(){
	NFCONT &= ~(1<<1);
}

void nand_deselect(){
	NFCONT |= 1<<1;
}

void nand_cmd(unsigned char cmd){
	NFCMMD = cmd;
}

void nand_addr(unsigned long addr){
	unsigned long row = addr/PAGE_SIZE;
	unsigned long column = addr%PAGE_SIZE;
	NFADDR = column&0xFF;
	NFADDR = (column>>8)&0xFF;
	NFADDR = row&0xFF;
	NFADDR = (row>>8)&0xFF;
	NFADDR = (row>>16)&0xFF;
}

void nand_ready(){
	while((NFSTAT&0x1) == 0);
}

void nand_reset(){
	nand_select();
	nand_cmd(0xff);
	nand_ready();
	nand_deselect();
}

void nand_read_status()
{
    int status;

    nand_cmd(0x70);
    status = NFDATA;

    if (status&0x01 == 0x01)
    {
        printf_str("\r\nCommand FAIL");
    }
    else
    {
        printf_str("\r\nCommand OK");
    }
}

#define GPOCON (*(volatile unsigned long*)(0x7F008140))
#define GPOPUD (*(volatile unsigned long*)(0x7F008148))
#define GPPCON (*(volatile unsigned long*)(0x7F008160))
#define GPPPUD (*(volatile unsigned long*)(0x7F008168))

void nand_init()
{
    GPOCON = (GPOCON & ~0xf) | 0xa;//nCS[3:2]
    GPOPUD &= ~0xf;
    GPPCON = (GPPCON & ~(0xfff << 4)) | (0xaaa << 4);//nWAIT, FALE, FCLE,
    GPPPUD &= ~(0xfff << 4);//FWEn, FREn, FRnB

    MEM_SYS_CFG &= ~(1<<1);
    NFCONF &= ~((0x7<<4)|(0x7<<8)|(0x7<<12)|(1<<30));
    NFCONF |= (TWRPH1<<4)|(TWRPH0<<8)|(TACLS<<12);
    NFCONT |= 1;
    NFCONT &= ~(1<<16);
    nand_reset();
}
/*
 * nand_start: must be the start of the page
 */
int nand_read(unsigned int nand_start, unsigned int ddr_start, unsigned int len)
{
    unsigned long rest = len;
    unsigned long addr = nand_start;
    unsigned long page;
    unsigned char *dest = (unsigned char *)ddr_start;
    int i;

    nand_select();
    while(rest > 0){
        nand_cmd(0x00);
        nand_addr(addr);
        nand_cmd(0x30);
        nand_ready();

        page = rest>PAGE_SIZE?PAGE_SIZE:rest;
        for(i = 0; i != page; ++i){
            *dest++ = NFDATA;
        }
        rest -= page;
        addr += page;
    }
    nand_deselect();

    return 0;
}

void nand_erase_block(unsigned long addr)
{

}

void nand_erase(unsigned int addr)
{
    int page = addr/PAGE_SIZE;
    //int blocks = size/BLOCK_SIZE;
    //int i = 0;

    nand_select();
    nand_cmd(0x60);
    NFADDR = page&0xff;
    NFADDR = (page>>8)&0xff;
    NFADDR = (page>>16)&0xff;
    nand_cmd(0xd0);
    nand_ready();
    nand_read_status();
    nand_deselect();
}

/*
 * nand_start: must be at the start of page
 */
void nand_write(unsigned int nand_start, unsigned char * buf, unsigned int len)
{
	unsigned long count = 0;
	unsigned long addr  = nand_start;
	int i = nand_start % PAGE_SIZE;

	nand_select();
	while (count < len)
	{
		nand_cmd(0x80);
		nand_addr(addr);
		for (; i < PAGE_SIZE && count < len; i++)
		{
                    NFDATA = buf[count++];
                    addr++;
		}
		nand_cmd(0x10);
		nand_ready();
                nand_read_status();
		i = 0;
	}
	nand_deselect();
}

void copy2ddr(unsigned long length){
	unsigned long rest = length;
	unsigned long size;
	unsigned long i;

	for(i = 0; i != 4; ++i){
		size = rest>2048?2048:rest;
		nand_read(PAGE_SIZE*i, 0x50000000+i*2048, size);
		rest -= size;
		if(rest == 0)
			return;
	}

	nand_read(PAGE_SIZE*4, 0x50000000+PAGE_SIZE, rest);
}

void store2nand(unsigned long ddr_start, unsigned long length){
	unsigned char* src = (unsigned char*)ddr_start;
	unsigned long rest = length;
	unsigned long size;
	unsigned long i;
	unsigned long blocks = (length+8*1024+0x100000-1)/0x100000;

	for(i = 0; i != blocks; ++i){
		//nand_erase(i*0x100000);
	}

	for(i = 0; i != 4; ++i){
		size = rest>2048?2048:rest;
		nand_write(PAGE_SIZE*i, src+2048*i, size);
		rest -= size;
		if(rest == 0)
			return;
	}

	nand_write(PAGE_SIZE*4, src+2048*4, rest);
}

void clear_bss(unsigned long begin, unsigned long length){
	unsigned long* dst = (unsigned long*)begin;
	unsigned long count = (length+3)/4;

	while(count--){
		*dst++ = 0;
	}
}
