#!/bin/bash

# Define the list of required packages
REQUIRED_PACKAGES=(
    "valgrind"
    "sqlite3"
    "libssl-dev"
    "libpcre3-dev"
    "cmake"
    "make"
    "gcc"
)

# Function to check if a package is installed
is_installed() {
    dpkg -l | grep -qw "$1"
}

# Update package list
echo "Updating package list..."
sudo apt-get update

# Check and install missing packages
for PACKAGE in "${REQUIRED_PACKAGES[@]}"; do
    if is_installed "$PACKAGE"; then
        echo "$PACKAGE is already installed."
    else
        echo "$PACKAGE is not installed. Installing..."
        sudo apt-get install -y "$PACKAGE"
    fi
done

echo "All required packages are installed."
