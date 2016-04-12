/*************************************************************************
	> File Name: cache_item.h
	> Author: 
	> Mail: 
	> Created Time: 2016年04月09日 星期六 00时17分35秒
 ************************************************************************/

#ifndef _CACHE_ITEM_H
#define _CACHE_ITEM_H
#include"hash_table_common.h"
#define MAX_VALUE_LEN 1024

/*定义每一个cache最小存储单元格式，需要支持链表操作*/
typedef struct cache_item
{
    int used_flg;
    /*这个缓存被使用的次数,提供为lru淘汰时候使用*/
    unsigned long use_counter;
    /*缓存过期时间*/
    int out_time;
    /*缓存的key，解决hash冲突时候使用，释放节点时候需要注意释放*/
    char *key_p;

    /*key的另一个hash值使用hash0，解决hash冲突时候使用*/
    unsigned long key_hash0_value;

    /*指向下一个节点的地址*/
    struct cache_item * cache_item_next_p;
    /*缓存的值,每次分配内存时候直接分配需要的字符串的长度*/
    char value[0];
}CacheItem;

/*hash table中元素结构，其实就是冲突链表的头*/
typedef struct cache_item_list
{
    /*table中冲突链表的头，这个是冲突链表的逻辑头，是一个结构体，不是一个指针，他的next才是真正的链表开始*/
    CacheItem cache_item_list_head;
    /*冲突链表的尾指针，每次插入时候的时间复杂度是O(1)*/
    CacheItem * cache_item_list_tail_p;
    /*这个冲突链表中的元素的总个数，不包含逻辑头*/
    int item_count;
	
    unsigned long keys_hash0_xor_value;
}CacheItemList;

/*获取一个item*/
CacheItem * new_cache_item(const char * key,const char *value,int out_time);

/*删除单个节点*/
int delete_cache_item(CacheItem * item_p);

/*删除整个链表，不包括逻辑头节点*/
int delete_cache_item_list(CacheItemList * head_p);

/*尾插法，插入一个节点在当前节点之后中*/
CacheItem * insert_cache_item_tail(CacheItem * item_tail_p,CacheItem * item_p);

/*获取一个新的链表*/
CacheItemList * new_cache_item_list();

/*向链表中尾插法插入一个节点,首先判断这个key是否在链表中，如果在，就更新这个节点*/
int insert_cache_item_list(CacheItemList * cache_item_list_p,CacheItem *item_p);

/*向链表中插入一个kv*/
int insert_cache_item_list_key_value(CacheItemList * cache_item_list_p,const char * key_p,const char *value,int out_time);

/*使用key去冲突链表中取出item*/
CacheItem* get_cache_item_by_key(CacheItemList * cache_item_list_p,const char * key_p);

/*判断这个key是否存在在这个list中，由于每个key只允许存在一个，所以使用xor hash0的方式判断A^B^A=B,暂时不用*/
int item_is_in_list_by_key(CacheItemList * cache_item_list_p,const char * key_p);

/*通过key删除冲突链表中的节点*/
int delete_cache_item_by_key(CacheItemList * cache_item_list_p,const char * key_p);

/*暂时不提供replace功能，因为不安全，一个程序去replace了，但是另一个程序却不知道*/
int replace_cache_item_by_key();

/*在list中查找key，如果存在key就返回这个节点的前一个节点的指针，如果不存在就返回NULL*/
CacheItem * get_item_pre_by_key(CacheItemList * item_list_p,const char * key_p);

/*打印单个节点数据*/
void show_item(CacheItem *item_p);

/*打印链表数据*/
void show_cache_item_list(CacheItemList *item_head_p);

/*判断一个帖子是否在这个链表中*/
int key_is_in_list(CacheItemList * cache_item_list_p,CacheItem *item_p);

#endif
