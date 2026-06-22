; call site: cs_str_compare   lib=libcis   arch=x86_64   size=92 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

000000000000024b <cs_str_compare>:
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:36
 24b:	f3 0f 1e fa          	endbr64
 24f:	55                   	push   %rbp
 250:	53                   	push   %rbx
 251:	51                   	push   %rcx
 252:	48 8b 3f             	mov    (%rdi),%rdi
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:638
 255:	48 85 ff             	test   %rdi,%rdi
 258:	74 06                	je     260 <cs_str_compare+0x15>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:639 (discriminator 1)
 25a:	48 8b 5f f8          	mov    -0x8(%rdi),%rbx
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:742
 25e:	eb 09                	jmp    269 <cs_str_compare+0x1e>
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:638 (discriminator 1)
 260:	31 db                	xor    %ebx,%ebx
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743 (discriminator 2)
 262:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 269 <cs_str_compare+0x1e>
_ZNSt17basic_string_viewIcSt11char_traitsIcEEC4EPKcm():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:603
 269:	48 8b 36             	mov    (%rsi),%rsi
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:638
 26c:	48 85 f6             	test   %rsi,%rsi
 26f:	74 06                	je     277 <cs_str_compare+0x2c>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:639 (discriminator 1)
 271:	48 8b 6e f8          	mov    -0x8(%rsi),%rbp
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:742
 275:	eb 09                	jmp    280 <cs_str_compare+0x35>
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:638 (discriminator 1)
 277:	31 ed                	xor    %ebp,%ebp
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743 (discriminator 2)
 279:	48 8d 35 00 00 00 00 	lea    0x0(%rip),%rsi        # 280 <cs_str_compare+0x35>
_ZNSt6detail6sv_minImEET_S1_S1_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:41
 280:	48 39 eb             	cmp    %rbp,%rbx
 283:	48 89 ea             	mov    %rbp,%rdx
 286:	48 0f 46 d3          	cmovbe %rbx,%rdx
_ZNSt11char_traitsIcE7compareEPKcS2_m():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:98
 28a:	e8 00 00 00 00       	call   28f <cs_str_compare+0x44>
_ZNKSt17basic_string_viewIcSt11char_traitsIcEE7compareES2_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:696
 28f:	85 c0                	test   %eax,%eax
 291:	75 10                	jne    2a3 <cs_str_compare+0x58>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:697 (discriminator 1)
 293:	83 c8 ff             	or     $0xffffffff,%eax
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:697
 296:	48 39 eb             	cmp    %rbp,%rbx
 299:	72 08                	jb     2a3 <cs_str_compare+0x58>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:698
 29b:	31 c0                	xor    %eax,%eax
 29d:	48 39 dd             	cmp    %rbx,%rbp
 2a0:	0f 92 c0             	setb   %al
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:36
 2a3:	5a                   	pop    %rdx
 2a4:	5b                   	pop    %rbx
 2a5:	5d                   	pop    %rbp
 2a6:	c3                   	ret

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
