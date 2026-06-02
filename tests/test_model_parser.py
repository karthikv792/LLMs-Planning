"""Tests for the tarski-backed PDDL model parser and writer."""

from __future__ import annotations

from planbench.model_parser import Parser_PDDL, parse_model, ModelWriter


def test_parse_model_returns_expected_keys(bw_paths):
    model = parse_model(bw_paths["domain"], bw_paths["instance"])
    for key in ("domain", "instance", "pred", "constants"):
        assert key in model, f"missing key {key!r} in parsed model"
    assert model["domain"]  # domain name non-empty
    assert model["pred"]    # at least one predicate


def test_parser_pddl_wrapper(bw_paths):
    parser = Parser_PDDL(bw_paths["domain"], bw_paths["instance"])
    model = parser.parse_PDDL()
    assert model["instance"]


def test_model_writer_roundtrip(bw_paths, tmp_path):
    """Parse → write → re-parse yields a structurally consistent model."""
    model = parse_model(bw_paths["domain"], bw_paths["instance"])

    out_domain = tmp_path / "domain.pddl"
    out_problem = tmp_path / "problem.pddl"
    ModelWriter(model).write_files(str(out_domain), str(out_problem))

    assert out_domain.exists() and out_domain.stat().st_size > 0
    assert out_problem.exists() and out_problem.stat().st_size > 0

    reparsed = parse_model(str(out_domain), str(out_problem))
    # Predicate vocabulary must survive the round-trip.  Each entry is
    # ``[name, [arg_sorts...]]``; compare the set of predicate names.
    names = {p[0] for p in model["pred"]}
    reparsed_names = {p[0] for p in reparsed["pred"]}
    assert names == reparsed_names
