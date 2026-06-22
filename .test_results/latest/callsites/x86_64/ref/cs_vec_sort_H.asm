; call site: cs_vec_sort_H   lib=ref   arch=x86_64   size=167 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/x86_64/ref.o:     file format elf64-x86-64


Disassembly of section .text:

000000000000092e <cs_vec_sort_H>:
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
 92e:	f3 0f 1e fa          	endbr64
 932:	55                   	push   %rbp
 933:	53                   	push   %rbx
 934:	48 83 ec 18          	sub    $0x18,%rsp
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEC4ERKS2_():
/usr/include/c++/13/bits/stl_iterator.h:1077
 938:	48 8b 6f 08          	mov    0x8(%rdi),%rbp
 93c:	48 8b 3f             	mov    (%rdi),%rdi
_ZSt6__sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1945 (discriminator 1)
 93f:	48 39 ef             	cmp    %rbp,%rdi
 942:	0f 84 86 00 00 00    	je     9ce <cs_vec_sort_H+0xa0>
_ZN9__gnu_cxxmiIP1HSt6vectorIS1_SaIS1_EEEENS_17__normal_iteratorIT_T0_E15difference_typeERKS9_SC_():
/usr/include/c++/13/bits/stl_iterator.h:1337 (discriminator 2)
 948:	48 89 eb             	mov    %rbp,%rbx
_ZSt6__sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1947
 94b:	48 89 ee             	mov    %rbp,%rsi
 94e:	48 89 7c 24 08       	mov    %rdi,0x8(%rsp)
_ZN9__gnu_cxxmiIP1HSt6vectorIS1_SaIS1_EEEENS_17__normal_iteratorIT_T0_E15difference_typeERKS9_SC_():
/usr/include/c++/13/bits/stl_iterator.h:1337 (discriminator 2)
 953:	48 ba cd cc cc cc cc 	movabs $0xcccccccccccccccd,%rdx
 95a:	cc cc cc 
 95d:	48 29 fb             	sub    %rdi,%rbx
 960:	48 89 d8             	mov    %rbx,%rax
 963:	48 c1 f8 03          	sar    $0x3,%rax
 967:	48 0f af c2          	imul   %rdx,%rax
_ZSt13__countl_zeroImEiT_():
/usr/include/c++/13/bit:215
 96b:	48 0f bd d0          	bsr    %rax,%rdx
 96f:	48 83 f2 3f          	xor    $0x3f,%rdx
 973:	48 85 c0             	test   %rax,%rax
 976:	b8 40 00 00 00       	mov    $0x40,%eax
 97b:	0f 45 c2             	cmovne %edx,%eax
_ZSt4__lgIlET_S0_():
/usr/include/c++/13/bits/stl_algobase.h:1518 (discriminator 1)
 97e:	ba 3f 00 00 00       	mov    $0x3f,%edx
 983:	29 c2                	sub    %eax,%edx
 985:	48 63 d2             	movslq %edx,%rdx
_ZSt6__sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1947
 988:	48 01 d2             	add    %rdx,%rdx
 98b:	e8 4b fe ff ff       	call   7db <_ZSt16__introsort_loopIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEElNS0_5__ops15_Iter_less_iterEEvT_SA_T0_T1_.isra.0>
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1857 (discriminator 1)
 990:	48 81 fb 80 02 00 00 	cmp    $0x280,%rbx
 997:	48 8b 7c 24 08       	mov    0x8(%rsp),%rdi
 99c:	7e 22                	jle    9c0 <cs_vec_sort_H+0x92>
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEl():
/usr/include/c++/13/bits/stl_iterator.h:1148
 99e:	48 8d 9f 80 02 00 00 	lea    0x280(%rdi),%rbx
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1859 (discriminator 1)
 9a5:	48 89 de             	mov    %rbx,%rsi
 9a8:	e8 c2 f8 ff ff       	call   26f <_ZSt16__insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_.isra.0>
_ZSt26__unguarded_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1839 (discriminator 1)
 9ad:	48 39 dd             	cmp    %rbx,%rbp
 9b0:	74 1c                	je     9ce <cs_vec_sort_H+0xa0>
/usr/include/c++/13/bits/stl_algo.h:1840
 9b2:	48 89 df             	mov    %rbx,%rdi
 9b5:	48 83 c3 28          	add    $0x28,%rbx
 9b9:	e8 26 f8 ff ff       	call   1e4 <_ZSt25__unguarded_linear_insertIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops14_Val_less_iterEEvT_T0_.isra.0>
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEppEv():
/usr/include/c++/13/bits/stl_iterator.h:1112
 9be:	eb ed                	jmp    9ad <cs_vec_sort_H+0x7f>
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
 9c0:	48 83 c4 18          	add    $0x18,%rsp
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1864
 9c4:	48 89 ee             	mov    %rbp,%rsi
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
 9c7:	5b                   	pop    %rbx
 9c8:	5d                   	pop    %rbp
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1864
 9c9:	e9 a1 f8 ff ff       	jmp    26f <_ZSt16__insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_.isra.0>
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
 9ce:	48 83 c4 18          	add    $0x18,%rsp
 9d2:	5b                   	pop    %rbx
 9d3:	5d                   	pop    %rbp
 9d4:	c3                   	ret

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
