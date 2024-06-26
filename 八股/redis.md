#### 什么是 redis

高性能的 kv 存储数据库



#### redis 数据类型

string, hash(map), set, zset, list

HyperLogLog, geo, pub/sub



#### redis 优势

速度快, 支持的数据类型较多, 灵活的 key 淘汰策略, 持久化



#### memcache 和 redis 的区别

1.memcache 数据存在在内存. redis 可以将数据持久化在内存中. redis 的数据更安全
2.memcache 只支持 string 类型, redis 支持数据较多
3.redis 构建了自己的 vm 机制, 比直接系统调用效率高

#### redis 是单进程单线程的

redis 利用队列技术将访问变成串行, 消除了传统数据库串行控制的开下



#### redis 持久化的机制

支持 RDB, AOF 两种.

- RDB: fork 一个进程, 以内存快照的方式将某一时刻的数据库状态写入到硬盘, 最后替换上次的持久化文件.
  优点

  - 容灾性好, 一个文件可以保存到安全的磁盘
  - 性能好. 子线程完成写操作, 主线程继续执行命令.

  缺点

  - 如果持久化过程中 redis 出现故障会发生数据丢失

- AOF: 将所有的写命令记录到 aof 文件中

  优点

  - 通过配置可以将每次写命令都保存到 aof 文件中, 即使服务中途故障,也能保证数据一致性
  - 通过 aof 重写,可以将一些命令合并,或删除其中的一些命令

  缺点

  - aof 文件恢复一般较慢
  - 数据大时, 执行效率低



#### save, bgsave

save 是前台显示生成快照, 期间会阻塞 redis 服务

bgsave 是后台生成快照, 自动触发使用的都是 bgsave



所以, 生成环境中不要执行 save 命令



#### 过期 key 的删除策略

- 定时删除. 设置一个 timer, 当 key 过期的时候立即删除.
- 惰性删除. 每次获取 key 的时候判断是否过期
- 定期删除. 例行任务,定时遍历所有的 key



#### redis 淘汰策略

- volatile-lru: 从设置过期时间的 key 挑选最近最少使用的
- volatile-ttl:  从 xxx key 挑选将要过期的 key
- volatile-random: 从 xxx key 随机挑选
- allkeys-lru: 从整体数据集中 挑选最近最少使用
- allkeys-random: 整体随机
- no-enviction: 禁止



#### 为什么 redis 把所有数据放在内存

主要原因是内存越来越便宜了.
redis 将数据放在内存,保证了查询的速度. 通过异步的方式将数据写入磁盘保证了数据的持久化



#### redis 同步机制

主节点做 bgsave, 并将后续的操作记录写到内存

从节点全量同步并加载 rdb 镜像后 再将此期间主节点的操作复制到从节点完成同步



#### redis 集群

redis sentinal 保证高可用, 当 master 故障后 , 通过选主讲 slave 提升成 master

redis cluster 保证可用性. 当单个 redis 内存不足时, 通过 cluster 分片存储.



#### jedis 和 redission 对比

 Jedis 是 Redis 的 Java 实现的客户端， 其 API 提供了比较全面的 Redis 命令的支持；Redisson 实 现了分布式和可扩展的 Java 数据结构，和 Jedis 相比，功能较为简单， 不支持字符串操作， 不支持排 序、事务、管道、分区等 Redis 特性。Redisson 的宗旨是促进使用者对 Redis 的关注分离，从而让使 用者能够将精力更集中地放在处理业务逻辑上。



#### redis 哈希槽

redis 集群没有使用一致性 hash, 引入了 hash 槽, redis 集群中有 16384 个哈希槽, 每个 key 通过 crc16 校验后对 16384 取模 确定放那个槽. 集群中的每个节点负责一部分的 hash 槽

$2^{14}个 hash 槽$



#### redis 主从复制

为了使部分节点或者大部分节点无法通讯的情况下集群依然可用, 每个节点都会有 N-1 个复制品



#### redis 集群如何选择数据库

目前不支持,默认都是 0



#### 怎么测试 redis 的连通性

ping 命令



#### 怎么理解 redis 事务



#### redis key 设置过期, 设置永久有效



#### redis 如何做内存优化

尽可能地使用散列表. 如 保存一个用户信息, 将 id,name 保存到一个 key 里, 而不是 id 一个 key, name 一个 key



#### redis 回收进程是怎么工作的



#### redis 内存用完会怎样

如果达到设置上线,继续写会报错. 可以设置淘汰策略,让 redis 清理部分内存



#### MySQL 里有 2000w 数据，redis 中只存 20w 的数据. 如何保证 redis 中的数据都是热点数据

设置淘汰策略



#### redis 的典型使用场景

- 会话缓存
  比如,保存用户购物时一系列的购买行为, 或者只存一个会话 token
- 全页缓存
  缓存最近加载过的页面
- 队列
  redis 提供了 list 和 set 操作, 可以构造一个队列
- 排行榜/计数器
  redis 提供了 zset ,可以方便第捞出前 n 项
- 发布/订阅
  其实可以用专门的消息队列,没不要搞 redis



#### 37、假如 Redis 里面有 1 亿个key，其中有 10w 个key 是以某个固 定的已知的前缀开头的，如果将它们全部找出来？ 

答： 使用 keys 指令可以扫出指定模式的 key 列表。 对方接着追问： 如果这个 redis 正在给线上的业务提供服务， 那使用 keys 指令会有什么问题？ 这个时候你要回答 redis 关键的一个特性：**redis 的单线程的**。**keys 指令会导致线程阻塞一段时间**， 线 上服务会停顿， 直到指令执行完毕， 服务才能恢复。这个时候可以使用 scan 指令， scan 指令可以无 阻塞的提取出指定模式的 key 列表， 但是会有一定的重复概率， 在客户端做一次去重就可以了， 但是 整体所花费的时间会比直接用 keys 指令长。



#### 如果有大量的 key 需要设置同一时间过期，一般需要注意什 么？

答：如果大量的 key 过期时间设置的过于集中，到过期的那个时间点，redis 可能会出现短暂的卡顿现 象。一般需要在时间上加一个随机值， 使得过期时间分散一些



#### 39、使用过 Redis 做异步队列么，你是怎么用的？



#### 40、使用过 Redis 分布式锁么，它是什么回事？



#### 41、如何实现集群中的 session 共享存储？

session 是用于维护 http 请求状态的, 通过 sessionid 获取到 session 实际保存的数据. 在分布式情况下, 如果 session 保存在本机, 显然,只有到本机的请求才能获取对应的 session 数据.

在集群环境下,往往会有转发, 这样,就不能保证一个用户的 http 请求都路由到最初的机器上了

如何解决

- 粘性 session
  Nginx 在转发时将统一用户的所有请求都转发到同一台机器上
- 服务器 session 复制
  一台服务器上创建/修改/删除一个 session 就将其广播到所有服务器上
- session 共享
  将 session 保存在 redis, memcached 等

























































