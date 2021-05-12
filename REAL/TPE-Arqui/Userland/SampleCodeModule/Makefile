include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_C=$(wildcard */[^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_LOADER=_loader.c

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_C=$(SOURCES_C:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_LOADER=$(SOURCES_LOADER:.c=.o)


all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_C) $(OBJECTS_ASM) $(OBJECTS_LOADER)
	$(LD) $(LDFLAGS) -T ./sampleCodeModule.ld $(OBJECTS_LOADER) $(OBJECTS_ASM) $(OBJECTS_C) $(OBJECTS) -o ../$(MODULE)
	$(LD) $(LDFLAGS) -T ./sampleCodeModule.ld --oformat=elf64-x86-64 -o ../0000-sampleCodeModule.elf $(OBJECTS_LOADER) $(OBJECTS_ASM) $(OBJECTS_C) $(OBJECTS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o */*.o ../*.elf

.PHONY: all clean print
