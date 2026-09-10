#pragma once
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::lang { class String; }
namespace jxx::io { class ModifiedUTF final { public: static ::jxx::lang::ByteArray encode(const ::jxx::Ptr<::jxx::lang::String>& value); static ::jxx::Ptr<::jxx::lang::String> decode(const ::jxx::lang::ByteArray& bytes); private: ModifiedUTF()=delete; }; }
