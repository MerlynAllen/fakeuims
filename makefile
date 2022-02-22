debug: 
	gcc *.c -g -o eduadmin
eduadmin: 
	gcc *.c -o eduadmin
clean:
	rm -f *.o eduadmin