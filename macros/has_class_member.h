#pragma once

// F 函数名
// C 检测器
// 用法
// 1、创建检测器
// DECLARE_HAS_CLASS_MEMBER(foo, fooable);
// 2、使用检测器检测
// struct Foo {
//   void foo(int i) {
//     std::cout << "foo " << i << std::endl;
//   }
// };
// if constexpr (fooable<Foo, int>::value) {
//   std::cout << "yes" << std::endl;
// } else {
//   std::cout << "no" << std::endl;
// }
#define DECLARE_HAS_CLASS_MEMBER(F, C)                                                  \
template<typename T, typename... Args>                                                  \
struct C {                                                                              \
    template<typename U>                                                                \
    constexpr static auto check(int)->                                                  \
        decltype(std::declval<U>().F(std::declval<Args>()...), std::true_type());       \
                                                                                        \
    template<typename U>                                                                \
    constexpr static std::false_type check(...);                                        \
                                                                                        \
    static constexpr bool value = decltype(check<T>(0))::value;                         \
}
