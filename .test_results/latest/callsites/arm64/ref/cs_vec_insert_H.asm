; call site: cs_vec_insert_H   lib=ref   arch=arm64   size=324 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm64/ref.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000438 <cs_vec_insert_H>:
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 438:	d101c3ff 	sub	sp, sp, #0x70
 43c:	90000003 	adrp	x3, 0 <__stack_chk_guard>
 440:	f9400063 	ldr	x3, [x3]
 444:	a9047bfd 	stp	x29, x30, [sp, #64]
 448:	910103fd 	add	x29, sp, #0x40
 44c:	a90553f3 	stp	x19, x20, [sp, #80]
 450:	a9065bf5 	stp	x21, x22, [sp, #96]
 454:	f9400064 	ldr	x4, [x3]
 458:	f9001fe4 	str	x4, [sp, #56]
 45c:	d2800004 	mov	x4, #0x0                   	// #0
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:141
 460:	a9400c14 	ldp	x20, x3, [x0]
 464:	f9400804 	ldr	x4, [x0, #16]
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEl():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_iterator.h:1148
 468:	9100a294 	add	x20, x20, #0x28
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:141
 46c:	eb04007f 	cmp	x3, x4
 470:	54000680 	b.eq	540 <cs_vec_insert_H+0x108>  // b.none
 474:	aa0003f3 	mov	x19, x0
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:147 (discriminator 2)
 478:	eb03029f 	cmp	x20, x3
 47c:	54000241 	b.ne	4c4 <cs_vec_insert_H+0x8c>  // b.any
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 480:	aa1403e0 	mov	x0, x20
 484:	94000000 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:152
 488:	f9400660 	ldr	x0, [x19, #8]
 48c:	9100a000 	add	x0, x0, #0x28
 490:	f9000660 	str	x0, [x19, #8]
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 494:	90000000 	adrp	x0, 0 <__stack_chk_guard>
 498:	f9400000 	ldr	x0, [x0]
 49c:	f9401fe2 	ldr	x2, [sp, #56]
 4a0:	f9400001 	ldr	x1, [x0]
 4a4:	eb010042 	subs	x2, x2, x1
 4a8:	d2800001 	mov	x1, #0x0                   	// #0
 4ac:	540005a1 	b.ne	560 <cs_vec_insert_H+0x128>  // b.any
 4b0:	a9447bfd 	ldp	x29, x30, [sp, #64]
 4b4:	a94553f3 	ldp	x19, x20, [sp, #80]
 4b8:	a9465bf5 	ldp	x21, x22, [sp, #96]
 4bc:	9101c3ff 	add	sp, sp, #0x70
 4c0:	d65f03c0 	ret
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 4c4:	910043f6 	add	x22, sp, #0x10
_ZNSt6vectorI1HSaIS0_EE16_Temporary_valueC4IJRKS0_EEEPS2_DpOT_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:1833
 4c8:	f90007f3 	str	x19, [sp, #8]
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 4cc:	aa1603e0 	mov	x0, x22
_ZNSt6vectorI1HSaIS0_EE16_Temporary_value8_StorageC4Ev():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:1852
 4d0:	390043ff 	strb	wzr, [sp, #16]
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 4d4:	94000000 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
_ZNSt6vectorI1HSaIS0_EE13_M_insert_auxIS0_EEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEOT_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:423
 4d8:	f9400660 	ldr	x0, [x19, #8]
_ZSt12construct_atI1HJS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS2_DpOS3_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 4dc:	d100a001 	sub	x1, x0, #0x28
 4e0:	94000000 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
_ZNSt6vectorI1HSaIS0_EE13_M_insert_auxIS0_EEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEOT_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:425
 4e4:	f9400675 	ldr	x21, [x19, #8]
 4e8:	9100a2a0 	add	x0, x21, #0x28
 4ec:	f9000660 	str	x0, [x19, #8]
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:430 (discriminator 1)
 4f0:	d100a2b3 	sub	x19, x21, #0x28
_ZNSt20__copy_move_backwardILb1ELb0ESt26random_access_iterator_tagE13__copy_move_bIP1HS4_EET0_T_S6_S5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:730
 4f4:	b202e7e0 	mov	x0, #0xcccccccccccccccc    	// #-3689348814741910324
 4f8:	cb140273 	sub	x19, x19, x20
 4fc:	f29999a0 	movk	x0, #0xcccd
 500:	9343fe73 	asr	x19, x19, #3
 504:	9b007e73 	mul	x19, x19, x0
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:731 (discriminator 1)
 508:	f100027f 	cmp	x19, #0x0
 50c:	540000ec 	b.gt	528 <cs_vec_insert_H+0xf0>
_ZNSt6vectorI1HSaIS0_EE13_M_insert_auxIS0_EEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEOT_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:436 (discriminator 1)
 510:	aa1603e1 	mov	x1, x22
 514:	aa1403e0 	mov	x0, x20
 518:	97fffeec 	bl	c8 <_ZN1HaSEOS_.isra.0>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.h:804
 51c:	910063e0 	add	x0, sp, #0x18
 520:	94000000 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
_ZNSt6vectorI1HSaIS0_EE16_Temporary_value8_StorageD4Ev():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:1853
 524:	17ffffdc 	b	494 <cs_vec_insert_H+0x5c>
_ZNSt20__copy_move_backwardILb1ELb0ESt26random_access_iterator_tagE13__copy_move_bIP1HS4_EET0_T_S6_S5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:732
 528:	d10142a1 	sub	x1, x21, #0x50
 52c:	d100a2b5 	sub	x21, x21, #0x28
 530:	aa1503e0 	mov	x0, x21
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:731 (discriminator 2)
 534:	d1000673 	sub	x19, x19, #0x1
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:732
 538:	97fffee4 	bl	c8 <_ZN1HaSEOS_.isra.0>
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:731 (discriminator 2)
 53c:	17fffff3 	b	508 <cs_vec_insert_H+0xd0>
 540:	aa0103e2 	mov	x2, x1
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:170 (discriminator 4)
 544:	90000001 	adrp	x1, 0 <__stack_chk_guard>
 548:	f9400021 	ldr	x1, [x1]
 54c:	f9401fe4 	ldr	x4, [sp, #56]
 550:	f9400023 	ldr	x3, [x1]
 554:	eb030084 	subs	x4, x4, x3
 558:	d2800003 	mov	x3, #0x0                   	// #0
 55c:	54000040 	b.eq	564 <cs_vec_insert_H+0x12c>  // b.none
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 560:	94000000 	bl	0 <__stack_chk_fail>
 564:	a9447bfd 	ldp	x29, x30, [sp, #64]
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:170 (discriminator 4)
 568:	aa1403e1 	mov	x1, x20
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 56c:	a94553f3 	ldp	x19, x20, [sp, #80]
 570:	a9465bf5 	ldp	x21, x22, [sp, #96]
 574:	9101c3ff 	add	sp, sp, #0x70
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:170 (discriminator 4)
 578:	14000000 	b	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>

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
