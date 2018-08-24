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
using namespace std;
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr;
    struct sockaddr_storage cli_addr;
    int n;
    if (argc < 2)
    {
        fprintf(stderr,"ERROR, no port provided\n");
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

    /* listen for incoming connection requests */

    listen(sockfd,20);
    clilen = sizeof cli_addr ;
    int pid;
    /* accept a new request, create a newsockfd */
    for(;;)
    {
        int wpid=waitpid(-1,0,WNOHANG);
        //cout<<wpid<<"Child reaped"<<endl;

        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            cout<<"ERROR on accept"<<endl;
        if ((pid = fork()) == -1)
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
            char buffer[32];
            bzero(buffer,32);
            recv(newsockfd, buffer, 31, 0);
            //char buffer[18];
            //cout<<buff<<endl;
            /*for(int i = 0;i<18;i++)
            {
                buffer[i] = buff[i+4];
            }*/

            //cout<<buffer<<endl;
            FILE* fp=fopen(&(buffer[4]),"r");
            if(fp==NULL)
            {
                //cout<<&(buffer[4])<<endl;
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
                    char s[512];
                    bzero(s, 512);
                    int Read = fread(s,1,512,fp);
                    total+=send(newsockfd,s, Read, 0);
                }
                //cout<<total<<endl;
            }
            close(newsockfd);
            break;
        }
    }

    return 0;
}
