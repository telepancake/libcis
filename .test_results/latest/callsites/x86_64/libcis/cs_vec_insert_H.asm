; call site: cs_vec_insert_H   lib=libcis   arch=x86_64   size=90 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

000000000000050b <cs_vec_insert_H>:
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 50b:	f3 0f 1e fa          	endbr64
 50f:	55                   	push   %rbp
 510:	48 89 f5             	mov    %rsi,%rbp
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:810
 513:	be 02 00 00 00       	mov    $0x2,%esi
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 518:	53                   	push   %rbx
 519:	48 89 fb             	mov    %rdi,%rbx
 51c:	48 83 ec 18          	sub    $0x18,%rsp
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:810
 520:	e8 00 00 00 00       	call   525 <cs_vec_insert_H+0x1a>
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/bits/construct_at.h:19 (discriminator 1)
 525:	48 89 ee             	mov    %rbp,%rsi
 528:	48 89 c7             	mov    %rax,%rdi
 52b:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
 530:	e8 00 00 00 00       	call   535 <cs_vec_insert_H+0x2a>
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:812
 535:	48 8b 03             	mov    (%rbx),%rax
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375 (discriminator 2)
 538:	48 89 da             	mov    %rbx,%rdx
 53b:	48 8d 35 00 00 00 00 	lea    0x0(%rip),%rsi        # 542 <cs_vec_insert_H+0x37>
 542:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 549 <cs_vec_insert_H+0x3e>
_ZNSt6vectorI1HSaIS0_EE7emplaceIJRKS0_EEEPS0_PS4_DpOT_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:812
 549:	48 8d 48 10          	lea    0x10(%rax),%rcx
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375 (discriminator 2)
 54d:	50                   	push   %rax
 54e:	6a 10                	push   $0x10
 550:	4c 8b 44 24 18       	mov    0x18(%rsp),%r8
 555:	4d 8d 48 10          	lea    0x10(%r8),%r9
 559:	e8 00 00 00 00       	call   55e <cs_vec_insert_H+0x53>
cs_vec_insert_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:31
 55e:	48 83 c4 28          	add    $0x28,%rsp
 562:	5b                   	pop    %rbx
 563:	5d                   	pop    %rbp
 564:	c3                   	ret

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
