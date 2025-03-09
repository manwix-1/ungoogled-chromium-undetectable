#!/usr/bin/env python3

import os
import json
from pathlib import Path
from typing import Dict, List, Set

class DependencyValidator:
    def __init__(self):
        self.required_components = {
            'core': [
                'components/ungoogled/core_automation/controller_manager.*',
                'components/ungoogled/core_automation/fingerprint_orchestrator.*',
                'components/ungoogled/core_automation/unified_protection.*',
                'components/ungoogled/core_automation/v8_protection.*',
                'components/ungoogled/core_automation/launch_protocol.*'
            ],
            'protection': [
                'components/ungoogled/webrtc/protection.*',
                'components/ungoogled/font/protection.*',
                'components/ungoogled/hardware/protection.*',
                'components/ungoogled/media/protection.*',
                'components/ungoogled/network/protection.*'
            ],
            'advanced': [
                'components/ungoogled/entropy/quantum_generator.*',
                'components/ungoogled/behavior/pattern_simulator.*',
                'components/ungoogled/deep_learning/protection_engine.*',
                'components/ungoogled/metamorphic/code_generator.*'
            ],
            'build': [
                'build/config.gn',
                'build/main.py',
                'build/config_manager.py',
                'build/validate_build_config.py'
            ]
        }
        self.required_components.update({
            'automation': [
                'components/ungoogled/playwright/playwright_controller.*',
                'components/ungoogled/playwright/virtual_display.*',
                'components/ungoogled/playwright/automation_bridge.*'
            ],
            'cursor': [
                'components/ungoogled/cursor/simulation.*',
                'components/ungoogled/cursor/behavior.*',
                'components/ungoogled/cursor/interaction.*'
            ]
        })
        
    def validate_all_dependencies(self, root_path: Path) -> bool:
        missing_files = []
        for category, files in self.required_components.items():
            for file_pattern in files:
                if not self._find_file(root_path, file_pattern):
                    missing_files.append(file_pattern)
        
        if missing_files:
            print("Missing required files:")
            for file in missing_files:
                print(f"  - {file}")
            return False
        return True

    def validate_build_configuration(self, config_path: Path) -> bool:
        required_features = {
            'enable_webrtc_protection',
            'enable_font_protection',
            'enable_hardware_protection',
            'enable_media_protection',
            'enable_network_protection',
            'enable_quantum_entropy',
            'enable_behavioral_simulation',
            'enable_deep_learning_protection',
            'enable_metamorphic_generation'
        }
        
        with open(config_path) as f:
            config = json.load(f)
            
        missing_features = required_features - set(config.keys())
        if missing_features:
            print("Missing required features in build config:")
            for feature in missing_features:
                print(f"  - {feature}")
            return False
        return True

    def _find_file(self, root_path: Path, pattern: str) -> bool:
        """
        Find files matching the given pattern under root_path
        """
        import fnmatch
        
        # Convert pattern to parts
        pattern_parts = pattern.split('/')
        current_path = root_path

        # Navigate through directories in pattern
        for i, part in enumerate(pattern_parts[:-1]):
            if not current_path.is_dir():
                return False
            
            # Find matching directory
            found = False
            for item in current_path.iterdir():
                if item.is_dir() and fnmatch.fnmatch(item.name, part):
                    current_path = item
                    found = True
                    break
                
            if not found:
                return False

        # Match final file pattern
        file_pattern = pattern_parts[-1]
        if current_path.is_dir():
            for item in current_path.iterdir():
                if fnmatch.fnmatch(item.name, file_pattern):
                    return True
                
        return False

    def _validate_file_contents(self, file_path: Path) -> bool:
        """
        Validate file contents for required components
        """
        required_patterns = {
            '.h': ['class', 'struct', 'namespace'],
            '.cc': ['#include', 'namespace'],
            '.py': ['class', 'def'],
            '.gn': ['sources', 'deps'],
        }
        
        extension = file_path.suffix
        if extension not in required_patterns:
            return True
        
        with open(file_path, 'r') as f:
            content = f.read()
        
        return all(pattern in content for pattern in required_patterns[extension])

    def validate_protection_components(self) -> bool:
        # Verify all protection components are properly integrated
        protection_paths = [
            'components/ungoogled/core_automation',
            'components/ungoogled/entropy',
            'components/ungoogled/behavior',
            'components/ungoogled/deep_learning',
            'components/ungoogled/metamorphic',
            'components/ungoogled/playwright'
        ]
        
        for path in protection_paths:
            if not Path(path).exists():
                logging.error(f"Missing protection component: {path}")
                return False
        return True
