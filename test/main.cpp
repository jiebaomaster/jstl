//
// Created by 洪陈杰 on 2022/8/13.
//

#include "main.h"
#include <iostream>
#include <string>
#include <cassert>

void foo(int a, double b) {
  std::cout << a << " " << b << std::endl;
}

int main() {
  jstl::tuple<int, double, char, std::string> t{1, 2.1, 'c', "str"};
  assert(jstl::get<0>(t) == 1);
  assert(jstl::get<1>(t) == 2.1);
  assert(jstl::get<2>(t) == 'c');
  assert(jstl::get<3>(t) == "str");

  jstl::function<void(int,double)> f(foo);
  int a = 1;
  f(a, 1.2);
  auto g = f;
  g(2, 3.3);
}