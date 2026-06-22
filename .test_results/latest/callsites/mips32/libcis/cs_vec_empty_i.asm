; call site: cs_vec_empty_i   lib=libcis   arch=mips32   size=36 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

00000434 <cs_vec_empty_i>:
cs_vec_empty_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:49
     434:	27bdffe0 	addiu	sp,sp,-32
_ZNKSt6vectorIiSaIiEE5emptyEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:691
     438:	8c840000 	lw	a0,0(a0)
cs_vec_empty_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:49
     43c:	afbf001c 	sw	ra,28(sp)
_ZNKSt6vectorIiSaIiEE5emptyEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:691
     440:	0c00001c 	jal	70 <_ZNKSt6vectorIiSaIiEE4sizeEv.isra.0>
     444:	00000000 	nop
cs_vec_empty_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:49
     448:	8fbf001c 	lw	ra,28(sp)
     44c:	2c420001 	sltiu	v0,v0,1
     450:	03e00008 	jr	ra
     454:	27bd0020 	addiu	sp,sp,32

Disassembly of section .text._ZNKSt19bad_optional_access4whatEv:

Disassembly of section .text._ZNSt19bad_optional_accessD2Ev:

Disassembly of section .text._ZNSt19bad_optional_accessD0Ev:

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

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_j.isra.0:

Disassembly of section .text._ZNKSt6vectorI1HSaIS0_EE9recommendEj:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE10grow_cold_Ej:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_j:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:
