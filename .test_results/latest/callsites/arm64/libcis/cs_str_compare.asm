; call site: cs_str_compare   lib=libcis   arch=arm64   size=108 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/.test_results/latest/callsites/arm64/libcis.o:     file format elf64-littleaarch64


Disassembly of section .text:

00000000000002b4 <cs_str_compare>:
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:36
 2b4:	a9be7bfd 	stp	x29, x30, [sp, #-32]!
 2b8:	910003fd 	mov	x29, sp
 2bc:	f9400000 	ldr	x0, [x0]
 2c0:	a90153f3 	stp	x19, x20, [sp, #16]
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743
 2c4:	b40001e0 	cbz	x0, 300 <cs_str_compare+0x4c>
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:639 (discriminator 1)
 2c8:	f85f8014 	ldur	x20, [x0, #-8]
_ZNSt17basic_string_viewIcSt11char_traitsIcEEC4EPKcm():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:603
 2cc:	f9400021 	ldr	x1, [x1]
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743
 2d0:	b4000201 	cbz	x1, 310 <cs_str_compare+0x5c>
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:639 (discriminator 1)
 2d4:	f85f8033 	ldur	x19, [x1, #-8]
_ZNSt6detail6sv_minImEET_S1_S1_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:41
 2d8:	eb13029f 	cmp	x20, x19
_ZNSt11char_traitsIcE7compareEPKcS2_m():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:98
 2dc:	9a939282 	csel	x2, x20, x19, ls	// ls = plast
 2e0:	94000000 	bl	0 <memcmp>
_ZNKSt17basic_string_viewIcSt11char_traitsIcEE7compareES2_():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:696
 2e4:	35000080 	cbnz	w0, 2f4 <cs_str_compare+0x40>
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:697
 2e8:	eb13029f 	cmp	x20, x19
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string_view:698
 2ec:	1a9f97e0 	cset	w0, hi	// hi = pmore
 2f0:	5a9f2000 	csinv	w0, w0, wzr, cs	// cs = hs, nlast
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/bench/callsites.cpp:36
 2f4:	a94153f3 	ldp	x19, x20, [sp, #16]
 2f8:	a8c27bfd 	ldp	x29, x30, [sp], #32
 2fc:	d65f03c0 	ret
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743 (discriminator 2)
 300:	90000000 	adrp	x0, 0 <_ZNKSs8capacityEv.isra.0>
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:638 (discriminator 1)
 304:	d2800014 	mov	x20, #0x0                   	// #0
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743 (discriminator 2)
 308:	91000000 	add	x0, x0, #0x0
 30c:	17fffff0 	b	2cc <cs_str_compare+0x18>
 310:	90000001 	adrp	x1, 0 <_ZNKSs8capacityEv.isra.0>
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:638 (discriminator 1)
 314:	d2800013 	mov	x19, #0x0                   	// #0
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-a7d5311b0eded3df4/include/string:743 (discriminator 2)
 318:	91000021 	add	x1, x1, #0x0
 31c:	17ffffef 	b	2d8 <cs_str_compare+0x24>

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
