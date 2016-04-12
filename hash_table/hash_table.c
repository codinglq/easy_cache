#include"hash_table.h"
/*hash 开始*/

/*使用静态全局变量，因为只在本文件中使用*/
static unsigned long crypt_table[1280]={0};

/*种子表格为初始化就初始化一下*/
static short cflg = 0;

/*生成随机字符串*/
char * random_str(int str_len)
{
    char * base_str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    char * str = malloc(sizeof(char)*(str_len+1));
    memset(str,0,sizeof(char)*(str_len+1));
    //srand(time(NULL));
    int srand_local = rand()%strlen(base_str);
    int loc = 0;
    for(loc = 0;loc<str_len;loc++)
    {
        str[loc] = base_str[srand_local];
        srand_local = rand()%strlen(base_str);
    }
    return str;
}

void prepare_crypt_table()
{ 
	unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;

	for( index1 = 0; index1 < 0x100; index1++ )
	{ 
		for( index2 = index1, i = 0; i < 5; i++, index2 += 0x100 )
		{ 
			unsigned long temp1, temp2;

			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp1 = (seed & 0xFFFF) << 0x10;

			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp2 = (seed & 0xFFFF);

			crypt_table[index2] = ( temp1 | temp2 ); 
		} 
	} 
} 

unsigned long hash_string(const char *str_value, unsigned long hash_type)
{ 
    if(!cflg)
    {
        prepare_crypt_table();
        cflg=1;
    }
	unsigned char *key  = (unsigned char *)str_value;
	unsigned long seed1 = 0x7FED7FED;
	unsigned long seed2 = 0xEEEEEEEE;
	int ch;

	while( *key != 0 )
	{ 
		ch = toupper(*key++);

		seed1 = crypt_table[(hash_type << 8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3; 
	}
	return seed1; 
}


CacheHashTable * new_cache_hash_table()
{
    CacheHashTable * cache_hash_table_p = (CacheHashTable*)malloc(sizeof(CacheHashTable));
    if(!cache_hash_table_p){
        DEBUG_TRACE("malloc error in new cache hash table");
        return NULL;
    }

    //DEBUG_CHECK_POINT(cache_hash_table_p,"das");
    cache_hash_table_p->curr_cache_table_size = 0;
    cache_hash_table_p->max_cache_table_size = 0;
    /*指针数组全部设置成NULL*/
    memset(cache_hash_table_p->cache_hash_table,0,sizeof(CacheItemList*)*MAX_TABLE_SIZE);

    return cache_hash_table_p;
}

int delete_cache_hash_table(CacheHashTable * cache_hash_table_p)
{

    if(!cache_hash_table_p)
    {
        DEBUG_TRACE("cache hash table is null in delete_cache_hash_table");
        return ERROR_FLH;
    }

    unsigned long counter = 0;
    unsigned long delete_counter = 0;
    while(counter<MAX_TABLE_SIZE)
    {
        if((cache_hash_table_p->cache_hash_table)[counter])
        {
            delete_cache_item_list(cache_hash_table_p->cache_hash_table[counter]);
            ++delete_counter;
        }
        ++counter;
    }

    /*删除整个表*/
    free(cache_hash_table_p);
#ifdef _DEBUG
    printf("Foreach %lu CacheItemLists on hash_table,delete %lu CacheItemLists\n",counter,delete_counter);
#endif
    return SUCCESS_FLG;
}

/*put 时候使用hash2 做hash*/
int put_cache_hash_table(CacheHashTable * cache_hash_table_p,const char * key_p,const char * value_p,int out_time)
{
    if(!key_p||!value_p||!cache_hash_table_p)
    {
        DEBUG_TRACE("key or value or hash table is null in put_cache_hash_table");
        return ERROR_FLH;
    }

    /*HASH2 value*/
    unsigned long key_hash2_value = hash_string(key_p,HASH_2);

    /*mo MAX_TABLE_SIZE*/
    unsigned long cache_hash_table_local = key_hash2_value%MAX_TABLE_SIZE;

    /*入股hashtable中的这个位置有hashlist，就将这个KV插入*/
    if(cache_hash_table_p->cache_hash_table[cache_hash_table_local])
    {
        CacheItemList * cache_item_list_p = cache_hash_table_p->cache_hash_table[cache_hash_table_local];
        /*如果key重复，就会进行更新操作*/
        int res = insert_cache_item_list_key_value(cache_item_list_p,key_p,value_p,out_time);
#ifdef _DEBUG
        printf("put KV---- key:%s value:%s out_time:%d local:%lu\n",key_p,value_p,out_time,cache_hash_table_local);
#endif
        DEBUG_TRACE("冲突！insert to hash table KV ok");
        return res;
    }else
    {
        CacheItemList * cache_item_list = new_cache_item_list();
        cache_hash_table_p->cache_hash_table[cache_hash_table_local] = cache_item_list;
        int res = insert_cache_item_list_key_value(cache_item_list,key_p,value_p,out_time);
#ifdef _DEBUG
        printf("put KV---- key:%s value:%s out_time:%d local:%lu\n",key_p,value_p,out_time,cache_hash_table_local);
#endif
        DEBUG_TRACE("未冲突！insert to hash table KV ok");
        return res;
    }
    return SUCCESS_FLG;
}


/*从hash table中取出一个item使用key*/
CacheItem * get_cache_hash_table(CacheHashTable * cache_hash_table_p,const char * key_p)
{
    
    if(!key_p||!cache_hash_table_p)
    {
        DEBUG_TRACE("key or value or hash table is null in get_cache_hash_table");
        return NULL;
    }

    /*HASH2 value*/
    unsigned long key_hash2_value = hash_string(key_p,HASH_2);

    /*mo MAX_TABLE_SIZE*/
    unsigned long cache_hash_table_local = key_hash2_value%MAX_TABLE_SIZE;
    
    if(cache_hash_table_p->cache_hash_table[cache_hash_table_local])
    {
        CacheItem * item_p = get_cache_item_by_key(cache_hash_table_p->cache_hash_table[cache_hash_table_local],key_p);
        if(item_p)
        {
            return item_p;
        }else
        {
            return NULL;
        }
    }
    return NULL;
}

/*从hash table中删除一个KV*/
int delete_cache_hash_table_item_by_key(CacheHashTable * cache_hash_table_p,const char *key_p)
{
    if(!key_p||!cache_hash_table_p)
    {
        DEBUG_TRACE("key or value or hash table is null in delete_cache_hash_table_item_by_key");
        return ERROR_FLH;
    }

    /*HASH2 value*/
    unsigned long key_hash2_value = hash_string(key_p,HASH_2);

    /*mo MAX_TABLE_SIZE*/
    unsigned long cache_hash_table_local = key_hash2_value%MAX_TABLE_SIZE;
    
    if(cache_hash_table_p->cache_hash_table[cache_hash_table_local])
    {
        CacheItemList* list = cache_hash_table_p->cache_hash_table[cache_hash_table_local];
        int res = delete_cache_item_by_key(list,key_p);
        return res;
    }
    return ERROR_FLH;
    
}

/*获取key*/
char * get_key(OutCacheItem *out_cache_item_p)
{
    if(!out_cache_item_p)
    {
        DEBUG_TRACE("out_cache_item_p is null in get_key");
        return NULL;
    }
    return out_cache_item_p->key_value_buf;
}

/*获取key*/
char * get_value(OutCacheItem *out_cache_item_p)
{
    if(!out_cache_item_p)
    {
        DEBUG_TRACE("out_cache_item_p is null in get_key");
        return NULL;
    }
    return (out_cache_item_p->key_value_buf)+(out_cache_item_p->value_offset);
}

int get_out_time(OutCacheItem * out_cache_item_p)
{
    if(!out_cache_item_p)
    {
        DEBUG_TRACE("null in out_cache_item_p");
        return ERROR_FLH;
    }
    return out_cache_item_p->out_time;
}

/*使用fifo淘汰这个链表上的节点
  最先插入链表中的元素总是排在前面。
*/
int FIFO_item(CacheItemList * item_list_p,int item_count)
{
	if(!item_list_p)
	{
		DEBUG_TRACE("FILO 时候，链表为空");
	}
	/*链表的第一个节点是逻辑头部*/
	
}
