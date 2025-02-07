#!/bin/bash
if [ "$1" = "clean" ]; then
    set -e
    echo "Cleaning..."
    rm -rf iso_root/ limine/ image.iso
    echo "Done!"
    exit
fi

# Download the latest Limine binary release for the 8.x branch.
git clone https://github.com/limine-bootloader/limine.git --branch=v8.x-binary --depth=1

# Build the "limine" utility.
make -C limine

# Create the ISO root directory.
mkdir -p iso_root/boot/limine

# Copy over the kernel and configuration.
# (Adjust paths as needed for your kernel and configuration.)
cp -v kernel/bin/calos iso_root/boot/
cp -v kernel/deps/limine/limine.conf limine/limine-uefi-cd.bin limine/limine.sys iso_root/boot/limine/

# Create the EFI boot tree and copy Limine's UEFI executables.
mkdir -p iso_root/EFI/BOOT
cp -v limine/BOOTX64.EFI iso_root/EFI/BOOT/
cp -v limine/BOOTIA32.EFI iso_root/EFI/BOOT/

# Create the bootable UEFI-only ISO.
xorriso -as mkisofs -R -J --efi-boot boot/limine/limine-uefi-cd.bin \
-efi-boot-part --efi-boot-image --protective-msdos-label \
iso_root -o image.iso

echo "UEFI-only ISO created as image.iso"