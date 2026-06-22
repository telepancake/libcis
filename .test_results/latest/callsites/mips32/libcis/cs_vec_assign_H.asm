; call site: cs_vec_assign_H   lib=libcis   arch=mips32   size=84 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

00000998 <cs_vec_assign_H>:
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:594
     998:	10850012 	beq	a0,a1,9e4 <cs_vec_assign_H+0x4c>
     99c:	00000000 	nop
     9a0:	8ca90000 	lw	t1,0(a1)
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
     9a4:	27bdffe0 	addiu	sp,sp,-32
     9a8:	00804025 	move	t0,a0
     9ac:	afbf001c 	sw	ra,28(sp)
_ZNKSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:661
     9b0:	0c000011 	jal	44 <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
     9b4:	01202025 	move	a0,t1
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:635
     9b8:	01202825 	move	a1,t1
_ZSt8distanceIPK1HENSt15iterator_traitsIT_E15difference_typeES4_S4_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/iterator:1087
     9bc:	00493823 	subu	a3,v0,t1
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
     9c0:	8fbf001c 	lw	ra,28(sp)
_ZNKSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:661
     9c4:	00403025 	move	a2,v0
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:635
     9c8:	3c02aaaa 	lui	v0,0xaaaa
_ZSt8distanceIPK1HENSt15iterator_traitsIT_E15difference_typeES4_S4_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/iterator:1087
     9cc:	00073883 	sra	a3,a3,0x2
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:635
     9d0:	3442aaab 	ori	v0,v0,0xaaab
     9d4:	70e23802 	mul	a3,a3,v0
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
     9d8:	27bd0020 	addiu	sp,sp,32
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:635
     9dc:	08000000 	j	0 <_ZNKSs8capacityEv.isra.0>
     9e0:	01002025 	move	a0,t0
     9e4:	03e00008 	jr	ra
     9e8:	00000000 	nop

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
