include ../config.mk

main: libMachineCode.o

libMachineCode.o: gnrlTrns.o opTrns.o instTrns.o
	ld -r -o libMachineCode.o gnrlTrns.o opTrns.o instTrns.o

clean:
	$(RM) *.o