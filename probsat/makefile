CC=gcc
CFLAGS= -Wall -Wextra -static -O3 -funroll-loops -fexpensive-optimizations 
#CFLAGS=  -pg -ggdb -Wall -lm   -Wno-missing-braces -static 

all: probsat

probsat:	probSAT.c
			$(CC) $(CFLAGS)  probSAT.c -lm -o probsat
clean:	
		rm -f probsat

