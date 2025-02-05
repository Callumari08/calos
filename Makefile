build:
	make -C kernel/
	./makebios.sh
	mkdir bin 
	mv image.iso bin/

clean:
	cd kernel
	make -C kernel/ clean
	./makebios.sh clean 