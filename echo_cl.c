#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define SOCK_PATH "echo_socket"
    int main(void)
    {
        int socket, t, lenght, done=0;
        struct sockaddr_un remote;
        char string[100];
        if ((socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
            perror("socket");//socket error
            exit(1);
        }
        printf("CONNECTING...\n");
        remote.sun_family = AF_UNIX;
        strcpy(remote.sun_path,"echo_socket");
        lenght = strlen(remote.sun_path) + sizeof(remote.sun_family);
        if (connect(socket, (struct sockaddr *)&remote, lenght) == -1) {
            perror("CONNECT");//conection fail
            exit(1);
        }
        printf("Connected.\n");//else conected
        do {
           printf("> ");
           fgets(string, 100, stdin);
	   if(string[0]=='\n')	break;//mexri na patithei to enter "\n"
           send(socket, string, strlen(string), 0); 
           t=recv(socket, string, 100, 0);
           string[t] = '\0';
           printf("echo> %s\n", string);
	   printf("Press enter to stop\n");
           if (t==1) done = 1;
         } while (!done);
        close(socket);
        return 0;
    }
