在一个项目中，我们开发的同学需要调用算法同学提供的一个接口，这个函数除了所有必要的参数之外，最后还有一个“多余的“参数`void*`类型，即一个可以指向任何类型的指针。这个参数之所以存在，是因为这个接口是一个算法过程的入口，在算法的最后会自动调用一个回调函数`callBack`，回调函数是我们自己定义的，可以用来回收所传进去的`void*`参数所开辟的内存，整个过程实现了算法和主流程的异步处理。其中算法可抽象成：

```cpp
void func_with_void_pointer(int par1, char par2, void* voidPointer);
```

而回调函数被我们定义为：

```cpp
void callBack(void* voidPointer){
    /*
    ...
    */
    delete voidPointer;
}
```

意思就是说：在`func_with_void_poitner`中被我们传入的`voidPointer`参数会在`callBack`中被销毁。被当作`void*`传进去的实际类型为`FrameInfo`，他是一帧图像所包含的基本信息，其中包括左右相机获取到的当前图像以及必要的其他信息，其中两幅图像的内存是new出来的，参考其构造函数，也在该对象的析构函数中被释放。

```cpp
class FrameInfo{
private:
    char* leftFrame;
    char* rightFrame;
    int id;
public:
    FrameInfo(){
        leftFrame = new char[FRAME_WIDTH * FRAME_HEIGHT];
        rightFrame = new char[FRAME_WIDTH * FRAME_HEIGHT];
    }
    ~FrameInfo(){
        if(leftFrame){
            delete[] leftFrame;
        }
        if(rightFrame){
            delete[] rightFrame;
        }
    }
}
```

然而当真正开始测试时，不一会儿便出现了大小为几个G的内存泄漏，之后通过Leak Detector定位到每帧实际泄露大小正好是两幅图像的大小，通过查阅资料以及试验后发现，释放指针`void*`那里有问题，正确的做法应该做一个强制转换，改写为：

```cpp
void callBack(void* voidPointer){
    /*
    ...
    */
    delete (FrameInfo*)voidPointer;
}
```

可是为什么呢？

**初探void的原理**

就C++而言，`void*`其实算他的老祖宗C留下来的产物了，我们先从void谈起。

void即为不确定类型——类型不确定从而所占内存不确定，所以诸如`void par = 10;`之类的声明是万万不可的，即**void类型不能声明实例对象**。在C语言中，void的作用主要有以下两大类：

1. 对函数返回类型的限定，利用void对象的大小不确定来限制函数不能有任何返回值——这就是我们常写的void作返回值的函数。
2. 对函数参数类型的限定，当函数不允许接受参数是，必须用void来限定函数的参数——当然现在没什么会这么写了:`int func(void);`。

但`void*`则不同，编译器会允许你做类似于`int someInt = 10; void* par = &someInt;`之类的操作，因为无论指向什么类型的指针，指针本身所占空间是一定的。我们可以认为`void*`就是一个通用指针，可以指向任意类型的指针。我们都知道，指针有两个属性：指向变量/对象的地址和长度，**但是指针指存储被指向变量的地址，长度则取决于指针的类型**，编译器根据指针的类型从指针指向的地址向后寻址，不同的类型则寻址范围不同，如`int*`从指定地址向后寻找4字节作为变量的存储单元。而我们将一个`void`类型的指针指向一个int类型的实例，**实际上是抹去了这一实例的类型信息**，因此在使用时我们要在心里清楚被抹去的类型信息。基于这一特性，对于许多对内存进行操作的函数，`void*`会产生四两拨千斤的效果：

```cpp
void* memcpy(void* dest, const void* src, size_t len);
void* memset(void* buffer, int c, size_t num);
```

对于没有泛型编程的C语言来说，任何类型的指针都可以传入`memcpy`和`memset`中，这也真实地体现了**内存操作函数**的意义，`void*`帮我们和编译器屏蔽了冗余的变量类型信息，而直接将内存暴露在我们面前。对于C++而言，对于某些范型可以实现的功能但又想找一种轻量化的实现方法，`void*`也不失为一种不错的选择。同时，基于这个特性，`void*`在使用时会不可避免的带来许多限制：

如果将`void*`类型指针赋给其他类型指针，则需要强制类型转换：

```cpp
int someInt = 10;
void* pvoid = &someInt;
int* pInt = (int*) pvoid;			//这里需要强制类型转换
```

void*指针不可解引用（取内容）：

```cpp
int someInt = 10;
void* pvoid = &someInt;
*pvoid ++;										//Error, void*无法解引用
```

若想正确删除掉void*指向的动态类型变量，需要进行强制类型转换：

回到最初提出的问题：为什么会出现内存泄漏呢？在`delete`的过程中，编译器并不知道该`void*`指向的什么类型的变量，自然无法正确调用原类型的析构函数，因而只是**简单地清空了一个指针**。

```cpp
delete voidPointer; 					//只是清空了一个指针
delete (FrameInfo*) voidPointer; 		//正确析构voidPointer指向的变量。
```

## **void\* 与 template**

上边说到了void*可作为一种轻量化的模板编程，这里详细分析一下二者的差别。

毫无疑问，在可能的情况下使用`void*`会提高编译速度，但这并不应该成为我们大量使用`void*`的理由，正如前面所看到的，`void*`会将很多隐藏的问题埋入深坑，相比`template`而言，编译期就能暴露的问题被推迟到运行时，隐含bug风险显著提升。就效率而言，最简单也是最好的例子莫过于来自C++的标准模板库函数`sort`以及C标准函数`qsort`。我们稍做实验，即可发现函数`sort`的效率要远高于`qsort`，即便二者都是基于相似的快速排序算法。

```cpp
//qsort的函数原型如下
void qsort(void *base, int nelem, int width, int (*fcmp)(const void *,const void *));
```

但在某些情况下，使用`template`确实有点杀鸡用牛刀了，这个时候`void*`还是可以派上用场的。