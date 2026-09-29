#include "ext/net/ssl/jxx.ext.net.ssl.KeyStoreBuilderParameters.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::ext::net::ssl
{
	KeyStoreBuilderParameters::KeyStoreBuilderParameters(const ::jxx::Ptr<::jxx::security::KeyStore::Builder>& b)
	{
		if (b == nullptr)throw ::jxx::lang::NullPointerException(); builders_ = ::jxx::NEW<BuilderArray>(1); (*builders_)[0] = b;
	}
	KeyStoreBuilderParameters::KeyStoreBuilderParameters(const ::jxx::Ptr<BuilderArray>& b)
	{
		if (b == nullptr)throw ::jxx::lang::NullPointerException(); if (b->length == 0)throw ::jxx::lang::IllegalArgumentException(); builders_ = ::jxx::NEW<BuilderArray>(b->length); for (::jxx::lang::jint i = 0; i < b->length; ++i) {
			if ((*b)[i] == nullptr)throw ::jxx::lang::NullPointerException(); (*builders_)[i] = (*b)[i];
		}
	}
	::jxx::Ptr<KeyStoreBuilderParameters::BuilderArray> KeyStoreBuilderParameters::getParameters()const
	{
		const auto c = ::jxx::NEW<BuilderArray>(builders_->length); for (::jxx::lang::jint i = 0; i < c->length; ++i)(*c)[i] = (*builders_)[i]; return c;
	}
}
