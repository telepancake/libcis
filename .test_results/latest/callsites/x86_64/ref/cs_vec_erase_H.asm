; call site: cs_vec_erase_H   lib=ref   arch=x86_64   size=102 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/.test_results/latest/callsites/x86_64/ref.o:     file format elf64-x86-64


Disassembly of section .text:

000000000000014a <cs_vec_erase_H>:
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:32
 14a:	f3 0f 1e fa          	endbr64
 14e:	41 54                	push   %r12
 150:	55                   	push   %rbp
 151:	48 89 fd             	mov    %rdi,%rbp
 154:	53                   	push   %rbx
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEC4ERKS2_():
/usr/include/c++/13/bits/stl_iterator.h:1077
 155:	48 8b 3f             	mov    (%rdi),%rdi
 158:	48 8b 5d 08          	mov    0x8(%rbp),%rbx
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEl():
/usr/include/c++/13/bits/stl_iterator.h:1148
 15c:	48 8d 47 50          	lea    0x50(%rdi),%rax
_ZNSt6vectorI1HSaIS0_EE8_M_eraseEN9__gnu_cxx17__normal_iteratorIPS0_S2_EE():
/usr/include/c++/13/bits/vector.tcc:184 (discriminator 3)
 160:	48 39 c3             	cmp    %rax,%rbx
 163:	75 19                	jne    17e <cs_vec_erase_H+0x34>
/usr/include/c++/13/bits/vector.tcc:186
 165:	48 8b 7d 08          	mov    0x8(%rbp),%rdi
 169:	48 8d 47 d8          	lea    -0x28(%rdi),%rax
_ZN1HD4Ev():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:20
 16d:	48 83 ef 20          	sub    $0x20,%rdi
_ZNSt6vectorI1HSaIS0_EE8_M_eraseEN9__gnu_cxx17__normal_iteratorIPS0_S2_EE():
/usr/include/c++/13/bits/vector.tcc:186
 171:	48 89 45 08          	mov    %rax,0x8(%rbp)
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:32
 175:	5b                   	pop    %rbx
 176:	5d                   	pop    %rbp
 177:	41 5c                	pop    %r12
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/include/c++/13/bits/basic_string.h:804
 179:	e9 00 00 00 00       	jmp    17e <cs_vec_erase_H+0x34>
_ZNSt11__copy_moveILb1ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/include/c++/13/bits/stl_algobase.h:411
 17e:	48 29 c3             	sub    %rax,%rbx
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEl():
/usr/include/c++/13/bits/stl_iterator.h:1148
 181:	48 83 c7 28          	add    $0x28,%rdi
_ZNSt11__copy_moveILb1ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/include/c++/13/bits/stl_algobase.h:411
 185:	48 b8 cd cc cc cc cc 	movabs $0xcccccccccccccccd,%rax
 18c:	cc cc cc 
 18f:	48 c1 fb 03          	sar    $0x3,%rbx
 193:	48 0f af d8          	imul   %rax,%rbx
 197:	4c 8d 67 28          	lea    0x28(%rdi),%r12
/usr/include/c++/13/bits/stl_algobase.h:411 (discriminator 1)
 19b:	48 85 db             	test   %rbx,%rbx
 19e:	7e c5                	jle    165 <cs_vec_erase_H+0x1b>
/usr/include/c++/13/bits/stl_algobase.h:413
 1a0:	4c 89 e6             	mov    %r12,%rsi
/usr/include/c++/13/bits/stl_algobase.h:411 (discriminator 3)
 1a3:	48 ff cb             	dec    %rbx
/usr/include/c++/13/bits/stl_algobase.h:413
 1a6:	e8 e1 fe ff ff       	call   8c <_ZN1HaSEOS_.isra.0>
/usr/include/c++/13/bits/stl_algobase.h:415
 1ab:	4c 89 e7             	mov    %r12,%rdi
/usr/include/c++/13/bits/stl_algobase.h:411 (discriminator 3)
 1ae:	eb e7                	jmp    197 <cs_vec_erase_H+0x4d>

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
