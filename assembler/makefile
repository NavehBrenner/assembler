include ../config.mk

main: assembler.o syntax.o
	$(LD) libAssembler.o assembler.o syntax.o

clean:
	$(RM) *.o