; call site: cs_str_append   lib=ref   arch=mips32   size=352 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/mips32/ref.o:     file format elf32-tradbigmips


Disassembly of section .text:

00000938 <cs_str_append>:
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
 938:	3c1c0000 	lui	gp,0x0
 93c:	27bdffc0 	addiu	sp,sp,-64
 940:	279c0000 	addiu	gp,gp,0
 944:	afb3002c 	sw	s3,44(sp)
 948:	8f930000 	lw	s3,0(gp)
_ZNSt11char_traitsIcE6lengthEPKc():
/usr/mips-linux-gnu/include/c++/12/bits/char_traits.h:389
 94c:	8f990000 	lw	t9,0(gp)
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
 950:	afb40030 	sw	s4,48(sp)
 954:	afb20028 	sw	s2,40(sp)
 958:	00a09025 	move	s2,a1
 95c:	afb10024 	sw	s1,36(sp)
 960:	afb00020 	sw	s0,32(sp)
 964:	00808025 	move	s0,a0
 968:	afbc0010 	sw	gp,16(sp)
_ZNSt11char_traitsIcE6lengthEPKc():
/usr/mips-linux-gnu/include/c++/12/bits/char_traits.h:389
 96c:	00a02025 	move	a0,a1
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
 970:	afbf003c 	sw	ra,60(sp)
 974:	afb60038 	sw	s6,56(sp)
 978:	afb50034 	sw	s5,52(sp)
 97c:	8e620000 	lw	v0,0(s3)
 980:	afa2001c 	sw	v0,28(sp)
_ZNSt11char_traitsIcE6lengthEPKc():
/usr/mips-linux-gnu/include/c++/12/bits/char_traits.h:389
 984:	0320f809 	jalr	t9
 988:	00000000 	nop
 98c:	00408825 	move	s1,v0
 990:	8e140004 	lw	s4,4(s0)
_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE15_M_check_lengthEjjPKc():
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.h:402
 994:	3c027fff 	lui	v0,0x7fff
 998:	3442ffff 	ori	v0,v0,0xffff
 99c:	00541023 	subu	v0,v0,s4
 9a0:	0051102b 	sltu	v0,v0,s1
 9a4:	10400005 	beqz	v0,9bc <cs_str_append+0x84>
 9a8:	8fbc0010 	lw	gp,16(sp)
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.h:403
 9ac:	3c040000 	lui	a0,0x0
 9b0:	8f990000 	lw	t9,0(gp)
 9b4:	0320f809 	jalr	t9
 9b8:	24840000 	addiu	a0,a0,0
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_appendEPKcj():
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.tcc:416
 9bc:	0234b021 	addu	s6,s1,s4
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.tcc:418
 9c0:	0c000000 	jal	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 9c4:	02002025 	move	a0,s0
 9c8:	00402825 	move	a1,v0
 9cc:	0056102b 	sltu	v0,v0,s6
 9d0:	14400013 	bnez	v0,a20 <cs_str_append+0xe8>
 9d4:	8fbc0010 	lw	gp,16(sp)
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.tcc:420
 9d8:	12200006 	beqz	s1,9f4 <cs_str_append+0xbc>
 9dc:	02203025 	move	a2,s1
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.tcc:421
 9e0:	8e040000 	lw	a0,0(s0)
 9e4:	02402825 	move	a1,s2
 9e8:	0c000000 	jal	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 9ec:	00942021 	addu	a0,a0,s4
 9f0:	8fbc0010 	lw	gp,16(sp)
_ZNSt11char_traitsIcE6assignERcRKc():
/usr/mips-linux-gnu/include/c++/12/bits/char_traits.h:348
 9f4:	8e020000 	lw	v0,0(s0)
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
 9f8:	8fa3001c 	lw	v1,28(sp)
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_lengthEj():
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.h:231
 9fc:	ae160004 	sw	s6,4(s0)
_ZNSt11char_traitsIcE6assignERcRKc():
/usr/mips-linux-gnu/include/c++/12/bits/char_traits.h:348
 a00:	00561021 	addu	v0,v0,s6
 a04:	a0400000 	sb	zero,0(v0)
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
 a08:	8e620000 	lw	v0,0(s3)
 a0c:	10620019 	beq	v1,v0,a74 <cs_str_append+0x13c>
 a10:	8fbf003c 	lw	ra,60(sp)
 a14:	8f990000 	lw	t9,0(gp)
 a18:	0320f809 	jalr	t9
 a1c:	00000000 	nop
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_mutateEjjPKcj():
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.tcc:332
 a20:	27a40018 	addiu	a0,sp,24
 a24:	0c000000 	jal	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 a28:	afb60018 	sw	s6,24(sp)
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.tcc:334
 a2c:	12800005 	beqz	s4,a44 <cs_str_append+0x10c>
 a30:	0040a825 	move	s5,v0
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.tcc:335
 a34:	8e050000 	lw	a1,0(s0)
 a38:	02803025 	move	a2,s4
 a3c:	0c000000 	jal	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 a40:	00402025 	move	a0,v0
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.tcc:336
 a44:	12200004 	beqz	s1,a58 <cs_str_append+0x120>
 a48:	02203025 	move	a2,s1
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.tcc:337
 a4c:	02b42021 	addu	a0,s5,s4
 a50:	0c000000 	jal	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 a54:	02402825 	move	a1,s2
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.tcc:342
 a58:	0c000000 	jal	0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj.isra.0>
 a5c:	02002025 	move	a0,s0
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_capacityEj():
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.h:263
 a60:	8fa20018 	lw	v0,24(sp)
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_mutateEjjPKcj():
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.tcc:342
 a64:	8fbc0010 	lw	gp,16(sp)
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7_M_dataEPc():
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.h:226
 a68:	ae150000 	sw	s5,0(s0)
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_mutateEjjPKcj():
/usr/mips-linux-gnu/include/c++/12/bits/basic_string.tcc:345
 a6c:	1000ffe1 	b	9f4 <cs_str_append+0xbc>
 a70:	ae020008 	sw	v0,8(s0)
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
 a74:	8fb60038 	lw	s6,56(sp)
 a78:	8fb50034 	lw	s5,52(sp)
 a7c:	8fb40030 	lw	s4,48(sp)
 a80:	8fb3002c 	lw	s3,44(sp)
 a84:	8fb20028 	lw	s2,40(sp)
 a88:	8fb10024 	lw	s1,36(sp)
 a8c:	8fb00020 	lw	s0,32(sp)
 a90:	03e00008 	jr	ra
 a94:	27bd0040 	addiu	sp,sp,64

Disassembly of section .text._ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7compareERKS4_:

Disassembly of section .text._ZNK1HltERKS_:

Disassembly of section .text._ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE11_M_is_localEv:

Disassembly of section .text._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv:

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
