; call site: cs_vec_insert_H   lib=libcis   arch=mips32   size=224 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

000007cc <cs_vec_insert_H>:
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
     7cc:	27bdffc0 	addiu	sp,sp,-64
     7d0:	3c1c0000 	lui	gp,0x0
     7d4:	afb10030 	sw	s1,48(sp)
     7d8:	00808825 	move	s1,a0
     7dc:	afb20034 	sw	s2,52(sp)
     7e0:	279c0000 	addiu	gp,gp,0
     7e4:	afb0002c 	sw	s0,44(sp)
     7e8:	00a09025 	move	s2,a1
     7ec:	afbf003c 	sw	ra,60(sp)
     7f0:	afb30038 	sw	s3,56(sp)
     7f4:	afbc0020 	sw	gp,32(sp)
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
     7f8:	0c000008 	jal	20 <_ZNKSt6vectorI1HSaIS0_EE4sizeEv.isra.0>
     7fc:	8c840000 	lw	a0,0(a0)
     800:	00408025 	move	s0,v0
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
     804:	10800006 	beqz	a0,820 <cs_vec_insert_H+0x54>
     808:	00001025 	move	v0,zero
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688
     80c:	8c82fff8 	lw	v0,-8(a0)
     810:	2403000c 	li	v1,12
     814:	006001f4 	teq	v1,zero,0x7
     818:	0043001b 	divu	zero,v0,v1
     81c:	00001012 	mflo	v0
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779
     820:	00501023 	subu	v0,v0,s0
     824:	2c420002 	sltiu	v0,v0,2
     828:	10400005 	beqz	v0,840 <cs_vec_insert_H+0x74>
     82c:	2413000c 	li	s3,12
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
     830:	26050002 	addiu	a1,s0,2
     834:	0c000000 	jal	0 <_ZNKSs8capacityEv.isra.0>
     838:	02202025 	move	a0,s1
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
     83c:	2413000c 	li	s3,12
     840:	8e220000 	lw	v0,0(s1)
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19
     844:	02402825 	move	a1,s2
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
     848:	72131802 	mul	v1,s0,s3
     84c:	00628021 	addu	s0,v1,v0
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19
     850:	0c000000 	jal	0 <_ZNKSs8capacityEv.isra.0>
     854:	02002025 	move	a0,s0
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
     858:	3c050000 	lui	a1,0x0
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19
     85c:	8fbc0020 	lw	gp,32(sp)
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
     860:	3c040000 	lui	a0,0x0
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:815
     864:	8e270000 	lw	a3,0(s1)
     868:	2602000c 	addiu	v0,s0,12
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375
     86c:	24a50000 	addiu	a1,a1,0
     870:	afb30018 	sw	s3,24(sp)
     874:	24840000 	addiu	a0,a0,0
     878:	afa20014 	sw	v0,20(sp)
     87c:	afb00010 	sw	s0,16(sp)
     880:	24e7000c 	addiu	a3,a3,12
     884:	8f990000 	lw	t9,0(gp)
     888:	0320f809 	jalr	t9
     88c:	02203025 	move	a2,s1
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
     890:	8fbf003c 	lw	ra,60(sp)
     894:	8fb30038 	lw	s3,56(sp)
     898:	8fb20034 	lw	s2,52(sp)
     89c:	8fb10030 	lw	s1,48(sp)
     8a0:	8fb0002c 	lw	s0,44(sp)
     8a4:	03e00008 	jr	ra
     8a8:	27bd0040 	addiu	sp,sp,64

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
