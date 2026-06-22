; call site: cs_vec_size_i   lib=libcis   arch=arm32   size=12 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

00000220 <cs_vec_size_i>:
cs_vec_size_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:48
 220:	6800      	ldr	r0, [r0, #0]
_ZNKSt6vectorIiSaIiEE4sizeEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:679
 222:	b110      	cbz	r0, 22a <cs_vec_size_i+0xa>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:680 (discriminator 1)
 224:	f850 0c04 	ldr.w	r0, [r0, #-4]
 228:	0880      	lsrs	r0, r0, #2
cs_vec_size_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:48
 22a:	4770      	bx	lr

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
