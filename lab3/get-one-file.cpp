#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <string>
#include <pthread.h>
#include <ctime>
#include <unistd.h>
#include <sys/time.h>
#include <csignal>

using namespace std;

int PORTNO;
struct hostent *server;
int num;
int time_total; 
int sleep_time = 0;
string filetyperequested;
string filenamerequested;
bool whetherdisplay;
int tot_received_sofar;
struct thread_data
{
    int sockfd,random,total,Received;
    string filename;
    stringstream randomc;
    char buffer[512];
    int filesize;
    FILE* fp;
    struct sockaddr_in serv_addr;
    int requests = 0;
    float response_time = 0;
    float response_time_final = 0;
    
};
void error(char *msg)
{
    perror(msg);
    exit(0);
}
thread_data* data;

void signalHandler( int signum )
{
    cout << "Received SIGINT; Downloaded "<<tot_received_sofar<<" bytes so far"<<endl;
   exit(signum);  

}
void *func(void *tid)
{
    time_t start_time = time(NULL);
    long id;
    id = (long)tid;

    /* create socket, get sockfd handle */
    data[id].sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (data[id].sockfd < 0)
        fprintf(stderr,"ERROR opening socket\n");

    /* fill in server address in sockaddr_in datastructure */
    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &data[id].serv_addr, sizeof(data[id].serv_addr));
    data[id].serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&data[id].serv_addr.sin_addr.s_addr,
          server->h_length);
    data[id].serv_addr.sin_port = htons(PORTNO);

    /* connect to server */
    
    if (connect(data[id].sockfd,(struct sockaddr *)&data[id].serv_addr,sizeof(data[id].serv_addr)) < 0)
        fprintf(stderr,"ERROR connecting\n");
        
////////////////////////////////////////////////////////////////////////////
    
    data[id].filename="get ";
    data[id].filename+=filenamerequested;
    
    timeval request_start_time;
    gettimeofday(&request_start_time,0);
    send(data[id].sockfd, data[id].filename.c_str(), strlen(data[id].filename.c_str()), 0);

   
    bool first_byte_received = false;
    data[id].total=0;
    while(true)
    {

        bzero(data[id].buffer, 512);
        data[id].Received = recv(data[id].sockfd, data[id].buffer, 511, 0);

        if(data[id].Received==0)
        {
            break;
        }
        if(!first_byte_received)
        {
            first_byte_received = true;
            timeval request_end_time;
            gettimeofday(&request_end_time, 0);
            data[id].response_time+=1000 * (request_end_time.tv_sec - request_start_time.tv_sec) + (request_end_time.tv_usec - request_start_time.tv_usec)/1000;;
        }
        if(whetherdisplay)
        {
            cout<<data[id].buffer; 
        } 
        data[id].total+=data[id].Received;
        tot_received_sofar = data[id].total;
    }
   
    {
        timeval request_end_time_final;
        gettimeofday(&request_end_time_final, 0);
        data[id].response_time_final+=1000 * (request_end_time_final.tv_sec - request_start_time.tv_sec) + (request_end_time_final.tv_usec - request_start_time.tv_usec)/1000;
        
    }
    data[id].requests++;
    close(data[id].sockfd);
    sleep(sleep_time);
}


int main(int argc, char *argv[])
{
    time_t main_start_time = time(NULL);
    //cout<<"hello"<<endl;
    ////////////////////////////////////////////////////////////////////////////
    signal(SIGINT, signalHandler);
    tot_received_sofar = 0;
    if (argc < 5)
    {
        fprintf(stderr,"usage %s file_name hostname port display \n", argv[0]);
        exit(0);
    }
    srand(time(NULL));
    PORTNO = atoi(argv[3]);
    server = gethostbyname(argv[2]);
    num = 1;
    //time_total = atoi(argv[4]);
    //sleep_time = 0;
    filenamerequested = argv[1];
    string fourth_argument(argv[4]);
    if(fourth_argument == "display")
    {
        whetherdisplay = true;
    }
    else 
    {
        whetherdisplay = false;
    }

    data=new thread_data[num];
    pthread_t threads[num];
    int rc;
    int i;
    for( i=0; i < num; i++ )
    {
        //cout << "main() : creating thread, " << i << endl;
        rc = pthread_create(&threads[i], NULL,func,(void *)i);
        if (rc)
        {
            fprintf(stderr,"Error:unable to create thread\n");
            //cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }
    void *status;
    for( i=0; i < num; i++ ){
      rc = pthread_join(threads[i], &status);
      if (rc){
         fprintf(stderr,"Error:unable to join\n");
         //cout << "Error:unable to join," << rc << endl;
         exit(-1);
      }
   }
    time_t end_time = time(NULL);
    int total_req_served = 0;
    float total_response_time = 0;
    float total_response_time_final = 0;
    for(int i = 0;i<num;i++)
    {
        total_req_served+=data[i].requests;
        total_response_time+=data[i].response_time;
        total_response_time_final+=data[i].response_time_final;
    }
    int total_run_time = end_time-main_start_time;
    pthread_exit(NULL);
    return 0;
}
