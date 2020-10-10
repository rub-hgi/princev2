CC = gcc
CCFLAGS = -O0 -ggdb -Wall

all: princev2cipher princev2test
clean:
	rm -f princev2cipher princev2test *.o

# Dependency rules

princev2test: princev2test.c princev2.c princev2.h key.c key.h block.c block.h misc.c misc.h
	$(CC) $(CCFLAGS) princev2test.c princev2.c key.c block.c misc.c -o $@

princev2cipher: princev2cipher.c princev2.c princev2.h key.c key.h block.c block.h misc.c misc.h
	$(CC) $(CCFLAGS) princev2cipher.c princev2.c key.c block.c misc.c -o $@
