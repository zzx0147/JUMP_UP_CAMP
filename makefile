RPiProgram : packetController.o doubleSensor.o UTServer.o
	gcc -o RPiProgram packetController.o doubleSensor.o UTServer.o -lwiringPi -lpthread

packetController.o : packetController.c
	gcc -c -o packetController.o packetController.c -LwiringPi -lpthread

doubleSensor.o : doubleSensor.c
	gcc -c -o doubleSensor.o doubleSensor.c -LwiringPi -lpthread

UTServer.o : UTServer.c
	gcc -c -o UTServer.o UTServer.c -lwiringPi -lpthread