一般来讲,savepoint 在 mysql 事务中用作暂存点

```mysql
begin;					-- 开启一个事务
SAVEPOINT savepoint_name;								-- 设置一个`保存点,以便后续如果发生回滚时可以只回滚一部分,之前的事务可以不用回滚

-- 这里可以自行一些其他 sql

ROLLBACK TO SAVEPOINT savepoint_name;		-- 返回到指定的保存点
rollback				-- 整个事务都回滚
```



savepoint 是在数据库事务处理中实现“子事务”（subtransaction），也称为嵌套事务的方法。事务可以回滚到 savepoint 而不影响 savepoint 创建前的变化, 不需要放弃整个事务。

ROLLBACK 回滚的用法可以设置保留点 SAVEPOINT，执行多条操作时，回滚到想要的那条语句之前。

使用 SAVEPOINT

```mysql
SAVEPOINT savepoint_name;    // 声明一个 savepoint

ROLLBACK TO savepoint_name;  // 回滚到savepoint
```

删除 SAVEPOINT

保留点再事务处理完成（执行一条 ROLLBACK 或 COMMIT）后自动释放。

MySQL5 以来，可以用:

```mysql
RELEASE SAVEPOINT savepoint_name;  // 删除指定保留点
```







###为什么与函数调用联系在一起 ?

函数调用其实也是保存当前的状态, 然后执行子程序代码, 子程序执行完后再回复当前状态. 因此 , 这两个过程其实是完全等价的.

如果将 sql 也做过程抽象,那么,实现函数调用的便是 savepoint.	

```mysql
-- 开启外层事务
START TRANSACTION;

-- 执行一系列的SQL语句											 -- 执行当前程序 

-- 开启内层事务
SAVEPOINT inner_transaction;							-- 准备函数调用

-- 执行一系列的SQL语句											 -- 执行子程序

-- 提交内层事务
COMMIT TO SAVEPOINT inner_transaction;		-- 子程序返回

-- 提交外层事务
COMMIT;																		-- 当前程序返回
```





更一般的目的是为了理解 spring 的事务传播.

```python
def fun_a:
  ...
	fun_b();
  ...
```

函数 a 调用了函数 b. 函数 b 可能执行失败.	因此, 函数 a 是继续执行还是抛出异常就需要考虑了.



spring事务传播:



**PROPAGATION_REQUIRED: **大部分情况下,函数 b 执行失败,那么函数 a 也是执行失败的.这也是事务传播的默认规则

**PROPAGATION_REQUIRES_NEW: **调用 fun_b 的时候加了一个 try_catch. 内部失败不影响外部

**PROPAGATION_NESTED: **嵌套事务, fun_a 最后执行失败的时候, fun_b 要回滚.



**PROPAGATION_SUPPORTS: **如果当前存在事务,则按照事务执行,否则非事务

**PROPAGATION_MANDATORY: **如果当前存在事务,则按照事务,否则报错.



**PROPAGATION_NOT_SUPPORTED: **挂起当前事务,并以非事务执行.

**PROPAGATION_NEVER: ** 如果存在事务则报错,否则以非事务执行.

