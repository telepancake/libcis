; call site: cs_vec_size_i   lib=libcis   arch=mips32   size=28 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/mips32/libcis.o:     file format elf32-tradbigmips


Disassembly of section .text:

000003f0 <cs_vec_size_i>:
cs_vec_size_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:48
     3f0:	8c830000 	lw	v1,0(a0)
_ZNKSt6vectorIiSaIiEE4sizeEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:679
     3f4:	10600003 	beqz	v1,404 <cs_vec_size_i+0x14>
     3f8:	00001025 	move	v0,zero
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:680
     3fc:	8c62fffc 	lw	v0,-4(v1)
     400:	00021082 	srl	v0,v0,0x2
cs_vec_size_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:48
     404:	03e00008 	jr	ra
     408:	00000000 	nop

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

Disassembly of section .text._ZNSt6vectorIiSaIiEE12ensure_free_Ej:

Disassembly of section .text._ZNKSt6vectorI1HSaIS0_EE9recommendEj:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_j.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_j:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:
