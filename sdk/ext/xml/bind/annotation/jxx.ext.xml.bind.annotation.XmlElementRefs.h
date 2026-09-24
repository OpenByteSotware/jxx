#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::ext::xml::bind::annotation { class XmlElementRef; }
namespace jxx::ext::xml::bind::annotation {
class XmlElementRefs final
    : public ::jxx::lang::ClassBase<XmlElementRefs, ::jxx::lang::Object> {
public:
    using RefArray = ::jxx::lang::JxxArray<::jxx::Ptr<XmlElementRef>, 1U>;
    explicit XmlElementRefs(const ::jxx::Ptr<RefArray>& value);
    ::jxx::Ptr<RefArray> value() const;
private:
    ::jxx::Ptr<RefArray> value_;
};
} // namespace jxx::ext::xml::bind::annotation
