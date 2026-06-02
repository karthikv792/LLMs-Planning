"""Tests for instance generators.

Pure dedup/validation logic and the (pure-Python) generalization generator are
tested offline and hermetically.  The standard blocksworld generator shells out
to a compiled ``pddlgenerators`` binary and writes into the dataset directory,
so it is exercised manually rather than in CI.
"""

from __future__ import annotations

import yaml

from planbench.generators import (
    BlocksworldInstanceGenerator,
    BlocksworldGeneralizationGenerator,
)


def _bw_config_dict():
    from conftest import DATA_DIR

    with open(DATA_DIR / "configs" / "blocksworld.yaml") as f:
        return yaml.safe_load(f)


def test_generator_constructs(in_data_dir):
    gen = BlocksworldInstanceGenerator("configs/blocksworld.yaml", seed=10)
    assert gen.data["domain_name"] == "blocksworld"
    assert isinstance(gen.hashset, set)


def test_convert_pddl_is_stable_hash(in_data_dir):
    """convert_pddl hashes by sorted init+goal, ignoring order/whitespace."""
    gen = BlocksworldInstanceGenerator("configs/blocksworld.yaml", seed=10)
    pddl_a = "(:init\n(clear a)\n(ontable a)\n(handempty)\n)\n(:goal\n(and\n(on a b)\n)\n)"
    pddl_b = "(:init\n(ontable a)\n(handempty)\n(clear a)\n)\n(:goal\n(and\n(on a b)\n)\n)"
    pddl_c = "(:init\n(clear b)\n(ontable b)\n(handempty)\n)\n(:goal\n(and\n(on b a)\n)\n)"
    h_a, h_b, h_c = gen.convert_pddl(pddl_a), gen.convert_pddl(pddl_b), gen.convert_pddl(pddl_c)
    assert h_a == h_b          # order/whitespace-insensitive
    assert h_a != h_c          # genuinely different instances differ
    assert len(h_a) == 32      # md5 hexdigest


def test_instance_ok_real_instance(in_data_dir, bw_paths):
    """A shipped instance has a non-trivial goal."""
    gen = BlocksworldInstanceGenerator("configs/blocksworld.yaml", seed=10)
    assert gen.instance_ok(bw_paths["domain"], bw_paths["instance"]) is True


def test_generalization_generation_is_hermetic(tmp_path, monkeypatch):
    """End-to-end (pure-Python) generation into an isolated tmp directory."""
    cfg = _bw_config_dict()
    cfg["generalized_instance_dir"] = "blocksworld/gen_test"
    cfg["instances_template"] = "instance-{}.pddl"
    cfg["n_instances"] = 3
    cfg_path = tmp_path / "bw_gen.yaml"
    cfg_path.write_text(yaml.safe_dump(cfg))

    monkeypatch.chdir(tmp_path)
    gen = BlocksworldGeneralizationGenerator(str(cfg_path))
    gen.t5_gen_generalization_instances(n_instances=3)

    out_dir = tmp_path / "instances" / "blocksworld" / "gen_test"
    produced = list(out_dir.glob("instance-*.pddl"))
    assert produced, "no generalization instances were generated"
    text = produced[0].read_text()
    assert "(:goal" in text and "(on " in text
