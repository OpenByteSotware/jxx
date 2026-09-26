#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.List.h"
namespace jxx::com::sun::net::httpserver { class Headers final:public ::jxx::util::HashMap<::jxx::lang::String,::jxx::util::List<::jxx::lang::String>>{public:using JxxSuper=::jxx::util::HashMap<::jxx::lang::String,::jxx::util::List<::jxx::lang::String>>;using Super=::jxx::lang::ClassBase<Headers,JxxSuper>;Headers();void add(const ::jxx::Ptr<::jxx::lang::String>& key,const ::jxx::Ptr<::jxx::lang::String>& value);void set(const ::jxx::Ptr<::jxx::lang::String>& key,const ::jxx::Ptr<::jxx::lang::String>& value);::jxx::Ptr<::jxx::lang::String> getFirst(const ::jxx::Ptr<::jxx::lang::String>& key);}; }
