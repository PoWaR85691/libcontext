#ifndef CONTEXT_DATA_H
#define CONTEXT_DATA_H

#include <memory>
#include <functional>
#include <any>
#include <typeindex>
#include <unordered_map>

namespace details {

struct ContextData
{
    std::unordered_map<std::type_index /* T */,
                       std::any /* Instance<T> */> singletons;
    std::unordered_map<std::type_index /* T */,
                       std::any /* Constructor<T> */> constructors;
};

}

#endif
