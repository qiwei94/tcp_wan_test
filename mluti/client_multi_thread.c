#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>

#define the_server_data_length 1000
#define live_time 4000

typedef struct MySocketInfo{
    int socketCon;
    unsigned long ipaddr;
    unsigned short port;
    int seq_num;
}_MySocketInfo;


typedef struct basic_sock_info{
    struct hostent host;
    unsigned short port;
    int seq_num;
}_basic_sock_info;



void *fun_thrReceiveHandler(void *socketCon);
int checkThrIsKill(pthread_t thr);


void *client_thread(void* arg_trans){
    _basic_sock_info arg=*((_basic_sock_info *)arg_trans);

    int seq_num=arg.seq_num;
    int port_num=arg.port;
    struct hostent *host=&(arg.host);

    int socketCon = socket(AF_INET, SOCK_STREAM, 0);
    if(socketCon < 0){
        printf("创建TCP连接套接字失败\n");
        //exit(-1);
        return;
    }
    /* 填充客户端端口地址信息，以便下面使用此地址和端口监听 */
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_family=AF_INET;
    

    //server_addr.sin_addr.s_addr=*((struct in_addr *)host->h_addr);
    server_addr.sin_addr=*((struct in_addr *)host->h_addr);
    
    server_addr.sin_port=htons(port_num);
    /* 连接服务器 */
    int res_con = connect(socketCon,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr));
    if(res_con != 0){
        printf("连接失败\n");
        //exit(-1);
        return;
    }
    
    //printf("连接成功,连接结果为：%d\n",res_con);
    
    char userStr[30] ;

    sprintf(userStr, " %d" , seq_num);

    //printf("userStr is %s\n",userStr);
    
    int sendMsg_len = write(socketCon,userStr,30);
       if(sendMsg_len > 0){
           printf("send success : %d\n",socketCon);
       }else{
           printf("发送失败\n");
       }
    //开启新的实时接受数据线程
    pthread_t thrReceive;
    pthread_create(&thrReceive,NULL,fun_thrReceiveHandler,&socketCon);

    /* 实时发送数据 */

    //printf("waiting for this %d seconds\n",live_time);
    sleep(live_time);
    close(socketCon);

}


int main(int argc, char const *argv[])
{
    printf("开始socket\n");

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
    
    //sleep(2);


    int i=0;
    for(i=0;i<client_thread_num;i++){
        printf("%d sub client thread process begin\n", (seq_num+i));

        _basic_sock_info arg;
        arg.port=port_num;
        arg.seq_num=seq_num+i;
        arg.host=*host;

        pthread_t client_sock;
        pthread_create(&client_sock,NULL,client_thread,&arg);
        usleep(50000);//wait for 20ms
    }
    printf("all connect and begin to listen from server\n");
    
    sleep(live_time);
    printf("yes finish all client thread\n");
    return 0;
}

void *fun_thrReceiveHandler(void *socketCon){
    char buffer[the_server_data_length];
    int _socketCon = *((int *)socketCon);
    //int buffer_length = recv(_socketCon,buffer,30,0);
    int buffer_length = read(_socketCon,buffer,the_server_data_length);
    if(buffer_length == 0){
        printf("服务器端异常关闭\n");
        exit(-1);
    }else if(buffer_length < 0){
        printf("接受客户端数据失败\n");
        //break;
    }
    buffer[buffer_length] = '\0';
    //printf("服务器说：%s\n",buffer);
    printf("server give %d bytes\n",buffer_length);

    printf("退出接受服务器数据线程\n");
    return NULL;
}

int checkThrIsKill(pthread_t thr){
    int res = 1;
    int res_kill = pthread_kill(thr,0);
    if(res_kill == 0){
        res = 0;
    }
    return res;
}
