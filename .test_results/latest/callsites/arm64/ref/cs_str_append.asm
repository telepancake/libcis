; call site: cs_str_append   lib=ref   arch=arm64   size=288 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm64/ref.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000830 <cs_str_append>:
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
 830:	d10143ff 	sub	sp, sp, #0x50
 834:	a9017bfd 	stp	x29, x30, [sp, #16]
 838:	910043fd 	add	x29, sp, #0x10
 83c:	a90253f3 	stp	x19, x20, [sp, #32]
 840:	aa0003f3 	mov	x19, x0
 844:	90000000 	adrp	x0, 0 <__stack_chk_guard>
 848:	f9400000 	ldr	x0, [x0]
 84c:	a9035bf5 	stp	x21, x22, [sp, #48]
 850:	aa0103f6 	mov	x22, x1
 854:	a90463f7 	stp	x23, x24, [sp, #64]
 858:	f9400001 	ldr	x1, [x0]
 85c:	f90007e1 	str	x1, [sp, #8]
 860:	d2800001 	mov	x1, #0x0                   	// #0
_ZNSt11char_traitsIcE6lengthEPKc():
/usr/aarch64-linux-gnu/include/c++/13/bits/char_traits.h:399
 864:	aa1603e0 	mov	x0, x22
 868:	94000000 	bl	0 <strlen>
 86c:	aa0003f4 	mov	x20, x0
 870:	f9400675 	ldr	x21, [x19, #8]
_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE15_M_check_lengthEmmPKc():
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.h:399 (discriminator 2)
 874:	92f00000 	mov	x0, #0x7fffffffffffffff    	// #9223372036854775807
 878:	cb150000 	sub	x0, x0, x21
 87c:	eb00029f 	cmp	x20, x0
 880:	54000089 	b.ls	890 <cs_str_append+0x60>  // b.plast
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.h:400
 884:	90000000 	adrp	x0, 0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
 888:	91000000 	add	x0, x0, #0x0
 88c:	94000000 	bl	0 <_ZSt20__throw_length_errorPKc>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_appendEPKcm():
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.tcc:416 (discriminator 1)
 890:	8b150297 	add	x23, x20, x21
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.tcc:418
 894:	aa1303e0 	mov	x0, x19
 898:	94000000 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
 89c:	aa0003e1 	mov	x1, x0
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.tcc:418 (discriminator 1)
 8a0:	eb0002ff 	cmp	x23, x0
 8a4:	54000248 	b.hi	8ec <cs_str_append+0xbc>  // b.pmore
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.tcc:420
 8a8:	b40000d4 	cbz	x20, 8c0 <cs_str_append+0x90>
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.tcc:421 (discriminator 2)
 8ac:	f9400260 	ldr	x0, [x19]
 8b0:	aa1403e2 	mov	x2, x20
 8b4:	aa1603e1 	mov	x1, x22
 8b8:	8b150000 	add	x0, x0, x21
 8bc:	94000000 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
_ZNSt11char_traitsIcE6assignERcRKc():
/usr/aarch64-linux-gnu/include/c++/13/bits/char_traits.h:358
 8c0:	f9400260 	ldr	x0, [x19]
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_lengthEm():
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.h:218
 8c4:	f9000677 	str	x23, [x19, #8]
_ZNSt11char_traitsIcE6assignERcRKc():
/usr/aarch64-linux-gnu/include/c++/13/bits/char_traits.h:358
 8c8:	3837681f 	strb	wzr, [x0, x23]
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
 8cc:	90000000 	adrp	x0, 0 <__stack_chk_guard>
 8d0:	f9400000 	ldr	x0, [x0]
 8d4:	f94007e2 	ldr	x2, [sp, #8]
 8d8:	f9400001 	ldr	x1, [x0]
 8dc:	eb010042 	subs	x2, x2, x1
 8e0:	d2800001 	mov	x1, #0x0                   	// #0
 8e4:	540002a0 	b.eq	938 <cs_str_append+0x108>  // b.none
 8e8:	94000000 	bl	0 <__stack_chk_fail>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_mutateEmmPKcm():
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.tcc:332 (discriminator 1)
 8ec:	910003e0 	mov	x0, sp
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.tcc:331 (discriminator 1)
 8f0:	f90003f7 	str	x23, [sp]
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.tcc:332 (discriminator 1)
 8f4:	97fffdc3 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
 8f8:	aa0003f8 	mov	x24, x0
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.tcc:334
 8fc:	b40000b5 	cbz	x21, 910 <cs_str_append+0xe0>
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.tcc:335 (discriminator 1)
 900:	f9400261 	ldr	x1, [x19]
 904:	aa1503e2 	mov	x2, x21
 908:	94000000 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.tcc:336
 90c:	b40000b4 	cbz	x20, 920 <cs_str_append+0xf0>
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.tcc:337
 910:	aa1403e2 	mov	x2, x20
 914:	aa1603e1 	mov	x1, x22
 918:	8b150300 	add	x0, x24, x21
 91c:	94000000 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.tcc:342
 920:	aa1303e0 	mov	x0, x19
 924:	94000000 	bl	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm.isra.0>
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_capacityEm():
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.h:250
 928:	f94003e0 	ldr	x0, [sp]
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7_M_dataEPc():
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.h:213
 92c:	f9000278 	str	x24, [x19]
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_capacityEm():
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.h:250
 930:	f9000a60 	str	x0, [x19, #16]
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_mutateEmmPKcm():
/usr/aarch64-linux-gnu/include/c++/13/bits/basic_string.tcc:345
 934:	17ffffe3 	b	8c0 <cs_str_append+0x90>
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
 938:	a9417bfd 	ldp	x29, x30, [sp, #16]
 93c:	a94253f3 	ldp	x19, x20, [sp, #32]
 940:	a9435bf5 	ldp	x21, x22, [sp, #48]
 944:	a94463f7 	ldp	x23, x24, [sp, #64]
 948:	910143ff 	add	sp, sp, #0x50
 94c:	d65f03c0 	ret

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
