; call site: cs_str_append   lib=libcis   arch=arm32   size=24 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

000003b4 <cs_str_append>:
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
 3b4:	b538      	push	{r3, r4, r5, lr}
 3b6:	4605      	mov	r5, r0
 3b8:	460c      	mov	r4, r1
_ZNSt11char_traitsIcE6lengthEPKc():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:104 (discriminator 1)
 3ba:	4608      	mov	r0, r1
 3bc:	f7ff fffe 	bl	0 <strlen>
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:800 (discriminator 1)
 3c0:	4621      	mov	r1, r4
_ZNSt11char_traitsIcE6lengthEPKc():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:104 (discriminator 1)
 3c2:	4602      	mov	r2, r0
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:800 (discriminator 1)
 3c4:	4628      	mov	r0, r5
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:35
 3c6:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:800 (discriminator 1)
 3ca:	e764      	b.n	296 <_ZNSs6appendEPKcj.isra.0>

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

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE10grow_cold_Ej:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_j.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_j:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:
