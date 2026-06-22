; call site: cs_vec_assign_H   lib=ref   arch=x86_64   size=418 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/ref.o:     file format elf64-x86-64


Disassembly of section .text:

000000000000050a <cs_vec_assign_H>:
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 50a:	f3 0f 1e fa          	endbr64
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:213
 50e:	48 39 f7             	cmp    %rsi,%rdi
 511:	0f 84 94 01 00 00    	je     6ab <cs_vec_assign_H+0x1a1>
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 517:	41 57                	push   %r15
 519:	49 89 f7             	mov    %rsi,%r15
 51c:	41 56                	push   %r14
 51e:	41 55                	push   %r13
 520:	41 54                	push   %r12
 522:	55                   	push   %rbp
 523:	53                   	push   %rbx
 524:	48 89 fb             	mov    %rdi,%rbx
 527:	48 83 ec 18          	sub    $0x18,%rsp
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/include/c++/13/bits/stl_vector.h:993
 52b:	48 8b 6e 08          	mov    0x8(%rsi),%rbp
 52f:	4c 8b 36             	mov    (%rsi),%r14
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/usr/include/c++/13/bits/stl_vector.h:1080
 532:	4c 8b 2f             	mov    (%rdi),%r13
 535:	48 8b 57 10          	mov    0x10(%rdi),%rdx
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/include/c++/13/bits/stl_vector.h:993
 539:	49 89 ec             	mov    %rbp,%r12
 53c:	4d 29 f4             	sub    %r14,%r12
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/usr/include/c++/13/bits/stl_vector.h:1080
 53f:	4c 29 ea             	sub    %r13,%rdx
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:236 (discriminator 1)
 542:	4c 39 e2             	cmp    %r12,%rdx
 545:	73 7a                	jae    5c1 <cs_vec_assign_H+0xb7>
cs_vec_assign_H():
/usr/include/c++/13/bits/new_allocator.h:134 (discriminator 2)
 547:	48 b8 f8 ff ff ff ff 	movabs $0x7ffffffffffffff8,%rax
 54e:	ff ff 7f 
 551:	4c 39 e0             	cmp    %r12,%rax
 554:	73 05                	jae    55b <cs_vec_assign_H+0x51>
/usr/include/c++/13/bits/new_allocator.h:139
 556:	e8 00 00 00 00       	call   55b <cs_vec_assign_H+0x51>
/usr/include/c++/13/bits/new_allocator.h:151
 55b:	4c 89 e7             	mov    %r12,%rdi
 55e:	e8 00 00 00 00       	call   563 <cs_vec_assign_H+0x59>
 563:	49 89 c5             	mov    %rax,%r13
_ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1HSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_():
/usr/include/c++/13/bits/stl_uninitialized.h:116
 566:	49 89 c7             	mov    %rax,%r15
/usr/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 1)
 569:	4c 39 f5             	cmp    %r14,%rbp
 56c:	74 15                	je     583 <cs_vec_assign_H+0x79>
_ZSt10_ConstructI1HJRKS0_EEvPT_DpOT0_():
/usr/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 56e:	4c 89 f6             	mov    %r14,%rsi
 571:	4c 89 ff             	mov    %r15,%rdi
 574:	49 83 c6 28          	add    $0x28,%r14
_ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1HSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_():
/usr/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 578:	49 83 c7 28          	add    $0x28,%r15
_ZSt10_ConstructI1HJRKS0_EEvPT_DpOT0_():
/usr/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 57c:	e8 00 00 00 00       	call   581 <cs_vec_assign_H+0x77>
_ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1HSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_():
/usr/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 581:	eb e6                	jmp    569 <cs_vec_assign_H+0x5f>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:240
 583:	4c 8b 73 08          	mov    0x8(%rbx),%r14
 587:	48 8b 2b             	mov    (%rbx),%rbp
_ZNSt12_Destroy_auxILb0EE9__destroyIP1HEEvT_S4_():
/usr/include/c++/13/bits/stl_construct.h:162 (discriminator 1)
 58a:	49 39 ee             	cmp    %rbp,%r14
 58d:	74 0f                	je     59e <cs_vec_assign_H+0x94>
_ZN1HD4Ev():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:20
 58f:	48 8d 7d 08          	lea    0x8(%rbp),%rdi
_ZNSt12_Destroy_auxILb0EE9__destroyIP1HEEvT_S4_():
/usr/include/c++/13/bits/stl_construct.h:162 (discriminator 2)
 593:	48 83 c5 28          	add    $0x28,%rbp
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/include/c++/13/bits/basic_string.h:804
 597:	e8 00 00 00 00       	call   59c <cs_vec_assign_H+0x92>
_ZNSt12_Destroy_auxILb0EE9__destroyIP1HEEvT_S4_():
/usr/include/c++/13/bits/stl_construct.h:162 (discriminator 2)
 59c:	eb ec                	jmp    58a <cs_vec_assign_H+0x80>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:244
 59e:	48 8b 3b             	mov    (%rbx),%rdi
_ZNSt12_Vector_baseI1HSaIS0_EE13_M_deallocateEPS0_m():
/usr/include/c++/13/bits/stl_vector.h:389
 5a1:	48 85 ff             	test   %rdi,%rdi
 5a4:	74 0c                	je     5b2 <cs_vec_assign_H+0xa8>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:244
 5a6:	48 8b 73 10          	mov    0x10(%rbx),%rsi
 5aa:	48 29 fe             	sub    %rdi,%rsi
_ZNSt15__new_allocatorI1HE10deallocateEPS0_m():
/usr/include/c++/13/bits/new_allocator.h:172
 5ad:	e8 00 00 00 00       	call   5b2 <cs_vec_assign_H+0xa8>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:245
 5b2:	4c 89 2b             	mov    %r13,(%rbx)
/usr/include/c++/13/bits/vector.tcc:246
 5b5:	4d 01 e5             	add    %r12,%r13
 5b8:	4c 89 6b 10          	mov    %r13,0x10(%rbx)
 5bc:	e9 d4 00 00 00       	jmp    695 <cs_vec_assign_H+0x18b>
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/include/c++/13/bits/stl_vector.h:993
 5c1:	48 8b 47 08          	mov    0x8(%rdi),%rax
 5c5:	48 89 c5             	mov    %rax,%rbp
 5c8:	48 89 04 24          	mov    %rax,(%rsp)
 5cc:	4c 29 ed             	sub    %r13,%rbp
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:248 (discriminator 1)
 5cf:	4c 39 e5             	cmp    %r12,%rbp
 5d2:	72 67                	jb     63b <cs_vec_assign_H+0x131>
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/include/c++/13/bits/stl_vector.h:993
 5d4:	48 ba cd cc cc cc cc 	movabs $0xcccccccccccccccd,%rdx
 5db:	cc cc cc 
 5de:	4c 89 e5             	mov    %r12,%rbp
 5e1:	48 c1 fd 03          	sar    $0x3,%rbp
 5e5:	48 0f af ea          	imul   %rdx,%rbp
 5e9:	4c 89 ea             	mov    %r13,%rdx
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIPK1HPS3_EET0_T_S8_S7_():
/usr/include/c++/13/bits/stl_algobase.h:386
 5ec:	49 89 ef             	mov    %rbp,%r15
/usr/include/c++/13/bits/stl_algobase.h:386 (discriminator 1)
 5ef:	4d 85 ff             	test   %r15,%r15
 5f2:	7e 22                	jle    616 <cs_vec_assign_H+0x10c>
/usr/include/c++/13/bits/stl_algobase.h:388
 5f4:	4c 89 f6             	mov    %r14,%rsi
 5f7:	48 89 d7             	mov    %rdx,%rdi
 5fa:	48 89 54 24 08       	mov    %rdx,0x8(%rsp)
/usr/include/c++/13/bits/stl_algobase.h:389
 5ff:	49 83 c6 28          	add    $0x28,%r14
/usr/include/c++/13/bits/stl_algobase.h:388
 603:	e8 44 fe ff ff       	call   44c <_ZN1HaSERKS_.isra.0>
/usr/include/c++/13/bits/stl_algobase.h:390
 608:	48 8b 54 24 08       	mov    0x8(%rsp),%rdx
/usr/include/c++/13/bits/stl_algobase.h:386 (discriminator 3)
 60d:	49 ff cf             	dec    %r15
/usr/include/c++/13/bits/stl_algobase.h:390
 610:	48 83 c2 28          	add    $0x28,%rdx
/usr/include/c++/13/bits/stl_algobase.h:386 (discriminator 3)
 614:	eb d9                	jmp    5ef <cs_vec_assign_H+0xe5>
 616:	31 d2                	xor    %edx,%edx
 618:	48 85 ed             	test   %rbp,%rbp
 61b:	48 0f 48 ea          	cmovs  %rdx,%rbp
 61f:	48 6b ed 28          	imul   $0x28,%rbp,%rbp
 623:	4c 01 ed             	add    %r13,%rbp
_ZNSt12_Destroy_auxILb0EE9__destroyIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS4_SaIS4_EEEEEEvT_SA_():
/usr/include/c++/13/bits/stl_construct.h:162 (discriminator 1)
 626:	48 39 2c 24          	cmp    %rbp,(%rsp)
 62a:	74 69                	je     695 <cs_vec_assign_H+0x18b>
_ZN1HD4Ev():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:20
 62c:	48 8d 7d 08          	lea    0x8(%rbp),%rdi
 630:	48 83 c5 28          	add    $0x28,%rbp
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/include/c++/13/bits/basic_string.h:804
 634:	e8 00 00 00 00       	call   639 <cs_vec_assign_H+0x12f>
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEppEv():
/usr/include/c++/13/bits/stl_iterator.h:1112
 639:	eb eb                	jmp    626 <cs_vec_assign_H+0x11c>
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/include/c++/13/bits/stl_algobase.h:386
 63b:	48 ba cd cc cc cc cc 	movabs $0xcccccccccccccccd,%rdx
 642:	cc cc cc 
 645:	48 c1 fd 03          	sar    $0x3,%rbp
 649:	48 0f af ea          	imul   %rdx,%rbp
/usr/include/c++/13/bits/stl_algobase.h:386 (discriminator 1)
 64d:	48 85 ed             	test   %rbp,%rbp
 650:	7e 18                	jle    66a <cs_vec_assign_H+0x160>
/usr/include/c++/13/bits/stl_algobase.h:388
 652:	4c 89 f6             	mov    %r14,%rsi
 655:	4c 89 ef             	mov    %r13,%rdi
/usr/include/c++/13/bits/stl_algobase.h:389
 658:	49 83 c6 28          	add    $0x28,%r14
/usr/include/c++/13/bits/stl_algobase.h:390
 65c:	49 83 c5 28          	add    $0x28,%r13
/usr/include/c++/13/bits/stl_algobase.h:388
 660:	e8 e7 fd ff ff       	call   44c <_ZN1HaSERKS_.isra.0>
/usr/include/c++/13/bits/stl_algobase.h:386 (discriminator 3)
 665:	48 ff cd             	dec    %rbp
 668:	eb e3                	jmp    64d <cs_vec_assign_H+0x143>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:257
 66a:	4c 8b 73 08          	mov    0x8(%rbx),%r14
/usr/include/c++/13/bits/vector.tcc:258
 66e:	4d 8b 6f 08          	mov    0x8(%r15),%r13
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/include/c++/13/bits/stl_vector.h:993
 672:	4c 89 f5             	mov    %r14,%rbp
 675:	48 2b 2b             	sub    (%rbx),%rbp
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:257 (discriminator 1)
 678:	49 03 2f             	add    (%r15),%rbp
_ZSt16__do_uninit_copyIP1HS1_ET0_T_S3_S2_():
/usr/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 1)
 67b:	49 39 ed             	cmp    %rbp,%r13
 67e:	74 15                	je     695 <cs_vec_assign_H+0x18b>
_ZSt10_ConstructI1HJRS0_EEvPT_DpOT0_():
/usr/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 680:	48 89 ee             	mov    %rbp,%rsi
 683:	4c 89 f7             	mov    %r14,%rdi
_ZSt16__do_uninit_copyIP1HS1_ET0_T_S3_S2_():
/usr/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 686:	48 83 c5 28          	add    $0x28,%rbp
 68a:	49 83 c6 28          	add    $0x28,%r14
_ZSt10_ConstructI1HJRS0_EEvPT_DpOT0_():
/usr/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 68e:	e8 00 00 00 00       	call   693 <cs_vec_assign_H+0x189>
_ZSt16__do_uninit_copyIP1HS1_ET0_T_S3_S2_():
/usr/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 693:	eb e6                	jmp    67b <cs_vec_assign_H+0x171>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:262
 695:	4c 03 23             	add    (%rbx),%r12
 698:	4c 89 63 08          	mov    %r12,0x8(%rbx)
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 69c:	48 83 c4 18          	add    $0x18,%rsp
 6a0:	5b                   	pop    %rbx
 6a1:	5d                   	pop    %rbp
 6a2:	41 5c                	pop    %r12
 6a4:	41 5d                	pop    %r13
 6a6:	41 5e                	pop    %r14
 6a8:	41 5f                	pop    %r15
 6aa:	c3                   	ret
 6ab:	c3                   	ret

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
