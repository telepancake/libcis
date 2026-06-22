; call site: cs_vec_erase_H   lib=libcis   arch=x86_64   size=63 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

00000000000001f4 <cs_vec_erase_H>:
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 1f4:	f3 0f 1e fa          	endbr64
 1f8:	50                   	push   %rax
_ZNSt6vectorI1HSaIS0_EE5beginEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:658
 1f9:	48 8b 0f             	mov    (%rdi),%rcx
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 1fc:	48 89 fe             	mov    %rdi,%rsi
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:889
 1ff:	48 89 cf             	mov    %rcx,%rdi
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375 (discriminator 2)
 202:	4c 8d 41 20          	lea    0x20(%rcx),%r8
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:889
 206:	e8 47 fe ff ff       	call   52 <_ZNKSt6vectorI1HSaIS0_EE8end_ptr_Ev.isra.0>
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375 (discriminator 2)
 20b:	57                   	push   %rdi
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32 (discriminator 1)
 20c:	48 8d 51 10          	lea    0x10(%rcx),%rdx
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375 (discriminator 2)
 210:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 217 <cs_vec_erase_H+0x23>
 217:	6a f0                	push   $0xfffffffffffffff0
 219:	48 89 d1             	mov    %rdx,%rcx
_ZNSt6vectorI1HSaIS0_EE5eraseEPKS0_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:889
 21c:	49 89 c1             	mov    %rax,%r9
_ZNSt6vectorI1HSaIS0_EE7rotate_EPS0_S3_S3_l():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:375 (discriminator 2)
 21f:	48 89 f2             	mov    %rsi,%rdx
 222:	48 8d 35 00 00 00 00 	lea    0x0(%rip),%rsi        # 229 <cs_vec_erase_H+0x35>
 229:	e8 00 00 00 00       	call   22e <cs_vec_erase_H+0x3a>
cs_vec_erase_H():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:32
 22e:	48 83 c4 18          	add    $0x18,%rsp
 232:	c3                   	ret

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
