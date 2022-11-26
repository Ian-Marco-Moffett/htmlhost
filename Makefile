CFLAGS = -Werror=implicit -Werror=implicit-function-declaration -Werror=implicit-int \
	-Werror=int-conversion -Wno-builtin-declaration-mismatch      										 \
	-Werror=incompatible-pointer-types -Werror=int-to-pointer-cast

.PHONY: all
all:
	mkdir -p bin/
	gcc -Iinclude src/*.c $(CFLAGS) -o bin/htmlhost
