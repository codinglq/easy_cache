# easy_cache
基于c语言的简单的缓存
使用HASH_0作为哈希冲突时候的第二次对比指纹。分配hashtable位置时候使用的HASH_2
t_main.c中测试了hash均匀性。分布较为均匀。
#####待解决问题：
	1. 线程安全问题。
	2. hash碰撞较多时候便利冲突表代价较大，现在的解决办法是将hashtable初始值设置较大！
	3. 网络模块实现，实现一种方法是采用传统的线程模型，第二种是采用异步队列方式，第三种是使用libevent。
	4. LRU和FIFO淘汰机制还没有实现。

#####编译hashTable：
    1. 进入hash_table 文件夹。
    2. gcc t_main.c cache_item.c hash_table.c -g -Wall
    3. /a.out
    4. 注：t_main.c是测试文件。
