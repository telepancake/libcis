; call site: cs_str_append   lib=libcis   arch=x86_64   size=46 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000bdd <cs_str_append>:
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:35
 bdd:	f3 0f 1e fa          	endbr64
 be1:	55                   	push   %rbp
 be2:	48 89 fd             	mov    %rdi,%rbp
_ZNSt11char_traitsIcE6lengthEPKc():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/string_view:104 (discriminator 1)
 be5:	48 89 f7             	mov    %rsi,%rdi
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:35
 be8:	48 83 ec 10          	sub    $0x10,%rsp
_ZNSt11char_traitsIcE6lengthEPKc():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/string_view:104 (discriminator 1)
 bec:	48 89 74 24 08       	mov    %rsi,0x8(%rsp)
 bf1:	e8 00 00 00 00       	call   bf6 <cs_str_append+0x19>
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/string:788 (discriminator 1)
 bf6:	48 8b 74 24 08       	mov    0x8(%rsp),%rsi
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:35
 bfb:	48 83 c4 10          	add    $0x10,%rsp
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/string:788 (discriminator 1)
 bff:	48 89 ef             	mov    %rbp,%rdi
_ZNSt11char_traitsIcE6lengthEPKc():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/string_view:104 (discriminator 1)
 c02:	48 89 c2             	mov    %rax,%rdx
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/bench/callsites.cpp:35
 c05:	5d                   	pop    %rbp
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-a27315e16cbfdd9f1/include/string:788 (discriminator 1)
 c06:	e9 7b fe ff ff       	jmp    a86 <_ZNSs6appendEPKcm.isra.0>

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
