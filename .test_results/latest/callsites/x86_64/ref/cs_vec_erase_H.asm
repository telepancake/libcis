; call site: cs_vec_erase_H   lib=ref   arch=x86_64   size=102 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/ref.o:     file format elf64-x86-64


Disassembly of section .text:

000000000000017e <cs_vec_erase_H>:
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 17e:	f3 0f 1e fa          	endbr64
 182:	41 54                	push   %r12
 184:	55                   	push   %rbp
 185:	48 89 fd             	mov    %rdi,%rbp
 188:	53                   	push   %rbx
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEC4ERKS2_():
/usr/include/c++/13/bits/stl_iterator.h:1077
 189:	48 8b 3f             	mov    (%rdi),%rdi
 18c:	48 8b 5d 08          	mov    0x8(%rbp),%rbx
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEl():
/usr/include/c++/13/bits/stl_iterator.h:1148
 190:	48 8d 47 50          	lea    0x50(%rdi),%rax
_ZNSt6vectorI1HSaIS0_EE8_M_eraseEN9__gnu_cxx17__normal_iteratorIPS0_S2_EE():
/usr/include/c++/13/bits/vector.tcc:184 (discriminator 3)
 194:	48 39 c3             	cmp    %rax,%rbx
 197:	75 19                	jne    1b2 <cs_vec_erase_H+0x34>
/usr/include/c++/13/bits/vector.tcc:186
 199:	48 8b 7d 08          	mov    0x8(%rbp),%rdi
 19d:	48 8d 47 d8          	lea    -0x28(%rdi),%rax
_ZN1HD4Ev():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:20
 1a1:	48 83 ef 20          	sub    $0x20,%rdi
_ZNSt6vectorI1HSaIS0_EE8_M_eraseEN9__gnu_cxx17__normal_iteratorIPS0_S2_EE():
/usr/include/c++/13/bits/vector.tcc:186
 1a5:	48 89 45 08          	mov    %rax,0x8(%rbp)
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 1a9:	5b                   	pop    %rbx
 1aa:	5d                   	pop    %rbp
 1ab:	41 5c                	pop    %r12
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/include/c++/13/bits/basic_string.h:804
 1ad:	e9 00 00 00 00       	jmp    1b2 <cs_vec_erase_H+0x34>
_ZNSt11__copy_moveILb1ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/include/c++/13/bits/stl_algobase.h:411
 1b2:	48 29 c3             	sub    %rax,%rbx
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEl():
/usr/include/c++/13/bits/stl_iterator.h:1148
 1b5:	48 83 c7 28          	add    $0x28,%rdi
_ZNSt11__copy_moveILb1ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/include/c++/13/bits/stl_algobase.h:411
 1b9:	48 b8 cd cc cc cc cc 	movabs $0xcccccccccccccccd,%rax
 1c0:	cc cc cc 
 1c3:	48 c1 fb 03          	sar    $0x3,%rbx
 1c7:	48 0f af d8          	imul   %rax,%rbx
 1cb:	4c 8d 67 28          	lea    0x28(%rdi),%r12
/usr/include/c++/13/bits/stl_algobase.h:411 (discriminator 1)
 1cf:	48 85 db             	test   %rbx,%rbx
 1d2:	7e c5                	jle    199 <cs_vec_erase_H+0x1b>
/usr/include/c++/13/bits/stl_algobase.h:413
 1d4:	4c 89 e6             	mov    %r12,%rsi
/usr/include/c++/13/bits/stl_algobase.h:411 (discriminator 3)
 1d7:	48 ff cb             	dec    %rbx
/usr/include/c++/13/bits/stl_algobase.h:413
 1da:	e8 e1 fe ff ff       	call   c0 <_ZN1HaSEOS_.isra.0>
/usr/include/c++/13/bits/stl_algobase.h:415
 1df:	4c 89 e7             	mov    %r12,%rdi
/usr/include/c++/13/bits/stl_algobase.h:411 (discriminator 3)
 1e2:	eb e7                	jmp    1cb <cs_vec_erase_H+0x4d>

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
