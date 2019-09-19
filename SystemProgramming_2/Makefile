#
# In order to execute this "Makefile" just type "make"
#

OBJS 	= ./main.o ./error_handling.o ./arguments.o ./string_library.o ./update_common_directory.o ./remove.o ./linked_list.o ./communication.o ./file.o
OUT  	= mirror_client
CC	= gcc
FLAGS	= -g -c
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

# create/compile the individual files >>separately<< 

main.o: ./main.c
	$(CC) $(FLAGS) ./main.c

error_handling.o: ./error_handling/error_handling.c
	$(CC) $(FLAGS) ./error_handling/error_handling.c

arguments.o: ./structures/arguments.c
	$(CC) $(FLAGS) ./structures/arguments.c

string_library.o: ./string_library/string_library.c
	$(CC) $(FLAGS) ./string_library/string_library.c

update_common_directory.o: ./routines/update_common_directory.c
	$(CC) $(FLAGS) ./routines/update_common_directory.c

remove.o: ./routines/remove.c
	$(CC) $(FLAGS) ./routines/remove.c

linked_list.o: ./data_structures/linked_list.c
	$(CC) $(FLAGS) ./data_structures/linked_list.c

communication.o: ./routines/communication.c
	$(CC) $(FLAGS) ./routines/communication.c

file.o: ./structures/file.c
	$(CC) $(FLAGS) ./structures/file.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)