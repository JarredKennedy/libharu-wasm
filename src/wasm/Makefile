libharu-wasm.html libharu-wasm.wasm libharu-wasm.js: libharu-wasm.o
	emcc -g --bind -s ALLOW_MEMORY_GROWTH=1 -s USE_ZLIB=1 -s USE_LIBPNG=1 -s EXIT_RUNTIME=1 libharu-wasm.o ../libhpdf.a -o libharu-wasm.html

libharu-wasm.o:
	emcc --bind -s ALLOW_MEMORY_GROWTH=1 -s USE_ZLIB=1 -s USE_LIBPNG=1 -std=c++11 -I../../include -c libharu-wasm.cpp

clean:
	rm -f libharu-wasm.js libharu-wasm.wasm libharu-wasm.html libharu-wasm.o