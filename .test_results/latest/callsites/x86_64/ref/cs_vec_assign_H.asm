; call site: cs_vec_assign_H   lib=ref   arch=x86_64   size=418 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/ref.o:     file format elf64-x86-64


Disassembly of section .text:

000000000000053e <cs_vec_assign_H>:
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 53e:	f3 0f 1e fa          	endbr64
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:213
 542:	48 39 f7             	cmp    %rsi,%rdi
 545:	0f 84 94 01 00 00    	je     6df <cs_vec_assign_H+0x1a1>
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 54b:	41 57                	push   %r15
 54d:	49 89 f7             	mov    %rsi,%r15
 550:	41 56                	push   %r14
 552:	41 55                	push   %r13
 554:	41 54                	push   %r12
 556:	55                   	push   %rbp
 557:	53                   	push   %rbx
 558:	48 89 fb             	mov    %rdi,%rbx
 55b:	48 83 ec 18          	sub    $0x18,%rsp
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/include/c++/13/bits/stl_vector.h:993
 55f:	48 8b 6e 08          	mov    0x8(%rsi),%rbp
 563:	4c 8b 36             	mov    (%rsi),%r14
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/usr/include/c++/13/bits/stl_vector.h:1080
 566:	4c 8b 2f             	mov    (%rdi),%r13
 569:	48 8b 57 10          	mov    0x10(%rdi),%rdx
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/include/c++/13/bits/stl_vector.h:993
 56d:	49 89 ec             	mov    %rbp,%r12
 570:	4d 29 f4             	sub    %r14,%r12
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/usr/include/c++/13/bits/stl_vector.h:1080
 573:	4c 29 ea             	sub    %r13,%rdx
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:236 (discriminator 1)
 576:	4c 39 e2             	cmp    %r12,%rdx
 579:	73 7a                	jae    5f5 <cs_vec_assign_H+0xb7>
cs_vec_assign_H():
/usr/include/c++/13/bits/new_allocator.h:134 (discriminator 2)
 57b:	48 b8 f8 ff ff ff ff 	movabs $0x7ffffffffffffff8,%rax
 582:	ff ff 7f 
 585:	4c 39 e0             	cmp    %r12,%rax
 588:	73 05                	jae    58f <cs_vec_assign_H+0x51>
/usr/include/c++/13/bits/new_allocator.h:139
 58a:	e8 00 00 00 00       	call   58f <cs_vec_assign_H+0x51>
/usr/include/c++/13/bits/new_allocator.h:151
 58f:	4c 89 e7             	mov    %r12,%rdi
 592:	e8 00 00 00 00       	call   597 <cs_vec_assign_H+0x59>
 597:	49 89 c5             	mov    %rax,%r13
_ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1HSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_():
/usr/include/c++/13/bits/stl_uninitialized.h:116
 59a:	49 89 c7             	mov    %rax,%r15
/usr/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 1)
 59d:	4c 39 f5             	cmp    %r14,%rbp
 5a0:	74 15                	je     5b7 <cs_vec_assign_H+0x79>
_ZSt10_ConstructI1HJRKS0_EEvPT_DpOT0_():
/usr/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 5a2:	4c 89 f6             	mov    %r14,%rsi
 5a5:	4c 89 ff             	mov    %r15,%rdi
 5a8:	49 83 c6 28          	add    $0x28,%r14
_ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1HSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_():
/usr/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 5ac:	49 83 c7 28          	add    $0x28,%r15
_ZSt10_ConstructI1HJRKS0_EEvPT_DpOT0_():
/usr/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 5b0:	e8 00 00 00 00       	call   5b5 <cs_vec_assign_H+0x77>
_ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1HSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_():
/usr/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 5b5:	eb e6                	jmp    59d <cs_vec_assign_H+0x5f>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:240
 5b7:	4c 8b 73 08          	mov    0x8(%rbx),%r14
 5bb:	48 8b 2b             	mov    (%rbx),%rbp
_ZNSt12_Destroy_auxILb0EE9__destroyIP1HEEvT_S4_():
/usr/include/c++/13/bits/stl_construct.h:162 (discriminator 1)
 5be:	49 39 ee             	cmp    %rbp,%r14
 5c1:	74 0f                	je     5d2 <cs_vec_assign_H+0x94>
_ZN1HD4Ev():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:20
 5c3:	48 8d 7d 08          	lea    0x8(%rbp),%rdi
_ZNSt12_Destroy_auxILb0EE9__destroyIP1HEEvT_S4_():
/usr/include/c++/13/bits/stl_construct.h:162 (discriminator 2)
 5c7:	48 83 c5 28          	add    $0x28,%rbp
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/include/c++/13/bits/basic_string.h:804
 5cb:	e8 00 00 00 00       	call   5d0 <cs_vec_assign_H+0x92>
_ZNSt12_Destroy_auxILb0EE9__destroyIP1HEEvT_S4_():
/usr/include/c++/13/bits/stl_construct.h:162 (discriminator 2)
 5d0:	eb ec                	jmp    5be <cs_vec_assign_H+0x80>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:244
 5d2:	48 8b 3b             	mov    (%rbx),%rdi
_ZNSt12_Vector_baseI1HSaIS0_EE13_M_deallocateEPS0_m():
/usr/include/c++/13/bits/stl_vector.h:389
 5d5:	48 85 ff             	test   %rdi,%rdi
 5d8:	74 0c                	je     5e6 <cs_vec_assign_H+0xa8>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:244
 5da:	48 8b 73 10          	mov    0x10(%rbx),%rsi
 5de:	48 29 fe             	sub    %rdi,%rsi
_ZNSt15__new_allocatorI1HE10deallocateEPS0_m():
/usr/include/c++/13/bits/new_allocator.h:172
 5e1:	e8 00 00 00 00       	call   5e6 <cs_vec_assign_H+0xa8>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:245
 5e6:	4c 89 2b             	mov    %r13,(%rbx)
/usr/include/c++/13/bits/vector.tcc:246
 5e9:	4d 01 e5             	add    %r12,%r13
 5ec:	4c 89 6b 10          	mov    %r13,0x10(%rbx)
 5f0:	e9 d4 00 00 00       	jmp    6c9 <cs_vec_assign_H+0x18b>
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/include/c++/13/bits/stl_vector.h:993
 5f5:	48 8b 47 08          	mov    0x8(%rdi),%rax
 5f9:	48 89 c5             	mov    %rax,%rbp
 5fc:	48 89 04 24          	mov    %rax,(%rsp)
 600:	4c 29 ed             	sub    %r13,%rbp
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:248 (discriminator 1)
 603:	4c 39 e5             	cmp    %r12,%rbp
 606:	72 67                	jb     66f <cs_vec_assign_H+0x131>
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/include/c++/13/bits/stl_vector.h:993
 608:	48 ba cd cc cc cc cc 	movabs $0xcccccccccccccccd,%rdx
 60f:	cc cc cc 
 612:	4c 89 e5             	mov    %r12,%rbp
 615:	48 c1 fd 03          	sar    $0x3,%rbp
 619:	48 0f af ea          	imul   %rdx,%rbp
 61d:	4c 89 ea             	mov    %r13,%rdx
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIPK1HPS3_EET0_T_S8_S7_():
/usr/include/c++/13/bits/stl_algobase.h:386
 620:	49 89 ef             	mov    %rbp,%r15
/usr/include/c++/13/bits/stl_algobase.h:386 (discriminator 1)
 623:	4d 85 ff             	test   %r15,%r15
 626:	7e 22                	jle    64a <cs_vec_assign_H+0x10c>
/usr/include/c++/13/bits/stl_algobase.h:388
 628:	4c 89 f6             	mov    %r14,%rsi
 62b:	48 89 d7             	mov    %rdx,%rdi
 62e:	48 89 54 24 08       	mov    %rdx,0x8(%rsp)
/usr/include/c++/13/bits/stl_algobase.h:389
 633:	49 83 c6 28          	add    $0x28,%r14
/usr/include/c++/13/bits/stl_algobase.h:388
 637:	e8 44 fe ff ff       	call   480 <_ZN1HaSERKS_.isra.0>
/usr/include/c++/13/bits/stl_algobase.h:390
 63c:	48 8b 54 24 08       	mov    0x8(%rsp),%rdx
/usr/include/c++/13/bits/stl_algobase.h:386 (discriminator 3)
 641:	49 ff cf             	dec    %r15
/usr/include/c++/13/bits/stl_algobase.h:390
 644:	48 83 c2 28          	add    $0x28,%rdx
/usr/include/c++/13/bits/stl_algobase.h:386 (discriminator 3)
 648:	eb d9                	jmp    623 <cs_vec_assign_H+0xe5>
 64a:	31 d2                	xor    %edx,%edx
 64c:	48 85 ed             	test   %rbp,%rbp
 64f:	48 0f 48 ea          	cmovs  %rdx,%rbp
 653:	48 6b ed 28          	imul   $0x28,%rbp,%rbp
 657:	4c 01 ed             	add    %r13,%rbp
_ZNSt12_Destroy_auxILb0EE9__destroyIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS4_SaIS4_EEEEEEvT_SA_():
/usr/include/c++/13/bits/stl_construct.h:162 (discriminator 1)
 65a:	48 39 2c 24          	cmp    %rbp,(%rsp)
 65e:	74 69                	je     6c9 <cs_vec_assign_H+0x18b>
_ZN1HD4Ev():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:20
 660:	48 8d 7d 08          	lea    0x8(%rbp),%rdi
 664:	48 83 c5 28          	add    $0x28,%rbp
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/include/c++/13/bits/basic_string.h:804
 668:	e8 00 00 00 00       	call   66d <cs_vec_assign_H+0x12f>
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEppEv():
/usr/include/c++/13/bits/stl_iterator.h:1112
 66d:	eb eb                	jmp    65a <cs_vec_assign_H+0x11c>
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/include/c++/13/bits/stl_algobase.h:386
 66f:	48 ba cd cc cc cc cc 	movabs $0xcccccccccccccccd,%rdx
 676:	cc cc cc 
 679:	48 c1 fd 03          	sar    $0x3,%rbp
 67d:	48 0f af ea          	imul   %rdx,%rbp
/usr/include/c++/13/bits/stl_algobase.h:386 (discriminator 1)
 681:	48 85 ed             	test   %rbp,%rbp
 684:	7e 18                	jle    69e <cs_vec_assign_H+0x160>
/usr/include/c++/13/bits/stl_algobase.h:388
 686:	4c 89 f6             	mov    %r14,%rsi
 689:	4c 89 ef             	mov    %r13,%rdi
/usr/include/c++/13/bits/stl_algobase.h:389
 68c:	49 83 c6 28          	add    $0x28,%r14
/usr/include/c++/13/bits/stl_algobase.h:390
 690:	49 83 c5 28          	add    $0x28,%r13
/usr/include/c++/13/bits/stl_algobase.h:388
 694:	e8 e7 fd ff ff       	call   480 <_ZN1HaSERKS_.isra.0>
/usr/include/c++/13/bits/stl_algobase.h:386 (discriminator 3)
 699:	48 ff cd             	dec    %rbp
 69c:	eb e3                	jmp    681 <cs_vec_assign_H+0x143>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:257
 69e:	4c 8b 73 08          	mov    0x8(%rbx),%r14
/usr/include/c++/13/bits/vector.tcc:258
 6a2:	4d 8b 6f 08          	mov    0x8(%r15),%r13
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/include/c++/13/bits/stl_vector.h:993
 6a6:	4c 89 f5             	mov    %r14,%rbp
 6a9:	48 2b 2b             	sub    (%rbx),%rbp
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:257 (discriminator 1)
 6ac:	49 03 2f             	add    (%r15),%rbp
_ZSt16__do_uninit_copyIP1HS1_ET0_T_S3_S2_():
/usr/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 1)
 6af:	49 39 ed             	cmp    %rbp,%r13
 6b2:	74 15                	je     6c9 <cs_vec_assign_H+0x18b>
_ZSt10_ConstructI1HJRS0_EEvPT_DpOT0_():
/usr/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 6b4:	48 89 ee             	mov    %rbp,%rsi
 6b7:	4c 89 f7             	mov    %r14,%rdi
_ZSt16__do_uninit_copyIP1HS1_ET0_T_S3_S2_():
/usr/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 6ba:	48 83 c5 28          	add    $0x28,%rbp
 6be:	49 83 c6 28          	add    $0x28,%r14
_ZSt10_ConstructI1HJRS0_EEvPT_DpOT0_():
/usr/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 6c2:	e8 00 00 00 00       	call   6c7 <cs_vec_assign_H+0x189>
_ZSt16__do_uninit_copyIP1HS1_ET0_T_S3_S2_():
/usr/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 6c7:	eb e6                	jmp    6af <cs_vec_assign_H+0x171>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/include/c++/13/bits/vector.tcc:262
 6c9:	4c 03 23             	add    (%rbx),%r12
 6cc:	4c 89 63 08          	mov    %r12,0x8(%rbx)
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 6d0:	48 83 c4 18          	add    $0x18,%rsp
 6d4:	5b                   	pop    %rbx
 6d5:	5d                   	pop    %rbp
 6d6:	41 5c                	pop    %r12
 6d8:	41 5d                	pop    %r13
 6da:	41 5e                	pop    %r14
 6dc:	41 5f                	pop    %r15
 6de:	c3                   	ret
 6df:	c3                   	ret

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
