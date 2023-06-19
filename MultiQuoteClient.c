//
//  MultiQuoteClient
//
//  Created by Maxim Carr
//  Username: psymc9
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>

#define kMULTIQUOTEPORT 1818

void serverResponse(int,char*,int);
int main(int argc, const char *argv[])
{
    char *inptr, buffer[256];
    char anotherOne[] = "ANOTHER\r\n";
    char endConn[] = "CLOSE\r\n";
    int numOfQuotes,sd,ok,i;
    struct sockaddr_in addr;
    struct hostent *ghost;

    //Open connection
    sd=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(kMULTIQUOTEPORT);
    ghost = gethostbyname(argv[1]);
    memcpy(&addr.sin_addr,ghost->h_addr_list[0],ghost->h_length);
    ok = connect(sd,(struct sockaddr*)&addr,sizeof(addr));
    
    numOfQuotes=strtol(argv[2],&inptr,10);
    if(ok==0){
        for(i=0;i<numOfQuotes;i++)
        {
            serverResponse(sd,buffer,256);
            printf("%s\n\n",buffer);
            write(sd,anotherOne,strlen(anotherOne));
        }
        write(sd,endConn,strlen(endConn));
    }
    close(sd);
    return 0;
}
void serverResponse(int sd, char *buffer,int size)
{
    char lb[8];
    int n,i,j=0,endLine=0;
    do
    {
        n=read(sd,lb,8);
        for(i=0;i<n;i++)
        {
            buffer[j]=lb[i];
            if(buffer[j]==10&&buffer[j-1]==13)
            {
                buffer[j-1]='\0';
                endLine =1;
                break;
            }
            j++;
        }
    }while((endLine==0)&&(n>0));
}
