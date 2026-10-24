#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
DRIVER_DIR="$ROOT_DIR/driver"
APP_DIR="$ROOT_DIR/userspace"
DEVICE_PATH="/dev/mmap_device"

echo "[+] Building kernel module"
make -C "$DRIVER_DIR"

echo "[+] Loading module into kernel"
sudo insmod "$DRIVER_DIR/mmap.ko"

if [ ! -e "$DEVICE_PATH" ]; then
    major=$(awk '$2 == "mmap_device" { print $1; exit }' /proc/devices 2>/dev/null || true)
    if [ -z "${major}" ]; then
        echo "[-] Could not detect the device major number for mmap_device" >&2
        exit 1
    fi

    echo "[+] Creating device node $DEVICE_PATH"
    sudo mknod "$DEVICE_PATH" c "$major" 0
fi

echo "[+] Driver loaded successfully"
echo "[+] To run the app: cd $APP_DIR && make -f MakeFile && ./mmap_app"
