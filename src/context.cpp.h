#ifndef CONTEXT_CPP_H
#define CONTEXT_CPP_H

#include "context.h"

#include <stdexcept>
#include <string>

using namespace std::string_literals;

namespace details {

template<typename T, typename = void>
struct DefaultConstructor
{
    static constexpr bool exists = false;

    static Context::Instance<T> create()
    {
        throw std::runtime_error(typeid(T).name() + " does not have default constructor"s);
    }
};

template<typename T>
struct DefaultConstructor<T, std::enable_if_t<std::is_default_constructible_v<T>>>
{
    static constexpr bool exists = true;

    static Context::Instance<T> create()
    {
        return std::make_shared<T>();
    }
};

}

template<typename T>
T* Context::singleton()
{
    auto sgtnIt = d.singletons.find(typeid(T));
    if (sgtnIt == d.singletons.end()) {
        sgtnIt = d.singletons.emplace(typeid(T), create<T>()).first;
    }
    auto& sgtn = sgtnIt->second;
    return std::any_cast<Instance<T>&>(sgtn).get();
}

template<typename T>
Context::Instance<T> Context::create()
{
    auto cntrIt = d.constructors.find(typeid(T));
    if (cntrIt != d.constructors.end()) {
        auto& cntr = std::any_cast<Constructor<T>&>(cntrIt->second);
        return cntr();
    } else {
        return details::DefaultConstructor<T>::create();
    }
}

template<typename T>
void Context::resetSingleton(Instance<T> sgtn)
{
    if (sgtn) {
        d.singletons[typeid(T)] = std::move(sgtn);
    } else {
        d.singletons.erase(typeid(T));
    }
}

template<typename T>
void Context::resetConstructor(Constructor<T> cntr)
{
    if (cntr) {
        d.constructors[typeid(T)] = std::move(cntr);
    } else {
        d.constructors.erase(typeid(T));
    }
}

#endif
