wasm: libhpdf.a
	make -C src/wasm

libhpdf.a:
	make -C src

clean:
	make -C src clean
	make -C src/wasm clean