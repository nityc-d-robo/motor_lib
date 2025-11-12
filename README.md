# motor_lib

## Building

### Standard Build

To build the entire project:
```bash
cargo build
```

To build in release mode:
```bash
cargo build --release
```

### Building usb_can_server for Different Target Architectures

The `usb_can_server` binary can be built for different target architectures using cross-compilation.

#### Method 1: Using Cargo directly

```bash
# Install the target architecture (if not already installed)
rustup target add <target-triple>

# Build for the specific target
cargo build --bin usb_can_server --target <target-triple>
```

Example targets:
- `aarch64-unknown-linux-gnu` - ARM64 Linux
- `armv7-unknown-linux-gnueabihf` - ARM32 Linux
- `x86_64-pc-windows-gnu` - Windows 64-bit
- `x86_64-apple-darwin` - macOS Intel
- `aarch64-apple-darwin` - macOS Apple Silicon

Example:
```bash
rustup target add aarch64-unknown-linux-gnu
cargo build --bin usb_can_server --target aarch64-unknown-linux-gnu
```

#### Method 2: Using the Build Script (Recommended)

The project includes a convenient shell script for building:

```bash
# Build for native architecture
./build-usb-server.sh

# Build for a specific target
./build-usb-server.sh --target aarch64-unknown-linux-gnu

# Build for a specific target in release mode
./build-usb-server.sh --target aarch64-unknown-linux-gnu --release

# Show all options
./build-usb-server.sh --help
```

The script will automatically:
- Check if the target is installed
- Prompt to install it if needed
- Build the binary
- Show the location of the built binary

#### Method 3: Using Make

The project includes a Makefile with convenient targets for building:

```bash
# Build for a specific target
make build-usb-server-target TARGET=aarch64-unknown-linux-gnu

# Build for a specific target in release mode
make build-usb-server-target-release TARGET=aarch64-unknown-linux-gnu

# Quick targets for common architectures
make build-usb-server-arm64      # ARM64 Linux
make build-usb-server-arm32      # ARM32 Linux
make build-usb-server-windows    # Windows x64

# List all available targets
make list-targets

# Show all available make targets
make help
```

### Cross-Compilation Setup

For cross-compilation, you may need to install the appropriate linker and system libraries:

**For ARM64 (aarch64):**
```bash
# On Ubuntu/Debian
sudo apt-get install gcc-aarch64-linux-gnu
```

**For ARM32 (armv7):**
```bash
# On Ubuntu/Debian
sudo apt-get install gcc-arm-linux-gnueabihf
```

Then configure the linker in `.cargo/config.toml` (see the file for examples).

### Finding the Built Binary

After building for a specific target, the binary will be located at:
```
target/<target-triple>/debug/usb_can_server
# or for release builds:
target/<target-triple>/release/usb_can_server
```

Example:
```
target/aarch64-unknown-linux-gnu/debug/usb_can_server
```
