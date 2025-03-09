#!/usr/bin/env python3

import sys
import logging
from pathlib import Path
from typing import Dict, List

class BuildValidator:
    def __init__(self):
        self.required_features = {
            "enable_fingerprinting_protection": True,
            "enable_advanced_timing_protection": True,
            "enable_behavior_simulation": True,
            "enable_automation_resistance": True
        }
        
    def validate_gn_flags(self, flags_path: Path, required_features: Dict[str, bool]) -> bool:
        with open(flags_path, 'r') as f:
            content = f.read()
            
        for feature, value in required_features.items():
            if f"{feature}={str(value).lower()}" not in content:
                logging.error(f"Missing required feature: {feature}")
                return False
        return True
        
    def validate_build_deps(self) -> bool:
        required_tools = ['ninja', 'gn', 'python3', 'ccache']
        missing_tools = []
        
        for tool in required_tools:
            if not self._check_tool_exists(tool):
                missing_tools.append(tool)
                
        if missing_tools:
            logging.error(f"Missing required tools: {', '.join(missing_tools)}")
            return False
        return True
    
    def _check_tool_exists(self, tool: str) -> bool:
        from shutil import which
        return which(tool) is not None

def validate_full_build():
    validator = BuildValidator()
    
    # Check all required features
    required_features = {
        "enable_fingerprinting_protection": True,
        "enable_advanced_timing_protection": True,
        "enable_behavior_simulation": True,
        "enable_automation_resistance": True,
        "enable_webrtc_protection": True,
        "enable_font_protection": True,
        "enable_hardware_protection": True,
        "enable_media_protection": True,
        "enable_quantum_entropy": True,
        "enable_deep_learning_protection": True,
        "enable_metamorphic_generation": True
    }
    
    # Validate GN flags
    if not validator.validate_gn_flags(Path('flags.gn'), required_features):
        logging.error("Missing required build features")
        return False
        
    # Validate all protection components
    if not validator.validate_protection_components():
        logging.error("Missing required protection components")
        return False
        
    return True

def main():
    logging.basicConfig(level=logging.INFO)
    
    if not validate_full_build():
        sys.exit(1)
    
    logging.info("Full build configuration validated successfully")
    sys.exit(0)

if __name__ == "__main__":
    main()
