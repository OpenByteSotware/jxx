#pragma once

#include <type_traits>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.Comparator.h"
#include "util/jxx.util.LinkedHashMap.h"

namespace com::google::gson::internal {

/**
 * JXX/C++17 representation of Gson 2.8.6 LinkedTreeMap<K,V>.
 *
 * The implementation preserves the behavior needed by Gson object
 * materialization: null keys are rejected, values may be null, lookup uses the
 * inherited map semantics, and iteration retains LinkedHashMap insertion order.
 *
 * Java reference parameters use const jxx::Ptr<T>&.
 */
template <typename K, typename V>
class LinkedTreeMap
    : public jxx::lang::ClassBase<
          LinkedTreeMap<K, V>,
          jxx::util::LinkedHashMap<K, V>> {
private:
    using JavaSuper = jxx::util::LinkedHashMap<K, V>;
    using Super = jxx::lang::ClassBase<LinkedTreeMap<K, V>, JavaSuper>;

public:
    using KeyType = K;
    using ValueType = V;

    LinkedTreeMap()
        : Super()
        , comparator_(nullptr) {
    }

    explicit LinkedTreeMap(
        const jxx::Ptr<jxx::util::Comparator<K>>& comparator)
        : Super()
        , comparator_(comparator) {
    }

    ~LinkedTreeMap() override = default;

    jxx::Ptr<V> put(
        const jxx::Ptr<K>& key,
        const jxx::Ptr<V>& value) override {

        if (key == nullptr) {
            throw jxx::lang::NullPointerException();
        }

        return JavaSuper::put(key, value);
    }

    jxx::Ptr<jxx::util::Comparator<K>> comparator() const {
        return comparator_;
    }

protected:
    /** Java-style shallow clone with independent map structure. */
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override {
        auto result = jxx::NEW<LinkedTreeMap<K, V>>(comparator_);

        auto self = jxx::CAST<LinkedTreeMap<K, V>>(this->thisPtr);
        if (self == nullptr) {
            throw jxx::lang::IllegalStateException();
        }

        auto entries = self->entrySet();
        if (entries != nullptr) {
            auto iterator = entries->iterator();
            while (iterator->hasNext()) {
                const auto entry = iterator->next();
                if (entry != nullptr) {
                    result->put(entry->getKey(), entry->getValue());
                }
            }
        }

        return jxx::CAST<jxx::lang::Object>(result);
    }

private:
    jxx::Ptr<jxx::util::Comparator<K>> comparator_;
};

} // namespace com::google::gson::internal
