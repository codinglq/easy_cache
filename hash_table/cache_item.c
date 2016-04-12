/*************************************************************************
> File Name: cache_item.c
> Author: 
> Mail: 
> Created Time: 2016年04月09日 星期六 00时32分46秒
************************************************************************/

#include"cache_item.h"
#include"hash_table.h"
#define _USE_MEMCPY

CacheItem * new_cache_item(const char * key,const char *value,int out_time)
{

    if(!key||out_time<0||!value)
    {
        perror("new_cache_item params ERROR!");
        return NULL;
    }

    /*测量字符串占用的内存时候，需要加上字符串结尾\0的内存，不然free时候会出错，就是strlen（str）+1*/
    CacheItem * item_p = (CacheItem*)malloc(sizeof(CacheItem)+STR_MEM_SIZE(value));
    if(!item_p)
    {
        perror("new_cache_item malloc ERROR!");
        return NULL;
    }
    /*初始化操作*/
    item_p->use_counter = 0;
    item_p->out_time = out_time;

    /*hash key 使用hash0 形式,分配hashtable使用hash2*/
    item_p->key_hash0_value = hash_string(key,HASH_0);

    item_p->cache_item_next_p = NULL;
    item_p->key_p = (char *)malloc(STR_MEM_SIZE(key));
    if(!item_p)
    {
        perror("new_cache_item malloc ERROR!");
        return NULL;
    }

    /*使用memcpy是更好一些,这个宏定义暂时写到了本文件中*/
    #ifdef _USE_MEMCPY
    memcpy(item_p->key_p,key,STR_MEM_SIZE(key));
    memcpy(item_p->value,value,STR_MEM_SIZE(value));
    DEBUG_TRACE("use memcpy");
    #else
    strcpy(item_p->key_p,key);
    strcpy(item_p->value,value);
    DEBUG_TRACE("use strcpy");
    #endif
    return item_p;
}


int delete_cache_item(CacheItem * item_p)
{
    if(!item_p||!item_p->key_p)
    {
        perror("delete_cache_item item is null or item_p->key_p is null");
        return ERROR_FLH;
    }
    /*key 是使用的单独申请的堆内存，所以要释放*/
    free(item_p->key_p);
    DEBUG_TRACE("free key ok");
    /*value字符串使用的是item一起申请的内存，所以释放一次就够了*/
    free(item_p);
    DEBUG_TRACE("free item ok");
    return SUCCESS_FLG;
}


CacheItem *insert_cache_item_tail(CacheItem * item_tail_p,CacheItem * item_p)
{
    if(!item_tail_p||!item_p)
    {
        DEBUG_TRACE("item_tail or item_p is null");
        return NULL;
    }

    item_tail_p->cache_item_next_p = item_p;
    item_p->cache_item_next_p = NULL;
    DEBUG_TRACE("insert ok");
    return item_p;
}

void show_cache_item_list(CacheItemList *item_head_p)
{
    if(CAECK_POINT_IS_NULL(item_head_p))
    {
        DEBUG_TRACE("cache item list head is null");
    }else
    {
        CacheItem * head_p = item_head_p->cache_item_list_head.cache_item_next_p;
        while(head_p)
        {
            printf("******\n key:%s\n value:%s\n out_time:%d\n use_counter:%lu\n",
                   head_p->key_p,head_p->value,head_p->out_time,
                   head_p->use_counter);
            head_p = head_p->cache_item_next_p;
        }
        printf("total count item:%d\n",item_head_p->item_count);
    }
}

int delete_cache_item_list(CacheItemList * item_list_p)
{
    if(!item_list_p)
    {
        DEBUG_TRACE("item list is null");
        return ERROR_FLH;
    }

    CacheItem *head_p = (item_list_p->cache_item_list_head.cache_item_next_p);
    CacheItem * next_item_p = head_p;
    CacheItem * curr_item_p = head_p;
    int counter = 0;
    while(next_item_p)
    {
        next_item_p = curr_item_p->cache_item_next_p;
        delete_cache_item(curr_item_p);
        DEBUG_TRACE("delete one item");
        curr_item_p = next_item_p;
        ++counter;
    }

    free(item_list_p);
#ifdef _DEBUG
    printf("Delete %d items\n",counter);    
#endif
    DEBUG_TRACE("delete_cache_item_list ok");
    return SUCCESS_FLG;
}

CacheItemList * new_cache_item_list()
{
    CacheItemList * item_list_p = (CacheItemList*)malloc(sizeof(CacheItemList));
    if(!item_list_p)
    {
        DEBUG_TRACE("new cacheItem list malloc ERROR");
        return NULL;
    }

    item_list_p->item_count = 0;
    item_list_p->cache_item_list_head.cache_item_next_p = NULL;
    item_list_p->cache_item_list_tail_p = &item_list_p->cache_item_list_head;
    item_list_p->keys_hash0_xor_value = 0;
    return item_list_p;
}

/*在list中查找key，如果存在key就返回这个节点的前一个指针，如果不存在就返回NULL*/
CacheItem * get_item_pre_by_key(CacheItemList * item_list_p,const char * key_p)
{
    
    if((!item_list_p)||!key_p)
    {
        DEBUG_TRACE("cache item list is or key is null");
        return NULL;
    }else
    {
        unsigned long key_hash0_value = hash_string(key_p,HASH_0);
        CacheItem * head_p = item_list_p->cache_item_list_head.cache_item_next_p;
        CacheItem * pre_item_p = &(item_list_p->cache_item_list_head);
        while(head_p)
        {
            if(head_p->key_hash0_value == key_hash0_value)
            {
                #ifdef _DEBUG
                printf("find key:%s ok!\n",key_p);
                #endif
                return pre_item_p;
            }
            pre_item_p = head_p;
            head_p = head_p->cache_item_next_p;
        }
        #ifdef _DEBUG
            printf("total count item:%d find key:%s\n",item_list_p->item_count,key_p);
        #endif
        return NULL;
    }
}


/*判断这个key是否存在在这个list中，由于每个key只允许存在一个，所以使用xor hash0的方式判断A^B^A=B*/
int item_is_in_list_by_key(CacheItemList * cache_item_list_p,const char * key_p)
{
    if(!cache_item_list_p||!key_p)
    {
        DEBUG_TRACE("null in item_is_in_list_by_key");
        return ERROR_FLH;
    }
    unsigned long key_hash0_value = hash_string(key_p,HASH_0);
    if(key_hash0_value == ((cache_item_list_p->keys_hash0_xor_value) ^ key_hash0_value))
    {
        return SUCCESS_FLG;
    }
    return ERROR_FLH;
}

/*如果这个key已经存在，就进行跟新操作*/
int insert_cache_item_list(CacheItemList * cache_item_list_p,CacheItem *item_p)
{
    if(!cache_item_list_p || !item_p)
    {
        DEBUG_TRACE("cache item list or item is NULL");
        return ERROR_FLH;
    }

    CacheItem * item_find_pre_p = get_item_pre_by_key(cache_item_list_p,item_p->key_p);
    if(item_find_pre_p)
    {
        /*找到了这个key，这个key已经存在在list中，就替换这个key的value*/
        #ifdef _DEBUG
            printf("key 重复，进行value更新");
        #endif
        item_p->cache_item_next_p = item_find_pre_p->cache_item_next_p->cache_item_next_p;
        delete_cache_item(item_find_pre_p->cache_item_next_p);
        item_find_pre_p->cache_item_next_p = item_p;
    }else
    {

        #ifdef _DEBUG
            printf("key 不重复，直接插入");
        #endif
        CacheItem * item_tail_p = insert_cache_item_tail(
        cache_item_list_p->cache_item_list_tail_p,
        item_p);
        cache_item_list_p->cache_item_list_tail_p = item_tail_p;
        cache_item_list_p->item_count+=1;

        /*keys xor*/
        //cache_item_list_p->keys_hash0_xor_value = cache_item_list_p->keys_hash0_xor_value ^ item_p->key_hash0_value;
    }
    return SUCCESS_FLG;
}

int insert_cache_item_list_key_value(CacheItemList * cache_item_list_p,const char * key_p,const char *value,int out_time)
{
    if(!cache_item_list_p||!key_p||!value)
    {
        DEBUG_TRACE("null in insert_cache_item_list_key_value");
        return ERROR_FLH;
    }
    
    CacheItem * item_p = new_cache_item(key_p,value,out_time);
    if(!item_p)
    {
        return ERROR_FLH;
    }
    int res = insert_cache_item_list(cache_item_list_p,item_p);
    return res;
}

CacheItem * get_cache_item_by_key(CacheItemList * cache_item_list_p,const char * key_p)
{
    if(!cache_item_list_p||!key_p)
    {
        DEBUG_TRACE("cache list or key is null");
        return NULL;
    }
    CacheItem * head_p = cache_item_list_p->cache_item_list_head.cache_item_next_p;
    unsigned long hash0_key = hash_string(key_p,HASH_0);

    while(head_p)
    {
        /*printf("******\n key:%s\n value:%s\n out_time:%d\n use_counter:%d\n",
               head_p->key_p,head_p->value,head_p->out_time,
               head_p->use_counter);
        */
        if(head_p->key_hash0_value == hash0_key)
        {
            head_p->use_counter+=1;
            return head_p;
        }
        head_p = head_p->cache_item_next_p;
    }

    DEBUG_TRACE("un find the key");
    //printf("total count item:%d\n",item_head_p->item_count);
    return NULL;
}

void show_item(CacheItem *item_p)
{
    if(!item_p)
    {
        DEBUG_TRACE("item is null in show_item");
    }else
    {
        printf(" ***ITEM***\n used_flg:%d\n use_counter:%lu\n key:%s\n key_hash0_value:%lu\n value:%s\n ***END ITEM***\n",
              item_p->used_flg,
              item_p->use_counter,
              item_p->key_p,
              item_p->key_hash0_value,
              item_p->value);
    }
}


int delete_cache_item_by_key(CacheItemList * cache_item_list_p,const char * key_p)
{
    if(!cache_item_list_p||!key_p)
    {
        DEBUG_TRACE("cache list or key is null in delete_cache_item_by_key");
        return ERROR_FLH;
    }

    /*记录当前节点*/
    CacheItem * item_p = cache_item_list_p->cache_item_list_head.cache_item_next_p;

    /*记录上一个节点*/
    CacheItem * pre_cache_item_p = &(cache_item_list_p->cache_item_list_head);

    unsigned long hash0_key = hash_string(key_p,HASH_0);

    while(item_p)
    {
        /*差找到要删除的节点*/
        if(item_p->key_hash0_value == hash0_key)
        {
            pre_cache_item_p->cache_item_next_p = item_p->cache_item_next_p;
            delete_cache_item(item_p);
            return SUCCESS_FLG;
        }
        pre_cache_item_p = item_p;
        item_p = item_p->cache_item_next_p;
    }

    DEBUG_TRACE("un find the key in delete_cache_item_by_key");
    return ERROR_FLH;
}

