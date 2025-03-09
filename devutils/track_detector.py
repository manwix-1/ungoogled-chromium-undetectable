#!/usr/bin/env python3
import re
import sys
import json
from pathlib import Path
from typing import Dict, List, Set

class TrackingDetector:
    def __init__(self):
        self.known_patterns = {
            'analytics': [
                r'google-analytics\.com',
                r'analytics.*collect',
                r'metrics.*collect'
            ],
            'fingerprinting': [
                r'navigator\..*platform',
                r'canvas\..*data',
                r'webgl\..*parameter',
                r'performance\..*timing',
                r'chrome\..*runtime'
            ],
            'network': [
                r'\.google\.com',
                r'gstatic\.com',
                r'googleapis\.com',
                r'chromium\.org'
            ]
        }
        
    def scan_source_tree(self, src_path: Path) -> Dict[str, Set[str]]:
        findings = {k: set() for k in self.known_patterns.keys()}
        
        for source_file in src_path.rglob('*'):
            if source_file.suffix in ['.cc', '.h', '.js', '.html']:
                self._scan_file(source_file, findings)
                
        return findings
        
    def _scan_file(self, file_path: Path, findings: Dict[str, Set[str]]) -> None:
        try:
            content = file_path.read_text()
            for category, patterns in self.known_patterns.items():
                for pattern in patterns:
                    matches = re.finditer(pattern, content, re.IGNORECASE)
                    for match in matches:
                        findings[category].add(f"{file_path}:{match.group(0)}")
        except Exception as e:
            print(f"Error scanning {file_path}: {e}", file=sys.stderr)

def main():
    src_path = Path('build/src')
    detector = TrackingDetector()
    
    print("Scanning for new tracking mechanisms...")
    findings = detector.scan_source_tree(src_path)
    
    # Generate patch templates for new findings
    patch_dir = Path('patches/extra/ungoogled-chromium')
    patch_dir.mkdir(parents=True, exist_ok=True)
    
    for category, matches in findings.items():
        if matches:
            patch_path = patch_dir / f"disable-new-{category}-tracking.patch"
            with open(patch_path, 'w') as f:
                f.write(f"# Disable new {category} tracking mechanisms\n\n")
                for match in sorted(matches):
                    f.write(f"# {match}\n")
                f.write("\n--- a/...\n+++ b/...\n")
    
    print("Results:")
    for category, matches in findings.items():
        print(f"\n{category.upper()} findings: {len(matches)}")
        for match in sorted(matches):
            print(f"  {match}")

if __name__ == "__main__":
    main()