#pragma once

extern "C++" {

typedef char32_t uchar_t;

#include <utils/baseutils.hpp>
#include <utility>

#define __NAMESPACE_DECL(x) namespace EXOS::x {
#define __NAMESPACE_END }
#define USE(x) using namespace x

namespace EXOS {
}
USE(EXOS);

/**
 * @brief 定义运算符以允许placement new
 */
#define __PLACEMENTNEW_DEFAULT            \
	void *operator new(size_t, void *ptr) { \
		return ptr;                           \
	}

/**
 * @brief 定义一个函数指针
 * @tparam R 返回值类型
 * @tparam ...P 参数类型
 */
template<typename R, typename... P>
using Fn = R (*)(P...);

template<typename T>
using Ptr = T *;
}

/**
 * @brief 在栈上new一个临时对象
 */
#define __snew(type, ...) (new(__builtin_alloca(sizeof(type))) type(__VA_ARGS__))