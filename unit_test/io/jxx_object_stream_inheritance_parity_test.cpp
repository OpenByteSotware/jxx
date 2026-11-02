#include <gtest/gtest.h>
#include <type_traits>
#include "io/jxx.io.DataInputStream.h"
#include "io/jxx.io.DataOutputStream.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.ObjectInput.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutput.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.OutputStream.h"

TEST(ObjectStreamInheritanceParity, ObjectInputStreamHasSourceSuperclassAndInterface) {
    static_assert(std::is_base_of_v<::jxx::io::InputStream, ::jxx::io::ObjectInputStream>);
    static_assert(std::is_base_of_v<::jxx::io::ObjectInput, ::jxx::io::ObjectInputStream>);
    static_assert(!std::is_base_of_v<::jxx::io::DataInputStream, ::jxx::io::ObjectInputStream>);
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::InputStream, ::jxx::io::ObjectInputStream>));
}

TEST(ObjectStreamInheritanceParity, ObjectOutputStreamHasSourceSuperclassAndInterface) {
    static_assert(std::is_base_of_v<::jxx::io::OutputStream, ::jxx::io::ObjectOutputStream>);
    static_assert(std::is_base_of_v<::jxx::io::ObjectOutput, ::jxx::io::ObjectOutputStream>);
    static_assert(!std::is_base_of_v<::jxx::io::DataOutputStream, ::jxx::io::ObjectOutputStream>);
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::OutputStream, ::jxx::io::ObjectOutputStream>));
}
