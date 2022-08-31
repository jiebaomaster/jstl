//
// Created by 洪陈杰 on 2022/8/13.
//

#ifndef JSTL_UTILITY_H
#define JSTL_UTILITY_H

#include "type_traits.h"

namespace jstl {
  /**
   * move
   * 移动，将参数 t 的类型强制转化为右值引用
   * 使用 remove_reference 避免引用折叠
   */
  template<typename T>
  inline typename remove_reference<T>::type&&
  move(T&& t) {
    return static_cast<typename remove_reference<T>::type&&>(t);
  }


  /**
   * forward
   * 完美转发，利用引用折叠保留引用的左右性
   * forward 类型参数 T 不能推导，需要手动指定，
   * 实际是将参数 t 的类型强制转化成 T 指定的类型
   *
   * T&& t&  -> t&&
   * T&  t&  -> t&
   * T&& t&& -> t&&
   * T&  t&& -> 错误，不能将右值转成左值
   */
  // 模版中配合万能引用使用的 forward 使用该路径
  template<typename T>
  inline T&& forward(typename remove_reference<T>::type& t) {
    return static_cast<T&&>(t);
  }

  template<typename T>
  inline T&& forward(typename remove_reference<T>::type&& t) {
    static_assert(!is_lvalue_reference<T>::value, "can not forward an rvalue as an lvalue");
    return static_cast<T&&>(t);
  }
}

#endif //JSTL_UTILITY_H
