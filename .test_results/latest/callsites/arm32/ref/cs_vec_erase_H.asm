; call site: cs_vec_erase_H   lib=ref   arch=arm32   size=60 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm32/ref.o:     file format elf32-littlearm


Disassembly of section .text:

00000102 <cs_vec_erase_H>:
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 102:	b570      	push	{r4, r5, r6, lr}
 104:	4605      	mov	r5, r0
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEC4ERKS2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_iterator.h:1077
 106:	6800      	ldr	r0, [r0, #0]
 108:	686c      	ldr	r4, [r5, #4]
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEi():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_iterator.h:1148
 10a:	f100 0340 	add.w	r3, r0, #64	@ 0x40
_ZNSt6vectorI1HSaIS0_EE8_M_eraseEN9__gnu_cxx17__normal_iteratorIPS0_S2_EE():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:184 (discriminator 3)
 10e:	42a3      	cmp	r3, r4
 110:	d108      	bne.n	124 <cs_vec_erase_H+0x22>
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:186
 112:	6868      	ldr	r0, [r5, #4]
 114:	f1a0 0320 	sub.w	r3, r0, #32
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.h:804
 118:	3818      	subs	r0, #24
_ZNSt6vectorI1HSaIS0_EE8_M_eraseEN9__gnu_cxx17__normal_iteratorIPS0_S2_EE():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:186
 11a:	606b      	str	r3, [r5, #4]
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 11c:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.h:804
 120:	f7ff bffe 	b.w	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
_ZNSt11__copy_moveILb1ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:411
 124:	1ae4      	subs	r4, r4, r3
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEi():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_iterator.h:1148
 126:	3020      	adds	r0, #32
_ZNSt11__copy_moveILb1ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:411
 128:	1164      	asrs	r4, r4, #5
 12a:	f100 0620 	add.w	r6, r0, #32
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:411 (discriminator 1)
 12e:	2c00      	cmp	r4, #0
 130:	ddef      	ble.n	112 <cs_vec_erase_H+0x10>
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:413
 132:	4631      	mov	r1, r6
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:411 (discriminator 3)
 134:	3c01      	subs	r4, #1
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:413
 136:	f7ff ffa6 	bl	86 <_ZN1HaSEOS_.isra.0>
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:415
 13a:	4630      	mov	r0, r6
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:411 (discriminator 3)
 13c:	e7f5      	b.n	12a <cs_vec_erase_H+0x28>

Disassembly of section .text._ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_is_localEv:

Disassembly of section .text._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv:

Disassembly of section .text._ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7compareERKS4_:

Disassembly of section .text._ZNK1HltERKS_:

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
