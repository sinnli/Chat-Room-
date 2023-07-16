#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define Buffer_SIZE 1024

int main(int argc,char *argv[]) {
    char Buffer[Buffer_SIZE];
    int i = 0;

    size_t nread;
    int port = atoi(argv[2]);
    FILE *fptr;
    int num_of_packets=atoi(argv[3]);
    int dataSend;
    unsigned long int sum;
    unsigned long int actualSum =0;
    sum=num_of_packets*Buffer_SIZE;
    struct sockaddr_in server_addr;
    int servsockt=socket(AF_INET,SOCK_DGRAM,0);
    fptr = fopen(argv[4],"r");
    if(!fptr){
        perror("file opening error!!");
	return(-1);	
    }
    printf("the file was opened");
    if(servsockt==-1)
    {
        perror("socket opening error!!");
        fclose(fptr);
        return(-1);
    }
    printf("the socket  was opened");

    server_addr.sin_family = AF_INET; // match the socket() call
    server_addr.sin_addr.s_addr =inet_addr(argv[1]); // bind to any local address
    server_addr.sin_port = htons(port);
    memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero); // zero structure out

    printf("sending the message ");
    for (i=0; i<num_of_packets;i++){
   
    nread = fread(Buffer, 1, sizeof(Buffer), fptr);
    dataSend=sendto(servsockt,Buffer,sizeof(Buffer),0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    bzero(Buffer,Buffer_SIZE);
    if(dataSend>0){
            
            actualSum+=dataSend;
            fscanf(fptr,"%s",Buffer);
 	    printf("the sendeing was good");

    }
    else if(dataSend<0)
    {
             perror("ERORR while sendeing!!");
         
    }


    }
    sum=num_of_packets*Buffer_SIZE;
    printf("The number of bytes we needed to send is: %lu\n",sum);
    printf("The actual number of bytes we send was: %lu\n",actualSum);

    if(close(servsockt)== -1){
        perror("Socket closing error!!\n");
        return (-1);

    }

    if(!fclose(fptr)){
        //perror("File closing error!!\n");
        return (-1);
    }
    return 0;
}



