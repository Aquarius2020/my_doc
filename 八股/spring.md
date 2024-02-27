#### spring 的优点:

spring 的主要特点, ioc, aop.所以,spring 的点为 依赖注入的解耦,切面编程把业务逻辑和非业务逻辑分离.减少样板模板.声明式事务减少了了事务的冗余性.



#### spring 的设计模式

工厂模式,如 getBean, 传入 name,返回一个 bean

工厂方法模式,如 FactoryBean, 

单例模式, 如 controller, service

代理模式, aop

观察者模式,如各种监听器



#### 什么是 AOP

aop 即切面编程, 将公用逻辑封装到切面(如缓存,日志等), 分离业务逻辑和其他逻辑,代码更加整洁, 可以降低系统的耦合度.



#### AOP 的实现方式

静态代理和动态代理. 

静态代理是编译期代码生成被代理类的子类, 如 AspectJ.

动态代理是运行期生成一个代理对象  ?



#### Spring Aop 的实现方式

spring 是通过动态代理实现的,有 jdk 和 cglib 两种实现方式. 原理getBean 返回的是代理类.调用方法都是代理类重写代理方法.



JDK 动态代理是基于接口实现的.  代理类和目标类实现相同的接口.如果没实现或没有就不能代理.

cglib 是通过继承实现的, 因此如果某个方法是 final, private 就不能代理.

默认 jdk, 如果没有接口就只能 cglib



#### AOP 术语

切面: 通知+切点

通知: 切面代码,增强代码

切点: 定义了被代理函数的过滤方法.  可以是类的过滤也可以是方法的过滤

连结点: 方法

引入: ?

目标对象: 被一个或多个切面所通知的对象,即代理对象.

织入: 切面应用到目标对象并创建新代理对象的过程.

- 编译期：切面在目标类编译时被织入。AspectJ的织入编译器是以这种方式织入切面的。
- 类加载期：切面在目标类加载到JVM时被织入。需要特殊的类加载器，它可以在目标类被引入应用之前增强该目标类的字节码。AspectJ5的加载时织入就支持以这种方式织入切面。
- 运行期：切面在应用运行的某个时刻被织入。一般情况下，在织入切面时，AOP容器会为目标对象动态地创建一个代理对象。SpringAOP就是以这种方式织入切面。

#### spring 的通知类型

Before,  after,  after-returning,  after-throwing, around

其实都是 around, 不过 spring 写了一些样板代码.



#### 什么是 IOC

依赖控制反转,将 bean 的生命周期托管到 spring,以达到类间解耦的目的.

管理方和使用方不需要彼此感知,资源交给第三方来管理. 通过集中管理,来实现资源的可配置,易管理,降低了双方的依赖度.

应用方和服务提供方的链接不需要程序员维护,而是交给容器维护.



#### 什么是依赖注入

依赖注入就是填充对象属性的过程. 可以的通过构造器注入也可以通过属性注入.



#### bean 的生命周期

- 调用构造方法创建 bean
- 反射调用 setter 进行依赖注入
- 尝试调用 setBeanName (BeanNameAware)
- 尝试调用 setBeanFactory(BeanFactoryAware    都是 xxxAware)
- 尝试调用 postProcessBeforeInitialization
- 尝试调用 afterPropertiesSet / 尝试调用  init-method 
- 尝试调用 postProcessAfterInitialization
- bean 初始化完成
- 单例
  - 缓存
  - 尝试调用  destory / 尝试调用 destory-method
- 如果Bean是多例的话，容器将Bean返回给用户，剩下的生命周期由用户控制



#### BeanFacotry 和 FacotryBean

Bean工厂,即 bean 容器.

FactoryBean, 一般设计模式会把设计模式的类型写上,即,通过工厂模式设计的类. 显然这个类是用来生产 bean 的. 

当一个 bean 的实现类是 FactoryBean 时, getBean 会再调用 FactoryBean#getObject() 进而返回对应的 bean



#### BeanFactory 和 ApplicationContext 有什么区别

前者是通用的容器工厂,后者是为 web 定制了一些功能,如国际化,资源管理,配置文件等.

前者是延迟加载 bean, 后者默认加载了所有单例 bean, 所以启动会慢.

##### 下面的两条是啥?

BeanFactory通常以编程的方式被创建，ApplicationContext还能以声明的方式创建，如使用ContextLoader。

BeanFactory和ApplicationContext都支持BeanPostProcessor、BeanFactoryPostProcessor的使用，但两者之间的区别是：BeanFactory需要手动注册，而ApplicationContext则是自动注册。

Bean 的注入方式

- @Bean
- @Component
- @Import
- FactoryBean 接口
- BeanDefinitionRegistryPostProcessor 直接写入 beanDefinition



#### @Bean 和 @Component 的区别

@bean 是基于 java 来装配 bean.  @component 是声明式的, 由 spring 自动装备.

@bean 用在方法上,对应的类需要@Configuration.



#### @Component、@Controller、@Repositor和@Service 的区别

业务含义上的区别,对 spring 本身没什么区别,都是声明了一个 bean



#### Spring 单例 bean 线程安全问题

spring 只是bean 容器,只保证创建单例 bean 是线程安全的, 使用 bean 是否是线程安全是开发者的问题.

一个 bean 就是一个对象的实例, 实例有状态是非线程安全的. 如果没有状态(没有数据成员)一定是线程安全的.



在某些业务场景下,有状态的 bean 仍然是线程安全的



#### Spring bean 如何保证并发安全

- 单例变原型.  性能开销大
- 数据成员变成函数的局部变量
- 使用并发安全的类
- threadlocal







