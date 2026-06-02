"""Tests for domain obfuscation name-mapping (offline)."""

from __future__ import annotations

import random

from planbench import obfuscation
from planbench.obfuscation import random_as_mapping, random_mapping


def test_random_as_mapping_covers_all_symbols(bw_config):
    random.seed(0)
    action_map, predicate_map = random_as_mapping(bw_config)
    assert set(action_map) == set(bw_config["actions"])
    assert set(predicate_map) == set(bw_config["predicates"])
    # all_as mode replaces every name with a run of 'a's
    for mapped in list(action_map.values()) + list(predicate_map.values()):
        assert set(mapped) == {"a"}
    # mappings are collision-free across the whole domain vocabulary
    all_mapped = list(action_map.values()) + list(predicate_map.values())
    assert len(all_mapped) == len(set(all_mapped))


def test_random_mapping_covers_all_symbols(bw_config):
    random.seed(0)
    # random_mapping consumes from the module-level ALL_WORDS pool; populate it.
    obfuscation.ALL_WORDS.clear()
    obfuscation.ALL_WORDS.extend(f"w{i:03d}" for i in range(100))
    action_map, predicate_map = random_mapping(bw_config)
    assert set(action_map) == set(bw_config["actions"])
    assert set(predicate_map) == set(bw_config["predicates"])
    all_mapped = list(action_map.values()) + list(predicate_map.values())
    assert len(all_mapped) == len(set(all_mapped))  # no reused words
