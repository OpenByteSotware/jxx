#include "io/jxx.io.BufferedOutputStream.h"

#include <cstdint>

#include "io/jxx.io.IOException.h"
#include "io/jxx.io.IOHelper.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::io
{

	BufferedOutputStream::BufferedOutputStream(
		const ::jxx::Ptr<OutputStream>& output)
		: BufferedOutputStream(output, 8192)
	{
	}

	BufferedOutputStream::BufferedOutputStream(
		const ::jxx::Ptr<OutputStream>& output,
		::jxx::lang::jint size)
		: Super(output)
		, buffer_(
			  ::jxx::NEW<::jxx::lang::ByteArrayType>(
				  static_cast<std::uint32_t>(
					  validateBufferSize_(size))))
	{
	}

	BufferedOutputStream::~BufferedOutputStream() = default;

	::jxx::lang::jint
		BufferedOutputStream::validateBufferSize_(
			::jxx::lang::jint size)
	{

		if (size <= 0) {
			throw ::jxx::lang::IllegalArgumentException();
		}

		return size;
	}

	void BufferedOutputStream::ensureOpen_() const
	{
		if (closed_ || buffer_ == nullptr || out_ == nullptr) {
			throw IOException();
		}
	}

	void BufferedOutputStream::flushBuffer_()
	{
		ensureOpen_();

		if (count_ <= 0) {
			return;
		}

		out_->write(buffer_, 0, count_);
		count_ = 0;
	}

	void BufferedOutputStream::write(
		::jxx::lang::jint value)
	{

		ensureOpen_();

		if (count_ ==
			static_cast<::jxx::lang::jint>(buffer_->length)) {
			flushBuffer_();
		}

		(*buffer_)[count_++] =
			static_cast<::jxx::lang::jbyte>(value);
	}

	void BufferedOutputStream::write(
		const ::jxx::lang::ByteArray& buffer,
		::jxx::lang::jint offset,
		::jxx::lang::jint length)
	{

		ensureOpen_();
		IOHelper::checkBounds(buffer, offset, length);

		if (length == 0) {
			return;
		}

		const auto capacity =
			static_cast<::jxx::lang::jint>(buffer_->length);

		if (length >= capacity) {
			flushBuffer_();
			out_->write(buffer, offset, length);
			return;
		}

		if (length > capacity - count_) {
			flushBuffer_();
		}

		for (::jxx::lang::jint index = 0;
			 index < length;
			 ++index) {
			(*buffer_)[count_ + index] =
				(*buffer)[offset + index];
		}

		count_ += length;
	}

	void BufferedOutputStream::flush()
	{
		ensureOpen_();
		flushBuffer_();
		out_->flush();
	}

	void BufferedOutputStream::close()
	{
		if (closed_) {
			return;
		}

		try {
			flushBuffer_();
			out_->flush();
		}
		catch (...) {
			try {
				out_->close();
			}
			catch (...) {
			}

			closed_ = true;
			buffer_.reset();
			out_.reset();
			throw;
		}

		out_->close();
		closed_ = true;
		buffer_.reset();
		out_.reset();
	}

} // namespace jxx::io
