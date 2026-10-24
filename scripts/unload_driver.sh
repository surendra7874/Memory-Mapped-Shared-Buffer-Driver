#!/bin/bash
set -euo pipefail

DEVICE_PATH="/dev/mmap_device"

if [ -e "$DEVICE_PATH" ]; then
    echo "[+] Removing device node $DEVICE_PATH"
    sudo rm -f "$DEVICE_PATH"
fi

if lsmod | grep -q '^mmap '; then
    echo "[+] Unloading mmap module"
    sudo rmmod mmap
else
    echo "[+] mmap module is not loaded"
fi

echo "[+] Driver unloaded successfully"
