; call site: cs_vec_push_H   lib=libcis   arch=arm32   size=44 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

00000460 <cs_vec_push_H>:
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
 460:	b570      	push	{r4, r5, r6, lr}
 462:	4605      	mov	r5, r0
 464:	460e      	mov	r6, r1
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:789
 466:	2101      	movs	r1, #1
 468:	f7ff fffe 	bl	0 <_ZNKSs8capacityEv.isra.0>
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19 (discriminator 1)
 46c:	4631      	mov	r1, r6
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:789
 46e:	4604      	mov	r4, r0
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19 (discriminator 1)
 470:	f7ff fffe 	bl	0 <_ZNKSs8capacityEv.isra.0>
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:791
 474:	682b      	ldr	r3, [r5, #0]
 476:	340c      	adds	r4, #12
 478:	1ae4      	subs	r4, r4, r3
_ZNSt6vectorI1HSaIS0_EE15set_size_elems_Ej():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:129
 47a:	b10b      	cbz	r3, 480 <cs_vec_push_H+0x20>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:136 (discriminator 1)
 47c:	f843 4c04 	str.w	r4, [r3, #-4]
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
 480:	4628      	mov	r0, r5
 482:	2101      	movs	r1, #1
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
 484:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
 488:	f7ff bffe 	b.w	0 <_ZNKSs8capacityEv.isra.0>

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
