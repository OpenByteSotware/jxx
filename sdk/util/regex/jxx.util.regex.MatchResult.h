#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"


namespace jxx {
namespace util {
namespace regex {

class MatchResult : public virtual jxx::lang::Object {
public:
    virtual ~MatchResult();

    virtual jxx::lang::jint start() = 0;
    virtual jxx::lang::jint start(jxx::lang::jint group) = 0;
    virtual jxx::lang::jint end() = 0;
    virtual jxx::lang::jint end(jxx::lang::jint group) = 0;
    virtual jxx::Ptr<jxx::lang::String> group() = 0;
    virtual jxx::Ptr<jxx::lang::String> group(jxx::lang::jint group) = 0;
    virtual jxx::lang::jint groupCount() = 0;
};

} // namespace regex
} // namespace util
} // namespace jxx
