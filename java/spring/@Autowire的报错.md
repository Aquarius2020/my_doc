



报错内容

`No qualifying bean of type 'com.org.demo.ClassA' avaliable`

找到对应的 Service 看了一眼. 代码中.

```java
@Service
class Service{
	@Autowrie
  ClassA bean;
}
```

显然,如果环境中找不到这个 bean,由于配置了包扫描路径,第一想法是是不是没扫描进来.看了一眼

```java
@Configuration
@EnableAutoConfiguration
@ComponentScan(	basePackages = {
  "com.a.b",
  "com.a.c"},
  lazyInit = true  
)
class ConfigWeb {
  
}
```

果然没配置 `com.org.demo`, 于是加上了该路径,启动 web 后,发现,让然报错.



看 A 的实现代码,代码中并没有标记 @Service 等. 因此,就算被扫描到了,也不会加入环境

```java
class ClassA{
}
```



全局搜索 **new ClassA** ,找到了,原来这个 bean 在另一个 config 中配置

```java
@Configuration
class ConfigDemo {
  @Bean
  ClassA() {
    return new ClassA();
  }
}
```



破案了, 由于 ClassA 没有标注@Service,不能自动生产 bean,只能,在 Demo 服务中, 通过 ConfigDemo 手工注入了一个 ClassA.

因此 Web 服务中找不到这个 bean,要想找到.要不自己去修改 ClassA 的实现(显然不行,代码都是别人的). 要不就是自己手动注册一个,即在 ConfigWeb 中标记一个

