all: 
	g++ -Wall -D_POSIX_PTHREAD_SEMANTICS sampleThreadProgram.c thread.c q.c -lpthread -lrt -o sampleThreadProgram
