; call site: cs_vec_push_int   lib=libcis   arch=arm64   size=180 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm64/libcis.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000570 <cs_vec_push_int>:
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:29
 570:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
 574:	910003fd 	mov	x29, sp
 578:	f90013f5 	str	x21, [sp, #32]
 57c:	2a0103f5 	mov	w21, w1
 580:	f9400001 	ldr	x1, [x0]
 584:	a90153f3 	stp	x19, x20, [sp, #16]
 588:	aa0003f4 	mov	x20, x0
_ZNSt6vectorIiSaIiEE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 58c:	aa0103e0 	mov	x0, x1
 590:	97fffebf 	bl	8c <_ZNKSt6vectorIiSaIiEE4sizeEv.isra.0>
 594:	aa0003f3 	mov	x19, x0
_ZNKSt6vectorIiSaIiEE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
 598:	b4000361 	cbz	x1, 604 <cs_vec_push_int+0x94>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688 (discriminator 1)
 59c:	f85f0020 	ldur	x0, [x1, #-16]
 5a0:	d342fc00 	lsr	x0, x0, #2
_ZNSt6vectorIiSaIiEE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779 (discriminator 1)
 5a4:	eb00027f 	cmp	x19, x0
 5a8:	54000081 	b.ne	5b8 <cs_vec_push_int+0x48>  // b.any
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 5ac:	91000661 	add	x1, x19, #0x1
 5b0:	aa1403e0 	mov	x0, x20
 5b4:	94000000 	bl	0 <_ZNKSs8capacityEv.isra.0>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
 5b8:	f9400282 	ldr	x2, [x20]
_ZSt12construct_atIiJRKiEEPT_S3_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 5bc:	b8337855 	str	w21, [x2, x19, lsl #2]
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:794
 5c0:	d37ef673 	lsl	x19, x19, #2
 5c4:	91001273 	add	x19, x19, #0x4
_ZNSt6vectorIiSaIiEE15set_size_elems_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:129
 5c8:	b4000042 	cbz	x2, 5d0 <cs_vec_push_int+0x60>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:136 (discriminator 1)
 5cc:	f81f8053 	stur	x19, [x2, #-8]
_ZNSt6vectorIiSaIiEE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 5d0:	aa0203e0 	mov	x0, x2
 5d4:	97fffeae 	bl	8c <_ZNKSt6vectorIiSaIiEE4sizeEv.isra.0>
_ZNKSt6vectorIiSaIiEE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
 5d8:	b40001a2 	cbz	x2, 60c <cs_vec_push_int+0x9c>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688 (discriminator 1)
 5dc:	f85f0041 	ldur	x1, [x2, #-16]
 5e0:	d342fc21 	lsr	x1, x1, #2
_ZNSt6vectorIiSaIiEE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779 (discriminator 1)
 5e4:	eb01001f 	cmp	x0, x1
 5e8:	54000161 	b.ne	614 <cs_vec_push_int+0xa4>  // b.any
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:29
 5ec:	f94013f5 	ldr	x21, [sp, #32]
_ZNSt6vectorIiSaIiEE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 5f0:	91000401 	add	x1, x0, #0x1
 5f4:	aa1403e0 	mov	x0, x20
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:29
 5f8:	a94153f3 	ldp	x19, x20, [sp, #16]
 5fc:	a8c37bfd 	ldp	x29, x30, [sp], #48
_ZNSt6vectorIiSaIiEE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 600:	14000000 	b	0 <_ZNKSs8capacityEv.isra.0>
_ZNKSt6vectorIiSaIiEE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687 (discriminator 1)
 604:	d2800000 	mov	x0, #0x0                   	// #0
 608:	17ffffe7 	b	5a4 <cs_vec_push_int+0x34>
 60c:	d2800001 	mov	x1, #0x0                   	// #0
 610:	17fffff5 	b	5e4 <cs_vec_push_int+0x74>
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:29
 614:	a94153f3 	ldp	x19, x20, [sp, #16]
 618:	f94013f5 	ldr	x21, [sp, #32]
 61c:	a8c37bfd 	ldp	x29, x30, [sp], #48
 620:	d65f03c0 	ret

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
