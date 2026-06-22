; call site: cs_str_compare   lib=libcis   arch=arm32   size=80 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm32/libcis.o:     file format elf32-littlearm


Disassembly of section .text:

000001e8 <cs_str_compare>:
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:36
 1e8:	6800      	ldr	r0, [r0, #0]
 1ea:	b538      	push	{r3, r4, r5, lr}
 1ec:	4b10      	ldr	r3, [pc, #64]	@ (230 <cs_str_compare+0x48>)
 1ee:	447b      	add	r3, pc
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743
 1f0:	b190      	cbz	r0, 218 <cs_str_compare+0x30>
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:639 (discriminator 1)
 1f2:	f850 5c04 	ldr.w	r5, [r0, #-4]
_ZNSt17basic_string_viewIcSt11char_traitsIcEEC4EPKcj():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:603
 1f6:	6809      	ldr	r1, [r1, #0]
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743
 1f8:	b191      	cbz	r1, 220 <cs_str_compare+0x38>
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:639 (discriminator 1)
 1fa:	f851 4c04 	ldr.w	r4, [r1, #-4]
_ZNSt11char_traitsIcE7compareEPKcS2_j():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:98
 1fe:	42a5      	cmp	r5, r4
 200:	462a      	mov	r2, r5
 202:	bf28      	it	cs
 204:	4622      	movcs	r2, r4
 206:	f7ff fffe 	bl	0 <memcmp>
_ZNKSt17basic_string_viewIcSt11char_traitsIcEE7compareES2_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:696
 20a:	b920      	cbnz	r0, 216 <cs_str_compare+0x2e>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:697
 20c:	42a5      	cmp	r5, r4
 20e:	d30b      	bcc.n	228 <cs_str_compare+0x40>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:698
 210:	bf8c      	ite	hi
 212:	2001      	movhi	r0, #1
 214:	2000      	movls	r0, #0
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:36
 216:	bd38      	pop	{r3, r4, r5, pc}
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743 (discriminator 2)
 218:	4a06      	ldr	r2, [pc, #24]	@ (234 <cs_str_compare+0x4c>)
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:638 (discriminator 1)
 21a:	2500      	movs	r5, #0
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743 (discriminator 2)
 21c:	5898      	ldr	r0, [r3, r2]
 21e:	e7ea      	b.n	1f6 <cs_str_compare+0xe>
 220:	4a04      	ldr	r2, [pc, #16]	@ (234 <cs_str_compare+0x4c>)
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:638 (discriminator 1)
 222:	2400      	movs	r4, #0
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743 (discriminator 2)
 224:	5899      	ldr	r1, [r3, r2]
 226:	e7ea      	b.n	1fe <cs_str_compare+0x16>
_ZNKSt17basic_string_viewIcSt11char_traitsIcEE7compareES2_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:697 (discriminator 1)
 228:	f04f 30ff 	mov.w	r0, #4294967295	@ 0xffffffff
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:36
 22c:	e7f3      	b.n	216 <cs_str_compare+0x2e>
 22e:	bf00      	nop
 230:	0000003e 	.word	0x0000003e
 234:	00000000 	.word	0x00000000

Disassembly of section .text._ZNSt6detail17move_construct_opI1HSaIS1_EEEvPvS3_S3_:

Disassembly of section .text._ZN20libcis_atomic_detail15atomic_spinlock4lockEv:

Disassembly of section .text._ZN20libcis_atomic_detail15atomic_spinlock6unlockEv:

Disassembly of section .text._ZNK1HltERKS_:

Disassembly of section .text._ZNSs11vdeallocateEv:

Disassembly of section .text._ZNSt6detail10destroy_opI1HSaIS1_EEEvPvS3_:

Disassembly of section .text._ZNSs9vallocateEj:

Disassembly of section .text._ZNSs9init_copyEPKcj:

Disassembly of section .text._ZN1HaSERKS_.isra.0:

Disassembly of section .text._ZNSt6vectorIiSaIiEE10grow_cold_Ej:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE10grow_cold_Ej:

Disassembly of section .text._ZN1HC2ERKS_:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16construct_at_endIPKS0_S5_EEvT_T0_j.isra.0:

Disassembly of section .text._ZNSt6vectorI1HSaIS0_EE16assign_with_sizeIPKS0_S5_EEvT_T0_j:

Disassembly of section .text._ZSt4swapI1HENSt9enable_ifIXaa23is_move_constructible_vIT_E20is_move_assignable_vIS2_EEvE4typeERS2_S5_:
