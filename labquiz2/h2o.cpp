#include <bits/stdc++.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <queue>

using namespace std;

// Add any global variables as needed
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t first=PTHREAD_COND_INITIALIZER;// condvar on which leaving atoms wait i.e sort of buffer
pthread_cond_t generalh=PTHREAD_COND_INITIALIZER;//generalh and generalo are condvars on which normal Hs and normal OS wait
pthread_cond_t generalo=PTHREAD_COND_INITIALIZER;
int h_leave=0;int o_leave=0;//leaving atoms' counts
void * Hydrogen(void *arg) {
  // You can't assume that arg points to some object in main since we will use custom main() to check your code.
 	
  	cout << "Hydrogen Thread Created" << endl;	// Don't remove this line
  	pthread_mutex_lock(&m);
  	while(h_leave>=2)
  	{
  		pthread_cond_wait(&generalh,&m);
  	}
  	h_leave++;
  	pthread_mutex_unlock(&m);

  	pthread_mutex_lock(&m);
  	if(h_leave<2||o_leave<1)
	{
		pthread_cond_wait(&first,&m);
	}
	pthread_cond_signal(&first);

  // TO-DO - Add your code here. Make sure that you wait if you can't form a water molecule.
  cout << "Hydrogen Thread Exiting" << endl; // Don't remove this line
  	h_leave--;
	if(h_leave==0&&o_leave==0){pthread_cond_signal(&generalh);pthread_cond_signal(&generalo);}
	pthread_mutex_unlock(&m);
}

void * Oxygen(void *arg) {
  // You can't assume that arg points to some object in main since we will use custom main() to check your code.
  cout << "Oxygen Thread Created" << endl; // Don't remove this line	
  	pthread_mutex_lock(&m);
  	while(o_leave>=1)
  	{
  		pthread_cond_wait(&generalo,&m);
  	}
  	o_leave++;
  	pthread_mutex_unlock(&m);

  	pthread_mutex_lock(&m);
  	if(h_leave<2||o_leave<1)
	{
		pthread_cond_wait(&first,&m);
	}
	pthread_cond_signal(&first);


  // TO-DO - Add your code here. Make sure that you wait if you can't form a water molecule.
  
  cout << "Oxygen Thread Exiting" << endl; // Don't remove this line
    o_leave--;
	if(h_leave==0&&o_leave==0){pthread_cond_signal(&generalh);pthread_cond_signal(&generalo);}
	pthread_mutex_unlock(&m);
}  

pthread_t create_hydrorgen_thread(){
  // You can't change the syntax or code of this function in anyway!
  int ret;
  pthread_t tid;
  if (ret = (pthread_create(&tid, NULL, Hydrogen, NULL)) != 0) {	//creating threads
    cout << "Error creating Hydrogen thread: "<< strerror(ret) << endl;
    exit(1);
  }
  return tid;
}

pthread_t create_oxygen_thread(){
  // You can't change the syntax or code of this function in anyway!
  int ret;
  pthread_t tid;
  if (ret = (pthread_create(&tid, NULL, Oxygen, NULL)) != 0) {	//creating threads
    cout << "Error creating Oxygen thread: " << strerror(ret) << endl;
    exit(1);
  }
  return tid;
}

int main(int argc, char *argv[]){
  // Note that this main program is just for testing purpose. We will use our custom main program to check your code.
  // Only thing that you are allowed to do is to create more threads, change their ordering and wait_seconds variable value.
  int wait_seconds = 2;
  vector<pthread_t> tid;
  tid.push_back(create_hydrorgen_thread());
  sleep(wait_seconds);
  tid.push_back(create_hydrorgen_thread());
  sleep(wait_seconds);
  tid.push_back(create_hydrorgen_thread());
  sleep(wait_seconds);
  tid.push_back(create_oxygen_thread());
  sleep(wait_seconds);
  tid.push_back(create_hydrorgen_thread());
  sleep(wait_seconds);
  tid.push_back(create_oxygen_thread());
  sleep(wait_seconds);
  int stat;
  for(int i = 0; i < tid.size(); ++i){	// joining all threads
    pthread_join(tid[i], (void **)&stat);
  }
  exit(0);
}

