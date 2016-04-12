/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2016年04月12日 星期二 00时09分55秒
 ************************************************************************/
#include"net_test.h"

void * net_call_back(void *params)
{
    int fd = *((int*)params);
    RecvItem rec_data;
    while(recv(fd,&rec_data,sizeof(rec_data),0)!=0)
    {
        printf("fd:%d key:%s value:%s at:%lu\n",fd,rec_data.key,rec_data.value,time(NULL));
    }
    return (void*)0;
}
int main(void)
{
    int sockfd;
    char *ip_addr = "127.0.0.1";
    short port = 8085;

    sockfd = socket(PF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("socketsocket error");
        return -1;
    }
    int on=1;
    //设置允许地址重用
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip_addr);

    int res = bind(sockfd,(struct sockaddr*)&(addr),sizeof(addr));
    if(res<0)
    {
        perror("bing error");
        return -1;
    }
    printf("bing success\n");
    res = listen(sockfd,1000);
    if(res<0)
    {
        perror("listen error");
    }
    printf("listen success\n");

    while(1024)
    {
        struct sockaddr_in in_addr;
        socklen_t len = sizeof(in_addr);

        int fd = accept(sockfd,(struct sockaddr*)&in_addr,&len);
        if(fd<0)
        {
            perror("accept erroror");
        }

        pthread_t pid;
        pthread_create(&pid,0,net_call_back,&fd);
    }
    return 0;
}
