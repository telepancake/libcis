; call site: cs_vec_push_int   lib=libcis   arch=arm32   size=38 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

000003c0 <cs_vec_push_int>:
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
 3c0:	b538      	push	{r3, r4, r5, lr}
 3c2:	4604      	mov	r4, r0
 3c4:	460d      	mov	r5, r1
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:789
 3c6:	2101      	movs	r1, #1
 3c8:	f7ff fffe 	bl	0 <_ZNKSs8capacityEv.isra.0>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:791
 3cc:	6822      	ldr	r2, [r4, #0]
_ZSt12construct_atIiJRKiEEPT_S3_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19 (discriminator 1)
 3ce:	f840 5b04 	str.w	r5, [r0], #4
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:791
 3d2:	1a80      	subs	r0, r0, r2
_ZNSt6vectorIiSaIiEE15set_size_elems_Ej():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:129
 3d4:	b10a      	cbz	r2, 3da <cs_vec_push_int+0x1a>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:136 (discriminator 1)
 3d6:	f842 0c04 	str.w	r0, [r2, #-4]
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
 3da:	4620      	mov	r0, r4
 3dc:	2101      	movs	r1, #1
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
 3de:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
 3e2:	f7ff bffe 	b.w	0 <_ZNKSs8capacityEv.isra.0>

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
