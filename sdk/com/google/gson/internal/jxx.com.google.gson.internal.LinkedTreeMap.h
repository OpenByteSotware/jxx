#pragma once

#include <type_traits>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.Comparator.h"
#include "util/jxx.util.LinkedHashMap.h"

namespace com::google::gson::internal {

/**
 * JXX C++17 representation of Gson 2.8.6 internal LinkedTreeMap<K, V>.
 *
 * Gson uses LinkedTreeMap for JSON object materialization. This JXX version
 * preserves insertion-order iteration by building on JXX LinkedHashMap and
 * preserves the Gson-visible null-key restriction. The optional comparator is
 * retained for Java API compatibility and future tree-index optimization.
 *
 * For Gson's normal JSON-object path, K is jxx::lang::String. String equality
 * and natural comparison agree, so lookup behavior matches the required path.
 */
template <typename K, typename V>
class LinkedTreeMap
    : public jxx::lang::ClassBase<LinkedTreeMap<K, V>, jxx::util::LinkedHashMap<K, V>> {
private:
    using Super =
        jxx::lang::ClassBase<
            LinkedTreeMap<K, V>,
            jxx::util::LinkedHashMap<K, V>>;

public:
    using KeyType = K;
    using ValueType = V;

    LinkedTreeMap()
        : Super() {
    }

    explicit LinkedTreeMap(
        const jxx::Ptr<jxx::util::Comparator<K>>& comparator)
        : Super()
        , comparator_(comparator) {
    }

    ~LinkedTreeMap() override = default;

    jxx::Ptr<V> put(const jxx::Ptr<K>& key, const jxx::Ptr<V>& value) override {

        if (key == nullptr) {
            throw jxx::lang::NullPointerException(
                jxx::NEW<jxx::lang::String>("key == null"));
        }

        return jxx::util::LinkedHashMap<K, V>::put(
            key,
            value);
    }

    /** Comparator supplied through the Java-compatible constructor. */
    jxx::Ptr<jxx::util::Comparator<K>> comparator() const {
        return comparator_;
    }

protected:
    JXX_OBJECT_CLONE(LinkedTreeMap<K, V>)

private:
    jxx::Ptr<jxx::util::Comparator<K>> comparator_;
};

} // namespace com::google::gson::internal
