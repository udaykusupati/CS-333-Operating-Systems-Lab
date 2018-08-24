#include <bits/stdc++.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

//Add any global variables you need here
vector<int> dummy_integers1;
vector<pthread_cond_t> condvar;
vector<bool> cond;
pthread_mutex_t m; 
int N;

void* thread_run(void* arg) {
  
  //This same thread_run function should be executed by all threads
	int thread_id = *((int *)arg);
	while(1)
	{
		pthread_mutex_lock(&m);
		while(cond[thread_id]==0)
		{
			pthread_cond_wait(&condvar[thread_id],&m);
		}
		printf("Thread %d\n", thread_id);
		if(thread_id==N-1)
		{
				cond[thread_id]=0;
				cond[0]=1;
				pthread_cond_signal(&condvar[0]);
		}
		else
		{
				cond[thread_id]=0;
				cond[thread_id+1]=1;
				pthread_cond_signal(&condvar[thread_id+1]);
		}
		pthread_mutex_unlock(&m);
	}
	int retval;
	pthread_exit(&retval);
}

int main(int argc, char *argv[]){
  
  if (argc != 2) {
    fprintf(stderr,"Wrong usage :%s N\n", argv[0]);
    exit(0);
  }

  N = atoi(argv[1]);
  printf("Read commandline argument %d\n", N);

  dummy_integers1.resize(N); //Example of how to resize global variables
  vector<int> dummy_integers2(N); //Another way to declare vector

  condvar.resize(N);
  cond.resize(N);
  pthread_mutex_init(&m,0);
  for(int i = 0; i < N; ++i){
    //You can use vectors like arrays
    dummy_integers1[i] = i;
    dummy_integers2[i] = i;
  }

  // Create N threads, and give them all the same thread_run function

  pthread_t thr[N];
  int tid[N];

  int i;
 for(int i=0; i < N; i++)
    {
      if(i==0)
      {
      	cond[i]=1;
      }
      else
      {
      	cond[i]=0;
      }
      pthread_cond_init(&condvar[i],0);
  	}
  for(int i=0; i < N; i++)
    {
      tid[i] = i;
      pthread_create(&thr[i], NULL, thread_run, (void *)&tid[i]);
    }

 for(int i=0; i < N; i++)
    {
      pthread_join(thr[i], NULL);
    }
    pthread_exit(NULL);
}
