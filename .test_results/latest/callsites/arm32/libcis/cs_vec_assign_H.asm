; call site: cs_vec_assign_H   lib=libcis   arch=arm32   size=44 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

00000420 <cs_vec_assign_H>:
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
 420:	b510      	push	{r4, lr}
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:594
 422:	4288      	cmp	r0, r1
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
 424:	4604      	mov	r4, r0
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:594
 426:	d00d      	beq.n	444 <cs_vec_assign_H+0x24>
 428:	6809      	ldr	r1, [r1, #0]
_ZNKSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:661
 42a:	4608      	mov	r0, r1
 42c:	f7ff fe0e 	bl	4c <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
_ZSt8distanceIPK1HENSt15iterator_traitsIT_E15difference_typeES4_S4_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/iterator:1087
 430:	1a43      	subs	r3, r0, r1
_ZNKSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:661
 432:	4602      	mov	r2, r0
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:635
 434:	4804      	ldr	r0, [pc, #16]	@ (448 <cs_vec_assign_H+0x28>)
_ZSt8distanceIPK1HENSt15iterator_traitsIT_E15difference_typeES4_S4_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/iterator:1087
 436:	109b      	asrs	r3, r3, #2
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:635
 438:	4343      	muls	r3, r0
 43a:	4620      	mov	r0, r4
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
 43c:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:635
 440:	f7ff bffe 	b.w	0 <_ZNKSs8capacityEv.isra.0>
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
 444:	bd10      	pop	{r4, pc}
 446:	bf00      	nop
 448:	aaaaaaab 	.word	0xaaaaaaab

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
