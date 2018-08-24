#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/time.h>
using namespace std;
int* count;
int* I;
int n;
char** filename;

void * func(void* tid)
{
    int id=(long)tid;
    //node* temp=(node *)tem;
    //char* filename=temp->filename;
    //int id=temp->id;
    FILE *myFile;
    myFile = fopen(filename[id], "r");
    if(myFile==NULL)cout<<"lan"<<endl;
// /cout<<filename[id]<<endl;
    //read file into array
    int numberArray;
    /* for(int j=0;j<10000;j++)
     {
     	numberArray[j]=0;
     }*/
    int i = 0;
    // fscanf (file, "%d", &i);
    while(!feof(myFile))
    {
        fscanf(myFile, "%d", &numberArray);
        //printf("Number is: %d\n\n", numberArray[i]);
        count[id]+=numberArray;
        i++;
    }
    I[id]=i;
    /* for(int j=0;j<i;j++)
     {
     	count[id]+=numberArray[j];
     }*/
    //count[id]=count[id];
    //cout<<count[id]<<endl;
    fclose(myFile);
    pthread_exit(0);
}
int main(int argc, char* argv[])
{
    n=0;
    //cout<<argv[2]<<endl;
    n=atoi(argv[1]);
    count=new int[n];
    I=new int[n];
    pthread_t* thread=new pthread_t[n];
    filename=new char*[n];
    for(int i=0; i<n; i++)
    {
        filename[i]=argv[2+i];
    }
    //node* temp=new node[n];
    //pthread_t p;
    //pthread_create(&p,NULL,fu,NULL);
    timeval start,end;
    gettimeofday(&start,NULL);
    for(int i=0; i<n; i++)
    {
        //temp[i].id=i;
        //temp[i].filename=filename;
        //int t;
        if(!pthread_create(&thread[i],NULL,func,(void *)i))
        {}
        /*{
        	cout<<"Error creating threads "<<t<<endl;
        }*/
    }
    void* status;
    for(int i=0; i<n; i++)
    {
        pthread_join(thread[i],&status);
    }

    //cout<<"lan"<<endl;
    double avg=0;
    int isum=0;
    for(int i=0; i<n; i++)
    {
        avg+=count[i];
        isum+=I[i];

        //cout<<count[i]<<endl;
    }
    avg=avg/isum;
    gettimeofday(&end,NULL);
    cout<<"Average is: "<<avg<<endl;
    cout<<"Time "<<-1*(start.tv_sec-end.tv_sec)-0.001*(start.tv_usec-end.tv_usec)<<endl;
    pthread_exit(0);
}
