"""
Root components package for Enhanced Ungoogled Chromium
"""

from .ungoogled.core.feature_registry import ProtectionFeatureRegistry
from .ungoogled.core.protection_system_manager import ProtectionSystemManager

__all__ = ['ProtectionFeatureRegistry', 'ProtectionSystemManager']
