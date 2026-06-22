; call site: cs_vec_push_H   lib=ref   arch=arm64   size=72 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm64/ref.o:     file format elf64-littleaarch64


Disassembly of section .text:

00000000000003f0 <cs_vec_push_H>:
_ZNSt6vectorI1HSaIS0_EE9push_backERKS0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:1283
 3f0:	a9409003 	ldp	x3, x4, [x0, #8]
 3f4:	eb04007f 	cmp	x3, x4
 3f8:	540001a0 	b.eq	42c <cs_vec_push_H+0x3c>  // b.none
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 3fc:	a9be7bfd 	stp	x29, x30, [sp, #-32]!
 400:	910003fd 	mov	x29, sp
 404:	f9000bf3 	str	x19, [sp, #16]
 408:	aa0003f3 	mov	x19, x0
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 40c:	aa0303e0 	mov	x0, x3
 410:	94000000 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
_ZNSt6vectorI1HSaIS0_EE9push_backERKS0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:1288
 414:	f9400660 	ldr	x0, [x19, #8]
 418:	9100a000 	add	x0, x0, #0x28
 41c:	f9000660 	str	x0, [x19, #8]
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 420:	f9400bf3 	ldr	x19, [sp, #16]
 424:	a8c27bfd 	ldp	x29, x30, [sp], #32
 428:	d65f03c0 	ret
 42c:	aa0103e2 	mov	x2, x1
_ZNSt6vectorI1HSaIS0_EE9push_backERKS0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:1292 (discriminator 1)
 430:	aa0303e1 	mov	x1, x3
 434:	14000000 	b	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>

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
