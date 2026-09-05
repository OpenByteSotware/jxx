#pragma once

#include "com/google/gson/internal/jxx.com.google.gson.internal.LinkedTreeMap.h"

namespace com::google::gson {

template <typename K, typename V>
void initializeLinkedTreeMapClass()
{
    using MapType =
        com::google::gson::internal::LinkedTreeMap<K, V>;

    (void)MapType::Class();
}

} // namespace com::google::gson
