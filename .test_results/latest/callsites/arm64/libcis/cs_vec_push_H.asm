; call site: cs_vec_push_H   lib=libcis   arch=arm64   size=196 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm64/libcis.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000704 <cs_vec_push_H>:
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 704:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
 708:	910003fd 	mov	x29, sp
 70c:	f90013f5 	str	x21, [sp, #32]
 710:	aa0103f5 	mov	x21, x1
 714:	f9400001 	ldr	x1, [x0]
 718:	a90153f3 	stp	x19, x20, [sp, #16]
 71c:	aa0003f4 	mov	x20, x0
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 720:	aa0103e0 	mov	x0, x1
 724:	97fffe3e 	bl	1c <_ZNKSt6vectorI1HSaIS0_EE4sizeEv.isra.0>
 728:	aa0003f3 	mov	x19, x0
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
 72c:	b40003e1 	cbz	x1, 7a8 <cs_vec_push_H+0xa4>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688 (discriminator 1)
 730:	f85f0020 	ldur	x0, [x1, #-16]
 734:	d344fc00 	lsr	x0, x0, #4
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779 (discriminator 1)
 738:	eb00027f 	cmp	x19, x0
 73c:	54000081 	b.ne	74c <cs_vec_push_H+0x48>  // b.any
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 740:	91000661 	add	x1, x19, #0x1
 744:	aa1403e0 	mov	x0, x20
 748:	94000000 	bl	0 <_ZNKSs8capacityEv.isra.0>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
 74c:	f9400280 	ldr	x0, [x20]
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 750:	aa1503e1 	mov	x1, x21
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
 754:	8b131013 	add	x19, x0, x19, lsl #4
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 758:	aa1303e0 	mov	x0, x19
 75c:	94000000 	bl	0 <_ZNKSs8capacityEv.isra.0>
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:794
 760:	f9400282 	ldr	x2, [x20]
 764:	91004273 	add	x19, x19, #0x10
 768:	cb020273 	sub	x19, x19, x2
_ZNSt6vectorI1HSaIS0_EE15set_size_elems_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:129
 76c:	b4000042 	cbz	x2, 774 <cs_vec_push_H+0x70>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:136 (discriminator 1)
 770:	f81f8053 	stur	x19, [x2, #-8]
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 774:	aa0203e0 	mov	x0, x2
 778:	97fffe29 	bl	1c <_ZNKSt6vectorI1HSaIS0_EE4sizeEv.isra.0>
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
 77c:	b40001a2 	cbz	x2, 7b0 <cs_vec_push_H+0xac>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688 (discriminator 1)
 780:	f85f0041 	ldur	x1, [x2, #-16]
 784:	d344fc21 	lsr	x1, x1, #4
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779 (discriminator 1)
 788:	eb01001f 	cmp	x0, x1
 78c:	54000161 	b.ne	7b8 <cs_vec_push_H+0xb4>  // b.any
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 790:	f94013f5 	ldr	x21, [sp, #32]
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 794:	91000401 	add	x1, x0, #0x1
 798:	aa1403e0 	mov	x0, x20
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 79c:	a94153f3 	ldp	x19, x20, [sp, #16]
 7a0:	a8c37bfd 	ldp	x29, x30, [sp], #48
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 7a4:	14000000 	b	0 <_ZNKSs8capacityEv.isra.0>
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687 (discriminator 1)
 7a8:	d2800000 	mov	x0, #0x0                   	// #0
 7ac:	17ffffe3 	b	738 <cs_vec_push_H+0x34>
 7b0:	d2800001 	mov	x1, #0x0                   	// #0
 7b4:	17fffff5 	b	788 <cs_vec_push_H+0x84>
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 7b8:	a94153f3 	ldp	x19, x20, [sp, #16]
 7bc:	f94013f5 	ldr	x21, [sp, #32]
 7c0:	a8c37bfd 	ldp	x29, x30, [sp], #48
 7c4:	d65f03c0 	ret

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
