#include <iostream>
#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>
#include <signal.h>
#include <pthread.h>
#include "semaphore.h"

using namespace std;

// Declare any global variables here
Semaphore a(0),b(0),c(1);
void *thread_run(void *data)
{

  int thread_id = *((int *)data);

  int i;
  for(i = 0; i < 5; i++)
    {

      // Add any synchronization code here
    	if(thread_id==0)
    	{
    		c.down();
    	}
    	else if(thread_id==1)
    	{
    		a.down();
    	}
    	else
    	{
    		b.down();
    	}

      printf("Thread %d ran\n", thread_id);
      if(thread_id==0)
    	{
    		a.up();
    	}
    	else if(thread_id==1)
    	{
    		b.up();
    	}
    	else
    	{
    		c.up();
    	}
	
      	//s.up();
      // Add any synchronization code here

    }

}



int main(int argc, char *argv[]){

  pthread_t thr[3];
  int tid[3];

  int i;

  for(int i=0; i < 3; i++)
    {
      tid[i] = i;
      pthread_create(&thr[i], NULL, thread_run, (void *)&tid[i]);
    }

 for(int i=0; i < 3; i++)
    {
      pthread_join(thr[i], NULL);
    }
    exit(0);
}

