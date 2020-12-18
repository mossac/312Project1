#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include <unistd.h>

#define PORT 4567
#define CLADDR_LEN 100
char name[256];
char othername[256];
char clientAddr[CLADDR_LEN];

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    printf("Provide User Name: ");
    bzero(name, 256);
    fgets(name, 255, stdin);

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = PORT;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0)
        error("ERROR on accept");

    //get other name
    bzero(othername, 256);
    if (read(newsockfd, othername, 256) < 0)
    {
        error("ERROR reading from socket");
    }
    printf("other name is: %s", othername);
    //send my name
    n = write(newsockfd, name, strlen(name));
    if (n < 0)
        error("ERROR writing to socket");

    inet_ntop(AF_INET, &(cli_addr.sin_addr), clientAddr, CLADDR_LEN);
    printf("Connection accepted from %s...\n", clientAddr);

    int pid = fork();
    if (pid < 0)
    {
        printf("ERROR FORKING\n");
    }
    if (pid == 0)
    {
        while (1)
        {
            printf("Please enter the message: ");
            bzero(buffer, 256);
             //printf("stdin\n");
            fgets(buffer, 255, stdin);
            //check to see if you can quit

            //printf("Buffer: %s , %d\n", buffer, newsockfd);
            n = write(newsockfd, buffer, strlen(buffer));
            //printf("printf, %d\n", sockfd);
            if (strcmp(buffer, "quit\n") == 0)
            {
                printf("QUIT!");
            
                kill(0, SIGINT);
                exit(1);
            }
        }
    }

    else
    {
        char recieved[256];
        for (;;)
        {
            bzero(recieved, 256);
            n = read(newsockfd, recieved, 256);
            if (n < 0)
                error("ERROR reading from socket");

            printf("\n<%s> %s\n", othername, recieved);
            fflush(stdout);
            if (strcmp(recieved, "quit\n") == 0)
            {
                printf("\nUSER HAS DISCONNECTED!");
                fflush(stdout);
                kill(0, SIGINT);
            }
    }
    }
}
