#include "com/google/gson/jxx.com.google.gson.TypeAdapter.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.NullSafeTypeAdapter.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.Streams.h"
#include "io/jxx.io.StringReader.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.StringWriter.h"
namespace com::google::gson {
::jxx::Ptr<::jxx::lang::Object> TypeAdapter::read(const ::jxx::Ptr<::com::google::gson::stream::JsonReader>& reader){if(reader==nullptr)throw ::jxx::lang::NullPointerException();return fromJsonTree(::com::google::gson::internal::Streams::parse(reader));}
void TypeAdapter::write(const ::jxx::Ptr<::com::google::gson::stream::JsonWriter>& writer,const ::jxx::Ptr<::jxx::lang::Object>& value){if(writer==nullptr)throw ::jxx::lang::NullPointerException();::com::google::gson::internal::Streams::write(toJsonTree(value),writer);}
::jxx::Ptr<::jxx::lang::Object> TypeAdapter::fromJson(const ::jxx::Ptr<::jxx::lang::String>& json){if(json==nullptr)throw ::jxx::lang::NullPointerException();const auto reader=::jxx::NEW<::com::google::gson::stream::JsonReader>(::jxx::NEW<::jxx::io::StringReader>(json));return read(reader);}
::jxx::Ptr<::jxx::lang::String> TypeAdapter::toJson(const ::jxx::Ptr<::jxx::lang::Object>& value){const auto writer=::jxx::NEW<::com::google::gson::internal::StringWriter>();const auto jsonWriter=::jxx::NEW<::com::google::gson::stream::JsonWriter>(::jxx::CAST<::jxx::io::Writer>(writer));write(jsonWriter,value);jsonWriter->flush();return writer->toString();}
::jxx::Ptr<TypeAdapter> TypeAdapter::nullSafe(
    const ::jxx::Ptr<TypeAdapter>& delegate) {
    if (delegate == nullptr) throw ::jxx::lang::NullPointerException();
    return std::make_shared<::com::google::gson::internal::NullSafeTypeAdapter>(delegate);
}
} // namespace com::google::gson
