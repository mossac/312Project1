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
char othername[256];

void main()
{
    char ip[256];

    printf("Provide User Name: ");
    bzero(name, 256);
    fgets(name, 255, stdin);
    name[strlen(name) - 1] = '\0';
    

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

    n = write(sockfd, name, strlen(name));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(othername, 256);
    if (read(sockfd, othername, 256) < 0)
    {
        error("ERROR reading from socket");
    }
   // printf("other name is: %s", othername);
    //send my name

    int fresult = fork();
    if (fresult < 0)
    {
        printf("ERROR FORKING\n");
    }
    if (fresult == 0)
    {
        printf("Transmitting Side on\n");
        while (1)

        {

            printf("Please enter the message: ");
            bzero(buffer, 256);
            fgets(buffer, 255, stdin);
            //check to see if you can quit
           
          
            //  printf("Buffer: %s\n", buffer);
            n = write(sockfd, buffer, strlen(buffer));
             if (strcmp(buffer, "quit\n") == 0)
            {
                printf("QUIT!");
                quit = true;
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
            n = read(sockfd, recieved, 255);
            if (n < 0)
                error("ERROR reading from socket");
            printf("%s\n", recieved);
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