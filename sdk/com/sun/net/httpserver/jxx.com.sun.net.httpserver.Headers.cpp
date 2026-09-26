#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Headers.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.ArrayList.h"
namespace jxx::com::sun::net::httpserver
{
	namespace
	{
		::jxx::Ptr<::jxx::lang::String> normalized(const ::jxx::Ptr<::jxx::lang::String>& key)
		{
			if (!key)throw ::jxx::lang::NullPointerException(); auto s = key->utf8(); for (auto& c : s)if (c >= 'A' && c <= 'Z')c = (char)(c - 'A' + 'a'); return ::jxx::NEW<::jxx::lang::String>(s.c_str());
		}
	}
	Headers::Headers() :JxxSuper()
	{
	}
	void Headers::add(const ::jxx::Ptr<::jxx::lang::String>& key, const ::jxx::Ptr<::jxx::lang::String>& value)
	{
		if (!value)
			throw ::jxx::lang::NullPointerException(); 
		auto k = normalized(key); auto list = this->get(k);
		if (!list) {
			list = ::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::String>>(); 
			this->put(k, list);
		}
		list->add(value);
	}
	void Headers::set(const ::jxx::Ptr<::jxx::lang::String>& key, const ::jxx::Ptr<::jxx::lang::String>& value)
	{
		if (!value)throw ::jxx::lang::NullPointerException(); auto list = ::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::String>>(); list->add(value); this->put(normalized(key), list);
	}
	::jxx::Ptr<::jxx::lang::String> Headers::getFirst(const ::jxx::Ptr<::jxx::lang::String>& key)
	{
		auto list = this->get(normalized(key)); return(!list || list->isEmpty()) ? nullptr : list->get(0);
	}
}
