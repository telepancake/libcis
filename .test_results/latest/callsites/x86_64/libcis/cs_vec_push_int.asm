; call site: cs_vec_push_int   lib=libcis   arch=x86_64   size=135 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

000000000000049a <cs_vec_push_int>:
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:29
 49a:	f3 0f 1e fa          	endbr64
 49e:	41 54                	push   %r12
 4a0:	41 89 f4             	mov    %esi,%r12d
 4a3:	55                   	push   %rbp
 4a4:	53                   	push   %rbx
 4a5:	48 8b 17             	mov    (%rdi),%rdx
 4a8:	48 89 fb             	mov    %rdi,%rbx
_ZNSt6vectorIiSaIiEE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 4ab:	48 89 d7             	mov    %rdx,%rdi
 4ae:	e8 bb fb ff ff       	call   6e <_ZNKSt6vectorIiSaIiEE4sizeEv.isra.0>
 4b3:	48 89 c5             	mov    %rax,%rbp
_ZNKSt6vectorIiSaIiEE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:683
 4b6:	31 c0                	xor    %eax,%eax
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
 4b8:	48 85 d2             	test   %rdx,%rdx
 4bb:	74 08                	je     4c5 <cs_vec_push_int+0x2b>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688 (discriminator 1)
 4bd:	48 8b 42 f0          	mov    -0x10(%rdx),%rax
 4c1:	48 c1 e8 02          	shr    $0x2,%rax
_ZNSt6vectorIiSaIiEE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779 (discriminator 1)
 4c5:	48 39 c5             	cmp    %rax,%rbp
 4c8:	75 0c                	jne    4d6 <cs_vec_push_int+0x3c>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 4ca:	48 8d 75 01          	lea    0x1(%rbp),%rsi
 4ce:	48 89 df             	mov    %rbx,%rdi
 4d1:	e8 00 00 00 00       	call   4d6 <cs_vec_push_int+0x3c>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:781
 4d6:	48 8b 13             	mov    (%rbx),%rdx
 4d9:	48 8d 04 aa          	lea    (%rdx,%rbp,4),%rax
_ZSt12construct_atIiJRKiEEPT_S3_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/bits/construct_at.h:19 (discriminator 1)
 4dd:	44 89 20             	mov    %r12d,(%rax)
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:794
 4e0:	48 83 c0 04          	add    $0x4,%rax
 4e4:	48 29 d0             	sub    %rdx,%rax
_ZNSt6vectorIiSaIiEE15set_size_elems_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:129
 4e7:	48 85 d2             	test   %rdx,%rdx
 4ea:	74 04                	je     4f0 <cs_vec_push_int+0x56>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:136 (discriminator 1)
 4ec:	48 89 42 f8          	mov    %rax,-0x8(%rdx)
_ZNSt6vectorIiSaIiEE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 4f0:	48 89 d7             	mov    %rdx,%rdi
_ZNKSt6vectorIiSaIiEE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687 (discriminator 1)
 4f3:	31 c9                	xor    %ecx,%ecx
_ZNSt6vectorIiSaIiEE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:778
 4f5:	e8 74 fb ff ff       	call   6e <_ZNKSt6vectorIiSaIiEE4sizeEv.isra.0>
_ZNKSt6vectorIiSaIiEE8capacityEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:687
 4fa:	48 85 d2             	test   %rdx,%rdx
 4fd:	74 08                	je     507 <cs_vec_push_int+0x6d>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:688 (discriminator 1)
 4ff:	48 8b 4a f0          	mov    -0x10(%rdx),%rcx
 503:	48 c1 e9 02          	shr    $0x2,%rcx
_ZNSt6vectorIiSaIiEE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:779 (discriminator 1)
 507:	48 39 c8             	cmp    %rcx,%rax
 50a:	75 10                	jne    51c <cs_vec_push_int+0x82>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 50c:	48 89 df             	mov    %rbx,%rdi
 50f:	48 8d 70 01          	lea    0x1(%rax),%rsi
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:29
 513:	5b                   	pop    %rbx
 514:	5d                   	pop    %rbp
 515:	41 5c                	pop    %r12
_ZNSt6vectorIiSaIiEE12ensure_free_Em():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:780
 517:	e9 00 00 00 00       	jmp    51c <cs_vec_push_int+0x82>
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:29
 51c:	5b                   	pop    %rbx
 51d:	5d                   	pop    %rbp
 51e:	41 5c                	pop    %r12
 520:	c3                   	ret

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
