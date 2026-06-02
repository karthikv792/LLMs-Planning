"""Obfuscation package for PlanBench.

Provides PDDL domain and instance obfuscation with three modes:
deceptive (WordNet words), randomized (alphanumeric), and all_as (variable 'a' chars).
"""

from .obfuscator import (
    random_mapping,
    random_as_mapping,
    create_new_config,
    translate_domain_pddl,
    translate_formula,
    replace_instance,
    convert_instances,
    create_obfuscated_instances,
    create_intro_from_translated_domain,
    STATE_TRACKING_TEXT,
    ALL_WORDS,
)

__all__ = [
    "random_mapping",
    "random_as_mapping",
    "create_new_config",
    "translate_domain_pddl",
    "translate_formula",
    "replace_instance",
    "convert_instances",
    "create_obfuscated_instances",
    "create_intro_from_translated_domain",
    "STATE_TRACKING_TEXT",
    "ALL_WORDS",
]
