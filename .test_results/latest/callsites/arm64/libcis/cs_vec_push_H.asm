; call site: cs_vec_push_H   lib=libcis   arch=arm64   size=88 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm64/libcis.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000640 <cs_vec_push_H>:
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
 640:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
 644:	910003fd 	mov	x29, sp
 648:	a90153f3 	stp	x19, x20, [sp, #16]
 64c:	aa0003f4 	mov	x20, x0
 650:	f90013f5 	str	x21, [sp, #32]
 654:	aa0103f5 	mov	x21, x1
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:789
 658:	d2800021 	mov	x1, #0x1                   	// #1
 65c:	94000000 	bl	0 <_ZNKSs8capacityEv.isra.0>
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19 (discriminator 1)
 660:	aa1503e1 	mov	x1, x21
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:789
 664:	aa0003f3 	mov	x19, x0
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19 (discriminator 1)
 668:	94000000 	bl	0 <_ZNKSs8capacityEv.isra.0>
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:791
 66c:	91004273 	add	x19, x19, #0x10
 670:	f9400280 	ldr	x0, [x20]
 674:	cb000273 	sub	x19, x19, x0
_ZNSt6vectorI1HSaIS0_EE15set_size_elems_Em():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:129
 678:	b4000040 	cbz	x0, 680 <cs_vec_push_H+0x40>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:136 (discriminator 1)
 67c:	f81f8013 	stur	x19, [x0, #-8]
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
 680:	f94013f5 	ldr	x21, [sp, #32]
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
 684:	aa1403e0 	mov	x0, x20
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
 688:	a94153f3 	ldp	x19, x20, [sp, #16]
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
 68c:	d2800021 	mov	x1, #0x1                   	// #1
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
 690:	a8c37bfd 	ldp	x29, x30, [sp], #48
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
 694:	14000000 	b	0 <_ZNKSs8capacityEv.isra.0>

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
