# Copyright (c) 2002 ACM/SIGDA

TARGET		= map

SRCS		= $(wildcard *.cpp)
HDRS		= $(wildcard *.h)
OBJS		= $(SRCS:.cpp=.o)
INCLUDES	=

# specify appropriate C compiler flags here:
CFLAGS		= -g $(INCLUDES)
LDFLAGS		=
LIBPATHS	=
LIBS		=
CC = g++
$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBPATHS) $(LIBS)

OBJS: $(HDRS)

.PHONY:	clean test

test:	$(TARGET) 
	./$(TARGET) ../test/test1
	./$(TARGET) ../test/test2
	./$(TARGET) ../test/test3
	./$(TARGET) ../test/test4
	./$(TARGET) ../test/test5
	./$(TARGET) ../test/test6
	./$(TARGET) ../test/test7
	./$(TARGET) ../test/test8

clean : 
	rm -f *.o $(TARGET)
