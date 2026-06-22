; call site: cs_str_append   lib=libcis   arch=mips32   size=84 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

00000690 <cs_str_append>:
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
     690:	3c1c0000 	lui	gp,0x0
     694:	27bdffd8 	addiu	sp,sp,-40
     698:	279c0000 	addiu	gp,gp,0
     69c:	afb10020 	sw	s1,32(sp)
     6a0:	00808825 	move	s1,a0
     6a4:	afb0001c 	sw	s0,28(sp)
_ZNSt11char_traitsIcE6lengthEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:104
     6a8:	00a02025 	move	a0,a1
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
     6ac:	afbf0024 	sw	ra,36(sp)
     6b0:	afbc0010 	sw	gp,16(sp)
_ZNSt11char_traitsIcE6lengthEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:104
     6b4:	8f990000 	lw	t9,0(gp)
     6b8:	0320f809 	jalr	t9
     6bc:	00a08025 	move	s0,a1
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:798
     6c0:	02002825 	move	a1,s0
_ZNSt11char_traitsIcE6lengthEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:104
     6c4:	8fbc0010 	lw	gp,16(sp)
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:798
     6c8:	02202025 	move	a0,s1
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
     6cc:	8fbf0024 	lw	ra,36(sp)
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:798
     6d0:	00403025 	move	a2,v0
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
     6d4:	8fb10020 	lw	s1,32(sp)
     6d8:	8fb0001c 	lw	s0,28(sp)
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:798
     6dc:	08000132 	j	4c8 <_ZNSs6appendEPKcj.isra.0>
     6e0:	27bd0028 	addiu	sp,sp,40

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
