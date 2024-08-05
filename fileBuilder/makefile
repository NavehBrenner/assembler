include ../config.mk

main: buildFiles.o fileHandler.o
	$(LD) libBuildFiles.o buildFiles.o fileHandler.o

clean:
	$(RM) *.o