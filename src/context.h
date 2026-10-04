#ifndef CONTEXT_H
#define CONTEXT_H

#include <memory>
#include <functional>

#include "context_data.h"

class Context
{
public:
    template <typename T>
    using Instance = std::shared_ptr<T>;

    template <typename T>
    using Constructor = std::function<Instance<T>()>;

    template<typename T>
    T* singleton();

    template<typename T>
    Instance<T> create();

    template<typename T>
    void resetSingleton(Instance<T> sgtn = nullptr);

    template<typename T>
    void resetConstructor(Constructor<T> cntr = nullptr);

    Context() = default;
    ~Context() = default;

    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

    Context(Context&&) = default;
    Context& operator=(Context&&) = default;

private:
    details::ContextData d;
};

#include "context.cpp.h"

#endif
