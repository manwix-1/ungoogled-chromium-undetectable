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
        return self.get_default_config()

    def get_default_config():
        return {
            "build": {
                "parallel_jobs": os.cpu_count(),
                "artifacts_dir": "build/artifacts",
                "protection_features": {
                    "hardware_masking": True,
                    "fingerprint_protection": True,
                    "process_isolation": True,
                    "sensor_protection": True,
                    "native_code_protection": True,
                    "browser_behavior_protection": True
                }
            },
            "protection": {
                "webgl_noise": 0.1,
                "canvas_noise": 0.15,
                "timing_jitter": 0.05,
                "sensor_noise": {
                    "accelerometer": 0.002,
                    "gyroscope": 0.001
                },
                "process_randomization": {
                    "memory_variation": 1024 * 1024,  # 1MB
                    "timing_variation": 0.03
                },
                "behavior_simulation": {
                    "input_delay": {"min": 10, "max": 100},
                    "movement_naturalization": 0.8,
                    "focus_patterns": True
                }
            }
        }

    def get_protection_config(self):
        return self.config["protection"]

    def get_build_config(self):
        return self.config["build"]
