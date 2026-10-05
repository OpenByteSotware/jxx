#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::lang { class String; }
namespace jxx::nio::file::attribute {
class FileTime final : public ::jxx::lang::ClassBase<FileTime,::jxx::lang::Object,::jxx::lang::Comparable<FileTime>> {
public:
 using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<FileTime,JxxSuper,::jxx::lang::Comparable<FileTime>>;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;
 explicit FileTime(::jxx::lang::jlong value)noexcept;
 static ::jxx::Ptr<FileTime> fromMillis(::jxx::lang::jlong value);
 ::jxx::lang::jlong toMillis()const noexcept;
 ::jxx::lang::jint compareTo(const ::jxx::Ptr<FileTime>& other)const override;
 ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& other)const override;
 ::jxx::lang::jint hashCode()const override;
 ::jxx::Ptr<::jxx::lang::String> toString()const override;
private: ::jxx::lang::jlong millis_;
};}
