; call site: cs_str_compare   lib=libcis   arch=arm64   size=108 bytes
; (source-annotated; this is the actual code emitted at one use)


/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/.test_results/latest/callsites/arm64/libcis.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000284 <cs_str_compare>:
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:36
 284:	a9be7bfd 	stp	x29, x30, [sp, #-32]!
 288:	910003fd 	mov	x29, sp
 28c:	f9400000 	ldr	x0, [x0]
 290:	a90153f3 	stp	x19, x20, [sp, #16]
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:741
 294:	b40001e0 	cbz	x0, 2d0 <cs_str_compare+0x4c>
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:637 (discriminator 1)
 298:	f85f8014 	ldur	x20, [x0, #-8]
_ZNSt17basic_string_viewIcSt11char_traitsIcEEC4EPKcm():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:603
 29c:	f9400021 	ldr	x1, [x1]
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:741
 2a0:	b4000201 	cbz	x1, 2e0 <cs_str_compare+0x5c>
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:637 (discriminator 1)
 2a4:	f85f8033 	ldur	x19, [x1, #-8]
_ZNSt6detail6sv_minImEET_S1_S1_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:41
 2a8:	eb13029f 	cmp	x20, x19
_ZNSt11char_traitsIcE7compareEPKcS2_m():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:98
 2ac:	9a939282 	csel	x2, x20, x19, ls	// ls = plast
 2b0:	94000000 	bl	0 <memcmp>
_ZNKSt17basic_string_viewIcSt11char_traitsIcEE7compareES2_():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:696
 2b4:	35000080 	cbnz	w0, 2c4 <cs_str_compare+0x40>
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:697
 2b8:	eb13029f 	cmp	x20, x19
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string_view:698
 2bc:	1a9f97e0 	cset	w0, hi	// hi = pmore
 2c0:	5a9f2000 	csinv	w0, w0, wzr, cs	// cs = hs, nlast
cs_str_compare():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/bench/callsites.cpp:36
 2c4:	a94153f3 	ldp	x19, x20, [sp, #16]
 2c8:	a8c27bfd 	ldp	x29, x30, [sp], #32
 2cc:	d65f03c0 	ret
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:741 (discriminator 2)
 2d0:	90000000 	adrp	x0, 0 <_ZNKSs8capacityEv.isra.0>
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:636 (discriminator 1)
 2d4:	d2800014 	mov	x20, #0x0                   	// #0
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:741 (discriminator 2)
 2d8:	91000000 	add	x0, x0, #0x0
 2dc:	17fffff0 	b	29c <cs_str_compare+0x18>
 2e0:	90000001 	adrp	x1, 0 <_ZNKSs8capacityEv.isra.0>
_ZNKSs4sizeEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:636 (discriminator 1)
 2e4:	d2800013 	mov	x19, #0x0                   	// #0
_ZNKSs4dataEv():
/home/user/libcis/.claude/worktrees/agent-af79cffdad00a4237/include/string:741 (discriminator 2)
 2e8:	91000021 	add	x1, x1, #0x0
 2ec:	17ffffef 	b	2a8 <cs_str_compare+0x24>

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
