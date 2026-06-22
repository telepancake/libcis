; call site: cs_vec_erase_H   lib=libcis   arch=x86_64   size=61 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

00000000000001c1 <cs_vec_erase_H>:
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 1c1:	f3 0f 1e fa          	endbr64
 1c5:	55                   	push   %rbp
 1c6:	53                   	push   %rbx
 1c7:	48 89 fb             	mov    %rdi,%rbx
 1ca:	50                   	push   %rax
_ZNSt6vectorI1HSaIS0_EE5beginEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:636
 1cb:	48 8b 2f             	mov    (%rdi),%rbp
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:866
 1ce:	e8 00 00 00 00       	call   1d3 <cs_vec_erase_H+0x12>
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32 (discriminator 1)
 1d3:	48 8d 4d 10          	lea    0x10(%rbp),%rcx
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:353 (discriminator 2)
 1d7:	4c 8d 45 20          	lea    0x20(%rbp),%r8
 1db:	52                   	push   %rdx
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:866
 1dc:	49 89 c1             	mov    %rax,%r9
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:353 (discriminator 2)
 1df:	48 89 da             	mov    %rbx,%rdx
 1e2:	48 8d 35 00 00 00 00 	lea    0x0(%rip),%rsi        # 1e9 <cs_vec_erase_H+0x28>
 1e9:	6a f0                	push   $0xfffffffffffffff0
 1eb:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 1f2 <cs_vec_erase_H+0x31>
 1f2:	e8 00 00 00 00       	call   1f7 <cs_vec_erase_H+0x36>
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 1f7:	48 83 c4 18          	add    $0x18,%rsp
 1fb:	5b                   	pop    %rbx
 1fc:	5d                   	pop    %rbp
 1fd:	c3                   	ret

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
