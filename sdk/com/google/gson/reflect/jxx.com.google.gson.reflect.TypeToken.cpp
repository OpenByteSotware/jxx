#include "com/google/gson/reflect/jxx.com.google.gson.reflect.TypeToken.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace com::google::gson::reflect {

TypeToken::TypeToken(
    const ::jxx::Ptr<::jxx::lang::ClassAny>& rawType)
    : rawType_(rawType) {
    if (rawType_ == nullptr) throw ::jxx::lang::NullPointerException();
}

::jxx::Ptr<TypeToken> TypeToken::get(
    const ::jxx::Ptr<::jxx::lang::ClassAny>& type) {
    return ::jxx::NEW<TypeToken>(type);
}

::jxx::Ptr<::jxx::lang::ClassAny> TypeToken::getRawType() const { return rawType_; }
::jxx::Ptr<::jxx::lang::ClassAny> TypeToken::getType() const { return rawType_; }

::jxx::lang::jbool TypeToken::equals(
    const ::jxx::Ptr<::jxx::lang::Object>& other) const {
    const auto token = std::dynamic_pointer_cast<TypeToken>(other);
    return token != nullptr && token->rawType_ == rawType_;
}

::jxx::lang::jint TypeToken::hashCode() const {
    return rawType_->hashCode();
}

::jxx::Ptr<::jxx::lang::String> TypeToken::toString() const {
    return rawType_->getName();
}

} // namespace com::google::gson::reflect
