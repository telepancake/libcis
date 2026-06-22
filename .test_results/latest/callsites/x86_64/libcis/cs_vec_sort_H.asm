; call site: cs_vec_sort_H   lib=libcis   arch=x86_64   size=53 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000c91 <cs_vec_sort_H>:
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:34
 c91:	f3 0f 1e fa          	endbr64
 c95:	41 51                	push   %r9
 c97:	48 8b 3f             	mov    (%rdi),%rdi
_ZNSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:660
 c9a:	e8 b3 f3 ff ff       	call   52 <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2103
 c9f:	48 39 f8             	cmp    %rdi,%rax
 ca2:	74 20                	je     cc4 <cs_vec_sort_H+0x33>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2113
 ca4:	48 89 c2             	mov    %rax,%rdx
 ca7:	48 89 c6             	mov    %rax,%rsi
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2114
 caa:	b9 01 00 00 00       	mov    $0x1,%ecx
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:34
 caf:	41 58                	pop    %r8
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2113
 cb1:	48 29 fa             	sub    %rdi,%rdx
 cb4:	48 c1 fa 04          	sar    $0x4,%rdx
_ZNSt6detail8bit_log2Ey():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2003 (discriminator 1)
 cb8:	48 0f bd d2          	bsr    %rdx,%rdx
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2112 (discriminator 1)
 cbc:	48 01 d2             	add    %rdx,%rdx
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/algorithm:2114
 cbf:	e9 7d f9 ff ff       	jmp    641 <_ZNSt6detail9introsortIP1HSt4lessIvEEEvT_S5_RT0_NSt15iterator_traitsIS5_E15difference_typeEb.isra.0>
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:34
 cc4:	58                   	pop    %rax
 cc5:	c3                   	ret

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
