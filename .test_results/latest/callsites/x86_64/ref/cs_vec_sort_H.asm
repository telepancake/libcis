; call site: cs_vec_sort_H   lib=ref   arch=x86_64   size=167 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/ref.o:     file format elf64-x86-64


Disassembly of section .text:

00000000000008fa <cs_vec_sort_H>:
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:34
 8fa:	f3 0f 1e fa          	endbr64
 8fe:	55                   	push   %rbp
 8ff:	53                   	push   %rbx
 900:	48 83 ec 18          	sub    $0x18,%rsp
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEC4ERKS2_():
/usr/include/c++/13/bits/stl_iterator.h:1077
 904:	48 8b 6f 08          	mov    0x8(%rdi),%rbp
 908:	48 8b 3f             	mov    (%rdi),%rdi
_ZSt6__sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1945 (discriminator 1)
 90b:	48 39 ef             	cmp    %rbp,%rdi
 90e:	0f 84 86 00 00 00    	je     99a <cs_vec_sort_H+0xa0>
_ZN9__gnu_cxxmiIP1HSt6vectorIS1_SaIS1_EEEENS_17__normal_iteratorIT_T0_E15difference_typeERKS9_SC_():
/usr/include/c++/13/bits/stl_iterator.h:1337 (discriminator 2)
 914:	48 89 eb             	mov    %rbp,%rbx
_ZSt6__sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1947
 917:	48 89 ee             	mov    %rbp,%rsi
 91a:	48 89 7c 24 08       	mov    %rdi,0x8(%rsp)
_ZN9__gnu_cxxmiIP1HSt6vectorIS1_SaIS1_EEEENS_17__normal_iteratorIT_T0_E15difference_typeERKS9_SC_():
/usr/include/c++/13/bits/stl_iterator.h:1337 (discriminator 2)
 91f:	48 ba cd cc cc cc cc 	movabs $0xcccccccccccccccd,%rdx
 926:	cc cc cc 
 929:	48 29 fb             	sub    %rdi,%rbx
 92c:	48 89 d8             	mov    %rbx,%rax
 92f:	48 c1 f8 03          	sar    $0x3,%rax
 933:	48 0f af c2          	imul   %rdx,%rax
_ZSt13__countl_zeroImEiT_():
/usr/include/c++/13/bit:215
 937:	48 0f bd d0          	bsr    %rax,%rdx
 93b:	48 83 f2 3f          	xor    $0x3f,%rdx
 93f:	48 85 c0             	test   %rax,%rax
 942:	b8 40 00 00 00       	mov    $0x40,%eax
 947:	0f 45 c2             	cmovne %edx,%eax
_ZSt4__lgIlET_S0_():
/usr/include/c++/13/bits/stl_algobase.h:1518 (discriminator 1)
 94a:	ba 3f 00 00 00       	mov    $0x3f,%edx
 94f:	29 c2                	sub    %eax,%edx
 951:	48 63 d2             	movslq %edx,%rdx
_ZSt6__sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1947
 954:	48 01 d2             	add    %rdx,%rdx
 957:	e8 4b fe ff ff       	call   7a7 <_ZSt16__introsort_loopIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEElNS0_5__ops15_Iter_less_iterEEvT_SA_T0_T1_.isra.0>
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1857 (discriminator 1)
 95c:	48 81 fb 80 02 00 00 	cmp    $0x280,%rbx
 963:	48 8b 7c 24 08       	mov    0x8(%rsp),%rdi
 968:	7e 22                	jle    98c <cs_vec_sort_H+0x92>
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEl():
/usr/include/c++/13/bits/stl_iterator.h:1148
 96a:	48 8d 9f 80 02 00 00 	lea    0x280(%rdi),%rbx
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1859 (discriminator 1)
 971:	48 89 de             	mov    %rbx,%rsi
 974:	e8 c2 f8 ff ff       	call   23b <_ZSt16__insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_.isra.0>
_ZSt26__unguarded_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1839 (discriminator 1)
 979:	48 39 dd             	cmp    %rbx,%rbp
 97c:	74 1c                	je     99a <cs_vec_sort_H+0xa0>
/usr/include/c++/13/bits/stl_algo.h:1840
 97e:	48 89 df             	mov    %rbx,%rdi
 981:	48 83 c3 28          	add    $0x28,%rbx
 985:	e8 26 f8 ff ff       	call   1b0 <_ZSt25__unguarded_linear_insertIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops14_Val_less_iterEEvT_T0_.isra.0>
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEppEv():
/usr/include/c++/13/bits/stl_iterator.h:1112
 98a:	eb ed                	jmp    979 <cs_vec_sort_H+0x7f>
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:34
 98c:	48 83 c4 18          	add    $0x18,%rsp
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1864
 990:	48 89 ee             	mov    %rbp,%rsi
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:34
 993:	5b                   	pop    %rbx
 994:	5d                   	pop    %rbp
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/include/c++/13/bits/stl_algo.h:1864
 995:	e9 a1 f8 ff ff       	jmp    23b <_ZSt16__insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_.isra.0>
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:34
 99a:	48 83 c4 18          	add    $0x18,%rsp
 99e:	5b                   	pop    %rbx
 99f:	5d                   	pop    %rbp
 9a0:	c3                   	ret

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
