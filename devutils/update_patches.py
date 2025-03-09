#!/usr/bin/env python3
import subprocess
import sys
from pathlib import Path
from typing import List, Tuple

class PatchUpdater:
    def __init__(self):
        self.patch_dirs = [
            'patches/core/ungoogled-chromium',
            'patches/extra/ungoogled-chromium'
        ]
        
    def update_patches(self) -> List[Tuple[Path, bool]]:
        results = []
        for patch_dir in self.patch_dirs:
            patch_path = Path(patch_dir)
            for patch_file in patch_path.glob('*.patch'):
                success = self._update_single_patch(patch_file)
                results.append((patch_file, success))
        return results
                
    def _update_single_patch(self, patch_file: Path) -> bool:
        try:
            # Extract original paths from patch
            with open(patch_file) as f:
                content = f.read()
                
            # Apply patch with fuzz factor to handle minor changes
            result = subprocess.run(
                ['patch', '--fuzz=2', '--force', '-p1'],
                input=content.encode(),
                capture_output=True
            )
            
            if result.returncode != 0:
                print(f"Failed to apply {patch_file}: {result.stderr.decode()}")
                return False
                
            # Generate updated patch
            subprocess.run(
                ['git', 'diff'],
                stdout=open(patch_file, 'w')
            )
            
            return True
            
        except Exception as e:
            print(f"Error updating {patch_file}: {e}", file=sys.stderr)
            return False

def main():
    print("Updating patches for new tracking mechanisms...")
    
    # First detect new tracking mechanisms
    subprocess.run(['python3', 'devutils/track_detector.py'])
    
    # Then update existing patches
    updater = PatchUpdater()
    results = updater.update_patches()
    
    # Report results
    success = sum(1 for _, ok in results if ok)
    total = len(results)
    
    print(f"\nPatch update complete: {success}/{total} successful")
    
    if success < total:
        print("\nFailed patches:")
        for patch, ok in results:
            if not ok:
                print(f"  {patch}")
        sys.exit(1)

if __name__ == "__main__":
    main()