; call site: cs_vec_sort_H   lib=ref   arch=arm32   size=76 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm32/ref.o:     file format elf32-littlearm


Disassembly of section .text:

00000678 <cs_vec_sort_H>:
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
 678:	b570      	push	{r4, r5, r6, lr}
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEC4ERKS2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_iterator.h:1077
 67a:	e9d0 5600 	ldrd	r5, r6, [r0]
_ZSt6__sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algo.h:1945 (discriminator 1)
 67e:	42ae      	cmp	r6, r5
 680:	d015      	beq.n	6ae <cs_vec_sort_H+0x36>
_ZN9__gnu_cxxmiIP1HSt6vectorIS1_SaIS1_EEEENS_17__normal_iteratorIT_T0_E15difference_typeERKS9_SC_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_iterator.h:1337 (discriminator 2)
 682:	1b74      	subs	r4, r6, r5
_ZSt6__sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algo.h:1947
 684:	4631      	mov	r1, r6
 686:	4628      	mov	r0, r5
_ZN9__gnu_cxxmiIP1HSt6vectorIS1_SaIS1_EEEENS_17__normal_iteratorIT_T0_E15difference_typeERKS9_SC_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_iterator.h:1337 (discriminator 2)
 688:	1162      	asrs	r2, r4, #5
 68a:	fab2 f282 	clz	r2, r2
_ZSt4__lgIiET_S0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:1518 (discriminator 1)
 68e:	f1c2 021f 	rsb	r2, r2, #31
_ZSt6__sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algo.h:1947
 692:	0052      	lsls	r2, r2, #1
 694:	f7ff ff8a 	bl	5ac <_ZSt16__introsort_loopIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEEiNS0_5__ops15_Iter_less_iterEEvT_SA_T0_T1_.isra.0>
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algo.h:1857 (discriminator 1)
 698:	f5b4 7f00 	cmp.w	r4, #512	@ 0x200
 69c:	dd0d      	ble.n	6ba <cs_vec_sort_H+0x42>
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEi():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_iterator.h:1148
 69e:	f505 7400 	add.w	r4, r5, #512	@ 0x200
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algo.h:1859 (discriminator 1)
 6a2:	4628      	mov	r0, r5
 6a4:	4621      	mov	r1, r4
 6a6:	f7ff fd85 	bl	1b4 <_ZSt16__insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_.isra.0>
_ZSt26__unguarded_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algo.h:1839 (discriminator 1)
 6aa:	42a6      	cmp	r6, r4
 6ac:	d100      	bne.n	6b0 <cs_vec_sort_H+0x38>
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
 6ae:	bd70      	pop	{r4, r5, r6, pc}
_ZSt26__unguarded_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algo.h:1840
 6b0:	4620      	mov	r0, r4
 6b2:	3420      	adds	r4, #32
 6b4:	f7ff fd43 	bl	13e <_ZSt25__unguarded_linear_insertIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops14_Val_less_iterEEvT_T0_.isra.0>
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEppEv():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_iterator.h:1112
 6b8:	e7f7      	b.n	6aa <cs_vec_sort_H+0x32>
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algo.h:1864
 6ba:	4631      	mov	r1, r6
 6bc:	4628      	mov	r0, r5
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
 6be:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algo.h:1864
 6c2:	e577      	b.n	1b4 <_ZSt16__insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_.isra.0>

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
