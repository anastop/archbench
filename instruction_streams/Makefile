INCLUDE_DIR = ../ 
LIBRARY_DIR = ./
UTIL_DIR = ../../util/

CC = gcc
CFLAGS = -O3 -Wall 
LDGLAGS = 
LIBS = -lpthread

CFLAGS += -I$(INCLUDE_DIR) -I$(UTIL_DIR)

PROGRAMS = streams 

all : $(PROGRAMS)

streams : access_pattern.o processor_map.o util.o test_streams.o ops.o threadinfo.o
	$(CC) $(LDFLAGS) access_pattern.o processor_map.o util.o ops.o test_streams.o threadinfo.o -o streams -L$(LIBRARY_DIR) $(LIBS)	

util.o : $(UTIL_DIR)/util.c
	$(CC) $(CFLAGS) -c $(UTIL_DIR)/util.c

processor_map.o : $(UTIL_DIR)/processor_map.c
	$(CC) $(CFLAGS) -c $(UTIL_DIR)/processor_map.c

access_pattern.o : ../memory_tests/access_pattern.c
	$(CC) $(CFLAGS) -c ../memory_tests/access_pattern.c

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f $(PROGRAMS) *.o  
