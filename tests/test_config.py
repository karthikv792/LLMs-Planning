"""Tests for planbench.config.DomainConfig."""

from __future__ import annotations

import glob
from pathlib import Path

import pytest
import yaml

from planbench.config import DomainConfig

from conftest import DATA_DIR

# All shipped domain configs (skip obfuscation-output / template artifacts that
# are intentionally partial and not meant to be loaded as full domain configs).
_CONFIG_FILES = sorted(
    p for p in glob.glob(str(DATA_DIR / "configs" / "*.yaml"))
    if Path(p).name not in {"config_mask.yaml", "default_template.yaml"}
)


@pytest.mark.parametrize("config_path", _CONFIG_FILES, ids=lambda p: Path(p).name)
def test_all_shipped_configs_load(config_path):
    """Every shipped domain config loads into a DomainConfig."""
    cfg = DomainConfig.from_yaml(config_path)
    assert cfg.domain_name
    assert cfg.domain_file
    assert cfg.instance_dir
    assert cfg.n_instances >= 0


def test_missing_required_field_raises(tmp_path):
    bad = tmp_path / "bad.yaml"
    bad.write_text("domain_name: foo\n")  # missing most required fields
    with pytest.raises(ValueError, match="missing required fields"):
        DomainConfig.from_yaml(bad)


def test_empty_config_raises(tmp_path):
    empty = tmp_path / "empty.yaml"
    empty.write_text("")
    with pytest.raises(ValueError, match="Empty config"):
        DomainConfig.from_yaml(empty)


def test_unknown_keys_are_filtered(tmp_path):
    cfg_path = tmp_path / "x.yaml"
    cfg_path.write_text(
        yaml.safe_dump(
            {
                "domain_name": "d",
                "domain_file": "d/domain.pddl",
                "instance_dir": "d/inst",
                "instances_template": "instance-{}.pddl",
                "n_instances": 3,
                "start": 1,
                "end": 3,
                "some_future_key": "ignored",  # not a dataclass field
            }
        )
    )
    cfg = DomainConfig.from_yaml(cfg_path)
    assert not hasattr(cfg, "some_future_key")
    assert cfg.n_instances == 3


def test_resolve_paths(bw_config_path):
    cfg = DomainConfig.from_yaml(bw_config_path)
    inst = cfg.resolve_instance_path(1, data_root="data")
    dom = cfg.resolve_domain_path(data_root="data")
    assert inst == Path("data/instances") / cfg.instance_dir / "instance-1.pddl"
    assert dom == Path("data/instances") / cfg.domain_file
