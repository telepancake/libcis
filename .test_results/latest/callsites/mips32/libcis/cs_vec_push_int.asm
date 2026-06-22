; call site: cs_vec_push_int   lib=libcis   arch=mips32   size=84 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

000006e4 <cs_vec_push_int>:
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
     6e4:	27bdffd8 	addiu	sp,sp,-40
     6e8:	afb10020 	sw	s1,32(sp)
     6ec:	00a08825 	move	s1,a1
     6f0:	afb0001c 	sw	s0,28(sp)
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:789
     6f4:	24050001 	li	a1,1
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
     6f8:	afbf0024 	sw	ra,36(sp)
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:789
     6fc:	0c000000 	jal	0 <_ZNKSs8capacityEv.isra.0>
     700:	00808025 	move	s0,a0
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:791
     704:	8e030000 	lw	v1,0(s0)
     708:	24420004 	addiu	v0,v0,4
_ZSt12construct_atIiJRKiEEPT_S3_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19
     70c:	ac51fffc 	sw	s1,-4(v0)
_ZNSt6vectorIiSaIiEE15set_size_elems_Ej():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:129
     710:	10600002 	beqz	v1,71c <cs_vec_push_int+0x38>
     714:	00431023 	subu	v0,v0,v1
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:136
     718:	ac62fffc 	sw	v0,-4(v1)
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
     71c:	8fbf0024 	lw	ra,36(sp)
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
     720:	02002025 	move	a0,s0
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
     724:	8fb10020 	lw	s1,32(sp)
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
     728:	24050001 	li	a1,1
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
     72c:	8fb0001c 	lw	s0,28(sp)
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
     730:	08000000 	j	0 <_ZNKSs8capacityEv.isra.0>
     734:	27bd0028 	addiu	sp,sp,40

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
