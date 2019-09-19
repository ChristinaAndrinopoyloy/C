#
# In order to execute this "Makefile" just type "make"
#

OBJS 	= ./server.o ./error_handling.o ./arguments.o ./string_library.o ./ip_port_list.o ./server_message.o ./server_helpers.o ./request_routines.o
OUT  	= dropbox_server
CC	= gcc
FLAGS	= -g -c
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

# create/compile the individual files >>separately<< 

server.o: ./server.c
	$(CC) $(FLAGS) ./server.c

error_handling.o: ../error_handling/error_handling.c
	$(CC) $(FLAGS) ../error_handling/error_handling.c

arguments.o: ../structures/arguments.c
	$(CC) $(FLAGS) ../structures/arguments.c

string_library.o: ../string_library/string_library.c
	$(CC) $(FLAGS) ../string_library/string_library.c

ip_port_list.o: ./server_data_structures/ip_port_list.c
	$(CC) $(FLAGS) ./server_data_structures/ip_port_list.c

server_message.o: ./modals/server_message.c
	$(CC) $(FLAGS) ./modals/server_message.c

server_helpers.o: ./routines/server_helpers.c
	$(CC) $(FLAGS) ./routines/server_helpers.c

request_routines.o: ./routines/request_routines.c
	$(CC) $(FLAGS) ./routines/request_routines.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)