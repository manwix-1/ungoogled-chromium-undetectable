"""
Ungoogled Chromium protection components
"""

from .core.feature_registry import ProtectionFeatureRegistry
from .core.protection_system_manager import ProtectionSystemManager
from .core_automation.unified_protection import UnifiedProtection

__all__ = [
    'ProtectionFeatureRegistry',
    'ProtectionSystemManager',
    'UnifiedProtection'
]
