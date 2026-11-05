#pragma once
#include "util/zip/jxx.util.zip.ZipException.h"
namespace jxx::util::jar { class JarException : public ::jxx::lang::ClassBase<JarException,::jxx::util::zip::ZipException>{public:using JxxSuper=::jxx::util::zip::ZipException;using Super=::jxx::lang::ClassBase<JarException,JxxSuper>;JarException();explicit JarException(const ::jxx::Ptr<::jxx::lang::String>&);explicit JarException(const char*);protected: ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;};}
