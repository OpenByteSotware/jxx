#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.SortedMap.h"
namespace jxx::io {class InputStream;class OutputStream;class File;}
namespace jxx::util::jar {class JarFile;class JarOutputStream;class Pack200 final:public ::jxx::lang::ClassBase<Pack200,::jxx::lang::Object>{public:class Packer:public ::jxx::lang::InterfaceBase<Packer>{public:virtual ::jxx::Ptr<::jxx::util::SortedMap<::jxx::lang::String,::jxx::lang::String>> properties()=0;virtual void pack(const ::jxx::Ptr<JarFile>&,const ::jxx::Ptr<::jxx::io::OutputStream>&)=0;virtual void pack(const ::jxx::Ptr<::jxx::io::InputStream>&,const ::jxx::Ptr<::jxx::io::OutputStream>&)=0;};class Unpacker:public ::jxx::lang::InterfaceBase<Unpacker>{public:virtual ::jxx::Ptr<::jxx::util::SortedMap<::jxx::lang::String,::jxx::lang::String>> properties()=0;virtual void unpack(const ::jxx::Ptr<::jxx::io::InputStream>&,const ::jxx::Ptr<JarOutputStream>&)=0;virtual void unpack(const ::jxx::Ptr<::jxx::io::File>&,const ::jxx::Ptr<JarOutputStream>&)=0;};static ::jxx::Ptr<Packer> newPacker();static ::jxx::Ptr<Unpacker> newUnpacker();private:Pack200()=delete;};}
