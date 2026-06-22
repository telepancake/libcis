; call site: cs_vec_push_int   lib=libcis   arch=x86_64   size=62 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

000000000000024d <cs_vec_push_int>:
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:29
 24d:	f3 0f 1e fa          	endbr64
 251:	55                   	push   %rbp
 252:	89 f5                	mov    %esi,%ebp
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:766
 254:	be 01 00 00 00       	mov    $0x1,%esi
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:29
 259:	53                   	push   %rbx
 25a:	48 89 fb             	mov    %rdi,%rbx
 25d:	50                   	push   %rax
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:766
 25e:	e8 00 00 00 00       	call   263 <cs_vec_push_int+0x16>
_ZNSt6vectorIiSaIiEE15set_size_elems_Em():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:118 (discriminator 1)
 263:	48 89 de             	mov    %rbx,%rsi
 266:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 26d <cs_vec_push_int+0x20>
_ZSt12construct_atIiJRKiEEPT_S3_DpOT0_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/bits/construct_at.h:19 (discriminator 1)
 26d:	89 28                	mov    %ebp,(%rax)
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:768
 26f:	48 8d 50 04          	lea    0x4(%rax),%rdx
 273:	48 2b 13             	sub    (%rbx),%rdx
_ZNSt6vectorIiSaIiEE15set_size_elems_Em():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:118 (discriminator 1)
 276:	e8 00 00 00 00       	call   27b <cs_vec_push_int+0x2e>
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:29
 27b:	5a                   	pop    %rdx
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:769
 27c:	48 89 df             	mov    %rbx,%rdi
 27f:	be 01 00 00 00       	mov    $0x1,%esi
cs_vec_push_int():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:29
 284:	5b                   	pop    %rbx
 285:	5d                   	pop    %rbp
_ZNSt6vectorIiSaIiEE12emplace_backIJRKiEEERiDpOT_():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/vector:769
 286:	e9 00 00 00 00       	jmp    28b <cs_vec_insert_H>

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
