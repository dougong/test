#include <sys/types.h>
#include <sys/socket.h>
#include<pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int count = 0;

void timer(int sig)  
{  
        if(SIGALRM == sig)  
        {  
                printf("count:%d\n",count);
		count = 0;  
                alarm(1);       //we contimue set the timer  
        }  
  
        return;  
}  


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s port\n", argv[0]);
        exit(1);
    }

    signal(SIGALRM, timer); //relate the signal and function  
  
    alarm(1);       //trigger the timer  
  
    printf("Welcome! This is a UDP server, I can only received message from client and reply with same message\n");
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int sock;
    if ( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }
    char buff[512];
    struct sockaddr_in clientAddr;
    int n;
    int len = sizeof(clientAddr);

    int i = 0;
    pid_t pid;

    printf("master proess:%d\n",getpid());

    for (i = 0; i < 3; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            printf("error!\n");
        }
	else if(pid == 0)
        {
            continue;
        }
	else
	{
	    printf("child pid:%d\n",pid);
            break;
	}
    }

    while (1)
    {
        n = recvfrom(sock, buff, 511, 0, (struct sockaddr*)&clientAddr, &len);
	//printf("cur proess:%d,count:%d\n",getpid(),count);
        if (n>0)
        {
	    count++;
            buff[n] = 0;
            //n = sendto(sock, buff, n, 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
           // if (n < 0)
           // {
           //     perror("sendto");
           //     break;
           // }
        }
        else
        {
            perror("recv");
            break;
        }
    }
    return 0;
}
