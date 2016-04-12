/*************************************************************************
	> File Name: net_test.h
	> Author: 
	> Mail: 
	> Created Time: 2016年04月12日 星期二 00时41分06秒
 ************************************************************************/

#ifndef _NET_TEST_H
#define _NET_TEST_H
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include"../hash_table/hash_table.h"

#define MAX_KEY 100
#define MAX_VALUE 1000

typedef struct recv_item
{
    char key[MAX_KEY];
    char value[MAX_VALUE];
}RecvItem;
#endif
