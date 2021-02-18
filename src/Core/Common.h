#pragma once

#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#define DEFINE_ENUM_OPERATORS(ENUM_TYPE) \
inline constexpr ENUM_TYPE operator|(const ENUM_TYPE & a, const ENUM_TYPE & b) { \
    return static_cast<ENUM_TYPE>(static_cast<int>(a) | static_cast<int>(b)); \
} \
inline constexpr ENUM_TYPE operator&(const ENUM_TYPE & a, const ENUM_TYPE & b) { \
    return static_cast<ENUM_TYPE>(static_cast<int>(a) & static_cast<int>(b)); \
}\
inline constexpr ENUM_TYPE operator<<(const ENUM_TYPE & a, const int & b) { \
    return static_cast<ENUM_TYPE>(static_cast<int>(a) << b); \
}\
inline constexpr ENUM_TYPE operator>>(const ENUM_TYPE & a, const int & b) { \
    return static_cast<ENUM_TYPE>(static_cast<int>(a) >> b); \
}\
inline constexpr bool operator||(const ENUM_TYPE & a, const ENUM_TYPE & b) { \
    return static_cast<int>(a) || static_cast<int>(b); \
}
