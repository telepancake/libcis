; call site: cs_vec_assign_H   lib=libcis   arch=mips32   size=84 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

00000738 <cs_vec_assign_H>:
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:594
     738:	10850012 	beq	a0,a1,784 <cs_vec_assign_H+0x4c>
     73c:	00000000 	nop
     740:	8ca90000 	lw	t1,0(a1)
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
     744:	27bdffe0 	addiu	sp,sp,-32
     748:	00804025 	move	t0,a0
     74c:	afbf001c 	sw	ra,28(sp)
_ZNKSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:661
     750:	0c000008 	jal	20 <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
     754:	01202025 	move	a0,t1
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:635
     758:	01202825 	move	a1,t1
_ZSt8distanceIPK1HENSt15iterator_traitsIT_E15difference_typeES4_S4_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/iterator:1087
     75c:	00493823 	subu	a3,v0,t1
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
     760:	8fbf001c 	lw	ra,28(sp)
_ZNKSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:661
     764:	00403025 	move	a2,v0
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:635
     768:	3c02aaaa 	lui	v0,0xaaaa
_ZSt8distanceIPK1HENSt15iterator_traitsIT_E15difference_typeES4_S4_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/iterator:1087
     76c:	00073883 	sra	a3,a3,0x2
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:635
     770:	3442aaab 	ori	v0,v0,0xaaab
     774:	70e23802 	mul	a3,a3,v0
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
     778:	27bd0020 	addiu	sp,sp,32
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:635
     77c:	08000000 	j	0 <_ZNKSs8capacityEv.isra.0>
     780:	01002025 	move	a0,t0
     784:	03e00008 	jr	ra
     788:	00000000 	nop

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

Disassembly of section .text._ZNSt6vectorIiSaIiEE12ensure_free_Ej:

Disassembly of section .text._ZNKSt6vectorI1HSaIS0_EE9recommendEj:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_j.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_j:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:
