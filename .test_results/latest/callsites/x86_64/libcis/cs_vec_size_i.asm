; call site: cs_vec_size_i   lib=libcis   arch=x86_64   size=23 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

000000000000028f <cs_vec_size_i>:
cs_vec_size_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:48
 28f:	f3 0f 1e fa          	endbr64
 293:	48 8b 17             	mov    (%rdi),%rdx
_ZNKSt6vectorIiSaIiEE4sizeEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:679 (discriminator 1)
 296:	31 c0                	xor    %eax,%eax
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:679
 298:	48 85 d2             	test   %rdx,%rdx
 29b:	74 08                	je     2a5 <cs_vec_size_i+0x16>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/vector:680 (discriminator 1)
 29d:	48 8b 42 f8          	mov    -0x8(%rdx),%rax
 2a1:	48 c1 e8 02          	shr    $0x2,%rax
cs_vec_size_i():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:48
 2a5:	c3                   	ret

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
