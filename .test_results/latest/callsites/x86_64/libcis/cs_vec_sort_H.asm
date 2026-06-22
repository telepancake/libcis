; call site: cs_vec_sort_H   lib=libcis   arch=x86_64   size=54 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000a19 <cs_vec_sort_H>:
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:34
 a19:	f3 0f 1e fa          	endbr64
 a1d:	53                   	push   %rbx
 a1e:	48 89 fb             	mov    %rdi,%rbx
_ZNSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:638
 a21:	e8 00 00 00 00       	call   a26 <cs_vec_sort_H+0xd>
_ZNSt6vectorI1HSaIS0_EE5beginEv():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:636
 a26:	48 8b 3b             	mov    (%rbx),%rdi
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/algorithm:2103
 a29:	48 39 c7             	cmp    %rax,%rdi
 a2c:	74 1f                	je     a4d <cs_vec_sort_H+0x34>
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/algorithm:2113
 a2e:	48 89 c2             	mov    %rax,%rdx
 a31:	48 89 c6             	mov    %rax,%rsi
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/algorithm:2114
 a34:	b9 01 00 00 00       	mov    $0x1,%ecx
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:34
 a39:	5b                   	pop    %rbx
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/algorithm:2113
 a3a:	48 29 fa             	sub    %rdi,%rdx
 a3d:	48 c1 fa 04          	sar    $0x4,%rdx
_ZNSt6detail8bit_log2Ey():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/algorithm:2003 (discriminator 1)
 a41:	48 0f bd d2          	bsr    %rdx,%rdx
_ZSt4sortIP1HSt4lessIvEEvT_S4_T0_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/algorithm:2112 (discriminator 1)
 a45:	48 01 d2             	add    %rdx,%rdx
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/algorithm:2114
 a48:	e9 6f f9 ff ff       	jmp    3bc <_ZNSt6detail9introsortIP1HSt4lessIvEEEvT_S5_RT0_NSt15iterator_traitsIS5_E15difference_typeEb.isra.0>
cs_vec_sort_H():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:34
 a4d:	5b                   	pop    %rbx
 a4e:	c3                   	ret

Disassembly of section .text._ZNKSt19bad_optional_access4whatEv:

Disassembly of section .text._ZNSt6detail17move_construct_opI1HSaIS1_EEEvPvS3_S3_:

Disassembly of section .text._ZNSt19bad_optional_accessD2Ev:

Disassembly of section .text._ZNSt19bad_optional_accessD0Ev:

Disassembly of section .text._ZN20libcis_atomic_detail15atomic_spinlock4lockEv:

Disassembly of section .text._ZN20libcis_atomic_detail15atomic_spinlock6unlockEv:

Disassembly of section .text._ZNKSs8end_ptr_Ev:

Disassembly of section .text._ZNKSs4sizeEv:

Disassembly of section .text._ZStssIcSt11char_traitsIcESaIcEEDaRKSbIT_T0_T1_ES8_:

Disassembly of section .text._ZNK1HltERKS_:

Disassembly of section .text._ZNSs11vdeallocateEv:

Disassembly of section .text._ZNSt6detail10destroy_opI1HSaIS1_EEEvPvS3_:

Disassembly of section .text._ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev:

Disassembly of section .text._ZNKSscvSt17basic_string_viewIcSt11char_traitsIcEEEv:

Disassembly of section .text._ZNSs9vallocateEm:

Disassembly of section .text._ZNSs9init_copyEPKcm:

Disassembly of section .text._ZNKSt6vectorIiSaIiEE4sizeEv:

Disassembly of section .text._ZNSt6vectorIiSaIiEE12ensure_free_Em:

Disassembly of section .text._ZNKSt6vectorI1HSaIS0_EE4sizeEv:

Disassembly of section .text._ZNKSt6vectorI1HSaIS0_EE8capacityEv:

Disassembly of section .text._ZNKSt6vectorI1HSaIS0_EE9recommendEm:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE12ensure_free_Em:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_m.isra.0:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:

Disassembly of section .text._ZNKSs8capacityEv:

Disassembly of section .text._ZN1HaSERKS_.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_m:
