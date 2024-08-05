include ../config.mk

main: lexer.o libTypes.o
	$(LD) libLexer.o lexer.o "../@types/libTypes.o"

libTypes.o:
	cd "../@types" && make

clean:
	$(RM) *.o