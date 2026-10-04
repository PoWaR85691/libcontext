#include <context.h>

#include <cassert>

void defaultCntrTest()
{
    struct WithDefaultCntr
    {
        WithDefaultCntr() {}
    };

    struct WithoutDefaultCntr
    {
        WithoutDefaultCntr(int) {}
    };

    struct PrivateDefaultCntr
    {
    private:
        PrivateDefaultCntr() {}
    };

    static_assert(details::DefaultConstructor<int>::exists);
    static_assert(details::DefaultConstructor<WithDefaultCntr>::exists);
    static_assert(!details::DefaultConstructor<WithoutDefaultCntr>::exists);
    static_assert(!details::DefaultConstructor<PrivateDefaultCntr>::exists);
}

struct TestStruct
{
    TestStruct() : value(5) {}
    TestStruct(int val) : value(val) {}
    int value;
};

void createDefaultTest()
{
    Context ctx;

    auto v = ctx.create<TestStruct>();
    assert(v->value == 5);
}

void createCustomCntrTest()
{
    Context ctx;

    ctx.resetConstructor<TestStruct>([] { return std::make_shared<TestStruct>(1); });
    auto v1 = ctx.create<TestStruct>();
    assert(v1->value == 1);

    ctx.resetConstructor<TestStruct>([] { return std::make_shared<TestStruct>(10); });
    auto v2 = ctx.create<TestStruct>();
    assert(v2->value == 10);
}

void singletonDefaultTest()
{
    Context ctx;

    auto s = ctx.singleton<TestStruct>();
    assert(s->value == 5);

    s->value = 11;

    auto s2 = ctx.singleton<TestStruct>();
    assert(s2->value == 11);
}

void singletonResetTest()
{
    Context ctx;

    auto s = ctx.singleton<TestStruct>();
    assert(s->value == 5);

    ctx.resetSingleton<TestStruct>(std::make_shared<TestStruct>(99));

    auto s2 = ctx.singleton<TestStruct>();
    assert(s2->value == 99);
}

void singletonCustomCntrTest()
{
    Context ctx;

    ctx.resetConstructor<TestStruct>([] { return std::make_shared<TestStruct>(42); });

    auto s = ctx.singleton<TestStruct>();
    assert(s->value == 42);
}

struct TestStructWithoutDefaultCntr
{
    TestStructWithoutDefaultCntr(int val) : value(val) {}
    int value;
};

void exceptOnNoCntrTest()
{
    Context ctx;

    try {
        ctx.singleton<TestStructWithoutDefaultCntr>();
        assert(false);
    } catch (const std::runtime_error&) {}
}

void resetCntrTest()
{
    Context ctx;

    ctx.resetConstructor<TestStructWithoutDefaultCntr>([] {
        return std::make_shared<TestStructWithoutDefaultCntr>(15);
    });

    auto s = ctx.singleton<TestStructWithoutDefaultCntr>();
    assert(s->value == 15);
}

struct Base
{
    virtual int foo() { return 0; }
};

struct Child : Base
{
    int foo() override { return 42; }
};

void polymorphicTest()
{
    Context ctx;

    ctx.resetConstructor<Base>([] { return std::make_shared<Child>(); });

    auto s = ctx.singleton<Base>();
    assert(s->foo() == 42);
}

int main()
{
    defaultCntrTest();
    createDefaultTest();
    createCustomCntrTest();
    singletonDefaultTest();
    singletonResetTest();
    singletonCustomCntrTest();
    exceptOnNoCntrTest();
    resetCntrTest();
    polymorphicTest();
}
