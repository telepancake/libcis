; call site: cs_vec_insert_H   lib=libcis   arch=arm64   size=100 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm64/libcis.o:     file format elf64-littleaarch64


Disassembly of section .text:

00000000000005dc <cs_vec_insert_H>:
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 5dc:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
 5e0:	910003fd 	mov	x29, sp
 5e4:	a90153f3 	stp	x19, x20, [sp, #16]
 5e8:	aa0003f3 	mov	x19, x0
 5ec:	f90013f5 	str	x21, [sp, #32]
 5f0:	aa0103f5 	mov	x21, x1
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:810
 5f4:	d2800041 	mov	x1, #0x2                   	// #2
 5f8:	94000000 	bl	0 <_ZNKSs8capacityEv.isra.0>
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19 (discriminator 1)
 5fc:	aa1503e1 	mov	x1, x21
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:810
 600:	aa0003f4 	mov	x20, x0
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19 (discriminator 1)
 604:	94000000 	bl	0 <_ZNKSs8capacityEv.isra.0>
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:812
 608:	f9400263 	ldr	x3, [x19]
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
 60c:	d2800206 	mov	x6, #0x10                  	// #16
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 610:	f94013f5 	ldr	x21, [sp, #32]
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
 614:	8b060285 	add	x5, x20, x6
 618:	aa1403e4 	mov	x4, x20
 61c:	aa1303e2 	mov	x2, x19
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 620:	a94153f3 	ldp	x19, x20, [sp, #16]
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
 624:	8b060063 	add	x3, x3, x6
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 628:	a8c37bfd 	ldp	x29, x30, [sp], #48
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
 62c:	90000001 	adrp	x1, 0 <_ZNKSs8capacityEv.isra.0>
 630:	90000000 	adrp	x0, 0 <_ZNKSs8capacityEv.isra.0>
 634:	91000021 	add	x1, x1, #0x0
 638:	91000000 	add	x0, x0, #0x0
 63c:	14000000 	b	0 <_ZNSt6detail6rotateEPKNS_8type_opsEPKNS_11storage_opsEPvS6_S6_S6_l>

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
