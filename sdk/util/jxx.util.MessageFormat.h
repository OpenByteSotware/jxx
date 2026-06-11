#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang {
class String;
}

namespace jxx::util {

class MessageFormat : public jxx::lang::Object {
private:
    jxx::Ptr<jxx::lang::String> pattern_;

public:
    explicit MessageFormat(const jxx::Ptr<jxx::lang::String>& pattern);

    static jxx::Ptr<MessageFormat> of(const jxx::Ptr<jxx::lang::String>& pattern);
    jxx::Ptr<jxx::lang::String> format(jxx::Ptr<jxx::lang::Object>* args, jxx::lang::jint count) const;
};

} // namespace jxx::util
