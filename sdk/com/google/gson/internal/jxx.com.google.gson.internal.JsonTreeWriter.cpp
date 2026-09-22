#include "com/google/gson/internal/jxx.com.google.gson.internal.JsonTreeWriter.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
namespace com::google::gson::internal {
JsonTreeWriter::JsonTreeWriter():element_(::com::google::gson::JsonNull::INSTANCE()){}
void JsonTreeWriter::set(const ::jxx::Ptr<::com::google::gson::JsonElement>& e){element_=e==nullptr?::com::google::gson::JsonNull::INSTANCE():e;}
::jxx::Ptr<::com::google::gson::JsonElement> JsonTreeWriter::get() const{return element_;}
}
