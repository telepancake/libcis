; call site: cs_vec_insert_H   lib=libcis   arch=arm64   size=160 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm64/libcis.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000664 <cs_vec_insert_H>:
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 664:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
 668:	910003fd 	mov	x29, sp
 66c:	f90013f5 	str	x21, [sp, #32]
 670:	aa0103f5 	mov	x21, x1
_ZNSt6vectorI1HSaIS0_EE5beginEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:658
 674:	f9400001 	ldr	x1, [x0]
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 678:	a90153f3 	stp	x19, x20, [sp, #16]
 67c:	aa0003f4 	mov	x20, x0
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 680:	aa0103e0 	mov	x0, x1
 684:	97fffe66 	bl	1c <_ZNKSt6vectorI1HSaIS0_EE4sizeEv.isra.0>
 688:	aa0003f3 	mov	x19, x0
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
 68c:	b4000381 	cbz	x1, 6fc <cs_vec_insert_H+0x98>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688 (discriminator 1)
 690:	f85f0020 	ldur	x0, [x1, #-16]
 694:	d344fc00 	lsr	x0, x0, #4
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779 (discriminator 1)
 698:	cb130000 	sub	x0, x0, x19
 69c:	f100041f 	cmp	x0, #0x1
 6a0:	54000088 	b.hi	6b0 <cs_vec_insert_H+0x4c>  // b.pmore
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 6a4:	91000a61 	add	x1, x19, #0x2
 6a8:	aa1403e0 	mov	x0, x20
 6ac:	94000000 	bl	0 <_ZNKSs8capacityEv.isra.0>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
 6b0:	f9400280 	ldr	x0, [x20]
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 6b4:	aa1503e1 	mov	x1, x21
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
 6b8:	8b131013 	add	x19, x0, x19, lsl #4
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 6bc:	aa1303e0 	mov	x0, x19
 6c0:	94000000 	bl	0 <_ZNKSs8capacityEv.isra.0>
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:815
 6c4:	f9400283 	ldr	x3, [x20]
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
 6c8:	d2800206 	mov	x6, #0x10                  	// #16
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 6cc:	f94013f5 	ldr	x21, [sp, #32]
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
 6d0:	8b060265 	add	x5, x19, x6
 6d4:	aa1303e4 	mov	x4, x19
 6d8:	aa1403e2 	mov	x2, x20
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 6dc:	a94153f3 	ldp	x19, x20, [sp, #16]
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
 6e0:	8b060063 	add	x3, x3, x6
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 6e4:	a8c37bfd 	ldp	x29, x30, [sp], #48
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
 6e8:	90000001 	adrp	x1, 0 <_ZNKSs8capacityEv.isra.0>
 6ec:	90000000 	adrp	x0, 0 <_ZNKSs8capacityEv.isra.0>
 6f0:	91000021 	add	x1, x1, #0x0
 6f4:	91000000 	add	x0, x0, #0x0
 6f8:	14000000 	b	0 <_ZNSt6detail6rotateEPKNS_8type_opsEPKNS_11storage_opsEPvS6_S6_S6_l>
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687 (discriminator 1)
 6fc:	d2800000 	mov	x0, #0x0                   	// #0
 700:	17ffffe6 	b	698 <cs_vec_insert_H+0x34>

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
