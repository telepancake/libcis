; call site: cs_vec_assign_H   lib=libcis   arch=arm32   size=42 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

000003e6 <cs_vec_assign_H>:
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 3e6:	b510      	push	{r4, lr}
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:594
 3e8:	4288      	cmp	r0, r1
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 3ea:	4604      	mov	r4, r0
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:594
 3ec:	d00d      	beq.n	40a <cs_vec_assign_H+0x24>
 3ee:	6809      	ldr	r1, [r1, #0]
_ZNKSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:661
 3f0:	4608      	mov	r0, r1
 3f2:	f7ff fe25 	bl	40 <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
_ZSt8distanceIPK1HENSt15iterator_traitsIT_E15difference_typeES4_S4_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/iterator:1087
 3f6:	1a43      	subs	r3, r0, r1
_ZNKSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:661
 3f8:	4602      	mov	r2, r0
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:635
 3fa:	4804      	ldr	r0, [pc, #16]	@ (40c <cs_vec_assign_H+0x26>)
_ZSt8distanceIPK1HENSt15iterator_traitsIT_E15difference_typeES4_S4_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/iterator:1087
 3fc:	109b      	asrs	r3, r3, #2
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:635
 3fe:	4343      	muls	r3, r0
 400:	4620      	mov	r0, r4
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 402:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:635
 406:	f7ff bffe 	b.w	0 <_ZNKSs8capacityEv.isra.0>
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 40a:	bd10      	pop	{r4, pc}
 40c:	aaaaaaab 	.word	0xaaaaaaab

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
