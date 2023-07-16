
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#define Buffer_SIZE 1024


int main(int argc,char *argv[]) {
    char incomingByte[Buffer_SIZE];
    int dataToreceive;
    int i = 0,check_bind;
    int num_of_packets = atoi(argv[3]);
    int portnumber=atoi(argv[2]);
    unsigned long int sum;
    unsigned long int actualSum = 0;
    sum = num_of_packets * Buffer_SIZE;
    struct sockaddr_in server_addr; //address of socket
    socklen_t addr_size=sizeof(server_addr);
    int servsockt = socket(AF_INET, SOCK_DGRAM, 0);

    if (servsockt == -1) {
        perror("socket opening error!!");
        return (-1);
    }
    server_addr.sin_family = AF_INET; // match the socket() call
    server_addr.sin_addr.s_addr = inet_addr(argv[1]); // bind to any local address
    server_addr.sin_port = htons(portnumber);
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero)); // zero structure
    //binding:
    check_bind = bind(servsockt, (struct sockaddr *) &server_addr, sizeof(server_addr));
   if(check_bind ==-1){
	perror("bind error!!");
	return(-1);
   }
   printf("Listening for UDP data:\n");
    while( i < num_of_packets) {
        dataToreceive = recv(servsockt, incomingByte, Buffer_SIZE, 0);
        if(dataToreceive<0)
        {
            perror("error occured!!-while reading");
            close(servsockt);
            return(-1);
        }
        else
        {
            printf("The Data is : %s",incomingByte);  //here we print the buffer to the screen
            actualSum += dataToreceive;
            bzero(incomingByte, Buffer_SIZE);
            i++;
        }

    }
    printf("\n");
    printf("The number of bytes we needed to receive is: %ld",sum);
    printf("The actual number of bytes we received was: %ld",actualSum);

    if(close(servsockt)== -1){
        perror("Socket closing error!!");
        return(-1);
    }

    return(0);
}


