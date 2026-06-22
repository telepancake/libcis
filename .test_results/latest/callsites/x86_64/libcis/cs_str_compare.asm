; call site: cs_str_compare   lib=libcis   arch=x86_64   size=79 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

00000000000001fe <cs_str_compare>:
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:36
 1fe:	f3 0f 1e fa          	endbr64
 202:	41 54                	push   %r12
 204:	55                   	push   %rbp
 205:	48 89 f5             	mov    %rsi,%rbp
 208:	53                   	push   %rbx
_ZNKSs7compareERKSs():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:1329
 209:	e8 00 00 00 00       	call   20e <cs_str_compare+0x10>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:1329 (discriminator 1)
 20e:	48 89 ef             	mov    %rbp,%rdi
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:1329
 211:	48 89 d3             	mov    %rdx,%rbx
 214:	49 89 c4             	mov    %rax,%r12
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:1329 (discriminator 1)
 217:	e8 00 00 00 00       	call   21c <cs_str_compare+0x1e>
_ZNSt11char_traitsIcE7compareEPKcS2_m():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:98
 21c:	4c 89 e7             	mov    %r12,%rdi
_ZNSt6detail6sv_minImEET_S1_S1_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:41
 21f:	48 39 da             	cmp    %rbx,%rdx
_ZNKSs7compareERKSs():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:1329 (discriminator 1)
 222:	48 89 d5             	mov    %rdx,%rbp
_ZNSt6detail6sv_minImEET_S1_S1_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:41
 225:	48 89 da             	mov    %rbx,%rdx
_ZNKSs7compareERKSs():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:1329 (discriminator 1)
 228:	48 89 c6             	mov    %rax,%rsi
_ZNSt6detail6sv_minImEET_S1_S1_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:41
 22b:	48 0f 46 d5          	cmovbe %rbp,%rdx
_ZNSt11char_traitsIcE7compareEPKcS2_m():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:98
 22f:	e8 00 00 00 00       	call   234 <cs_str_compare+0x36>
_ZNKSt17basic_string_viewIcSt11char_traitsIcEE7compareES2_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:696
 234:	85 c0                	test   %eax,%eax
 236:	75 10                	jne    248 <cs_str_compare+0x4a>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:697 (discriminator 1)
 238:	83 c8 ff             	or     $0xffffffff,%eax
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:697
 23b:	48 39 eb             	cmp    %rbp,%rbx
 23e:	72 08                	jb     248 <cs_str_compare+0x4a>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:698
 240:	31 c0                	xor    %eax,%eax
 242:	48 39 dd             	cmp    %rbx,%rbp
 245:	0f 92 c0             	setb   %al
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:36
 248:	5b                   	pop    %rbx
 249:	5d                   	pop    %rbp
 24a:	41 5c                	pop    %r12
 24c:	c3                   	ret

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
