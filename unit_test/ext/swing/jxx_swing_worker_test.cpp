#include <gtest/gtest.h>

#include "ext/swing/jxx.ext.swing.SwingWorker.h"
#include "lang/jxx.lang.String.h"

namespace {

class StringWorker final
    : public ::jxx::ext::swing::SwingWorker<
          ::jxx::lang::String,
          ::jxx::lang::String> {
public:
    ::jxx::lang::jbool doneCalled{false};

protected:
    ::jxx::Ptr<::jxx::lang::String> doInBackground() override {
        setProgress(100);
        return ::jxx::NEW<::jxx::lang::String>("complete");
    }

    void done() override {
        doneCalled = true;
    }
};

TEST(SwingWorkerParity, ExecutesAndReturnsResult)
{
    const auto worker = ::jxx::NEW<StringWorker>();
    worker->execute();
    const auto result = worker->get();
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->toString()->utf8(), "complete");
    EXPECT_TRUE(worker->isDone());
    EXPECT_EQ(worker->getProgress(), 100);
}

TEST(SwingWorkerParity, CancelTransitionsToDone)
{
    const auto worker = ::jxx::NEW<StringWorker>();
    EXPECT_TRUE(worker->cancel(false));
    EXPECT_TRUE(worker->isCancelled());
    EXPECT_TRUE(worker->isDone());
    EXPECT_THROW(worker->get(), ::jxx::util::concurrent::CancellationException);
}

} // namespace
