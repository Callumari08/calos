build:
	make -C kernel/
	./makebios.sh
	mv image.iso bin/

build_usb:
	make -C kernel/
	./makebios_usb.sh
	mv image.hdd bin/

# run uses QEMU
run: build
	qemu-system-x86_64 \
	-enable-kvm \
	-m 2G \
	-cdrom bin/image.iso
	

clean:
	cd kernel
	make -C kernel/ clean
	./makebios.sh clean 