; call site: cs_vec_erase_H   lib=ref   arch=arm64   size=128 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm64/ref.o:     file format elf64-littleaarch64


Disassembly of section .text:

00000000000001ac <cs_vec_erase_H>:
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 1ac:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
 1b0:	910003fd 	mov	x29, sp
 1b4:	a90153f3 	stp	x19, x20, [sp, #16]
 1b8:	aa0003f4 	mov	x20, x0
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEC4ERKS2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_iterator.h:1077
 1bc:	f9400000 	ldr	x0, [x0]
 1c0:	f9400693 	ldr	x19, [x20, #8]
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEl():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_iterator.h:1148
 1c4:	91014001 	add	x1, x0, #0x50
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 1c8:	f90013f5 	str	x21, [sp, #32]
_ZNSt6vectorI1HSaIS0_EE8_M_eraseEN9__gnu_cxx17__normal_iteratorIPS0_S2_EE():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:184 (discriminator 3)
 1cc:	eb13003f 	cmp	x1, x19
 1d0:	54000121 	b.ne	1f4 <cs_vec_erase_H+0x48>  // b.any
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:186
 1d4:	f9400680 	ldr	x0, [x20, #8]
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 1d8:	f94013f5 	ldr	x21, [sp, #32]
_ZNSt6vectorI1HSaIS0_EE8_M_eraseEN9__gnu_cxx17__normal_iteratorIPS0_S2_EE():
/usr/aarch64-linux-gnu/include/c++/13/bits/vector.tcc:186
 1dc:	d100a001 	sub	x1, x0, #0x28
 1e0:	f9000681 	str	x1, [x20, #8]
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.h:804
 1e4:	d1008000 	sub	x0, x0, #0x20
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 1e8:	a94153f3 	ldp	x19, x20, [sp, #16]
 1ec:	a8c37bfd 	ldp	x29, x30, [sp], #48
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED4Ev():
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.h:804
 1f0:	14000000 	b	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
_ZNSt11__copy_moveILb1ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:411
 1f4:	cb010273 	sub	x19, x19, x1
 1f8:	b202e7e1 	mov	x1, #0xcccccccccccccccc    	// #-3689348814741910324
 1fc:	f29999a1 	movk	x1, #0xcccd
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEl():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_iterator.h:1148
 200:	9100a000 	add	x0, x0, #0x28
_ZNSt11__copy_moveILb1ELb0ESt26random_access_iterator_tagE8__copy_mIP1HS4_EET0_T_S6_S5_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:411
 204:	9343fe73 	asr	x19, x19, #3
 208:	9b017e73 	mul	x19, x19, x1
 20c:	9100a015 	add	x21, x0, #0x28
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:411 (discriminator 1)
 210:	f100027f 	cmp	x19, #0x0
 214:	54fffe0d 	b.le	1d4 <cs_vec_erase_H+0x28>
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:413
 218:	aa1503e1 	mov	x1, x21
 21c:	97ffffab 	bl	c8 <_ZN1HaSEOS_.isra.0>
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:415
 220:	aa1503e0 	mov	x0, x21
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:411 (discriminator 3)
 224:	d1000673 	sub	x19, x19, #0x1
 228:	17fffff9 	b	20c <cs_vec_erase_H+0x60>

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
