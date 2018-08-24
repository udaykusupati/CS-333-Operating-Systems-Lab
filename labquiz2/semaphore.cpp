#include <iostream>
#include <bits/stdc++.h>
#include <stdio.h>
#include "semaphore.h"

using namespace std;

Semaphore::Semaphore(int N){

  // add code here
  cnt=N;
  pthread_mutex_init(&m,0);
  pthread_cond_init(&can_down,0);
}
void Semaphore::down(){

  // add code here
	
	pthread_mutex_lock(&m);
	while(cnt<=0)
	{
		pthread_cond_wait(&can_down,&m);
	}
	cnt--;
	pthread_mutex_unlock(&m);
	return; 
}

void Semaphore::up(){

  // add code here
	pthread_mutex_lock(&m);
	cnt++;
	pthread_cond_signal(&can_down);
	pthread_mutex_unlock(&m);
	return;
}

