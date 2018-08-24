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

using namespace std;

class Semaphore{

  //Add member variables
	int cnt;
	pthread_mutex_t m;
	pthread_cond_t can_down;

public:
 Semaphore(int N);
 void down();
 void up(); 
};

