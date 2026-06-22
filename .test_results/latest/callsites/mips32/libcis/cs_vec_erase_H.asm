; call site: cs_vec_erase_H   lib=libcis   arch=mips32   size=100 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

00000334 <cs_vec_erase_H>:
_ZNSt6vectorI1HSaIS0_EE5beginEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:658
     334:	8c870000 	lw	a3,0(a0)
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
     338:	3c1c0000 	lui	gp,0x0
     33c:	27bdffd0 	addiu	sp,sp,-48
     340:	279c0000 	addiu	gp,gp,0
     344:	00803025 	move	a2,a0
     348:	afbf002c 	sw	ra,44(sp)
     34c:	afbc0020 	sw	gp,32(sp)
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:892
     350:	0c000011 	jal	44 <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
     354:	00e02025 	move	a0,a3
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
     358:	3c050000 	lui	a1,0x0
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:892
     35c:	8fbc0020 	lw	gp,32(sp)
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
     360:	3c040000 	lui	a0,0x0
     364:	2403fff4 	li	v1,-12
     368:	afa20014 	sw	v0,20(sp)
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:892
     36c:	24e20018 	addiu	v0,a3,24
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
     370:	24a50000 	addiu	a1,a1,0
     374:	afa30018 	sw	v1,24(sp)
     378:	afa20010 	sw	v0,16(sp)
     37c:	24e7000c 	addiu	a3,a3,12
     380:	8f990000 	lw	t9,0(gp)
     384:	0320f809 	jalr	t9
     388:	24840000 	addiu	a0,a0,0
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
     38c:	8fbf002c 	lw	ra,44(sp)
     390:	03e00008 	jr	ra
     394:	27bd0030 	addiu	sp,sp,48

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
