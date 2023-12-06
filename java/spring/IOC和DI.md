**IOC**  即 依赖控制反转.    程序不再控制对象的声明走起. 负责管理这些对象的容器叫做 ioc 容器

**DI**  组件之间的依赖关系由容器运行时决定.无需任何代码即可指定目标志愿,而不需要关心目标资源来自何处,由谁实现.

简答地说就是解耦了资源的提供者和资源的使用者.

**Bean**  由 ioc 容器实例化组装和管理的对象.

![img](pic/IOC和DI/GenericGroovyApplicationContext-6dwIrF.png)

阅读 spring 源码: 创建容器，并从容器中获取 Bean ，来测试环境是否 OK。

根据元数据的不同，创建容器的方式也不同，下面参考官方文档，简单介绍下创建容器的几种方式：

```java
public class AnnotationConfigApplicationTest {

    public static void main(String[] args) {

        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();

        context.register(JavaConfig.class);
        context.refresh();

        System.out.println(context.getBean(UserComponent.class));

    }
}
```

```java
public class GroovyConfigApplicationTest {

    public static void main(String[] args) {

        GenericGroovyApplicationContext context = new GenericGroovyApplicationContext("SpringConfig.groovy");

        System.out.println(context.getBean(NotesServiceImpl.class));

    }
}
```
