; call site: cs_vec_assign_H   lib=libcis   arch=x86_64   size=48 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

00000000000004db <cs_vec_assign_H>:
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 4db:	f3 0f 1e fa          	endbr64
_ZNSt6vectorI1HSaIS0_EEaSERKS2_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:594
 4df:	48 39 f7             	cmp    %rsi,%rdi
 4e2:	74 26                	je     50a <cs_vec_assign_H+0x2f>
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 4e4:	50                   	push   %rax
 4e5:	48 8b 36             	mov    (%rsi),%rsi
 4e8:	49 89 f8             	mov    %rdi,%r8
_ZNKSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:661
 4eb:	48 89 f7             	mov    %rsi,%rdi
 4ee:	e8 5f fb ff ff       	call   52 <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:635
 4f3:	4c 89 c7             	mov    %r8,%rdi
cs_vec_assign_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:33
 4f6:	41 58                	pop    %r8
_ZSt8distanceIPK1HENSt15iterator_traitsIT_E15difference_typeES4_S4_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/iterator:1087
 4f8:	48 89 c1             	mov    %rax,%rcx
_ZNKSt6vectorI1HSaIS0_EE3endEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:661
 4fb:	48 89 c2             	mov    %rax,%rdx
_ZSt8distanceIPK1HENSt15iterator_traitsIT_E15difference_typeES4_S4_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/iterator:1087
 4fe:	48 29 f1             	sub    %rsi,%rcx
 501:	48 c1 f9 04          	sar    $0x4,%rcx
_ZNSt6vectorI1HSaIS0_EE6assignIPKS0_EEvT_S6_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:635
 505:	e9 00 00 00 00       	jmp    50a <cs_vec_assign_H+0x2f>
 50a:	c3                   	ret

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
