"""
Core automation components for protection system
"""

from .controller_manager import ControllerManager
from .fingerprint_orchestrator import FingerprintOrchestrator
from .unified_protection import UnifiedProtection
from .adaptive_protection import AdaptiveProtection
from .protection_orchestrator import ProtectionOrchestrator
from .browser_instance import BrowserInstance

__all__ = [
    'ControllerManager',
    'FingerprintOrchestrator',
    'UnifiedProtection',
    'AdaptiveProtection',
    'ProtectionOrchestrator',
    'BrowserInstance'
]
