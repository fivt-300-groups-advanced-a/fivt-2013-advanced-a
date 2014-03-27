debug: clean
	mkdir tmp
	cd tmp && cmake -DCMAKE_BUILD_TYPE=Debug ./..
	$(MAKE) -C tmp
	rm -r -f tmp

release: clean
	mkdir tmp
	cd tmp && cmake ./..
	$(MAKE) -C tmp
	rm -r -f tmp

release_in:
	cmake ./..
	make

clean:
	rm -r -f tmp
	rm -r -f bin

run:
	./bin/main
