; call site: cs_vec_push_int   lib=libcis   arch=arm64   size=68 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm64/libcis.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000558 <cs_vec_push_int>:
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
 558:	a9be7bfd 	stp	x29, x30, [sp, #-32]!
 55c:	910003fd 	mov	x29, sp
 560:	a90153f3 	stp	x19, x20, [sp, #16]
 564:	aa0003f3 	mov	x19, x0
 568:	2a0103f4 	mov	w20, w1
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:789
 56c:	d2800021 	mov	x1, #0x1                   	// #1
 570:	94000000 	bl	0 <_ZNKSs8capacityEv.isra.0>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:791
 574:	f9400262 	ldr	x2, [x19]
_ZSt12construct_atIiJRKiEEPT_S3_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19 (discriminator 1)
 578:	b8004414 	str	w20, [x0], #4
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:791
 57c:	cb020001 	sub	x1, x0, x2
_ZNSt6vectorIiSaIiEE15set_size_elems_Em():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:129
 580:	b4000042 	cbz	x2, 588 <cs_vec_push_int+0x30>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:136 (discriminator 1)
 584:	f81f8041 	stur	x1, [x2, #-8]
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
 588:	aa1303e0 	mov	x0, x19
 58c:	d2800021 	mov	x1, #0x1                   	// #1
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
 590:	a94153f3 	ldp	x19, x20, [sp, #16]
 594:	a8c27bfd 	ldp	x29, x30, [sp], #32
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
 598:	14000000 	b	0 <_ZNKSs8capacityEv.isra.0>

Disassembly of section .text._ZNKSt19bad_optional_access4whatEv:

Disassembly of section .text._ZNSt6detail17move_construct_opI1HSaIS1_EEEvPvS3_S3_:

Disassembly of section .text._ZNSt19bad_optional_accessD2Ev:

Disassembly of section .text._ZNSt19bad_optional_accessD0Ev:

Disassembly of section .text._ZN20libcis_atomic_detail15atomic_spinlock4lockEv:

Disassembly of section .text._ZN20libcis_atomic_detail15atomic_spinlock6unlockEv:

Disassembly of section .text._ZNK1HltERKS_:

Disassembly of section .text._ZNSs11vdeallocateEv:

Disassembly of section .text._ZNSt6detail10destroy_opI1HSaIS1_EEEvPvS3_:

Disassembly of section .text._ZNSs9vallocateEm:

Disassembly of section .text._ZNSs9init_copyEPKcm:

Disassembly of section .text._ZN1HaSERKS_.isra.0:

Disassembly of section .text._ZNSt6vectorIiSaIiEE12ensure_free_Em:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE12ensure_free_Em:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_m.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_m:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:
