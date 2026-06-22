; call site: cs_vec_insert_H   lib=libcis   arch=mips32   size=148 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

0000078c <cs_vec_insert_H>:
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
     78c:	27bdffc8 	addiu	sp,sp,-56
     790:	3c1c0000 	lui	gp,0x0
     794:	afb20030 	sw	s2,48(sp)
     798:	279c0000 	addiu	gp,gp,0
     79c:	00a09025 	move	s2,a1
     7a0:	afbf0034 	sw	ra,52(sp)
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:810
     7a4:	24050002 	li	a1,2
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
     7a8:	afb1002c 	sw	s1,44(sp)
     7ac:	afb00028 	sw	s0,40(sp)
     7b0:	afbc0020 	sw	gp,32(sp)
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:810
     7b4:	0c000000 	jal	0 <_ZNKSs8capacityEv.isra.0>
     7b8:	00808825 	move	s1,a0
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19
     7bc:	02402825 	move	a1,s2
     7c0:	00402025 	move	a0,v0
     7c4:	0c000000 	jal	0 <_ZNKSs8capacityEv.isra.0>
     7c8:	00408025 	move	s0,v0
     7cc:	8fbc0020 	lw	gp,32(sp)
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
     7d0:	2402000c 	li	v0,12
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:812
     7d4:	8e270000 	lw	a3,0(s1)
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
     7d8:	3c050000 	lui	a1,0x0
     7dc:	3c040000 	lui	a0,0x0
     7e0:	afa20018 	sw	v0,24(sp)
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:812
     7e4:	2602000c 	addiu	v0,s0,12
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
     7e8:	afb00010 	sw	s0,16(sp)
     7ec:	8f990000 	lw	t9,0(gp)
     7f0:	24e7000c 	addiu	a3,a3,12
     7f4:	24a50000 	addiu	a1,a1,0
     7f8:	afa20014 	sw	v0,20(sp)
     7fc:	24840000 	addiu	a0,a0,0
     800:	0320f809 	jalr	t9
     804:	02203025 	move	a2,s1
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
     808:	8fbf0034 	lw	ra,52(sp)
     80c:	8fb20030 	lw	s2,48(sp)
     810:	8fb1002c 	lw	s1,44(sp)
     814:	8fb00028 	lw	s0,40(sp)
     818:	03e00008 	jr	ra
     81c:	27bd0038 	addiu	sp,sp,56

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
