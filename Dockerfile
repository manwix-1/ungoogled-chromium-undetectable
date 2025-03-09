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

# Download latest Ungoogled Chromium (134.0.6998.35-1)
RUN mkdir -p /opt/chrome
WORKDIR /opt/chrome
RUN wget https://github.com/ungoogled-software/ungoogled-chromium-binaries/releases/download/134.0.6998.35-1/ungoogled-chromium_134.0.6998.35-1_linux.tar.xz -O chrome.tar.xz \
    && tar xf chrome.tar.xz \
    && rm chrome.tar.xz

# Set up protection layers
COPY components/ungoogled /opt/protection_layers/
WORKDIR /app

# Install Python requirements
COPY requirements.txt .
RUN pip3 install -r requirements.txt

# Copy protection configurations
COPY config/ /app/config/
COPY patches/ /app/patches/

# Verify protection system integrity
RUN python3 -m build.dependency_validator \
    && python3 -m protection_system.verify_integrity

# Initialize protection components
RUN python3 -m protection_system.initialize \
    --chrome-path=/opt/chrome \
    --protection-level=maximum \
    --enable-all-features \
    --verify-integrity=true

# Run integration tests
RUN python3 -m pytest testing/integration/

# Set up environment variables for all protection features
ENV PROTECTION_SYSTEM_INITIALIZED=true \
    PROTECTION_INTEGRITY_VERIFIED=true \
    COMPONENT_INTERCONNECTION_VERIFIED=true

ENTRYPOINT ["python3", "server.py"]
