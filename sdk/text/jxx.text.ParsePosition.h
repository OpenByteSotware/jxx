#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::text {

class ParsePosition final
    : public ::jxx::lang::ClassBase<ParsePosition, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<ParsePosition, JxxSuper>;

    explicit ParsePosition(::jxx::lang::jint index);

    ::jxx::lang::jint getIndex() const noexcept;
    void setIndex(::jxx::lang::jint index) noexcept;
    ::jxx::lang::jint getErrorIndex() const noexcept;
    void setErrorIndex(::jxx::lang::jint errorIndex) noexcept;

    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override;
    ::jxx::lang::jint hashCode() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;

private:
    ::jxx::lang::jint index_;
    ::jxx::lang::jint errorIndex_ = -1;
};

} // namespace jxx::text
