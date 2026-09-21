#include <gtest/gtest.h>

#include <atomic>

#include "io/jxx.io.InvalidObjectException.h"
#include "lang/jxx.lang.Enum.h"
#include "lang/jxx.lang.InheritableThreadLocal.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"

namespace {
using ::jxx::lang::Enum;
using ::jxx::lang::InheritableThreadLocal;
using ::jxx::lang::Runnable;
using ::jxx::lang::String;
using ::jxx::lang::Thread;

class TransformingLocal final : public InheritableThreadLocal<String> {
public:
    std::atomic<int> childCalls{0};
protected:
    ::jxx::Ptr<String> childValue(const ::jxx::Ptr<String>& parent) override {
        ++childCalls;
        return ::jxx::NEW<String>(parent->utf8() + "-child");
    }
};

class CaptureRunnable final : public ::jxx::lang::ClassBase<CaptureRunnable, ::jxx::lang::Object, Runnable> {
public:
    using JxxSuper = ::jxx::lang::Object;
    CaptureRunnable(const ::jxx::Ptr<TransformingLocal>& local,
                    ::jxx::Ptr<String>& result)
        : local_(local), result_(result) {}
    void run() override { result_ = local_->get(); }
private:
    ::jxx::Ptr<TransformingLocal> local_;
    ::jxx::Ptr<String>& result_;
};

class SampleEnum final : public Enum<SampleEnum> {
public:
    SampleEnum(const ::jxx::Ptr<String>& name, ::jxx::lang::jint ordinal)
        : Enum<SampleEnum>(name, ordinal) {}
};

TEST(InheritableThreadLocalParityTest, CapturesTransformedValueWhenChildIsConstructed) {
    const auto local = ::jxx::NEW<TransformingLocal>();
    local->set(::jxx::NEW<String>("parent"));
    ::jxx::Ptr<String> childValue;
    const auto runnable = ::jxx::NEW<CaptureRunnable>(local, childValue);
    const auto child = ::jxx::NEW<Thread>(::jxx::CAST<Runnable>(runnable));

    EXPECT_EQ(1, local->childCalls.load());
    local->set(::jxx::NEW<String>("changed"));
    child->start();
    child->join();

    ASSERT_NE(nullptr, childValue);
    EXPECT_EQ("parent-child", childValue->utf8());
    EXPECT_EQ("changed", local->get()->utf8());
}

TEST(InheritableThreadLocalParityTest, ChildWithoutParentValueUsesInitialValue) {
    const auto local = ::jxx::NEW<TransformingLocal>();
    ::jxx::Ptr<String> childValue;
    const auto runnable = ::jxx::NEW<CaptureRunnable>(local, childValue);
    const auto child = ::jxx::NEW<Thread>(::jxx::CAST<Runnable>(runnable));
    EXPECT_EQ(0, local->childCalls.load());
    child->start();
    child->join();
    EXPECT_EQ(nullptr, childValue);
}

TEST(EnumParityTest, NameOrdinalIdentityAndOrderingAreStable) {
    const auto first = ::jxx::NEW<SampleEnum>(::jxx::NEW<String>("FIRST"), 0);
    const auto second = ::jxx::NEW<SampleEnum>(::jxx::NEW<String>("SECOND"), 1);
    EXPECT_EQ("FIRST", first->name()->utf8());
    EXPECT_EQ(0, first->ordinal());
    EXPECT_EQ("FIRST", first->toString()->utf8());
    EXPECT_LT(first->compareTo(second), 0);
    EXPECT_TRUE(first->equals(::jxx::CAST<::jxx::lang::Object>(first)));
    EXPECT_FALSE(first->equals(::jxx::CAST<::jxx::lang::Object>(second)));
}

TEST(EnumParityTest, DeserializationHooksRejectEnumReconstruction) {
    const auto value = ::jxx::NEW<SampleEnum>(::jxx::NEW<String>("ONLY"), 0);
    EXPECT_THROW(value->readObject(nullptr), ::jxx::io::InvalidObjectException);
    EXPECT_THROW(value->readObjectNoData(), ::jxx::io::InvalidObjectException);
}

} // namespace
