#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


#define Buffer_SIZE 1024
int main(int argc,char *argv[]) {
int port,maxfdp;
    char bufferForData[Buffer_SIZE];
    struct sockaddr_in serverAddr,clientAddr;
    int nm = argc;
    socklen_t Addr_size,len;
    int serverSelect;
    fd_set rset,rset2;
    ssize_t n;
    int i;
  int socketArray[nm-1];
    struct sockaddr_in Addr[nm-1];
    nm  = nm-1;
    maxfdp = nm;
    bzero(bufferForData, Buffer_SIZE);
    //printf("the nm is :%d\n",nm);
   // printf("preparing everything\n");
    FD_ZERO(&rset);
    for (i = 0; i < nm; i++) {
 socketArray[i] = socket(AF_INET, SOCK_DGRAM, 0);
     //   printf("socket opened\n");
        if (socketArray[i] == -1) {
            perror("socket opening error!!");
            return (-1);
        }
port = atoi(argv[i+1]);
       // printf("the port is:%d\n",port);
        memset(Addr[i].sin_zero, '\0', sizeof(Addr[i].sin_zero)); // zero structure out
        Addr[i].sin_family = AF_INET; // match the socket() call
        Addr[i].sin_addr.s_addr =  htonl(INADDR_ANY); // check!
        Addr[i].sin_port = htons(port);
        if (bind(socketArray[i], (struct sockaddr *) &Addr[i], sizeof(Addr[i])) == -1)
//works only for  legal port numbers
  {
            perror("bind error!!");
            close(socketArray[i]);
            return (-1);
        }
        FD_SET(socketArray[i], &rset);

    }
printf("waiting for clientsss\n");
    while(1) {
      //  printf("in while\n");
        rset2 = rset; //saving the rset
        serverSelect = select(FD_SETSIZE, &rset2, NULL, NULL,NULL);
       // printf("after slelellkjkj\n");
        if (serverSelect < 0) {
            perror("Selecting  Error!!");
  } 
        else {
               for (i = 0; i < FD_SETSIZE; ++i) {
                //printf("getting clients\n");
                 if (FD_ISSET(i+1, &rset2)) {
                    //printf("insideee\n");
                    len = sizeof(clientAddr);
                   // printf("insidee$$$$$$$$$$$$$$$$$$$$$$$e\n"); 
 n = recvfrom(i+1, bufferForData, Buffer_SIZE, 0, (struct sockaddr *) &clientAddr, &len);
                   // printf("adtererererererer\n");
                    //getting the package of client
                    getsockname(i+1, (struct sockaddr *) &Addr, &len);
                    //get the address struct of socket to know its port
                    printf("Package of clientwith port num :%d\n", (int) ntohs(Addr->sin_port)); 
                    bzero(bufferForData, Buffer_SIZE);
  }

               }
        }
	FD_ZERO(&rset);	
       for (i = 0; i < nm; i++) {
           FD_SET(socketArray[i], &rset);
 }
 }
    for (i = 0; i < nm; i++) {
            if (close(socketArray[i]) == -1) {
                perror("Socket closing error!!");
                return (-1);
            }
    }
   return 0;
 }