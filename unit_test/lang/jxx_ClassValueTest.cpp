#include <gtest/gtest.h>

#include <stdexcept>

#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.ClassValue.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace {

class CountingClassValue final
    : public jxx::lang::ClassValue<jxx::lang::jint> {
public:
    CountingClassValue() = default;

    jxx::lang::jint computationCount() const noexcept {
        return computationCount_;
    }

protected:
    jxx::lang::jint computeValue(
        const jxx::Ptr<jxx::lang::ClassAny>& type) override {
        (void)type;
        return ++computationCount_;
    }

private:
    jxx::lang::jint computationCount_ = 0;
};

class ThrowingClassValue final
    : public jxx::lang::ClassValue<jxx::lang::jint> {
public:
    ThrowingClassValue() = default;

    jxx::lang::jint computationCount() const noexcept {
        return computationCount_;
    }

protected:
    jxx::lang::jint computeValue(
        const jxx::Ptr<jxx::lang::ClassAny>& type) override {
        (void)type;
        ++computationCount_;
        if (computationCount_ == 1) {
            throw std::runtime_error("expected ClassValue computation failure");
        }
        return computationCount_;
    }

private:
    jxx::lang::jint computationCount_ = 0;
};

TEST(ClassValueTest, CachesValueForSameClass) {
    auto value = jxx::NEW<CountingClassValue>();
    const auto type = jxx::lang::String::Class();

    EXPECT_EQ(value->get(type), 1);
    EXPECT_EQ(value->get(type), 1);
    EXPECT_EQ(value->computationCount(), 1);
}

TEST(ClassValueTest, MaintainsIndependentValuesPerClass) {
    auto value = jxx::NEW<CountingClassValue>();

    EXPECT_EQ(value->get(jxx::lang::String::Class()), 1);
    EXPECT_EQ(value->get(jxx::lang::Boolean::Class()), 2);
    EXPECT_EQ(value->get(jxx::lang::String::Class()), 1);
    EXPECT_EQ(value->get(jxx::lang::Boolean::Class()), 2);
    EXPECT_EQ(value->computationCount(), 2);
}

TEST(ClassValueTest, RemoveForcesRecomputation) {
    auto value = jxx::NEW<CountingClassValue>();
    const auto type = jxx::lang::String::Class();

    EXPECT_EQ(value->get(type), 1);
    value->remove(type);
    EXPECT_EQ(value->get(type), 2);
    EXPECT_EQ(value->get(type), 2);
    EXPECT_EQ(value->computationCount(), 2);
}

TEST(ClassValueTest, ExceptionIsPropagatedAndNotCached) {
    auto value = jxx::NEW<ThrowingClassValue>();
    const auto type = jxx::lang::String::Class();

    EXPECT_THROW(value->get(type), std::runtime_error);
    EXPECT_EQ(value->computationCount(), 1);

    EXPECT_EQ(value->get(type), 2);
    EXPECT_EQ(value->get(type), 2);
    EXPECT_EQ(value->computationCount(), 2);
}

TEST(ClassValueTest, NullClassIsRejected) {
    auto value = jxx::NEW<CountingClassValue>();

    EXPECT_THROW(
        value->get(nullptr),
        jxx::lang::NullPointerException);
    EXPECT_THROW(
        value->remove(nullptr),
        jxx::lang::NullPointerException);
    EXPECT_EQ(value->computationCount(), 0);
}

} // namespace
