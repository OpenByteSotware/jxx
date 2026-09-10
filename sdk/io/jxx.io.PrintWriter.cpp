#include "io/jxx.io.PrintWriter.h"
#include <string>
#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.OutputStreamWriter.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::io
{
	PrintWriter::PrintWriter(const ::jxx::Ptr<Writer>& o) :PrintWriter(o, false)
	{
	} PrintWriter::PrintWriter(const ::jxx::Ptr<Writer>& o, ::jxx::lang::jbool a) :out_(o), autoFlush_(a)
	{
		if (!o)throw ::jxx::lang::NullPointerException();
	} PrintWriter::PrintWriter(const ::jxx::Ptr<OutputStream>& o) :PrintWriter(o, false)
	{
	} PrintWriter::PrintWriter(const ::jxx::Ptr<OutputStream>& o, ::jxx::lang::jbool a) :PrintWriter(::jxx::NEW<OutputStreamWriter>(o), a)
	{
	} PrintWriter::~PrintWriter()
	{
		close();
	} void PrintWriter::flush()
	{
		if (!out_)return; try {
			out_->flush();
		}
		catch (...) {
			setError();
		}
	} void PrintWriter::close()
	{
		if (closed_)return; flush(); try {
			if (out_)out_->close();
		}
		catch (...) {
			setError();
		}out_.reset(); closed_ = true;
	} ::jxx::lang::jbool PrintWriter::checkError()
	{
		flush(); return trouble_;
	} void PrintWriter::write(::jxx::lang::jint v)
	{
		try {
			out_->write(v);
		}
		catch (...) {
			setError();
		}
	} void PrintWriter::write(const ::jxx::lang::CharArray& b, ::jxx::lang::jint o, ::jxx::lang::jint l)
	{
		try {
			out_->write(b, o, l);
		}
		catch (...) {
			setError();
		}
	} void PrintWriter::write(const ::jxx::Ptr<::jxx::lang::String>& s, ::jxx::lang::jint o, ::jxx::lang::jint l)
	{
		try {
			out_->write(s, o, l);
		}
		catch (...) {
			setError();
		}
	} void PrintWriter::text(const ::jxx::Ptr<::jxx::lang::String>& s)
	{
		auto v = s ? s : jxx::NEW<::jxx::lang::String>("null"); write(v, 0, v->length());
	} void PrintWriter::print(const ::jxx::Ptr<::jxx::lang::String>& v)
	{
		text(v);
	} void PrintWriter::print(const ::jxx::Ptr<::jxx::lang::Object>& v)
	{
		text(v ? v->toString() : nullptr);
	} void PrintWriter::print(::jxx::lang::jbool v)
	{
		text(::jxx::NEW<::jxx::lang::String>(v ? "true" : "false"));
	} void PrintWriter::print(::jxx::lang::jchar v)
	{
		text(::jxx::NEW<::jxx::lang::String>(std::u16string(1, v)));
	} void PrintWriter::print(::jxx::lang::jint v)
	{
		text(::jxx::NEW<::jxx::lang::String>(std::to_string(v)));
	} void PrintWriter::print(::jxx::lang::jlong v)
	{
		text(::jxx::NEW<::jxx::lang::String>(std::to_string(v)));
	} void PrintWriter::print(::jxx::lang::jfloat v)
	{
		text(::jxx::NEW<::jxx::lang::String>(std::to_string(v)));
	} void PrintWriter::print(::jxx::lang::jdouble v)
	{
		text(::jxx::NEW<::jxx::lang::String>(std::to_string(v)));
	} void PrintWriter::println()
	{
		text(::jxx::NEW<::jxx::lang::String>("\n")); if (autoFlush_)flush();
	} void PrintWriter::println(const ::jxx::Ptr<::jxx::lang::String>& v)
	{
		print(v); println();
	} void PrintWriter::println(const ::jxx::Ptr<::jxx::lang::Object>& v)
	{
		print(v); println();
	} void PrintWriter::println(::jxx::lang::jbool v)
	{
		print(v); println();
	} void PrintWriter::println(::jxx::lang::jchar v)
	{
		print(v); println();
	} void PrintWriter::println(::jxx::lang::jint v)
	{
		print(v); println();
	} void PrintWriter::println(::jxx::lang::jlong v)
	{
		print(v); println();
	} void PrintWriter::println(::jxx::lang::jfloat v)
	{
		print(v); println();
	} void PrintWriter::println(::jxx::lang::jdouble v)
	{
		print(v); println();
	} void PrintWriter::setError()
	{
		trouble_ = true;
	} void PrintWriter::clearError()
	{
		trouble_ = false;
	}
}
