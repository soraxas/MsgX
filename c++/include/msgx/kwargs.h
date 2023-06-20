#pragma once

#include <string>

#include "msgx/conversion/all.h"

namespace msgx
{

// forward declare
class MessageX;
class BindableOpaqueItem;

// namespace
//{
//  hide the following class

using KwargsBindedFunctor = std::function<void(msgx::BindableOpaqueItem &item)>;

class KwargsIntermediate;

class Kwargs
{
private:
    Kwargs(std::string key, KwargsBindedFunctor functor);

public:
    friend KwargsIntermediate;
    friend MessageX;

    std::string key_;
    KwargsBindedFunctor build_functor_;
};

// an intermediate representation before the assignment
class KwargsIntermediate
{
public:
    explicit KwargsIntermediate(std::string key);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-unconventional-assign-operator"
    template <typename T>
    Kwargs operator=(T &val)
    {
        using decay_T = typename std::decay<T>::type;

        return Kwargs{key_, [val = std::forward<decay_T>(val)](msgx::BindableOpaqueItem &item)
                      { ::msgx::detail::call_conversion<decay_T>(item, val); }};
    }

    template <typename T>
    Kwargs operator=(T &&val)
    {
        using decay_T = typename std::decay<T>::type;

        // move the non-copyable rvalue to a shared pointer
        auto non_copyable = std::make_shared<decay_T>(std::forward<T>(val));

        return Kwargs{key_, [val = std::move(non_copyable)](msgx::BindableOpaqueItem &item)
                      { ::msgx::detail::call_conversion<decay_T>(item, std::move(*val)); }};
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

//}  // namespace

inline namespace kwargs
{

/** Allow user to use this kwargs trick
 *
 * e.g. msg.Mapping(
 *          "xs"_kw = {1, 2, 3},
 *          "ys"_kw = {4, 5, 6}
 *      );
 *
 * @param str The key of the mapping
 * @return An intermediate Kwargs representation, to be consumed by Mapping
 */
inline KwargsIntermediate operator""_kw(const char *str, std::size_t)
{
    return KwargsIntermediate{str};
}

}  // namespace kwargs

}  // namespace msgx
