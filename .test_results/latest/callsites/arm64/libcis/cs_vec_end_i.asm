; call site: cs_vec_end_i   lib=libcis   arch=arm64   size=24 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm64/libcis.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000348 <cs_vec_end_i>:
_ZNSt6vectorIiSaIiEE3endEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:660
 348:	f9400000 	ldr	x0, [x0]
_ZNKSt6vectorIiSaIiEE8end_ptr_Ev():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:113
 34c:	b4000080 	cbz	x0, 35c <cs_vec_end_i+0x14>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:114 (discriminator 1)
 350:	f85f8001 	ldur	x1, [x0, #-8]
 354:	927ef421 	and	x1, x1, #0xfffffffffffffffc
 358:	8b010000 	add	x0, x0, x1
cs_vec_end_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:50
 35c:	d65f03c0 	ret

Disassembly of section .text._ZNKSt19bad_optional_access4whatEv:

Disassembly of section .text._ZNSt6detail17move_construct_opI1HSaIS1_EEEvPvS3_S3_:

Disassembly of section .text._ZNSt19bad_optional_accessD2Ev:

Disassembly of section .text._ZNSt19bad_optional_accessD0Ev:

Disassembly of section .text._ZN20libcis_atomic_detail15atomic_spinlock4lockEv:

Disassembly of section .text._ZN20libcis_atomic_detail15atomic_spinlock6unlockEv:

Disassembly of section .text._ZNK1HltERKS_:

Disassembly of section .text._ZNSs11vdeallocateEv:

Disassembly of section .text._ZNSt6detail10destroy_opI1HSaIS1_EEEvPvS3_:

Disassembly of section .text._ZNSs9vallocateEm:

Disassembly of section .text._ZNSs9init_copyEPKcm:

Disassembly of section .text._ZN1HaSERKS_.isra.0:

Disassembly of section .text._ZNSt6vectorIiSaIiEE10grow_cold_Em:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE10grow_cold_Em:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_m.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_m:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:
