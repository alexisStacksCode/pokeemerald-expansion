.PHONY: all clean

SRCS = main.c

EXE :=
ifeq ($(OS),Windows_NT)
	EXE := .exe
endif

all: mgba-rom-test-hydra$(EXE)
	@:

mgba-rom-test-hydra$(EXE): $(SRCS)
	$(CC) $(SRCS) -Werror=implicit-function-declaration -o $@ -lm $(LDFLAGS)

clean:
	$(RM) mgba-rom-test-hydra$(EXE)
