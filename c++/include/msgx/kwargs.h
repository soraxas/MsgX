#pragma once

#include <string>

#include "msgx/conversion/all.h"

template <class F>
auto make_shared_function(F &&f)
{
    return [pf = std::make_shared<std::decay_t<F>>(std::forward<F>(f))](auto &&...args) -> decltype(auto)
    { return (*pf)(decltype(args)(args)...); };
}

namespace msgx
{

// forward declare
class MessageX;
class BindableOpaqueItem;

namespace
{
// hide the following class

using KwargsBindedFunctor = std::function<void(msgx::BindableOpaqueItem &item)>;

class KwargsIntermediate;

class Kwargs
{
private:
    Kwargs(std::string key, KwargsBindedFunctor functor) : key_(std::move(key)), build_functor_(std::move(functor))
    {
    }

    friend KwargsIntermediate;
    friend MessageX;

    std::string key_;
    KwargsBindedFunctor build_functor_;
};

// an intermediate representation before the assignment
class KwargsIntermediate
{
public:
    explicit KwargsIntermediate(std::string key) : key_(std::move(key))
    {
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-unconventional-assign-operator"
    template <typename T>
    Kwargs operator=(T &val)
    {
        return Kwargs{key_, [val = std::forward<T>(val)](msgx::BindableOpaqueItem &item)
                      { ::msgx::conversion::opaque_item(item, val); }};
    }

    template <typename T>
    Kwargs operator=(T &&val)
    {
        // move the non-copyable rvalue to a shared pointer
        auto non_copyable = std::make_shared<std::decay_t<T>>(std::forward<T>(val));

        return Kwargs{key_, [val = std::move(non_copyable)](msgx::BindableOpaqueItem &item)
                      { ::msgx::conversion::opaque_item(item, std::move(*val)); }};
    }

    template <typename T>
    Kwargs operator=(const std::initializer_list<T> &other)
    {
        return operator=<const std::initializer_list<T>>(other);
    }
#pragma clang diagnostic pop

private:
    std::string key_;
};

}  // namespace

inline namespace kwargs
{
// allow user to use this kwargs track
KwargsIntermediate operator""_kw(const char *str, std::size_t)
{
    return KwargsIntermediate{str};
}
}  // namespace kwargs

}  // namespace msgx
