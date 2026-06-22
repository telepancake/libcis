; call site: cs_vec_push_int   lib=ref   arch=x86_64   size=54 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/.test_results/latest/callsites/x86_64/ref.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000055 <cs_vec_push_int>:
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:29
  55:	f3 0f 1e fa          	endbr64
  59:	48 83 ec 18          	sub    $0x18,%rsp
  5d:	89 74 24 0c          	mov    %esi,0xc(%rsp)
_ZNSt6vectorIiSaIiEE9push_backERKi():
/usr/include/c++/13/bits/stl_vector.h:1283
  61:	48 8b 77 08          	mov    0x8(%rdi),%rsi
  65:	48 3b 77 10          	cmp    0x10(%rdi),%rsi
  69:	74 11                	je     7c <cs_vec_push_int+0x27>
_ZSt12construct_atIiJRKiEEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS3_DpOS4_():
/usr/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
  6b:	8b 44 24 0c          	mov    0xc(%rsp),%eax
_ZNSt6vectorIiSaIiEE9push_backERKi():
/usr/include/c++/13/bits/stl_vector.h:1288
  6f:	48 83 c6 04          	add    $0x4,%rsi
_ZSt12construct_atIiJRKiEEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS3_DpOS4_():
/usr/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
  73:	89 46 fc             	mov    %eax,-0x4(%rsi)
_ZNSt6vectorIiSaIiEE9push_backERKi():
/usr/include/c++/13/bits/stl_vector.h:1288
  76:	48 89 77 08          	mov    %rsi,0x8(%rdi)
  7a:	eb 0a                	jmp    86 <cs_vec_push_int+0x31>
/usr/include/c++/13/bits/stl_vector.h:1292 (discriminator 1)
  7c:	48 8d 54 24 0c       	lea    0xc(%rsp),%rdx
  81:	e8 00 00 00 00       	call   86 <cs_vec_push_int+0x31>
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:29
  86:	48 83 c4 18          	add    $0x18,%rsp
  8a:	c3                   	ret

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
