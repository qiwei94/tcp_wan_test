#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#define max_socket_num 20000
#define max_thread_num 20000
#define data_length 1000
//服务器端

void *fun_thrReceiveHandler(void *socketInfo);
void *fun_thrAcceptHandler(void *socketListen);
//1:是 0：否
int checkThrIsKill(pthread_t thr);

typedef struct MySocketInfo{
    int socketCon;
    char *ipaddr;
    uint16_t port;
}_MySocketInfo;

// 客户端数组
struct MySocketInfo arrConSocket[max_socket_num];
int conClientCount = 0;

// 接受客户端线程列表
pthread_t arrThrReceiveClient[max_thread_num];
int thrReceiveClientCount = 0;

int main(int argc, char const *argv[])
{
    //初始化全局变量
    //memset(arrConSocket,0,sizeof(struct MySocketInfo)*10);

    if(argc!=5){
        fprintf(stderr, "usage %s content\n", argv[0]);
        exit(1);
        }

    // get the port number
    char con_port[30] ;
    strncpy(con_port,argv[1],sizeof(argv[1]));
    int port_num=atoi(con_port);

    //get the connect number
    char char_con_num[30];
    strncpy(char_con_num,argv[2],sizeof(argv[2]));
    int int_con_num=atoi(char_con_num);

    //get each thread send times
    char char_thread_send[30];
    strncpy(char_thread_send,argv[3],sizeof(argv[3]));
    int int_thread_send=atoi(char_thread_send);


    //get the send interval
    char char_send_interval[30];
    strncpy(char_send_interval,argv[4],sizeof(argv[4]));
    int int_send_interval=atoi(char_send_interval);



    printf("开始socket\n");
    /* 创建TCP连接的Socket套接字 */
    int socketListen = socket(AF_INET, SOCK_STREAM, 0);
    if(socketListen < 0){
        printf("创建TCP套接字失败\n");
        exit(-1);
    }else{
        printf("创建套接字成功\n");
    }
    /* 填充服务器端口地址信息，以便下面使用此地址和端口监听 */
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY); /* 这里地址使用全0，即所有 */
    
    server_addr.sin_port=htons(port_num);
    
    if(bind(socketListen, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) != 0){
        perror("绑定ip地址、端口号失败\n");
        exit(-1);
    }else{
        printf("绑定ip地址,端口号\n");
    }
    /* 开始监听相应的端口 */
    if(listen(socketListen, 10) != 0){
        printf("开启监听失败\n");
        exit(-1);
    }else{
        printf("开启监听成功\n");
    }
    /* 接受连接套接字 */
    pthread_t thrAccept;
    pthread_create(&thrAccept,NULL,fun_thrAcceptHandler,&socketListen);

    /* 实时发送数据 */
    while(1){
        //判断线程存活多少
        int i;
        for(i=0;i<thrReceiveClientCount;i++){
            if(checkThrIsKill(arrThrReceiveClient[i]) == 1){
                printf("有个线程被杀了\n");
                thrReceiveClientCount--;
            }
        }
        printf("当前有接受数据线程多少个：%d\n",thrReceiveClientCount);

        // 可以录入用户操作选项，并进行相应操作
        char userStr[30] = {'0'};
        char data_block[data_length];
        memset(data_block, '0', sizeof(char)*data_length);
    
        if(conClientCount>=int_con_num){
            printf("GET TO THE TOTAL NUMBER,AND BEGIN TO SEND,wait for 10 seconds:\n");
            printf("data_block is %s\n", data_block);
            printf("the send interval is %d us\n", int_send_interval);
            int i;
            sleep(10);
            for(i=0; i<conClientCount; i++){
                //int sendMsg_len = send(arrConSocket[i].socketCon, userStr, 30, 0);
                printf("NUMBER:%d :",i);
                int j=0;
                printf(" to %s:%d : ",arrConSocket[i].ipaddr,arrConSocket[i].port);
                for(j=0;j<int_thread_send;j++){
                    int sendMsg_len = write(arrConSocket[i].socketCon,data_block,data_length);
                    if(sendMsg_len > 0){
                        printf("%d ",j);
                    }else{
                        printf("\n %d fail \n",j);
                    }
                    usleep(int_send_interval);

                }
                printf("\n");
            }
        printf("WE FINISH IT !\n");
            break;
        }


        // 发送消息
        sleep(1);
    }

    // 等待子进程退出
    printf("等待子线程退出，即将退出！,wait for 100 seconds\n");
    char *message;
    sleep(100);
    pthread_join(thrAccept,(void *)&message);
    printf("%s\n",message);
    printf("finish all  \n");
    return 0;
}

void *fun_thrAcceptHandler(void *socketListen){
    while(1){
        int sockaddr_in_size = sizeof(struct sockaddr_in);
        struct sockaddr_in client_addr;
        int _socketListen = *((int *)socketListen);
        printf("accept blocking\n");
        int socketCon = accept(_socketListen, (struct sockaddr *)(&client_addr), (socklen_t *)(&sockaddr_in_size));
        
        if(socketCon < 0){
            printf("连接失败\n");
        }else{
            /*
            save resource 
            */
            //printf("连接成功 ip: %s:%d\r\n",inet_ntoa(client_addr.sin_addr),client_addr.sin_port);
        }
        
        //printf("连接套接字为：%d\n",socketCon);
        
        //开启新的通讯线程，负责同连接上来的客户端进行通讯
        _MySocketInfo socketInfo;
        socketInfo.socketCon = socketCon;
        socketInfo.ipaddr = inet_ntoa(client_addr.sin_addr);
        socketInfo.port = client_addr.sin_port;
        arrConSocket[conClientCount] = socketInfo;
        conClientCount++;
        
        //printf("connect %d client\n",conClientCount);

        pthread_t thrReceive = 0;
        pthread_create(&thrReceive,NULL,fun_thrReceiveHandler,&socketInfo);
        arrThrReceiveClient[thrReceiveClientCount] = thrReceive;
        thrReceiveClientCount++;
        if(thrReceiveClientCount==max_socket_num){
            printf("IT GET TO THE HIGHEST NUMBER OF total_thread_num\n");
            break;
        }
        //让进程休息1秒
        sleep(0.1);
    }

    char *s = "安全退出接受进程";
    pthread_exit(s);
}

void *fun_thrReceiveHandler(void *socketInfo){
    char buffer[30];
    int buffer_length;
    _MySocketInfo _socketInfo = *((_MySocketInfo *)socketInfo);
    while(1){
        //添加对buffer清零
        bzero(&buffer,sizeof(buffer));
        //printf("read blocking \n");
        buffer_length = read(_socketInfo.socketCon,buffer,30);
        if(buffer_length == 0){
            printf("%s:%d 客户端关闭\n",_socketInfo.ipaddr,_socketInfo.port);
            conClientCount--;
            break;
        }else if(buffer_length < 0){
            printf("接受客户端数据失败\n");
            break;
        }
        buffer[buffer_length] = '\0';
        /*
        save resource 
        */
        //printf("%s:%d 说：%s\n",_socketInfo.ipaddr,_socketInfo.port,buffer);
        printf("get:%s\n", buffer);
        sleep(10);
    }
    printf("接受数据线程结束了\n");
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
