#pragma once
#include "lang/jxx_types.h"
namespace jxx::lang { class ClassAny; namespace class_info_detail {
jxx::Ptr<ClassAny> ensureObjectRegistered();
jxx::Ptr<ClassAny> ensureSerializableRegistered();
jxx::Ptr<ClassAny> ensureCloneableRegistered();
jxx::Ptr<ClassAny> ensureCharSequenceRegistered();
jxx::Ptr<ClassAny> ensureComparableRegistered();
jxx::Ptr<ClassAny> ensureStringRegistered();
} }
