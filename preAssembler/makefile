include ../config.mk

main: libLexer.o libPreAssembler.o libTypes.o libBuildFiles.o
	gcc -g "../fileBuilder/libBuildFiles.o" "../lexer/libLexer.o" "./libPreAssembler.o" -o preAssembler

libPreAssembler.o: macro.o macroParser.o
	$(LD) libPreAssembler.o macro.o macroParser.o

libLexer.o:
	cd "../lexer" && make

libTypes.o:
	cd "../@types" && make

libBuildFiles.o:
	cd "../fileBuilder" && make

clean:
	$(RM) *.o


