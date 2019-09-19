#
# In order to execute this "Makefile" just type "make"
#

OBJS 	= main.o ./factory.o ./helper.o ./sem_mem.o -lpthread
OUT  	= myfactory
CC	= gcc
FLAGS   = -c -Wall 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

# create/compile the individual files >>separately<< 

main.o: ./main.c
	$(CC) $(FLAGS) ./main.c

factory.o: ./factory.c
	$(CC) $(FLAGS) ./factory.c

helper.o: ./helper.c
	$(CC) $(FLAGS) ./helper.c

sem_mem.o: ./sem_mem.c
	$(CC) $(FLAGS) ./sem_mem.c

# clean house
clean:
	rm *.o