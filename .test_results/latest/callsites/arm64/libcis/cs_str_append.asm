; call site: cs_str_append   lib=libcis   arch=arm64   size=52 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm64/libcis.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000524 <cs_str_append>:
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
 524:	a9be7bfd 	stp	x29, x30, [sp, #-32]!
 528:	910003fd 	mov	x29, sp
 52c:	a90153f3 	stp	x19, x20, [sp, #16]
 530:	aa0003f4 	mov	x20, x0
 534:	aa0103f3 	mov	x19, x1
_ZNSt11char_traitsIcE6lengthEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:104 (discriminator 1)
 538:	aa0103e0 	mov	x0, x1
 53c:	94000000 	bl	0 <strlen>
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:798 (discriminator 1)
 540:	aa1303e1 	mov	x1, x19
 544:	aa0003e2 	mov	x2, x0
 548:	aa1403e0 	mov	x0, x20
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
 54c:	a94153f3 	ldp	x19, x20, [sp, #16]
 550:	a8c27bfd 	ldp	x29, x30, [sp], #32
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:798 (discriminator 1)
 554:	17ffff96 	b	3ac <_ZNSs6appendEPKcm.isra.0>

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

Disassembly of section .text._ZNSt6vectorIiSaIiEE12ensure_free_Em:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE12ensure_free_Em:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_m.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_m:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:
