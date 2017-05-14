
all: socket_example switched

socket_example:
	gcc donor_socket.c -o donor -g
	gcc snoopy_socket.c -o snoopy -g

switched: 
	gcc donor.c -o d -g
	gcc snoopy.c -o s -g

clean: 
	rm donor snoopy d s