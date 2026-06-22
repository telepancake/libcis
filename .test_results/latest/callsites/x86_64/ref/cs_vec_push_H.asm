; call site: cs_vec_push_H   lib=ref   arch=x86_64   size=45 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/.test_results/latest/callsites/x86_64/ref.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000315 <cs_vec_push_H>:
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:30
 315:	f3 0f 1e fa          	endbr64
 319:	53                   	push   %rbx
 31a:	48 89 fb             	mov    %rdi,%rbx
_ZNSt6vectorI1HSaIS0_EE9push_backERKS0_():
/usr/include/c++/13/bits/stl_vector.h:1283
 31d:	48 8b 7f 08          	mov    0x8(%rdi),%rdi
 321:	48 3b 7b 10          	cmp    0x10(%rbx),%rdi
 325:	74 0c                	je     333 <cs_vec_push_H+0x1e>
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 327:	e8 00 00 00 00       	call   32c <cs_vec_push_H+0x17>
_ZNSt6vectorI1HSaIS0_EE9push_backERKS0_():
/usr/include/c++/13/bits/stl_vector.h:1288
 32c:	48 83 43 08 28       	addq   $0x28,0x8(%rbx)
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:30
 331:	5b                   	pop    %rbx
 332:	c3                   	ret
_ZNSt6vectorI1HSaIS0_EE9push_backERKS0_():
/usr/include/c++/13/bits/stl_vector.h:1292 (discriminator 1)
 333:	48 89 f2             	mov    %rsi,%rdx
 336:	48 89 fe             	mov    %rdi,%rsi
 339:	48 89 df             	mov    %rbx,%rdi
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:30
 33c:	5b                   	pop    %rbx
_ZNSt6vectorI1HSaIS0_EE9push_backERKS0_():
/usr/include/c++/13/bits/stl_vector.h:1292 (discriminator 1)
 33d:	e9 00 00 00 00       	jmp    342 <cs_vec_insert_H>

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
