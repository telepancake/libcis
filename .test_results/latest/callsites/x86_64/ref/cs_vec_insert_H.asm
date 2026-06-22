; call site: cs_vec_insert_H   lib=ref   arch=x86_64   size=266 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/x86_64/ref.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000376 <cs_vec_insert_H>:
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 376:	f3 0f 1e fa          	endbr64
 37a:	41 55                	push   %r13
 37c:	41 54                	push   %r12
 37e:	55                   	push   %rbp
 37f:	53                   	push   %rbx
 380:	48 89 fb             	mov    %rdi,%rbx
 383:	48 83 ec 48          	sub    $0x48,%rsp
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEl():
/usr/include/c++/13/bits/stl_iterator.h:1148
 387:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
 38e:	00 00 
 390:	48 89 44 24 38       	mov    %rax,0x38(%rsp)
 395:	48 8b 07             	mov    (%rdi),%rax
 398:	48 8d 68 28          	lea    0x28(%rax),%rbp
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/include/c++/13/bits/vector.tcc:141
 39c:	48 8b 47 08          	mov    0x8(%rdi),%rax
 3a0:	48 3b 47 10          	cmp    0x10(%rdi),%rax
 3a4:	0f 84 91 00 00 00    	je     43b <cs_vec_insert_H+0xc5>
/usr/include/c++/13/bits/vector.tcc:147 (discriminator 2)
 3aa:	48 39 c5             	cmp    %rax,%rbp
 3ad:	75 12                	jne    3c1 <cs_vec_insert_H+0x4b>
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 3af:	48 89 ef             	mov    %rbp,%rdi
 3b2:	e8 00 00 00 00       	call   3b7 <cs_vec_insert_H+0x41>
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/include/c++/13/bits/vector.tcc:152
 3b7:	48 83 43 08 28       	addq   $0x28,0x8(%rbx)
 3bc:	e9 9f 00 00 00       	jmp    460 <cs_vec_insert_H+0xea>
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 3c1:	4c 8d 64 24 10       	lea    0x10(%rsp),%r12
_ZNSt6vectorI1HSaIS0_EE16_Temporary_valueC4IJRKS0_EEEPS2_DpOT_():
/usr/include/c++/13/bits/stl_vector.h:1833
 3c6:	48 89 7c 24 08       	mov    %rdi,0x8(%rsp)
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 3cb:	4c 89 e7             	mov    %r12,%rdi
_ZNSt6vectorI1HSaIS0_EE16_Temporary_value8_StorageC4Ev():
/usr/include/c++/13/bits/stl_vector.h:1852
 3ce:	c6 44 24 10 00       	movb   $0x0,0x10(%rsp)
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 3d3:	e8 00 00 00 00       	call   3d8 <cs_vec_insert_H+0x62>
_ZNSt6vectorI1HSaIS0_EE13_M_insert_auxIS0_EEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEOT_():
/usr/include/c++/13/bits/vector.tcc:424
 3d8:	48 8b 7b 08          	mov    0x8(%rbx),%rdi
 3dc:	48 8d 77 d8          	lea    -0x28(%rdi),%rsi
_ZSt12construct_atI1HJS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS2_DpOS3_():
/usr/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 3e0:	e8 00 00 00 00       	call   3e5 <cs_vec_insert_H+0x6f>
_ZNSt6vectorI1HSaIS0_EE13_M_insert_auxIS0_EEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEOT_():
/usr/include/c++/13/bits/vector.tcc:425
 3e5:	4c 8b 6b 08          	mov    0x8(%rbx),%r13
 3e9:	49 8d 45 28          	lea    0x28(%r13),%rax
 3ed:	48 89 43 08          	mov    %rax,0x8(%rbx)
/usr/include/c++/13/bits/vector.tcc:430
 3f1:	49 8d 5d d8          	lea    -0x28(%r13),%rbx
_ZNSt20__copy_move_backwardILb1ELb0ESt26random_access_iterator_tagE13__copy_move_bIP1HS4_EET0_T_S6_S5_():
/usr/include/c++/13/bits/stl_algobase.h:730
 3f5:	48 b8 cd cc cc cc cc 	movabs $0xcccccccccccccccd,%rax
 3fc:	cc cc cc 
 3ff:	48 29 eb             	sub    %rbp,%rbx
 402:	48 c1 fb 03          	sar    $0x3,%rbx
 406:	48 0f af d8          	imul   %rax,%rbx
/usr/include/c++/13/bits/stl_algobase.h:731 (discriminator 1)
 40a:	48 85 db             	test   %rbx,%rbx
 40d:	7e 15                	jle    424 <cs_vec_insert_H+0xae>
/usr/include/c++/13/bits/stl_algobase.h:732
 40f:	49 8d 75 b0          	lea    -0x50(%r13),%rsi
 413:	49 83 ed 28          	sub    $0x28,%r13
/usr/include/c++/13/bits/stl_algobase.h:731 (discriminator 2)
 417:	48 ff cb             	dec    %rbx
/usr/include/c++/13/bits/stl_algobase.h:732
 41a:	4c 89 ef             	mov    %r13,%rdi
 41d:	e8 9e fc ff ff       	call   c0 <_ZN1HaSEOS_.isra.0>
/usr/include/c++/13/bits/stl_algobase.h:731 (discriminator 2)
 422:	eb e6                	jmp    40a <cs_vec_insert_H+0x94>
_ZNSt6vectorI1HSaIS0_EE13_M_insert_auxIS0_EEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEOT_():
/usr/include/c++/13/bits/vector.tcc:436 (discriminator 1)
 424:	48 89 ef             	mov    %rbp,%rdi
 427:	4c 89 e6             	mov    %r12,%rsi
 42a:	e8 91 fc ff ff       	call   c0 <_ZN1HaSEOS_.isra.0>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/include/c++/13/bits/basic_string.h:804
 42f:	48 8d 7c 24 18       	lea    0x18(%rsp),%rdi
 434:	e8 00 00 00 00       	call   439 <cs_vec_insert_H+0xc3>
_ZNSt6vectorI1HSaIS0_EE16_Temporary_value8_StorageD4Ev():
/usr/include/c++/13/bits/stl_vector.h:1853
 439:	eb 25                	jmp    460 <cs_vec_insert_H+0xea>
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/include/c++/13/bits/vector.tcc:170 (discriminator 4)
 43b:	48 8b 44 24 38       	mov    0x38(%rsp),%rax
 440:	64 48 2b 04 25 28 00 	sub    %fs:0x28,%rax
 447:	00 00 
 449:	75 25                	jne    470 <cs_vec_insert_H+0xfa>
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 44b:	48 83 c4 48          	add    $0x48,%rsp
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/include/c++/13/bits/vector.tcc:170 (discriminator 4)
 44f:	48 89 f2             	mov    %rsi,%rdx
 452:	48 89 ee             	mov    %rbp,%rsi
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 455:	5b                   	pop    %rbx
 456:	5d                   	pop    %rbp
 457:	41 5c                	pop    %r12
 459:	41 5d                	pop    %r13
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/include/c++/13/bits/vector.tcc:170 (discriminator 4)
 45b:	e9 00 00 00 00       	jmp    460 <cs_vec_insert_H+0xea>
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 460:	48 8b 44 24 38       	mov    0x38(%rsp),%rax
 465:	64 48 2b 04 25 28 00 	sub    %fs:0x28,%rax
 46c:	00 00 
 46e:	74 05                	je     475 <cs_vec_insert_H+0xff>
 470:	e8 00 00 00 00       	call   475 <cs_vec_insert_H+0xff>
 475:	48 83 c4 48          	add    $0x48,%rsp
 479:	5b                   	pop    %rbx
 47a:	5d                   	pop    %rbp
 47b:	41 5c                	pop    %r12
 47d:	41 5d                	pop    %r13
 47f:	c3                   	ret

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
