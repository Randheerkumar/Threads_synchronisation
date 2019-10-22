#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

#include "q.h"

smutex_t lock;
scond_t wait,cwait,pwait;


static void go(int n);
//static void pause(int n);

bool isProducer(int i,int *arr);


#define NTHREADS 100
static thread_t threads[100];




// initilizations

int count=0;
int n=10; // no of threads 
int p=5; // no of producer
int c=n-p-1; // no of consumer
int minn=100000, master_idx=0;
struct Queue *election_q=createQueue();
struct Queue *data_q=createQueue();

int stop_cond=0;

int *producer, *consumer, *wait_c,*wait_p,*no_of_p,*no_of_c;


int ii=0,jj=0,m=0;
int part2=0;

int buffer_size=0;
int rand_num=0;
int limit=0;



void produce(int thread_No){

	
	for(int i=0; i<1000; i++)
	{
		if(limit > 999) return;
		//printf("you are in producer fn , buffer_size is%d",buffer_size);
		smutex_lock(&lock);
		rand_num=rand();
		if(buffer_size > 99) 
		{
			wait_p[thread_No]++;
			scond_wait(&pwait,&lock);
			//thread_yield();

		
		}
		else{
			no_of_p[thread_No]++;
			enQueue(data_q,rand_num);
			buffer_size++;
			limit++;
		}
		smutex_unlock(&lock);
		if(limit > 999) return;
		thread_sleep(0,(rand()%10)*1000);

	}


}

void consume(int thread_No){

		for(int i=0; i<1000; i++)
		{	
			//smutex_lock(&lock);
			if(limit > 999) {
				return ;
			}
			smutex_lock(&lock);
			if(buffer_size > 0){
				no_of_c[thread_No]++;
				deQueue(data_q);
				buffer_size--;
			}

			else{
				wait_c[thread_No]++;
				scond_wait(&cwait,&lock);
				
			}
			smutex_unlock(&lock);
			if(limit > 999) {
				return ;
			}

			thread_sleep(0,(rand()%6)*1000);  // waiting less than 6 ms;
		}
}



int main(int argc,char** argv){
	
	int i;

	if(argc > 2)
	{
		n=atoi(argv[1]);
		p=atoi(argv[2]);
	}
	c=n-p-1;
 producer=(int *) malloc(sizeof(int)*p);
 no_of_p=(int *) malloc(sizeof(int)*n);
 wait_p=(int *) malloc(sizeof(int)*n);

 consumer=(int *) malloc(sizeof(int)*c);
 no_of_c=(int *) malloc(sizeof(int)*n);
 wait_c=(int *) malloc(sizeof(int)*n);

 for(int i=0; i<n; i++){
  no_of_p[i]=0;
  wait_p[i]=0;
 }
 for(int i=0; i<n ;i++){
 	no_of_c[i]=0;
 	no_of_p[i]=0;
 }




	//init(n,p);
     smutex_init(&lock);
     scond_init(&wait);
     scond_init(&cwait);
     scond_init(&pwait);

	for(i=0;i<n;i++){
		thread_create(&(threads[i]),&go,i);
	}

    while( count < n);

	 printf("master thread is %d \n", master_idx);

	 
	 

 while(stop_cond==0){
 	thread_yield();
 }
 thread_join(threads[master_idx]);
 
 printf("no of times\n");
 int iii=0;
 i=0;
for(iii=0; iii<n; iii++){
	if(producer[i]==iii)
	{	
		printf("%dth producer waits and produce: %d and %d\n",i,wait_p[iii],no_of_p[iii]);
		i++;
	}
}
i=0;
for(int iii=0; iii<n; iii++){
	if(consumer[i]==iii){
		printf("%dth consumer waits and consume: %d and %d\n",i,wait_c[iii],no_of_c[iii]);
		i++;
	}
}



	printf("Main thread done.\n");
	return 0;
}

void go(int ith_thread){

smutex_lock(&lock);
//int ith_thread=thread_no;
int num=rand()%1001;
if(minn > num){
   minn=num;
   master_idx=ith_thread;
}
count++;
// printf("count ans random number : %d and %d\n",ith_thread,num);
  
  enQueue(election_q,num);
    if(count==n){

    	smutex_unlock(&lock);
        scond_broadcast(&wait, &lock);
    }
    else{
    	scond_wait(&wait,&lock);
    }

  
 smutex_unlock(&lock);

  if(master_idx==ith_thread){
				while(ii < p){
			  				if(m == master_idx){
			  					m++;
			  					continue;
			  				}
			  				producer[ii++]=m++;
			  			}	
			
			  	while(jj < c){
			
			  				if(m == master_idx){
			  					m++;
			  					continue;
			  				}
			  				consumer[jj++]=m++;
			  	}
			  	
			  	part2=1;
			  	scond_broadcast(&wait,&lock);
			  		while(true){
			  				smutex_lock(&lock);
			  				if(limit > 999){
			  					smutex_unlock(&lock);
						  		break;
  								
  								
			  				}
			  				smutex_unlock(&lock);
			  				scond_broadcast(&wait,&lock);
			  				scond_broadcast(&cwait,&lock);
			  				scond_broadcast(&pwait,&lock);

			  		}
			  		stop_cond=1;
			  		for(int i=0; i<p; i++){
			  				thread_join(threads[producer[i]]);
			  			}
  					for(int i=0; i<c; i++)
  					{
  					
  					thread_join(threads[consumer[i]]);
  					}
			  			  	

}
		  
  else {

  	smutex_lock(&lock);
  	 
  		if(!part2){
  			scond_wait(&wait,&lock);
  		}
  		//printf("part 2 is :%d\n",part2);
  		smutex_unlock(&lock);

  		if(isProducer(ith_thread,producer)){
  			produce(ith_thread);

  		}
  		else{
  			consume(ith_thread);
  		}

  }

	
	printf(" exit! thread no :%d\n",ith_thread);
	
	thread_exit(100+ith_thread);
	//Notreached
}


bool isProducer(int j, int *arr){

	
	for(int i=0; i<p; i++){
		if(arr[i]== j) {
			return true;
		}
	}
	return false;

}
