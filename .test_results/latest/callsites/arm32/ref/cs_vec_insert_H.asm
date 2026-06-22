; call site: cs_vec_insert_H   lib=ref   arch=arm32   size=206 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm32/ref.o:     file format elf32-littlearm


Disassembly of section .text:

0000027e <cs_vec_insert_H>:
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 27e:	b5f0      	push	{r4, r5, r6, r7, lr}
 280:	460a      	mov	r2, r1
 282:	4d2e      	ldr	r5, [pc, #184]	@ (33c <cs_vec_insert_H+0xbe>)
 284:	4b2e      	ldr	r3, [pc, #184]	@ (340 <cs_vec_insert_H+0xc2>)
 286:	b08b      	sub	sp, #44	@ 0x2c
 288:	447d      	add	r5, pc
 28a:	4604      	mov	r4, r0
 28c:	58eb      	ldr	r3, [r5, r3]
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEi():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_iterator.h:1148
 28e:	6805      	ldr	r5, [r0, #0]
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 290:	681b      	ldr	r3, [r3, #0]
 292:	9309      	str	r3, [sp, #36]	@ 0x24
 294:	f04f 0300 	mov.w	r3, #0
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:141
 298:	e9d0 3601 	ldrd	r3, r6, [r0, #4]
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEi():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_iterator.h:1148
 29c:	3520      	adds	r5, #32
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:141
 29e:	42b3      	cmp	r3, r6
 2a0:	d03a      	beq.n	318 <cs_vec_insert_H+0x9a>
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:147 (discriminator 2)
 2a2:	429d      	cmp	r5, r3
 2a4:	d111      	bne.n	2ca <cs_vec_insert_H+0x4c>
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 2a6:	4628      	mov	r0, r5
 2a8:	f7ff fffe 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:152
 2ac:	6863      	ldr	r3, [r4, #4]
 2ae:	3320      	adds	r3, #32
 2b0:	6063      	str	r3, [r4, #4]
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 2b2:	4a24      	ldr	r2, [pc, #144]	@ (344 <cs_vec_insert_H+0xc6>)
 2b4:	4b22      	ldr	r3, [pc, #136]	@ (340 <cs_vec_insert_H+0xc2>)
 2b6:	447a      	add	r2, pc
 2b8:	58d3      	ldr	r3, [r2, r3]
 2ba:	681a      	ldr	r2, [r3, #0]
 2bc:	9b09      	ldr	r3, [sp, #36]	@ 0x24
 2be:	405a      	eors	r2, r3
 2c0:	f04f 0300 	mov.w	r3, #0
 2c4:	d132      	bne.n	32c <cs_vec_insert_H+0xae>
 2c6:	b00b      	add	sp, #44	@ 0x2c
 2c8:	bdf0      	pop	{r4, r5, r6, r7, pc}
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 2ca:	af01      	add	r7, sp, #4
_ZNSt6vectorI1HSaIS0_EE16_Temporary_valueC4IJRKS0_EEEPS2_DpOT_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:1833
 2cc:	9000      	str	r0, [sp, #0]
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 2ce:	4638      	mov	r0, r7
_ZNSt6vectorI1HSaIS0_EE16_Temporary_value8_StorageC4Ev():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:1852
 2d0:	2300      	movs	r3, #0
 2d2:	f88d 3004 	strb.w	r3, [sp, #4]
_ZSt12construct_atI1HJRKS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS4_DpOS5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 2d6:	f7ff fffe 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
_ZNSt6vectorI1HSaIS0_EE13_M_insert_auxIS0_EEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEOT_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:423
 2da:	6860      	ldr	r0, [r4, #4]
_ZSt12construct_atI1HJS0_EEDTgsnwcvPvLi0E_T_pispcl7declvalIT0_EEEEPS2_DpOS3_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:97 (discriminator 1)
 2dc:	f1a0 0120 	sub.w	r1, r0, #32
 2e0:	f7ff fffe 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
_ZNSt6vectorI1HSaIS0_EE13_M_insert_auxIS0_EEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEOT_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:425
 2e4:	6866      	ldr	r6, [r4, #4]
 2e6:	f106 0320 	add.w	r3, r6, #32
 2ea:	6063      	str	r3, [r4, #4]
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:430 (discriminator 1)
 2ec:	f1a6 0420 	sub.w	r4, r6, #32
_ZNSt20__copy_move_backwardILb1ELb0ESt26random_access_iterator_tagE13__copy_move_bIP1HS4_EET0_T_S6_S5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:730
 2f0:	1b64      	subs	r4, r4, r5
 2f2:	1164      	asrs	r4, r4, #5
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:731 (discriminator 1)
 2f4:	2c00      	cmp	r4, #0
 2f6:	dc07      	bgt.n	308 <cs_vec_insert_H+0x8a>
_ZNSt6vectorI1HSaIS0_EE13_M_insert_auxIS0_EEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEOT_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:436 (discriminator 1)
 2f8:	4628      	mov	r0, r5
 2fa:	4639      	mov	r1, r7
 2fc:	f7ff fec3 	bl	86 <_ZN1HaSEOS_.isra.0>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.h:804
 300:	a803      	add	r0, sp, #12
 302:	f7ff fffe 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
_ZNSt6vectorI1HSaIS0_EE16_Temporary_value8_StorageD4Ev():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:1853
 306:	e7d4      	b.n	2b2 <cs_vec_insert_H+0x34>
_ZNSt20__copy_move_backwardILb1ELb0ESt26random_access_iterator_tagE13__copy_move_bIP1HS4_EET0_T_S6_S5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:732
 308:	f1a6 0140 	sub.w	r1, r6, #64	@ 0x40
 30c:	3e20      	subs	r6, #32
 30e:	4630      	mov	r0, r6
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:731 (discriminator 2)
 310:	3c01      	subs	r4, #1
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:732
 312:	f7ff feb8 	bl	86 <_ZN1HaSEOS_.isra.0>
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:731 (discriminator 2)
 316:	e7ed      	b.n	2f4 <cs_vec_insert_H+0x76>
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 318:	490b      	ldr	r1, [pc, #44]	@ (348 <cs_vec_insert_H+0xca>)
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:170 (discriminator 4)
 31a:	4b09      	ldr	r3, [pc, #36]	@ (340 <cs_vec_insert_H+0xc2>)
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 31c:	4479      	add	r1, pc
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:170 (discriminator 4)
 31e:	58cb      	ldr	r3, [r1, r3]
 320:	6819      	ldr	r1, [r3, #0]
 322:	9b09      	ldr	r3, [sp, #36]	@ 0x24
 324:	4059      	eors	r1, r3
 326:	f04f 0300 	mov.w	r3, #0
 32a:	d001      	beq.n	330 <cs_vec_insert_H+0xb2>
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 32c:	f7ff fffe 	bl	0 <__stack_chk_fail>
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:170 (discriminator 4)
 330:	4629      	mov	r1, r5
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 332:	b00b      	add	sp, #44	@ 0x2c
 334:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
_ZNSt6vectorI1HSaIS0_EE6insertEN9__gnu_cxx17__normal_iteratorIPKS0_S2_EERS5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:170 (discriminator 4)
 338:	f7ff bffe 	b.w	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 33c:	000000b0 	.word	0x000000b0
 340:	00000000 	.word	0x00000000
 344:	0000008a 	.word	0x0000008a
 348:	00000028 	.word	0x00000028

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
