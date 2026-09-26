#pragma once
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::ext::net::ssl
{
	class SNIServerName :public ::jxx::lang::ClassBase<SNIServerName, ::jxx::lang::Object>
	{
	public:using JxxSuper = ::jxx::lang::Object;
	using Super = ::jxx::lang::ClassBase<SNIServerName, JxxSuper>; 
	virtual ~SNIServerName()override = default; ::jxx::lang::jint getType()const noexcept; 
	::jxx::lang::ByteArray getEncoded()const; 
	protected:
	SNIServerName(::jxx::lang::jint type, const ::jxx::lang::ByteArray& encoded); 
	private:
		::jxx::lang::jint type_; ::jxx::lang::ByteArray encoded_;
	};
}
