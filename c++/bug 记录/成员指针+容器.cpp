#include <iostream>
#include "unordered_map"
#include "vector"
#include "memory"

using namespace std;

class obj {
public:
    ~obj() {
        cout << "析构 obj @" << this << endl;
    }
};

class dog {
public:
    obj *data;

    dog(obj *ptr) {
        data = ptr;
    }


    ~dog() {
        cout << data << endl;
        delete data;            // 由于没有深拷贝,扩容后这里一定会挂
        data = nullptr;         // 这玩意写不写没有什么用.     本质上是两个对象指向了同一内存.还被释放了两次.
    }
};

void test_dogs() {


    auto p = new int(1);
    delete p;
    // delete p;     tmp(24258,0x1e7f4de00) malloc: *** error for object 0x600001444010: pointer being freed was not allocated

    vector <dog> ds;
    ds.reserve(0);
    ds.emplace_back(new obj());    // 第二次触发扩容时,第一次被释放的内存又被释放了
    ds.emplace_back(new obj());
}


// class 中要用指针时就使用 shared_ptr.
// 如果这个 class 确定不会放容器中,那么可以使用 unique_ptr
// 但是深拷贝性能影响又大
class smart_dog {
public:
    shared_ptr <obj> data;

    smart_dog(obj *ptr) : data{ptr} {
    }

    ~smart_dog() {
        cout << "使用智能指针,没 bug 了" << endl;
    }

};

void test_smart_dog() {
    vector <smart_dog> ds;
    ds.reserve(0);
    ds.emplace_back(new obj);
    ds.emplace_back(new obj);
}

int main() {
    {
        test_smart_dog();
    }
    cout << "------------ test smart dog ok ----------" << endl;
    {
        test_dogs();
    }
}