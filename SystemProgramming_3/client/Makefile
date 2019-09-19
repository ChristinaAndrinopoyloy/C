#
# In order to execute this "Makefile" just type "make"
#

OBJS 	= ./client.o ./error_handling.o ./arguments.o ./string_library.o ./client_helpers.o ./client_message.o ./client_list.o ./cyclic_buffer.o ./server_client_helpers.o ./server_client_request_routines.o ./file.o ./file_storage.o ./date_time_library.o ./date_time_helpers.o
OUT  	= dropbox_client
CC	= gcc
FLAGS	= -g -c
LIBS 	= -lpthread
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) $(LIBS) -o $@

# create/compile the individual files >>separately<< 

client.o: ./client.c
	$(CC) $(FLAGS) ./client.c

error_handling.o: ../error_handling/error_handling.c
	$(CC) $(FLAGS) ../error_handling/error_handling.c

arguments.o: ../structures/arguments.c
	$(CC) $(FLAGS) ../structures/arguments.c

string_library.o: ../string_library/string_library.c
	$(CC) $(FLAGS) ../string_library/string_library.c

client_helpers.o: ./routines/client_helpers.c
	$(CC) $(FLAGS) ./routines/client_helpers.c

client_message.o: ./modals/client_message.c
	$(CC) $(FLAGS) ./modals/client_message.c

client_list.o: ./client_data_structures/client_list.c
	$(CC) $(FLAGS) ./client_data_structures/client_list.c

cyclic_buffer.o: ./client_data_structures/cyclic_buffer.c
	$(CC) $(FLAGS) ./client_data_structures/cyclic_buffer.c

server_client_helpers.o: ./routines/server_client_helpers.c
	$(CC) $(FLAGS) ./routines/server_client_helpers.c

server_client_request_routines.o: ./routines/server_client_request_routines.c
	$(CC) $(FLAGS) ./routines/server_client_request_routines.c

file.o: ./routines/file.c
	$(CC) $(FLAGS) ./routines/file.c

file_storage.o: ./modals/file_storage.c
	$(CC) $(FLAGS) ./modals/file_storage.c	

date_time_library.o: ../date_time_library/date_time_library.c
	$(CC) $(FLAGS) ../date_time_library/date_time_library.c

date_time_helpers.o: ../date_time_library/date_time_helpers.c
	$(CC) $(FLAGS) ../date_time_library/date_time_helpers.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)