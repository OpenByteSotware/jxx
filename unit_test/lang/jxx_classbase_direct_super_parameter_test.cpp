#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace {

class NamedBase
    : public ::jxx::lang::ClassBase<NamedBase, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<NamedBase, JxxSuper>;

    NamedBase() : Super(), name_(::jxx::NEW<::jxx::lang::String>("default")) {}
    explicit NamedBase(const ::jxx::Ptr<::jxx::lang::String>& name)
        : Super(), name_(name) {}

    ::jxx::Ptr<::jxx::lang::String> name() const { return name_; }

private:
    ::jxx::Ptr<::jxx::lang::String> name_;
};

class DefaultDerived final
    : public ::jxx::lang::ClassBase<DefaultDerived, NamedBase> {
public:
    using JxxSuper = NamedBase;
    using Super = ::jxx::lang::ClassBase<DefaultDerived, JxxSuper>;

    DefaultDerived() : Super() {}
};

class NamedDerived final
    : public ::jxx::lang::ClassBase<NamedDerived, NamedBase> {
public:
    using JxxSuper = NamedBase;
    using Super = ::jxx::lang::ClassBase<NamedDerived, JxxSuper>;

    explicit NamedDerived(const ::jxx::Ptr<::jxx::lang::String>& name)
        : Super(name) {}
};

static_assert(std::is_same_v<
    DefaultDerived::JxxSuperType,
    NamedBase>);
static_assert(std::is_same_v<
    NamedDerived::JxxSuperType,
    NamedBase>);

TEST(ClassBaseParity, ExistingJxxSuperAndDefaultSuperPatternRemainValid)
{
    ::jxx::initialize();
    auto value = ::jxx::NEW<DefaultDerived>();
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(value->name()->utf8(), "default");
    EXPECT_NE(value->getClass(), nullptr);
}

TEST(ClassBaseParity, SuperArgumentsReachDirectSuperclassConstructor)
{
    ::jxx::initialize();
    auto value = ::jxx::NEW<NamedDerived>(
        ::jxx::NEW<::jxx::lang::String>("forwarded"));
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(value->name()->utf8(), "forwarded");
    EXPECT_NE(value->getClass(), nullptr);
}

} // namespace
