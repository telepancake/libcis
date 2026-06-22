; call site: cs_vec_push_int   lib=libcis   arch=x86_64   size=67 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000498 <cs_vec_push_int>:
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
 498:	f3 0f 1e fa          	endbr64
 49c:	55                   	push   %rbp
 49d:	89 f5                	mov    %esi,%ebp
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:789
 49f:	be 01 00 00 00       	mov    $0x1,%esi
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
 4a4:	48 83 ec 10          	sub    $0x10,%rsp
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:789
 4a8:	48 89 7c 24 08       	mov    %rdi,0x8(%rsp)
 4ad:	e8 00 00 00 00       	call   4b2 <cs_vec_push_int+0x1a>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:791
 4b2:	48 8b 7c 24 08       	mov    0x8(%rsp),%rdi
_ZSt12construct_atIiJRKiEEPT_S3_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19 (discriminator 1)
 4b7:	89 28                	mov    %ebp,(%rax)
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:791
 4b9:	48 83 c0 04          	add    $0x4,%rax
 4bd:	48 8b 17             	mov    (%rdi),%rdx
 4c0:	48 29 d0             	sub    %rdx,%rax
_ZNSt6vectorIiSaIiEE15set_size_elems_Em():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:129
 4c3:	48 85 d2             	test   %rdx,%rdx
 4c6:	74 04                	je     4cc <cs_vec_push_int+0x34>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:136 (discriminator 1)
 4c8:	48 89 42 f8          	mov    %rax,-0x8(%rdx)
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
 4cc:	48 83 c4 10          	add    $0x10,%rsp
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
 4d0:	be 01 00 00 00       	mov    $0x1,%esi
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:29
 4d5:	5d                   	pop    %rbp
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
 4d6:	e9 00 00 00 00       	jmp    4db <cs_vec_assign_H>

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
