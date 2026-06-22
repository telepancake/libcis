; call site: cs_vec_push_H   lib=libcis   arch=arm32   size=112 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

000004d4 <cs_vec_push_H>:
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 4d4:	6802      	ldr	r2, [r0, #0]
 4d6:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 4d8:	4604      	mov	r4, r0
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 4da:	4610      	mov	r0, r2
 4dc:	f7ff fd96 	bl	c <_ZNKSt6vectorI1HSaIS0_EE4sizeEv.isra.0>
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 4e0:	460e      	mov	r6, r1
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 4e2:	4607      	mov	r7, r0
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
 4e4:	b12a      	cbz	r2, 4f2 <cs_vec_push_H+0x1e>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688 (discriminator 1)
 4e6:	f852 3c08 	ldr.w	r3, [r2, #-8]
 4ea:	4a15      	ldr	r2, [pc, #84]	@ (540 <cs_vec_push_H+0x6c>)
 4ec:	fba3 3202 	umull	r3, r2, r3, r2
 4f0:	08d2      	lsrs	r2, r2, #3
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779 (discriminator 1)
 4f2:	4297      	cmp	r7, r2
 4f4:	d103      	bne.n	4fe <cs_vec_push_H+0x2a>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 4f6:	1c79      	adds	r1, r7, #1
 4f8:	4620      	mov	r0, r4
 4fa:	f7ff fffe 	bl	0 <_ZNKSs8capacityEv.isra.0>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
 4fe:	6825      	ldr	r5, [r4, #0]
 500:	230c      	movs	r3, #12
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 502:	4631      	mov	r1, r6
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
 504:	fb03 5507 	mla	r5, r3, r7, r5
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 508:	4628      	mov	r0, r5
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:794
 50a:	350c      	adds	r5, #12
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 50c:	f7ff fffe 	bl	0 <_ZNKSs8capacityEv.isra.0>
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:794
 510:	6822      	ldr	r2, [r4, #0]
 512:	1aad      	subs	r5, r5, r2
_ZNSt6vectorI1HSaIS0_EE15set_size_elems_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:129
 514:	b10a      	cbz	r2, 51a <cs_vec_push_H+0x46>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:136 (discriminator 1)
 516:	f842 5c04 	str.w	r5, [r2, #-4]
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 51a:	4610      	mov	r0, r2
 51c:	f7ff fd76 	bl	c <_ZNKSt6vectorI1HSaIS0_EE4sizeEv.isra.0>
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
 520:	b12a      	cbz	r2, 52e <cs_vec_push_H+0x5a>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688 (discriminator 1)
 522:	f852 3c08 	ldr.w	r3, [r2, #-8]
 526:	4a06      	ldr	r2, [pc, #24]	@ (540 <cs_vec_push_H+0x6c>)
 528:	fba3 3202 	umull	r3, r2, r3, r2
 52c:	08d2      	lsrs	r2, r2, #3
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779 (discriminator 1)
 52e:	4290      	cmp	r0, r2
 530:	d105      	bne.n	53e <cs_vec_push_H+0x6a>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 532:	1c41      	adds	r1, r0, #1
 534:	4620      	mov	r0, r4
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 536:	e8bd 40f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, lr}
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 53a:	f7ff bffe 	b.w	0 <_ZNKSs8capacityEv.isra.0>
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 53e:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 540:	aaaaaaab 	.word	0xaaaaaaab

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
