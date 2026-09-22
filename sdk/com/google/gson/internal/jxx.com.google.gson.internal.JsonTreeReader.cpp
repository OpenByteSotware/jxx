#include "com/google/gson/internal/jxx.com.google.gson.internal.JsonTreeReader.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
namespace com::google::gson::internal {
JsonTreeReader::JsonTreeReader(const ::jxx::Ptr<::com::google::gson::JsonElement>& e):element_(e==nullptr?::com::google::gson::JsonNull::INSTANCE():e){}
::jxx::Ptr<::com::google::gson::JsonElement> JsonTreeReader::get() const{return element_;}
}
