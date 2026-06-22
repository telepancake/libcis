; call site: cs_vec_erase_H   lib=libcis   arch=x86_64   size=63 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

000000000000020c <cs_vec_erase_H>:
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 20c:	f3 0f 1e fa          	endbr64
 210:	50                   	push   %rax
_ZNSt6vectorI1HSaIS0_EE5beginEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:658
 211:	48 8b 0f             	mov    (%rdi),%rcx
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 214:	48 89 fe             	mov    %rdi,%rsi
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:892
 217:	48 89 cf             	mov    %rcx,%rdi
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375 (discriminator 2)
 21a:	4c 8d 41 20          	lea    0x20(%rcx),%r8
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:892
 21e:	e8 37 fe ff ff       	call   5a <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375 (discriminator 2)
 223:	57                   	push   %rdi
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32 (discriminator 1)
 224:	48 8d 51 10          	lea    0x10(%rcx),%rdx
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375 (discriminator 2)
 228:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 22f <cs_vec_erase_H+0x23>
 22f:	6a f0                	push   $0xfffffffffffffff0
 231:	48 89 d1             	mov    %rdx,%rcx
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:892
 234:	49 89 c1             	mov    %rax,%r9
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:375 (discriminator 2)
 237:	48 89 f2             	mov    %rsi,%rdx
 23a:	48 8d 35 00 00 00 00 	lea    0x0(%rip),%rsi        # 241 <cs_vec_erase_H+0x35>
 241:	e8 00 00 00 00       	call   246 <cs_vec_erase_H+0x3a>
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:32
 246:	48 83 c4 18          	add    $0x18,%rsp
 24a:	c3                   	ret

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
