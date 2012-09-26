UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
all:
	gcc -shared -fPIC forcedproxy.c -ldl -o forcedproxy.so
clean:
	rm forcedproxy.so
endif
ifeq ($(UNAME), Darwin)
all:
	gcc -flat_namespace -dynamiclib -current_version 1.0 forcedproxy.c -o forcedproxy.dylib
clean:
	rm forcedproxy.dylib
endif
