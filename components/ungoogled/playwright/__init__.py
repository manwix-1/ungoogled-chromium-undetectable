"""
Playwright integration and automation components
"""

from .playwright_controller import PlaywrightController
from .automation_bridge import AutomationBridge
from .virtual_display import VirtualDisplay

__all__ = [
    'PlaywrightController',
    'AutomationBridge',
    'VirtualDisplay'
]
