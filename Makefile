BIN_DIR := bin
KERNEL_DIR := kernel

.PHONY: all build_uefi build_bios build_bios_usb run clean

all: build_uefi build_bios build_bios_usb

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

build_uefi: $(BIN_DIR)
	$(MAKE) -C $(KERNEL_DIR)
	./makeuefi.sh
	mv image.iso $(BIN_DIR)/

build_bios: $(BIN_DIR)
	$(MAKE) -C $(KERNEL_DIR)
	./makebios.sh
	mv image.iso $(BIN_DIR)/

build_bios_usb: $(BIN_DIR)
	$(MAKE) -C $(KERNEL_DIR)
	./makebios_usb.sh
	mv image.hdd $(BIN_DIR)/

run: build_uefi
	qemu-system-x86_64 \
		-enable-kvm \
		-m 2G \
		-drive if=pflash,format=raw,readonly=on,file=/usr/share/edk2/x64/OVMF_CODE.4m.fd \
		-drive if=pflash,format=raw,file=QEMU/UEFI/OVMF_VARS.4m.fd \
		-cdrom $(BIN_DIR)/image.iso

clean:
	$(MAKE) -C $(KERNEL_DIR) clean
	./makebios.sh clean
	rm -rf $(BIN_DIR)
