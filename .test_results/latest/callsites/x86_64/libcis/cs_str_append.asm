; call site: cs_str_append   lib=libcis   arch=x86_64   size=46 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

000000000000046a <cs_str_append>:
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
 46a:	f3 0f 1e fa          	endbr64
 46e:	55                   	push   %rbp
 46f:	48 89 fd             	mov    %rdi,%rbp
_ZNSt11char_traitsIcE6lengthEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:104 (discriminator 1)
 472:	48 89 f7             	mov    %rsi,%rdi
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
 475:	48 83 ec 10          	sub    $0x10,%rsp
_ZNSt11char_traitsIcE6lengthEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:104 (discriminator 1)
 479:	48 89 74 24 08       	mov    %rsi,0x8(%rsp)
 47e:	e8 00 00 00 00       	call   483 <cs_str_append+0x19>
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:798 (discriminator 1)
 483:	48 8b 74 24 08       	mov    0x8(%rsp),%rsi
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
 488:	48 83 c4 10          	add    $0x10,%rsp
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:798 (discriminator 1)
 48c:	48 89 ef             	mov    %rbp,%rdi
_ZNSt11char_traitsIcE6lengthEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:104 (discriminator 1)
 48f:	48 89 c2             	mov    %rax,%rdx
cs_str_append():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:35
 492:	5d                   	pop    %rbp
_ZNSs6appendEPKc():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:798 (discriminator 1)
 493:	e9 90 fe ff ff       	jmp    328 <_ZNSs6appendEPKcm.isra.0>

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
