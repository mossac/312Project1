#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define PORT 4567

#define BUF_SIZE 256
#define CLADDR_LEN 100
char IP[] = "172.17.14.247";
bool quit = false;
char name[256];

void main()
{
    char ip[256];

    printf("Provide User Name: ");
    bzero(name, 256);
    fgets(name, 255, stdin);
    name[strlen(name) - 1] = '\0';
    strcat(name, ": ");

    // printf("name : %s \n",name);
    // n = write(sockfd, buffer, strlen(buffer));
    //printf("Your Name:%s", buffer);
   
            idk();
        
    
}

int idk(int argc, char *argv[])
{

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    char sample[256];

    portno = PORT;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        herror("ERROR opening socket");
    server = gethostbyname(IP);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        herror("ERROR connecting");
    int fresult = fork();
    if(fresult < 0){
        printf("ERROR FORKING\n");
    }
    if (fresult == 0){
        printf("Transmitting Side on\n");
    while (1)
    
        
        {
       
            
            printf("Please enter the message: ");
            bzero(buffer, 256);
            fgets(buffer, 255, stdin);
            //check to see if you can quit
            if (strcmp(buffer, "quit\n") == 0)
            {
                printf("QUIT!");
                quit = true;
                kill(0, SIGINT);
                exit(1);
            }
            char temp[strlen(name)];

            strcpy(temp, name);
            strcat(temp, buffer);
            printf("Buffer: %s\n", buffer);
            n = write(sockfd, temp, strlen(temp));
        }
    }
    else  {
        printf("Recieving Side on\n");
        wait(1) ;
       while (1)
        {
            
            if (n < 0)
                herror("ERROR writing to socket");

            bzero(buffer, 256);
            n = read(sockfd, buffer, 255);

            if (n < 0)
                herror("ERROR reading from socket");
            printf("%s\n", buffer);
            //fflush(stdout);
        }
    }
}