build:
	make -C kernel/
	./makebios.sh
	mkdir bin/
	mv image.iso bin/

build_usb:
	make -C kernel/
	./makebios_usb.sh
	mkdir bin/
	mv image.hdd bin/

# run uses QEMU
run: build
	qemu-system-x86_64 \
	-enable-kvm \
	-m 2G \
	-cdrom bin/image.iso
clean:
	make -C kernel/ clean
	./makebios.sh clean
	rm -r bin/