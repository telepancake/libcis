; call site: cs_str_compare   lib=libcis   arch=mips32   size=148 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

00000398 <cs_str_compare>:
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:36
     398:	3c1c0000 	lui	gp,0x0
     39c:	8c840000 	lw	a0,0(a0)
     3a0:	27bdffd8 	addiu	sp,sp,-40
     3a4:	279c0000 	addiu	gp,gp,0
     3a8:	afbf0024 	sw	ra,36(sp)
     3ac:	afb20020 	sw	s2,32(sp)
     3b0:	afb1001c 	sw	s1,28(sp)
     3b4:	afb00018 	sw	s0,24(sp)
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743
     3b8:	10800015 	beqz	a0,410 <cs_str_compare+0x78>
     3bc:	afbc0010 	sw	gp,16(sp)
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:639
     3c0:	8c90fffc 	lw	s0,-4(a0)
_ZNSt17basic_string_viewIcSt11char_traitsIcEEC4EPKcj():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:603
     3c4:	8ca50000 	lw	a1,0(a1)
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743
     3c8:	10a00015 	beqz	a1,420 <cs_str_compare+0x88>
     3cc:	00008825 	move	s1,zero
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:639
     3d0:	8cb1fffc 	lw	s1,-4(a1)
_ZNSt11char_traitsIcE7compareEPKcS2_j():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:98
     3d4:	0230902b 	sltu	s2,s1,s0
     3d8:	8f990000 	lw	t9,0(gp)
     3dc:	02203025 	move	a2,s1
     3e0:	0320f809 	jalr	t9
     3e4:	0212300a 	movz	a2,s0,s2
_ZNKSt17basic_string_viewIcSt11char_traitsIcEE7compareES2_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:696
     3e8:	14400004 	bnez	v0,3fc <cs_str_compare+0x64>
     3ec:	8fbf0024 	lw	ra,36(sp)
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:698
     3f0:	2402ffff 	li	v0,-1
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:697
     3f4:	0211802b 	sltu	s0,s0,s1
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:698
     3f8:	0250100a 	movz	v0,s2,s0
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:36
     3fc:	8fb20020 	lw	s2,32(sp)
     400:	8fb1001c 	lw	s1,28(sp)
     404:	8fb00018 	lw	s0,24(sp)
     408:	03e00008 	jr	ra
     40c:	27bd0028 	addiu	sp,sp,40
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743
     410:	3c040000 	lui	a0,0x0
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:638
     414:	00008025 	move	s0,zero
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743
     418:	1000ffea 	b	3c4 <cs_str_compare+0x2c>
     41c:	24840000 	addiu	a0,a0,0
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743
     420:	3c050000 	lui	a1,0x0
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743
     424:	1000ffeb 	b	3d4 <cs_str_compare+0x3c>
     428:	24a50000 	addiu	a1,a1,0

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
