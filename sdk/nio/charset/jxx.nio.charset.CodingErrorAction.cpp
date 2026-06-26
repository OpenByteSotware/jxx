#include "nio/charset/jxx.nio.charset.CodingErrorAction.h"

namespace jxx::nio::charset
{
    jxx::Ptr<CodingErrorAction> CodingErrorAction::IGNORE = jxx::NEW<CodingErrorAction>(jxx::NEW<jxx::lang::String>("IGNORE"));
    jxx::Ptr<CodingErrorAction> CodingErrorAction::REPLACE = jxx::NEW<CodingErrorAction>(jxx::NEW<jxx::lang::String>("REPLACE"));
    jxx::Ptr<CodingErrorAction> CodingErrorAction::REPORT = jxx::NEW<CodingErrorAction>(jxx::NEW<jxx::lang::String>("REPORT"));

    CodingErrorAction::CodingErrorAction(jxx::Ptr<jxx::lang::String> name) : name_(std::move(name)) {}
    jxx::Ptr<jxx::lang::String> CodingErrorAction::toString() const { return name_; }
    jxx::lang::jbool CodingErrorAction::equals(jxx::Ptr<jxx::lang::Object> other) const
    {
        auto o = std::dynamic_pointer_cast<CodingErrorAction>(other);
        return o && name_ && o->name_ && name_->equals(o->name_);
    }
    jxx::lang::jint CodingErrorAction::hashCode() const { return name_ ? name_->hashCode() : 0; }
}
