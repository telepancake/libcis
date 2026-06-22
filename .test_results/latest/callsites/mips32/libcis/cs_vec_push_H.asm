; call site: cs_vec_push_H   lib=libcis   arch=mips32   size=104 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

00000820 <cs_vec_push_H>:
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
     820:	27bdffd8 	addiu	sp,sp,-40
     824:	afb20020 	sw	s2,32(sp)
     828:	00a09025 	move	s2,a1
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:789
     82c:	24050001 	li	a1,1
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
     830:	afbf0024 	sw	ra,36(sp)
     834:	afb1001c 	sw	s1,28(sp)
     838:	00808825 	move	s1,a0
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:789
     83c:	0c000000 	jal	0 <_ZNKSs8capacityEv.isra.0>
     840:	afb00018 	sw	s0,24(sp)
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19
     844:	02402825 	move	a1,s2
     848:	00402025 	move	a0,v0
     84c:	0c000000 	jal	0 <_ZNKSs8capacityEv.isra.0>
     850:	00408025 	move	s0,v0
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:791
     854:	8e220000 	lw	v0,0(s1)
     858:	2610000c 	addiu	s0,s0,12
_ZNSt6vectorI1HSaIS0_EE15set_size_elems_Ej():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:129
     85c:	10400002 	beqz	v0,868 <cs_vec_push_H+0x48>
     860:	02028023 	subu	s0,s0,v0
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:136
     864:	ac50fffc 	sw	s0,-4(v0)
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
     868:	8fbf0024 	lw	ra,36(sp)
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
     86c:	02202025 	move	a0,s1
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
     870:	8fb20020 	lw	s2,32(sp)
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
     874:	24050001 	li	a1,1
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
     878:	8fb1001c 	lw	s1,28(sp)
     87c:	8fb00018 	lw	s0,24(sp)
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
     880:	08000000 	j	0 <_ZNKSs8capacityEv.isra.0>
     884:	27bd0028 	addiu	sp,sp,40

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
