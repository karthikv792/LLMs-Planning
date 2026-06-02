"""Domain configuration dataclass for PlanBench.

Loads and validates YAML config files used across all planning benchmark tasks.
Covers all 22 config keys found across both codebases' config files.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

import yaml


@dataclass
class DomainConfig:
    """Configuration for a planning domain.

    Required fields are present in all domain configs. Optional fields
    are used by specific tasks or domain variants.
    """

    # Required: present in all configs
    domain_name: str
    domain_file: str
    instance_dir: str
    instances_template: str
    n_instances: int
    start: int
    end: int

    # Present in most configs but not all (e.g., sokoban)
    domain_intro: str | None = None
    actions: dict[str, str] | None = None
    predicates: dict[str, str] | None = None
    encoded_objects: dict[str, str] | None = None

    # Optional intro variants (used by specific tasks)
    domain_intro_state_tracking: str | None = None  # t1_cot
    domain_intro_zero_shot: str | None = None  # t1_zero
    domain_intro_cost: str | None = None  # t2 (optimality)

    # Optional paths
    generalized_instance_dir: str | None = None  # t5 (generalization)

    # Optional mappings
    predicate_mapping: dict[str, str] | None = None
    encoded_objects_compact: dict[str, str] | None = None

    # Optional obfuscation (obfuscated domain configs)
    action_obfuscation: dict[str, str] | None = None
    predicate_obfuscation: dict[str, str] | None = None

    # Optional metadata
    objects: list[str] | None = None
    callbacks: list[str] | None = None

    @classmethod
    def from_yaml(cls, path: str | Path) -> DomainConfig:
        """Load a DomainConfig from a YAML file.

        Args:
            path: Path to the YAML config file.

        Returns:
            A DomainConfig instance populated from the YAML data.

        Raises:
            FileNotFoundError: If the config file doesn't exist.
            ValueError: If required fields are missing.
        """
        path = Path(path)
        with open(path) as f:
            data: dict[str, Any] = yaml.safe_load(f)

        if data is None:
            raise ValueError(f"Empty config file: {path}")

        required_fields = [
            "domain_name",
            "domain_file",
            "instance_dir",
            "instances_template",
            "n_instances",
            "start",
            "end",
        ]
        missing = [f for f in required_fields if f not in data]
        if missing:
            raise ValueError(
                f"Config {path} missing required fields: {', '.join(missing)}"
            )

        # Filter to only known fields to avoid unexpected kwargs
        known_fields = {f.name for f in cls.__dataclass_fields__.values()}
        filtered = {k: v for k, v in data.items() if k in known_fields}

        return cls(**filtered)

    def resolve_instance_path(
        self, instance_id: int, data_root: str | Path = "data"
    ) -> Path:
        """Resolve the path to a specific problem instance.

        Args:
            instance_id: The instance number.
            data_root: Root directory containing instance files.

        Returns:
            Path to the instance PDDL file.
        """
        data_root = Path(data_root)
        instance_file = self.instances_template.format(instance_id)
        return data_root / "instances" / self.instance_dir / instance_file

    def resolve_domain_path(self, data_root: str | Path = "data") -> Path:
        """Resolve the path to the domain PDDL file.

        Args:
            data_root: Root directory containing domain files.

        Returns:
            Path to the domain PDDL file.
        """
        data_root = Path(data_root)
        return data_root / "instances" / self.domain_file
