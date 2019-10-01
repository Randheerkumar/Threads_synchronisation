
# Threads_synchronisation:

## producer buffer problem implemented using condition variable and locks:
	User will give two integr : number of threads and number of producer threads.
    One thread will be randomly choosen by program , and this thread will be master thread. The master thread will choose given number of prodcuer thread , after it will assisgned rest thread as consumer thread.

    Now producer threads have to produce 1000 random number together and insert into a queue. since maximum size of queue is fixed. if no more item can be insert into queue then consumer have to delete the item from queue.
