#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H
#include"hash_table_common.h"
#include"cache_item.h"

/*hash table 结构*/
typedef struct hash_table
{
    /*存储冲突链表的头 指针数组,hash取模直接定位*/
    CacheItemList *cache_hash_table[MAX_TABLE_SIZE];
    /*当前table中的item个数*/
    unsigned long curr_cache_table_size;
    /*当前table最大存储item个数*/
    unsigned long max_cache_table_size;
}CacheHashTable;

/*生成随机字符串*/
char * random_str(int str_len);

//准备散列时候用到的数组
void prepare_crypt_table();

//散列函数
unsigned long hash_string(const char *lpszFileName, unsigned long dwHashType);

/*获取一个hash table*/
CacheHashTable * new_cache_hash_table();

/*删除一个hash table*/
int delete_cache_hash_table(CacheHashTable * cache_hash_table_p);

/*向hash table 插入一个KV*/
int put_cache_hash_table(CacheHashTable *cache_hash_table_p,const char * key_p,const char * value_p,int out_time);

/*从hash table中取出一个item使用key*/
CacheItem* get_cache_hash_table(CacheHashTable *cache_hash_table_p,const char * key_p);

/*从hash table中删除一个KV*/
int delete_cache_hash_table_item_by_key(CacheHashTable * cache_hash_table_p,const char *key_p);

#endif
