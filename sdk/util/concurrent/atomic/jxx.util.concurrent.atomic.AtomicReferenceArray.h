#pragma once

#include <mutex>
#include <vector>

#include "io/jxx.io.InvalidObjectException.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::util::concurrent::atomic {

template<typename E>
class AtomicReferenceArray final
    : public ::jxx::lang::ClassBase<
          AtomicReferenceArray<E>,
          ::jxx::lang::Object,
          ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        AtomicReferenceArray<E>, JxxSuper, ::jxx::io::SerializableI>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return JxxClassInfoMarker::Class();
    }

    explicit AtomicReferenceArray(::jxx::lang::jint length)
        : Super(), values_(checkedLength_(length)) {}

    explicit AtomicReferenceArray(
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<E>, 1U>>& array)
        : Super() {
        if (array == nullptr) throw ::jxx::lang::NullPointerException();
        values_.reserve(array->length);
        for (std::uint32_t index = 0; index < array->length; ++index) {
            values_.push_back((*array)[index]);
        }
    }

    ::jxx::lang::jint length() const noexcept {
        return static_cast<::jxx::lang::jint>(values_.size());
    }

    ::jxx::Ptr<E> get(::jxx::lang::jint index) const {
        std::lock_guard<std::mutex> lock(mutex_);
        return values_[checkedIndex_(index)];
    }

    void set(::jxx::lang::jint index, const ::jxx::Ptr<E>& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        values_[checkedIndex_(index)] = value;
    }

    void lazySet(::jxx::lang::jint index, const ::jxx::Ptr<E>& value) {
        set(index, value);
    }

    ::jxx::Ptr<E> getAndSet(
        ::jxx::lang::jint index,
        const ::jxx::Ptr<E>& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto& current = values_[checkedIndex_(index)];
        auto previous = current;
        current = value;
        return previous;
    }

    ::jxx::lang::jbool compareAndSet(
        ::jxx::lang::jint index,
        const ::jxx::Ptr<E>& expected,
        const ::jxx::Ptr<E>& update) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto& current = values_[checkedIndex_(index)];
        if (current.get() != expected.get()) return false;
        current = update;
        return true;
    }

    ::jxx::lang::jbool weakCompareAndSet(
        ::jxx::lang::jint index,
        const ::jxx::Ptr<E>& expected,
        const ::jxx::Ptr<E>& update) {
        return compareAndSet(index, expected, update);
    }

    void writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) override {
        if (output == nullptr) throw ::jxx::lang::NullPointerException();
        std::lock_guard<std::mutex> lock(mutex_);
        output->writeInt(static_cast<::jxx::lang::jint>(values_.size()));
        for (const auto& value : values_) {
            output->writeObject(::jxx::CAST<::jxx::lang::Object>(value));
        }
    }

    void readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) override {
        if (input == nullptr) throw ::jxx::lang::NullPointerException();
        const auto size = input->readInt();
        if (size < 0 || size > MAX_SERIALIZED_LENGTH_) {
            throw ::jxx::io::InvalidObjectException(
                ::jxx::NEW<::jxx::lang::String>(
                    "Invalid AtomicReferenceArray serialized length"));
        }

        std::vector<::jxx::Ptr<E>> restored;
        restored.reserve(static_cast<std::size_t>(size));
        for (::jxx::lang::jint index = 0; index < size; ++index) {
            const auto object = input->readObject();
            if (object == nullptr) {
                restored.push_back(nullptr);
                continue;
            }
            const auto value = std::dynamic_pointer_cast<E>(object);
            if (value == nullptr) {
                throw ::jxx::io::InvalidObjectException(
                    ::jxx::NEW<::jxx::lang::String>(
                        "AtomicReferenceArray element has incompatible type"));
            }
            restored.push_back(value);
        }

        std::lock_guard<std::mutex> lock(mutex_);
        values_ = std::move(restored);
    }

    void readObjectNoData() override {
        std::lock_guard<std::mutex> lock(mutex_);
        values_.clear();
    }

private:
    static constexpr ::jxx::lang::jint MAX_SERIALIZED_LENGTH_ =
        16 * 1024 * 1024;

    static std::size_t checkedLength_(::jxx::lang::jint length) {
        if (length < 0) throw ::jxx::lang::IllegalArgumentException();
        return static_cast<std::size_t>(length);
    }

    std::size_t checkedIndex_(::jxx::lang::jint index) const {
        if (index < 0 || index >= length())
            throw ::jxx::lang::IndexOutOfBoundsException();
        return static_cast<std::size_t>(index);
    }

    mutable std::mutex mutex_;
    std::vector<::jxx::Ptr<E>> values_;
};

} // namespace jxx::util::concurrent::atomic
