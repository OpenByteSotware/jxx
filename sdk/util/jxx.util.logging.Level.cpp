#include "util/jxx.util.logging.Level.h"
#include "lang/jxx.lang.Exceptions.h"
namespace jxx::util::logging
{
	namespace
	{
		jxx::Ptr<jxx::lang::String> S(const char* v)
		{
			return jxx::NEW<jxx::lang::String>(v);
		}
	}
	Level::Level(const jxx::Ptr<jxx::lang::String>& n, jxx::lang::jint v) :Level(n, v, nullptr)
	{
	} Level::Level(const jxx::Ptr<jxx::lang::String>& n, jxx::lang::jint v, const jxx::Ptr<jxx::lang::String>& b) :name_(n), value_(v), bundleName_(b)
	{
		if (n == nullptr)throw jxx::lang::NullPointerException();
	}
#define LEVEL_FN(N,V) jxx::Ptr<Level> Level::N(){static auto x=jxx::NEW<Level>(S(#N),V);return x;}
	LEVEL_FN(OFF, 2147483647) LEVEL_FN(SEVERE, 1000) LEVEL_FN(WARNING, 900) LEVEL_FN(INFO, 800) LEVEL_FN(CONFIG, 700) LEVEL_FN(FINE, 500) LEVEL_FN(FINER, 400) LEVEL_FN(FINEST, 300) LEVEL_FN(ALL, -2147483647)
#undef LEVEL_FN
		jxx::Ptr<jxx::lang::String> Level::getName()const
	{
		return name_;
	}jxx::Ptr<jxx::lang::String> Level::getLocalizedName()const
	{
		return name_;
	}jxx::Ptr<jxx::lang::String> Level::getResourceBundleName()const
	{
		return bundleName_;
	}jxx::lang::jint Level::intValue()const noexcept
	{
		return value_;
	}jxx::Ptr<jxx::lang::String> Level::toString()const
	{
		return name_;
	}
	jxx::Ptr<Level> Level::parse(const jxx::Ptr<jxx::lang::String>& n)
	{
		if (n == nullptr)throw jxx::lang::NullPointerException(); for (auto x : { OFF(),SEVERE(),WARNING(),INFO(),CONFIG(),FINE(),FINER(),FINEST(),ALL() })if (x->getName()->equals(jxx::CAST<jxx::lang::Object>(n)))return x; throw jxx::lang::IllegalArgumentException();
	}
} // namespace jxx::util::logging
