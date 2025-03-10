from typing import Dict, List, Set
from pathlib import Path
import logging

class ProtectionFeatureRegistry:
    """Manages protection feature registration and dependencies"""
    
    def __init__(self):
        self._features: Dict[str, Dict] = {
            "webrtc": {
                "enabled": True,
                "path": "components/ungoogled/webrtc/protection",
                "dependencies": ["network", "hardware"]
            },
            "fonts": {
                "enabled": True,
                "path": "components/ungoogled/font/protection",
                "dependencies": ["hardware"]
            },
            "hardware": {
                "enabled": True,
                "path": "components/ungoogled/hardware/protection",
                "dependencies": ["quantum_entropy"]
            },
            "quantum_entropy": {
                "enabled": True,
                "path": "components/ungoogled/entropy/quantum_generator",
                "dependencies": []
            },
            "deep_learning": {
                "enabled": True,
                "path": "components/ungoogled/deep_learning/protection_engine",
                "dependencies": ["behavioral_simulation"]
            },
            "behavioral_simulation": {
                "enabled": True,
                "path": "components/ungoogled/behavior/pattern_simulator",
                "dependencies": []
            }
        }
        self._initialized = False

    def validate_dependencies(self) -> bool:
        """Validate all feature dependencies are met"""
        for name, feature in self._features.items():
            for dep in feature["dependencies"]:
                if dep not in self._features:
                    logging.error(f"Missing dependency {dep} for feature {name}")
                    return False
        return True

    def get_enabled_features(self) -> Set[str]:
        """Get list of all enabled features"""
        return {name for name, feature in self._features.items() 
                if feature["enabled"]}

    def initialize(self) -> bool:
        """Initialize all registered features"""
        if not self.validate_dependencies():
            return False
            
        self._initialized = True
        return True
