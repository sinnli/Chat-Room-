/*tcp_receiver
 ============================================================================
 Name        : lab8.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define Buffer_SIZE 1024

int main(int argc,char *argv[]) {
    int is_connected;
    char incomingByte[Buffer_SIZE];
    int num_of_packets = atoi(argv[3]);
    int portnumber=atoi(argv[2]);
    int dataToreceive;
    int sum;
    int actualSum = 0;
    sum=num_of_packets*Buffer_SIZE;
    struct sockaddr_in server_addr; //address of socket
    socklen_t addr_size=sizeof(server_addr);
    int clientsockt=socket(AF_INET,SOCK_STREAM,0);
    if(clientsockt==-1)
    {
        perror("socket opening error!!");
        exit(1);
    }
    memset(&server_addr, '\0', sizeof(server_addr)); // zero structure out
    server_addr.sin_family = AF_INET; // match the socket() call
    server_addr.sin_addr.s_addr =inet_addr(argv[1]); // bind to any local address
    server_addr.sin_port =  htons(portnumber);

    is_connected = connect(clientsockt, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if(is_connected==-1)
    {
        perror("connection error!!");
        close(clientsockt);
        exit(1);
    }
    while(num_of_packets!=0)
    {
        dataToreceive=recv(clientsockt,incomingByte,Buffer_SIZE,0);
        if(dataToreceive<0)
        {
            perror("error occured!!-while reading");
            close(clientsockt);
            exit(1);
        }
        else if(dataToreceive == 0)
        {
            break;
        }
        else
        {
            printf("The Data is : %s",incomingByte);  //here we print the buffer to the screen
            num_of_packets=num_of_packets-1;
            actualSum += Buffer_SIZE;
            bzero(incomingByte, Buffer_SIZE);
        }

    }
    printf("\n");
    printf("The number of bytes we needed to receive is: %d\n",sum);
    printf("The actual number of bytes we received was: %d\n",actualSum);

    if(close(clientsockt)== -1){
        perror("Socket closing error!!");
        exit(1);

    }
    return 0;

}


















