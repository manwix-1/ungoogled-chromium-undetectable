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
    tar \
    xz-utils \
    && rm -rf /var/lib/apt/lists/*

# Download and install Ungoogled Chromium portable Linux version
RUN wget https://github.com/ungoogled-software/ungoogled-chromium-portablelinux/releases/download/134.0.6998.35-1/ungoogled-chromium_134.0.6998.35-1_linux.tar.xz \
    && echo "f20616cebbaac86ee357a7037da8e0450f0e5100e0ee3f09e53232490ddb722a ungoogled-chromium_134.0.6998.35-1_linux.tar.xz" | sha256sum --check \
    && tar xf ungoogled-chromium_134.0.6998.35-1_linux.tar.xz \
    && mkdir -p /usr/local/ungoogled-chromium \
    && mv ungoogled-chromium_134.0.6998.35-1_linux/* /usr/local/ungoogled-chromium/ \
    && ln -sf /usr/local/ungoogled-chromium/chrome /usr/local/bin/ungoogled-chromium \
    && rm ungoogled-chromium_134.0.6998.35-1_linux.tar.xz

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
