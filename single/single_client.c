#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>


int main(int argc, char const *argv[])
{	


	if(argc!=5){
        fprintf(stderr, "usage %s content\n", argv[0]);
        exit(1);
        }

    //SEQ NUM:
    char userStr[30] ;
    strncpy(userStr,argv[1],sizeof(argv[1]));
    int seq_num=atoi(userStr);
    printf("basic offset is %d\n", seq_num);
    
    
    //IP:
    struct hostent *host;
    if((host=gethostbyname(argv[2]))==NULL){
        fprintf(stderr, "gethostbyname error\n");
        exit(1);
    }

    //printf("ip is %s\n", inet_ntoa(host);

    //PORT:
    char RAW_PORT[30] ;
    strncpy(RAW_PORT,argv[3],sizeof(argv[3]));
    int port_num=atoi(RAW_PORT);
    printf("port is %d\n", port_num);


    //client thread num:
    char RAW_CLIENT_THREAD_NUM[30] ;
    strncpy(RAW_CLIENT_THREAD_NUM,argv[4],sizeof(argv[4]));
    int client_thread_num=atoi(RAW_CLIENT_THREAD_NUM);
    printf("client_thread_num is %d\n", client_thread_num);
    

	int sockfd;
	char buffer[1024];
	struct sockaddr_in server_addr;//the server address
	int nbytes;

	/******************************create the socket********************************/
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		fprintf(stderr, " socket errno %s\n",strerror(errno));
		exit(1);
	}


	/*****fill the client infomation******/
	bzero(&server_addr,sizeof(server_addr));//init the server_addr to zero
	server_addr.sin_family=AF_INET;//use ipv4 
	server_addr.sin_port=htons(port_num);
	server_addr.sin_addr=*((struct in_addr *)host->h_addr);

	//string server_address=server_addr.sin_addr;
	printf("the server add is  \n");
	if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1){
		fprintf(stderr, " connect error %s\n", strerror(errno));
		exit(1);
	}


/*
	int sendMsg_len = write(socketCon,userStr,30);
    if(sendMsg_len > 0){
      	printf("send success : %d\n",socketCon);
       	}else{
        printf("发送失败\n");
       }
*/
	/**************** connect suucessfully ****************/
	int count=0;
	while(1){
		if((nbytes=read(sockfd,buffer,1024))==-1){
			fprintf(stderr, " read error  %s\n", strerror(errno));
			//exit(1);
		}else{
			printf("%d:s\n", count);
			count++;
		}

		buffer[nbytes]='\0';
		if (nbytes==0){
			break;
		}
		
	}
	printf("all finished , bye \n");
	close(sockfd);
	exit(0);
	return 0;
}