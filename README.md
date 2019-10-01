
# Threads_synchronisation:

## producer-consumer problem implemented using condition variable and locks:
	User will give two integer : number of threads and number of producer threads.
    One thread will be randomly chosen by program , and this thread will be master thread. The master thread will choose given number of producer thread , after it will assigned rest thread as consumer thread.

    Now producer threads have to produce 1000 random number together and insert into a queue. since maximum size of queue is fixed. if no more item can be insert into queue then consumer have to delete the item from queue.
