; call site: cs_str_append   lib=libcis   arch=mips32   size=84 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

000006c0 <cs_str_append>:
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
     6c0:	3c1c0000 	lui	gp,0x0
     6c4:	27bdffd8 	addiu	sp,sp,-40
     6c8:	279c0000 	addiu	gp,gp,0
     6cc:	afb10020 	sw	s1,32(sp)
     6d0:	00808825 	move	s1,a0
     6d4:	afb0001c 	sw	s0,28(sp)
_ZNSt11char_traitsIcE6lengthEPKc():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:104
     6d8:	00a02025 	move	a0,a1
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
     6dc:	afbf0024 	sw	ra,36(sp)
     6e0:	afbc0010 	sw	gp,16(sp)
_ZNSt11char_traitsIcE6lengthEPKc():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:104
     6e4:	8f990000 	lw	t9,0(gp)
     6e8:	0320f809 	jalr	t9
     6ec:	00a08025 	move	s0,a1
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:800
     6f0:	02002825 	move	a1,s0
_ZNSt11char_traitsIcE6lengthEPKc():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:104
     6f4:	8fbc0010 	lw	gp,16(sp)
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:800
     6f8:	02202025 	move	a0,s1
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
     6fc:	8fbf0024 	lw	ra,36(sp)
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:800
     700:	00403025 	move	a2,v0
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
     704:	8fb10020 	lw	s1,32(sp)
     708:	8fb0001c 	lw	s0,28(sp)
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:800
     70c:	0800013e 	j	4f8 <_ZNSs6appendEPKcj.isra.0>
     710:	27bd0028 	addiu	sp,sp,40

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
