; call site: cs_str_append   lib=ref   arch=x86_64   size=251 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/x86_64/ref.o:     file format elf64-x86-64


Disassembly of section .text:

00000000000006e0 <cs_str_append>:
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
 6e0:	f3 0f 1e fa          	endbr64
 6e4:	41 57                	push   %r15
 6e6:	41 56                	push   %r14
 6e8:	49 89 f6             	mov    %rsi,%r14
 6eb:	41 55                	push   %r13
 6ed:	41 54                	push   %r12
 6ef:	55                   	push   %rbp
 6f0:	53                   	push   %rbx
 6f1:	48 89 fb             	mov    %rdi,%rbx
_ZNSt11char_traitsIcE6lengthEPKc():
/usr/include/c++/13/bits/char_traits.h:399
 6f4:	48 89 f7             	mov    %rsi,%rdi
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
 6f7:	48 83 ec 18          	sub    $0x18,%rsp
 6fb:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
 702:	00 00 
 704:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
 709:	31 c0                	xor    %eax,%eax
_ZNSt11char_traitsIcE6lengthEPKc():
/usr/include/c++/13/bits/char_traits.h:399
 70b:	e8 00 00 00 00       	call   710 <cs_str_append+0x30>
 710:	48 8b 6b 08          	mov    0x8(%rbx),%rbp
 714:	49 89 c4             	mov    %rax,%r12
_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE15_M_check_lengthEmmPKc():
/usr/include/c++/13/bits/basic_string.h:399 (discriminator 2)
 717:	48 b8 ff ff ff ff ff 	movabs $0x7fffffffffffffff,%rax
 71e:	ff ff 7f 
 721:	48 29 e8             	sub    %rbp,%rax
 724:	4c 39 e0             	cmp    %r12,%rax
 727:	73 0c                	jae    735 <cs_str_append+0x55>
/usr/include/c++/13/bits/basic_string.h:400
 729:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 730 <cs_str_append+0x50>
 730:	e8 00 00 00 00       	call   735 <cs_str_append+0x55>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_appendEPKcm():
/usr/include/c++/13/bits/basic_string.tcc:418
 735:	48 89 df             	mov    %rbx,%rdi
/usr/include/c++/13/bits/basic_string.tcc:416 (discriminator 1)
 738:	4d 8d 3c 2c          	lea    (%r12,%rbp,1),%r15
/usr/include/c++/13/bits/basic_string.tcc:418
 73c:	e8 00 00 00 00       	call   741 <cs_str_append+0x61>
 741:	48 89 c6             	mov    %rax,%rsi
/usr/include/c++/13/bits/basic_string.tcc:418 (discriminator 1)
 744:	4c 39 f8             	cmp    %r15,%rax
 747:	72 18                	jb     761 <cs_str_append+0x81>
/usr/include/c++/13/bits/basic_string.tcc:420
 749:	4d 85 e4             	test   %r12,%r12
 74c:	74 5d                	je     7ab <cs_str_append+0xcb>
/usr/include/c++/13/bits/basic_string.tcc:421 (discriminator 2)
 74e:	48 8b 3b             	mov    (%rbx),%rdi
 751:	4c 89 e2             	mov    %r12,%rdx
 754:	4c 89 f6             	mov    %r14,%rsi
 757:	48 01 ef             	add    %rbp,%rdi
 75a:	e8 00 00 00 00       	call   75f <cs_str_append+0x7f>
 75f:	eb 4a                	jmp    7ab <cs_str_append+0xcb>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_mutateEmmPKcm():
/usr/include/c++/13/bits/basic_string.tcc:332 (discriminator 1)
 761:	48 89 e7             	mov    %rsp,%rdi
/usr/include/c++/13/bits/basic_string.tcc:331 (discriminator 1)
 764:	4c 89 3c 24          	mov    %r15,(%rsp)
/usr/include/c++/13/bits/basic_string.tcc:332 (discriminator 1)
 768:	e8 93 f8 ff ff       	call   0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
 76d:	49 89 c5             	mov    %rax,%r13
/usr/include/c++/13/bits/basic_string.tcc:334
 770:	48 85 ed             	test   %rbp,%rbp
 773:	74 13                	je     788 <cs_str_append+0xa8>
/usr/include/c++/13/bits/basic_string.tcc:335 (discriminator 1)
 775:	48 8b 33             	mov    (%rbx),%rsi
 778:	48 89 ea             	mov    %rbp,%rdx
 77b:	48 89 c7             	mov    %rax,%rdi
 77e:	e8 00 00 00 00       	call   783 <cs_str_append+0xa3>
/usr/include/c++/13/bits/basic_string.tcc:336
 783:	4d 85 e4             	test   %r12,%r12
 786:	74 10                	je     798 <cs_str_append+0xb8>
/usr/include/c++/13/bits/basic_string.tcc:337
 788:	49 8d 7c 2d 00       	lea    0x0(%r13,%rbp,1),%rdi
 78d:	4c 89 e2             	mov    %r12,%rdx
 790:	4c 89 f6             	mov    %r14,%rsi
 793:	e8 00 00 00 00       	call   798 <cs_str_append+0xb8>
/usr/include/c++/13/bits/basic_string.tcc:342
 798:	48 89 df             	mov    %rbx,%rdi
 79b:	e8 00 00 00 00       	call   7a0 <cs_str_append+0xc0>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_capacityEm():
/usr/include/c++/13/bits/basic_string.h:250
 7a0:	48 8b 04 24          	mov    (%rsp),%rax
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7_M_dataEPc():
/usr/include/c++/13/bits/basic_string.h:213
 7a4:	4c 89 2b             	mov    %r13,(%rbx)
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_capacityEm():
/usr/include/c++/13/bits/basic_string.h:250
 7a7:	48 89 43 10          	mov    %rax,0x10(%rbx)
_ZNSt11char_traitsIcE6assignERcRKc():
/usr/include/c++/13/bits/char_traits.h:358
 7ab:	48 8b 03             	mov    (%rbx),%rax
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_lengthEm():
/usr/include/c++/13/bits/basic_string.h:218
 7ae:	4c 89 7b 08          	mov    %r15,0x8(%rbx)
_ZNSt11char_traitsIcE6assignERcRKc():
/usr/include/c++/13/bits/char_traits.h:358
 7b2:	42 c6 04 38 00       	movb   $0x0,(%rax,%r15,1)
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
 7b7:	48 8b 44 24 08       	mov    0x8(%rsp),%rax
 7bc:	64 48 2b 04 25 28 00 	sub    %fs:0x28,%rax
 7c3:	00 00 
 7c5:	74 05                	je     7cc <cs_str_append+0xec>
 7c7:	e8 00 00 00 00       	call   7cc <cs_str_append+0xec>
 7cc:	48 83 c4 18          	add    $0x18,%rsp
 7d0:	5b                   	pop    %rbx
 7d1:	5d                   	pop    %rbp
 7d2:	41 5c                	pop    %r12
 7d4:	41 5d                	pop    %r13
 7d6:	41 5e                	pop    %r14
 7d8:	41 5f                	pop    %r15
 7da:	c3                   	ret

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
