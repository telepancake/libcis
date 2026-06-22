; call site: cs_vec_sort_H   lib=ref   arch=mips32   size=160 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/mips32/ref.o:     file format elf32-tradbigmips


Disassembly of section .text:

00000c48 <cs_vec_sort_H>:
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
 c48:	27bdffd0 	addiu	sp,sp,-48
 c4c:	00801025 	move	v0,a0
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEC4ERKS2_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_iterator.h:1073
 c50:	8c840000 	lw	a0,0(a0)
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
 c54:	afb10028 	sw	s1,40(sp)
 c58:	afbf002c 	sw	ra,44(sp)
 c5c:	afb00024 	sw	s0,36(sp)
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEC4ERKS2_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_iterator.h:1073
 c60:	8c510004 	lw	s1,4(v0)
_ZSt6__sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algo.h:1940
 c64:	12240011 	beq	s1,a0,cac <cs_vec_sort_H+0x64>
 c68:	02248023 	subu	s0,s1,a0
/usr/mips-linux-gnu/include/c++/12/bits/stl_algo.h:1942
 c6c:	afa4001c 	sw	a0,28(sp)
_ZSt4__lgi():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:1508
 c70:	2406001f 	li	a2,31
_ZN9__gnu_cxxmiIP1HSt6vectorIS1_SaIS1_EEEENS_17__normal_iteratorIT_T0_E15difference_typeERKS9_SC_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_iterator.h:1333
 c74:	00101143 	sra	v0,s0,0x5
_ZSt6__sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algo.h:1942
 c78:	02202825 	move	a1,s1
_ZSt4__lgi():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:1508
 c7c:	70421020 	clz	v0,v0
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algo.h:1852
 c80:	2a100201 	slti	s0,s0,513
_ZSt4__lgi():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algobase.h:1508
 c84:	00c23023 	subu	a2,a2,v0
_ZSt6__sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algo.h:1942
 c88:	0c0002a6 	jal	a98 <_ZSt16__introsort_loopIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEEiNS0_5__ops15_Iter_less_iterEEvT_SA_T0_T1_.isra.0>
 c8c:	00063040 	sll	a2,a2,0x1
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algo.h:1852
 c90:	1600000f 	bnez	s0,cd0 <cs_vec_sort_H+0x88>
 c94:	8fa4001c 	lw	a0,28(sp)
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEi():
/usr/mips-linux-gnu/include/c++/12/bits/stl_iterator.h:1144
 c98:	24900200 	addiu	s0,a0,512
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algo.h:1854
 c9c:	0c0000dc 	jal	370 <_ZSt16__insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_.isra.0>
 ca0:	02002825 	move	a1,s0
_ZSt26__unguarded_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algo.h:1834
 ca4:	16300006 	bne	s1,s0,cc0 <cs_vec_sort_H+0x78>
 ca8:	00000000 	nop
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
 cac:	8fbf002c 	lw	ra,44(sp)
 cb0:	8fb10028 	lw	s1,40(sp)
 cb4:	8fb00024 	lw	s0,36(sp)
 cb8:	03e00008 	jr	ra
 cbc:	27bd0030 	addiu	sp,sp,48
_ZSt26__unguarded_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algo.h:1835
 cc0:	0c0000a8 	jal	2a0 <_ZSt25__unguarded_linear_insertIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops14_Val_less_iterEEvT_T0_.isra.0>
 cc4:	02002025 	move	a0,s0
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEppEv():
/usr/mips-linux-gnu/include/c++/12/bits/stl_iterator.h:1108
 cc8:	1000fff6 	b	ca4 <cs_vec_sort_H+0x5c>
 ccc:	26100020 	addiu	s0,s0,32
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
 cd0:	8fbf002c 	lw	ra,44(sp)
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algo.h:1859
 cd4:	02202825 	move	a1,s1
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
 cd8:	8fb00024 	lw	s0,36(sp)
 cdc:	8fb10028 	lw	s1,40(sp)
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/mips-linux-gnu/include/c++/12/bits/stl_algo.h:1859
 ce0:	080000dc 	j	370 <_ZSt16__insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_.isra.0>
 ce4:	27bd0030 	addiu	sp,sp,48

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
