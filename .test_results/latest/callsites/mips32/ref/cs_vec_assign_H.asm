; call site: cs_vec_assign_H   lib=ref   arch=mips32   size=504 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/mips32/ref.o:     file format elf32-tradbigmips


Disassembly of section .text:

00000740 <cs_vec_assign_H>:
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:207
 740:	1085007b 	beq	a0,a1,930 <cs_vec_assign_H+0x1f0>
 744:	00000000 	nop
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 748:	27bdffc8 	addiu	sp,sp,-56
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:1077
 74c:	8c820008 	lw	v0,8(a0)
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 750:	3c1c0000 	lui	gp,0x0
 754:	afb30024 	sw	s3,36(sp)
 758:	279c0000 	addiu	gp,gp,0
 75c:	afb20020 	sw	s2,32(sp)
 760:	afb1001c 	sw	s1,28(sp)
 764:	afb40028 	sw	s4,40(sp)
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:990
 768:	8cb30004 	lw	s3,4(a1)
 76c:	8cb10000 	lw	s1,0(a1)
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:1077
 770:	8c920000 	lw	s2,0(a0)
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 774:	afb5002c 	sw	s5,44(sp)
 778:	00a0a825 	move	s5,a1
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:990
 77c:	0271a023 	subu	s4,s3,s1
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 780:	afb00018 	sw	s0,24(sp)
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:1077
 784:	00521023 	subu	v0,v0,s2
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 788:	afbf0034 	sw	ra,52(sp)
 78c:	afb60030 	sw	s6,48(sp)
 790:	00808025 	move	s0,a0
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:230
 794:	0054102b 	sltu	v0,v0,s4
 798:	10400034 	beqz	v0,86c <cs_vec_assign_H+0x12c>
 79c:	afbc0010 	sw	gp,16(sp)
_ZNSt15__new_allocatorI1HE8allocateEjPKv():
/usr/mips-linux-gnu/include/c++/12/bits/new_allocator.h:131
 7a0:	3c027fff 	lui	v0,0x7fff
 7a4:	3442ffe1 	ori	v0,v0,0xffe1
 7a8:	0282102b 	sltu	v0,s4,v0
 7ac:	14400004 	bnez	v0,7c0 <cs_vec_assign_H+0x80>
 7b0:	8f990000 	lw	t9,0(gp)
/usr/mips-linux-gnu/include/c++/12/bits/new_allocator.h:136
 7b4:	8f990000 	lw	t9,0(gp)
 7b8:	0320f809 	jalr	t9
 7bc:	00000000 	nop
/usr/mips-linux-gnu/include/c++/12/bits/new_allocator.h:148
 7c0:	0320f809 	jalr	t9
 7c4:	02802025 	move	a0,s4
 7c8:	8fbc0010 	lw	gp,16(sp)
 7cc:	00409025 	move	s2,v0
_ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1HSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_uninitialized.h:116
 7d0:	0040a825 	move	s5,v0
/usr/mips-linux-gnu/include/c++/12/bits/stl_uninitialized.h:119
 7d4:	1671001b 	bne	s3,s1,844 <cs_vec_assign_H+0x104>
 7d8:	02202825 	move	a1,s1
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:234
 7dc:	8e110000 	lw	s1,0(s0)
 7e0:	8e130004 	lw	s3,4(s0)
_ZNSt12_Destroy_auxILb0EE9__destroyIP1HEEvT_S4_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_construct.h:162
 7e4:	1671001d 	bne	s3,s1,85c <cs_vec_assign_H+0x11c>
 7e8:	26240008 	addiu	a0,s1,8
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:236
 7ec:	8e040000 	lw	a0,0(s0)
_ZNSt12_Vector_baseI1HSaIS0_EE13_M_deallocateEPS0_j():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:386
 7f0:	10800004 	beqz	a0,804 <cs_vec_assign_H+0xc4>
 7f4:	8f990000 	lw	t9,0(gp)
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:238
 7f8:	8e050008 	lw	a1,8(s0)
_ZNSt15__new_allocatorI1HE10deallocateEPS0_j():
/usr/mips-linux-gnu/include/c++/12/bits/new_allocator.h:169
 7fc:	0320f809 	jalr	t9
 800:	00a42823 	subu	a1,a1,a0
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:239
 804:	ae120000 	sw	s2,0(s0)
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:240
 808:	02549021 	addu	s2,s2,s4
 80c:	ae120008 	sw	s2,8(s0)
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:256
 810:	8e020000 	lw	v0,0(s0)
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 814:	8fbf0034 	lw	ra,52(sp)
 818:	8fb60030 	lw	s6,48(sp)
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:256
 81c:	00541021 	addu	v0,v0,s4
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 820:	8fb5002c 	lw	s5,44(sp)
 824:	8fb40028 	lw	s4,40(sp)
 828:	8fb30024 	lw	s3,36(sp)
 82c:	8fb20020 	lw	s2,32(sp)
 830:	8fb1001c 	lw	s1,28(sp)
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:256
 834:	ae020004 	sw	v0,4(s0)
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 838:	8fb00018 	lw	s0,24(sp)
 83c:	03e00008 	jr	ra
 840:	27bd0038 	addiu	sp,sp,56
_ZSt10_ConstructI1HJRKS0_EEvPT_DpOT0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_construct.h:119
 844:	0c000000 	jal	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 848:	02a02025 	move	a0,s5
_ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1HSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_uninitialized.h:119
 84c:	26b50020 	addiu	s5,s5,32
_ZSt10_ConstructI1HJRKS0_EEvPT_DpOT0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_construct.h:119
 850:	8fbc0010 	lw	gp,16(sp)
 854:	1000ffdf 	b	7d4 <cs_vec_assign_H+0x94>
 858:	26310020 	addiu	s1,s1,32
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.h:807
 85c:	0c000000 	jal	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 860:	26310020 	addiu	s1,s1,32
cs_vec_assign_H():
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.h:807
 864:	1000ffdf 	b	7e4 <cs_vec_assign_H+0xa4>
 868:	8fbc0010 	lw	gp,16(sp)
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:990
 86c:	8c930004 	lw	s3,4(a0)
 870:	02729823 	subu	s3,s3,s2
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:242
 874:	0274102b 	sltu	v0,s3,s4
 878:	14400017 	bnez	v0,8d8 <cs_vec_assign_H+0x198>
 87c:	00139943 	sra	s3,s3,0x5
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:990
 880:	00149943 	sra	s3,s4,0x5
 884:	0240b025 	move	s6,s2
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIPK1HPS3_EET0_T_S8_S7_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:383
 888:	0260a825 	move	s5,s3
 88c:	1ea0000c 	bgtz	s5,8c0 <cs_vec_assign_H+0x180>
 890:	02202825 	move	a1,s1
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEC4ERKS2_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_iterator.h:1072
 894:	2a620000 	slti	v0,s3,0
/usr/mips-linux-gnu/include/c++/12/bits/stl_iterator.h:1073
 898:	8e110004 	lw	s1,4(s0)
_ZNSt12_Destroy_auxILb0EE9__destroyIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS4_SaIS4_EEEEEEvT_SA_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_construct.h:160
 89c:	0002980b 	movn	s3,zero,v0
 8a0:	00139940 	sll	s3,s3,0x5
 8a4:	02539021 	addu	s2,s2,s3
/usr/mips-linux-gnu/include/c++/12/bits/stl_construct.h:162
 8a8:	1232ffd9 	beq	s1,s2,810 <cs_vec_assign_H+0xd0>
 8ac:	26440008 	addiu	a0,s2,8
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.h:807
 8b0:	0c000000 	jal	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 8b4:	26520020 	addiu	s2,s2,32
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEppEv():
/usr/mips-linux-gnu/include/c++/12/bits/stl_iterator.h:1108
 8b8:	1000fffb 	b	8a8 <cs_vec_assign_H+0x168>
 8bc:	00000000 	nop
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIPK1HPS3_EET0_T_S8_S7_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:385
 8c0:	0c000189 	jal	624 <_ZN1HaSERKS_.isra.0>
 8c4:	02c02025 	move	a0,s6
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:386
 8c8:	26310020 	addiu	s1,s1,32
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:387
 8cc:	26d60020 	addiu	s6,s6,32
cs_vec_assign_H():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:383
 8d0:	1000ffee 	b	88c <cs_vec_assign_H+0x14c>
 8d4:	26b5ffff 	addiu	s5,s5,-1
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:383
 8d8:	1e60000f 	bgtz	s3,918 <cs_vec_assign_H+0x1d8>
 8dc:	02202825 	move	a1,s1
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:990
 8e0:	8e120004 	lw	s2,4(s0)
 8e4:	8e020000 	lw	v0,0(s0)
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:251
 8e8:	8eb10000 	lw	s1,0(s5)
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:252
 8ec:	8eb30004 	lw	s3,4(s5)
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:990
 8f0:	02421023 	subu	v0,s2,v0
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:251
 8f4:	02228821 	addu	s1,s1,v0
_ZSt16__do_uninit_copyIP1HS1_ET0_T_S3_S2_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_uninitialized.h:119
 8f8:	1271ffc5 	beq	s3,s1,810 <cs_vec_assign_H+0xd0>
 8fc:	00000000 	nop
_ZSt10_ConstructI1HJRS0_EEvPT_DpOT0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_construct.h:119
 900:	02202825 	move	a1,s1
 904:	0c000000 	jal	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 908:	02402025 	move	a0,s2
_ZSt16__do_uninit_copyIP1HS1_ET0_T_S3_S2_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_uninitialized.h:119
 90c:	26310020 	addiu	s1,s1,32
cs_vec_assign_H():
/usr/mips-linux-gnu/include/c++/12/bits/stl_uninitialized.h:119
 910:	1000fff9 	b	8f8 <cs_vec_assign_H+0x1b8>
 914:	26520020 	addiu	s2,s2,32
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:385
 918:	0c000189 	jal	624 <_ZN1HaSERKS_.isra.0>
 91c:	02402025 	move	a0,s2
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:386
 920:	26310020 	addiu	s1,s1,32
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:387
 924:	26520020 	addiu	s2,s2,32
cs_vec_assign_H():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:383
 928:	1000ffeb 	b	8d8 <cs_vec_assign_H+0x198>
 92c:	2673ffff 	addiu	s3,s3,-1
 930:	03e00008 	jr	ra
 934:	00000000 	nop

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
