#!/usr/bin/env python3

import logging
from config_manager import BuildConfigManager
from build_orchestrator import ChromiumBuildOrchestrator
from protection_validator import ProtectionValidator
from fingerprint_checker import FingerprintChecker

def main():
    logging.basicConfig(
        level=logging.INFO,
        format='%(asctime)s - %(levelname)s - %(message)s'
    )
    
    try:
        # Initialize build configuration
        config_manager = BuildConfigManager()
        
        # Initialize protection validator
        protection_validator = ProtectionValidator()
        
        # Initialize fingerprint checker
        fingerprint_checker = FingerprintChecker()
        
        # Setup ccache
        logging.info("Setting up ccache...")
        config_manager.setup_ccache()
        
        # Initialize build orchestrator
        orchestrator = ChromiumBuildOrchestrator(
            config_manager,
            protection_validator,
            fingerprint_checker
        )
        
        # Prepare build environment
        logging.info("Preparing build environment...")
        orchestrator.prepare_build_environment()
        
        # Verify protection features before sync
        logging.info("Verifying protection features...")
        protection_validator.verify_pre_build()
        
        # Sync source code
        logging.info("Syncing source code...")
        orchestrator.sync_source()
        
        # Verify fingerprinting protections
        logging.info("Checking fingerprint protections...")
        fingerprint_checker.verify_implementations()
        
        # Execute build
        logging.info("Building Chromium with protections...")
        orchestrator.build()
        
        # Verify post-build protections
        logging.info("Verifying post-build protections...")
        protection_validator.verify_post_build()
        
        # Archive artifacts with protection metadata
        logging.info("Archiving protected build artifacts...")
        orchestrator.archive_artifacts()
        
        logging.info("Protected build completed successfully!")
        
    except Exception as e:
        logging.error(f"Build failed: {str(e)}")
        raise

if __name__ == "__main__":
    main()
