; call site: cs_vec_push_H   lib=libcis   arch=x86_64   size=148 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

00000000000005e3 <cs_vec_push_H>:
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 5e3:	f3 0f 1e fa          	endbr64
 5e7:	41 54                	push   %r12
 5e9:	49 89 f4             	mov    %rsi,%r12
 5ec:	55                   	push   %rbp
 5ed:	48 89 fd             	mov    %rdi,%rbp
 5f0:	53                   	push   %rbx
 5f1:	48 8b 17             	mov    (%rdi),%rdx
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 5f4:	48 89 d7             	mov    %rdx,%rdi
 5f7:	e8 18 fa ff ff       	call   14 <_ZNKSt6vectorI1HSaIS0_EE4sizeEv.isra.0>
 5fc:	48 89 c3             	mov    %rax,%rbx
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:683
 5ff:	31 c0                	xor    %eax,%eax
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
 601:	48 85 d2             	test   %rdx,%rdx
 604:	74 08                	je     60e <cs_vec_push_H+0x2b>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688 (discriminator 1)
 606:	48 8b 42 f0          	mov    -0x10(%rdx),%rax
 60a:	48 c1 e8 04          	shr    $0x4,%rax
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779 (discriminator 1)
 60e:	48 39 c3             	cmp    %rax,%rbx
 611:	75 0c                	jne    61f <cs_vec_push_H+0x3c>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 613:	48 8d 73 01          	lea    0x1(%rbx),%rsi
 617:	48 89 ef             	mov    %rbp,%rdi
 61a:	e8 00 00 00 00       	call   61f <cs_vec_push_H+0x3c>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
 61f:	48 c1 e3 04          	shl    $0x4,%rbx
 623:	48 03 5d 00          	add    0x0(%rbp),%rbx
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 627:	4c 89 e6             	mov    %r12,%rsi
 62a:	48 89 df             	mov    %rbx,%rdi
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:794
 62d:	48 83 c3 10          	add    $0x10,%rbx
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 631:	e8 00 00 00 00       	call   636 <cs_vec_push_H+0x53>
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:794
 636:	48 8b 55 00          	mov    0x0(%rbp),%rdx
 63a:	48 29 d3             	sub    %rdx,%rbx
_ZNSt6vectorI1HSaIS0_EE15set_size_elems_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:129
 63d:	48 85 d2             	test   %rdx,%rdx
 640:	74 04                	je     646 <cs_vec_push_H+0x63>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:136 (discriminator 1)
 642:	48 89 5a f8          	mov    %rbx,-0x8(%rdx)
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 646:	48 89 d7             	mov    %rdx,%rdi
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687 (discriminator 1)
 649:	31 c9                	xor    %ecx,%ecx
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 64b:	e8 c4 f9 ff ff       	call   14 <_ZNKSt6vectorI1HSaIS0_EE4sizeEv.isra.0>
_ZNKSt6vectorI1HSaIS0_EE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
 650:	48 85 d2             	test   %rdx,%rdx
 653:	74 08                	je     65d <cs_vec_push_H+0x7a>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688 (discriminator 1)
 655:	48 8b 4a f0          	mov    -0x10(%rdx),%rcx
 659:	48 c1 e9 04          	shr    $0x4,%rcx
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779 (discriminator 1)
 65d:	48 39 c8             	cmp    %rcx,%rax
 660:	75 10                	jne    672 <cs_vec_push_H+0x8f>
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 662:	5b                   	pop    %rbx
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 663:	48 89 ef             	mov    %rbp,%rdi
 666:	48 8d 70 01          	lea    0x1(%rax),%rsi
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 66a:	5d                   	pop    %rbp
 66b:	41 5c                	pop    %r12
_ZNSt6vectorI1HSaIS0_EE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 66d:	e9 00 00 00 00       	jmp    672 <cs_vec_push_H+0x8f>
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:30
 672:	5b                   	pop    %rbx
 673:	5d                   	pop    %rbp
 674:	41 5c                	pop    %r12
 676:	c3                   	ret

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
