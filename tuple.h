//
// Created by 洪陈杰 on 2022/8/13.
//

#ifndef JSTL_TUPLE_H
#define JSTL_TUPLE_H

namespace jstl {
  /**
   * 元祖，任意类型的组合
   * https://zhuanlan.zhihu.com/p/143715615
   *
   * 基本思路：模版的递归继承
   * std::tuple< char, short, int, double > 将继承自
   * ①.std::tuple < short, int, double >    继续“递归”继承
   * ②.std::tuple < int, double >
   * ③.std::tuple < double >
   * ④.std::tuple <>                        递归终止
   *
   * 对象内存结构是先父后子的。因此递归继承导致在 tuple 对象的内存结构中，
   * 变量的存放顺序（变量在对象内存中的地址高低）与实际声明的先后顺序相反
   *
   * 注意 tuple 的递归继承需要使用 private 继承，这是为了防止 tuple 产生
   * 错误的多态函数调用，
   * 如 void foo(tuple<double>) 被传入 tuple<int, double> 类型的实参，
   * 因为 public 继承链下，tuple<int, double> 是 tuple<double> 的子类
   */

  // tuple 原始版本
  template<typename ...args_type>
  class tuple;

  // tuple 无参，递归终止条件
  template<>
  class tuple<>{};

  // tuple 的带参偏特化，第一个参数被特化成 this_type 了
  // 递归向下，每次递归确定一个 "this_type val"
  template<typename this_type, typename ...args_type>
  class tuple<this_type, args_type...> : private tuple<args_type...> {
  public:
    // 保存本层的 val，this_type 右边的保存在父类中
    this_type val;
    tuple(this_type v, args_type ... params) : tuple<args_type...>(params...) {
      val = v;
    }
    this_type getVal() { return val; }
  };

  /**
   * element 辅助类，用于从 tuple 中取出对应位置的值
   * @tparam N 需要取出的位置
   * @tparam args_type tuple 的类型参数
   */
  template<int N, typename ...args_type>
  struct element;

  // N > tuple size 时 get 出错，tuple 已经展开到无参数了，N 还不为 0
  template<int N>
  struct element<N, tuple<>> {
    static_assert(N > 0, "tuple field get out of range");
  };

  // tuple展开模板，通过对自身偏特化版本“继承”，从而获得对应的节点类型
  template<int N, typename this_type, typename ... args_type>
  struct element<N, tuple<this_type, args_type...>>
          : public element<N-1, tuple<args_type...>>{};

  // 展开到对应节点，偏特化 element 的第一个参数为 0
  template<typename this_type, typename ... args_type>
  struct element<0, tuple<this_type, args_type...>> {
    using value_type = this_type;
    // N 为 0 时，tuple 继承链的类型
    using class_type = tuple<this_type, args_type...>;
  };

  /**
   * 辅助类，用于从 tuple 中取出对应位置的值
   * @tparam N 需要取出的位置
   * @tparam args_type tuple 的类型参数
   * @param t 操作的 tuple
   */
  template<int N, typename ...args_type>
  auto get(tuple<args_type...> &t) {
    // 展开到对应层 tuple 类型
    using class_type = typename element<N, tuple<args_type...>>::class_type;
    // 子类向父类转化，即取父类的 val
    return ((class_type&)t).getVal();
  }
}

#endif //JSTL_TUPLE_H
