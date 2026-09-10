#pragma once
#include <cstdio>
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::io {
class FileDescriptor final : public ::jxx::lang::ClassBase<FileDescriptor,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<FileDescriptor,JxxSuper>;
 static ::jxx::Ptr<FileDescriptor> in; static ::jxx::Ptr<FileDescriptor> out; static ::jxx::Ptr<FileDescriptor> err;
 FileDescriptor(); explicit FileDescriptor(std::FILE* handle,::jxx::lang::jbool owned=false); ~FileDescriptor() override;
 ::jxx::lang::jbool valid() const; void sync();
 std::FILE* nativeHandle() const;
private: std::FILE* handle_=nullptr; ::jxx::lang::jbool owned_=false;
};
} // namespace jxx::io
