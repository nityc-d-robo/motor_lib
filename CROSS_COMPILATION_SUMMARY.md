# Cross-Compilation Support Summary

## Issue
The user asked: "src/bin/usb_can_server.rsだけビルドのターゲットアーキテクチャを変えたいよみたいな時にどうしたらいいですか？"
(Translation: "What should I do when I want to change the build target architecture only for src/bin/usb_can_server.rs?")

## Solution
This PR implements comprehensive cross-compilation support for the `usb_can_server` binary, allowing it to be built for different target architectures while keeping the rest of the project's build configuration unchanged.

## Implementation Details

### 1. Cargo Configuration (.cargo/config.toml)
- Created a `.cargo/config.toml` file with:
  - Documentation on how to build for different targets
  - Examples for common target architectures
  - Commented-out linker configurations for cross-compilation
  - This file serves as a central reference for build configuration

### 2. Build Script (build-usb-server.sh)
- Interactive shell script that simplifies cross-compilation
- Features:
  - Automatically checks if target is installed
  - Prompts user to install missing targets
  - Supports both debug and release builds
  - Shows location of built binary
  - Comprehensive help text with examples

### 3. Makefile
- Convenient make targets for common build scenarios
- Includes:
  - Generic targets for any architecture
  - Quick shortcuts for ARM64, ARM32, Windows
  - Target listing functionality
  - Automatic target installation check
  - Help documentation

### 4. Documentation
- **README.md**: Updated with English documentation
  - Three methods for building with different targets
  - Examples for common architectures
  - Cross-compilation setup instructions
- **BUILD_JA.md**: New Japanese documentation
  - Comprehensive guide for Japanese users
  - Step-by-step instructions
  - Troubleshooting section

## Usage Examples

### Method 1: Direct Cargo Command
```bash
rustup target add aarch64-unknown-linux-gnu
cargo build --bin usb_can_server --target aarch64-unknown-linux-gnu
```

### Method 2: Build Script (Recommended)
```bash
./build-usb-server.sh --target aarch64-unknown-linux-gnu --release
```

### Method 3: Makefile
```bash
make build-usb-server-arm64
# or
make build-usb-server-target TARGET=aarch64-unknown-linux-gnu
```

## Supported Target Architectures

| Target | Platform | Use Case |
|--------|----------|----------|
| x86_64-unknown-linux-gnu | Linux x86_64 | Native Linux builds |
| aarch64-unknown-linux-gnu | Linux ARM64 | Raspberry Pi 4, etc. |
| armv7-unknown-linux-gnueabihf | Linux ARM32 | Raspberry Pi 3, etc. |
| x86_64-pc-windows-gnu | Windows x86_64 | Windows builds |
| x86_64-apple-darwin | macOS Intel | macOS Intel builds |
| aarch64-apple-darwin | macOS ARM64 | Apple Silicon builds |

## Cross-Compilation Requirements

For actual cross-compilation (not just native builds), users may need:

1. **Install the target**: `rustup target add <target-triple>`
2. **Install cross-compiler** (if compiling from Linux to ARM):
   - For ARM64: `sudo apt-get install gcc-aarch64-linux-gnu`
   - For ARM32: `sudo apt-get install gcc-arm-linux-gnueabihf`
3. **Configure linker** in `.cargo/config.toml` if needed

## Benefits

1. **Flexibility**: Build for different architectures without changing project structure
2. **Ease of Use**: Multiple methods to suit different workflows
3. **Documentation**: Comprehensive guides in both English and Japanese
4. **Automation**: Scripts handle target installation and verification
5. **No Breaking Changes**: Existing build process remains unchanged

## Testing

All three methods have been tested and verified to work correctly:
- ✅ Native builds (x86_64-unknown-linux-gnu)
- ✅ Build script functionality
- ✅ Makefile targets
- ✅ Documentation accuracy

## Security Summary

No security vulnerabilities introduced:
- All changes are configuration, documentation, and build scripts
- No code changes to the actual application
- CodeQL analysis: No applicable code changes detected
