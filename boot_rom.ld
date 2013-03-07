/*本文件是由makefile生成的*/ 
ENTRY(_start)
MEMORY
{  
	rom (rx)  : org = 0xc000000,   len = 8k
}
SECTIONS
{
	. = 0xc000000; 
	 
	.text : 
	{  
	   * (.text)
	} >rom 
	 
	.rodata : 
	{  
	   * (.rodata*)
	} >rom 
	 
	.data :  
	{  
	   * (.data)
	} >rom 
	 
	.bss : 
	{ 
	   * (.bss)
	} >rom 
	zi_sysload_limit = .;    
	 
	_ISR_STARTADDRESS = 0x0c7fff00;    
	USR_stack_size   =  0x1000;     /*用户模式栈,启动多事件调度前用，并非main的栈*/   
	SVC_stack_size   =  0x10000;   /*操作系统栈，中断也使用*/  
	Undef_stack_size =  0x100;     /*为定义指令异常栈*/    
	Abort_stack_size =  0x100;     /*数据异常栈*/  
	IRQ_stack_size   =  0x1000;    /*irq异常栈,可支持68次中断嵌套*/    
	FIQ_stack_size   =  0x0;        /*fiq栈,44b0x不使用fiq*/   
	 
	FIQ_stack        =  _ISR_STARTADDRESS; 
	IRQ_stack        =  FIQ_stack   - FIQ_stack_size;  
	Abort_stack      =  IRQ_stack   - IRQ_stack_size;  
	IRQ_stack_bottom =  Abort_stack - 4;  
	Undef_stack      =  Abort_stack - Abort_stack_size;    
	SVC_stack        =  Undef_stack - Undef_stack_size;    
	USR_stack        =  SVC_stack   - SVC_stack_size;  
	cfg_heap_top     =  (USR_stack - USR_stack_size)&( ~ 7);  /*向下对齐*/ 
	cfg_heap_bottom  =  (zi_sysload_limit + 7)&( ~ 7);          /*向上对齐*/ 
	 
	user_irq         =  _ISR_STARTADDRESS; 
	user_fiq         =  _ISR_STARTADDRESS + 4; 
	user_undef       =  _ISR_STARTADDRESS + 8; 
	user_swi         =  _ISR_STARTADDRESS + 12;    
	user_pabort      =  _ISR_STARTADDRESS + 16;    
	user_dabort      =  _ISR_STARTADDRESS + 20;    
	user_reserved    =  _ISR_STARTADDRESS + 24;    
	 
}  
