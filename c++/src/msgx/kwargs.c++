#include "msgx/kwargs.h"

namespace msgx
{

Kwargs::Kwargs(std::string key, KwargsBindedFunctor functor) : key_(std::move(key)), build_functor_(std::move(functor))
{
}

KwargsIntermediate::KwargsIntermediate(std::string key) : key_(std::move(key))
{
}

}  // namespace msgx
