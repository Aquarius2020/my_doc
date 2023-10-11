
#include "common.h"

using namespace std;

void print_list(vector<int> list) {
  for (auto c : list) {
    cout << c << " ";
  }
  cout << endl;
}

void perm(vector<int> list, int k) {
  int m = list.size();
  if (k == m)
    print_list(list);
  else {
    for (int i = k; i < m; i++) {
      swap(list[i], list[k]);
      perm(list, k + 1);
      swap(list[i], list[k]);
    }
  }
}

struct frame {
  int status = 0;
  int k = 0;
  int i = 0;
};

vector<frame> stacks;
bool empty() { return stacks.empty(); }
frame &top() { return stacks.back(); }
void push(frame frame) { stacks.push_back(frame); }
void pop() { stacks.pop_back(); }

void caf_prem(vector<int> &list, int k, int m) {
  push({.k = k});
  while (!empty()) {
    switch (top().status) {
    case 0:
      if (top().k == m) {
        top().status = 1;
      } else {
        top().status = 2;
      }
      break;
    case 1:
      print_list(list);
      top().status = -1;
      break;
    case 2:
      top().i = top().k;
      top().status = 3;
      break;
    case 3:
      if (top().i >= m) {
        top().status = -1;
      } else {
        top().status = 4;
      }
      break;
    case 4:
      swap(list[top().i], list[top().k]);
      top().status = 5;
      break;
    case 5:
      top().status = 6;
      // caf_prem(list, k + 1,m);
      push({0, top().k + 1});
      break;
    case 6:
      swap(list[top().i], list[top().k]);
      top().i = top().i + 1;
      top().status = 3;
      break;
    case -1:
      pop();
      break;
    }
  }
}

// 一上来就先设置转移状态
void caf_prem_v2(vector<int> &list, int k, int m) {
  push({.k = k});
  while (!empty()) {
    switch (top().status) {
    case 0:
      if (top().k == m) {
        top().status = 1;
      } else {
        top().status = 2;
      }
      break;
    case 1:
      top().status = -1;
      print_list(list);
      break;
    case 2:
      top().status = 3;
      top().i = top().k;
      break;
    case 3:
      if (top().i >= m) {
        top().status = -1;
      } else {
        top().status = 4;
      }
      break;
    case 4:
      top().status = 5;
      swap(list[top().i], list[top().k]);
      break;
    case 5:
      top().status = 6;
      // caf_prem(list, k + 1,m);
      push({0, top().k + 1});
      break;
    case 6:
      top().status = 3;
      swap(list[top().i], list[top().k]);
      top().i = top().i + 1;
      break;
    case -1:
      pop();
      break;
    }
  }
}


void caf_prem_v3(vector<int> &list, int k, int m) {
  push({.k = k});
  while (!empty()) {
    switch (top().status) {           // status 相当于计算机中的 pc 寄存器,用于存储下一条要执行的指令的地址
                                      // 因此,进入代码块(指令)的第一件事就是先设置当前指令执行完后下一条应该执行到哪里
    case 0:                           // 0 状态代表进入函数状态,因此,无论是否是递归进来的,初始状态必然是 0
      if (top().k == m) {
        top().status = 1;
      } else {
        top().status = 2;
      }
      break;
    case 1:
      top().status = -1;
      print_list(list);
      break;
    case 2:
      top().status = 3;
      top().i = top().k;
      break;
    case 3:
      if (top().i >= m) {
        top().status = -1;
      } else {
        top().status = 4;
      }
      break;
    case 4:
      top().status = 5;
      swap(list[top().i], list[top().k]);
//      break;              // 没有入栈(函数调用),出栈(函数返回)的状态理论上都可以合并.  因为他们都属于同一帧
//                          // 不过感觉合不合并意义都不大,因为在拆分状态图的时候该合并的就合并了. 不合并的原因是因为合并后状态图的流转会变的很复杂
//    case 5:
      top().status = 6;             // 在进行新的函数调用时,必须先设置好本次状态机下次要流转的状态
      // caf_prem(list, k + 1,m);
      push({0, top().k + 1});
      break;
    case 6:
      top().status = 3;
      swap(list[top().i], list[top().k]);
      top().i = top().i + 1;
      break;
    case -1:
      pop();
      break;
    }
  }
}

int main() {
  vector<int> data{1, 2};
  perm(data, 0);
  cout << "---" << endl;
  caf_prem_v3(data, 0, data.size());
}
