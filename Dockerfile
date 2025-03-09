FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    wget \
    python3 \
    python3-pip \
    libglib2.0-0 \
    libnss3 \
    libxcb1 \
    libxcomposite1 \
    libxcursor1 \
    libxdamage1 \
    libxfixes3 \
    libxi6 \
    libxrandr2 \
    libxrender1 \
    libxss1 \
    libxtst6 \
    git \
    curl \
    unzip \
    && rm -rf /var/lib/apt/lists/*

# Download and install Ungoogled Chromium AppImage
RUN wget https://github.com/ungoogled-software/ungoogled-chromium-binaries/releases/download/134.0.6998.35-1/ungoogled-chromium_134.0.6998.35-1.AppImage \
    && echo "4caf1422187185afd1cb1631880119906428d0c74b0228fafc79c901a64b5da7 ungoogled-chromium_134.0.6998.35-1.AppImage" | sha256sum --check \
    && chmod +x ungoogled-chromium_134.0.6998.35-1.AppImage \
    && mv ungoogled-chromium_134.0.6998.35-1.AppImage /usr/local/bin/ungoogled-chromium

# Set up protection layers
COPY components/ungoogled /opt/protection_layers/
WORKDIR /app

# Install Python requirements
COPY requirements.txt .
RUN pip3 install -r requirements.txt

# Copy protection configurations
COPY config/ /app/config/
COPY patches/ /app/patches/

# Initialize protection components
RUN python3 -m protection_system.initialize \
    --chrome-path=/usr/local/bin/ungoogled-chromium \
    --protection-level=maximum \
    --enable-all-features

ENTRYPOINT ["python3", "server.py"]
