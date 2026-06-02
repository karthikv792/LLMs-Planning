# Adding a new PDDL domain

This guide walks through adding a new planning domain to PlanBench so you can evaluate LLMs on it.
We use a running example: **Gripper** — a robot with two grippers that must move balls between
rooms (a classic IPC domain).

By the end you will have a domain that works with the natural-language plan-generation tasks
(`t1`, `t1_zero`, …) and the PDDL tasks (`t1_pddl`, …), evaluated soundly with VAL.

> **Read first:** [ARCHITECTURE.md](ARCHITECTURE.md), especially "The translation layer". The single
> most important thing to understand is that PlanBench renders PDDL into natural language and parses
> the model's natural-language answer back into PDDL — and **both directions are domain-aware.**

## What a domain needs

| # | Artifact | Where | Required for |
|---|----------|-------|--------------|
| 1 | Domain PDDL file | `data/instances/<domain>/<domain>_domain.pddl` | everything |
| 2 | Problem instances (PDDL) | `data/instances/<domain>/...` | everything |
| 3 | Config YAML | `data/configs/<domain>.yaml` | everything |
| 4 | Forward translation branch (PDDL → NL) | `translation/pddl_to_text.py` | NL tasks (`t1`, `t1_zero`, `t1_cot`, `t2`, `t4`–`t8`) |
| 5 | Backward translation branch (NL → PDDL) | `translation/text_to_pddl.py` | NL tasks |
| 6 | Instance generator (optional) | `generators/<domain>.py` + `cli.py` | only if you want `planbench generate` |

If you only care about the **PDDL-in / PDDL-out** tasks (`t1_pddl`, `t1_zero_pddl`), you can skip
steps 4–5: those tasks feed raw PDDL to the model and validate raw PDDL back, so no
natural-language translation is involved.

---

## Step 1 — Add the domain PDDL file

Drop your domain definition in `data/instances/gripper/gripper_domain.pddl`:

```lisp
(define (domain gripper)
  (:requirements :strips)
  (:predicates (room ?r) (ball ?b) (gripper ?g)
               (at-robby ?r) (at ?b ?r) (free ?g) (carry ?o ?g))
  (:action move
     :parameters (?from ?to)
     :precondition (and (room ?from) (room ?to) (at-robby ?from))
     :effect (and (at-robby ?to) (not (at-robby ?from))))
  (:action pick
     :parameters (?obj ?room ?gripper)
     :precondition (and (ball ?obj) (room ?room) (gripper ?gripper)
                        (at ?obj ?room) (at-robby ?room) (free ?gripper))
     :effect (and (carry ?obj ?gripper) (not (at ?obj ?room)) (not (free ?gripper))))
  (:action drop
     :parameters (?obj ?room ?gripper)
     :precondition (and (ball ?obj) (room ?room) (gripper ?gripper)
                        (carry ?obj ?gripper) (at-robby ?room))
     :effect (and (at ?obj ?room) (free ?gripper) (not (carry ?obj ?gripper)))))
```

Sanity-check that PlanBench can parse it:

```python
from planbench.model_parser import parse_model
m = parse_model("data/instances/gripper/gripper_domain.pddl",
                "data/instances/gripper/instance-1.pddl")
print(sorted(m["pred"]))
```

## Step 2 — Add problem instances

Put instances at `data/instances/gripper/instance-{}.pddl` (the `{}` is the instance id). A minimal
instance:

```lisp
(define (problem gripper-1) (:domain gripper)
  (:objects rooma roomb ball1 ball2 left right)
  (:init (room rooma) (room roomb) (ball ball1) (ball ball2)
         (gripper left) (gripper right) (free left) (free right)
         (at-robby rooma) (at ball1 rooma) (at ball2 rooma))
  (:goal (and (at ball1 roomb) (at ball2 roomb))))
```

You can author these by hand, copy them from an IPC distribution, or write a generator (Step 6).
Make sure every instance is **solvable** and has a **non-trivial goal** (the generators' `instance_ok`
check enforces this — a goal already true in the initial state is rejected).

To produce ground-truth optimal plans for cost-aware tasks (`t2`), set `$FAST_DOWNWARD` so the
pipeline can call Fast Downward.

## Step 3 — Write the config YAML

Create `data/configs/gripper.yaml`. Required keys are validated by
`DomainConfig.from_yaml`; the rest drive the natural-language rendering.

```yaml
# ── required ──────────────────────────────────────────────
domain_name: gripper                       # MUST match your translation branch keys
domain_file: gripper/gripper_domain.pddl   # relative to data/instances/
instance_dir: gripper                      # relative to data/instances/
instances_template: instance-{}.pddl
n_instances: 100
start: 1
end: 100

# ── natural-language rendering ────────────────────────────
domain_intro: |
  A robot with a left gripper and a right gripper moves balls between rooms.
  Here are the actions I can do:

  Move from one room to another.
  Pick up a ball in a room with a free gripper.
  Drop a ball it is carrying in a room.

  Restrictions:
  The robot can only pick up a ball if a gripper is free and the ball and robot are in the same room.
  The robot can only drop a ball it is currently carrying.

# how each PREDICATE is verbalised; {} placeholders are filled with object names
predicates:
  at-robby: "the robot is in the {}"
  at: "{} is in the {}"
  free: "the {} gripper is free"
  carry: "the robot is carrying {} in the {} gripper"

# how each ACTION is verbalised (key = PDDL action name)
actions:
  move: "move from the {} to the {}"
  pick: "pick up {} in the {} with the {} gripper"
  drop: "drop {} in the {} with the {} gripper"

# PDDL symbol → human-readable name used in prompts AND for parsing answers back
encoded_objects:
  rooma: "room A"
  roomb: "room B"
  ball1: "ball 1"
  ball2: "ball 2"
  left: "left"
  right: "right"
```

Optional keys you may add later: `domain_intro_zero_shot`, `domain_intro_state_tracking`
(for `t1_cot`), `domain_intro_cost` (for `t2`), `generalized_instance_dir` (for `t5`),
`encoded_objects_compact`, and the `action_obfuscation`/`predicate_obfuscation` mappings for
obfuscated variants. See `data/configs/blocksworld.yaml` for a fully-featured example.

Verify the config loads:

```python
from planbench.config import DomainConfig
cfg = DomainConfig.from_yaml("data/configs/gripper.yaml")
```

## Step 4 — Forward translation (PDDL → natural language)

`translation/pddl_to_text.py` turns a parsed instance into the prompt's INIT/GOAL text. Three
functions branch on `data['domain_name']` and need a `gripper` case:

- `parse_problem(...)` — the inner `parse()` builds each predicate's phrase.
- `fill_template(...)` — assembles the final INIT/GOAL/PLAN block.
- `instance_to_text(...)` — top-level entry; renders objects + predicates.

For many domains the existing **generic, template-driven** rendering (filling `predicates[...]`
with `encoded_objects[...]`) is enough — model your branch on the simplest existing case and only
special-case what your domain needs (e.g. blocksworld's "on the table" phrasing). Add:

```python
# inside instance_to_text(...), alongside the blocksworld/logistics branches
elif 'gripper' in data['domain_name']:
    # usually: reuse the generic predicate-template rendering
    ...
```

> Tip: start by copying the `logistics` branches — logistics, like gripper, has multi-argument
> predicates (`at ?obj ?room`) and reads more generically than blocksworld.

## Step 5 — Backward translation (NL → PDDL)

`translation/text_to_pddl.py` parses the model's natural-language answer back into a grounded PDDL
plan so VAL can score it. Add two things:

1. A `text_to_plan_gripper(text, action_set, plan_file, data, ground_flag=False)` function. Model it
   on `text_to_plan_blocksworld` (around line 423). The contract is:
   - **inputs:** the model's raw text, `action_set` = `problem.actions` (tarski action schemas, used
     to learn each action's arity via `.parameters.vars()`), the `plan_file` path, and the config
     `data`.
   - **behaviour:** map each NL action phrase back to its PDDL action, recover the ordered objects
     via `encoded_objects`, and emit `(action obj1 obj2 ...)` lines.
   - **outputs:** write the plan to `plan_file` and `return plan, readable_plan`.
2. A branch in the `text_to_plan` dispatcher:

   ```python
   elif 'gripper' in data['domain_name']:
       return text_to_plan_gripper(text, action_set, plan_file, data, ground_flag)
   ```

If you use the state-tracking (`t7`) or verification (`t3`) tasks, add a `text_to_state_gripper`
and a branch in `text_to_state` at the `# ADD SPECIFIC TRANSLATION FOR EACH DOMAIN HERE` hook.

> **Naming gotcha:** the dispatchers use substring matching (`'blocksworld' in domain_name`). Pick a
> `domain_name` that doesn't accidentally contain another domain's name.

## Step 6 — (Optional) Instance generator

To support `planbench generate --config gripper`, add a generator under `generators/gripper.py`
subclassing `InstanceGeneratorBase` and implementing `gen_goal_directed_instances(n, max_objs)`
(reuse `convert_pddl` for dedup and `instance_ok` for goal validity), then export it from
`generators/__init__.py` and add a dispatch branch in `cli.py`'s `_cmd_generate`. If you author
instances by hand, you can skip this entirely.

## Step 7 — Validate end-to-end

```bash
cd data

# render a prompt and eyeball the natural language
planbench prompt --task t1 --config gripper --engine gpt-4o_chat --specific_instances 1

# full loop on a couple of instances
export OPENAI_API_KEY=sk-...
export VAL=/path/to/VAL/.../bin
planbench run --task t1 --config gripper --engine gpt-4o_chat --specific_instances 1 2
```

Check `data/results/gripper/gpt-4o_chat/task_1_plan_generation.json` for `llm_correct` flags. The
PDDL-only path needs no translation branches and is a good first smoke test:

```bash
planbench run --task t1_pddl --config gripper --engine gpt-4o_chat --specific_instances 1
```

## Step 8 — Add a regression test

Add your config to the suite so it can't silently break. `tests/test_config.py` already loads every
shipped config; a `gripper.yaml` is picked up automatically. For the translation branches, add a
small test modelled on `tests/test_translation.py` (parse an instance, assert `instance_to_text`
mentions a room/ball, and that `text_to_plan` round-trips a hand-written gripper plan).

Run the suite:

```bash
pip install -e ".[dev]"
pytest
```

---

## Variants you can derive for free

- **Mystery / obfuscated:** `planbench obfuscate --config gripper --obfuscation_type randomized`
  renames actions and predicates to nonsense tokens, producing a new config + instances that test
  whether the model is reasoning or pattern-matching on familiar words.
- **Unsolvable:** include unsolvable instances and prompt the model to detect infeasibility;
  `validate_plan_unsolvable` recognises a "no plan possible" answer.

## Checklist

- [ ] `data/instances/<domain>/<domain>_domain.pddl`
- [ ] `data/instances/<domain>/instance-*.pddl` (solvable, non-trivial goals)
- [ ] `data/configs/<domain>.yaml` (loads via `DomainConfig.from_yaml`)
- [ ] Forward branches in `pddl_to_text.py` (NL tasks)
- [ ] `text_to_plan_<domain>` + dispatcher branch in `text_to_pddl.py` (NL tasks)
- [ ] `text_to_state_<domain>` + branch (only for `t3`/`t7`)
- [ ] (optional) generator + `cli.py` wiring
- [ ] End-to-end run validates with VAL
- [ ] A test covering the config + translation
