#pragma once

extern "C++" {

#define __NAMESPACE_DECL(x) namespace EXOS::x {
#define __NAMESPACE_END }
#define USE(x) using namespace x

/**
 * @brief 定义运算符以允许placement new
 */
#define __PLACEMENTNEW_DEFAULT void* operator new(size_t size, void* ptr) {return ptr;}

/**
 * @brief 定义一个函数指针
 * @tparam R 返回值类型
 * @tparam ...P 参数类型
 */
template<typename R, typename... P>
using Fn = R(*)(P...);

template<typename T>
using Ptr = T*;

template<typename T>
using Ref = T&;

}