from setuptools import setup, find_packages

setup(
    name="enhanced-ungoogled-chromium",
    version="1.0.0",
    packages=find_packages(),
    include_package_data=True,
    install_requires=[
        "playwright>=1.30.0",
        "requests>=2.28.0",
        "websockets>=10.4",
        "numpy>=1.23.0",
        "tensorflow>=2.11.0",
        "cryptography>=39.0.0",
        "scipy>=1.10.0",
        "pillow>=9.4.0",
        "pyOpenSSL>=23.0.0",
        "fonttools>=4.39.0",
        "opencv-python>=4.7.0",
        "tensorflow-probability>=0.19.0",
        "pyOpenGL>=3.1.6",
        "quantum-random>=1.0.0",
        "torch>=1.9.0",
        "scikit-learn>=0.24.2"
    ],
)