#pragma once

#include <string>
#include <vector>

#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::util {

class StringJoiner final
    : public ::jxx::lang::ClassBase<StringJoiner, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<StringJoiner, JxxSuper>;

    explicit StringJoiner(
        const ::jxx::Ptr<::jxx::lang::CharSequence>& delimiter);

    StringJoiner(
        const ::jxx::Ptr<::jxx::lang::CharSequence>& delimiter,
        const ::jxx::Ptr<::jxx::lang::CharSequence>& prefix,
        const ::jxx::Ptr<::jxx::lang::CharSequence>& suffix);

    ::jxx::Ptr<StringJoiner> setEmptyValue(
        const ::jxx::Ptr<::jxx::lang::CharSequence>& emptyValue);
    ::jxx::Ptr<StringJoiner> add(
        const ::jxx::Ptr<::jxx::lang::CharSequence>& newElement);
    ::jxx::Ptr<StringJoiner> merge(const ::jxx::Ptr<StringJoiner>& other);
    ::jxx::lang::jint length() const;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

private:
    static std::string text_(
        const ::jxx::Ptr<::jxx::lang::CharSequence>& value);
    std::string joined_() const;

    std::string prefix_;
    std::string delimiter_;
    std::string suffix_;
    std::string emptyValue_;
    std::vector<std::string> elements_;
};

} // namespace jxx::util
