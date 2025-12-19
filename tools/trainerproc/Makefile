.PHONY: all clean

CFLAGS := -Wall -O2
SRCS := main.c

EXE :=
ifeq ($(OS),Windows_NT)
	EXE := .exe
endif

all: trainerproc$(EXE)
	@:

trainerproc$(EXE): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

clean:
	$(RM) trainerproc$(EXE)
