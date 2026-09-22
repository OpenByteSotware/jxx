#include "com/google/gson/jxx.com.google.gson.ToNumberPolicy.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace com::google::gson { namespace {
class Strategy final : public ::jxx::lang::Object, public ToNumberStrategy {
public:
 explicit Strategy(ToNumberPolicy policy):policy_(policy){}
 ::jxx::Ptr<JsonPrimitive> readNumber(const ::jxx::Ptr<::jxx::lang::String>& lexical) override {
  if(lexical==nullptr) throw ::jxx::lang::NullPointerException();
  const auto text=lexical->utf8();
  if(policy_==ToNumberPolicy::DOUBLE) return ::jxx::NEW<JsonPrimitive>(static_cast<::jxx::lang::jdouble>(std::stod(text)));
  if(policy_==ToNumberPolicy::LONG_OR_DOUBLE && text.find_first_of(".eE")==std::string::npos) return ::jxx::NEW<JsonPrimitive>(static_cast<::jxx::lang::jlong>(std::stoll(text)));
  if(policy_==ToNumberPolicy::BIG_DECIMAL) return ::jxx::NEW<JsonPrimitive>(lexical);
  return ::jxx::NEW<JsonPrimitive>(static_cast<::jxx::lang::jdouble>(std::stod(text)));
 }
private: ToNumberPolicy policy_;
}; }
::jxx::Ptr<ToNumberStrategy> ToNumberPolicies::strategy(ToNumberPolicy policy){return std::make_shared<Strategy>(policy);}
} // namespace com::google::gson
