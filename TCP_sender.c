//tcp sender

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define Buffer_SIZE 1024

int main(int argc,char *argv[]) {
    char buffer_to_send[Buffer_SIZE];
    int i;
    int num_of_packets=atoi(argv[3]);
    int dataSend;
    int portnumber=atoi(argv[2]);
    int sum;
    sum=num_of_packets*Buffer_SIZE;
    int actualSum =0;
    struct sockaddr_in server_addr;
    int serversockt;
    int clientsocket;
    struct sockaddr_in new_addr;
    socklen_t addr_size=sizeof(new_addr);
    size_t dataread;
    FILE *fptr ;
    fptr= fopen(argv[4],"r");
    if(!fptr){
        perror("file opening error!!");
        exit(1);
    }
    serversockt=socket(AF_INET,SOCK_STREAM,0);
    if(serversockt==-1)
    {
        perror("socket opening error!!");
        fclose(fptr);
        exit(1);
    }
    //set socket:
    memset(&server_addr, '\0', sizeof(server_addr)); // zero structure out
    server_addr.sin_family = AF_INET; // match the socket() call
    server_addr.sin_addr.s_addr =inet_addr(argv[1]); // bind to any local address
    server_addr.sin_port = htons(portnumber);
    if(bind(serversockt, (struct sockaddr *) &server_addr, sizeof(server_addr) ) == -1){
        perror("bind error!!");
        close(serversockt);
        fclose(fptr);
        exit(1);
    }
    if(listen(serversockt,5)==-1){
        perror("listen  error!!");
        fclose(fptr); //is there a need to close socket?
        close(serversockt);
        exit(1);

    }
    clientsocket = accept(serversockt, (struct sockaddr *) &new_addr, &addr_size);
    if(clientsocket ==-1 )
    {
        perror("accepting  error!!");
        fclose(fptr); //is there a need to close socket?
        close(serversockt);
        exit(1);
    }
    while(num_of_packets!=0)            //loop until all the data send to dest
    {
        dataread=fread(buffer_to_send, 1, sizeof(buffer_to_send), fptr);
        dataSend=send(clientsocket,buffer_to_send,strlen(buffer_to_send),0);
        if(dataSend<0)
        {
            perror("error occured!!-while sending");
        }
        else if(dataSend == 0)
        {
            break;
        }
        else
        {
            num_of_packets=num_of_packets-1;
            actualSum+=Buffer_SIZE;
        }
        bzero(buffer_to_send,Buffer_SIZE);

    }
    printf("The number of bytes we needed to send is: %d\n",sum);
    printf("The actual number of bytes we send was: %d\n",actualSum);

    if(close(serversockt)== -1){
        perror("Socket closing error!!");
        exit(1);

    }
    fclose(fptr); 

    return 0;

}