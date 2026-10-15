#pragma once
#include "lang/jxx_types.h"
namespace jxx::lang { class String; class ClassAny; class EnumValue { public: virtual ~EnumValue() = default; virtual jxx::Ptr<String> enumName() const = 0; virtual jxx::Ptr<ClassAny> enumDeclaringClass() const = 0; }; }
