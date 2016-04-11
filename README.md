# easy_cache
基于c语言的简单的缓存

#####待解决问题
	1. 线程安全问题。
	2. hash碰撞较多时候便利冲突表代价较大，现在的解决办法是将hashtable初始值设置较大！
	3. 网络模块实现，实现一种方法是采用传统的线程模型，第二种是采用异步队列方式，第三种是使用libevent。

#####编译hashTable：
    1. 进入hash_table 文件夹。
    2. gcc t_main.c cache_item.c hash_table.c -g -Wall
    3. /a.out
    4. 注：t_main.c是测试文件。
