#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;
long long int sum;

void *prin (void *arg)
{
int s2 = (int) arg;
int done=0, n,tempsum=0; 
char string[100];
printf("Connected.\n");
do {
        n = recv(s2, string, 100, 0);
	if (!n)  break;
	string[n] = '\0';
	tempsum++;
	int nums=1,i,j,z,A[nums];
	for(i=0;string[i]!='\0';i++)
		if(string[i]==' ')
			nums++;	
	char temp[10];j=0;z=0;
	for(i=0;string[i]!='\0';i++)
	{
		if(string[i]!=' ')//to keno
		{	temp[j]=string[i];
			j++;}
		else
		{	temp[j]='\0';//EOF
			A[z]=atoi(temp);
			j=0;z++;}
	}
		temp[j]='\0';
		A[z]=atoi(temp);
	double x=0;
	for(i=0;i<nums;i++)
		x=x+A[i];
	m=m/(double)nums;
	
	for(i=0;i<nums;i++)
		printf("%d ",A[i]);
	printf("\n"); 
	char str_back[20];
	if(x>10)
		{sprintf(str_back,"%.2f",x);
		strcat(str_back, "YES");}//it is ok
	else
		strncpy(str_back,"NO", 20);//it has failed
	send(s2, str_back, strlen(str_back), 0);
        if (n==1) done = 1; 
} while (!done);
close(s2);

pthread_mutex_lock(&mut);
sum=sum+tempsum;//critical
pthread_mutex_unlock(&mut);
pthread_exit(NULL);
}
int main(void)
    {
        int socket, s2, i, t, lenght;
        struct sockaddr_un local, remote;
        pthread_t thread[50];
        if ((socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }
        local.sun_family = AF_UNIX;
        strcpy(local.sun_path, "echo_socket");
        unlink(local.sun_path);
        lenght = strlen(local.sun_path) + sizeof(local.sun_family);
        if (bind(socket, (struct sockaddr *)&local, lenght) == -1) {
            perror("bind");
            exit(1);
        }
        if (listen(socket, 5) == -1) {
            perror("listen");
            exit(1);
        }
        i=0;
	long long int clients=0;
        for(;;) {
            printf("Waiting for a connection...\n");
            t = sizeof(remote);
            if ((s2 = accept(socket, (struct sockaddr *)&remote, &t)) == -1) {
                perror("accept");
                exit(1);
            }
            pthread_create(&(thread[i++]), NULL, prin, (void *)s2);		
	    clients++;
	    printf("CLIENT:  %d \n Sucesfull Processes: %d \n",clients,sum);
         }
        return 0;
    }
