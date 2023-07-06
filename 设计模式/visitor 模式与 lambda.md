## visitor 设计模式与 lambda 函数

visitor 能传的参数其实是函数指针，
一种是定义的自由函数
一种是函数类（类重载了 () 运算符，能够处理参数）
在引入 lambda 表达式之后，能够很方便地完成以前需要使用函数类才能完成的功能，而且 lambda 表达式的引入也解决了定义函数类的复杂性







# [Java 的双重分发与 Visitor 模式](https://www.cnblogs.com/rinack/p/9859654.html)

**双重分发(Double Dispatch)**

什么是双重分发?

谈起面向对象的程序设计时，常说起的面向对象的「多态」，其中关于多态，经常有一个说法是「父类引用指向子类对象」。

这种父类的引用指向子类对象的写法类似下面这种：

```
Animal animal = new Dog(); 
animal.bark(); 
```

另一种常用的形式是

[![复制代码](pic/visitor 模式与 lambda/copycode.gif)](javascript:void(0);)

```
public class Keeper { 
  
     public void say(Animal a) { 
         System.out.println("Animal say"); 
     } 
  
     public void say(Dog dog) { 
         System.out.println("dog say"); 
     } 
} 
Animal animal = new Animal(); 
Animal dog = new Dog(); 
Keeper keeper = new Keeper(); 
keeper.say(animal); 
keep.say(dog); 
```

[![复制代码](pic/visitor 模式与 lambda/copycode.gif)](javascript:void(0);)

那上面的keeper调用两次say的时候，会输出什么内容呢?会调用到两个不同的方法吗?

实际上在这两次调用的时候，都会调用到say(Animal a)这个方法。由于这些内容在编译期就能确实下来，这就是 Java 的 静态分发。

从上面的图我们看到，对于两次调用生成的字节码，确实都指向了say(Animal a)这个方法，运行时直接执行方法，输出了对应的内容。

那对应的animal.bark() 为什么最终会调用到 dog 类的方法?这是在运行时确定具体的方法接收者的类型并执行。这就是所谓的动态分发，在运行时确定具体的方法，实现面向对象的多态。

**分发(Dispatch)**

分发就是指最终确定一个要执行的方法的过程。

对于 Java 等静态语言来说，都是通过 单一分发(Single Dispatch)来进行的方法执行。

比如这样一行代码

```
dog.eat(new Bone()) 
```

最终执行要选择的eat方法,只会根据dog的具体类型选择到对应的方法，而传入的参数并不能影响到对应方法的选择，这种就是 single Dispatch

为了让传入的真实参数，这里就是Bone来真正起到作用，就需要用到Double Dispatch或者叫做Multiple Dispatch

也就是说最终决定调用方法是哪一个的，不仅仅是方法的接收者，还受参数类型的决定。

**Visitor 模式**

在GoF 的设计模式中，Visitor 模式就使用到了Double Dispatch 达到了调用真实对象的目的。

对于Visitor 模式，最常用的例子是树的遍历。比如在处理到节点和树叶时的方式有区别，此归通过 visitor 的双重分发，实现对于不同的 Element ，执行不同的内容。

代码类似这样：

```
node.accept(new ConcreateVisitor()); 
leaf.accept(new ConcreateVisitor()); 
```

node 中的 accept方法，会将自己的真实类型再次传递回visitor

```
public void accept(Visitor v) { 
    v.visit(this); 
} 
```

此时，在visitor中，就能根据真实的类型来调用具体的方法，对应node 和 leaf 分别有类似这样的方法：

```
public void visit(Node n); 
public void visit(Leaf l); 
```

Visitor 总结起来一般是包含 visitor 接口，在visitor 接口中，包含各个即将被访问的 Element对象的处理逻辑。在 各个Element 的具体实现中，再将自己的类型传递回visitor 进行二次分发，实现确切逻辑的调用。

**在Tomcat中的应用**

Visitor 在Tomcat中也有应用，典型的是解析EL表达式。

比如org.apache.el.parser.Node

这个类中包含一个accept(NodeVisitor visitor)方法

实际的 Node 类型有很多，但在真实调用的这个时候，会通过

```
public void accept(NodeVisitor visitor) throws Exception { 
        visitor.visit(this); 
```

将真实类型传回visitor, vistor中会调用具体的方法，从而实现参数也能起到决定作用的功能。

[![复制代码](pic/visitor 模式与 lambda/copycode.gif)](javascript:void(0);)

```
public void visit(Node node) throws ELException { 
        if (node instanceof AstFunction) { 
 
            AstFunction funcNode = (AstFunction) node; 
 
            Method m = null; 
 
           } else if (xxx) { 
 } 
```

[![复制代码](pic/visitor 模式与 lambda/copycode.gif)](javascript:void(0);)

这里一般会声明多个visit方法，然后上面的visit(this)会直接定位到目标方法上。

以上就是 Java 中的各类分发，以及 visitor这种模式通过模式的形式来实现双重分发的效果。

