/*************************************************************************
> File Name: t_main.c
> Author: 
> Mail: 
> Created Time: 2016年04月09日 星期六 00时04分42秒
************************************************************************/

#include"hash_table.h"
#include"cache_item.h"

int main(void)
{
    printf("xor:%d\n",1^1^2^2^3^3^1);
    /*CacheItem * item_p = new_cache_item("0key","abc",1);
    CacheItem * item_p1 = new_cache_item("1key","bcd",10);
    CacheItem * item_p2 = new_cache_item("2key","dsads",10);
    CacheItem * item_p3 = new_cache_item("3key","lxdssdap",10);
    insert_cache_item(item_p,item_p1);
    insert_cache_item(item_p1,item_p2);
    insert_cache_item(item_p2,item_p3);
    show_cache_item_list(item_p);
    //int res = delete_cache_item(item_p);
    int res = delete_cache_item_list(item_p);*/


    /*
    CacheItem * item_p = new_cache_item("0key","abc",1);
    CacheItem * item_p1 = new_cache_item("1key","bcd",10);
    CacheItem * item_p2 = new_cache_item("2key","dsads",10);
    CacheItem * item_p3 = new_cache_item("0key","lxdssdap",10);

    CacheItemList * list = new_cache_item_list();
    int res = insert_cache_item_list(list,item_p);
    printf("%d\n",res);
    res = insert_cache_item_list(list,item_p1);
    printf("%d\n",res);
    res = insert_cache_item_list(list,item_p2);
    printf("%d\n",res);
    res = insert_cache_item_list(list,item_p3);
    printf("%d\n",res);


    CacheItem * item = get_cache_item_by_key(list,"0key");

    item = get_cache_item_by_key(list,"0key");

    show_item(item);
    //res = delete_cache_item_by_key(list,"0key");
    printf("*****UUUU::%d\n",res);
    //show_cache_item_list(list);
    res = delete_cache_item_list(list);

    printf("%d\n",res);
    */


    CacheHashTable * hashTable = new_cache_hash_table();
    /*
    unsigned long res = put_cache_hash_table(hashTable,"9","lqqa",10);
    printf("%lu\n",res);
    res = put_cache_hash_table(hashTable,"8","abc",20);
    res = put_cache_hash_table(hashTable,"a8","abc",20);
    res = put_cache_hash_table(hashTable,"8d","abc",20);
    res = put_cache_hash_table(hashTable,"8dd","谁是大英雄？",20);
    res = put_cache_hash_table(hashTable,"9xcxc","opiu",2220);
    res = put_cache_hash_table(hashTable,"8dd","小哪吒！",20);
    res = delete_cache_hash_table_item_by_key(hashTable,"9");
    CacheItem* value = get_cache_hash_table(hashTable,"8dd");
    show_item(value);
    res = delete_cache_hash_table(hashTable);
    printf("%lu\n",res);
*/

    /*
    unsigned long res = hash_string("abcde",HASH_1);
    printf("%lu\n",res);

    res = hash_string("abcde",HASH_0);
    printf("%lu\n",res);
    */
    srand(time(0));
    int i = 0;
    for(;i<1000000;i++)
    {
        char * key = random_str(20);
        char * value = random_str(300);
        unsigned long res = put_cache_hash_table(hashTable,key,value,10);
        if(i>999998)
        {
            printf("key:%s value:%s res:%lu\n",key,value,res);
        }
        free(key);
        free(value);
    }
    /*
    char str_key[20];
    scanf("%s",str_key);
    int start = time(NULL);
    CacheItem * item = get_cache_hash_table(hashTable,str_key);
    int end = time(NULL);
    show_item(item);
    */
    int start = time(NULL);
    unsigned long res = delete_cache_hash_table(hashTable);
    int end = time(NULL);
    printf("time:%dms res:%lu\n",(end-start),res);
    return 0;
}
