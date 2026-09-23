#include <gtest/gtest.h>
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "net/internal/jxx.net.internal.NativeSocketState.h"

namespace
{
	TEST(SocketStreamBoundsTest, RejectsInvalidRangesBeforeNativeIo)
	{
		auto state = std::make_shared<::jxx::net::internal::NativeSocketState>(); 
		::jxx::net::internal::NativeSocketInputStream input(state); 
		::jxx::net::internal::NativeSocketOutputStream output(state);
		auto bytes = std::make_shared<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(4);
		EXPECT_THROW(input.read(bytes, -1, 1), ::jxx::lang::IndexOutOfBoundsException);
		EXPECT_THROW(input.read(bytes, 0, 5), ::jxx::lang::IndexOutOfBoundsException);
		EXPECT_THROW(output.write(bytes, 3, 2), ::jxx::lang::IndexOutOfBoundsException);
		::jxx::lang::ByteArray nullBytes; EXPECT_THROW(input.read(nullBytes, 0, 1),
			::jxx::lang::NullPointerException);
		EXPECT_THROW(output.write(nullBytes, 0, 1), ::jxx::lang::NullPointerException);
	}
}
