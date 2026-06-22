; call site: cs_vec_push_int   lib=ref   arch=arm32   size=32 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm32/ref.o:     file format elf32-littlearm


Disassembly of section .text:

00000066 <cs_vec_push_int>:
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
  66:	b513      	push	{r0, r1, r4, lr}
_ZNSt6vectorIiSaIiEE9push_backERKi():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:1283
  68:	e9d0 3401 	ldrd	r3, r4, [r0, #4]
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
  6c:	9101      	str	r1, [sp, #4]
_ZNSt6vectorIiSaIiEE9push_backERKi():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:1283
  6e:	42a3      	cmp	r3, r4
  70:	d004      	beq.n	7c <cs_vec_push_int+0x16>
_ZSt12construct_atIiJRKiEEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS3_DpOS4_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
  72:	f843 1b04 	str.w	r1, [r3], #4
_ZNSt6vectorIiSaIiEE9push_backERKi():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:1288
  76:	6043      	str	r3, [r0, #4]
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
  78:	b002      	add	sp, #8
  7a:	bd10      	pop	{r4, pc}
_ZNSt6vectorIiSaIiEE9push_backERKi():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:1292 (discriminator 1)
  7c:	aa01      	add	r2, sp, #4
  7e:	4619      	mov	r1, r3
  80:	f7ff fffe 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
  84:	e7f8      	b.n	78 <cs_vec_push_int+0x12>

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
