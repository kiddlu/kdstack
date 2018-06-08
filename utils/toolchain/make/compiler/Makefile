CC=$(CROSS_COMPILE)gcc
COMPILER = $(shell $(CC) -dM -E -  < /dev/null | grep _WIN32 > null && echo "MINGW"  || echo "POSIX")

all:
	@echo "Compile is $(CC)"
	@echo "Type is $(COMPILER)"