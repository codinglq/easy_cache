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

/*每个out item被插入完成或者get完成之后就会被释放
*这样的结构会方便网络传输和节省空间
* */
typedef struct out_cache_item
{
    /*过期时间*/
    int out_time;
    /*key的偏移量*/
    int key_offset;
    /*value基于key的偏移量*/
    int value_offset;
    /*存储key和value的数组*/
    char key_value_buf[0]
}OutCacheItem;

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

/*获取key*/
char * get_key(OutCacheItem *out_cache_item_p);

/*获取value*/
char * get_value(OutCacheItem * out_cache_item_p);

/*获取outtime*/
int get_out_time(OutCacheItem *out_);

/*向hashtable中插入一个put元素*/
int put(CacheHashTable *cache_hash_table_p,OutCacheItem *out_cache_item_p);

/*在hashtable中取出一个key为传入的元素*/
CacheItem * get(CacheHashTable *cache_hash_table_p,OutCacheItem *out_cache_item_p);
#endif
