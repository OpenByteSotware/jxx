#pragma once

#include <cstddef>
#include <string>

#include "io/jxx.io.Reader.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Iterator.h"

namespace com::google::gson {
class JsonElement;

class JsonStreamParser final
    : public ::jxx::lang::ClassBase<
          JsonStreamParser,
          ::jxx::lang::Object,
          ::jxx::util::Iterator<JsonElement>> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        JsonStreamParser,
        JxxSuper,
        ::jxx::util::Iterator<JsonElement>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    explicit JsonStreamParser(
        const ::jxx::Ptr<::jxx::lang::String>& json);
    explicit JsonStreamParser(
        const ::jxx::Ptr<::jxx::io::Reader>& reader);

    ::jxx::lang::jbool hasNext() override;
    ::jxx::Ptr<JsonElement> next() override;
    void remove() override;

private:
    void load_();
    void skipWhitespace_();
    std::string nextValue_();

    ::jxx::Ptr<::jxx::io::Reader> reader_;
    std::string source_;
    std::size_t position_ = 0U;
    ::jxx::lang::jbool loaded_ = false;
};

} // namespace com::google::gson
