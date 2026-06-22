; call site: cs_vec_sort_H   lib=libcis   arch=mips32   size=120 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

00001270 <cs_vec_sort_H>:
_ZNSt6vectorI1HSaIS0_EE5beginEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:658
    1270:	8c880000 	lw	t0,0(a0)
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
    1274:	27bdffe0 	addiu	sp,sp,-32
    1278:	afbf001c 	sw	ra,28(sp)
_ZNSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:660
    127c:	0c000011 	jal	44 <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
    1280:	01002025 	move	a0,t0
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/algorithm:2103
    1284:	11020015 	beq	t0,v0,12dc <cs_vec_sort_H+0x6c>
    1288:	00481823 	subu	v1,v0,t0
    128c:	00402825 	move	a1,v0
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/algorithm:2113
    1290:	3c02aaaa 	lui	v0,0xaaaa
    1294:	00031883 	sra	v1,v1,0x2
    1298:	3442aaab 	ori	v0,v0,0xaaab
    129c:	70621802 	mul	v1,v1,v0
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/algorithm:2112
    12a0:	000317c3 	sra	v0,v1,0x1f
_ZNSt6detail8bit_log2Ey():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/algorithm:2003
    12a4:	1040000a 	beqz	v0,12d0 <cs_vec_sort_H+0x60>
    12a8:	00000000 	nop
    12ac:	70431820 	clz	v1,v0
    12b0:	2406003f 	li	a2,63
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
    12b4:	8fbf001c 	lw	ra,28(sp)
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/algorithm:2114
    12b8:	24070001 	li	a3,1
_ZNSt6detail8bit_log2Ey():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/algorithm:2003
    12bc:	00c33023 	subu	a2,a2,v1
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
    12c0:	27bd0020 	addiu	sp,sp,32
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/algorithm:2114
    12c4:	00063040 	sll	a2,a2,0x1
    12c8:	080002b3 	j	acc <_ZNSt6detail9introsortIP1HSt4lessIvEEEvT_S5_RT0_NSt15iterator_traitsIS5_E15difference_typeEb.isra.0>
    12cc:	01002025 	move	a0,t0
_ZNSt6detail8bit_log2Ey():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/algorithm:2003
    12d0:	70631820 	clz	v1,v1
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/algorithm:2003
    12d4:	1000fff6 	b	12b0 <cs_vec_sort_H+0x40>
    12d8:	24630020 	addiu	v1,v1,32
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:34
    12dc:	8fbf001c 	lw	ra,28(sp)
    12e0:	03e00008 	jr	ra
    12e4:	27bd0020 	addiu	sp,sp,32

Disassembly of section .text._ZNKSt19bad_optional_access4whatEv:

Disassembly of section .text._ZNSt19bad_optional_accessD2Ev:

Disassembly of section .text._ZNSt19bad_optional_accessD0Ev:

Disassembly of section .text._ZNSt6detail17move_construct_opI1HSaIS1_EEEvPvS3_S3_:

Disassembly of section .text._ZN20libcis_atomic_detail15atomic_spinlock4lockEv:

Disassembly of section .text._ZN20libcis_atomic_detail15atomic_spinlock6unlockEv:

Disassembly of section .text._ZNK1HltERKS_:

Disassembly of section .text._ZNSs11vdeallocateEv:

Disassembly of section .text._ZNSt6detail10destroy_opI1HSaIS1_EEEvPvS3_:

Disassembly of section .text._ZNSs9vallocateEj:

Disassembly of section .text._ZNSs9init_copyEPKcj:

Disassembly of section .text._ZN1HaSERKS_.isra.0:

Disassembly of section .text._ZNSt6vectorIiSaIiEE10grow_cold_Ej:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_j.isra.0:

Disassembly of section .text._ZNKSt6vectorI1HSaIS0_EE9recommendEj:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE10grow_cold_Ej:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_j:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:
