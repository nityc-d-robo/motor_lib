# Makefile for building motor_lib with different target architectures
# Especially useful for cross-compiling usb_can_server

# Default target architecture (current host)
DEFAULT_TARGET := $(shell rustc -vV | grep host | cut -d' ' -f2)

# Common target architectures
TARGETS := \
	x86_64-unknown-linux-gnu \
	aarch64-unknown-linux-gnu \
	armv7-unknown-linux-gnueabihf \
	x86_64-pc-windows-gnu

# Default build (native architecture)
.PHONY: build
build:
	cargo build

# Build usb_can_server with default target
.PHONY: build-usb-server
build-usb-server:
	cargo build --bin usb_can_server

# Build usb_can_server in release mode
.PHONY: build-usb-server-release
build-usb-server-release:
	cargo build --bin usb_can_server --release

# Build usb_can_server for a specific target
# Usage: make build-usb-server-target TARGET=aarch64-unknown-linux-gnu
.PHONY: build-usb-server-target
build-usb-server-target:
ifndef TARGET
	$(error TARGET is not set. Usage: make build-usb-server-target TARGET=<target-triple>)
endif
	@echo "Building usb_can_server for target: $(TARGET)"
	@echo "Checking if target is installed..."
	@rustup target list --installed | grep -q $(TARGET) || \
		(echo "Target $(TARGET) not installed. Installing..." && rustup target add $(TARGET))
	cargo build --bin usb_can_server --target $(TARGET)

# Build usb_can_server for a specific target in release mode
.PHONY: build-usb-server-target-release
build-usb-server-target-release:
ifndef TARGET
	$(error TARGET is not set. Usage: make build-usb-server-target-release TARGET=<target-triple>)
endif
	@echo "Building usb_can_server for target: $(TARGET) (release mode)"
	@echo "Checking if target is installed..."
	@rustup target list --installed | grep -q $(TARGET) || \
		(echo "Target $(TARGET) not installed. Installing..." && rustup target add $(TARGET))
	cargo build --bin usb_can_server --target $(TARGET) --release

# Quick targets for common architectures
.PHONY: build-usb-server-arm64
build-usb-server-arm64:
	$(MAKE) build-usb-server-target TARGET=aarch64-unknown-linux-gnu

.PHONY: build-usb-server-arm32
build-usb-server-arm32:
	$(MAKE) build-usb-server-target TARGET=armv7-unknown-linux-gnueabihf

.PHONY: build-usb-server-windows
build-usb-server-windows:
	$(MAKE) build-usb-server-target TARGET=x86_64-pc-windows-gnu

# List available targets
.PHONY: list-targets
list-targets:
	@echo "Installed targets:"
	@rustup target list --installed
	@echo ""
	@echo "Common targets for cross-compilation:"
	@echo "  - aarch64-unknown-linux-gnu (ARM64 Linux)"
	@echo "  - armv7-unknown-linux-gnueabihf (ARM32 Linux)"
	@echo "  - x86_64-pc-windows-gnu (Windows 64-bit)"
	@echo "  - x86_64-apple-darwin (macOS Intel)"
	@echo "  - aarch64-apple-darwin (macOS Apple Silicon)"
	@echo ""
	@echo "To install a target: rustup target add <target-triple>"

# Clean build artifacts
.PHONY: clean
clean:
	cargo clean

# Help
.PHONY: help
help:
	@echo "Motor Lib Build Targets:"
	@echo ""
	@echo "  make build                          - Build all targets (default architecture)"
	@echo "  make build-usb-server               - Build usb_can_server (default architecture)"
	@echo "  make build-usb-server-release       - Build usb_can_server in release mode"
	@echo ""
	@echo "  make build-usb-server-target TARGET=<target>"
	@echo "                                      - Build usb_can_server for specific target"
	@echo "  make build-usb-server-target-release TARGET=<target>"
	@echo "                                      - Build usb_can_server for specific target (release)"
	@echo ""
	@echo "Quick targets for common architectures:"
	@echo "  make build-usb-server-arm64         - Build for ARM64 Linux"
	@echo "  make build-usb-server-arm32         - Build for ARM32 Linux"
	@echo "  make build-usb-server-windows       - Build for Windows x64"
	@echo ""
	@echo "  make list-targets                   - List installed and common targets"
	@echo "  make clean                          - Clean build artifacts"
	@echo ""
	@echo "Examples:"
	@echo "  make build-usb-server-target TARGET=aarch64-unknown-linux-gnu"
	@echo "  make build-usb-server-arm64"
	@echo ""
