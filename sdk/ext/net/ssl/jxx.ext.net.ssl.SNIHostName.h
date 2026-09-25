#pragma once
#include "ext/net/ssl/jxx.ext.net.ssl.SNIServerName.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::net::ssl {class SNIHostName final:public ::jxx::lang::ClassBase<SNIHostName,SNIServerName>{public:using JxxSuper=SNIServerName;using Super=::jxx::lang::ClassBase<SNIHostName,JxxSuper>;explicit SNIHostName(const ::jxx::Ptr<::jxx::lang::String>& hostname);explicit SNIHostName(const ::jxx::lang::ByteArray& encoded);::jxx::Ptr<::jxx::lang::String> getAsciiName()const;private: ::jxx::Ptr<::jxx::lang::String> hostname_;};}
