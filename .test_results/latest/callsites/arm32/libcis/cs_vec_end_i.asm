; call site: cs_vec_end_i   lib=libcis   arch=arm32   size=16 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

0000023e <cs_vec_end_i>:
_ZNSt6vectorIiSaIiEE3endEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:660
 23e:	6800      	ldr	r0, [r0, #0]
_ZNKSt6vectorIiSaIiEE8end_ptr_Ev():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:113
 240:	b120      	cbz	r0, 24c <cs_vec_end_i+0xe>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:114 (discriminator 1)
 242:	f850 3c04 	ldr.w	r3, [r0, #-4]
 246:	f023 0303 	bic.w	r3, r3, #3
 24a:	4418      	add	r0, r3
cs_vec_end_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:50
 24c:	4770      	bx	lr

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

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE12ensure_free_Ej:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_j.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_j:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:
