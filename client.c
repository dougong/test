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
    if (argc != 3)
    {
        printf("Usage: %s ip port", argv[0]);
        exit(1);
    }

    signal(SIGALRM, timer); //relate the signal and function  
  
    alarm(1);       //trigger the timer  

    printf("This is a UDP client\n");
    struct sockaddr_in addr;
    int sock;

    if ( (sock=socket(AF_INET, SOCK_DGRAM, 0)) <0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Incorrect ip address!");
        close(sock);
        exit(1);
    }

    char buff[512] = "i am a test program! hello world!\n";
    int len = sizeof(addr);
    while (1)
    {
        int n;
        n = sendto(sock, buff, strlen(buff), 0, (struct sockaddr *)&addr, sizeof(addr));
	count++;
	//printf("count:%d\n",count);
        if (n < 0)
        {
            perror("sendto");
            close(sock);
            break;
        }
       // n = recvfrom(sock, buff, 512, 0, (struct sockaddr *)&addr, &len);
       // if (n>0)
       // {
       //     buff[n] = 0;
            //printf("received:");
            //puts(buff);
        //}
        else if (n==0)
        {
            printf("server closed\n");
            close(sock);
            break;
        }
        else if (n < -1)
        {
            perror("recvfrom");
            close(sock);
            break;
        }
    }
    
    return 0;
}
