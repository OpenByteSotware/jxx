#include "io/jxx.io.Console.h"
#include "io/jxx.io.BufferedReader.h"
#include "io/jxx.io.PrintWriter.h"
#include "io/jxx.io.Reader.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::io
{
	Console::Console(const ::jxx::Ptr<Reader>& r, const ::jxx::Ptr<PrintWriter>& w) :reader_(r), writer_(w)
	{
		if (!r || !w)throw ::jxx::lang::NullPointerException();
	} ::jxx::Ptr<PrintWriter> Console::writer()const
	{
		return writer_;
	} ::jxx::Ptr<Reader> Console::reader()const
	{
		return reader_;
	} Console& Console::format(const ::jxx::Ptr<::jxx::lang::String>& f)
	{
		writer_->print(f); return *this;
	} Console& Console::printf(const ::jxx::Ptr<::jxx::lang::String>& f)
	{
		return format(f);
	} ::jxx::Ptr<::jxx::lang::String> Console::readLine()
	{
		auto b = ::jxx::NEW<BufferedReader>(reader_); return b->readLine();
	} ::jxx::Ptr<::jxx::lang::String> Console::readLine(const ::jxx::Ptr<::jxx::lang::String>& f)
	{
		writer_->print(f); writer_->flush(); return readLine();
	} ::jxx::lang::CharArray Console::readPassword()
	{
		auto s = readLine(); return s ? s->toCharArray() : nullptr;
	} ::jxx::lang::CharArray Console::readPassword(const ::jxx::Ptr<::jxx::lang::String>& f)
	{
		writer_->print(f); writer_->flush(); return readPassword();
	} void Console::flush()
	{
		writer_->flush();
	}
}
