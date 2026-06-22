; call site: cs_vec_erase_H   lib=libcis   arch=arm32   size=76 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

0000019c <cs_vec_erase_H>:
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 19c:	b530      	push	{r4, r5, lr}
 19e:	4601      	mov	r1, r0
_ZNSt6vectorI1HSaIS0_EE5beginEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:658
 1a0:	6804      	ldr	r4, [r0, #0]
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 1a2:	b087      	sub	sp, #28
 1a4:	4d0d      	ldr	r5, [pc, #52]	@ (1dc <cs_vec_erase_H+0x40>)
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:892
 1a6:	4620      	mov	r0, r4
 1a8:	f7ff ff50 	bl	4c <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
 1ac:	4b0c      	ldr	r3, [pc, #48]	@ (1e0 <cs_vec_erase_H+0x44>)
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 1ae:	447d      	add	r5, pc
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
 1b0:	460a      	mov	r2, r1
 1b2:	58eb      	ldr	r3, [r5, r3]
 1b4:	9304      	str	r3, [sp, #16]
 1b6:	4b0b      	ldr	r3, [pc, #44]	@ (1e4 <cs_vec_erase_H+0x48>)
 1b8:	58eb      	ldr	r3, [r5, r3]
 1ba:	9305      	str	r3, [sp, #20]
 1bc:	f06f 030b 	mvn.w	r3, #11
 1c0:	e9cd 0301 	strd	r0, r3, [sp, #4]
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:892
 1c4:	f104 0318 	add.w	r3, r4, #24
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
 1c8:	e9dd 1004 	ldrd	r1, r0, [sp, #16]
 1cc:	9300      	str	r3, [sp, #0]
 1ce:	f104 030c 	add.w	r3, r4, #12
 1d2:	f7ff fffe 	bl	0 <_ZNSt6detail6rotateEPKNS_8type_opsEPKNS_11storage_opsEPvS6_S6_S6_i>
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 1d6:	b007      	add	sp, #28
 1d8:	bd30      	pop	{r4, r5, pc}
 1da:	bf00      	nop
 1dc:	0000002a 	.word	0x0000002a
	...

Disassembly of section .text._ZNSt6detail17move_construct_opI1HSaIS1_EEEvPvS3_S3_:

Disassembly of section .text._ZN20libcis_atomic_detail15atomic_spinlock4lockEv:

Disassembly of section .text._ZN20libcis_atomic_detail15atomic_spinlock6unlockEv:

Disassembly of section .text._ZNK1HltERKS_:

Disassembly of section .text._ZNSs11vdeallocateEv:

Disassembly of section .text._ZNSt6detail10destroy_opI1HSaIS1_EEEvPvS3_:

Disassembly of section .text._ZNSs9vallocateEj:

Disassembly of section .text._ZNSs9init_copyEPKcj:

Disassembly of section .text._ZN1HaSERKS_.isra.0:

Disassembly of section .text._ZNSt6vectorIiSaIiEE10grow_cold_Ej:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE10grow_cold_Ej:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_j.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_j:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:
