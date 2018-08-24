#include  <stdio.h>
#include  <sys/types.h>
#include  <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<vector>
#include<iostream>
using namespace std;
#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
vector<pid_t> back;
void handler(int signum)
{

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
char * filename;
FILE* fp;
void sim(char** tokens,bool bg,bool f)
{
    pid_t pid;
    if(f==1)
    {

        fp=fopen(filename,"w");
    }
    if((pid=fork())<0)
    {
        cout<<"Error in fork ";
    }
    else if(pid==0)
    {
        if(f==1)
        {
            //dup2(fp,filename);
        }
        execvp(tokens[0],tokens);
    }
    else
    {
        //dup2(fp,1);
        if(bg==0)
        {
            int status;
            waitpid(pid,&status,0);
        }
        else
        {
            setpgid(pid,pid);
            back.push_back(pid);
        }
    }
    return;
}
void chd(char** tokens)
{
    if(chdir(tokens[1])<0)
    {
        cout<<"Cannot change directory"<<endl;
    }
    return;
}
void caller(char** newtokens,bool bg)
{
    /*for(int i=0;newtokens[i]!=NULL;i++){
     printf("found token %s\n", newtokens[i]);
               }
               cout<<"over"<<endl;*/
    bool f=0;
    int n=0;
    bool fla=0;
    int pos=0;
    while(newtokens[n]!=NULL)
    {
        if(strcmp(newtokens[n],">")==0)
        {
            fla=1;
            f=1;
            pos=n;
            break;
        }
        n++;
    }
    if(fla==1)
    {
        filename=newtokens[pos];
        if(newtokens[pos+1]==NULL||newtokens[pos+2]!=NULL)
        {
            cout<<"Error in input"<<endl;
        }
        else
        {
            char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
            int k=0;
            int j=0;//cout<<"lan"<<endl;
            while(newtokens[k]!=NULL)
            {
                if(strcmp(newtokens[k],">")!=0)
                {
                    tokens[j]=newtokens[k];
                    j++;
                }
                else
                {
                    sim(newtokens,bg,f);
                    break;
                    //cout<<newtokens[0]<<endl;
                }
                k++;
            }
        }
        //if(bg==0)close(fp);

        return;
    }
    if(strcmp(newtokens[0],"cd")==0)
    {
        if(newtokens[2]!=NULL||newtokens[1]==0)
        {
            cout<<"Incorrect arguments number"<<endl;
        }
        else
        {
            chd(newtokens);
        }
        return;
    }
    if(strcmp(newtokens[0],"ls")==0)
    {
        sim(newtokens,bg,f);
        return;
    }
    if(strcmp(newtokens[0],"cat")==0)
    {
        sim(newtokens,bg,f);
        return;
    }
    if(strcmp(newtokens[0],"echo")==0)
    {
        sim(newtokens,bg,f);
        return;
    }
    if(strcmp(newtokens[0],"exit")==0)
    {
        for(int q=0; q<back.size(); q++)
        {
            kill(back[q],SIGINT);
            int id=waitpid(back[q],0,WNOHANG);
            if(id>0)cout<<"Background process with pid "<<id<<" terminated"<<endl;
        }
        exit(0);
    }

    sim(newtokens,bg,f);
    return;
}
int main()
{
    signal(SIGINT,handler);
    char  line[MAX_INPUT_SIZE];
    char  **tokens;
    int i;
    while (1)
    {

        for(int q=0; q<back.size(); q++)
        {
            int id=waitpid(back[q],0,WNOHANG);
            if(id>0)cout<<"Background process with pid "<<id<<" completed"<<endl;
        }
        printf("Hello>");
        bzero(line, MAX_INPUT_SIZE);
        gets(line);
        printf("Got command %s\n", line);
        line[strlen(line)] = '\n'; //terminate with new line
        tokens = tokenize(line);

        //do whatever you want with the commands, here we just print them
        int k=0;
        int j=0;
        int l=0;
        vector< char** > A;
        char **newtokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
        A.push_back(newtokens);
        int flag=0;
        bool bg=0;
        while(tokens[k]!=NULL)
        {
            if(strcmp(tokens[k],";;")==0)
            {
                flag=1;
                break;
            }
            else if(strcmp(tokens[k],"&&")==0)
            {
                flag=2;
                cout<<"lan"<<endl;
                break;
            }
            k++;
        }
        k=0;
        if(flag==1)
        {
            while(tokens[k]!=NULL)
            {
                if(strcmp(tokens[k],";;")!=0)
                {
                    A[l][j]=tokens[k];
                    j++;
                }
                else
                {
                    caller(A[l],bg);
                    char **newtokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
                    A.push_back(newtokens);
                    j=0;
                    l++;
                    //cout<<newtokens[0]<<endl;
                }
                k++;
            }
            caller(A[l],bg);
        }
        else if(flag==2)
        {
            bg=1;
            while(tokens[k]!=NULL)
            {
                if(strcmp(tokens[k],"&&")!=0)
                {
                    A[l][j]=tokens[k];
                    j++;
                }
                else
                {
                    caller(A[l],bg);
                    char **newtokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
                    A.push_back(newtokens);
                    j=0;
                    l++;
                }
                k++;
            }
            caller(A[l],bg);
        }
        else
        {
            bg=0;
            caller(tokens,bg);
        }

        for(i=0; tokens[i]!=NULL; i++)
        {
            printf("found token %s\n", tokens[i]);
        }

        // Freeing the allocated memory
        for(i=0; tokens[i]!=NULL; i++)
        {
            free(tokens[i]);
        }
        free(tokens);
    }


}


