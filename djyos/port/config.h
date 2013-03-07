//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:�ѹ���ģ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: �ں�����ͷ�ļ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef _PORT_CONFIG_H_
#define _PORT_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//��Ӳ����ص�����
#define M   1000000
#define cn_mclk         (266*M)         //mʱ��
#define cn_hclkx2       (266*M)         //DDRʱ��
#define cn_hclk         (133*M)         //������������(ahb)ʱ��=133M
#define cn_pclk         66500000        //������������(apb)ʱ��=66.5M
#define cn_eclk         84666667        //�ⲿʱ��=84.67M
#define cn_timer_clk    cn_pclk         //��ʱ��ʱ��Դ

//�洢����С�˵�����
#define cn_little_endian       0
#define cn_big_endian          1
#define cn_cpu_endian          cn_little_endian

//��CPU�ֳ���ص�����
#define cn_cpu_bits     32  //�������ֳ�
#define cn_point_bit    32  //CPU��ַλ������ֲ�ؼ�
#define cn_cpu_bits_suffix_zero 5
#define cn_byte_wide    8   //�ֽ�λ��

typedef unsigned  long long uint64_t;
typedef signed    long long sint64_t;
typedef unsigned  int       uint32_t;
typedef unsigned  short     uint16_t;
typedef           int       sint32_t;
typedef short     int       sint16_t;
typedef unsigned  char      uint8_t;
typedef signed    char      sint8_t;
//ptu32_t������cpu��Ѱַ��Χ�йأ�Ҳ��CPU�Ľṹ�йأ������ϵͳ��ָ�볤��С�ڻ�
//����32λ����ptu32_t������Ϊ32λ��������ָ��ȳ���������;�ж�:
//1.�����ڴ�����ָ���޶�,�������������ڴ泤�ȡ�
//2.���ڿ�����Ҫת����ָ��ʹ�õ��������ݣ�����struct pan_device�ṹ�е�
//  ptu32_t  private_tag
//һ����˵ptu32_t��ucpu_t��ͬ,��Ҳ������,������8λ����16λ����,����51����ucpu_t
//��8λ��,��Ѱַ��Χ��16λ��,ptu32_t=16λ.80c296��Ƭ����ucpu_t��16λ�ģ�Ѱַ��Χ
//��24λ�ģ�ptu32_t=32λ��
typedef uint32_t            ptu32_t;

//������CPU������һ��ָ�����������ͣ���������Ҫ�������������ԵĲ������Լ�
//��Ҫ���ٵĲ�����
//���������Ժ�ԭ�Ӳ�������ͬ:
//������������ָ���κ�ʱ���ܶ�����ȷֵ�����ݣ������������:
//  uint32_t    b32;
//  b32++;  //b32��ԭֵ��0x0000ffff
//��32λrisc���ϣ�ִ�й���Ϊ��
//ȡb32��ַ���Ĵ���0-->ȡb32����ֵ���Ĵ���1-->�Ĵ���1��1-->�ѼĴ���1д��b32.
//�����������������ﱻ�жϻ��߱������ȼ��̴߳�ϣ����жϻ�����ȼ��߳��ж�b32��
//Ҫô�õ�0x0000ffff��Ҫô�õ�0x00010000,������������Ϊ����ȷ����.
//����16λ���ϣ�ִ�й�����
//1��ȡ0x0000���Ĵ���r0��ȡ0xffff���Ĵ���r1
//2��ִ�мӲ�����ʹr0=0x0001,r1=0x0000
//3����r0д��b32�ĸ�16λ��
//4����r1д��b32�ĵ�16λ��
//����ڵ�3�͵�4����֮�䱻�жϴ�ϣ����ж����b32�����õ������0x00001ffff��
//��ôԭ�Ӳ�����?��������ִ�й��̲����κ��жϻ��߸����ȼ��̴߳�ϣ�����������룬
//���b32������Ϊԭ�ӱ�������������16λ����32λ���ϣ�b32�����Եõ���ȷ��ֵ��Ŀ
//ǰ��djyos����֧��ԭ�ӱ�����ֻ��ʹ�û�����ʵ��ԭ�ӱ�����Ч����
typedef uint32_t        ucpu_t;
    #define cn_limit_ucpu   cn_limit_uint32
    #define cn_min_ucpu     (0)
typedef sint32_t        scpu_t;
    #define cn_limit_scpu   cn_limit_sint32
    #define cn_min_scpu     cn_min_sint32
//�����ǿ����������ͣ�һ����˵��CPU�����ǿ����������ͣ�����Щϵͳ�洢�����С��
//CPU�ֳ������´Ӵ洢����һ��cpu�ֳ�������Ҫ����洢�����ڣ�����,16λ��32λcpu
//��,�ֽڲ���Ҳ����һ�����.������Щ���ֳ�û��Ҫ�������������˵��ʹ����洢��
//��ȵȳ��ı���Ҳ��������ٵġ�����16λSDRAM��44b0x����������16λ��.
typedef uint16_t        ufast_t;
    #define cn_limit_ufast  cn_limit_uint16
    #define cn_min_ufast    (0)
typedef sint16_t        sfast_t;
    #define cn_limit_sfast  cn_limit_sint16
    #define cn_min_sfast    cn_min_sint16

typedef ufast_t    bool_t;
#define true    1
#define false   0

//���¶���tick����
#define cn_tick_ms      1       //����ϵͳ�ں�ʱ�����峤�ȣ��Ժ���Ϊ��λ��
#define cn_tick_hz      1000    //�ں�ʱ��Ƶ�ʣ���λΪhz��
#define cn_fine_us      1       //����ϵͳ�ں˾���ʱ�����峤�ȣ���΢��Ϊ��λ��
                                //����ʾ����һ��tick���忪ʼ�������ŵ�ʱ������
#define cn_fine_hz      1000000 //�ں˾���ʱ��Ƶ�ʣ���cn_fine_us�ĵ�����

//���涨��cpuϵͳ��ջ����
#define cn_full_down_stack  0   //������������ջ
#define cn_empty_down_stack 1   //���������Ŀ�ջ
#define cn_full_up_stack    2   //������������ջ
#define cn_empty_up_stack   3   //���������Ŀ�ջ
#define cn_stack_type       cn_full_down_stack

//�����Ƿ�֧��mmu���ڲ�֧��mmu��cpu�ϣ�������Ϊfalse����֧��mmu�Ļ����ϣ���
//����Ƿ����ʹ��mmu���ж�̬�ڴ�ӳ�䣬������Ϊtrue��������Ϊfalse��
//�˳����붯̬�ڴ�����㷨�йء�
#define cn_mmu          false       //44b0x��֧��mmu
#define cn_phy_mem      0x800000    //�����ڴ�������֧��mmu�������塣

//cpu����ģʽ����
#define cn_mode_si          0   //��ӳ��ģʽ
#define cn_mode_dlsp        1   //��̬���ص�����ģʽ
#define cn_mode_mp          2   //�����ģʽ��ֻ��mmu=true�ſ�ѡ���ģʽ
#define cn_run_mode         cn_mode_si

#define cn_device_limit     20     //�����豸������
#define cn_handle_limit     20     //�����豸���������
#define cn_locks_limit      20     //��������п��õ���������������������ָ�û�
                //����semp_creat��mutex_create����������������Ƕ�뵽�豸�е�����
#define cn_events_limit     20     //�¼�����
#define cn_evtts_limit      16     //���¼�������
#define cn_wdt_limit        5      //������������

//���ڴ������صĳ���
#define cn_page_size        0x1000      //��ҳ�ߴ�Ϊ4Kbytes
#define cn_page_size_suffix_zero    12  //ҳ�ߴ��׺0�ĸ���
#define cn_block_limit      0x400000    //����ߴ�
#define cn_mem_recycle      true        //֧���ڴ����----���ù���
#define cn_mem_pools        (10)        //������10���ڴ��

//��ͬʱ�򿪵��ļ�������ע��༶·����ÿһ������һ���ļ�������ͬ��·�����ظ���
#define cn_opened_fsnode_limit   16

//�����ں�ջ����ȼ��㷽��:ջ����������Ǹ�ϵͳ����+sizeof(struct thread_vm)
//djyos for 44b0x��ֲ�汾�й̶���Ϊ8Kbytes��
#define cn_kernel_stack     0x2000 //�����ں�ջ���߳���ҪΪϵͳ����������ӵ�ջ

//����һЩ�������صĺ�
#define mem_align           1           //���Ŀ��ϵͳû�ж���Ҫ�󣬸�Ϊ0

#define align_down_2(x)     ((x)&(~1))        //2�ֽ�,���¶���,
#define align_down_4(x)     ((x)&(~3))        //4�ֽ�,���¶���,
#define align_down_8(x)     ((x)&(~7))        //8�ֽ�,���¶���,
#define align_down_16(x)    ((x)&(~15))       //16�ֽ�,���¶���,
#define align_up_2(x)       (((x)+1)&(~1))    //2�ֽ�,���϶���,
#define align_up_4(x)       (((x)+3)&(~3))    //4�ֽ�,���϶���,
#define align_up_8(x)       (((x)+7)&(~7))    //8�ֽ�,���϶���,
#define align_up_16(x)      (((x)+15)&(~15))  //16�ֽ�,���϶���,

//����������ϵͳ�����ݶ��뷽ʽ����ֲ�ؼ�
#define align_down(x)       align_down_8(x)   //gcc-armҪ��8�ֽڶ���
#define align_up(x)         align_up_8(x)     //gcc-armҪ��8�ֽڶ���

//���ж���ص�����
#define cn_int_num  26  //44b0x��26���ж�Դ.

//�������
#define cfg_djyfs           1   //�Ƿ�����ļ�ϵͳ
#define cfg_djyfs_flash     1   //�Ƿ����flash�ļ�ϵͳ������
#define cfg_djyfs_ram       0   //�Ƿ����ram�ļ�ϵͳ

#ifdef __cplusplus
}
#endif
#endif // _PORT_CONFIG_H_


