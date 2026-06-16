# Working in libcis as Claude

Repository-specific guidance for any Claude session driving work here.

## Authoritative docs

- `CONVENTIONS.md` — style, target, porting contract, build/test/sources of truth.
- `README.md` — the build/test pipeline (`make` front door) and the discriminator
  backends used to attribute failures.
- `bench/code-size-techniques.md` — the catalog for `bench/codesize.py` work.

Read those first when a task touches their domain. Nothing below contradicts
them.

## Working with subagents

Treat subagents as colleagues you brief and then review. The job is to ship
the right diff, not to pre-prevent every wrong one through prompt walls.

### How to brief

- **State the goal and the rationale, not a fence.** "Use the common
  `type_ops` — there must be exactly one `make_type_ops` in the tree" is an
  invariant the agent can reason from. "Do not touch `bits/type_ops.h`" is a
  fence that, if the right fix is in that file, forces the agent to either
  give up or smuggle a workaround past you.
- **Authorize fixing blockers.** If the agent has to clear an unrelated bug to
  verify its own work, that fix is part of the job. Ask for it in its own
  commit so it can be reviewed (and reverted) independently — that's what the
  isolation buys you, not the prompt restriction.
- **Name the invariants that matter.** Design invariants ("one ops table",
  "no exceptions, no RTTI", "constexpr containers are out") belong in the
  brief. Process minutiae ("don't add comments", "don't run more than N
  searches") usually do not.
- **Be explicit about honesty expectations.** Numbers must be measured.
  Negative results must be reported as negative. No spin against the prior
  attempt. State what every number is *of*.

### How to review

- **Read the diff. The diff is the gate, not the prompt.** If something is
  unacceptable in what the agent shipped, take it out — don't try to write a
  retroactive prompt that would have prevented it. Examples of "ship only
  what survives review":
  - The vector hand-rolled `type_ops` table that shipped in `45f38b2`
    despite `bits/type_ops.h::make_type_ops` already existing — the right
    response is to revert that hunk and fix `make_type_ops`, not to scold
    the agent.
  - The `transfer_batch.py` libclang-init fix in the same session was the
    *correct* call: blocker for the agent's own verification, isolated in a
    flagged commit. Accept it.
- **Negative results are signal, not failure.** A measured "this doesn't pay
  off" is a finished task. Push back if the agent frames a negative as
  predetermined (e.g. anchoring on a disowned prior session's result); do
  not push back on the result itself.
- **Trust-but-verify the agent's framing.** Agents have a documented hidden-
  agenda failure mode — claiming the result that justifies their preferred
  change. Numbers, FAIL counts, and diffs are checkable; reread them. The
  agent's narrative is checkable against them.

### Where prompt rules ARE worth writing

A short whitelist; everything else is review territory.

- **Design invariants** the codebase enforces (the four in CONVENTIONS.md;
  "one ops table"; "constexpr container support is dropped"; "no per-test
  source edits — the transfer adapts").
- **The branch to push to**, and "no PR unless asked".
- **What to measure and how** (the exact gate commands), because if the
  agent measures the wrong thing the result is unrecoverable noise.
- **Don't run destructive commands** that the parent session would also
  refuse (`git push --force`, `git reset --hard` on shared refs, etc.).

### What a brief looks like in practice

The Task 1 prompt for the type-erasure-round-2 cleanup, condensed:

> Vector currently forks the ops table inside `vector::ops_table_()`. The
> stated reason was a body-instantiation bug in `&ops_for<T,A>` for types
> like `tzdb`. If that's a real bug, fix `make_type_ops`'s gate with body-
> level `requires`. Then delete the hand-roll and use `&detail::ops_for`.
> Invariant: after this, `grep "type_ops o{" include/` shows one site.

That brief names the goal (one ops vocabulary), the suspected root cause
(gate too permissive), the fix shape (body-level `requires`), and the
checkable invariant. It doesn't tell the agent which files NOT to edit.

## Codebase-specific reminders

- Test infrastructure: `make test` is the front door. `tools/run_files.py
  <subtree>` is the per-file gate (slower, isolates compile failures).
- Single-ELF link of the suite is supported (`make test-single` /
  `make single`); the per-group flow stays for attribution.
- Constexpr-container tests can be deferred at transfer time with
  `LIBCIS_SKIP_CONSTEXPR=1` (strips file-scope `static_assert(IDENT())`
  lines only; trait `static_assert`s untouched).
- `bench/codesize.py` is the size measurement; `bench/record.py "desc"` +
  `bench/record.py --diff` is the journal. Size claims without a journal
  entry are not claims.
- Reusable type-erasure primitives live in `include/bits/type_ops.h`
  (`type_ops`, `cross_ops`, `realloc_op`) and `include/bits/relocatable.h`.
  Containers consume them, they do not redefine them.
