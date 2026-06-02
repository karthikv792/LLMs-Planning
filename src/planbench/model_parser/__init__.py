"""PDDL model parser package for PlanBench.

Provides parsing and writing of PDDL domain/problem models.
"""

from .parser import parse_model
from .writer import ModelWriter


class Parser_PDDL:
    def __init__(self, domain_file: str, problem_file: str) -> None:
        self.domain_file = domain_file
        self.problem_file = problem_file

    def parse_PDDL(self) -> dict:
        model = parse_model(self.domain_file, self.problem_file)
        return model

    def write_PDDL(self, model: dict) -> None:
        writer = ModelWriter(model)
        writer.write_model()


__all__ = ["parse_model", "ModelWriter", "Parser_PDDL"]
