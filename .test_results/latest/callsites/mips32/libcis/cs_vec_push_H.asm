; call site: cs_vec_push_H   lib=libcis   arch=mips32   size=236 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

000008ac <cs_vec_push_H>:
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
     8ac:	27bdffd0 	addiu	sp,sp,-48
     8b0:	afb10020 	sw	s1,32(sp)
     8b4:	00808825 	move	s1,a0
     8b8:	afb30028 	sw	s3,40(sp)
     8bc:	00a09825 	move	s3,a1
     8c0:	afb0001c 	sw	s0,28(sp)
     8c4:	afbf002c 	sw	ra,44(sp)
     8c8:	afb20024 	sw	s2,36(sp)
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
     8cc:	0c000008 	jal	20 <_ZNKSt6vectorI1HSaIS0_EE4sizeEv.isra.0>
     8d0:	8c840000 	lw	a0,0(a0)
     8d4:	00001825 	move	v1,zero
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
     8d8:	10800006 	beqz	a0,8f4 <cs_vec_push_H+0x48>
     8dc:	00408025 	move	s0,v0
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688
     8e0:	8c82fff8 	lw	v0,-8(a0)
     8e4:	2403000c 	li	v1,12
     8e8:	006001f4 	teq	v1,zero,0x7
     8ec:	0043001b 	divu	zero,v0,v1
     8f0:	00001812 	mflo	v1
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779
     8f4:	16030005 	bne	s0,v1,90c <cs_vec_push_H+0x60>
     8f8:	2412000c 	li	s2,12
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
     8fc:	26050001 	addiu	a1,s0,1
     900:	0c000000 	jal	0 <_ZNKSs8capacityEv.isra.0>
     904:	02202025 	move	a0,s1
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
     908:	2412000c 	li	s2,12
     90c:	8e220000 	lw	v0,0(s1)
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19
     910:	02602825 	move	a1,s3
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
     914:	72121802 	mul	v1,s0,s2
     918:	00628021 	addu	s0,v1,v0
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19
     91c:	0c000000 	jal	0 <_ZNKSs8capacityEv.isra.0>
     920:	02002025 	move	a0,s0
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:794
     924:	2610000c 	addiu	s0,s0,12
     928:	8e240000 	lw	a0,0(s1)
_ZNSt6vectorI1HSaIS0_EE15set_size_elems_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:129
     92c:	1480000d 	bnez	a0,964 <cs_vec_push_H+0xb8>
     930:	02048023 	subu	s0,s0,a0
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
     934:	0c000008 	jal	20 <_ZNKSt6vectorI1HSaIS0_EE4sizeEv.isra.0>
     938:	00002825 	move	a1,zero
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779
     93c:	14a20010 	bne	a1,v0,980 <cs_vec_push_H+0xd4>
     940:	8fbf002c 	lw	ra,44(sp)
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
     944:	02202025 	move	a0,s1
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
     948:	8fb30028 	lw	s3,40(sp)
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
     94c:	24a50001 	addiu	a1,a1,1
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
     950:	8fb20024 	lw	s2,36(sp)
     954:	8fb10020 	lw	s1,32(sp)
     958:	8fb0001c 	lw	s0,28(sp)
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
     95c:	08000000 	j	0 <_ZNKSs8capacityEv.isra.0>
     960:	27bd0030 	addiu	sp,sp,48
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
     964:	0c000008 	jal	20 <_ZNKSt6vectorI1HSaIS0_EE4sizeEv.isra.0>
     968:	ac90fffc 	sw	s0,-4(a0)
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688
     96c:	8c83fff8 	lw	v1,-8(a0)
     970:	024001f4 	teq	s2,zero,0x7
     974:	0072001b 	divu	zero,v1,s2
     978:	1000fff0 	b	93c <cs_vec_push_H+0x90>
     97c:	00002812 	mflo	a1
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
     980:	8fb30028 	lw	s3,40(sp)
     984:	8fb20024 	lw	s2,36(sp)
     988:	8fb10020 	lw	s1,32(sp)
     98c:	8fb0001c 	lw	s0,28(sp)
     990:	03e00008 	jr	ra
     994:	27bd0030 	addiu	sp,sp,48

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
