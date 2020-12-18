INCLUDE_DIR = ../ 
LIBRARY_DIR = ./
UTIL_PARENT = ../../

CC = gcc
CFLAGS = -O3 -Wall -fprefetch-loop-arrays #-fno-tree-vectorize 
LDGLAGS = 
LIBS = -lpthread

CFLAGS += -I$(INCLUDE_DIR) -I$(UTIL_PARENT)

PROGRAMS = test_hierarchy memops_scalability ptp_comm mixed_mem randacc seqacc

all : $(PROGRAMS)

seqacc: util.o access_pattern.o seqacc.o
	$(CC) $(LDFLAGS) util.o access_pattern.o seqacc.o -o seqacc -L$(LIBRARY_DIR) $(LIBS)

randacc: util.o access_pattern.o randacc.o
	$(CC) $(LDFLAGS) util.o access_pattern.o randacc.o -o randacc -L$(LIBRARY_DIR) $(LIBS)

test_hierarchy : util.o access_pattern.o test_hierarchy.o
	$(CC) $(LDFLAGS) util.o access_pattern.o test_hierarchy.o -o test_hierarchy -L$(LIBRARY_DIR) $(LIBS)

memops_scalability : access_pattern.o processor_map.o util.o memops_scalability.o 
	$(CC) $(LDFLAGS) access_pattern.o processor_map.o util.o memops_scalability.o -o memops_scalability -L$(LIBRARY_DIR) $(LIBS)    

ptp_comm : processor_map.o util.o ptp_comm_2threads.o
	$(CC) $(LDFLAGS) processor_map.o util.o ptp_comm_2threads.o -o ptp_comm -L$(LIBRARY_DIR) $(LIBS)    

mixed_mem : processor_map.o util.o mixed_mem.o
	$(CC) $(LDFLAGS) processor_map.o util.o mixed_mem.o -o mixed_mem -L$(LIBRARY_DIR) $(LIBS)   

util.o : $(UTIL_PARENT)/util/util.c
	$(CC) $(CFLAGS) -c $(UTIL_PARENT)/util/util.c

processor_map.o : $(UTIL_PARENT)/util/processor_map.c
	$(CC) $(CFLAGS) -c $(UTIL_PARENT)/util/processor_map.c

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f $(PROGRAMS) *.o 
