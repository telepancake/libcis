; call site: cs_vec_push_int   lib=libcis   arch=arm32   size=84 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

000003cc <cs_vec_push_int>:
_ZNSt6vectorIiSaIiEE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:777
 3cc:	6803      	ldr	r3, [r0, #0]
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:29
 3ce:	b570      	push	{r4, r5, r6, lr}
 3d0:	4605      	mov	r5, r0
_ZNSt6vectorIiSaIiEE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 3d2:	4618      	mov	r0, r3
 3d4:	f7ff fe48 	bl	68 <_ZNKSt6vectorIiSaIiEE4sizeEv.isra.0>
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:29
 3d8:	460e      	mov	r6, r1
_ZNSt6vectorIiSaIiEE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 3da:	4604      	mov	r4, r0
_ZNKSt6vectorIiSaIiEE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
 3dc:	b113      	cbz	r3, 3e4 <cs_vec_push_int+0x18>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688 (discriminator 1)
 3de:	f853 3c08 	ldr.w	r3, [r3, #-8]
 3e2:	089b      	lsrs	r3, r3, #2
_ZNSt6vectorIiSaIiEE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779 (discriminator 1)
 3e4:	429c      	cmp	r4, r3
 3e6:	d103      	bne.n	3f0 <cs_vec_push_int+0x24>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 3e8:	1c61      	adds	r1, r4, #1
 3ea:	4628      	mov	r0, r5
 3ec:	f7ff fffe 	bl	0 <_ZNKSs8capacityEv.isra.0>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
 3f0:	682b      	ldr	r3, [r5, #0]
_ZSt12construct_atIiJRKiEEPT_S3_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 3f2:	f843 6024 	str.w	r6, [r3, r4, lsl #2]
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:794
 3f6:	00a4      	lsls	r4, r4, #2
 3f8:	3404      	adds	r4, #4
_ZNSt6vectorIiSaIiEE15set_size_elems_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:129
 3fa:	b10b      	cbz	r3, 400 <cs_vec_push_int+0x34>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:136 (discriminator 1)
 3fc:	f843 4c04 	str.w	r4, [r3, #-4]
_ZNSt6vectorIiSaIiEE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 400:	4618      	mov	r0, r3
 402:	f7ff fe31 	bl	68 <_ZNKSt6vectorIiSaIiEE4sizeEv.isra.0>
_ZNKSt6vectorIiSaIiEE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
 406:	b113      	cbz	r3, 40e <cs_vec_push_int+0x42>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688 (discriminator 1)
 408:	f853 3c08 	ldr.w	r3, [r3, #-8]
 40c:	089b      	lsrs	r3, r3, #2
_ZNSt6vectorIiSaIiEE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779 (discriminator 1)
 40e:	4298      	cmp	r0, r3
 410:	d105      	bne.n	41e <cs_vec_push_int+0x52>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 412:	1c41      	adds	r1, r0, #1
 414:	4628      	mov	r0, r5
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:29
 416:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
_ZNSt6vectorIiSaIiEE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 41a:	f7ff bffe 	b.w	0 <_ZNKSs8capacityEv.isra.0>
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:29
 41e:	bd70      	pop	{r4, r5, r6, pc}

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
