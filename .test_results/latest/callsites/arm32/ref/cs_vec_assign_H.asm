; call site: cs_vec_assign_H   lib=ref   arch=arm32   size=256 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm32/ref.o:     file format elf32-littlearm


Disassembly of section .text:

000003e8 <cs_vec_assign_H>:
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
 3e8:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:213
 3ec:	4288      	cmp	r0, r1
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
 3ee:	4604      	mov	r4, r0
 3f0:	4688      	mov	r8, r1
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:213
 3f2:	d026      	beq.n	442 <cs_vec_assign_H+0x5a>
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:993
 3f4:	e9d1 5a00 	ldrd	r5, sl, [r1]
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:1080
 3f8:	f8d0 9000 	ldr.w	r9, [r0]
 3fc:	6883      	ldr	r3, [r0, #8]
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:993
 3fe:	ebaa 0705 	sub.w	r7, sl, r5
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:1080
 402:	eba3 0309 	sub.w	r3, r3, r9
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:236 (discriminator 1)
 406:	42bb      	cmp	r3, r7
 408:	d22b      	bcs.n	462 <cs_vec_assign_H+0x7a>
cs_vec_assign_H():
/usr/arm-linux-gnueabihf/include/c++/13/bits/new_allocator.h:134 (discriminator 2)
 40a:	4b36      	ldr	r3, [pc, #216]	@ (4e4 <cs_vec_assign_H+0xfc>)
 40c:	429f      	cmp	r7, r3
 40e:	d901      	bls.n	414 <cs_vec_assign_H+0x2c>
/usr/arm-linux-gnueabihf/include/c++/13/bits/new_allocator.h:139
 410:	f7ff fffe 	bl	0 <_ZSt28__throw_bad_array_new_lengthv>
/usr/arm-linux-gnueabihf/include/c++/13/bits/new_allocator.h:151
 414:	4638      	mov	r0, r7
 416:	f7ff fffe 	bl	0 <_Znwj>
 41a:	4606      	mov	r6, r0
_ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1HSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_uninitialized.h:116
 41c:	4680      	mov	r8, r0
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 1)
 41e:	45aa      	cmp	sl, r5
 420:	d111      	bne.n	446 <cs_vec_assign_H+0x5e>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:240
 422:	e9d4 5800 	ldrd	r5, r8, [r4]
_ZNSt12_Destroy_auxILb0EE9__destroyIP1HEEvT_S4_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:162 (discriminator 1)
 426:	45a8      	cmp	r8, r5
 428:	d115      	bne.n	456 <cs_vec_assign_H+0x6e>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:242
 42a:	6820      	ldr	r0, [r4, #0]
_ZNSt12_Vector_baseI1HSaIS0_EE13_M_deallocateEPS0_j():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:389
 42c:	b118      	cbz	r0, 436 <cs_vec_assign_H+0x4e>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:244
 42e:	68a1      	ldr	r1, [r4, #8]
_ZNSt15__new_allocatorI1HE10deallocateEPS0_j():
/usr/arm-linux-gnueabihf/include/c++/13/bits/new_allocator.h:172
 430:	1a09      	subs	r1, r1, r0
 432:	f7ff fffe 	bl	0 <_ZdlPvj>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:245
 436:	6026      	str	r6, [r4, #0]
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:246
 438:	443e      	add	r6, r7
 43a:	60a6      	str	r6, [r4, #8]
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:262
 43c:	6823      	ldr	r3, [r4, #0]
 43e:	443b      	add	r3, r7
 440:	6063      	str	r3, [r4, #4]
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
 442:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
_ZSt10_ConstructI1HJRKS0_EEvPT_DpOT0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 446:	4629      	mov	r1, r5
 448:	4640      	mov	r0, r8
 44a:	3520      	adds	r5, #32
_ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1HSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 44c:	f108 0820 	add.w	r8, r8, #32
_ZSt10_ConstructI1HJRKS0_EEvPT_DpOT0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 450:	f7ff fffe 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
_ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1HSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 454:	e7e3      	b.n	41e <cs_vec_assign_H+0x36>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.h:804
 456:	f105 0008 	add.w	r0, r5, #8
_ZNSt12_Destroy_auxILb0EE9__destroyIP1HEEvT_S4_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:162 (discriminator 2)
 45a:	3520      	adds	r5, #32
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.h:804
 45c:	f7ff fffe 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
_ZNSt12_Destroy_auxILb0EE9__destroyIP1HEEvT_S4_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:162 (discriminator 2)
 460:	e7e1      	b.n	426 <cs_vec_assign_H+0x3e>
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:993
 462:	6846      	ldr	r6, [r0, #4]
 464:	eba6 0609 	sub.w	r6, r6, r9
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:248 (discriminator 1)
 468:	42b7      	cmp	r7, r6
 46a:	d81d      	bhi.n	4a8 <cs_vec_assign_H+0xc0>
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:993
 46c:	117e      	asrs	r6, r7, #5
 46e:	46ca      	mov	sl, r9
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIPK1HPS3_EET0_T_S8_S7_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:386
 470:	46b0      	mov	r8, r6
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:386 (discriminator 1)
 472:	f1b8 0f00 	cmp.w	r8, #0
 476:	dc0d      	bgt.n	494 <cs_vec_assign_H+0xac>
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEC4ERKS2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_iterator.h:1076
 478:	ea26 75e6 	bic.w	r5, r6, r6, asr #31
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_iterator.h:1077
 47c:	f8d4 8004 	ldr.w	r8, [r4, #4]
_ZNSt12_Destroy_auxILb0EE9__destroyIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS4_SaIS4_EEEEEEvT_SA_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:160
 480:	eb09 1545 	add.w	r5, r9, r5, lsl #5
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:162 (discriminator 1)
 484:	45a8      	cmp	r8, r5
 486:	d0d9      	beq.n	43c <cs_vec_assign_H+0x54>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/arm-linux-gnueabihf/include/c++/13/bits/basic_string.h:804
 488:	f105 0008 	add.w	r0, r5, #8
 48c:	3520      	adds	r5, #32
 48e:	f7ff fffe 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEppEv():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_iterator.h:1112
 492:	e7f7      	b.n	484 <cs_vec_assign_H+0x9c>
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIPK1HPS3_EET0_T_S8_S7_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:388
 494:	4629      	mov	r1, r5
 496:	4650      	mov	r0, sl
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:389
 498:	3520      	adds	r5, #32
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:390
 49a:	f10a 0a20 	add.w	sl, sl, #32
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:386 (discriminator 3)
 49e:	f108 38ff 	add.w	r8, r8, #4294967295	@ 0xffffffff
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:388
 4a2:	f7ff ff53 	bl	34c <_ZN1HaSERKS_.isra.0>
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:386 (discriminator 3)
 4a6:	e7e4      	b.n	472 <cs_vec_assign_H+0x8a>
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:386
 4a8:	1176      	asrs	r6, r6, #5
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:386 (discriminator 1)
 4aa:	2e00      	cmp	r6, #0
 4ac:	dc10      	bgt.n	4d0 <cs_vec_assign_H+0xe8>
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:993
 4ae:	e9d4 3600 	ldrd	r3, r6, [r4]
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:257 (discriminator 1)
 4b2:	f8d8 5000 	ldr.w	r5, [r8]
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_vector.h:993
 4b6:	1af3      	subs	r3, r6, r3
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:258
 4b8:	f8d8 8004 	ldr.w	r8, [r8, #4]
/usr/arm-linux-gnueabihf/include/c++/13/bits/vector.tcc:257 (discriminator 1)
 4bc:	441d      	add	r5, r3
_ZSt16__do_uninit_copyIP1HS1_ET0_T_S3_S2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 1)
 4be:	45a8      	cmp	r8, r5
 4c0:	d0bc      	beq.n	43c <cs_vec_assign_H+0x54>
_ZSt10_ConstructI1HJRS0_EEvPT_DpOT0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 4c2:	4629      	mov	r1, r5
 4c4:	4630      	mov	r0, r6
_ZSt16__do_uninit_copyIP1HS1_ET0_T_S3_S2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 4c6:	3520      	adds	r5, #32
 4c8:	3620      	adds	r6, #32
_ZSt10_ConstructI1HJRS0_EEvPT_DpOT0_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 4ca:	f7ff fffe 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
_ZSt16__do_uninit_copyIP1HS1_ET0_T_S3_S2_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 4ce:	e7f6      	b.n	4be <cs_vec_assign_H+0xd6>
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:388
 4d0:	4629      	mov	r1, r5
 4d2:	4648      	mov	r0, r9
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:389
 4d4:	3520      	adds	r5, #32
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:390
 4d6:	f109 0920 	add.w	r9, r9, #32
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:386 (discriminator 3)
 4da:	3e01      	subs	r6, #1
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:388
 4dc:	f7ff ff36 	bl	34c <_ZN1HaSERKS_.isra.0>
/usr/arm-linux-gnueabihf/include/c++/13/bits/stl_algobase.h:386 (discriminator 3)
 4e0:	e7e3      	b.n	4aa <cs_vec_assign_H+0xc2>
 4e2:	bf00      	nop
 4e4:	7fffffe0 	.word	0x7fffffe0

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
