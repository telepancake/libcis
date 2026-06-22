; call site: cs_vec_push_H   lib=ref   arch=mips32   size=76 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/mips32/ref.o:     file format elf32-tradbigmips


Disassembly of section .text:

00000490 <cs_vec_push_H>:
_ZNSt6vectorI1HSaIS0_EE9push_backERKS0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:1280
 490:	8c820004 	lw	v0,4(a0)
 494:	8c830008 	lw	v1,8(a0)
 498:	1043000e 	beq	v0,v1,4d4 <cs_vec_push_H+0x44>
 49c:	00a03025 	move	a2,a1
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 4a0:	27bdffe0 	addiu	sp,sp,-32
 4a4:	afb00018 	sw	s0,24(sp)
 4a8:	00808025 	move	s0,a0
 4ac:	afbf001c 	sw	ra,28(sp)
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_construct.h:97
 4b0:	0c000000 	jal	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 4b4:	00402025 	move	a0,v0
_ZNSt6vectorI1HSaIS0_EE9push_backERKS0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:1285
 4b8:	8e020004 	lw	v0,4(s0)
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 4bc:	8fbf001c 	lw	ra,28(sp)
_ZNSt6vectorI1HSaIS0_EE9push_backERKS0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:1285
 4c0:	24420020 	addiu	v0,v0,32
 4c4:	ae020004 	sw	v0,4(s0)
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 4c8:	8fb00018 	lw	s0,24(sp)
 4cc:	03e00008 	jr	ra
 4d0:	27bd0020 	addiu	sp,sp,32
_ZNSt6vectorI1HSaIS0_EE9push_backERKS0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_vector.h:1289
 4d4:	08000000 	j	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 4d8:	00402825 	move	a1,v0

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
