#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "util/logging/jxx.util.logging.Formatter.h"
namespace jxx::util::logging
{
	class SimpleFormatter : public Formatter
	{
	public: jxx::Ptr<jxx::lang::String> format(const jxx::Ptr<LogRecord>& record) override; 
    using Super = Formatter;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<SimpleFormatter, Formatter>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return JxxClassInfoMarker::Class();
    }

		  JXX_OBJECT_CLONE(SimpleFormatter)

	};
}
