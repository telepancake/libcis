; call site: cs_vec_push_H   lib=libcis   arch=x86_64   size=78 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

00000000000002e5 <cs_vec_push_H>:
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:30
 2e5:	f3 0f 1e fa          	endbr64
 2e9:	41 54                	push   %r12
 2eb:	49 89 f4             	mov    %rsi,%r12
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:758
 2ee:	be 01 00 00 00       	mov    $0x1,%esi
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:30
 2f3:	55                   	push   %rbp
 2f4:	48 89 fd             	mov    %rdi,%rbp
 2f7:	53                   	push   %rbx
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:758
 2f8:	e8 00 00 00 00       	call   2fd <cs_vec_push_H+0x18>
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/bits/construct_at.h:19 (discriminator 1)
 2fd:	4c 89 e6             	mov    %r12,%rsi
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:758
 300:	48 89 c3             	mov    %rax,%rbx
_ZSt12construct_atI1HJRKS0_EEPT_S4_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/bits/construct_at.h:19 (discriminator 1)
 303:	48 89 c7             	mov    %rax,%rdi
 306:	e8 00 00 00 00       	call   30b <cs_vec_push_H+0x26>
_ZNSt6vectorI1HSaIS0_EE15set_size_elems_Em():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:118 (discriminator 1)
 30b:	48 89 ee             	mov    %rbp,%rsi
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:760
 30e:	48 8d 53 10          	lea    0x10(%rbx),%rdx
_ZNSt6vectorI1HSaIS0_EE15set_size_elems_Em():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:118 (discriminator 1)
 312:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 319 <cs_vec_push_H+0x34>
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:760
 319:	48 2b 55 00          	sub    0x0(%rbp),%rdx
_ZNSt6vectorI1HSaIS0_EE15set_size_elems_Em():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:118 (discriminator 1)
 31d:	e8 00 00 00 00       	call   322 <cs_vec_push_H+0x3d>
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:30
 322:	5b                   	pop    %rbx
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:761
 323:	48 89 ef             	mov    %rbp,%rdi
 326:	be 01 00 00 00       	mov    $0x1,%esi
cs_vec_push_H():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:30
 32b:	5d                   	pop    %rbp
 32c:	41 5c                	pop    %r12
_ZNSt6vectorI1HSaIS0_EE12emplace_backIJRKS0_EEERS0_DpOT_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:761
 32e:	e9 00 00 00 00       	jmp    333 <_ZNSt6detail5sort3IP1HSt4lessIvEEEbT_S5_S5_RT0_.isra.0>

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

Disassembly of section .text._ZNKSt6vectorIiSaIiEE8capacityEv:

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
