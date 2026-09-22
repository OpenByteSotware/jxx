#include "com/google/gson/internal/jxx.com.google.gson.internal.GsonContext.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace com::google::gson::internal {
GsonContext::GsonContext(const ::jxx::Ptr<::com::google::gson::Gson>& gson):gson_(gson){if(gson_==nullptr)throw ::jxx::lang::NullPointerException();}
::jxx::Ptr<::com::google::gson::JsonElement> GsonContext::serialize(const ::jxx::Ptr<::jxx::lang::Object>& source){return source==nullptr?::com::google::gson::JsonNull::INSTANCE():nullptr;}
::jxx::Ptr<::com::google::gson::JsonElement> GsonContext::serialize(const ::jxx::Ptr<::jxx::lang::Object>& source,const ::jxx::Ptr<::jxx::lang::ClassAny>& type){if(type==nullptr)throw ::jxx::lang::NullPointerException();const auto adapter=gson_->getAdapter(type);return adapter==nullptr?nullptr:adapter->toJsonTree(source);}
::jxx::Ptr<::jxx::lang::Object> GsonContext::deserialize(const ::jxx::Ptr<::com::google::gson::JsonElement>& json,const ::jxx::Ptr<::jxx::lang::ClassAny>& type){if(type==nullptr)throw ::jxx::lang::NullPointerException();const auto adapter=gson_->getAdapter(type);return adapter==nullptr?nullptr:adapter->fromJsonTree(json);}
} // namespace com::google::gson::internal
