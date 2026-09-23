#include <gtest/gtest.h>

#include <cstdio>

#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.FileOutputStream.h"
#include "io/jxx.io.SyncFailedException.h"
#include "lang/jxx.lang.String.h"

namespace {
TEST(FileDescriptorSyncTest, SyncSucceedsWhileOpenAndFailsAfterOwningStreamClose) {
    const auto path=::jxx::NEW<::jxx::lang::String>("jxx_filedescriptor_sync.tmp");
    std::remove(path->utf8().c_str());
    const auto output=::jxx::NEW<::jxx::io::FileOutputStream>(path);
    const auto descriptor=output->getFD();
    output->write(9);
    EXPECT_NO_THROW(descriptor->sync());
    output->close();
    EXPECT_THROW(descriptor->sync(),::jxx::io::SyncFailedException);
    std::remove(path->utf8().c_str());
}
}
