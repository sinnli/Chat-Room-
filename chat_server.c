#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


#define Buffer_SIZE 512
//degel,ip ,port ,socket
struct quene{
  int degel;
  int socket;
  struct sockaddr_in addr;
};
int main(int argc,char *argv[]) {
    char buffer[Buffer_SIZE];
    int welsock,port,i,k,j,enter,ready,con,mess_l,fine = 0,no_more_space = 0;
    socklen_t len;
    char namee[6];
    unsigned char mess_len[2];
    ssize_t n,name,mess,a;
    fd_set fdset,fdset2;
    struct quene *users;
    struct sockaddr_in cliaddr, servaddr;
    users = (struct quene*)malloc(atoi(argv[2]+2) * sizeof(struct quene));
    // if memory cannot be allocated
    if(users == NULL)
    {
        perror("Error! memory not allocated.");
        return(-1);
    }
    for(i = 0;i<atoi(argv[2]+2);i++){ //assigning 0 to all
        users[i].degel = 0;
    }
    FD_ZERO(&fdset); // clear the set
    welsock = socket(AF_INET, SOCK_STREAM, 0);
    if(welsock==-1)
    {
        perror("welcome socket opening error!!");
        // deallocating the memory
        free(users);
        return(-1);
    }
    port = atoi(argv[1]);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    if(bind(welsock, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
        perror("welcome socket binding error!!");
        close(welsock);
        // deallocating the memory
        free(users);
        return(-1);
    }
   if(listen(welsock, atoi(argv[2]))<0){
        perror("listen  error!!");
        close(welsock);
        // deallocating the memory
        free(users);
        return(-1);
    } 
     //setting lenght of listen quene to argv[2]
    enter = fileno(stdin);
    //the welsock in data stucture
    users[1].socket = welsock;
    users[1].degel = 1;
    users[1].addr = servaddr;
    users[0].degel = 1;
    users[0].socket = enter;
    bzero(buffer, sizeof(buffer));
   printf("ready for clients:\n");
//add welcome socket and stdin to fdset:
	FD_SET(welsock, &fdset);
	FD_SET(enter,&fdset);
 while(1) {

         fdset2 =fdset;
        // printf("before select\n");
        ready = select(FD_SETSIZE, &fdset2, NULL, NULL, NULL);
        if(ready<0){
            perror("Select error");
        }
        else
       {for(j=0;j<FD_SETSIZE;++j){
     	           if (FD_ISSET(j, &fdset2)) {
				//printf("found a set socket\n");
            		       if(j ==enter){
                		      //stdin
					// printf("stdin detected\n");
		                       fine = 1;
                		       break;
            		        }
                                if(j == welsock)//getting new socket connection
  {
                                     //  printf("new client\n");      
				       len = sizeof(cliaddr);
                                       con = accept(welsock, (struct sockaddr *) &cliaddr, &len);
                                       for (i = 0; i < atoi(argv[2])+2; i++) {
                                               no_more_space = 1;
                                               if (users[i].degel == 0) {


//assigning the new socket
                                                      users[i].degel = 1;
                                                      users[i].socket = con;
                                                      getsockname(con, (struct sockaddr *) &cliaddr,&len);           
                                                     //get the address struct of socket to know its port
users[i].addr = cliaddr;
                                                      no_more_space = 0;
						      FD_SET(con, &fdset);    //add new socket to fdset
					    	      FD_CLR(j,&fdset2);
                                                      break;

                                                 }
                                          }
if (no_more_space == 1) {
                                           close(con);
                                         //close con (the new socket)
                                           no_more_space = 0;
                                         }
                                }
              //check the rest if fd_set
else {
                        

		// message in buffer now  send on to everyone
		          bzero(buffer, sizeof(buffer));
			  name = recv(j,namee,sizeof(namee),0);
			  mess  = recv(j,mess_len,sizeof(mess_len),0);
			  mess_l = (int)(mess_len[0]);

                         n = recv(j, buffer, mess_l, 0);
                         if(n<0){
  perror("receiving error!!");
                          fine= 1;
                          break;
			
			} 
                        //recv is 0 ->closed connection
  if(n==0){
	                    
                             if(close(j)<0){
  			          perror("error of seocket closing");
		             }
			    FD_CLR(j,&fdset);
			    printf("nUser %s has disconnected from the server\n",namee);
          		     // set listenfd and udpfd in readset
                               for (i = 0; i < atoi(argv[2]+2); i++) {
   if(j == users[i].socket){
					users[i].degel = 0; 
					}
				}						
	                 } 
   
   // message in buffer now  send on to everyone
                        else{
				 printf("User: %s has connected to the chat server.\n",namee);
			  for (i = 2; i < atoi(argv[2]); i++) {
                           if(users[i].degel==1 && users[i].socket != j){
		                 a =  send(users[i].socket , namee , sizeof(namee) , 0 );
				 a =  send(users[i].socket , mess_len , sizeof(mess_len) , 0 );
		                 a =  send(users[i].socket , buffer , mess_l , 0 );
   if(a<0){
                                   fine = 1;
                                   break;
                                  }
				
                             }//if

		           }//inner for of else
		        } 
		    FD_CLR(j,&fdset2);      
  }//ele of j -> mees
            }//if fd set
       

     }//for of j
   if(fine){
	  break;
	}  
  }//elser after select
      if(fine){
	break;
	} 
}//end while
  //fee everything

for (i = 0; i < atoi(argv[2]+2); ++i) {
    if(close(users[i].socket)<0){
        perror("error of seocket closing");
        return(-1);
    }
  }
// deallocating the memory
    free(users);
    printf("done delocating\n");
    return 0;
}