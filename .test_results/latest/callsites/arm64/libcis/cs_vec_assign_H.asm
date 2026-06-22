; call site: cs_vec_assign_H   lib=libcis   arch=arm64   size=64 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm64/libcis.o:     file format elf64-littleaarch64


Disassembly of section .text:

000000000000059c <cs_vec_assign_H>:
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:594
 59c:	eb01001f 	cmp	x0, x1
 5a0:	540001c0 	b.eq	5d8 <cs_vec_assign_H+0x3c>  // b.none
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 5a4:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
 5a8:	aa0003e4 	mov	x4, x0
 5ac:	910003fd 	mov	x29, sp
 5b0:	f9400025 	ldr	x5, [x1]
_ZNKSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:661
 5b4:	aa0503e0 	mov	x0, x5
 5b8:	97fffeaa 	bl	60 <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 5bc:	a8c17bfd 	ldp	x29, x30, [sp], #16
_ZSt8distanceIPK1HENSt15iterator_traitsIT_E15difference_typeES4_S4_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/iterator:1087
 5c0:	cb050003 	sub	x3, x0, x5
_ZNKSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:661
 5c4:	aa0003e2 	mov	x2, x0
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:635
 5c8:	aa0503e1 	mov	x1, x5
 5cc:	9344fc63 	asr	x3, x3, #4
 5d0:	aa0403e0 	mov	x0, x4
 5d4:	14000000 	b	0 <_ZNKSs8capacityEv.isra.0>
 5d8:	d65f03c0 	ret

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
