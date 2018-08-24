#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
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
#include <vector>



using namespace std;

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64


char * server, * portno;
bool server_executed; 

char **tokenize(char *line);
void change_dir(char ** tokens);//cd
void listdirectorycontents(char ** tokens);//ls
void server_details(char ** tokens);//server
void getf1_first(char ** tokens);//getf1 with one argument
void getsq(char ** tokens);//getsq
void getfl_outfile(char ** tokens);//getfl > 
void getfl_pipe(char ** tokens);//getfl | 
void myexec(char ** tokens, bool whethertodisplay, int k);//utility function defined which generates the params for exec and calls it 

bool run; 
void signalHandler( int signum )
{
    //stop=true;
    signal(SIGINT, signalHandler);
    // cleanup and close up stuff here  
    // terminate program  
    run = 0;
   //exit(signum);  

}
char **tokenize(char *line)
{
    char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
    int i, tokenIndex = 0, tokenNo = 0;

    for(i =0; i < strlen(line); i++)
    {

        char readChar = line[i];

        if (readChar == ' ' || readChar == '\n' || readChar == '\t')
        {
            token[tokenIndex] = '\0';
            if (tokenIndex != 0)
            {
                tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
                strcpy(tokens[tokenNo++], token);
                tokenIndex = 0;
            }
        }
        else
        {
            token[tokenIndex++] = readChar;
        }
    }
    free(token);
    tokens[tokenNo] = NULL ;
    return tokens;
}


void myexec(char ** tokens, bool whethertodisplay, int k)//function to call exec in my way
{
    string my = "./get-one-file-sig";
    char * first = new char[20];
    strcpy(first, my.c_str());
    char ** second;
    second = new char*[6];
                    
    second[0] = new char[9];

    int j = 0;
    for(;tokens[k][j]!='\0';j++)
    {

    }
    j++;
    second[1] = new char[j];
    j = 0;
    for(;server[j]!='\0';j++)
    {

    }
    j++;
    second[2] = new char[j];
    
    j = 0;
    for(;portno[j]!='\0';j++)
    {

    }
    j++;
    second[3] = new char[j];
    

    second[4] = new char[8];
    strcpy(second[0], first);

    strcpy(second[1], tokens[k]);

    strcpy(second[2], server);
    strcpy(second[3], portno);
    if(whethertodisplay)
    {
        strcpy(second[4], "display");
    }
    else
    {
        strcpy(second[4], "nodisplay");
    }
    second[5] = NULL;
    if (execvp(first, second) < 0)       /* execute the command  */
    {
        printf("*** ERROR: exec failed in getf1 filename\n");
        return;
    }
    else 
    {
        return;
    }
}



void change_dir(char ** tokens)//function to change directory using chdir() call
{
    if(tokens[2] != NULL)
    {
        cout<<"Error in usage of cd command, more than required arguments passed"<<endl;
        return;
    }
    if(tokens[1] == NULL)
    {
        cout<<"Error in usage of cd command, less than required arguments passed"<<endl;
        return;
    }
    if(chdir(tokens[1]) < 0)
    {
        cout<<"Could not cd into it, folder does not exist"<<endl;
        return;
    }
    return;
}

    
void listdirectorycontents(char ** tokens)//function to change directory using exec on /bin/ls through execvp 
{
    if(tokens[1]!=NULL)
    {
        cout<<"No arguments expected to ls"<<endl;
        return;
    }
    pid_t  pid;

    if ((pid = fork()) < 0)
    {
        /* fork a child process           */

        printf("*** ERROR: forking child process failed in ls \n");

    }
    else if (pid == 0)            /* for the child process:         */
    {   
        if (execvp(tokens[0], tokens) < 0)       /* execute the command  */
        {
            printf("*** ERROR: exec failed in ls \n");
            return;
        }
    }
    else                                    /* for the parent:      */
    {
        int status;
        waitpid(pid,&status,0);
        return;
    }
}


void server_details(char ** tokens)//function to store the server ip address and portno
{
    if(tokens[2] == NULL || tokens[3] != NULL)
    {
        printf("Incorrect usage of server command\n");
        return;
    }
    server_executed = true;
    int j = 0;
    for(;tokens[1][j]!='\0';j++)
    {

    }
    j++;
    server = new char[j];
    j = 0;
    for(;tokens[2][j]!='\0';j++)
    {

    }
    j++;
    portno = new char[j];
    strcpy(server, tokens[1]);
    strcpy(portno, tokens[2]);
    return;
}

void getfl_first(char ** tokens)//function corresponds to get_fl filename
{
    if(tokens[1] == NULL)
    {
        printf("Too less arguments given to getf1 command, check usage\n");
        return;
    }
    if(!server_executed)
    {
        printf("Server details not yet known to connect, first use server command, then use this\n");
        return;
    }
    int k = 1;
    while(tokens[k]!=NULL)
    {
        pid_t  pid;
        if ((pid = fork()) < 0)
        {
            printf("*** ERROR: forking child process failed in getf1 filename\n");
            return;
        }
        else if (pid == 0)            /* for the child process:         */
        {   
            myexec(tokens, true, k);
        }
        else                                    /* for the parent:      */
        {
            int status;
            waitpid(pid,&status,0);
        }
        k++;
    }
    return;
}



void getsq(char ** tokens)//function corresponds to get_sq filename1 filename2 filename3.....
{
    if(tokens[1] == NULL)
    {
        printf("Too less arguments given to getf1 command, check usage\n");
        return;
    }
    if(!server_executed)
    {
        printf("Server details not yet known to connect, first use server command, then use this\n");
        return;
    }
    int k = 1;
    while(tokens[k]!=NULL)
    {
        pid_t  pid;
        if ((pid = fork()) < 0)
        {
            printf("*** ERROR: forking child process failed in getf1 filename\n");
            return;
        }
        else if (pid == 0)            /* for the child process:         */
        {   
            myexec(tokens, false, k);
        }
        else                                    /* for the parent:      */
        {
            int status;
            waitpid(pid,&status,0);
            if(!run)///if the process of a file is prematurely terminated using Ctrl C then run is set to 0 by signl handler and therfore subsequent sequence is not continued
            {
                break;
            }
        }
        k++;
    }
    return;
}


void getpl(char ** tokens)//function to download files parallely
{
    if(tokens[1] == NULL)
    {
        printf("Too less arguments given to getf1 command, check usage\n");
        return;
    }
    if(!server_executed)
    {
        printf("Server details not yet known to connect, first use server command, then use this\n");
        return;
    }
    int k = 1;
    while(tokens[k]!=NULL)
    {
        pid_t  pid;
        if ((pid = fork()) < 0)
        {
            /* fork a child process           */

            printf("*** ERROR: forking child process failed in getf1 filename\n");
            return;
        }
        else if (pid == 0)            /* for the child process:         */
        {   
            myexec(tokens, false, k);
        }
        k++;
    }
    while(waitpid(-1, 0, 0)>0)
    {
        continue;
    }
    return;
}

void getfl_outfile(char ** tokens)//function to get output in a file
{
    if(tokens[3]==NULL)
    {
        fprintf(stderr,"Too less arguments given to getfl with ouptut redirection\n");
        return;
    }
    if(tokens[4]!=NULL)
    {
        fprintf(stderr,"Too many arguments given to getfl with ouptut redirection\n");
        return;
    }
    pid_t pid;
    if ((pid = fork()) < 0)
    {
            /* fork a child process           */
        fprintf(stderr,"*** ERROR: forking child process failed in getfl outfile\n");
        return;
    }
    else if(pid == 0)
    {
        int fd = open(tokens[3], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        dup2(fd, 1);
        close(fd);
        myexec(tokens, true, 1);
        return;
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
    return;
}
void getfl_pipe(char ** tokens)//function to get output to pipe and redirecting it to input of another command
{
    if(tokens[3]==NULL)
    {
        fprintf(stderr,"Too less arguments given to getfl with ouptut redirection\n");
        return;
    }
    pid_t pid;
    int pipefd[2];//pipe
    pipe(pipefd);
    if ((pid = fork()) < 0)
    {
            /* fork a child process           */
        fprintf(stderr,"*** ERROR: forking child process failed in getfl outfile\n");
        return;
    }
    
    else if(pid == 0)
    {  
        dup2(pipefd[1],1);//linking stdout and writefd of pipe
        close(pipefd[1]);
        close(pipefd[0]);
        myexec(tokens, true, 1);
        return;
    }
    else
    {
        pid_t pid2;
        if((pid2 = fork()) < 0)
        {
            fprintf(stderr,"*** ERROR: forking child process failed in getfl outfile\n");
            return;
        }
        else if(pid2 == 0)
        {

            tokens++;
            tokens++;
            tokens++;
            dup2(pipefd[0],0);//linking stdin and readfd of pipe
            close(pipefd[1]);
            close(pipefd[0]);
            if (execvp(tokens[0], tokens) < 0)       /* execute the command  */
            {
                printf("*** ERROR: exec failed in cmd \n");
                return;
            }
        }
        int status;
        int status_2;
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid, &status, 0);
        waitpid(pid2, &status_2, 0);    
    }
    return;
}
void getbg(char ** tokens, vector<pid_t> & back)//function for background running
{
    if(tokens[1] == NULL)
    {
        printf("Too less arguments given to getbg command, check usage\n");
        return;
    }
    if(tokens[2]!=NULL)
    {
        printf("Too many arguments given to getbg command, check usage\n");
        return;
    }
    if(!server_executed)
    {
        printf("Server details not yet known to connect, first use server command, then use this\n");
        return;
    }
    int k = 1;
    while(tokens[k]!=NULL)
    {
        pid_t  pid;
        if ((pid = fork()) < 0)
        {
            /* fork a child process           */

            printf("*** ERROR: forking child process failed in getbg filename\n");
            return;
        }
        else if (pid == 0)            /* for the child process:         */
        {   
            myexec(tokens, false, k);
        }

		else
		{
			setpgid(pid,pid);//changing group pid so that it doesnt terminate on Ctrl C
			back.push_back(pid);//storing pid
		}
        k++;
    }
    return;
}

void my_exit(char ** tokens, vector<pid_t> & back)
{
    for(int i = 0; i<back.size(); i++)//killing all background processes appropriately and reaping
    {
        kill(back[i], SIGINT);
        int status;
        int killed_pid = waitpid(back[i],&status,0);
    }
    exit(0);
}

void gre(char ** tokens)//grep
{
    if(tokens[1]!=NULL)
    {
        cout<<"No arguments expected to grep"<<endl;
        return;
    }
    pid_t  pid;

    if ((pid = fork()) < 0)
    {
        /* fork a child process           */

        printf("*** ERROR: forking child process failed in greo \n");

    }
    else if (pid == 0)            /* for the child process:         */
    {   
        if (execvp(tokens[0], tokens) < 0)       /* execute the command  */
        {
            printf("*** ERROR: exec failed in grep \n");
            return;
        }
    }
    else                                    /* for the parent:      */
    {
        int status;
        waitpid(pid,&status,0);
        return;
    }
}
void eco(char ** tokens)//echo
{
    
    pid_t  pid;

    if ((pid = fork()) < 0)
    {
        /* fork a child process           */

        printf("*** ERROR: forking child process failed in echo \n");

    }
    else if (pid == 0)            /* for the child process:         */
    {   
        if (execvp(tokens[0], tokens) < 0)       /* execute the command  */
        {
            printf("*** ERROR: exec failed in echo \n");
            return;
        }
    }
    else                                    /* for the parent:      */
    {
        int status;
        waitpid(pid,&status,0);
        return;
    }
}
void mycat(char ** tokens)//cat
{
    if(tokens[1]!=NULL)
    {
        cout<<"No arguments expected to cat"<<endl;
        return;
    }
    pid_t  pid;

    if ((pid = fork()) < 0)
    {
        /* fork a child process           */

        printf("*** ERROR: forking child process failed in cat \n");

    }
    else if (pid == 0)            /* for the child process:         */
    {   
        if (execvp(tokens[0], tokens) < 0)       /* execute the command  */
        {
            printf("*** ERROR: exec failed in cat \n");
            return;
        }
    }
    else                                    /* for the parent:      */
    {
        int status;
        waitpid(pid,&status,0);
        return;
    }
}

int main()
{
    signal(SIGINT, signalHandler);
    run = 1;
    char  line[MAX_INPUT_SIZE];
    char  **tokens;
    int i;
    server_executed = false;
    vector<pid_t> back;
    while (true)
    {
        run = 1;
        int status;
        int killed_pid;
        for(int p=0;p<back.size();p++)//reaping dead background processes periodically
        {
        	killed_pid = waitpid(back[p],&status,WNOHANG);
        	if(killed_pid>0)
        	{
           		 printf("Background process with a pid of %d finished\n", killed_pid);
        	}
        }
        
        printf("Hello>");
        bzero(line, MAX_INPUT_SIZE);
        gets(line);
        printf("Got command %s\n", line);
        line[strlen(line)] = '\n'; //terminate with new line
        tokens = tokenize(line);
        if(strcmp(tokens[0],"cd")==0)
        {
            change_dir(tokens);
            continue;
        }

        //ls
        if(strcmp(tokens[0],"ls")==0)
        {
            listdirectorycontents(tokens);
            continue;
        }

        if(strcmp(tokens[0],"server")==0)
        {
            server_details(tokens);
            continue;
        }
        
        if(strcmp(tokens[0],"getfl")==0 )
        {
            if(tokens[1] == NULL)
            {
                printf("Too less arguments to getfl\n"); 
                continue;   
            }
            if(tokens[2] == NULL)
            {
                getfl_first(tokens);
                continue;
            }
            if(strcmp(tokens[2],">")==0)
            {
                getfl_outfile(tokens);
                continue;
            } 
            if(strcmp(tokens[2],"|")==0)
            {
                getfl_pipe(tokens);
                continue;
            }
            printf("Incorrect usage of the command getfl\n");
            continue;
        }

        if(strcmp(tokens[0],"getsq")==0)
        {
            getsq(tokens);
            continue;
        }

        if(strcmp(tokens[0],"getpl")==0)
        {
            getpl(tokens);
            continue;
        }

        if(strcmp(tokens[0],"getbg")==0)
        {
            getbg(tokens, back);
            continue;
        }

        if(strcmp(tokens[0],"grep")==0)
        {
            gre(tokens);
            continue;
        }
         if(strcmp(tokens[0],"echo")==0)
        {
            eco(tokens);
            continue;
        }
         if(strcmp(tokens[0],"cat")==0)
        {
            mycat(tokens);
            continue;
        }

        if(strcmp(tokens[0],"exit")==0)
        {
            my_exit(tokens, back);
            continue;
        }
        printf("No such command found\n");
        //continue;

        // Freeing the allocated memory
        for(i=0; tokens[i]!=NULL; i++)
        {
            free(tokens[i]);
        }
        free(tokens);
            
    }
}


