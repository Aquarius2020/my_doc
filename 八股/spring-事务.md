#### @Transactional

事务的传播行为

- 支持事务
  - PROPAGATION_REQUIRED   	Require, 没有事务就创建,有事务就加入
  - PROPAGATION_SUPPORTS      SUPPORTS 就是可有可无
  - PROPAGATION_MANDATORY   **MANDATORY 强制的**, 如果当前不存在事务则报错
  - PROPAGATION_REQUIRES_NEW     总是开启新事物
  - PROPAGATION_NESTED       **NESTED 嵌套**     嵌套事务执行或新建一个事务

- 不支持事务
  - PROPAGATION_NOT_SUPPORTED						有事务的话就挂起再执行,以非事务执行
  - PROPAGATION_NEVER   有事务的话就报错



**PROPAGATION_NESTED 与PROPAGATION_REQUIRES_NEW的区别:**

1. PROPAGATION_REQUIRES_NEW： 每次都会新建一个事务，如果当前有事务存在，则挂起当前存在的事务。新的事务创建后，与之前的事务没有关联。这也意味着新的事务与旧的事务失败或成功没有关系。

2. PROPAGATION_NESTED： 开始新的嵌套事务，如果当前存在事务，则这个事务在当前事务基础上嵌套起来，形成一个父子事务，如果子事务提交，则父事务可以看到提交的效果，如果子事务回滚，父事务才能看到回滚的效果。

总的来说，PROPAGATION_REQUIRES_NEW每次都会新建一个事务，和当前的事务没有关联，而PROPAGATION_NESTED则在当前事务存在的情况下创建一个子事务，如果子事务操作成功，只有父事务也成功，才会真正的提交事务，否则子事务的执行结果可以被回滚。

搞得乱七八糟的, 实际上这是 javer 没学懂.

嵌套事务就是 sql 中定义的事务.
新建事务就是完全新的,当然前面事务是什么情况怎么处理是无所谓的



#### 事务失效的问题

- 原理篇
  - aop 依赖继承或接口. 显然,继承的时候会有问题.   子类只能访问 public 接口
    final 不能继承,当然也无法代理
  - 类内部调用
    - AopContext.currentProxy() 可以获取当前对象的代理类. 也以解决内部调用 aop 失效的问题
- 没有托管 spring
- 当前数据库不支持事务



#### spring 怎么解决循环依赖问题

循环依赖大部分情况下是设计出了问题, 需要考虑 设计是否合理. spring3.0 已经默认不支持循环依赖了.

至于 spring 是如何解决的, 通过三级缓存. 只能解决部分问题: 非构造器注入的,单例的 bean







