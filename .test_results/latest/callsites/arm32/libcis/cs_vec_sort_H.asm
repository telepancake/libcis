; call site: cs_vec_sort_H   lib=libcis   arch=arm32   size=60 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

00000988 <cs_vec_sort_H>:
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:34
 988:	b510      	push	{r4, lr}
_ZNSt6vectorI1HSaIS0_EE5beginEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:658
 98a:	6804      	ldr	r4, [r0, #0]
_ZNSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:660
 98c:	4620      	mov	r0, r4
 98e:	f7ff fb57 	bl	40 <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
 992:	4601      	mov	r1, r0
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2103
 994:	4284      	cmp	r4, r0
 996:	d012      	beq.n	9be <cs_vec_sort_H+0x36>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2113
 998:	1b02      	subs	r2, r0, r4
 99a:	4b09      	ldr	r3, [pc, #36]	@ (9c0 <cs_vec_sort_H+0x38>)
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2114
 99c:	4620      	mov	r0, r4
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2113
 99e:	1092      	asrs	r2, r2, #2
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:34
 9a0:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2113
 9a4:	435a      	muls	r2, r3
_ZNSt6detail8bit_log2Ey():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2003
 9a6:	17d3      	asrs	r3, r2, #31
 9a8:	bf0a      	itet	eq
 9aa:	fab2 f282 	clzeq	r2, r2
 9ae:	fab3 f283 	clzne	r2, r3
 9b2:	3220      	addeq	r2, #32
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2114
 9b4:	2301      	movs	r3, #1
_ZNSt6detail8bit_log2Ey():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2003 (discriminator 1)
 9b6:	f1c2 023f 	rsb	r2, r2, #63	@ 0x3f
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2114
 9ba:	409a      	lsls	r2, r3
 9bc:	e594      	b.n	4e8 <_ZNSt6detail9introsortIP1HSt4lessIvEEEvT_S5_RT0_NSt15iterator_traitsIS5_E15difference_typeEb.isra.0>
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:34
 9be:	bd10      	pop	{r4, pc}
 9c0:	aaaaaaab 	.word	0xaaaaaaab

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
