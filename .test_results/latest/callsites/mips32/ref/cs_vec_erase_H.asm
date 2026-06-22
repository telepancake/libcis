; call site: cs_vec_erase_H   lib=ref   arch=mips32   size=120 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/mips32/ref.o:     file format elf32-tradbigmips


Disassembly of section .text:

00000228 <cs_vec_erase_H>:
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 228:	27bdffd8 	addiu	sp,sp,-40
 22c:	afb1001c 	sw	s1,28(sp)
 230:	00808825 	move	s1,a0
 234:	afb00018 	sw	s0,24(sp)
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEC4ERKS2_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_iterator.h:1073
 238:	8c840000 	lw	a0,0(a0)
 23c:	8e300004 	lw	s0,4(s1)
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 240:	afbf0024 	sw	ra,36(sp)
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEi():
/usr/mips-linux-gnu/include/c++/12/bits/stl_iterator.h:1144
 244:	24820040 	addiu	v0,a0,64
_ZNSt6vectorI1HSaIS0_EE8_M_eraseEN9__gnu_cxx17__normal_iteratorIPS0_S2_EE():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:178
 248:	1450000b 	bne	v0,s0,278 <cs_vec_erase_H+0x50>
 24c:	afb20020 	sw	s2,32(sp)
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:180
 250:	8e240004 	lw	a0,4(s1)
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 254:	8fbf0024 	lw	ra,36(sp)
 258:	8fb20020 	lw	s2,32(sp)
_ZNSt6vectorI1HSaIS0_EE8_M_eraseEN9__gnu_cxx17__normal_iteratorIPS0_S2_EE():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:180
 25c:	2482ffe0 	addiu	v0,a0,-32
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 260:	8fb00018 	lw	s0,24(sp)
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.h:807
 264:	2484ffe8 	addiu	a0,a0,-24
_ZNSt6vectorI1HSaIS0_EE8_M_eraseEN9__gnu_cxx17__normal_iteratorIPS0_S2_EE():
/usr/mips-linux-gnu/include/c++/12/bits/vector.tcc:180
 268:	ae220004 	sw	v0,4(s1)
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 26c:	8fb1001c 	lw	s1,28(sp)
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.h:807
 270:	08000000 	j	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 274:	27bd0028 	addiu	sp,sp,40
_ZNSt11__copy_moveILb1ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:408
 278:	02028023 	subu	s0,s0,v0
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEi():
/usr/mips-linux-gnu/include/c++/12/bits/stl_iterator.h:1144
 27c:	24840020 	addiu	a0,a0,32
_ZNSt11__copy_moveILb1ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:408
 280:	00108143 	sra	s0,s0,0x5
 284:	1a00fff2 	blez	s0,250 <cs_vec_erase_H+0x28>
 288:	24920020 	addiu	s2,a0,32
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:410
 28c:	0c000044 	jal	110 <_ZN1HaSEOS_.isra.0>
 290:	02402825 	move	a1,s2
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:408
 294:	2610ffff 	addiu	s0,s0,-1
cs_vec_erase_H():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:412
 298:	1000fffa 	b	284 <cs_vec_erase_H+0x5c>
 29c:	02402025 	move	a0,s2

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
