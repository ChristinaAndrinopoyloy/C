#
# In order to execute this "Makefile" just type "make"
#

OBJS 	= main.o ./read_data.o ./hashtable.o ./linked_list.o ./tree.o error_handling.o ./transaction_routines.o ./user_interaction.o ./user_interaction_helpers.o ./date_time_library.o ./date_time_helpers.o
OUT  	= bitcoin
CC	= gcc
FLAGS	= -g -c
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

# create/compile the individual files >>separately<< 

main.o: ./main.c
	$(CC) $(FLAGS) ./main.c

read_data.o: ./helpers/read_data.c
	$(CC) $(FLAGS) ./helpers/read_data.c

hashtable.o: ./data_structures/hashtable.c
	$(CC) $(FLAGS) ./data_structures/hashtable.c

linked_list.o: ./data_structures/linked_list.c
	$(CC) $(FLAGS) ./data_structures/linked_list.c

tree.o: ./data_structures/tree.c
	$(CC) $(FLAGS) ./data_structures/tree.c

error_handling.o: ./error_handling/error_handling.c
	$(CC) $(FLAGS) ./error_handling/error_handling.c

transaction_routines.o: ./helpers/transaction_routines.c
	$(CC) $(FLAGS) ./helpers/transaction_routines.c

user_interaction.o: ./user_interaction/user_interaction.c
	$(CC) $(FLAGS) ./user_interaction/user_interaction.c	

user_interaction_helpers.o: ./user_interaction/user_interaction_helpers.c
	$(CC) $(FLAGS) ./user_interaction/user_interaction_helpers.c

date_time_library.o: ./date_time_library/date_time_library.c
	$(CC) $(FLAGS) ./date_time_library/date_time_library.c

date_time_helpers.o: ./date_time_library/date_time_helpers.c
	$(CC) $(FLAGS) ./date_time_library/date_time_helpers.c
	
# clean house
clean:
	rm -f $(OBJS) $(OUT)