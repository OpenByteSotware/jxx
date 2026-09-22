#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace com::google::gson {
namespace { std::string quoted(const std::string& in){ std::string out="\""; for(char c:in){ if(c=='\"'||c=='\\') out.push_back('\\'); if(c=='\n'){out+="\\n";continue;} out.push_back(c);} return out+"\""; } }
JsonPrimitive::JsonPrimitive(const ::jxx::Ptr<::jxx::lang::String>& value):kind_(Kind::STRING),value_(value){if(!value_)throw ::jxx::lang::NullPointerException();}
JsonPrimitive::JsonPrimitive(::jxx::lang::jbool value):kind_(Kind::BOOLEAN),value_(::jxx::lang::Boolean::toString(value)){}
JsonPrimitive::JsonPrimitive(::jxx::lang::jlong value):kind_(Kind::INTEGER),value_(::jxx::lang::Long::toString(value)){}
JsonPrimitive::JsonPrimitive(::jxx::lang::jdouble value):kind_(Kind::DECIMAL),value_(::jxx::lang::Double::toString(value)){}
::jxx::lang::jbool JsonPrimitive::isString() const noexcept{return kind_==Kind::STRING;}
::jxx::lang::jbool JsonPrimitive::isBoolean() const noexcept{return kind_==Kind::BOOLEAN;}
::jxx::lang::jbool JsonPrimitive::isNumber() const noexcept{return kind_==Kind::INTEGER||kind_==Kind::DECIMAL;}
::jxx::Ptr<::jxx::lang::String> JsonPrimitive::getAsString(){return value_;}
::jxx::lang::jbool JsonPrimitive::getAsBoolean(){return ::jxx::lang::Boolean::parseBoolean(value_);}
::jxx::lang::jint JsonPrimitive::getAsInt(){return static_cast<::jxx::lang::jint>(::jxx::lang::Long::parseLong(value_));}
::jxx::lang::jlong JsonPrimitive::getAsLong(){return ::jxx::lang::Long::parseLong(value_);}
::jxx::lang::jdouble JsonPrimitive::getAsDouble(){return ::jxx::lang::Double::parseDouble(value_);}
::jxx::Ptr<::jxx::lang::String> JsonPrimitive::toString() const{return kind_==Kind::STRING?::jxx::NEW<::jxx::lang::String>(quoted(value_->utf8())):value_;}
::jxx::lang::jbool JsonPrimitive::equals(
    const ::jxx::Ptr<::jxx::lang::Object>& other) const {
    const auto primitive = std::dynamic_pointer_cast<JsonPrimitive>(other);
    if (primitive == nullptr) return false;
    if (isNumber() && primitive->isNumber()) return ::jxx::lang::Double::parseDouble(value_) == primitive->getAsDouble();
    return kind_ == primitive->kind_ && value_->equals(primitive->value_);
}

::jxx::lang::jint JsonPrimitive::hashCode() const {
    if (isNumber()) {
        const auto bits = static_cast<unsigned long long>(std::hash<double>{}(::jxx::lang::Double::parseDouble(value_)));
        return static_cast<::jxx::lang::jint>(bits ^ (bits >> 32U));
    }
    return value_ == nullptr ? 0 : value_->hashCode();
}

} // namespace com::google::gson
