#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Object.h"
#include "util/jar/jxx.util.jar.Attributes.h"
#include "util/jxx.util.HashMap.h"
namespace jxx::io {class InputStream;class OutputStream;}
namespace jxx::util::jar {class Manifest final:public ::jxx::lang::ClassBase<Manifest,::jxx::lang::Object,::jxx::lang::Cloneable>{public:Manifest();Manifest(const ::jxx::Ptr<Manifest>&);explicit Manifest(const ::jxx::Ptr<::jxx::io::InputStream>&);::jxx::Ptr<Attributes> getMainAttributes()const;::jxx::Ptr<::jxx::util::Map<::jxx::lang::String,Attributes>> getEntries()const;::jxx::Ptr<Attributes> getAttributes(const ::jxx::Ptr<::jxx::lang::String>&)const;void clear();void write(const ::jxx::Ptr<::jxx::io::OutputStream>&)const;void read(const ::jxx::Ptr<::jxx::io::InputStream>&);::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>&)const override;::jxx::lang::jint hashCode()const override;private: ::jxx::Ptr<Attributes> main_;::jxx::Ptr<::jxx::util::HashMap<::jxx::lang::String,Attributes>> entries_;};}
