; call site: cs_vec_erase_H   lib=libcis   arch=arm32   size=76 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

00000184 <cs_vec_erase_H>:
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 184:	b530      	push	{r4, r5, lr}
 186:	4601      	mov	r1, r0
_ZNSt6vectorI1HSaIS0_EE5beginEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:658
 188:	6804      	ldr	r4, [r0, #0]
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 18a:	b087      	sub	sp, #28
 18c:	4d0d      	ldr	r5, [pc, #52]	@ (1c4 <cs_vec_erase_H+0x40>)
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:889
 18e:	4620      	mov	r0, r4
 190:	f7ff ff56 	bl	40 <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
 194:	4b0c      	ldr	r3, [pc, #48]	@ (1c8 <cs_vec_erase_H+0x44>)
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 196:	447d      	add	r5, pc
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
 198:	460a      	mov	r2, r1
 19a:	58eb      	ldr	r3, [r5, r3]
 19c:	9304      	str	r3, [sp, #16]
 19e:	4b0b      	ldr	r3, [pc, #44]	@ (1cc <cs_vec_erase_H+0x48>)
 1a0:	58eb      	ldr	r3, [r5, r3]
 1a2:	9305      	str	r3, [sp, #20]
 1a4:	f06f 030b 	mvn.w	r3, #11
 1a8:	e9cd 0301 	strd	r0, r3, [sp, #4]
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:889
 1ac:	f104 0318 	add.w	r3, r4, #24
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
 1b0:	e9dd 1004 	ldrd	r1, r0, [sp, #16]
 1b4:	9300      	str	r3, [sp, #0]
 1b6:	f104 030c 	add.w	r3, r4, #12
 1ba:	f7ff fffe 	bl	0 <_ZNSt6detail6rotateEPKNS_8type_opsEPKNS_11storage_opsEPvS6_S6_S6_i>
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 1be:	b007      	add	sp, #28
 1c0:	bd30      	pop	{r4, r5, pc}
 1c2:	bf00      	nop
 1c4:	0000002a 	.word	0x0000002a
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

Disassembly of section .text._ZNSt6vectorIiSaIiEE12ensure_free_Ej:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_j.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_j:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:
