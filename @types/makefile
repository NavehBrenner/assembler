include ../config.mk

main: token.o symbol.o word.o
	$(LD) libTypes.o token.o symbol.o word.o

clean:
	$(RM) *.o