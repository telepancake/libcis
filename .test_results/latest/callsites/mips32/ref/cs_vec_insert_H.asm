; call site: cs_vec_insert_H   lib=ref   arch=mips32   size=328 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/mips32/ref.o:     file format elf32-tradbigmips


Disassembly of section .text:

000004dc <cs_vec_insert_H>:
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 4dc:	3c1c0000 	lui	gp,0x0
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:141
 4e0:	8c830008 	lw	v1,8(a0)
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 4e4:	27bdffa8 	addiu	sp,sp,-88
 4e8:	279c0000 	addiu	gp,gp,0
 4ec:	afb3004c 	sw	s3,76(sp)
 4f0:	8f930000 	lw	s3,0(gp)
 4f4:	afb10044 	sw	s1,68(sp)
 4f8:	afbf0054 	sw	ra,84(sp)
 4fc:	afb40050 	sw	s4,80(sp)
 500:	8e620000 	lw	v0,0(s3)
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEi():
/usr/mips-linux-gnu/include/c++/12/bits/stl_iterator.h:1144
 504:	8c910000 	lw	s1,0(a0)
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 508:	afb20048 	sw	s2,72(sp)
 50c:	afa2003c 	sw	v0,60(sp)
 510:	afb00040 	sw	s0,64(sp)
 514:	afbc0010 	sw	gp,16(sp)
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:141
 518:	8c820004 	lw	v0,4(a0)
 51c:	10430031 	beq	v0,v1,5e4 <cs_vec_insert_H+0x108>
 520:	26310020 	addiu	s1,s1,32
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:142
 524:	16220012 	bne	s1,v0,570 <cs_vec_insert_H+0x94>
 528:	00808025 	move	s0,a0
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_construct.h:97
 52c:	0c000000 	jal	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 530:	02202025 	move	a0,s1
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:147
 534:	8e020004 	lw	v0,4(s0)
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_construct.h:97
 538:	8fbc0010 	lw	gp,16(sp)
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:147
 53c:	24420020 	addiu	v0,v0,32
 540:	ae020004 	sw	v0,4(s0)
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 544:	8fa3003c 	lw	v1,60(sp)
 548:	8e620000 	lw	v0,0(s3)
 54c:	14620029 	bne	v1,v0,5f4 <cs_vec_insert_H+0x118>
 550:	8fbf0054 	lw	ra,84(sp)
 554:	8fb40050 	lw	s4,80(sp)
 558:	8fb3004c 	lw	s3,76(sp)
 55c:	8fb20048 	lw	s2,72(sp)
 560:	8fb10044 	lw	s1,68(sp)
 564:	8fb00040 	lw	s0,64(sp)
 568:	03e00008 	jr	ra
 56c:	27bd0058 	addiu	sp,sp,88
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_construct.h:97
 570:	27b4001c 	addiu	s4,sp,28
_ZNSt6vectorI1HSaIS0_EE16_Temporary_valueC4IJRKS0_EEEPS2_DpOT_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:1830
 574:	afa40018 	sw	a0,24(sp)
_ZNSt6vectorI1HSaIS0_EE16_Temporary_value8_StorageC4Ev():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:1849
 578:	a3a0001c 	sb	zero,28(sp)
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_construct.h:97
 57c:	0c000000 	jal	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 580:	02802025 	move	a0,s4
_ZNSt6vectorI1HSaIS0_EE13_M_insert_auxIS0_EEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEOT_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:417
 584:	8e040004 	lw	a0,4(s0)
_ZSt12construct_atI1HJS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS2_DpOS3_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_construct.h:97
 588:	0c000000 	jal	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 58c:	2485ffe0 	addiu	a1,a0,-32
_ZNSt6vectorI1HSaIS0_EE13_M_insert_auxIS0_EEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEOT_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:419
 590:	8e120004 	lw	s2,4(s0)
 594:	26420020 	addiu	v0,s2,32
 598:	ae020004 	sw	v0,4(s0)
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:424
 59c:	2650ffe0 	addiu	s0,s2,-32
_ZNSt20__copy_move_backwardILb1ELb0ESt26random_access_iterator_tagE13__copy_move_bIP1HS4_EET0_T_S6_S5_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:723
 5a0:	02118023 	subu	s0,s0,s1
 5a4:	00108143 	sra	s0,s0,0x5
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:724
 5a8:	1e000008 	bgtz	s0,5cc <cs_vec_insert_H+0xf0>
 5ac:	2645ffc0 	addiu	a1,s2,-64
_ZNSt6vectorI1HSaIS0_EE13_M_insert_auxIS0_EEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEOT_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:430
 5b0:	02802825 	move	a1,s4
 5b4:	0c000044 	jal	110 <_ZN1HaSEOS_.isra.0>
 5b8:	02202025 	move	a0,s1
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.h:807
 5bc:	0c000000 	jal	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 5c0:	27a40024 	addiu	a0,sp,36
cs_vec_insert_H():
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.h:807
 5c4:	1000ffdf 	b	544 <cs_vec_insert_H+0x68>
 5c8:	8fbc0010 	lw	gp,16(sp)
_ZNSt20__copy_move_backwardILb1ELb0ESt26random_access_iterator_tagE13__copy_move_bIP1HS4_EET0_T_S6_S5_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:725
 5cc:	2652ffe0 	addiu	s2,s2,-32
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:724
 5d0:	2610ffff 	addiu	s0,s0,-1
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:725
 5d4:	0c000044 	jal	110 <_ZN1HaSEOS_.isra.0>
 5d8:	02402025 	move	a0,s2
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:724
 5dc:	1000fff2 	b	5a8 <cs_vec_insert_H+0xcc>
 5e0:	00000000 	nop
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:164
 5e4:	8fa3003c 	lw	v1,60(sp)
 5e8:	8e620000 	lw	v0,0(s3)
 5ec:	10620004 	beq	v1,v0,600 <cs_vec_insert_H+0x124>
 5f0:	8fbf0054 	lw	ra,84(sp)
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 5f4:	8f990000 	lw	t9,0(gp)
 5f8:	0320f809 	jalr	t9
 5fc:	00000000 	nop
 600:	00a03025 	move	a2,a1
 604:	8fb40050 	lw	s4,80(sp)
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:164
 608:	02202825 	move	a1,s1
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 60c:	8fb3004c 	lw	s3,76(sp)
 610:	8fb20048 	lw	s2,72(sp)
 614:	8fb10044 	lw	s1,68(sp)
 618:	8fb00040 	lw	s0,64(sp)
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:164
 61c:	08000000 	j	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 620:	27bd0058 	addiu	sp,sp,88

Disassembly of section .text._ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7compareERKS4_:

Disassembly of section .text._ZNK1HltERKS_:

Disassembly of section .text._ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_is_localEv:

Disassembly of section .text._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv:

Disassembly of section .text._ZNSt6vectorIiSaIiEE17_M_realloc_insertIJRKiEEEvN9__gnu_cxx17__normal_iteratorIPiS1_EEDpOT_:

Disassembly of section .text._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7_S_copyEPcPKcj:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZN1HC2EOS_:

Disassembly of section .text._ZSt13__adjust_heapIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEEiS2_NS0_5__ops15_Iter_less_iterEEvT_T0_SB_T1_T2_.isra.0:

Disassembly of section .text._ZSt10__pop_heapIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_SA_RT0_.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE17_M_realloc_insertIJRKS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_:

Disassembly of section .text._ZSt14__partial_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_SA_T0_:

Disassembly of section .text._ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE8capacityEv:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXsrSt6__and_IJSt6__not_ISt15__is_tuple_likeIT_EESt21is_move_constructibleIS5_ESt18is_move_assignableIS5_EEE5valueEvE4typeERS5_SF_:
