#!/bin/bash
set -e

# Create download directory
mkdir -p downloads
cd downloads

# Download AppImage
echo "Downloading Ungoogled Chromium AppImage..."
wget https://github.com/ungoogled-software/ungoogled-chromium-binaries/releases/download/134.0.6998.35-1/ungoogled-chromium_134.0.6998.35-1.AppImage

# Verify SHA256 checksum
echo "Verifying checksum..."
echo "4caf1422187185afd1cb1631880119906428d0c74b0228fafc79c901a64b5da7 ungoogled-chromium_134.0.6998.35-1.AppImage" | sha256sum --check

# Make executable
chmod +x ungoogled-chromium_134.0.6998.35-1.AppImage

# Optional: Move to system location
sudo mkdir -p /opt/ungoogled-chromium
sudo mv ungoogled-chromium_134.0.6998.35-1.AppImage /opt/ungoogled-chromium/
sudo ln -sf /opt/ungoogled-chromium/ungoogled-chromium_134.0.6998.35-1.AppImage /usr/local/bin/ungoogled-chromium

# Clean up
cd ..
rm -rf downloads