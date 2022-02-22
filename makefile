
help.o:
	objcopy -B i386 -I binary -O elf64-x86-64 HELP help.o
debug: help.o
	gcc *.c help.o -g -o eduadmin
eduadmin: 
	gcc *.c help.o -o eduadmin
clean:
	rm -f *.o eduadmin