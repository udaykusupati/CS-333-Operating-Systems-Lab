/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     char buffer[4098];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     /* create socket */

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     /* fill in port number to listen on. IP address can be anything (INADDR_ANY) */

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     /* bind socket to this port number on this machine */

     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     
     /* listen for incoming connection requests */

     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     int tr=0;
while(tr==0){
     /* accept a new request, create a newsockfd */

     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");

     /* read message from client */

     bzero(buffer,4098);
     n = read(newsockfd,buffer,4097);
     if (n < 0) error("ERROR reading from socket");
     //printf("Here is the message: %s\n",buffer);

     if(buffer[0]=='1')
     {
        int i=1;
        for(i=1;i<strlen(buffer);i++)
        {
            buffer[i]=buffer[i]+'1';
        }
     }
     else
     {
        int i=1;
        for(i=1;i<strlen(buffer);i++)
        {
            buffer[i]=buffer[i]-'1';
        }
     }
     /* send reply to client */

     n = write(newsockfd,&buffer[1],4096);
     if (n < 0) error("ERROR writing to socket");
    }
     return 0; 
}
