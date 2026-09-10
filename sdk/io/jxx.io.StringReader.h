#pragma once
#include "io/jxx.io.Reader.h"
namespace jxx::lang
{
	class String;
}
namespace jxx::io
{
	class StringReader final : public ::jxx::lang::ClassBase<StringReader, Reader>
	{
	public:
		using JxxSuper = Reader; using Super = ::jxx::lang::ClassBase<StringReader, JxxSuper>;
		explicit StringReader(const ::jxx::Ptr<::jxx::lang::String>& value);
		::jxx::lang::jint read() override; 
		::jxx::lang::jint read(const ::jxx::lang::CharArray& buffer, ::jxx::lang::jint offset, 
			::jxx::lang::jint length) override;
		::jxx::lang::jlong skip(::jxx::lang::jlong count) override;
		::jxx::lang::jbool ready() override;
		::jxx::lang::jbool markSupported()const override; 
		void mark(::jxx::lang::jint limit) override; 
		void reset() override;
		void close() override;
	private: void ensureOpen()const;
		   ::jxx::Ptr<::jxx::lang::String> value_; ::jxx::lang::jint position_ = 0, mark_ = 0;
	};
} // namespace jxx::io
