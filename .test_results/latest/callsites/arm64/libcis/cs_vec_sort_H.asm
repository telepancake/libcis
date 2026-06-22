; call site: cs_vec_sort_H   lib=libcis   arch=arm64   size=80 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm64/libcis.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000edc <cs_vec_sort_H>:
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
 edc:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
 ee0:	910003fd 	mov	x29, sp
_ZNSt6vectorI1HSaIS0_EE5beginEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:658
 ee4:	f9400004 	ldr	x4, [x0]
_ZNSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:660
 ee8:	aa0403e0 	mov	x0, x4
 eec:	97fffc63 	bl	78 <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/algorithm:2103
 ef0:	eb00009f 	cmp	x4, x0
 ef4:	54000180 	b.eq	f24 <cs_vec_sort_H+0x48>  // b.none
 ef8:	aa0003e1 	mov	x1, x0
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/algorithm:2113
 efc:	cb040000 	sub	x0, x0, x4
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
 f00:	a8c17bfd 	ldp	x29, x30, [sp], #16
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/algorithm:2113
 f04:	9344fc00 	asr	x0, x0, #4
_ZNSt6detail8bit_log2Ey():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/algorithm:2003
 f08:	dac01000 	clz	x0, x0
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/algorithm:2112 (discriminator 1)
 f0c:	528007e2 	mov	w2, #0x3f                  	// #63
 f10:	4b000042 	sub	w2, w2, w0
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/algorithm:2114
 f14:	52800023 	mov	w3, #0x1                   	// #1
 f18:	aa0403e0 	mov	x0, x4
 f1c:	9ac32042 	lsl	x2, x2, x3
 f20:	17fffe59 	b	884 <_ZNSt6detail9introsortIP1HSt4lessIvEEEvT_S5_RT0_NSt15iterator_traitsIS5_E15difference_typeEb.isra.0>
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
 f24:	a8c17bfd 	ldp	x29, x30, [sp], #16
 f28:	d65f03c0 	ret

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

Disassembly of section .text._ZNSt6vectorIiSaIiEE10grow_cold_Em:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE10grow_cold_Em:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_m.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_m:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:
