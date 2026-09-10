#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Exception.h"

namespace jxx::ext::xml::parsers {

class ParserConfigurationException
    : public ::jxx::lang::Exception {
public:
    using JxxSuper = ::jxx::lang::Exception;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<ParserConfigurationException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    ParserConfigurationException();
    explicit ParserConfigurationException(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    ParserConfigurationException(const ParserConfigurationException&) = default;
    ParserConfigurationException(ParserConfigurationException&&) noexcept = default;
    ParserConfigurationException& operator=(const ParserConfigurationException&) = default;
    ParserConfigurationException& operator=(ParserConfigurationException&&) noexcept = default;
    ~ParserConfigurationException() override;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::ext::xml::parsers
