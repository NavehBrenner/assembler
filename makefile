include config.mk

main: allLibs
	gcc -g "./assembler/libAssembler.o" "./lexer/libLexer.o" "./machineCode/libMachineCode.o" "./fileBuilder/libBuildFiles.o" -o main

allLibs:
	cd "./assembler" && make
	cd "./fileBuilder" && make
	cd "./lexer" && make
	cd "./machineCode" && make
	cd "./preAssembler" && make

clean:
	cd "./@types" && make clean
	cd "./assembler" && make clean
	cd "./fileBuilder" && make clean
	cd "./lexer" && make clean
	cd "./machineCode" && make clean
	cd "./preAssembler" && make clean

rm: del

del:
	cd "./tests" && make clean
