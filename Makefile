
all: socket_example

socket_example:
	gcc donor_socket.c -o donor
	gcc snoopy_socket.c -o snoopy

clean: 
	rm donor snoopy