#currentdir =$(shell pwd)
currentdir = E:/Awork/EclipseWorkspace/sdboot6410/djyos/port

#��������Ǳ�д�Լ�����Ŀ¼���е�makefileʱΨһ��Ҫ�޸ĵģ�������ĵ���djyos��makefile�ļ�˵����
subdir =
boot_rom =initcpu.s memcfg.c nand.c uart.c main.c
rom_init =
preload =
sysload =
critical =

#���²���������makefile����ͬ�������ڹ��̸�Ŀ¼��һ��mksubinc�ļ�������makefile����
#  include $(topdir)/mksubinc�������������makeʱ�ᱨ�?
#  makefile:11: /cygdrive/g/siyf/mksubinc: No such file or directory
#���෽Ŭ�������޽���취��ֻ���Ƚ���֮��
ifneq ($(boot_rom),)
boot_rom_asm =$(filter %.s,$(boot_rom))
boot_rom_asmobj =$(patsubst  %.s,%.o,$(boot_rom_asm))
boot_rom_c =$(filter %.c,$(boot_rom))
boot_rom_cobj =$(patsubst  %.c,%.o,$(boot_rom_c))
boot_rom_obj =$(patsubst  %.o,$(currentdir)/%.o,$(boot_rom_asmobj) $(boot_rom_cobj))
endif

ifneq ($(rom_init),)
rom_init_asm =$(filter %.s,$(rom_init))
rom_init_asmobj =$(patsubst  %.s,%.o,$(rom_init_asm))
rom_init_c =$(filter %.c,$(rom_init))
rom_init_cobj =$(patsubst  %.c,%.o,$(rom_init_c))
rom_init_obj =$(patsubst  %.o,$(currentdir)/%.o,$(rom_init_asmobj) $(rom_init_cobj))
endif

ifneq ($(preload),)
preload_asm =$(filter %.s,$(preload))
preload_asmobj =$(patsubst  %.s,%.o,$(preload_asm))
preload_c =$(filter %.c,$(preload))
preload_cobj =$(patsubst  %.c,%.o,$(preload_c))
preload_obj =$(patsubst  %.o,$(currentdir)/%.o,$(preload_asmobj) $(preload_cobj))
endif

ifneq ($(sysload),)
sysload_asm =$(filter %.s,$(sysload))
sysload_asmobj =$(patsubst  %.s,%.o,$(sysload_asm))
sysload_c =$(filter %.c,$(sysload))
sysload_cobj =$(patsubst  %.c,%.o,$(sysload_c))
sysload_obj =$(patsubst  %.o,$(currentdir)/%.o,$(sysload_asmobj) $(sysload_cobj))
endif

ifneq ($(critical),)
critical_asm =$(filter %.s,$(critical))
critical_asmobj =$(patsubst  %.s,%.o,$(critical_asm))
critical_c =$(filter %.c,$(critical))
critical_cobj =$(patsubst  %.c,%.o,$(critical_c))
critical_obj =$(patsubst  %.o,$(currentdir)/%.o,$(critical_asmobj) $(critical_cobj))
endif

debug :$(subdir) $(preload_asmobj) $(preload_cobj) $(critical_asmobj) \
                  $(critical_cobj) $(sysload_asmobj) $(sysload_cobj) output_obj_list
boot_rom: $(subdir) $(boot_rom_asmobj) $(boot_rom_cobj) output_obj_list

run_inram :$(subdir) $(rom_init_asmobj) $(rom_init_cobj) $(preload_asmobj) \
                  $(preload_cobj) $(critical_asmobj) $(critical_cobj) $(sysload_asmobj) \
                  $(sysload_cobj) output_obj_list
run_inflash :$(subdir) $(rom_init_asmobj) $(rom_init_cobj) $(preload_asmobj) \
                  $(preload_cobj) $(critical_asmobj) $(critical_cobj) $(sysload_asmobj) \
                  $(sysload_cobj) output_obj_list

.PHONY: output_obj_list
output_obj_list:
	echo "hello world"
	@printf "rom_init_obj +=$(rom_init_obj)\n\
boot_rom_obj +=$(boot_rom_obj)\n\
critical_obj +=$(critical_obj)\n\
preload_obj +=$(preload_obj)\n\
sysload_obj +=$(sysload_obj)\n" >> $(topdir)/ld_obj_list

$(boot_rom_asmobj):%.o:%.S
	$(CC) $(ASFLAGS) $(incdir) $< -o $@
$(boot_rom_cobj)  : %.o:%.c
	$(CC) $(CFLAGS) $(incdir) $< -o $@
$(rom_init_asmobj):%.o:%.S
	$(CC) $(ASFLAGS) $(incdir) $< -o $@
$(rom_init_cobj)  : %.o:%.c
	$(CC) $(CFLAGS) $(incdir) $< -o $@
$(preload_asmobj):%.o:%.S
	$(CC) $(ASFLAGS) $(incdir) $< -o $@
$(preload_cobj)  : %.o:%.c
	$(CC) $(CFLAGS) $(incdir) $< -o $@
$(critical_asmobj):%.o:%.S
	$(CC) $(ASFLAGS) $(incdir) $< -o $@
$(critical_cobj)  : %.o:%.c
	$(CC) $(CFLAGS) $(incdir) $< -o $@
$(sysload_asmobj):%.o:%.S
	$(CC) $(ASFLAGS) $(incdir) $< -o $@
$(sysload_cobj)  : %.o:%.c
	$(CC) $(CFLAGS) $(incdir) $< -o $@

.PHONY:$(subdir)
$(subdir) :
	$(sub_make) -C $@ $(MAKECMDGOALS)

.PHONY: clean
clean:$(subdir)
	rm -f $(boot_rom_obj) $(rom_init_obj) $(sysload_obj) $(critical_obj) $(preload_obj)
