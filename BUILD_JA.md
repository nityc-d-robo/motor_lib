# ビルドガイド - usb_can_serverの異なるターゲットアーキテクチャでのビルド

## 概要

`src/bin/usb_can_server.rs` を異なるターゲットアーキテクチャでビルドする方法を説明します。

## 方法1: Cargoコマンドを直接使用

### ステップ1: ターゲットをインストール

```bash
rustup target add <ターゲット名>
```

例:
```bash
# ARM64 Linux用
rustup target add aarch64-unknown-linux-gnu

# ARM32 Linux用
rustup target add armv7-unknown-linux-gnueabihf

# Windows 64bit用
rustup target add x86_64-pc-windows-gnu
```

### ステップ2: ビルド実行

```bash
cargo build --bin usb_can_server --target <ターゲット名>
```

例:
```bash
# ARM64 Linux用にビルド
cargo build --bin usb_can_server --target aarch64-unknown-linux-gnu

# リリースビルド
cargo build --bin usb_can_server --target aarch64-unknown-linux-gnu --release
```

## 方法2: ビルドスクリプトを使用（推奨）

プロジェクトには便利なビルドスクリプトが用意されています：

```bash
# ネイティブアーキテクチャ用にビルド
./build-usb-server.sh

# 特定のターゲット向けにビルド
./build-usb-server.sh --target aarch64-unknown-linux-gnu

# 特定のターゲット向けにリリースビルド
./build-usb-server.sh --target aarch64-unknown-linux-gnu --release

# すべてのオプションを表示
./build-usb-server.sh --help
```

スクリプトは自動的に以下を行います：
- ターゲットがインストールされているか確認
- 必要に応じてインストールを促す
- バイナリをビルド
- ビルドされたバイナリの場所を表示

## 方法3: Makefileを使用

プロジェクトにはMakefileも用意されており、簡単にビルドできます。

### 基本的な使い方

```bash
# 特定のターゲット向けにビルド
make build-usb-server-target TARGET=aarch64-unknown-linux-gnu

# リリースビルド
make build-usb-server-target-release TARGET=aarch64-unknown-linux-gnu
```

### よく使うターゲット用のショートカット

```bash
# ARM64 Linux用
make build-usb-server-arm64

# ARM32 Linux用
make build-usb-server-arm32

# Windows 64bit用
make build-usb-server-windows
```

### 利用可能なターゲット一覧を表示

```bash
make list-targets
```

### すべてのMakeコマンドを表示

```bash
make help
```

## 主なターゲット一覧

| ターゲット名 | プラットフォーム | 説明 |
|------------|----------------|------|
| `x86_64-unknown-linux-gnu` | Linux x86_64 | 標準的なLinux 64bit |
| `aarch64-unknown-linux-gnu` | Linux ARM64 | Raspberry Pi 4など |
| `armv7-unknown-linux-gnueabihf` | Linux ARM32 | Raspberry Pi 3など |
| `x86_64-pc-windows-gnu` | Windows x86_64 | Windows 64bit |
| `x86_64-apple-darwin` | macOS Intel | macOS (Intel CPU) |
| `aarch64-apple-darwin` | macOS ARM64 | macOS (Apple Silicon) |

## クロスコンパイルの設定

異なるアーキテクチャ向けにビルドする場合、リンカーとシステムライブラリのインストールが必要な場合があります。

### ARM64 (aarch64) 向け

```bash
# Ubuntu/Debian の場合
sudo apt-get install gcc-aarch64-linux-gnu
```

`.cargo/config.toml` に以下を追加:
```toml
[target.aarch64-unknown-linux-gnu]
linker = "aarch64-linux-gnu-gcc"
```

### ARM32 (armv7) 向け

```bash
# Ubuntu/Debian の場合
sudo apt-get install gcc-arm-linux-gnueabihf
```

`.cargo/config.toml` に以下を追加:
```toml
[target.armv7-unknown-linux-gnueabihf]
linker = "arm-linux-gnueabihf-gcc"
```

## ビルド成果物の場所

ビルドされたバイナリは以下の場所に生成されます:

```
target/<ターゲット名>/debug/usb_can_server
# または、リリースビルドの場合:
target/<ターゲット名>/release/usb_can_server
```

例:
```
target/aarch64-unknown-linux-gnu/debug/usb_can_server
target/aarch64-unknown-linux-gnu/release/usb_can_server
```

## 実用例

### Raspberry Pi 4 (ARM64) 向けにリリースビルド

```bash
# ターゲットのインストール（初回のみ）
rustup target add aarch64-unknown-linux-gnu

# リリースビルド
make build-usb-server-target-release TARGET=aarch64-unknown-linux-gnu
# または
cargo build --bin usb_can_server --target aarch64-unknown-linux-gnu --release

# ビルド成果物の場所
ls -l target/aarch64-unknown-linux-gnu/release/usb_can_server
```

### Windows向けにビルド（Linuxホストから）

```bash
# ターゲットのインストール
rustup target add x86_64-pc-windows-gnu

# ビルド
make build-usb-server-windows
# または
cargo build --bin usb_can_server --target x86_64-pc-windows-gnu
```

## トラブルシューティング

### リンカーエラーが発生する場合

クロスコンパイル用のリンカーがインストールされているか確認してください:

```bash
# ARM64用
which aarch64-linux-gnu-gcc

# ARM32用
which arm-linux-gnueabihf-gcc
```

### ターゲットがインストールされているか確認

```bash
rustup target list --installed
```

### 利用可能なすべてのターゲットを表示

```bash
rustup target list
```
