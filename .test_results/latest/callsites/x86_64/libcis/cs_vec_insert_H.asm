; call site: cs_vec_insert_H   lib=libcis   arch=x86_64   size=146 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000551 <cs_vec_insert_H>:
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 551:	f3 0f 1e fa          	endbr64
 555:	41 54                	push   %r12
 557:	49 89 f4             	mov    %rsi,%r12
 55a:	55                   	push   %rbp
 55b:	48 89 fd             	mov    %rdi,%rbp
 55e:	53                   	push   %rbx
 55f:	48 83 ec 10          	sub    $0x10,%rsp
_ZNSt6vectorI1HSaIS0_EE5beginEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:658
 563:	48 8b 17             	mov    (%rdi),%rdx
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 566:	48 89 d7             	mov    %rdx,%rdi
 569:	e8 a6 fa ff ff       	call   14 <_ZNKSt6vectorI1HSaIS0_EE4sizeEv.isra.0>
 56e:	48 89 c3             	mov    %rax,%rbx
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:683
 571:	31 c0                	xor    %eax,%eax
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
 573:	48 85 d2             	test   %rdx,%rdx
 576:	74 08                	je     580 <cs_vec_insert_H+0x2f>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688 (discriminator 1)
 578:	48 8b 42 f0          	mov    -0x10(%rdx),%rax
 57c:	48 c1 e8 04          	shr    $0x4,%rax
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779 (discriminator 1)
 580:	48 29 d8             	sub    %rbx,%rax
 583:	48 83 f8 01          	cmp    $0x1,%rax
 587:	77 0c                	ja     595 <cs_vec_insert_H+0x44>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 589:	48 8d 73 02          	lea    0x2(%rbx),%rsi
 58d:	48 89 ef             	mov    %rbp,%rdi
 590:	e8 00 00 00 00       	call   595 <cs_vec_insert_H+0x44>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
 595:	4c 8b 45 00          	mov    0x0(%rbp),%r8
 599:	48 c1 e3 04          	shl    $0x4,%rbx
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 59d:	4c 89 e6             	mov    %r12,%rsi
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
 5a0:	49 01 d8             	add    %rbx,%r8
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 5a3:	4c 89 c7             	mov    %r8,%rdi
 5a6:	4c 89 44 24 08       	mov    %r8,0x8(%rsp)
 5ab:	e8 00 00 00 00       	call   5b0 <cs_vec_insert_H+0x5f>
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:815
 5b0:	48 8b 45 00          	mov    0x0(%rbp),%rax
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375 (discriminator 2)
 5b4:	48 89 ea             	mov    %rbp,%rdx
 5b7:	48 8d 35 00 00 00 00 	lea    0x0(%rip),%rsi        # 5be <cs_vec_insert_H+0x6d>
 5be:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 5c5 <cs_vec_insert_H+0x74>
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:815
 5c5:	48 8d 48 10          	lea    0x10(%rax),%rcx
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375 (discriminator 2)
 5c9:	50                   	push   %rax
 5ca:	6a 10                	push   $0x10
 5cc:	4c 8b 44 24 18       	mov    0x18(%rsp),%r8
 5d1:	4d 8d 48 10          	lea    0x10(%r8),%r9
 5d5:	e8 00 00 00 00       	call   5da <cs_vec_insert_H+0x89>
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:31
 5da:	48 83 c4 20          	add    $0x20,%rsp
 5de:	5b                   	pop    %rbx
 5df:	5d                   	pop    %rbp
 5e0:	41 5c                	pop    %r12
 5e2:	c3                   	ret

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
