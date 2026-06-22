; call site: cs_vec_end_i   lib=libcis   arch=x86_64   size=24 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/x86_64/libcis.o:     file format elf64-x86-64


Disassembly of section .text:

00000000000002c6 <cs_vec_end_i>:
cs_vec_end_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:50
 2c6:	f3 0f 1e fa          	endbr64
_ZNSt6vectorIiSaIiEE3endEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:660
 2ca:	48 8b 07             	mov    (%rdi),%rax
_ZNKSt6vectorIiSaIiEE8end_ptr_Ev():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:113
 2cd:	48 85 c0             	test   %rax,%rax
 2d0:	74 0b                	je     2dd <cs_vec_end_i+0x17>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/vector:114 (discriminator 1)
 2d2:	48 8b 50 f8          	mov    -0x8(%rax),%rdx
 2d6:	48 83 e2 fc          	and    $0xfffffffffffffffc,%rdx
 2da:	48 01 d0             	add    %rdx,%rax
cs_vec_end_i():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:50
 2dd:	c3                   	ret

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
