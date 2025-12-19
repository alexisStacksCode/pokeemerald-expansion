CC ?= gcc
.PHONY: all clean

SRCS = gbafix.c

EXE :=
ifeq ($(OS),Windows_NT)
	EXE := .exe
endif

all: gbafix$(EXE)
	@:

gbafix$(EXE): $(SRCS)
	$(CC) $(SRCS) -o $@ $(LDFLAGS)

clean:
	$(RM) gbafix gbafix.exe
