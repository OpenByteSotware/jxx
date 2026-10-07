#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Exception.h"
namespace jxx::util::concurrent
{
	class TimeoutException : public jxx::lang::Exception
	{
public:
    using JxxSuper = jxx::lang::Exception;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<TimeoutException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    TimeoutException() = default;

    TimeoutException(const TimeoutException&) = default;
    TimeoutException(TimeoutException&&) noexcept = default;
    TimeoutException& operator=(const TimeoutException&) = default;
    TimeoutException& operator=(TimeoutException&&) noexcept = default;
    ~TimeoutException() override = default;

	public: using jxx::lang::Exception::Exception; protected: JXX_OBJECT_CLONE(TimeoutException)
	};
}
