"""
Root components package for Enhanced Ungoogled Chromium
"""

from .ungoogled import (
    ProtectionSystemManager,
    ProtectionFeatureRegistry,
    UnifiedProtection
)

__version__ = '1.0.0'

__all__ = [
    'ProtectionSystemManager',
    'ProtectionFeatureRegistry',
    'UnifiedProtection'
]
