/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <queue>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <queue>
using namespace std;
void error(char *msg)
{
    perror(msg);
    exit(1);
}
queue<int> Q;
pthread_mutex_t M= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t has_space=PTHREAD_COND_INITIALIZER;
pthread_cond_t has_clients=PTHREAD_COND_INITIALIZER;
void * func(void * tid)
{
	int fd;
	while(1)
	{
		//cout<<(long)tid<<endl;
		pthread_mutex_lock(&m);
		while(Q.size()==0)
		{
			//cout<<"waiting for clients"<<endl;
			pthread_cond_wait(&has_clients,&m);
		}
		fd=Q.front();
		//cout<<"new work "<<fd<<endl;
		Q.pop();
		pthread_cond_signal(&has_space);
		pthread_mutex_unlock(&m);

			char buffer[31];
            bzero(buffer,31);
            recv(fd, buffer, 30, 0);
            //cout<<&buffer[4]<<endl;
            FILE* fp=fopen(&buffer[4],"r");

            if(fp==NULL)
            {
                cout<<"error opening file"<<endl;
            }
            else
            {
                //cout<<buffer<<" file opened"<<endl;

                fseek (fp , 0 , SEEK_END);
                int filesize = ftell (fp);
                rewind (fp);
                //cout<<filesize<<endl;
                int total=0;
                while(total<filesize)
                {
                    char s[513];
                    bzero(s, 513);
                    int Read = fread(s,1,512,fp);
                    total+=send(fd,s, Read, 0);
                }
                //cout<<total<<endl;
            }
            fclose(fp);
            close(fd);
            //cout<<"work done"<<endl;

	}
	int retval;
	pthread_exit(&retval);
}
int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr;
    struct sockaddr_storage cli_addr;
    int n;
    if (argc < 4)
    {
        fprintf(stderr,"ERROR, incorrect no of provided\n");
        exit(1);
    }

    /* create socket */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        cout<<"ERROR opening socket"<<endl;

    /* fill in port number to listen on. IP address can be anything (INADDR_ANY) */

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    int yes=1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    /* bind socket to this port number on this machine */

    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        cout<<"ERROR on binding"<<endl;
    clilen = sizeof cli_addr ;
    //int pid;
    int no_threads=atoi(argv[2]);
    pthread_t* threads=new pthread_t[no_threads];
    int bound=atoi(argv[3]);

    /* listen for incoming connection requests */
    listen(sockfd,bound);
    for(long t=0;t<no_threads;t++)
    {
    	if(pthread_create(&threads[t],NULL,&func,(void *)t)<0)
    	{
    		cout<<"Error creating thread "<<t<<endl; 
    	}
    }
    /* accept a new request, create a newsockfd */
    bool open=1;
    while(1)
    {
        //int wpid=waitpid(-1,0,WNOHANG);
        //cout<<wpid<<"Child reaped"<<endl;
        pthread_mutex_lock(&m);
        while(Q.size()>=bound&&bound!=0)
        {
        	//open=0;
        	//close(sockfd);
        	pthread_cond_wait(&has_space,&m);
        }
        pthread_mutex_unlock(&m);
        if(!open)
        {
		        	  /* create socket */

		    sockfd = socket(AF_INET, SOCK_STREAM, 0);
		    if (sockfd < 0)
		        cout<<"ERROR opening socket"<<endl;
				    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		    /* bind socket to this port number on this machine */

		    if (bind(sockfd, (struct sockaddr *) &serv_addr,
		             sizeof(serv_addr)) < 0)
		        cout<<"ERROR on binding"<<endl;
		    listen(sockfd,bound);
		    open=1;
        }
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            cout<<"ERROR on accept"<<endl;
        pthread_mutex_lock(&m);
        Q.push(newsockfd);
        //cout<<"new client "<<newsockfd<<endl;
       	pthread_cond_signal(&has_clients);
		pthread_mutex_unlock(&m);
		//cout<<"HI"<<endl;
       /* if ((pid = fork()) == -1)
        {
            close(newsockfd);
            //continue;
        }
        else if(pid > 0)
        {
            //cout<<pid<<"Child created"<<endl;
            close(newsockfd);
            //continue;
        }
        else if(pid == 0)
        {
            char buffer[21];
            bzero(buffer,21);
            recv(newsockfd, buffer, 20, 0);
            //cout<<buffer<<endl;
            FILE* fp=fopen(buffer,"r");
            if(fp==NULL)
            {
                cout<<"error opening file"<<endl;
            }
            else
            {
                //cout<<buffer<<" file opened"<<endl;

                fseek (fp , 0 , SEEK_END);
                int filesize = ftell (fp);
                rewind (fp);
                //cout<<filesize<<endl;
                int total=0;
                while(total<filesize)
                {
                    char s[513];
                    bzero(s, 513);
                    int Read = fread(s,1,512,fp);
                    total+=send(newsockfd,s, Read, 0);
                }
                //cout<<total<<endl;
            }
            close(newsockfd);
            break;
        }*/
    }
    return 0;
}
