; call site: cs_str_append   lib=ref   arch=x86_64   size=251 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/ref.o:     file format elf64-x86-64


Disassembly of section .text:

00000000000006ac <cs_str_append>:
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
 6ac:	f3 0f 1e fa          	endbr64
 6b0:	41 57                	push   %r15
 6b2:	41 56                	push   %r14
 6b4:	49 89 f6             	mov    %rsi,%r14
 6b7:	41 55                	push   %r13
 6b9:	41 54                	push   %r12
 6bb:	55                   	push   %rbp
 6bc:	53                   	push   %rbx
 6bd:	48 89 fb             	mov    %rdi,%rbx
_ZNSt11char_traitsIcE6lengthEPKc():
/usr/include/c++/13/bits/char_traits.h:399
 6c0:	48 89 f7             	mov    %rsi,%rdi
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
 6c3:	48 83 ec 18          	sub    $0x18,%rsp
 6c7:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
 6ce:	00 00 
 6d0:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
 6d5:	31 c0                	xor    %eax,%eax
_ZNSt11char_traitsIcE6lengthEPKc():
/usr/include/c++/13/bits/char_traits.h:399
 6d7:	e8 00 00 00 00       	call   6dc <cs_str_append+0x30>
 6dc:	48 8b 6b 08          	mov    0x8(%rbx),%rbp
 6e0:	49 89 c4             	mov    %rax,%r12
_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE15_M_check_lengthEmmPKc():
/usr/include/c++/13/bits/basic_string.h:399 (discriminator 2)
 6e3:	48 b8 ff ff ff ff ff 	movabs $0x7fffffffffffffff,%rax
 6ea:	ff ff 7f 
 6ed:	48 29 e8             	sub    %rbp,%rax
 6f0:	4c 39 e0             	cmp    %r12,%rax
 6f3:	73 0c                	jae    701 <cs_str_append+0x55>
/usr/include/c++/13/bits/basic_string.h:400
 6f5:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 6fc <cs_str_append+0x50>
 6fc:	e8 00 00 00 00       	call   701 <cs_str_append+0x55>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_appendEPKcm():
/usr/include/c++/13/bits/basic_string.tcc:418
 701:	48 89 df             	mov    %rbx,%rdi
/usr/include/c++/13/bits/basic_string.tcc:416 (discriminator 1)
 704:	4d 8d 3c 2c          	lea    (%r12,%rbp,1),%r15
/usr/include/c++/13/bits/basic_string.tcc:418
 708:	e8 00 00 00 00       	call   70d <cs_str_append+0x61>
 70d:	48 89 c6             	mov    %rax,%rsi
/usr/include/c++/13/bits/basic_string.tcc:418 (discriminator 1)
 710:	4c 39 f8             	cmp    %r15,%rax
 713:	72 18                	jb     72d <cs_str_append+0x81>
/usr/include/c++/13/bits/basic_string.tcc:420
 715:	4d 85 e4             	test   %r12,%r12
 718:	74 5d                	je     777 <cs_str_append+0xcb>
/usr/include/c++/13/bits/basic_string.tcc:421 (discriminator 2)
 71a:	48 8b 3b             	mov    (%rbx),%rdi
 71d:	4c 89 e2             	mov    %r12,%rdx
 720:	4c 89 f6             	mov    %r14,%rsi
 723:	48 01 ef             	add    %rbp,%rdi
 726:	e8 00 00 00 00       	call   72b <cs_str_append+0x7f>
 72b:	eb 4a                	jmp    777 <cs_str_append+0xcb>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_mutateEmmPKcm():
/usr/include/c++/13/bits/basic_string.tcc:332 (discriminator 1)
 72d:	48 89 e7             	mov    %rsp,%rdi
/usr/include/c++/13/bits/basic_string.tcc:331 (discriminator 1)
 730:	4c 89 3c 24          	mov    %r15,(%rsp)
/usr/include/c++/13/bits/basic_string.tcc:332 (discriminator 1)
 734:	e8 c7 f8 ff ff       	call   0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
 739:	49 89 c5             	mov    %rax,%r13
/usr/include/c++/13/bits/basic_string.tcc:334
 73c:	48 85 ed             	test   %rbp,%rbp
 73f:	74 13                	je     754 <cs_str_append+0xa8>
/usr/include/c++/13/bits/basic_string.tcc:335 (discriminator 1)
 741:	48 8b 33             	mov    (%rbx),%rsi
 744:	48 89 ea             	mov    %rbp,%rdx
 747:	48 89 c7             	mov    %rax,%rdi
 74a:	e8 00 00 00 00       	call   74f <cs_str_append+0xa3>
/usr/include/c++/13/bits/basic_string.tcc:336
 74f:	4d 85 e4             	test   %r12,%r12
 752:	74 10                	je     764 <cs_str_append+0xb8>
/usr/include/c++/13/bits/basic_string.tcc:337
 754:	49 8d 7c 2d 00       	lea    0x0(%r13,%rbp,1),%rdi
 759:	4c 89 e2             	mov    %r12,%rdx
 75c:	4c 89 f6             	mov    %r14,%rsi
 75f:	e8 00 00 00 00       	call   764 <cs_str_append+0xb8>
/usr/include/c++/13/bits/basic_string.tcc:342
 764:	48 89 df             	mov    %rbx,%rdi
 767:	e8 00 00 00 00       	call   76c <cs_str_append+0xc0>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_capacityEm():
/usr/include/c++/13/bits/basic_string.h:250
 76c:	48 8b 04 24          	mov    (%rsp),%rax
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7_M_dataEPc():
/usr/include/c++/13/bits/basic_string.h:213
 770:	4c 89 2b             	mov    %r13,(%rbx)
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_capacityEm():
/usr/include/c++/13/bits/basic_string.h:250
 773:	48 89 43 10          	mov    %rax,0x10(%rbx)
_ZNSt11char_traitsIcE6assignERcRKc():
/usr/include/c++/13/bits/char_traits.h:358
 777:	48 8b 03             	mov    (%rbx),%rax
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_lengthEm():
/usr/include/c++/13/bits/basic_string.h:218
 77a:	4c 89 7b 08          	mov    %r15,0x8(%rbx)
_ZNSt11char_traitsIcE6assignERcRKc():
/usr/include/c++/13/bits/char_traits.h:358
 77e:	42 c6 04 38 00       	movb   $0x0,(%rax,%r15,1)
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
 783:	48 8b 44 24 08       	mov    0x8(%rsp),%rax
 788:	64 48 2b 04 25 28 00 	sub    %fs:0x28,%rax
 78f:	00 00 
 791:	74 05                	je     798 <cs_str_append+0xec>
 793:	e8 00 00 00 00       	call   798 <cs_str_append+0xec>
 798:	48 83 c4 18          	add    $0x18,%rsp
 79c:	5b                   	pop    %rbx
 79d:	5d                   	pop    %rbp
 79e:	41 5c                	pop    %r12
 7a0:	41 5d                	pop    %r13
 7a2:	41 5e                	pop    %r14
 7a4:	41 5f                	pop    %r15
 7a6:	c3                   	ret

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
