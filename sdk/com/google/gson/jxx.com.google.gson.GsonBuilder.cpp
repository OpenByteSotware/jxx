#include "com/google/gson/jxx.com.google.gson.GsonBuilder.h"
#include "com/google/gson/jxx.com.google.gson.Gson.h"

namespace com::google::gson {

::jxx::Ptr<GsonBuilder> GsonBuilder::serializeNulls() {
    serializeNulls_ = true;
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<GsonBuilder> GsonBuilder::disableHtmlEscaping() {
    htmlSafe_ = false;
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<GsonBuilder> GsonBuilder::setPrettyPrinting() {
    prettyPrinting_ = true;
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<GsonBuilder> GsonBuilder::setLenient() {
    lenient_ = true;
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<Gson> GsonBuilder::create() const {
    return ::jxx::NEW<Gson>(
        serializeNulls_, htmlSafe_, prettyPrinting_, lenient_);
}

} // namespace com::google::gson
