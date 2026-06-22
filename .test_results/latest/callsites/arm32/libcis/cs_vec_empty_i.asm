; call site: cs_vec_empty_i   lib=libcis   arch=arm32   size=16 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

0000023c <cs_vec_empty_i>:
_ZNKSt6vectorIiSaIiEE5emptyEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:691
 23c:	6800      	ldr	r0, [r0, #0]
cs_vec_empty_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:49
 23e:	b508      	push	{r3, lr}
_ZNKSt6vectorIiSaIiEE5emptyEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:691
 240:	f7ff ff12 	bl	68 <_ZNKSt6vectorIiSaIiEE4sizeEv.isra.0>
cs_vec_empty_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:49
 244:	fab0 f080 	clz	r0, r0
 248:	0940      	lsrs	r0, r0, #5
 24a:	bd08      	pop	{r3, pc}

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
