#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalThreadStateException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx.lang.ThreadGroup.h"
#include "lang/jxx.lang.buildin_array.h"

namespace {
using ::jxx::lang::IllegalThreadStateException;
using ::jxx::lang::String;
using ::jxx::lang::Thread;
using ::jxx::lang::ThreadGroup;

TEST(ThreadGroupParityTest, ParentRelationshipAndPriorityAreInherited) {
    const auto parent = ::jxx::NEW<ThreadGroup>(::jxx::NEW<String>("parent"));
    parent->setMaxPriority(6);
    const auto child = ::jxx::NEW<ThreadGroup>(parent, ::jxx::NEW<String>("child"));

    EXPECT_EQ(parent.get(), child->getParent().get());
    EXPECT_TRUE(parent->parentOf(child));
    EXPECT_FALSE(child->parentOf(parent));
    EXPECT_EQ(6, child->getMaxPriority());

    child->destroy();
    parent->destroy();
}

TEST(ThreadGroupParityTest, MaxPriorityIsClampedAndPropagated) {
    const auto parent = ::jxx::NEW<ThreadGroup>(::jxx::NEW<String>("priority-parent"));
    const auto child = ::jxx::NEW<ThreadGroup>(parent, ::jxx::NEW<String>("priority-child"));

    parent->setMaxPriority(Thread::MAX_PRIORITY + 100);
    EXPECT_EQ(Thread::MAX_PRIORITY, parent->getMaxPriority());
    parent->setMaxPriority(4);
    EXPECT_EQ(4, parent->getMaxPriority());
    EXPECT_EQ(4, child->getMaxPriority());

    child->destroy();
    parent->destroy();
}

TEST(ThreadGroupParityTest, EnumerationHonorsRecursionFlag) {
    const auto parent = ::jxx::NEW<ThreadGroup>(::jxx::NEW<String>("enumeration-parent"));
    const auto child = ::jxx::NEW<ThreadGroup>(parent, ::jxx::NEW<String>("enumeration-child"));
    const auto groups = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::Ptr<ThreadGroup>, 1U>>(4U);

    EXPECT_EQ(1, parent->enumerate(groups, false));
    EXPECT_EQ(child.get(), (*groups)[0].get());
    EXPECT_EQ(1, parent->activeGroupCount());

    child->destroy();
    parent->destroy();
}

TEST(ThreadGroupParityTest, DestroyedGroupsRejectNewChildren) {
    const auto group = ::jxx::NEW<ThreadGroup>(::jxx::NEW<String>("destroyed"));
    group->destroy();
    EXPECT_TRUE(group->isDestroyed());

    const auto childName = ::jxx::NEW<String>("invalid-child");
    EXPECT_THROW((void)::jxx::NEW<ThreadGroup>(group, childName),
                 IllegalThreadStateException);
}

TEST(ThreadGroupParityTest, ToStringContainsNameAndPriority) {
    const auto group = ::jxx::NEW<ThreadGroup>(::jxx::NEW<String>("description"));
    const auto text = group->toString()->utf8();
    EXPECT_NE(std::string::npos, text.find("description"));
    EXPECT_NE(std::string::npos, text.find("maxpri="));
    group->destroy();
}

} // namespace
