; call site: cs_vec_erase_H   lib=libcis   arch=arm64   size=64 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm64/libcis.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000274 <cs_vec_erase_H>:
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 274:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
 278:	aa0003e2 	mov	x2, x0
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
 27c:	928001e6 	mov	x6, #0xfffffffffffffff0    	// #-16
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 280:	910003fd 	mov	x29, sp
_ZNSt6vectorI1HSaIS0_EE5beginEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:658
 284:	f9400003 	ldr	x3, [x0]
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:892
 288:	aa0303e0 	mov	x0, x3
 28c:	97ffff7b 	bl	78 <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 290:	a8c17bfd 	ldp	x29, x30, [sp], #16
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
 294:	aa0003e5 	mov	x5, x0
 298:	91008064 	add	x4, x3, #0x20
 29c:	90000001 	adrp	x1, 0 <_ZNKSs8capacityEv.isra.0>
 2a0:	91004063 	add	x3, x3, #0x10
 2a4:	91000021 	add	x1, x1, #0x0
 2a8:	90000000 	adrp	x0, 0 <_ZNKSs8capacityEv.isra.0>
 2ac:	91000000 	add	x0, x0, #0x0
 2b0:	14000000 	b	0 <_ZNSt6detail6rotateEPKNS_8type_opsEPKNS_11storage_opsEPvS6_S6_S6_l>

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
