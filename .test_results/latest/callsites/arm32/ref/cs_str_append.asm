; call site: cs_str_append   lib=ref   arch=arm32   size=196 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm32/ref.o:     file format elf32-littlearm


Disassembly of section .text:

000004e8 <cs_str_append>:
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
 4e8:	e92d 43f7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
 4ec:	4604      	mov	r4, r0
 4ee:	4a2b      	ldr	r2, [pc, #172]	@ (59c <cs_str_append+0xb4>)
 4f0:	4b2b      	ldr	r3, [pc, #172]	@ (5a0 <cs_str_append+0xb8>)
_ZNSt11char_traitsIcE6lengthEPKc():
/usr/arm-linux-gnueabihf/include/c++/13/bits/char_traits.h:399
 4f2:	4608      	mov	r0, r1
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
 4f4:	447a      	add	r2, pc
 4f6:	460f      	mov	r7, r1
 4f8:	58d3      	ldr	r3, [r2, r3]
 4fa:	681b      	ldr	r3, [r3, #0]
 4fc:	9301      	str	r3, [sp, #4]
 4fe:	f04f 0300 	mov.w	r3, #0
_ZNSt11char_traitsIcE6lengthEPKc():
/usr/arm-linux-gnueabihf/include/c++/13/bits/char_traits.h:399
 502:	f7ff fffe 	bl	0 <strlen>
 506:	6866      	ldr	r6, [r4, #4]
_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE15_M_check_lengthEjjPKc():
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.h:399 (discriminator 2)
 508:	f06f 4300 	mvn.w	r3, #2147483648	@ 0x80000000
_ZNSt11char_traitsIcE6lengthEPKc():
/usr/arm-linux-gnueabihf/include/c++/13/bits/char_traits.h:399
 50c:	4605      	mov	r5, r0
_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE15_M_check_lengthEjjPKc():
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.h:399 (discriminator 2)
 50e:	1b9b      	subs	r3, r3, r6
 510:	4298      	cmp	r0, r3
 512:	d903      	bls.n	51c <cs_str_append+0x34>
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.h:400
 514:	4823      	ldr	r0, [pc, #140]	@ (5a4 <cs_str_append+0xbc>)
 516:	4478      	add	r0, pc
 518:	f7ff fffe 	bl	0 <_ZSt20__throw_length_errorPKc>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_appendEPKcj():
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.tcc:416 (discriminator 1)
 51c:	eb00 0806 	add.w	r8, r0, r6
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.tcc:418
 520:	4620      	mov	r0, r4
 522:	f7ff fffe 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 526:	4601      	mov	r1, r0
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.tcc:418 (discriminator 1)
 528:	4580      	cmp	r8, r0
 52a:	d818      	bhi.n	55e <cs_str_append+0x76>
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.tcc:420
 52c:	b12d      	cbz	r5, 53a <cs_str_append+0x52>
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.tcc:421 (discriminator 2)
 52e:	6820      	ldr	r0, [r4, #0]
 530:	462a      	mov	r2, r5
 532:	4639      	mov	r1, r7
 534:	4430      	add	r0, r6
 536:	f7ff fffe 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
_ZNSt11char_traitsIcE6assignERcRKc():
/usr/arm-linux-gnueabihf/include/c++/13/bits/char_traits.h:358
 53a:	6823      	ldr	r3, [r4, #0]
 53c:	2200      	movs	r2, #0
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_lengthEj():
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.h:218
 53e:	f8c4 8004 	str.w	r8, [r4, #4]
_ZNSt11char_traitsIcE6assignERcRKc():
/usr/arm-linux-gnueabihf/include/c++/13/bits/char_traits.h:358
 542:	f803 2008 	strb.w	r2, [r3, r8]
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
 546:	4a18      	ldr	r2, [pc, #96]	@ (5a8 <cs_str_append+0xc0>)
 548:	4b15      	ldr	r3, [pc, #84]	@ (5a0 <cs_str_append+0xb8>)
 54a:	447a      	add	r2, pc
 54c:	58d3      	ldr	r3, [r2, r3]
 54e:	681a      	ldr	r2, [r3, #0]
 550:	9b01      	ldr	r3, [sp, #4]
 552:	405a      	eors	r2, r3
 554:	f04f 0300 	mov.w	r3, #0
 558:	d01d      	beq.n	596 <cs_str_append+0xae>
 55a:	f7ff fffe 	bl	0 <__stack_chk_fail>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_mutateEjjPKcj():
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.tcc:332 (discriminator 1)
 55e:	4668      	mov	r0, sp
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.tcc:331 (discriminator 1)
 560:	f8cd 8000 	str.w	r8, [sp]
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.tcc:332 (discriminator 1)
 564:	f7ff fd4c 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 568:	4681      	mov	r9, r0
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.tcc:334
 56a:	b96e      	cbnz	r6, 588 <cs_str_append+0xa0>
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.tcc:337
 56c:	462a      	mov	r2, r5
 56e:	4639      	mov	r1, r7
 570:	eb09 0006 	add.w	r0, r9, r6
 574:	f7ff fffe 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.tcc:342
 578:	4620      	mov	r0, r4
 57a:	f7ff fffe 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_capacityEj():
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.h:250
 57e:	9b00      	ldr	r3, [sp, #0]
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7_M_dataEPc():
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.h:213
 580:	f8c4 9000 	str.w	r9, [r4]
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_capacityEj():
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.h:250
 584:	60a3      	str	r3, [r4, #8]
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_mutateEjjPKcj():
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.tcc:345
 586:	e7d8      	b.n	53a <cs_str_append+0x52>
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.tcc:335 (discriminator 1)
 588:	6821      	ldr	r1, [r4, #0]
 58a:	4632      	mov	r2, r6
 58c:	f7ff fffe 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.tcc:336 (discriminator 1)
 590:	2d00      	cmp	r5, #0
 592:	d0f1      	beq.n	578 <cs_str_append+0x90>
 594:	e7ea      	b.n	56c <cs_str_append+0x84>
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
 596:	b003      	add	sp, #12
 598:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 59c:	000000a4 	.word	0x000000a4
 5a0:	00000000 	.word	0x00000000
 5a4:	0000008a 	.word	0x0000008a
 5a8:	0000005a 	.word	0x0000005a

Disassembly of section .text._ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_is_localEv:

Disassembly of section .text._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv:

Disassembly of section .text._ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7compareERKS4_:

Disassembly of section .text._ZNK1HltERKS_:

Disassembly of section .text._ZNSt6vectorIiSaIiEE17_M_realloc_insertIJRKiEEEvN9__gnu_cxx17__normal_iteratorIPiS1_EEDpOT_:

Disassembly of section .text._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7_S_copyEPcPKcj:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZN1HC2EOS_:

Disassembly of section .text._ZSt13__adjust_heapIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEEiS2_NS0_5__ops15_Iter_less_iterEEvT_T0_SB_T1_T2_.isra.0:

Disassembly of section .text._ZSt10__pop_heapIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_SA_RT0_.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE17_M_realloc_insertIJRKS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_:

Disassembly of section .text._ZSt14__partial_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_SA_T0_:

Disassembly of section .text._ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE8capacityEv:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXsrSt6__and_IJSt6__not_ISt15__is_tuple_likeIT_EESt21is_move_constructibleIS5_ESt18is_move_assignableIS5_EEE5valueEvE4typeERS5_SF_:
