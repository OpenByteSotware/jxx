#pragma once
#include "lang/jxx.lang.h"
#include "lang/jxx.lang.ArrayStoreException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Iterable.h"
#include "util/jxx.util.wildcard.CollectionAny.h"
#include "util/jxx.util.wildcard.CollectionExtends.h"
namespace jxx::util {
template <typename E>
class Collection : public jxx::lang::InterfaceBase<Collection<E>, jxx::lang::Iterable<E>> {
public:
    virtual ~Collection() = default;
    virtual jxx::lang::jint size() = 0;
    virtual jxx::lang::jbool isEmpty() = 0;
    virtual jxx::lang::jbool contains(const jxx::Ptr<jxx::lang::Object>& object) = 0;
    virtual jxx::Ptr<Iterator<E>> iterator() = 0;
    virtual jxx::lang::ObjectArray toArray() = 0;

    template <typename T>
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<T>, 1U>> toArray(
        const ::jxx::Ptr<
            ::jxx::lang::JxxArray<::jxx::Ptr<T>, 1U>>& array) {
        if (array == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }

        const auto collectionSize = size();
        auto result = array;
        if (array->length < collectionSize) {
            result = ::jxx::NEW<
                ::jxx::lang::JxxArray<::jxx::Ptr<T>, 1U>>(
                    collectionSize);
        }

        auto values = iterator();
        auto index = static_cast<::jxx::lang::jint>(0);
        while (values->hasNext()) {
            const auto element = values->next();
            const auto converted = ::jxx::CAST<T>(element);
            if (element != nullptr && converted == nullptr) {
                throw ::jxx::lang::ArrayStoreException();
            }
            (*result)[index++] = converted;
        }

        if (result->length > collectionSize) {
            (*result)[collectionSize] = nullptr;
        }
        return result;
    }
    virtual jxx::lang::jbool add(const jxx::Ptr<E>& element) = 0;
    virtual jxx::lang::jbool remove(const jxx::Ptr<jxx::lang::Object>& object) = 0;
    virtual jxx::lang::jbool containsAll(const jxx::Ptr<wildcard::CollectionAny>& collection) = 0;
    virtual jxx::lang::jbool addAll(const jxx::Ptr<wildcard::CollectionExtends<E>>& collection) = 0;
    virtual jxx::lang::jbool removeAll(const jxx::Ptr<wildcard::CollectionAny>& collection) = 0;
    virtual jxx::lang::jbool retainAll(const jxx::Ptr<wildcard::CollectionAny>& collection) = 0;
    virtual void clear() = 0;
};
} // namespace jxx::util
