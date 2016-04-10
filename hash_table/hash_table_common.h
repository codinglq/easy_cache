/*************************************************************************
	> File Name: hash_table_common.h
	> Author: 
	> Mail: 
	> Created Time: 2016年04月09日 星期六 00时35分37秒
 ************************************************************************/

/*Commomn 头文件*/
#ifndef _HASH_TABLE_COMMON_H
#define _HASH_TABLE_COMMON_H
//#define _DEBUG
#define HASH_0 0
#define HASH_1 1
#define HASH_2 2
#define LRU 0
#define FIFO 1
#define MAX_CACHE_LEN 100
#define MAX_CACHE_SIZE -1
#define ERROR_FLH -1
#define SUCCESS_FLG 0
//字符串占用的内存大小
#define STR_MEM_SIZE(str) ((((1)+strlen(str))*(sizeof(char))))
//字符串和指向自己的指针占用的内存的大小
#define STR_CACHE_MEM_SIZE(str) ((STR_MEM_SIZE(str))+(sizeof(char*)))

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>
/*debug use*/
#ifdef _DEBUG

#define DEBUG_TRACE(out_msg) printf(out_msg);\
    printf(" line:%d file:%s\n",__LINE__,__FILE__)
#else
    #define DEBUG_TRACE(out_msg) 
#endif

#define CAECK_POINT_IS_NULL(p) (!p)

/*hash table 中lcache item list 个数*/
#define MAX_TABLE_CACHE_ITEM_LIST_SIZE 100000
#define MAX_TABLE_SIZE MAX_TABLE_CACHE_ITEM_LIST_SIZE

/*最大key字节*/
#define MAX_CACHE_PUT_KEY_SIZE 1*1024*1024
/*最大value字节*/
#define MAX_CACHE_PUT_VALUE_SIZE 2*1024*1024


/*检查指针，输入指针和空指针时候返回的信息*/
#define DEBUG_CHECK_POINT(point,msg) if(!point)\
{\
    DEBUG_TRACE(msg);\
    return NULL;\
}

#endif
