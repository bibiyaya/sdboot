#export topdir =$(shell pwd)
export topdir = E:/Awork/EclipseWorkspace/sdboot6410
include ./djyos/port/arch.def

#�ڴ��г����а�Դ�ļ�����Ŀ¼����Ŀ¼����Ŀ¼�����г�
subdir = djyos

#'('��$֮�䲻���пո�
ifeq ($(MAKECMDGOALS),boot_rom) #�����ʼ��Ӳ�����֣��ʺϷ���������
    target = boot_rom
    op =-O2
    dbg = -gdwarf-2 -Dboot
endif

#'('��'$'֮�䲻���пո�
ifeq ($(MAKECMDGOALS),)
    target = debug
    op =
    dbg = -gdwarf-2 -Ddebug
endif

#���غ����ж���ram����������boot_rom֧�֡��ȱ���boot_rom����
#����debug_ram����boot_romд��flash���ϵ�ִ�У�Ȼ���÷���������debug_ram.elf���ԡ�
ifeq ($(MAKECMDGOALS),debug)
    target = debug
    op =
    dbg = -gdwarf-2 -Ddebug
endif

#��¼��flash�У�����ʱ����bootloader�Ѵ���cp��ram������
#'('��$֮�䲻���пո�
ifeq ($(MAKECMDGOALS),run_inram)
    target = run_inram
    op =-O2
    dbg =
endif

#��¼��flash�У�����ֱ����flash������
#'('��$֮�䲻���пո�
ifeq ($(MAKECMDGOALS),run_inflash)   #���غ����ж���flash
    target = run_inflash
    op =-O2
    dbg =
endif

export target
export CC = arm-none-eabi-gcc
export AS = arm-none-eabi-gcc
export objcopy = arm-none-eabi-objcopy -I elf32-littlearm -O binary
export objdump = arm-none-eabi-objdump -D
export size = arm-none-eabi-size
export incdir =-I$(topdir)/djyos/include -I$(topdir)/djyos/port/include

export sub_make = @make --no-print-directory
export CFLAGS  = -c -mcpu=$(CPU) -Wall $(dbg) $(op) -nostartfiles -std=gnu99 #-pedantic
export ASFLAGS = -c -mcpu=$(CPU) -Wall $(dbg) $(op)    #-feliminate-dwarf2-dups
export LDFLAGS = -mcpu=$(CPU) -Wall -nostartfiles -lm -lc

debug \
run_inram \
run_inflash \
boot_rom: rm_obj_list $(subdir)     #rm_obj_list $(subdir)����д˳���ܵߵ�
	$(sub_make) -f make_ld $(MAKECMDGOALS)

#rom_obj��preload_obj��sysload_obj��critical_obj�⼸������ָʾ�õı����ڸ���Ŀ¼
#����ɲ�������ld_obj�ļ��У�����ɾ��֮���Ա�������ɸ��ļ�
.PHONY: rm_obj_list
rm_obj_list:
	rm -f ld_obj_list

#�������Ŀ¼���룬��Ŀ¼�г�����Ŀ¼�µ�Դ�ļ���.o�⣬������Ŀ¼�¸��ļ���
#�����������ld_obj�ļ�
.PHONY: $(subdir)
$(subdir) :
	$(sub_make) -C $@ $(MAKECMDGOALS)

.PHONY: clean
clean:$(subdir)
	rm -f *.ld ld_obj_list
