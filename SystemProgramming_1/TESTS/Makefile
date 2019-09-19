#
# In order to execute this "Makefile" just type "make"
#

OBJS = test_main.o test_linked_list.o
OUT	= tests
CC = g++ -g
FLAGS = -c -Wall -std=c++11 -pthread
LIBS = -lgtest_main -lgtest -lpthread
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $@

# Test Files
test_main.o: test_main.cpp
	$(CC) $(FLAGS) test_main.cpp $(LIBS)

test_linked_list.o: test_linked_list.cpp
	$(CC) $(FLAGS) test_linked_list.cpp $(LIBS)

# Files TO Test
linked_list.o: ../data_structures/linked_list.cpp
	$(CC) $(FLAGS) ../data_structures/linked_list.cpp $(LIBS)

# clean house
clean:
rm -f $(OBJS) $(OUT)