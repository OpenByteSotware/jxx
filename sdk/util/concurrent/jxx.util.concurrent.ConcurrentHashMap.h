#pragma once

#include <mutex>
#include <utility>
#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentMap.h"
#include "util/jxx.util.HashMap.h"

namespace jxx::util::concurrent {

template<typename K, typename V>
class ConcurrentHashMap final
    : public ::jxx::lang::ClassBase<
          ConcurrentHashMap<K, V>,
          ::jxx::util::HashMap<K, V>,
          ConcurrentMap<K, V>> {
public:
    using JxxSuper = ::jxx::util::HashMap<K, V>;
    using Super = ::jxx::lang::ClassBase<
        ConcurrentHashMap<K, V>, JxxSuper, ConcurrentMap<K, V>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return JxxClassInfoMarker::Class();
    }

    ConcurrentHashMap() : Super() {}
    explicit ConcurrentHashMap(::jxx::lang::jint initialCapacity)
        : Super(initialCapacity) {}

    ::jxx::lang::jint size() override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        return JxxSuper::size();
    }

    ::jxx::lang::jbool isEmpty() override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        return JxxSuper::isEmpty();
    }

    ::jxx::lang::jbool containsKey(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        return JxxSuper::containsKey(key);
    }

    ::jxx::lang::jbool containsValue(
        const ::jxx::Ptr<::jxx::lang::Object>& value) override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        return JxxSuper::containsValue(value);
    }

    ::jxx::Ptr<V> get(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        return JxxSuper::get(key);
    }

    ::jxx::Ptr<V> put(
        const ::jxx::Ptr<K>& key,
        const ::jxx::Ptr<V>& value) override {
        require_(key, value);
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        return JxxSuper::put(key, value);
    }

    ::jxx::Ptr<V> remove(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        return JxxSuper::remove(key);
    }

    void putAll(const ::jxx::Ptr<::jxx::util::Map<K, V>>& map) override {
        if (!map) throw ::jxx::lang::NullPointerException();
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        JxxSuper::putAll(map);
    }

    void clear() override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        JxxSuper::clear();
    }

    ::jxx::Ptr<V> putIfAbsent(
        const ::jxx::Ptr<K>& key,
        const ::jxx::Ptr<V>& value) override {
        require_(key, value);
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        auto objectKey = ::jxx::CAST<::jxx::lang::Object>(key);
        auto current = JxxSuper::get(objectKey);
        if (current) return current;
        JxxSuper::put(key, value);
        return nullptr;
    }

    ::jxx::lang::jbool remove(
        const ::jxx::Ptr<::jxx::lang::Object>& key,
        const ::jxx::Ptr<::jxx::lang::Object>& value) override {
        if (!key || !value) return false;
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        auto current = JxxSuper::get(key);
        auto currentObject = ::jxx::CAST<::jxx::lang::Object>(current);
        if (!currentObject || !currentObject->equals(value)) return false;
        JxxSuper::remove(key);
        return true;
    }

    ::jxx::lang::jbool replace(
        const ::jxx::Ptr<K>& key,
        const ::jxx::Ptr<V>& oldValue,
        const ::jxx::Ptr<V>& newValue) override {
        require_(key, newValue);
        if (!oldValue) throw ::jxx::lang::NullPointerException();
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        auto current = JxxSuper::get(::jxx::CAST<::jxx::lang::Object>(key));
        auto currentObject = ::jxx::CAST<::jxx::lang::Object>(current);
        auto oldObject = ::jxx::CAST<::jxx::lang::Object>(oldValue);
        if (!currentObject || !currentObject->equals(oldObject)) return false;
        JxxSuper::put(key, newValue);
        return true;
    }

    ::jxx::Ptr<V> replace(
        const ::jxx::Ptr<K>& key,
        const ::jxx::Ptr<V>& value) override {
        require_(key, value);
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        auto objectKey = ::jxx::CAST<::jxx::lang::Object>(key);
        if (!JxxSuper::containsKey(objectKey)) return nullptr;
        return JxxSuper::put(key, value);
    }

    ::jxx::Ptr<V> computeIfAbsent(
        const ::jxx::Ptr<K>& key,
        const ::jxx::Ptr<::jxx::util::function::Function<K, V>>& function) override {
        if (!key || !function) throw ::jxx::lang::NullPointerException();
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        auto objectKey = ::jxx::CAST<::jxx::lang::Object>(key);
        auto current = JxxSuper::get(objectKey);
        if (current) return current;
        auto replacement = function->apply(key);
        if (replacement) JxxSuper::put(key, replacement);
        return replacement;
    }

    ::jxx::Ptr<V> computeIfPresent(
        const ::jxx::Ptr<K>& key,
        const ::jxx::Ptr<::jxx::util::function::BiFunction<K, V, V>>& function) override {
        if (!key || !function) throw ::jxx::lang::NullPointerException();
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        auto objectKey = ::jxx::CAST<::jxx::lang::Object>(key);
        auto current = JxxSuper::get(objectKey);
        if (!current) return nullptr;
        auto replacement = function->apply(key, current);
        if (replacement) JxxSuper::put(key, replacement);
        else JxxSuper::remove(objectKey);
        return replacement;
    }

    ::jxx::Ptr<V> compute(
        const ::jxx::Ptr<K>& key,
        const ::jxx::Ptr<::jxx::util::function::BiFunction<K, V, V>>& function) override {
        if (!key || !function) throw ::jxx::lang::NullPointerException();
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        auto objectKey = ::jxx::CAST<::jxx::lang::Object>(key);
        auto current = JxxSuper::get(objectKey);
        auto replacement = function->apply(key, current);
        if (replacement) JxxSuper::put(key, replacement);
        else JxxSuper::remove(objectKey);
        return replacement;
    }

    ::jxx::Ptr<V> merge(
        const ::jxx::Ptr<K>& key,
        const ::jxx::Ptr<V>& value,
        const ::jxx::Ptr<::jxx::util::function::BiFunction<V, V, V>>& function) override {
        require_(key, value);
        if (!function) throw ::jxx::lang::NullPointerException();
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        auto objectKey = ::jxx::CAST<::jxx::lang::Object>(key);
        auto current = JxxSuper::get(objectKey);
        auto replacement = current ? function->apply(current, value) : value;
        if (replacement) JxxSuper::put(key, replacement);
        else JxxSuper::remove(objectKey);
        return replacement;
    }

    void forEach(
        ::jxx::lang::jlong parallelismThreshold,
        const ::jxx::Ptr<::jxx::util::function::BiConsumer<K, V>>& action) {
        (void)parallelismThreshold;
        if (!action) throw ::jxx::lang::NullPointerException();
        for (const auto& item : snapshot_()) action->accept(item.first, item.second);
    }

    template<typename U>
    void forEach(
        ::jxx::lang::jlong parallelismThreshold,
        const ::jxx::Ptr<::jxx::util::function::BiFunction<K, V, U>>& transformer,
        const ::jxx::Ptr<::jxx::util::function::Consumer<U>>& action) {
        (void)parallelismThreshold;
        if (!transformer || !action) throw ::jxx::lang::NullPointerException();
        for (const auto& item : snapshot_()) {
            auto result = transformer->apply(item.first, item.second);
            if (result) action->accept(result);
        }
    }

    void forEachKey(::jxx::lang::jlong threshold,
        const ::jxx::Ptr<::jxx::util::function::Consumer<K>>& action) {
        (void)threshold; if (!action) throw ::jxx::lang::NullPointerException();
        for (const auto& item : snapshot_()) action->accept(item.first);
    }

    void forEachValue(::jxx::lang::jlong threshold,
        const ::jxx::Ptr<::jxx::util::function::Consumer<V>>& action) {
        (void)threshold; if (!action) throw ::jxx::lang::NullPointerException();
        for (const auto& item : snapshot_()) action->accept(item.second);
    }

    template<typename U>
    ::jxx::Ptr<U> search(::jxx::lang::jlong threshold,
        const ::jxx::Ptr<::jxx::util::function::BiFunction<K, V, U>>& function) {
        (void)threshold; if (!function) throw ::jxx::lang::NullPointerException();
        for (const auto& item : snapshot_()) {
            auto result = function->apply(item.first, item.second);
            if (result) return result;
        }
        return nullptr;
    }

    template<typename U>
    ::jxx::Ptr<U> searchKeys(::jxx::lang::jlong threshold,
        const ::jxx::Ptr<::jxx::util::function::Function<K, U>>& function) {
        (void)threshold; if (!function) throw ::jxx::lang::NullPointerException();
        for (const auto& item : snapshot_()) { auto r=function->apply(item.first); if(r)return r; }
        return nullptr;
    }

    template<typename U>
    ::jxx::Ptr<U> searchValues(::jxx::lang::jlong threshold,
        const ::jxx::Ptr<::jxx::util::function::Function<V, U>>& function) {
        (void)threshold; if (!function) throw ::jxx::lang::NullPointerException();
        for (const auto& item : snapshot_()) { auto r=function->apply(item.second); if(r)return r; }
        return nullptr;
    }

    template<typename U>
    ::jxx::Ptr<U> reduce(::jxx::lang::jlong threshold,
        const ::jxx::Ptr<::jxx::util::function::BiFunction<K, V, U>>& transformer,
        const ::jxx::Ptr<::jxx::util::function::BiFunction<U, U, U>>& reducer) {
        (void)threshold;
        if (!transformer || !reducer) throw ::jxx::lang::NullPointerException();
        ::jxx::Ptr<U> result;
        for (const auto& item : snapshot_()) {
            auto next=transformer->apply(item.first,item.second);
            if(next) result=result?reducer->apply(result,next):next;
        }
        return result;
    }

    ::jxx::Ptr<K> reduceKeys(::jxx::lang::jlong threshold,
        const ::jxx::Ptr<::jxx::util::function::BiFunction<K, K, K>>& reducer) {
        (void)threshold; if(!reducer)throw ::jxx::lang::NullPointerException();
        ::jxx::Ptr<K> result; for(const auto& item:snapshot_()) result=result?reducer->apply(result,item.first):item.first; return result;
    }

    template<typename U>
    ::jxx::Ptr<U> reduceKeys(::jxx::lang::jlong threshold,
        const ::jxx::Ptr<::jxx::util::function::Function<K, U>>& transformer,
        const ::jxx::Ptr<::jxx::util::function::BiFunction<U, U, U>>& reducer) {
        (void)threshold; if(!transformer||!reducer)throw ::jxx::lang::NullPointerException();
        ::jxx::Ptr<U> result; for(const auto& item:snapshot_()){auto next=transformer->apply(item.first);if(next)result=result?reducer->apply(result,next):next;}return result;
    }

    ::jxx::Ptr<V> reduceValues(::jxx::lang::jlong threshold,
        const ::jxx::Ptr<::jxx::util::function::BiFunction<V, V, V>>& reducer) {
        (void)threshold; if(!reducer)throw ::jxx::lang::NullPointerException();
        ::jxx::Ptr<V> result; for(const auto& item:snapshot_()) result=result?reducer->apply(result,item.second):item.second; return result;
    }

    template<typename U>
    ::jxx::Ptr<U> reduceValues(::jxx::lang::jlong threshold,
        const ::jxx::Ptr<::jxx::util::function::Function<V, U>>& transformer,
        const ::jxx::Ptr<::jxx::util::function::BiFunction<U, U, U>>& reducer) {
        (void)threshold; if(!transformer||!reducer)throw ::jxx::lang::NullPointerException();
        ::jxx::Ptr<U> result; for(const auto& item:snapshot_()){auto next=transformer->apply(item.second);if(next)result=result?reducer->apply(result,next):next;}return result;
    }

    ::jxx::lang::jlong mappingCount() {
        return static_cast<::jxx::lang::jlong>(size());
    }

private:
    using SnapshotItem = std::pair<::jxx::Ptr<K>, ::jxx::Ptr<V>>;

    std::vector<SnapshotItem> snapshot_() {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        std::vector<SnapshotItem> result;
        auto entries = JxxSuper::entrySet();
        auto iterator = entries->iterator();
        result.reserve(static_cast<std::size_t>(JxxSuper::size()));
        while (iterator->hasNext()) {
            auto entry = iterator->next();
            result.emplace_back(entry->getKey(), entry->getValue());
        }
        return result;
    }

    static void require_(
        const ::jxx::Ptr<K>& key,
        const ::jxx::Ptr<V>& value) {
        if (!key || !value) throw ::jxx::lang::NullPointerException();
    }

    mutable std::recursive_mutex mutex_;
};

} // namespace jxx::util::concurrent
