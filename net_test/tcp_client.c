/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2016年04月12日 星期二 00时26分56秒
 ************************************************************************/
#include"net_test.h"

int main(void)
{
    int sockfd;
    char * ip_addr = "192.168.0.103";
    short port = 8085;
    sockfd = socket(PF_INET,SOCK_STREAM,0);
    int on=1;
    //设置允许地址重用
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip_addr);
    int res = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(res<0)
    {
        perror("connect error");
        return -1;
    }

    int i = 0;
    srand(time(NULL));
    for(;i<100;i++)
    {
        
        RecvItem  send_data;
        char * key_p = random_str(10);
        char * value_p = random_str(20);
        strcpy(send_data.key,key_p);
        strcpy(send_data.value,value_p);
        send(sockfd,&send_data,sizeof(send_data),0);
        free(key_p);
        free(value_p);
    }
    return 0;
}
