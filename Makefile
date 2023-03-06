TARGET = main

CC = g++
CFLAGS = -g -fsanitize=address

SRC_DIR = ./Source/

SRC = 	$(SRC_DIR)main.cpp			\
		$(SRC_DIR)Stack.cpp			\
	  	$(SRC_DIR)Log.cpp			\
		$(SRC_DIR)HashCounters.cpp

$(TARGET) :
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean :
	rm -f *.o $(TARGET)
