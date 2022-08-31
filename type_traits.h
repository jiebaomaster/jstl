//
// Created by 洪陈杰 on 2022/8/13.
//

#ifndef JSTL_TYPE_TRAITS_H
#define JSTL_TYPE_TRAITS_H

namespace jstl {
  struct true_type { static const bool value = true; };
  struct false_type { static const bool value = false; };

  /**
   * 判断 T 是否是左值引用
   */
  template<typename T>
  struct is_lvalue_reference : public false_type {};
  template<typename T>
  struct is_lvalue_reference<T&> : public true_type {};

  /**
   * 去除类型的引用
   * 利用模版特化，不同的引用类型调用不同的特化版本
   */
  template<typename T>
  struct remove_reference { using type = T; };
  template<typename T>
  struct remove_reference<T&> { using type = T; };
  template<typename T>
  struct remove_reference<T&&> { using type = T; };
}

#endif //JSTL_TYPE_TRAITS_H
