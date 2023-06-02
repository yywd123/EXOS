#pragma once

extern "C++" {

/**
 * @brief 定义运算符以允许placement new
 */
#define __PLACEMENTNEW_DEFAULT void* operator new(size_t size, void* ptr) {return ptr;}

/**
 * @brief 
 * @tparam R 返回值类型
 * @tparam ...P 参数类型
 */
template<typename R, typename... P>
using Fn = R(*)(P...);

}