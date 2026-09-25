#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::transform {
class OutputKeys final : public ::jxx::lang::ClassBase<OutputKeys, ::jxx::lang::Object> {
public:
    static ::jxx::Ptr<::jxx::lang::String> METHOD();
    static ::jxx::Ptr<::jxx::lang::String> VERSION();
    static ::jxx::Ptr<::jxx::lang::String> ENCODING();
    static ::jxx::Ptr<::jxx::lang::String> OMIT_XML_DECLARATION();
    static ::jxx::Ptr<::jxx::lang::String> STANDALONE();
    static ::jxx::Ptr<::jxx::lang::String> DOCTYPE_PUBLIC();
    static ::jxx::Ptr<::jxx::lang::String> DOCTYPE_SYSTEM();
    static ::jxx::Ptr<::jxx::lang::String> CDATA_SECTION_ELEMENTS();
    static ::jxx::Ptr<::jxx::lang::String> INDENT();
    static ::jxx::Ptr<::jxx::lang::String> MEDIA_TYPE();
private:
    OutputKeys() = delete;
};
}
