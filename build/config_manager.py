import os
import json
from pathlib import Path

class BuildConfigManager:
    def __init__(self, config_path="build_config.json"):
        self.config_path = config_path
        self.config = self._load_config()

    def _load_config(self):
        if os.path.exists(self.config_path):
            with open(self.config_path, 'r') as f:
                return json.load(f)
        return {
            "build": {
                "parallel_jobs": os.cpu_count(),
                "artifacts_dir": "build/artifacts",
                "protection_features": {
                    "hardware_masking": True,
                    "fingerprint_protection": True,
                    "process_isolation": True
                }
            },
            "protection": {
                "webgl_noise": 0.1,
                "canvas_noise": 0.15,
                "timing_jitter": 0.05
            }
        }

    def get_protection_config(self):
        return self.config["protection"]

    def get_build_config(self):
        return self.config["build"]
