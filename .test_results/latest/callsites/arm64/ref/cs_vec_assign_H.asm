; call site: cs_vec_assign_H   lib=ref   arch=arm64   size=448 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm64/ref.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000670 <cs_vec_assign_H>:
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:213
 670:	eb01001f 	cmp	x0, x1
 674:	54000dc0 	b.eq	82c <cs_vec_assign_H+0x1bc>  // b.none
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
 678:	a9bb7bfd 	stp	x29, x30, [sp, #-80]!
 67c:	910003fd 	mov	x29, sp
 680:	a90153f3 	stp	x19, x20, [sp, #16]
 684:	aa0003f3 	mov	x19, x0
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:993
 688:	f9400034 	ldr	x20, [x1]
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
 68c:	a90363f7 	stp	x23, x24, [sp, #48]
 690:	aa0103f7 	mov	x23, x1
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:1080
 694:	f9400018 	ldr	x24, [x0]
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
 698:	a9025bf5 	stp	x21, x22, [sp, #32]
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:1080
 69c:	f9400800 	ldr	x0, [x0, #16]
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
 6a0:	f90023f9 	str	x25, [sp, #64]
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:993
 6a4:	f9400439 	ldr	x25, [x1, #8]
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:1080
 6a8:	cb180000 	sub	x0, x0, x24
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:993
 6ac:	cb140336 	sub	x22, x25, x20
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:236 (discriminator 1)
 6b0:	eb16001f 	cmp	x0, x22
 6b4:	54000522 	b.cs	758 <cs_vec_assign_H+0xe8>  // b.hs, b.nlast
cs_vec_assign_H():
/usr/aarch64-linux-gnu/include/c++/13/bits/new_allocator.h:134 (discriminator 2)
 6b8:	b27defe0 	mov	x0, #0x7ffffffffffffff8    	// #9223372036854775800
 6bc:	eb0002df 	cmp	x22, x0
 6c0:	54000049 	b.ls	6c8 <cs_vec_assign_H+0x58>  // b.plast
/usr/aarch64-linux-gnu/include/c++/13/bits/new_allocator.h:139
 6c4:	94000000 	bl	0 <_ZSt28__throw_bad_array_new_lengthv>
/usr/aarch64-linux-gnu/include/c++/13/bits/new_allocator.h:151
 6c8:	aa1603e0 	mov	x0, x22
 6cc:	94000000 	bl	0 <_Znwm>
 6d0:	aa0003f5 	mov	x21, x0
_ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1HSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_uninitialized.h:116
 6d4:	aa0003f7 	mov	x23, x0
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 1)
 6d8:	eb14033f 	cmp	x25, x20
 6dc:	540002a1 	b.ne	730 <cs_vec_assign_H+0xc0>  // b.any
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:240
 6e0:	a9405e74 	ldp	x20, x23, [x19]
_ZNSt12_Destroy_auxILb0EE9__destroyIP1HEEvT_S4_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:162 (discriminator 1)
 6e4:	eb1402ff 	cmp	x23, x20
 6e8:	54000301 	b.ne	748 <cs_vec_assign_H+0xd8>  // b.any
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:242
 6ec:	f9400260 	ldr	x0, [x19]
_ZNSt12_Vector_baseI1HSaIS0_EE13_M_deallocateEPS0_m():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:389
 6f0:	b4000080 	cbz	x0, 700 <cs_vec_assign_H+0x90>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:244
 6f4:	f9400a61 	ldr	x1, [x19, #16]
_ZNSt15__new_allocatorI1HE10deallocateEPS0_m():
/usr/aarch64-linux-gnu/include/c++/13/bits/new_allocator.h:172
 6f8:	cb000021 	sub	x1, x1, x0
 6fc:	94000000 	bl	0 <_ZdlPvm>
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:245
 700:	f9000275 	str	x21, [x19]
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:246
 704:	8b1602b5 	add	x21, x21, x22
 708:	f9000a75 	str	x21, [x19, #16]
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:262
 70c:	f9400260 	ldr	x0, [x19]
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
 710:	a94363f7 	ldp	x23, x24, [sp, #48]
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:262
 714:	8b160000 	add	x0, x0, x22
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
 718:	a9425bf5 	ldp	x21, x22, [sp, #32]
 71c:	f94023f9 	ldr	x25, [sp, #64]
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:262
 720:	f9000660 	str	x0, [x19, #8]
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:33
 724:	a94153f3 	ldp	x19, x20, [sp, #16]
 728:	a8c57bfd 	ldp	x29, x30, [sp], #80
 72c:	d65f03c0 	ret
_ZSt10_ConstructI1HJRKS0_EEvPT_DpOT0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 730:	aa1403e1 	mov	x1, x20
 734:	aa1703e0 	mov	x0, x23
 738:	9100a294 	add	x20, x20, #0x28
_ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1HSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 73c:	9100a2f7 	add	x23, x23, #0x28
_ZSt10_ConstructI1HJRKS0_EEvPT_DpOT0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 740:	94000000 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
_ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1HSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 744:	17ffffe5 	b	6d8 <cs_vec_assign_H+0x68>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.h:804
 748:	91002280 	add	x0, x20, #0x8
_ZNSt12_Destroy_auxILb0EE9__destroyIP1HEEvT_S4_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:162 (discriminator 2)
 74c:	9100a294 	add	x20, x20, #0x28
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.h:804
 750:	94000000 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
_ZNSt12_Destroy_auxILb0EE9__destroyIP1HEEvT_S4_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:162 (discriminator 2)
 754:	17ffffe4 	b	6e4 <cs_vec_assign_H+0x74>
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:993
 758:	f9400675 	ldr	x21, [x19, #8]
 75c:	b202e7e0 	mov	x0, #0xcccccccccccccccc    	// #-3689348814741910324
 760:	f29999a0 	movk	x0, #0xcccd
 764:	cb1802b5 	sub	x21, x21, x24
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:248 (discriminator 1)
 768:	eb1502df 	cmp	x22, x21
 76c:	54000328 	b.hi	7d0 <cs_vec_assign_H+0x160>  // b.pmore
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:993
 770:	9343fed5 	asr	x21, x22, #3
 774:	aa1803f9 	mov	x25, x24
 778:	9b007eb5 	mul	x21, x21, x0
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIPK1HPS3_EET0_T_S8_S7_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:386
 77c:	aa1503f7 	mov	x23, x21
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:386 (discriminator 1)
 780:	f10002ff 	cmp	x23, #0x0
 784:	5400018c 	b.gt	7b4 <cs_vec_assign_H+0x144>
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEC4ERKS2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_iterator.h:1076
 788:	f10002bf 	cmp	x21, #0x0
 78c:	d2800500 	mov	x0, #0x28                  	// #40
 790:	9a9fa2b4 	csel	x20, x21, xzr, ge	// ge = tcont
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_iterator.h:1077
 794:	f9400677 	ldr	x23, [x19, #8]
_ZNSt12_Destroy_auxILb0EE9__destroyIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS4_SaIS4_EEEEEEvT_SA_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:160
 798:	9b006294 	madd	x20, x20, x0, x24
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:162 (discriminator 1)
 79c:	eb1402ff 	cmp	x23, x20
 7a0:	54fffb60 	b.eq	70c <cs_vec_assign_H+0x9c>  // b.none
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.h:804
 7a4:	91002280 	add	x0, x20, #0x8
 7a8:	9100a294 	add	x20, x20, #0x28
 7ac:	94000000 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEppEv():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_iterator.h:1112
 7b0:	17fffffb 	b	79c <cs_vec_assign_H+0x12c>
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIPK1HPS3_EET0_T_S8_S7_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:388
 7b4:	aa1403e1 	mov	x1, x20
 7b8:	aa1903e0 	mov	x0, x25
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:389
 7bc:	9100a294 	add	x20, x20, #0x28
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:390
 7c0:	9100a339 	add	x25, x25, #0x28
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:386 (discriminator 3)
 7c4:	d10006f7 	sub	x23, x23, #0x1
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:388
 7c8:	97ffff6d 	bl	57c <_ZN1HaSERKS_.isra.0>
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:386 (discriminator 3)
 7cc:	17ffffed 	b	780 <cs_vec_assign_H+0x110>
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:386
 7d0:	9343feb5 	asr	x21, x21, #3
 7d4:	9b007eb5 	mul	x21, x21, x0
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:386 (discriminator 1)
 7d8:	f10002bf 	cmp	x21, #0x0
 7dc:	540001ac 	b.gt	810 <cs_vec_assign_H+0x1a0>
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:993
 7e0:	a9405660 	ldp	x0, x21, [x19]
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:258
 7e4:	a9405ef4 	ldp	x20, x23, [x23]
_ZNKSt6vectorI1HSaIS0_EE4sizeEv():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_vector.h:993
 7e8:	cb0002a0 	sub	x0, x21, x0
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:257 (discriminator 1)
 7ec:	8b000294 	add	x20, x20, x0
_ZSt16__do_uninit_copyIP1HS1_ET0_T_S3_S2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 1)
 7f0:	eb1402ff 	cmp	x23, x20
 7f4:	54fff8c0 	b.eq	70c <cs_vec_assign_H+0x9c>  // b.none
_ZSt10_ConstructI1HJRS0_EEvPT_DpOT0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 7f8:	aa1403e1 	mov	x1, x20
 7fc:	aa1503e0 	mov	x0, x21
_ZSt16__do_uninit_copyIP1HS1_ET0_T_S3_S2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 800:	9100a294 	add	x20, x20, #0x28
 804:	9100a2b5 	add	x21, x21, #0x28
_ZSt10_ConstructI1HJRS0_EEvPT_DpOT0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_construct.h:119 (discriminator 1)
 808:	94000000 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
_ZSt16__do_uninit_copyIP1HS1_ET0_T_S3_S2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_uninitialized.h:119 (discriminator 2)
 80c:	17fffff9 	b	7f0 <cs_vec_assign_H+0x180>
_ZNSt11__copy_moveILb0ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:388
 810:	aa1403e1 	mov	x1, x20
 814:	aa1803e0 	mov	x0, x24
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:389
 818:	9100a294 	add	x20, x20, #0x28
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:390
 81c:	9100a318 	add	x24, x24, #0x28
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:386 (discriminator 3)
 820:	d10006b5 	sub	x21, x21, #0x1
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:388
 824:	97ffff56 	bl	57c <_ZN1HaSERKS_.isra.0>
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:386 (discriminator 3)
 828:	17ffffec 	b	7d8 <cs_vec_assign_H+0x168>
 82c:	d65f03c0 	ret

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
