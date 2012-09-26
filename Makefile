UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
all:
	gcc -m64 -shared -fPIC forcedproxy.c -ldl -o x64/forcedproxy.so
	gcc -m32 -shared -fPIC forcedproxy.c -ldl -o forcedproxy.so
clean:
	rm x64/forcedproxy.so forcedproxy.so
endif
ifeq ($(UNAME), Darwin)
all:
	gcc -m64 -flat_namespace -dynamiclib -current_version 1.0 forcedproxy.c -o x64/forcedproxy.dylib
	gcc -m32 -flat_namespace -dynamiclib -current_version 1.0 forcedproxy.c -o forcedproxy.dylib
clean:
	rm x64/forcedproxy.dylib forcedproxy.dylib
endif
