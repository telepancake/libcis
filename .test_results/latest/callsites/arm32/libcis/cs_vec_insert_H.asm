; call site: cs_vec_insert_H   lib=libcis   arch=arm32   size=136 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

0000044c <cs_vec_insert_H>:
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 44c:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
 450:	4605      	mov	r5, r0
_ZNSt6vectorI1HSaIS0_EE5beginEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:658
 452:	6804      	ldr	r4, [r0, #0]
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 454:	4e1c      	ldr	r6, [pc, #112]	@ (4c8 <cs_vec_insert_H+0x7c>)
 456:	b087      	sub	sp, #28
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 458:	4620      	mov	r0, r4
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 45a:	460f      	mov	r7, r1
 45c:	447e      	add	r6, pc
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 45e:	f7ff fdd5 	bl	c <_ZNKSt6vectorI1HSaIS0_EE4sizeEv.isra.0>
 462:	4680      	mov	r8, r0
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
 464:	b12c      	cbz	r4, 472 <cs_vec_insert_H+0x26>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688 (discriminator 1)
 466:	f854 3c08 	ldr.w	r3, [r4, #-8]
 46a:	4c16      	ldr	r4, [pc, #88]	@ (4c4 <cs_vec_insert_H+0x78>)
 46c:	fba3 3404 	umull	r3, r4, r3, r4
 470:	08e4      	lsrs	r4, r4, #3
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779 (discriminator 1)
 472:	eba4 0408 	sub.w	r4, r4, r8
 476:	2c01      	cmp	r4, #1
 478:	d804      	bhi.n	484 <cs_vec_insert_H+0x38>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 47a:	f108 0102 	add.w	r1, r8, #2
 47e:	4628      	mov	r0, r5
 480:	f7ff fffe 	bl	0 <_ZNKSs8capacityEv.isra.0>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
 484:	682c      	ldr	r4, [r5, #0]
 486:	f04f 090c 	mov.w	r9, #12
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 48a:	4639      	mov	r1, r7
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
 48c:	fb09 4408 	mla	r4, r9, r8, r4
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 490:	4620      	mov	r0, r4
 492:	f7ff fffe 	bl	0 <_ZNKSs8capacityEv.isra.0>
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
 496:	4a0d      	ldr	r2, [pc, #52]	@ (4cc <cs_vec_insert_H+0x80>)
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:815
 498:	682b      	ldr	r3, [r5, #0]
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
 49a:	444b      	add	r3, r9
 49c:	58b2      	ldr	r2, [r6, r2]
 49e:	9204      	str	r2, [sp, #16]
 4a0:	4a0b      	ldr	r2, [pc, #44]	@ (4d0 <cs_vec_insert_H+0x84>)
 4a2:	58b2      	ldr	r2, [r6, r2]
 4a4:	9205      	str	r2, [sp, #20]
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:815
 4a6:	eb04 0209 	add.w	r2, r4, r9
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
 4aa:	f8cd 9008 	str.w	r9, [sp, #8]
 4ae:	e9cd 4200 	strd	r4, r2, [sp]
 4b2:	462a      	mov	r2, r5
 4b4:	e9dd 1004 	ldrd	r1, r0, [sp, #16]
 4b8:	f7ff fffe 	bl	0 <_ZNSt6detail6rotateEPKNS_8type_opsEPKNS_11storage_opsEPvS6_S6_S6_i>
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 4bc:	b007      	add	sp, #28
 4be:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 4c2:	bf00      	nop
 4c4:	aaaaaaab 	.word	0xaaaaaaab
 4c8:	00000068 	.word	0x00000068
	...

Disassembly of section .text._ZNSt6detail17move_construct_opI1HSaIS1_EEEvPvS3_S3_:

Disassembly of section .text._ZN20libcis_atomic_detail15atomic_spinlock4lockEv:

Disassembly of section .text._ZN20libcis_atomic_detail15atomic_spinlock6unlockEv:

Disassembly of section .text._ZNK1HltERKS_:

Disassembly of section .text._ZNSs11vdeallocateEv:

Disassembly of section .text._ZNSt6detail10destroy_opI1HSaIS1_EEEvPvS3_:

Disassembly of section .text._ZNSs9vallocateEj:

Disassembly of section .text._ZNSs9init_copyEPKcj:

Disassembly of section .text._ZN1HaSERKS_.isra.0:

Disassembly of section .text._ZNSt6vectorIiSaIiEE10grow_cold_Ej:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE10grow_cold_Ej:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_j.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_j:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:
