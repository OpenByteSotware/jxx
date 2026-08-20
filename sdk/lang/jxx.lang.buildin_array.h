#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "lang/jxx.lang.ByteType.h"
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang {

    // Forward declaration.
    template <typename T, std::uint32_t Rank>
    class JxxArray;

    // Produces nested initializer_list types for multidimensional literals.
    template <typename T, std::uint32_t Rank>
    struct JInitList {
        static_assert(Rank > 1U, "JInitList recursion requires Rank > 1");
        using type = std::initializer_list<typename JInitList<T, Rank - 1U>::type>;
    };

    template <typename T>
    struct JInitList<T, 1U> {
        using type = std::initializer_list<T>;
    };

    namespace array_detail {

        template <typename Integer>
        using EnableIntegral = std::enable_if_t<
            std::is_integral_v<std::remove_cv_t<std::remove_reference_t<Integer>>>,
            int>;

        template <typename Integer, EnableIntegral<Integer> = 0>
        inline jxx::lang::jint checkedLength(Integer value) {
            using Raw = std::remove_cv_t<std::remove_reference_t<Integer>>;

            if constexpr (std::is_signed_v<Raw>) {
                if (value < 0) {
                    //throw jxx::lang::NegativeArraySizeException();
                }
            }

            using Unsigned = std::make_unsigned_t<Raw>;
            const auto unsignedValue = static_cast<Unsigned>(value);

            if (static_cast<std::uintmax_t>(unsignedValue) >
                static_cast<std::uintmax_t>(
                    std::numeric_limits<jxx::lang::jint>::max())) {
               // throw jxx::lang::NegativeArraySizeException();
            }

            return static_cast<jxx::lang::jint>(unsignedValue);
        }

        inline std::size_t nativeSize(jxx::lang::jint value) noexcept {
            return static_cast<std::size_t>(value);
        }

        [[noreturn]] inline void throwIndex(jxx::lang::jint /*index*/) {
            ///throw jxx::lang::ArrayIndexOutOfBoundsException();
        }

        [[noreturn]] inline void throwNullRow() {
            //throw jxx::lang::NullPointerException();
        }

    } // namespace array_detail

    // ============================================================================
    // Rank 1 specialization: T[]
    // ============================================================================
    template <typename T>
    class JxxArray<T, 1U> : public virtual jxx::lang::Object {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using iterator = T*;
        using const_iterator = const T*;

        // Java-compatible public field. Array length never changes.
        const jxx::lang::jint length;

    private:
        struct CheckedLengthTag {};

        std::unique_ptr<T[]> data_;

        JxxArray(jxx::lang::jint checkedLength, CheckedLengthTag)
            : length(checkedLength),
            data_(checkedLength == 0
                ? nullptr
                : std::make_unique<T[]>(
                    array_detail::nativeSize(checkedLength))) {}

        void checkIndex_(jxx::lang::jint index) const {
            if (index < 0 || index >= length) {
                //array_detail::throwIndex(index);
            }
        }

    public:
        JxxArray()
            : length(0), data_(nullptr) {}

        template <typename Integer, array_detail::EnableIntegral<Integer> = 0>
        explicit JxxArray(Integer requestedLength)
            : JxxArray(array_detail::checkedLength(requestedLength),
                CheckedLengthTag{}) {}

        template <typename Integer, array_detail::EnableIntegral<Integer> = 0>
        explicit JxxArray(const std::array<Integer, 1U>& dimensions)
            : JxxArray(dimensions[0]) {}

        JxxArray(std::initializer_list<T> values)
            : JxxArray(array_detail::checkedLength(values.size()),
                CheckedLengthTag{}) {
            std::copy(values.begin(), values.end(), begin());
        }

        explicit JxxArray(const std::vector<T>& values)
            : JxxArray(array_detail::checkedLength(values.size()),
                CheckedLengthTag{}) {
            std::copy(values.begin(), values.end(), begin());
        }

        explicit JxxArray(std::vector<T>&& values)
            : JxxArray(array_detail::checkedLength(values.size()),
                CheckedLengthTag{}) {
            std::move(values.begin(), values.end(), begin());
        }

        template <std::size_t Count>
        explicit JxxArray(const std::array<T, Count>& values)
            : JxxArray(array_detail::checkedLength(Count), CheckedLengthTag{}) {
            std::copy(values.begin(), values.end(), begin());
        }

        template <std::size_t Count>
        explicit JxxArray(std::array<T, Count>&& values)
            : JxxArray(array_detail::checkedLength(Count), CheckedLengthTag{}) {
            std::move(values.begin(), values.end(), begin());
        }

        JxxArray(const JxxArray& other)
            : JxxArray(other.length, CheckedLengthTag{}) {
            std::copy(other.begin(), other.end(), begin());
        }

        JxxArray& operator=(const JxxArray& other) {
            if (this == &other) {
                return *this;
            }
            if (length != other.length) {
               // throw jxx::lang::IllegalArgumentException();
            }
            std::copy(other.begin(), other.end(), begin());
            return *this;
        }

        JxxArray(JxxArray&& other)
            : JxxArray(other.length, CheckedLengthTag{}) {
            std::move(other.begin(), other.end(), begin());
            std::fill(other.begin(), other.end(), T{});
        }

        JxxArray& operator=(JxxArray&& other) {
            if (this == &other) {
                return *this;
            }
            if (length != other.length) {
                //throw jxx::lang::IllegalArgumentException();
            }
            std::move(other.begin(), other.end(), begin());
            std::fill(other.begin(), other.end(), T{});
            return *this;
        }

        virtual ~JxxArray() = default;

        // Java-style checked indexing.
        reference operator[](jxx::lang::jint index) {
            checkIndex_(index);
            return data_[array_detail::nativeSize(index)];
        }

        const_reference operator[](jxx::lang::jint index) const {
            checkIndex_(index);
            return data_[array_detail::nativeSize(index)];
        }

        // Checked convenience alias.
        reference at(jxx::lang::jint index) {
            return (*this)[index];
        }

        const_reference at(jxx::lang::jint index) const {
            return (*this)[index];
        }

        pointer data() noexcept {
            return data_.get();
        }

        const_pointer data() const noexcept {
            return data_.get();
        }

        iterator begin() noexcept {
            return data_.get();
        }

        const_iterator begin() const noexcept {
            return data_.get();
        }

        const_iterator cbegin() const noexcept {
            return data_.get();
        }

        iterator end() noexcept {
            return data_ == nullptr
                ? nullptr
                : data_.get() + array_detail::nativeSize(length);
        }

        const_iterator end() const noexcept {
            return data_ == nullptr
                ? nullptr
                : data_.get() + array_detail::nativeSize(length);
        }

        const_iterator cend() const noexcept {
            return end();
        }

        size_type size() const noexcept {
            return array_detail::nativeSize(length);
        }

        jxx::lang::jbool empty() const noexcept {
            return static_cast<jxx::lang::jbool>(length == 0);
        }

        void fill(const T& value) {
            std::fill(begin(), end(), value);
        }

        friend bool operator==(const JxxArray& left, const JxxArray& right) {
            return left.length == right.length &&
                std::equal(left.begin(), left.end(), right.begin());
        }

        friend bool operator!=(const JxxArray& left, const JxxArray& right) {
            return !(left == right);
        }

    protected:
        jxx::Ptr<jxx::lang::Object> cloneImpl() const override {
            return jxx::NEW<JxxArray<T, 1U>>(*this);
        }
    };

    // ============================================================================
    // Rank N implementation: T[][]... where Rank >= 2
    // ============================================================================
    template <typename T, std::uint32_t Rank>
    class JxxArray : public virtual jxx::lang::Object {
        static_assert(Rank >= 2U,
            "Use JxxArray<T,1U> for rank-one arrays");

    public:
        using value_type = T;
        using SubArray = JxxArray<T, Rank - 1U>;
        using RowPointer = jxx::Ptr<SubArray>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using InitList = typename JInitList<T, Rank>::type;
        using iterator = typename std::vector<RowPointer>::iterator;
        using const_iterator = typename std::vector<RowPointer>::const_iterator;

        // Java-compatible fixed outer-array length.
        const jxx::lang::jint length;

    private:
        struct CheckedLengthTag {};
        struct RectangularTag {};

        std::vector<RowPointer> elements_;

        JxxArray(jxx::lang::jint checkedLength, CheckedLengthTag)
            : length(checkedLength),
            elements_(array_detail::nativeSize(checkedLength)) {}

        explicit JxxArray(
            const std::array<jxx::lang::jint, Rank>& dimensions,
            RectangularTag)
            : JxxArray(dimensions[0], CheckedLengthTag{}) {
            const auto tail = tailDimensions_(dimensions);
            for (jxx::lang::jint i = 0; i < length; ++i) {
                elements_[array_detail::nativeSize(i)] =
                    jxx::NEW<SubArray>(tail);
            }
        }

        void checkIndex_(jxx::lang::jint index) const {
            if (index < 0 || index >= length) {
                array_detail::throwIndex(index);
            }
        }

        template <typename Integer>
        static std::array<jxx::lang::jint, Rank> validateDimensions_(
            const std::array<Integer, Rank>& dimensions) {
            std::array<jxx::lang::jint, Rank> checked{};
            for (std::size_t i = 0; i < Rank; ++i) {
                checked[i] = array_detail::checkedLength(dimensions[i]);
            }
            return checked;
        }

        template <typename... Dimensions>
        static std::array<jxx::lang::jint, Rank> makeDimensions_(
            Dimensions... dimensions) {
            static_assert(sizeof...(Dimensions) == Rank,
                "Dimension count must equal JxxArray rank");

            const std::array<std::intmax_t, Rank> supplied{
                static_cast<std::intmax_t>(dimensions)... };

            std::array<jxx::lang::jint, Rank> checked{};
            for (std::size_t i = 0; i < Rank; ++i) {
                checked[i] = array_detail::checkedLength(supplied[i]);
            }
            return checked;
        }

        static std::array<jxx::lang::jint, Rank - 1U> tailDimensions_(
            const std::array<jxx::lang::jint, Rank>& dimensions) {
            std::array<jxx::lang::jint, Rank - 1U> tail{};
            for (std::size_t i = 1U; i < Rank; ++i) {
                tail[i - 1U] = dimensions[i];
            }
            return tail;
        }

    public:
        class RowProxy {
        private:
            RowPointer* slot_;

            void requireRow_() const {
                if (slot_ == nullptr || *slot_ == nullptr) {
                    array_detail::throwNullRow();
                }
            }

        public:
            explicit RowProxy(RowPointer* slot)
                : slot_(slot) {}

            RowProxy(const RowProxy&) = default;

            RowProxy& operator=(const RowProxy& other) {
                if (this != &other) {
                    *slot_ = other.ptr();
                }
                return *this;
            }

            RowProxy& operator=(std::nullptr_t) {
                *slot_ = nullptr;
                return *this;
            }

            RowProxy& operator=(const RowPointer& row) {
                *slot_ = row;
                return *this;
            }

            RowProxy& operator=(RowPointer&& row) {
                *slot_ = std::move(row);
                return *this;
            }

            explicit operator bool() const noexcept {
                return slot_ != nullptr && *slot_ != nullptr;
            }

            operator RowPointer& () noexcept {
                return *slot_;
            }

            operator const RowPointer& () const noexcept {
                return *slot_;
            }

            // Recursive checked indexing: a[i][j][k].
            decltype(auto) operator[](jxx::lang::jint index) {
                requireRow_();
                return (**slot_)[index];
            }

            decltype(auto) operator[](jxx::lang::jint index) const {
                requireRow_();
                return (**slot_)[index];
            }

            decltype(auto) at(jxx::lang::jint index) {
                requireRow_();
                return (**slot_).at(index);
            }

            decltype(auto) at(jxx::lang::jint index) const {
                requireRow_();
                return (**slot_).at(index);
            }

            RowPointer& ptr() noexcept {
                return *slot_;
            }

            const RowPointer& ptr() const noexcept {
                return *slot_;
            }

            SubArray& get() {
                requireRow_();
                return **slot_;
            }

            const SubArray& get() const {
                requireRow_();
                return **slot_;
            }

            SubArray* operator->() {
                requireRow_();
                return slot_->get();
            }

            const SubArray* operator->() const {
                requireRow_();
                return slot_->get();
            }
        };

        class ConstRowProxy {
        private:
            const RowPointer* slot_;

            void requireRow_() const {
                if (slot_ == nullptr || *slot_ == nullptr) {
                    array_detail::throwNullRow();
                }
            }

        public:
            explicit ConstRowProxy(const RowPointer* slot)
                : slot_(slot) {}

            explicit operator bool() const noexcept {
                return slot_ != nullptr && *slot_ != nullptr;
            }

            operator const RowPointer& () const noexcept {
                return *slot_;
            }

            decltype(auto) operator[](jxx::lang::jint index) const {
                requireRow_();
                return (**slot_)[index];
            }

            decltype(auto) at(jxx::lang::jint index) const {
                requireRow_();
                return (**slot_).at(index);
            }

            const RowPointer& ptr() const noexcept {
                return *slot_;
            }

            const SubArray& get() const {
                requireRow_();
                return **slot_;
            }

            const SubArray* operator->() const {
                requireRow_();
                return slot_->get();
            }
        };

        JxxArray()
            : length(0), elements_() {}

        // Java: new T[n][]...; all child rows start null.
        template <typename Integer, array_detail::EnableIntegral<Integer> = 0>
        explicit JxxArray(Integer outerLength)
            : JxxArray(array_detail::checkedLength(outerLength),
                CheckedLengthTag{}) {}

        // Rectangular allocation from std::array dimensions.
        template <typename Integer, array_detail::EnableIntegral<Integer> = 0>
        explicit JxxArray(const std::array<Integer, Rank>& dimensions)
            : JxxArray(validateDimensions_(dimensions), RectangularTag{}) {}

        template <typename... Dimensions,
            std::enable_if_t<
            sizeof...(Dimensions) == Rank &&
            std::conjunction_v<
            std::is_integral<
            std::remove_cv_t<
            std::remove_reference_t<
            Dimensions>>>...>,
            int> = 0>
            explicit JxxArray(
                Dimensions... dimensions)
            : JxxArray(
                makeDimensions_(
                    dimensions...)) {}

        // Nested Java-style initializer with non-null rows.
        JxxArray(InitList values)
            : JxxArray(array_detail::checkedLength(values.size()),
                CheckedLengthTag{}) {
            std::size_t index = 0;
            for (const auto& row : values) {
                elements_[index++] = jxx::NEW<SubArray>(row);
            }
        }

        // Row-reference initializer, including null rows.
        JxxArray(std::initializer_list<RowPointer> rows)
            : JxxArray(array_detail::checkedLength(rows.size()),
                CheckedLengthTag{}) {
            std::copy(rows.begin(), rows.end(), elements_.begin());
        }

        explicit JxxArray(const std::vector<RowPointer>& rows)
            : JxxArray(array_detail::checkedLength(rows.size()),
                CheckedLengthTag{}) {
            std::copy(rows.begin(), rows.end(), elements_.begin());
        }

        explicit JxxArray(std::vector<RowPointer>&& rows)
            : JxxArray(array_detail::checkedLength(rows.size()),
                CheckedLengthTag{}) {
            std::move(rows.begin(), rows.end(), elements_.begin());
        }

        template <std::size_t Count>
        explicit JxxArray(const std::array<RowPointer, Count>& rows)
            : JxxArray(array_detail::checkedLength(Count), CheckedLengthTag{}) {
            std::copy(rows.begin(), rows.end(), elements_.begin());
        }

        template <std::size_t Count>
        explicit JxxArray(std::array<RowPointer, Count>&& rows)
            : JxxArray(array_detail::checkedLength(Count), CheckedLengthTag{}) {
            std::move(rows.begin(), rows.end(), elements_.begin());
        }

        // Multidimensional copy is shallow with respect to child arrays.
        JxxArray(const JxxArray& other)
            : length(other.length), elements_(other.elements_) {}

        JxxArray& operator=(const JxxArray& other) {
            if (this == &other) {
                return *this;
            }
            if (length != other.length) {
                //throw jxx::lang::IllegalArgumentException();
            }
            elements_ = other.elements_;
            return *this;
        }

        JxxArray(JxxArray&& other)
            : length(other.length), elements_(std::move(other.elements_)) {
            other.elements_.clear();
            other.elements_.resize(array_detail::nativeSize(other.length));
        }

        JxxArray& operator=(JxxArray&& other) {
            if (this == &other) {
                return *this;
            }
            if (length != other.length) {
                //throw jxx::lang::IllegalArgumentException();
            }
            elements_ = std::move(other.elements_);
            other.elements_.clear();
            other.elements_.resize(array_detail::nativeSize(other.length));
            return *this;
        }

        virtual ~JxxArray() = default;

        // Checked outer access.
        RowProxy operator[](jxx::lang::jint index) {
            checkIndex_(index);
            return RowProxy(&elements_[array_detail::nativeSize(index)]);
        }

        ConstRowProxy operator[](jxx::lang::jint index) const {
            checkIndex_(index);
            return ConstRowProxy(&elements_[array_detail::nativeSize(index)]);
        }

        RowProxy at(jxx::lang::jint index) {
            return (*this)[index];
        }

        ConstRowProxy at(jxx::lang::jint index) const {
            return (*this)[index];
        }

        // Direct access to the lower-rank array reference.
        RowPointer& row(jxx::lang::jint index) {
            checkIndex_(index);
            return elements_[array_detail::nativeSize(index)];
        }

        const RowPointer& row(jxx::lang::jint index) const {
            checkIndex_(index);
            return elements_[array_detail::nativeSize(index)];
        }

        iterator begin() noexcept {
            return elements_.begin();
        }

        const_iterator begin() const noexcept {
            return elements_.begin();
        }

        const_iterator cbegin() const noexcept {
            return elements_.cbegin();
        }

        iterator end() noexcept {
            return elements_.end();
        }

        const_iterator end() const noexcept {
            return elements_.end();
        }

        const_iterator cend() const noexcept {
            return elements_.cend();
        }

        size_type size() const noexcept {
            return array_detail::nativeSize(length);
        }

        jxx::lang::jbool empty() const noexcept {
            return static_cast<jxx::lang::jbool>(length == 0);
        }

        void clearRows() noexcept {
            std::fill(elements_.begin(), elements_.end(), nullptr);
        }

        // Recursively fills non-null leaf arrays.
        void fill(const T& value) {
            for (auto& child : elements_) {
                if (child != nullptr) {
                    child->fill(value);
                }
            }
        }

        friend bool operator==(const JxxArray& left, const JxxArray& right) {
            if (left.length != right.length) {
                return false;
            }

            for (jxx::lang::jint i = 0; i < left.length; ++i) {
                const auto position = array_detail::nativeSize(i);
                const auto& leftRow = left.elements_[position];
                const auto& rightRow = right.elements_[position];

                if (leftRow == nullptr || rightRow == nullptr) {
                    if (leftRow != rightRow) {
                        return false;
                    }
                }
                else if (*leftRow != *rightRow) {
                    return false;
                }
            }
            return true;
        }

        friend bool operator!=(const JxxArray& left, const JxxArray& right) {
            return !(left == right);
        }

    protected:
        jxx::Ptr<jxx::lang::Object> cloneImpl() const override {
            return jxx::NEW<JxxArray<T, Rank>>(*this);
        }
    };

    // Generic array aliases.
    template <typename T, std::uint32_t Rank>
    using JxxArrayRef = jxx::Ptr<JxxArray<T, Rank>>;

    // Rank 1 concrete types.
    using BooleanArrayType = JxxArray<jbool, 1U>;
    using ByteArrayType = JxxArray<jbyte, 1U>;
    using CharArrayType = JxxArray<jchar, 1U>;
    using ShortArrayType = JxxArray<jshort, 1U>;
    using IntArrayType = JxxArray<jint, 1U>;
    using LongArrayType = JxxArray<jlong, 1U>;
    using FloatArrayType = JxxArray<jfloat, 1U>;
    using DoubleArrayType = JxxArray<jdouble, 1U>;
    using ObjectArrayType = JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>;

    // Rank 1 Java references.
    using BooleanArray = jxx::Ptr<BooleanArrayType>;
    using ByteArray = jxx::Ptr<ByteArrayType>;
    using CharArray = jxx::Ptr<CharArrayType>;
    using ShortArray = jxx::Ptr<ShortArrayType>;
    using IntArray = jxx::Ptr<IntArrayType>;
    using LongArray = jxx::Ptr<LongArrayType>;
    using FloatArray = jxx::Ptr<FloatArrayType>;
    using DoubleArray = jxx::Ptr<DoubleArrayType>;
    using ObjectArray = jxx::Ptr<ObjectArrayType>;

    // Rank 2 concrete types.
    using BooleanArray2DType = JxxArray<jbool, 2U>;
    using ByteArray2DType = JxxArray<jbyte, 2U>;
    using CharArray2DType = JxxArray<jchar, 2U>;
    using ShortArray2DType = JxxArray<jshort, 2U>;
    using IntArray2DType = JxxArray<jint, 2U>;
    using LongArray2DType = JxxArray<jlong, 2U>;
    using FloatArray2DType = JxxArray<jfloat, 2U>;
    using DoubleArray2DType = JxxArray<jdouble, 2U>;
    using ObjectArray2DType = JxxArray<jxx::Ptr<jxx::lang::Object>, 2U>;

    // Rank 2 Java references.
    using BooleanArray2D = jxx::Ptr<BooleanArray2DType>;
    using ByteArray2D = jxx::Ptr<ByteArray2DType>;
    using CharArray2D = jxx::Ptr<CharArray2DType>;
    using ShortArray2D = jxx::Ptr<ShortArray2DType>;
    using IntArray2D = jxx::Ptr<IntArray2DType>;
    using LongArray2D = jxx::Ptr<LongArray2DType>;
    using FloatArray2D = jxx::Ptr<FloatArray2DType>;
    using DoubleArray2D = jxx::Ptr<DoubleArray2DType>;
    using ObjectArray2D = jxx::Ptr<ObjectArray2DType>;

    // Rank 3 concrete types.
    using BooleanArray3DType = JxxArray<jbool, 3U>;
    using ByteArray3DType = JxxArray<jbyte, 3U>;
    using CharArray3DType = JxxArray<jchar, 3U>;
    using ShortArray3DType = JxxArray<jshort, 3U>;
    using IntArray3DType = JxxArray<jint, 3U>;
    using LongArray3DType = JxxArray<jlong, 3U>;
    using FloatArray3DType = JxxArray<jfloat, 3U>;
    using DoubleArray3DType = JxxArray<jdouble, 3U>;
    using ObjectArray3DType = JxxArray<jxx::Ptr<jxx::lang::Object>, 3U>;

    // Rank 3 Java references.
    using BooleanArray3D = jxx::Ptr<BooleanArray3DType>;
    using ByteArray3D = jxx::Ptr<ByteArray3DType>;
    using CharArray3D = jxx::Ptr<CharArray3DType>;
    using ShortArray3D = jxx::Ptr<ShortArray3DType>;
    using IntArray3D = jxx::Ptr<IntArray3DType>;
    using LongArray3D = jxx::Ptr<LongArray3DType>;
    using FloatArray3D = jxx::Ptr<FloatArray3DType>;
    using DoubleArray3D = jxx::Ptr<DoubleArray3DType>;
    using ObjectArray3D = jxx::Ptr<ObjectArray3DType>;

} // namespace jxx::lang
