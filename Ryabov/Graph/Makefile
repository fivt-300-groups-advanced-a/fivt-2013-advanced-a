debug:
	mkdir -p tmp
	cd tmp && cmake -DCMAKE_BUILD_TYPE=Debug ./..
	$(MAKE) -C tmp

release:
	mkdir -p tmp
	cd tmp && cmake ./..
	$(MAKE) -C tmp

clean:
	rm -r -f tmp
	rm -r -f bin

run: debug
	./bin/main
