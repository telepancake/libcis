; call site: cs_vec_insert_H   lib=libcis   arch=arm32   size=80 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

00000410 <cs_vec_insert_H>:
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 410:	b5f0      	push	{r4, r5, r6, r7, lr}
 412:	460f      	mov	r7, r1
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:810
 414:	2102      	movs	r1, #2
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 416:	b087      	sub	sp, #28
 418:	4604      	mov	r4, r0
 41a:	4e0e      	ldr	r6, [pc, #56]	@ (454 <cs_vec_insert_H+0x44>)
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:810
 41c:	f7ff fffe 	bl	0 <_ZNKSs8capacityEv.isra.0>
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19 (discriminator 1)
 420:	4639      	mov	r1, r7
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:810
 422:	4605      	mov	r5, r0
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19 (discriminator 1)
 424:	f7ff fffe 	bl	0 <_ZNKSs8capacityEv.isra.0>
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
 428:	4a0b      	ldr	r2, [pc, #44]	@ (458 <cs_vec_insert_H+0x48>)
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 42a:	447e      	add	r6, pc
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:812
 42c:	6823      	ldr	r3, [r4, #0]
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
 42e:	330c      	adds	r3, #12
 430:	58b2      	ldr	r2, [r6, r2]
 432:	9204      	str	r2, [sp, #16]
 434:	4a09      	ldr	r2, [pc, #36]	@ (45c <cs_vec_insert_H+0x4c>)
 436:	58b2      	ldr	r2, [r6, r2]
 438:	9205      	str	r2, [sp, #20]
 43a:	220c      	movs	r2, #12
 43c:	9202      	str	r2, [sp, #8]
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:812
 43e:	18aa      	adds	r2, r5, r2
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
 440:	e9dd 1004 	ldrd	r1, r0, [sp, #16]
 444:	e9cd 5200 	strd	r5, r2, [sp]
 448:	4622      	mov	r2, r4
 44a:	f7ff fffe 	bl	0 <_ZNSt6detail6rotateEPKNS_8type_opsEPKNS_11storage_opsEPvS6_S6_S6_i>
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 44e:	b007      	add	sp, #28
 450:	bdf0      	pop	{r4, r5, r6, r7, pc}
 452:	bf00      	nop
 454:	00000026 	.word	0x00000026
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

Disassembly of section .text._ZNSt6vectorIiSaIiEE12ensure_free_Ej:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_j.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_j:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:
