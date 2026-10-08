#pragma once

#include "text/jxx.text.Format.h"

#include "lang/jxx_types.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang {
class String;
}

namespace jxx::text {

class MessageFormat : public Format {
public:
    using JxxSuper = Format;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<MessageFormat, JxxSuper>;
    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

private:
    ::jxx::Ptr<::jxx::lang::String> pattern_;

public:
    explicit MessageFormat(const ::jxx::Ptr<::jxx::lang::String>& pattern);

    static ::jxx::Ptr<MessageFormat> of(const ::jxx::Ptr<::jxx::lang::String>& pattern);
    ::jxx::Ptr<::jxx::lang::String> format(const ::jxx::Ptr<::jxx::lang::Object>* args, ::jxx::lang::jint count) const;
};

} // namespace jxx::text
