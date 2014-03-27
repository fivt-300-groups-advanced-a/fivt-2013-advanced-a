debug: clean
	mkdir tmp
	./build.sh -DCMAKE_BUILD_TYPE=Debug ./..
	rm -r -f tmp

release: clean
	mkdir tmp
	./build.sh ./..
	rm -r -f tmp

clean:
	rm -r -f tmp
	rm -r -f bin
