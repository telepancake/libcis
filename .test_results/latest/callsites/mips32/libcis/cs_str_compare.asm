; call site: cs_str_compare   lib=libcis   arch=mips32   size=148 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

0000035c <cs_str_compare>:
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:36
     35c:	3c1c0000 	lui	gp,0x0
     360:	8c840000 	lw	a0,0(a0)
     364:	27bdffd8 	addiu	sp,sp,-40
     368:	279c0000 	addiu	gp,gp,0
     36c:	afbf0024 	sw	ra,36(sp)
     370:	afb20020 	sw	s2,32(sp)
     374:	afb1001c 	sw	s1,28(sp)
     378:	afb00018 	sw	s0,24(sp)
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:741
     37c:	10800015 	beqz	a0,3d4 <cs_str_compare+0x78>
     380:	afbc0010 	sw	gp,16(sp)
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:637
     384:	8c90fffc 	lw	s0,-4(a0)
_ZNSt17basic_string_viewIcSt11char_traitsIcEEC4EPKcj():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:603
     388:	8ca50000 	lw	a1,0(a1)
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:741
     38c:	10a00015 	beqz	a1,3e4 <cs_str_compare+0x88>
     390:	00008825 	move	s1,zero
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:637
     394:	8cb1fffc 	lw	s1,-4(a1)
_ZNSt11char_traitsIcE7compareEPKcS2_j():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:98
     398:	0230902b 	sltu	s2,s1,s0
     39c:	8f990000 	lw	t9,0(gp)
     3a0:	02203025 	move	a2,s1
     3a4:	0320f809 	jalr	t9
     3a8:	0212300a 	movz	a2,s0,s2
_ZNKSt17basic_string_viewIcSt11char_traitsIcEE7compareES2_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:696
     3ac:	14400004 	bnez	v0,3c0 <cs_str_compare+0x64>
     3b0:	8fbf0024 	lw	ra,36(sp)
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:698
     3b4:	2402ffff 	li	v0,-1
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:697
     3b8:	0211802b 	sltu	s0,s0,s1
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:698
     3bc:	0250100a 	movz	v0,s2,s0
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:36
     3c0:	8fb20020 	lw	s2,32(sp)
     3c4:	8fb1001c 	lw	s1,28(sp)
     3c8:	8fb00018 	lw	s0,24(sp)
     3cc:	03e00008 	jr	ra
     3d0:	27bd0028 	addiu	sp,sp,40
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:741
     3d4:	3c040000 	lui	a0,0x0
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:636
     3d8:	00008025 	move	s0,zero
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:741
     3dc:	1000ffea 	b	388 <cs_str_compare+0x2c>
     3e0:	24840000 	addiu	a0,a0,0
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:741
     3e4:	3c050000 	lui	a1,0x0
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:741
     3e8:	1000ffeb 	b	398 <cs_str_compare+0x3c>
     3ec:	24a50000 	addiu	a1,a1,0

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
