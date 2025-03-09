#!/bin/bash
set -e

# Create download directory
mkdir -p downloads
cd downloads

# Download tar.xz
echo "Downloading Ungoogled Chromium portable Linux archive..."
wget https://github.com/ungoogled-software/ungoogled-chromium-portablelinux/releases/download/134.0.6998.35-1/ungoogled-chromium_134.0.6998.35-1_linux.tar.xz

# Verify SHA256 checksum
echo "Verifying checksum..."
echo "f20616cebbaac86ee357a7037da8e0450f0e5100e0ee3f09e53232490ddb722a ungoogled-chromium_134.0.6998.35-1_linux.tar.xz" | sha256sum --check

# Extract archive
tar xf ungoogled-chromium_134.0.6998.35-1_linux.tar.xz

# Optional: Move to system location
sudo mkdir -p /opt/ungoogled-chromium
sudo mv ungoogled-chromium_134.0.6998.35-1_linux/* /opt/ungoogled-chromium/
sudo ln -sf /opt/ungoogled-chromium/chrome /usr/local/bin/ungoogled-chromium

# Clean up
cd ..
rm -rf downloads
