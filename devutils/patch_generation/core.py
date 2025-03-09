#!/usr/bin/env python3
import subprocess
import json
import logging
from pathlib import Path
from typing import Dict, List, Set, Optional
from dataclasses import dataclass

@dataclass
class PatchConfig:
    category: str
    component: str
    patterns: List[str]
    modifications: Dict[str, str]
    dependencies: List[str]

class ChromiumPatchGenerator:
    def __init__(self, config_path: Path = Path("config/patch_generation.json")):
        self.config = self._load_config(config_path)
        self.logger = logging.getLogger("patch_generator")
        
    def _load_config(self, path: Path) -> Dict:
        with open(path) as f:
            return json.load(f)
    
    def analyze_chromium_changes(self, old_version: str, new_version: str) -> List[PatchConfig]:
        changes = []
        
        # Analyze fingerprinting changes
        changes.extend(self._analyze_fingerprint_changes(old_version, new_version))
        
        # Analyze automation detection
        changes.extend(self._analyze_automation_detection(old_version, new_version))
        
        # Analyze behavior monitoring
        changes.extend(self._analyze_behavior_monitoring(old_version, new_version))
        
        return changes

    def _analyze_fingerprint_changes(self, old_ver: str, new_ver: str) -> List[PatchConfig]:
        fingerprint_patterns = [
            (r"navigator\.(userAgent|platform|vendor)", "user_agent"),
            (r"canvas(\.toDataURL|\.getImageData)", "canvas"),
            (r"webgl(\.getParameter|\.getSupportedExtensions)", "webgl"),
            (r"audioContext\.(createOscillator|createAnalyser)", "audio"),
            (r"performance\.(now|timing|memory)", "performance")
        ]
        
        changes = []
        diff = self._get_version_diff(old_ver, new_ver)
        
        for pattern, component in fingerprint_patterns:
            if self._pattern_changed(diff, pattern):
                changes.append(PatchConfig(
                    category="fingerprint",
                    component=component,
                    patterns=[pattern],
                    modifications=self._get_fingerprint_modifications(component),
                    dependencies=self._get_dependencies(component)
                ))
        
        return changes

    def _get_fingerprint_modifications(self, component: str) -> Dict[str, str]:
        base_mods = {
            "user_agent": {
                "target": "navigator.userAgent",
                "modification": """
                    const originalUserAgent = navigator.userAgent;
                    Object.defineProperty(navigator, 'userAgent', {
                        get: function() {
                            return ProtectionEngine.getRandomizedUA(originalUserAgent);
                        }
                    });
                """
            },
            "canvas": {
                "target": "HTMLCanvasElement.prototype",
                "modification": """
                    const originalGetContext = HTMLCanvasElement.prototype.getContext;
                    HTMLCanvasElement.prototype.getContext = function() {
                        const context = originalGetContext.apply(this, arguments);
                        return CanvasProtection.applyNoise(context);
                    };
                """
            }
            # Add other component modifications
        }
        return base_mods.get(component, {})

    def generate_patches(self, changes: List[PatchConfig]) -> Dict[str, Path]:
        generated_patches = {}
        
        for change in changes:
            patch_path = self._generate_single_patch(change)
            if patch_path:
                generated_patches[f"{change.category}_{change.component}"] = patch_path
                
        return generated_patches

    def _generate_single_patch(self, config: PatchConfig) -> Optional[Path]:
        patch_template = self._get_patch_template(config.category)
        if not patch_template:
            return None
            
        patch_content = self._fill_patch_template(
            template=patch_template,
            config=config
        )
        
        patch_path = Path(f"patches/extra/ungoogled-chromium/{config.category}_{config.component}.patch")
        patch_path.parent.mkdir(parents=True, exist_ok=True)
        patch_path.write_text(patch_content)
        
        return patch_path

    def _get_patch_template(self, category: str) -> Optional[str]:
        template_path = Path(f"devutils/patch_generation/templates/{category}_template.patch")
        if template_path.exists():
            return template_path.read_text()
        return None

    def _fill_patch_template(self, template: str, config: PatchConfig) -> str:
        replacements = {
            "COMPONENT": config.component,
            "CATEGORY": config.category,
            "MODIFICATIONS": self._generate_modification_code(config.modifications),
            "DEPENDENCIES": self._generate_dependency_includes(config.dependencies)
        }
        
        for key, value in replacements.items():
            template = template.replace(f"{{{{ {key} }}}}", value)
            
        return template

    def validate_patches(self, patches: Dict[str, Path]) -> bool:
        for name, path in patches.items():
            if not self._validate_single_patch(path):
                self.logger.error(f"Validation failed for patch: {name}")
                return False
        return True

    def _validate_single_patch(self, patch_path: Path) -> bool:
        try:
            result = subprocess.run(
                ["git", "apply", "--check", str(patch_path)],
                capture_output=True,
                text=True
            )
            return result.returncode == 0
        except Exception as e:
            self.logger.error(f"Patch validation error: {e}")
            return False

def main():
    logging.basicConfig(level=logging.INFO)
    generator = ChromiumPatchGenerator()
    
    # Get versions to compare
    old_version = "103.0.5060.114"
    new_version = "104.0.5112.79"
    
    # Analyze changes
    changes = generator.analyze_chromium_changes(old_version, new_version)
    
    # Generate patches
    patches = generator.generate_patches(changes)
    
    # Validate patches
    if generator.validate_patches(patches):
        print("Successfully generated and validated patches:")
        for name, path in patches.items():
            print(f"  - {name}: {path}")
    else:
        print("Patch generation failed validation")

if __name__ == "__main__":
    main()