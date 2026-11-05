#pragma once
#include <unordered_map>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::util::jar {
class Attributes final:public ::jxx::lang::ClassBase<Attributes,::jxx::lang::Object,::jxx::lang::Cloneable>{public:
 class Name final:public ::jxx::lang::ClassBase<Name,::jxx::lang::Object>{public:explicit Name(const ::jxx::Ptr<::jxx::lang::String>&);::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>&)const override;::jxx::lang::jint hashCode()const override;::jxx::Ptr<::jxx::lang::String> toString()const override;private: ::jxx::Ptr<::jxx::lang::String> name_;};
 Attributes();explicit Attributes(::jxx::lang::jint);Attributes(const ::jxx::Ptr<Attributes>&);::jxx::lang::jint size()const;::jxx::lang::jbool isEmpty()const;::jxx::lang::jbool containsValue(const ::jxx::Ptr<::jxx::lang::Object>&)const;::jxx::lang::jbool containsKey(const ::jxx::Ptr<::jxx::lang::Object>&)const;::jxx::Ptr<::jxx::lang::Object> get(const ::jxx::Ptr<::jxx::lang::Object>&)const;::jxx::Ptr<::jxx::lang::String> getValue(const ::jxx::Ptr<::jxx::lang::String>&)const;::jxx::Ptr<::jxx::lang::String> getValue(const ::jxx::Ptr<Name>&)const;::jxx::Ptr<::jxx::lang::Object> put(const ::jxx::Ptr<::jxx::lang::Object>&,const ::jxx::Ptr<::jxx::lang::Object>&);::jxx::Ptr<::jxx::lang::String> putValue(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<::jxx::lang::String>&);::jxx::Ptr<::jxx::lang::Object> remove(const ::jxx::Ptr<::jxx::lang::Object>&);void putAll(const ::jxx::Ptr<Attributes>&);void clear();::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>&)const override;::jxx::lang::jint hashCode()const override;
 static ::jxx::Ptr<Name> MANIFEST_VERSION,SIGNATURE_VERSION,CONTENT_TYPE,CLASS_PATH,MAIN_CLASS,SEALED,EXTENSION_LIST,EXTENSION_NAME,IMPLEMENTATION_TITLE,IMPLEMENTATION_VERSION,IMPLEMENTATION_VENDOR,IMPLEMENTATION_VENDOR_ID,IMPLEMENTATION_URL,SPECIFICATION_TITLE,SPECIFICATION_VERSION,SPECIFICATION_VENDOR;
private:
 friend class Manifest;
 std::unordered_map<std::string,::jxx::Ptr<::jxx::lang::String>> values_;};}
