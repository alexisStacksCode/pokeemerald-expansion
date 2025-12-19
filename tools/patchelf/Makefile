.PHONY: all clean

SRCS = patchelf.c

EXE :=
ifeq ($(OS),Windows_NT)
	EXE := .exe
endif

all: patchelf$(EXE)
	@:

patchelf$(EXE): $(SRCS)
	$(CC) $(SRCS) -o $@ $(LDFLAGS)

clean:
	$(RM) patchelf$(EXE)
