# 完成上下文切换的任务，汇编完成更加高效（该功能将被频繁调用）

# gp, tp 不需要保存，这两个寄存器被用于特殊目的，tp用于存储hartid，

 # 定义一个reg_save宏，base为参数
.macro reg_save base
    # 将ra存储到 base 位置, 用参数表示寄存器需加 \
    sw ra, 0(\base)   
    # 指针和数据中间隔一段地址
    sw sp, 4(\base)   
	sw t0, 16(\base)
	sw t1, 20(\base)
	sw t2, 24(\base)
	sw s0, 28(\base)
	sw s1, 32(\base)
	sw a0, 36(\base)
	sw a1, 40(\base)
	sw a2, 44(\base)
	sw a3, 48(\base)
	sw a4, 52(\base)
	sw a5, 56(\base)
	sw a6, 60(\base)
    

	sw a7, 64(\base)
	sw s2, 68(\base)
	sw s3, 72(\base)
	sw s4, 76(\base)
	sw s5, 80(\base)
	sw s6, 84(\base)
	sw s7, 88(\base)
	sw s8, 92(\base)
	sw s9, 96(\base)
	sw s10, 100(\base)
	sw s11, 104(\base)
	sw t3, 108(\base)
	sw t4, 112(\base)
	sw t5, 116(\base)
	# we don't save t6 here, due to we have used
	# it as base, we have to save t6 in an extra step
	# outside of reg_save
.endm

.macro reg_restore reg_base  // 恢复寄存器的宏
    lw ra, 0(\reg_base)   
    lw sp, 4(\reg_base)   
    lw t0, 16(\reg_base)
    lw t1, 20(\reg_base)
    lw t2, 24(\reg_base)
    lw s0, 28(\reg_base)
    lw s1, 32(\reg_base)
    lw a0, 36(\reg_base)
    lw a1, 40(\reg_base)
    lw a2, 44(\reg_base)
    lw a3, 48(\reg_base)
    lw a4, 52(\reg_base)
    lw a5, 56(\reg_base)
    lw a6, 60(\reg_base)
    lw a7, 64(\reg_base)
    lw s2, 68(\reg_base)
    lw s3, 72(\reg_base)
    lw s4, 76(\reg_base)
    lw s5, 80(\reg_base)
    lw s6, 84(\reg_base)   
    lw s7, 88(\reg_base)
    lw s8, 92(\reg_base)
    lw s9, 96(\reg_base)
    lw s10, 100(\reg_base)
    lw s11, 104(\reg_base)
    lw t3, 108(\reg_base)
    lw t4, 112(\reg_base)
    lw t5, 116(\reg_base)
    lw t6, 120(\reg_base)
# t6 用于存储保存在内存中的上下文的起始地址
.endm

.text

# void switch_to(struct context *next)
# 定义 switch_to 全局变量，在c文件中将该变量当成函数地址来使用
.global switch_to 
.align 4
switch_to:
    # 上下文的切换通过更改 mscratch(保存当前上下文在内存中的地址) 特权寄存器来实现
    # store last context
    # t6 保存当前任务的上下文地址
    csrrw t6, mscratch, t6 // swap t6, mscratch
    beqz t6, 1f 
    // lw ra, 0(t6)
    reg_save t6

    mv t5, t6
    csrr t6, mscratch
    sw t6, 120(t5)

1:
    # 将a0存入到
    csrw mscratch, a0   # a0存放的是传递过来的next
    mv t6, a0
    reg_restore t6     # t6 == a0 为next的上下文指针，更改玩mscratch之后，进行上下文恢复

    # 上下文转换成功，返回ra继续执行
    ret

    # restore next context

.end