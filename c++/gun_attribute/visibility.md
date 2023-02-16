### 原文 [How to use the __attribute__((visibility(“default”)))? | 易学教程 (e-learn.cn)](https://www.e-learn.cn/topic/3778645)

阅读GNU维基中的可见性，很明显。

以C++教程为例

```cpp
// classes example
#include <iostream>
using namespace std;

class Rectangle {
    int width, height;
  public:
    void set_values (int,int);
    int area() {return width*height;}
};

void Rectangle::set_values (int x, int y) {
  width = x;
  height = y;
}
```

是否可以在不更改代码的情况下按第一个链接所示进行公开和本地？`area()``set_values(int,int)`

我写了我的制作文件来获取`.so`

```bash
someproj.so : someproj.cpp
    g++ --std=c++11 -O3 -fPIC -shared someproj.cpp -o someproj.so
```

修改为通过添加隐藏所有符号`-fvisibility=hidden`

```
someproj.so : someproj.cpp
    g++ --std=c++11 -O3 -fvisibility=hidden -fPIC -shared someproj.cpp -o someproj.so
```

是否可以通过修改上面的编译命令来自定义公开哪些功能？

当前使用 4.7.2 版本的 gcc

### 回答1:

> 是否可以通过修改上面的编译命令来自定义公开哪些功能？

否。编译选项 -fvisibility=[默认|内部|隐藏|受保护] （请注意，它*不是*链接选项）使编译器将指定的动态可见性类型归因于*所有*全局符号 在编译单元中生成，但通过在*源代码中*应用反补贴而明确排除的*除外*。这就回答了你的另一个问题：`__attribute__((visibility(....)))`

> 是否可以在不更改代码的情况下使 area（） 公开和 set_values（int，int） 本地，如第一个链接所示？

也没有。

*您将*如何更改源代码以动态制作 可见，而所有其他全局符号隐藏以进行动态链接 ？ 下面是一个演练：`Rectangle::area()``-fvisibility=hidden`

让我们从：

**矩形.cpp （1）**

```
class Rectangle {
    int width, height;
  public:
    void set_values (int,int);
    int area() {return width*height;}

};

void Rectangle::set_values (int x, int y) {
  width = x;
  height = y;
}
```

并简单地将其编译为PIC，以便：`rectangle.o`

```
$ g++ -Wall -c -fPIC rectangle.cpp
```

然后检查全局符号表：

```
$ nm -C rectangle.o
0000000000000000 T Rectangle::set_values(int, int)
```

请注意，它不存在。它不适用于 *根本没有*链接，因此不会出现其动态可见性的问题。`Rectangle::area`

那是因为它在类定义中内*联*定义并且从未调用 在编译单元中，因此 GCC 甚至不需要编译其定义。它消失了 从对象文件。

`Rectangle::set_values`另一方面，不是内联定义的，因此编译器 发出全局符号和定义。

要使某些可见性类型有资格，我们首先需要使 它是一个全局符号，因为它不是内联定义的：`Rectangle::area`

**矩形.cpp （2）**

```
class Rectangle {
    int width, height;
  public:
    void set_values (int,int);
    int area();

};

int Rectangle::area() {return width*height;}

void Rectangle::set_values (int x, int y) {
  width = x;
  height = y;
}
```

重新编译并再次检查全局符号表：

```
$ g++ -Wall -c -fPIC rectangle.cpp
$ nm -C rectangle.o
000000000000001a T Rectangle::set_values(int, int)
0000000000000000 T Rectangle::area()
```

好。现在出现了 的全局定义。`Rectangle::area`

接下来，让我们从以下位置创建一个共享库：`librectangle.so``rectangle.o`

```
$ g++ -o librectangle.so --shared rectangle.o
```

以下是其全局符号表中的符号：`Rectangle::*`

```
$ nm -C librectangle.so | grep 'Rectangle::'
00000000000005d4 T Rectangle::set_values(int, int)
00000000000005ba T Rectangle::area()
```

以下是其*动态*符号表中的符号：`Rectangle::*`

```
$ nm -CD librectangle.so | grep 'Rectangle::'
00000000000005d4 T Rectangle::set_values(int, int)
00000000000005ba T Rectangle::area()
```

它们是一样的。

现在让我们隐藏这些符号以进行动态链接。我们需要重新*编译然后重新*链接共享库：`rectangle.cpp`

```
$ g++ -Wall -c -fPIC -fvisibility=hidden rectangle.cpp
$ g++ -o librectangle.so --shared rectangle.o
```

以下是全局符号表中的符号：`Rectangle::*`

```
$ nm -C librectangle.so | grep 'Rectangle::'
0000000000000574 t Rectangle::set_values(int, int)
000000000000055a t Rectangle::area()
```

它们和以前一样。

以下是*动态*符号表中现在的符号：`Rectangle::*`

```
$ nm -CD librectangle.so | grep 'Rectangle::'; echo Done
Done
```

现在*没有*了，多亏了.`-fvisibility=hidden`

最后，让我们动态可见，保留所有 其他全局符号动态隐藏。我们需要更改源代码 再：`Rectangle::area`

**矩形.cpp （3）**

```
class Rectangle {
    int width, height;
  public:
    void set_values (int,int);
    __attribute__((visibility("default"))) int area();

};

int Rectangle::area() {return width*height;}

void Rectangle::set_values (int x, int y) {
  width = x;
  height = y;
}
```

然后重新编译并重新链接：

```
$ g++ -Wall -c -fPIC -fvisibility=hidden rectangle.cpp
$ g++ -o librectangle.so --shared rectangle.o
```

全局符号表仍显示：

```
$ nm -C librectangle.so | grep 'Rectangle::'
00000000000005a4 t Rectangle::set_values(int, int)
000000000000058a T Rectangle::area()
```

动态符号表仅显示：

```
$ nm -CD librectangle.so | grep 'Rectangle::'
000000000000058a T Rectangle::area()
```

`Rectangle::area`现在是共享库公开的唯一符号 动态联动。

**在你走之前...**

还有一件事：

> 是否可以在不更改代码的情况下使 area（） 公开和 set_values（int，int） 本地，如第一个链接所示？

为动态链接隐藏符号不会使其*本地化*。动态可见性（默认|内部|隐藏|受保护） 仅是*全局*符号的属性。出于链接目的，*本地*符号不存在。唯一的方法 使一个符号本地化，否则将是全局的：-

- 在 C 或C++源代码中，使用 static 关键字限定其定义
- 在C++源中，将其定义括在匿名命名空间中

然后，符号不会出现在全局或动态符号表中。





### 补充

这些函数可能是要严格匹配才能生效

```c
// 比如头文件声明了 static，但是源文件中没有,比如f1
// 在比如 头文件中声明了 const，但是源文件中没有
// h
static int f1();
int f2(const int a);

// cpp
int f1(){
}
int f2(int a){
}
```