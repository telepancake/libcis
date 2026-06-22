; call site: cs_vec_push_H   lib=ref   arch=arm32   size=38 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm32/ref.o:     file format elf32-littlearm


Disassembly of section .text:

00000258 <cs_vec_push_H>:
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
 258:	b538      	push	{r3, r4, r5, lr}
 25a:	460a      	mov	r2, r1
 25c:	4604      	mov	r4, r0
_ZNSt6vectorI1HSaIS0_EE9push_backERKS0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:1283
 25e:	e9d0 3501 	ldrd	r3, r5, [r0, #4]
 262:	42ab      	cmp	r3, r5
 264:	d006      	beq.n	274 <cs_vec_push_H+0x1c>
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 266:	4618      	mov	r0, r3
 268:	f7ff fffe 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
_ZNSt6vectorI1HSaIS0_EE9push_backERKS0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:1288
 26c:	6863      	ldr	r3, [r4, #4]
 26e:	3320      	adds	r3, #32
 270:	6063      	str	r3, [r4, #4]
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
 272:	bd38      	pop	{r3, r4, r5, pc}
_ZNSt6vectorI1HSaIS0_EE9push_backERKS0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:1292 (discriminator 1)
 274:	4619      	mov	r1, r3
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
 276:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
_ZNSt6vectorI1HSaIS0_EE9push_backERKS0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:1292 (discriminator 1)
 27a:	f7ff bffe 	b.w	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>

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
