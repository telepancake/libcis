; call site: cs_str_compare   lib=libcis   arch=x86_64   size=92 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000233 <cs_str_compare>:
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:36
 233:	f3 0f 1e fa          	endbr64
 237:	55                   	push   %rbp
 238:	53                   	push   %rbx
 239:	51                   	push   %rcx
 23a:	48 8b 3f             	mov    (%rdi),%rdi
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:636
 23d:	48 85 ff             	test   %rdi,%rdi
 240:	74 06                	je     248 <cs_str_compare+0x15>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:637 (discriminator 1)
 242:	48 8b 5f f8          	mov    -0x8(%rdi),%rbx
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:740
 246:	eb 09                	jmp    251 <cs_str_compare+0x1e>
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:636 (discriminator 1)
 248:	31 db                	xor    %ebx,%ebx
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:741 (discriminator 2)
 24a:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 251 <cs_str_compare+0x1e>
_ZNSt17basic_string_viewIcSt11char_traitsIcEEC4EPKcm():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:603
 251:	48 8b 36             	mov    (%rsi),%rsi
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:636
 254:	48 85 f6             	test   %rsi,%rsi
 257:	74 06                	je     25f <cs_str_compare+0x2c>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:637 (discriminator 1)
 259:	48 8b 6e f8          	mov    -0x8(%rsi),%rbp
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:740
 25d:	eb 09                	jmp    268 <cs_str_compare+0x35>
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:636 (discriminator 1)
 25f:	31 ed                	xor    %ebp,%ebp
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:741 (discriminator 2)
 261:	48 8d 35 00 00 00 00 	lea    0x0(%rip),%rsi        # 268 <cs_str_compare+0x35>
_ZNSt6detail6sv_minImEET_S1_S1_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:41
 268:	48 39 eb             	cmp    %rbp,%rbx
 26b:	48 89 ea             	mov    %rbp,%rdx
 26e:	48 0f 46 d3          	cmovbe %rbx,%rdx
_ZNSt11char_traitsIcE7compareEPKcS2_m():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:98
 272:	e8 00 00 00 00       	call   277 <cs_str_compare+0x44>
_ZNKSt17basic_string_viewIcSt11char_traitsIcEE7compareES2_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:696
 277:	85 c0                	test   %eax,%eax
 279:	75 10                	jne    28b <cs_str_compare+0x58>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:697 (discriminator 1)
 27b:	83 c8 ff             	or     $0xffffffff,%eax
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:697
 27e:	48 39 eb             	cmp    %rbp,%rbx
 281:	72 08                	jb     28b <cs_str_compare+0x58>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:698
 283:	31 c0                	xor    %eax,%eax
 285:	48 39 dd             	cmp    %rbx,%rbp
 288:	0f 92 c0             	setb   %al
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:36
 28b:	5a                   	pop    %rdx
 28c:	5b                   	pop    %rbx
 28d:	5d                   	pop    %rbp
 28e:	c3                   	ret

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
