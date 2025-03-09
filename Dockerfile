FROM ubuntu:22.04

# Install dependencies in a single layer
RUN apt-get update && apt-get install -y \
    wget \
    software-properties-common \
    && add-apt-repository ppa:deadsnakes/ppa \
    && apt-get update \
    && apt-get install -y \
    python3.11 \
    python3.11-distutils \
    python3.11-dev \
    python3.11-venv \
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

# Set up Python 3.11
RUN curl -sS https://bootstrap.pypa.io/get-pip.py | python3.11 \
    && python3.11 -m pip install --upgrade pip

# Download and install Ungoogled Chromium
RUN wget https://github.com/ungoogled-software/ungoogled-chromium-portablelinux/releases/download/134.0.6998.35-1/ungoogled-chromium_134.0.6998.35-1_linux.tar.xz \
    && echo "f20616cebbaac86ee357a7037da8e0450f0e5100e0ee3f09e53232490ddb722a ungoogled-chromium_134.0.6998.35-1_linux.tar.xz" | sha256sum --check \
    && tar xf ungoogled-chromium_134.0.6998.35-1_linux.tar.xz \
    && mkdir -p /usr/local/ungoogled-chromium \
    && mv ungoogled-chromium_134.0.6998.35-1_linux/* /usr/local/ungoogled-chromium/ \
    && ln -sf /usr/local/ungoogled-chromium/chrome /usr/local/bin/ungoogled-chromium \
    && rm ungoogled-chromium_134.0.6998.35-1_linux.tar.xz

# Set up the application directory
WORKDIR /app

# Copy project files
COPY pyproject.toml .
COPY components/ components/
COPY protection_system/ protection_system/
COPY config/ config/
COPY patches/ patches/

# Install build dependencies first
RUN python3.11 -m pip install --no-cache-dir hatchling

# Create virtual environment and install dependencies
RUN python3.11 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"
RUN pip install --no-cache-dir -e .

# Initialize protection components
RUN python3.11 -m protection_system.initialize \
    --chrome-path=/usr/local/bin/ungoogled-chromium \
    --protection-level=maximum \
    --enable-all-features

ENTRYPOINT ["python3.11", "server.py"]
