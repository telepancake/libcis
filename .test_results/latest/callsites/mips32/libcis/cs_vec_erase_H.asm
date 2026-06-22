; call site: cs_vec_erase_H   lib=libcis   arch=mips32   size=100 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

000002f8 <cs_vec_erase_H>:
_ZNSt6vectorI1HSaIS0_EE5beginEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:658
     2f8:	8c870000 	lw	a3,0(a0)
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
     2fc:	3c1c0000 	lui	gp,0x0
     300:	27bdffd0 	addiu	sp,sp,-48
     304:	279c0000 	addiu	gp,gp,0
     308:	00803025 	move	a2,a0
     30c:	afbf002c 	sw	ra,44(sp)
     310:	afbc0020 	sw	gp,32(sp)
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:889
     314:	0c000008 	jal	20 <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
     318:	00e02025 	move	a0,a3
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
     31c:	3c050000 	lui	a1,0x0
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:889
     320:	8fbc0020 	lw	gp,32(sp)
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
     324:	3c040000 	lui	a0,0x0
     328:	2403fff4 	li	v1,-12
     32c:	afa20014 	sw	v0,20(sp)
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:889
     330:	24e20018 	addiu	v0,a3,24
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375
     334:	24a50000 	addiu	a1,a1,0
     338:	afa30018 	sw	v1,24(sp)
     33c:	afa20010 	sw	v0,16(sp)
     340:	24e7000c 	addiu	a3,a3,12
     344:	8f990000 	lw	t9,0(gp)
     348:	0320f809 	jalr	t9
     34c:	24840000 	addiu	a0,a0,0
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
     350:	8fbf002c 	lw	ra,44(sp)
     354:	03e00008 	jr	ra
     358:	27bd0030 	addiu	sp,sp,48

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
