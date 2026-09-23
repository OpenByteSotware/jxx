#include "com/google/gson/internal/jxx.com.google.gson.internal.TreeTypeAdapter.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace com::google::gson::internal
{
	TreeTypeAdapter::TreeTypeAdapter(const ::jxx::Ptr<::com::google::gson::JsonSerializer>& s, const ::jxx::Ptr<::com::google::gson::JsonDeserializer>& d, const ::jxx::Ptr<::jxx::lang::ClassAny>& t, const ::jxx::Ptr<::com::google::gson::JsonSerializationContext>& sc, const ::jxx::Ptr<::com::google::gson::JsonDeserializationContext>& dc) :serializer_(s), deserializer_(d), type_(t), serializationContext_(sc), deserializationContext_(dc)
	{
		if (type_ == nullptr)throw ::jxx::lang::NullPointerException();
	}
	::jxx::Ptr<::jxx::lang::Object> TreeTypeAdapter::fromJsonTree(const ::jxx::Ptr<::com::google::gson::JsonElement>& e)
	{
		return deserializer_ == nullptr ? nullptr : deserializer_->deserialize(e, type_, deserializationContext_);
	}
	::jxx::Ptr<::com::google::gson::JsonElement> TreeTypeAdapter::toJsonTree(const ::jxx::Ptr<::jxx::lang::Object>& v)
	{
		return serializer_ == nullptr ? nullptr : serializer_->serialize(v, type_, serializationContext_);
	}
}
