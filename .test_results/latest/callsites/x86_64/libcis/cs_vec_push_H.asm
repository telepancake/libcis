; call site: cs_vec_push_H   lib=libcis   arch=x86_64   size=75 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000565 <cs_vec_push_H>:
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
 565:	f3 0f 1e fa          	endbr64
 569:	41 54                	push   %r12
 56b:	49 89 f4             	mov    %rsi,%r12
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:789
 56e:	be 01 00 00 00       	mov    $0x1,%esi
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
 573:	55                   	push   %rbp
 574:	48 89 fd             	mov    %rdi,%rbp
 577:	53                   	push   %rbx
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:789
 578:	e8 00 00 00 00       	call   57d <cs_vec_push_H+0x18>
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19 (discriminator 1)
 57d:	4c 89 e6             	mov    %r12,%rsi
 580:	48 89 c7             	mov    %rax,%rdi
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:789
 583:	48 89 c3             	mov    %rax,%rbx
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19 (discriminator 1)
 586:	e8 00 00 00 00       	call   58b <cs_vec_push_H+0x26>
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:791
 58b:	48 8b 45 00          	mov    0x0(%rbp),%rax
 58f:	48 83 c3 10          	add    $0x10,%rbx
 593:	48 29 c3             	sub    %rax,%rbx
_ZNSt6vectorI1HSaIS0_EE15set_size_elems_Em():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:129
 596:	48 85 c0             	test   %rax,%rax
 599:	74 04                	je     59f <cs_vec_push_H+0x3a>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:136 (discriminator 1)
 59b:	48 89 58 f8          	mov    %rbx,-0x8(%rax)
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
 59f:	5b                   	pop    %rbx
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
 5a0:	48 89 ef             	mov    %rbp,%rdi
 5a3:	be 01 00 00 00       	mov    $0x1,%esi
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:30
 5a8:	5d                   	pop    %rbp
 5a9:	41 5c                	pop    %r12
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:792
 5ab:	e9 00 00 00 00       	jmp    5b0 <_ZNSt6detail5sort3IP1HSt4lessIvEEEbT_S5_S5_RT0_.isra.0>

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
