; call site: cs_vec_sort_H   lib=ref   arch=arm64   size=168 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm64/ref.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000ad4 <cs_vec_sort_H>:
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:34
 ad4:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
 ad8:	910003fd 	mov	x29, sp
 adc:	a90153f3 	stp	x19, x20, [sp, #16]
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEC4ERKS2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_iterator.h:1077
 ae0:	f9400014 	ldr	x20, [x0]
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:34
 ae4:	f90013f5 	str	x21, [sp, #32]
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEC4ERKS2_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_iterator.h:1077
 ae8:	f9400415 	ldr	x21, [x0, #8]
_ZSt6__sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algo.h:1945 (discriminator 1)
 aec:	eb1402bf 	cmp	x21, x20
 af0:	540002a0 	b.eq	b44 <cs_vec_sort_H+0x70>  // b.none
_ZN9__gnu_cxxmiIP1HSt6vectorIS1_SaIS1_EEEENS_17__normal_iteratorIT_T0_E15difference_typeERKS9_SC_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_iterator.h:1337 (discriminator 2)
 af4:	cb1402b3 	sub	x19, x21, x20
 af8:	b202e7e1 	mov	x1, #0xcccccccccccccccc    	// #-3689348814741910324
 afc:	f29999a1 	movk	x1, #0xcccd
_ZSt4__lgIlET_S0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:1518 (discriminator 1)
 b00:	528007e2 	mov	w2, #0x3f                  	// #63
_ZN9__gnu_cxxmiIP1HSt6vectorIS1_SaIS1_EEEENS_17__normal_iteratorIT_T0_E15difference_typeERKS9_SC_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_iterator.h:1337 (discriminator 2)
 b04:	9343fe60 	asr	x0, x19, #3
 b08:	9b017c00 	mul	x0, x0, x1
_ZSt6__sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algo.h:1947
 b0c:	aa1503e1 	mov	x1, x21
 b10:	dac01000 	clz	x0, x0
_ZSt4__lgIlET_S0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algobase.h:1518 (discriminator 1)
 b14:	4b000042 	sub	w2, w2, w0
_ZSt6__sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algo.h:1947
 b18:	aa1403e0 	mov	x0, x20
 b1c:	937f7c42 	sbfiz	x2, x2, #1, #32
 b20:	97ffff8c 	bl	950 <_ZSt16__introsort_loopIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEElNS0_5__ops15_Iter_less_iterEEvT_SA_T0_T1_.isra.0>
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algo.h:1857 (discriminator 1)
 b24:	f10a027f 	cmp	x19, #0x280
 b28:	540001ed 	b.le	b64 <cs_vec_sort_H+0x90>
_ZNK9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEplEl():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_iterator.h:1148
 b2c:	910a0293 	add	x19, x20, #0x280
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algo.h:1859 (discriminator 1)
 b30:	aa1403e0 	mov	x0, x20
 b34:	aa1303e1 	mov	x1, x19
 b38:	97fffdea 	bl	2e0 <_ZSt16__insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_.isra.0>
_ZSt26__unguarded_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algo.h:1839 (discriminator 1)
 b3c:	eb1302bf 	cmp	x21, x19
 b40:	540000a1 	b.ne	b54 <cs_vec_sort_H+0x80>  // b.any
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:34
 b44:	a94153f3 	ldp	x19, x20, [sp, #16]
 b48:	f94013f5 	ldr	x21, [sp, #32]
 b4c:	a8c37bfd 	ldp	x29, x30, [sp], #48
 b50:	d65f03c0 	ret
_ZSt26__unguarded_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algo.h:1840
 b54:	aa1303e0 	mov	x0, x19
 b58:	9100a273 	add	x19, x19, #0x28
 b5c:	97fffdb4 	bl	22c <_ZSt25__unguarded_linear_insertIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops14_Val_less_iterEEvT_T0_.isra.0>
_ZN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS1_SaIS1_EEEppEv():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_iterator.h:1112
 b60:	17fffff7 	b	b3c <cs_vec_sort_H+0x68>
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algo.h:1864
 b64:	aa1503e1 	mov	x1, x21
 b68:	aa1403e0 	mov	x0, x20
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:34
 b6c:	a94153f3 	ldp	x19, x20, [sp, #16]
 b70:	f94013f5 	ldr	x21, [sp, #32]
 b74:	a8c37bfd 	ldp	x29, x30, [sp], #48
_ZSt22__final_insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_():
/usr/aarch64-linux-gnu/include/c++/13/bits/stl_algo.h:1864
 b78:	17fffdda 	b	2e0 <_ZSt16__insertion_sortIN9__gnu_cxx17__normal_iteratorIP1HSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_less_iterEEvT_SA_T0_.isra.0>

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
