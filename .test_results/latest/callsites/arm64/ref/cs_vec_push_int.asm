; call site: cs_vec_push_int   lib=ref   arch=arm64   size=56 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm64/ref.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000090 <cs_vec_push_int>:
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
  90:	a9be7bfd 	stp	x29, x30, [sp, #-32]!
  94:	910003fd 	mov	x29, sp
_ZNSt6vectorIiSaIiEE9push_backERKi():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:1283
  98:	a9409003 	ldp	x3, x4, [x0, #8]
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
  9c:	b9001fe1 	str	w1, [sp, #28]
_ZNSt6vectorIiSaIiEE9push_backERKi():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:1283
  a0:	eb04007f 	cmp	x3, x4
  a4:	540000a0 	b.eq	b8 <cs_vec_push_int+0x28>  // b.none
_ZSt12construct_atIiJRKiEEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS3_DpOS4_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
  a8:	b8004461 	str	w1, [x3], #4
_ZNSt6vectorIiSaIiEE9push_backERKi():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:1288
  ac:	f9000403 	str	x3, [x0, #8]
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
  b0:	a8c27bfd 	ldp	x29, x30, [sp], #32
  b4:	d65f03c0 	ret
_ZNSt6vectorIiSaIiEE9push_backERKi():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:1292 (discriminator 1)
  b8:	910073e2 	add	x2, sp, #0x1c
  bc:	aa0303e1 	mov	x1, x3
  c0:	94000000 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
  c4:	17fffffb 	b	b0 <cs_vec_push_int+0x20>

Disassembly of section .text._ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7compareERKS4_:

Disassembly of section .text._ZNK1HltERKS_:

Disassembly of section .text._ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_is_localEv:

Disassembly of section .text._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv:

Disassembly of section .text._ZNSt6vectorIiSaIiEE17_M_realloc_insertIJRKiEEEvN9__gnu_cxx17__normal_iteratorIPiS1_EEDpOT_:

Disassembly of section .text._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7_S_copyEPcPKcm:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZN1HC2EOS_:

Disassembly of section .text._ZSt13__adjust_heapIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEElS2_NS0_5__ops15_Iter_less_iterEEvT_T0_SB_T1_T2_.isra.0:

Disassembly of section .text._ZSt10__pop_heapIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_SA_RT0_.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE17_M_realloc_insertIJRKS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_:

Disassembly of section .text._ZSt14__partial_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_SA_T0_:

Disassembly of section .text._ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE8capacityEv:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXsrSt6__and_IJSt6__not_ISt15__is_tuple_likeIT_EESt21is_move_constructibleIS5_ESt18is_move_assignableIS5_EEE5valueEvE4typeERS5_SF_:
