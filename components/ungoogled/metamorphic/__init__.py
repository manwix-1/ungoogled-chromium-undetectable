"""
Metamorphic code generation and protection components
"""

from .code_generator import MetamorphicCodeGenerator
from .pattern_transformer import PatternTransformer
from .behavior_simulator import BehaviorSimulator

__all__ = [
    'MetamorphicCodeGenerator',
    'PatternTransformer',
    'BehaviorSimulator'
]
