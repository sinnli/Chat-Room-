









/*
 * chat_client.c
 *
 *  Created on: Jun 8, 2021
 *      Author: ubu
 */

/*
 * tcp_rx.c
 *
 *  Created on: Jun 7, 2021
 *      Author: ubu
 */


#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#define MAX_BUF 1024
int main(int argc, char *argv[]){

    //variables for data:

    char buffer[MAX_BUF];
    ssize_t user,lendata,buff,username,datamsg;
    size_t lenstr= strlen(argv[4])+1; // +1 for the '\0';
    char name[6];
    strcpy(name,argv[3]);
    unsigned char messagelen[2];
    messagelen[0]=lenstr;
    char msgdata[lenstr];
    strcpy(msgdata,argv[4]);
    int msg_len = (int)(messagelen[0]); //cast for use in while loop later to get the int value of the len  !

    //variables for socket:
    struct sockaddr_in client;
    socklen_t size=sizeof client;
    int port = atoi(argv[2]);
    int clientSocket;
    int client_connect;
    int serv_selc;
    fd_set fdset,fdset_copy;
    FD_ZERO(&fdset); // clear the set

    //helper variables
    int i;
    int flag=1; // help to exit the loop function later !!

    //set socket:
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);                   //create the socket

    if (clientSocket==-1)
    {
        perror("Error Openning socket\n");
        close(clientSocket);
        return 0;
    }

    client.sin_family = AF_INET;                                 //Address family = Internet
    client.sin_port = htons(port);                               //set port using htons
    client.sin_addr.s_addr = inet_addr(argv[1]);                  //set ip address
    memset(client.sin_zero, '\0', sizeof client.sin_zero);   // Set all bits of the padding field to 0

    client_connect=connect(clientSocket, (struct sockaddr *) &client,size); //Connect the socket to the server using the address struct
    if(client_connect==1) {
        perror("Error while connecting");
        close(clientSocket);                     //if error connecting print and close
        return 0;
    }

    //send details to server:
    user=send(clientSocket,name,sizeof(name),0);
    lendata = send(clientSocket,messagelen,sizeof(messagelen),0);
    datamsg = send(clientSocket,msgdata,sizeof(msgdata),0);
    if (user<0 ||lendata<0 ||datamsg<0 ){
        perror("Error sending data \n");
        close(clientSocket);
        return 0;
    }

    FD_SET(clientSocket, &fdset);
    FD_SET(fileno(stdin),&fdset);

    //Recieving information from the server:

    printf("welcome to the chat room!!\n");
    while(flag)
    {
        fdset_copy = fdset;                        //copy of fdset for the select function  for each loop !!
        serv_selc=select(FD_SETSIZE,&fdset_copy , NULL , NULL,NULL);     //block until user input
        if(serv_selc<0){
            perror("Select error");
            flag=0;
            break;
        }

        else{                            //reading message
            for(i=0;i<FD_SETSIZE;++i){
                if(FD_ISSET(i,&fdset_copy))
                {
                    if(i==fileno(stdin)){       //if user pressed enter- disconnect
                        flag=0;
                        bzero(buffer, sizeof(buffer));
                        break;
                    }
                    else{                       //recieve information:

                        username = recv(i, name, sizeof(name), 0);  //receive the data
                        if(username<0)
                        {
                            perror("Error  to recieving data!!");
                            flag=0;
                            bzero(buffer, sizeof(buffer));
                            break;
                        }
                        if(username==0)
                        {                                        //connection was closed
                            flag=0;
                            bzero(buffer, sizeof(buffer));
                            break;
                        }
                        bzero(buffer, sizeof(buffer));
                        datamsg = recv(i, messagelen, sizeof(messagelen), 0);
                        buff =  recv(i, buffer, msg_len, 0);
                        if(datamsg<0|| buff < 0){
                            perror("Error recieving");
                            flag=0;
                            bzero(buffer, sizeof(buffer));
                            break;
                        }
                        printf("\nUser: %s has said: %s\n",name,buffer);
                        FD_CLR(i,&fdset_copy );
                    }

                }
            }


        }

    }
    // the end of the program, send a closing message  to the server !!
    strcpy(buffer,argv[3]);
    buff = send(clientSocket,buffer,strlen(buffer)+1,0);
    if(buff<0)
    {
        perror("sending error!");
    }
    printf("\ndisconnected from chat\n");
    close(clientSocket);
    return 0;

}














