//
// Created by 洪陈杰 on 2022/8/13.
//

#ifndef JSTL_FUNCTION_H
#define JSTL_FUNCTION_H

#include "utility.h"

namespace jstl {
  // 默认特化不实现
  template<typename T>
  class function;

  template<typename ret_type, typename ...args_type>
  class function<ret_type(args_type...)> {
    // 虚基类，可调用对象的类型，规定可调用对象能够执行的操作，包括拷贝和被调用
    struct callable_base {
      virtual ret_type operator()(args_type&&... params) = 0;
      virtual callable_base *copy() const = 0;
      virtual ~callable_base() = default;
    };

    // 实际可调用的类型，继承虚基类，func_type 表示 function 实际容纳的可调用类型
    template<typename func_type>
    struct callable_derived : public callable_base {
      func_type f;

      callable_derived(func_type functor) : f(functor) {}

      ret_type operator()(args_type &&... params) override {
        return f(forward<args_type>(params)...);
      }

      callable_base *copy() const override {
        return new callable_derived<func_type>(f);
      }
    };

    struct callable_base *base; // 多态指针，指向在堆上分配的可调用对象
  public:
    function() : base(nullptr) {}

    template<typename T>
    function(T f) : base(new callable_derived<T>(f)) {}

    function(const function &f) {
      base = f.base->copy();
    }
    // 拷贝复制 function
    function &operator=(const function &f) {
      if (base != f.base) {
        delete base;
        base = f.base->copy();
        return *this;
      }
    }

    ~function() {
      delete base;
    }
    // 调用 function
    ret_type operator()(args_type... params) {
      return (*base)(forward<args_type>(params)...);
    }
  };
}

#endif //JSTL_FUNCTION_H
