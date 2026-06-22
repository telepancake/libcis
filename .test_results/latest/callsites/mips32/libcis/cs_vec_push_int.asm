; call site: cs_vec_push_int   lib=libcis   arch=mips32   size=184 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

00000714 <cs_vec_push_int>:
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:29
     714:	27bdffd8 	addiu	sp,sp,-40
     718:	afb1001c 	sw	s1,28(sp)
     71c:	00808825 	move	s1,a0
     720:	afb20020 	sw	s2,32(sp)
     724:	00a09025 	move	s2,a1
     728:	afb00018 	sw	s0,24(sp)
     72c:	afbf0024 	sw	ra,36(sp)
_ZNSt6vectorIiSaIiEE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
     730:	0c00001c 	jal	70 <_ZNKSt6vectorIiSaIiEE4sizeEv.isra.0>
     734:	8c840000 	lw	a0,0(a0)
     738:	00408025 	move	s0,v0
_ZNKSt6vectorIiSaIiEE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
     73c:	10800003 	beqz	a0,74c <cs_vec_push_int+0x38>
     740:	00001025 	move	v0,zero
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688
     744:	8c82fff8 	lw	v0,-8(a0)
     748:	00021082 	srl	v0,v0,0x2
_ZNSt6vectorIiSaIiEE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779
     74c:	16020003 	bne	s0,v0,75c <cs_vec_push_int+0x48>
     750:	26050001 	addiu	a1,s0,1
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
     754:	0c000000 	jal	0 <_ZNKSs8capacityEv.isra.0>
     758:	02202025 	move	a0,s1
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
     75c:	8e240000 	lw	a0,0(s1)
     760:	00108080 	sll	s0,s0,0x2
     764:	00908021 	addu	s0,a0,s0
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:794
     768:	26100004 	addiu	s0,s0,4
_ZSt12construct_atIiJRKiEEPT_S3_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19
     76c:	ae12fffc 	sw	s2,-4(s0)
_ZNSt6vectorIiSaIiEE15set_size_elems_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:129
     770:	1480000c 	bnez	a0,7a4 <cs_vec_push_int+0x90>
     774:	02048023 	subu	s0,s0,a0
_ZNSt6vectorIiSaIiEE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
     778:	0c00001c 	jal	70 <_ZNKSt6vectorIiSaIiEE4sizeEv.isra.0>
     77c:	00002825 	move	a1,zero
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779
     780:	14a2000d 	bne	a1,v0,7b8 <cs_vec_push_int+0xa4>
     784:	8fbf0024 	lw	ra,36(sp)
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
     788:	02202025 	move	a0,s1
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:29
     78c:	8fb20020 	lw	s2,32(sp)
_ZNSt6vectorIiSaIiEE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
     790:	24a50001 	addiu	a1,a1,1
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:29
     794:	8fb1001c 	lw	s1,28(sp)
     798:	8fb00018 	lw	s0,24(sp)
_ZNSt6vectorIiSaIiEE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
     79c:	08000000 	j	0 <_ZNKSs8capacityEv.isra.0>
     7a0:	27bd0028 	addiu	sp,sp,40
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
     7a4:	0c00001c 	jal	70 <_ZNKSt6vectorIiSaIiEE4sizeEv.isra.0>
     7a8:	ac90fffc 	sw	s0,-4(a0)
_ZNKSt6vectorIiSaIiEE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688
     7ac:	8c85fff8 	lw	a1,-8(a0)
     7b0:	1000fff3 	b	780 <cs_vec_push_int+0x6c>
     7b4:	00052882 	srl	a1,a1,0x2
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:29
     7b8:	8fb20020 	lw	s2,32(sp)
     7bc:	8fb1001c 	lw	s1,28(sp)
     7c0:	8fb00018 	lw	s0,24(sp)
     7c4:	03e00008 	jr	ra
     7c8:	27bd0028 	addiu	sp,sp,40

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
