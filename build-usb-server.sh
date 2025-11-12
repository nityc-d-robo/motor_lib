#!/bin/bash
# Script to build usb_can_server for different target architectures
# Usage: ./build-usb-server.sh [target] [options]

set -e

# Default values
TARGET=""
RELEASE=""
HELP=false

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --target|-t)
            TARGET="$2"
            shift 2
            ;;
        --release|-r)
            RELEASE="--release"
            shift
            ;;
        --help|-h)
            HELP=true
            shift
            ;;
        *)
            echo "Unknown option: $1"
            HELP=true
            shift
            ;;
    esac
done

# Show help
if [ "$HELP" = true ]; then
    cat << EOF
Usage: ./build-usb-server.sh [OPTIONS]

Build usb_can_server binary for different target architectures.

OPTIONS:
    -t, --target <TARGET>    Target architecture triple (e.g., aarch64-unknown-linux-gnu)
    -r, --release            Build in release mode
    -h, --help              Show this help message

EXAMPLES:
    # Build for native architecture
    ./build-usb-server.sh

    # Build for ARM64 Linux
    ./build-usb-server.sh --target aarch64-unknown-linux-gnu

    # Build for ARM64 Linux in release mode
    ./build-usb-server.sh --target aarch64-unknown-linux-gnu --release

    # Build for Windows
    ./build-usb-server.sh --target x86_64-pc-windows-gnu

COMMON TARGETS:
    x86_64-unknown-linux-gnu        - Linux x86_64 (native on most Linux systems)
    aarch64-unknown-linux-gnu       - Linux ARM64 (Raspberry Pi 4, etc.)
    armv7-unknown-linux-gnueabihf   - Linux ARM32 (Raspberry Pi 3, etc.)
    x86_64-pc-windows-gnu           - Windows x86_64
    x86_64-apple-darwin             - macOS Intel
    aarch64-apple-darwin            - macOS Apple Silicon

NOTE:
    For cross-compilation, you may need to install the appropriate toolchain:
    - rustup target add <target-triple>
    - Install cross-compiler (e.g., gcc-aarch64-linux-gnu for ARM64)

EOF
    exit 0
fi

# Build command
if [ -z "$TARGET" ]; then
    echo "Building usb_can_server for native architecture..."
    cargo build --bin usb_can_server $RELEASE
    
    if [ -z "$RELEASE" ]; then
        echo "Binary location: target/debug/usb_can_server"
    else
        echo "Binary location: target/release/usb_can_server"
    fi
else
    echo "Building usb_can_server for target: $TARGET"
    
    # Check if target is installed
    if ! rustup target list --installed | grep -q "^$TARGET\$"; then
        echo "Target $TARGET is not installed."
        read -p "Do you want to install it? (y/n) " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            rustup target add "$TARGET"
        else
            echo "Aborting. Please install the target with: rustup target add $TARGET"
            exit 1
        fi
    fi
    
    cargo build --bin usb_can_server --target "$TARGET" $RELEASE
    
    if [ -z "$RELEASE" ]; then
        echo "Binary location: target/$TARGET/debug/usb_can_server"
    else
        echo "Binary location: target/$TARGET/release/usb_can_server"
    fi
fi

echo "Build completed successfully!"
