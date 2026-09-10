#pragma once
#include <cstdio>
#include "io/jxx.io.InputStream.h"
namespace jxx::lang { class String; }
namespace jxx::io { class File; class FileDescriptor; class FileInputStream final : public ::jxx::lang::ClassBase<FileInputStream,InputStream> { public: using JxxSuper=InputStream;using Super=::jxx::lang::ClassBase<FileInputStream,JxxSuper>; explicit FileInputStream(const ::jxx::Ptr<::jxx::lang::String>& name); explicit FileInputStream(const ::jxx::Ptr<File>& file); explicit FileInputStream(const ::jxx::Ptr<FileDescriptor>& descriptor); ~FileInputStream()override; ::jxx::lang::jint read()override; ::jxx::lang::jint read(const ::jxx::lang::ByteArray& b,::jxx::lang::jint o,::jxx::lang::jint l)override; ::jxx::lang::jlong skip(::jxx::lang::jlong n)override; ::jxx::lang::jint available()override; void close()override; ::jxx::Ptr<FileDescriptor> getFD()const; private: std::FILE* handle_=nullptr;::jxx::Ptr<FileDescriptor> descriptor_;::jxx::lang::jbool owned_=false; }; }
