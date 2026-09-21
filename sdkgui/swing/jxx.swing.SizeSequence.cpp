#include "swing/jxx.swing.SizeSequence.h"

#include <algorithm>
#include <cstdint>

#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::swing
{
    SizeSequence::SizeSequence() : Super()
    {
    }

    SizeSequence::SizeSequence(::jxx::lang::jint entries)
        : SizeSequence(entries, 0)
    {
    }

    SizeSequence::SizeSequence(
        ::jxx::lang::jint entries,
        ::jxx::lang::jint value)
        : Super()
    {
        checkLength(entries);
        sizes_.assign(static_cast<std::size_t>(entries), value);
    }

    SizeSequence::SizeSequence(const IntArray& sizes) : Super()
    {
        setSizes(sizes);
    }

    void SizeSequence::checkLength(::jxx::lang::jint length)
    {
        if (length < 0)
            throw ::jxx::lang::IllegalArgumentException("length");
    }

    void SizeSequence::checkIndex(::jxx::lang::jint index) const
    {
        if (index < 0
            || index >= static_cast<::jxx::lang::jint>(sizes_.size()))
            throw ::jxx::lang::IndexOutOfBoundsException("index");
    }

    void SizeSequence::setSizes(::jxx::lang::jint length)
    {
        checkLength(length);
        sizes_.assign(static_cast<std::size_t>(length), 0);
    }

    void SizeSequence::setSizes(const IntArray& sizes)
    {
        sizes_.clear();
        if (sizes == nullptr) return;
        sizes_.reserve(sizes->length);
        for (std::uint32_t index = 0; index < sizes->length; ++index)
            sizes_.push_back((*sizes)[index]);
    }

    SizeSequence::IntArray SizeSequence::getSizes() const
    {
        auto result = ::jxx::NEW<::jxx::lang::JxxArray<
            ::jxx::lang::jint, 1>>(
                static_cast<std::uint32_t>(sizes_.size()));
        for (std::uint32_t index = 0; index < result->length; ++index)
            (*result)[index] = sizes_[index];
        return result;
    }

    ::jxx::lang::jint SizeSequence::getPosition(
        ::jxx::lang::jint index) const
    {
        if (index < 0
            || index > static_cast<::jxx::lang::jint>(sizes_.size()))
            throw ::jxx::lang::IndexOutOfBoundsException("index");
        ::jxx::lang::jint position = 0;
        for (::jxx::lang::jint current = 0; current < index; ++current)
            position += sizes_[static_cast<std::size_t>(current)];
        return position;
    }

    ::jxx::lang::jint SizeSequence::getIndex(
        ::jxx::lang::jint position) const
    {
        if (position < 0) return 0;
        ::jxx::lang::jint offset = 0;
        for (std::size_t index = 0; index < sizes_.size(); ++index)
        {
            offset += sizes_[index];
            if (position < offset)
                return static_cast<::jxx::lang::jint>(index);
        }
        return static_cast<::jxx::lang::jint>(sizes_.size());
    }

    ::jxx::lang::jint SizeSequence::getSize(
        ::jxx::lang::jint index) const
    {
        checkIndex(index);
        return sizes_[static_cast<std::size_t>(index)];
    }

    void SizeSequence::setSize(
        ::jxx::lang::jint index,
        ::jxx::lang::jint size)
    {
        checkIndex(index);
        sizes_[static_cast<std::size_t>(index)] = size;
    }

    void SizeSequence::insertEntries(
        ::jxx::lang::jint start,
        ::jxx::lang::jint length,
        ::jxx::lang::jint value)
    {
        checkLength(length);
        if (start < 0
            || start > static_cast<::jxx::lang::jint>(sizes_.size()))
            throw ::jxx::lang::IndexOutOfBoundsException("start");
        sizes_.insert(
            sizes_.begin() + start,
            static_cast<std::size_t>(length),
            value);
    }

    void SizeSequence::removeEntries(
        ::jxx::lang::jint start,
        ::jxx::lang::jint length)
    {
        checkLength(length);
        if (start < 0
            || start + length
                > static_cast<::jxx::lang::jint>(sizes_.size()))
            throw ::jxx::lang::IndexOutOfBoundsException("range");
        sizes_.erase(
            sizes_.begin() + start,
            sizes_.begin() + start + length);
    }
}
