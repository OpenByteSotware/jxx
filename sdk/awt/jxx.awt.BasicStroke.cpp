#include "awt/jxx.awt.BasicStroke.h"

#include <cstdint>
#include <cstring>

#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    BasicStroke::BasicStroke()
        : BasicStroke(1.0f, CAP_SQUARE, JOIN_MITER, 10.0f,
            nullptr, 0.0f) {}

    BasicStroke::BasicStroke(::jxx::lang::jfloat width)
        : BasicStroke(width, CAP_SQUARE, JOIN_MITER, 10.0f,
            nullptr, 0.0f) {}

    BasicStroke::BasicStroke(::jxx::lang::jfloat width,
        ::jxx::lang::jint cap, ::jxx::lang::jint join)
        : BasicStroke(width, cap, join, 10.0f, nullptr, 0.0f) {}

    BasicStroke::BasicStroke(::jxx::lang::jfloat width,
        ::jxx::lang::jint cap, ::jxx::lang::jint join,
        ::jxx::lang::jfloat miterLimit)
        : BasicStroke(width, cap, join, miterLimit, nullptr, 0.0f) {}

    BasicStroke::BasicStroke(::jxx::lang::jfloat width,
        ::jxx::lang::jint cap, ::jxx::lang::jint join,
        ::jxx::lang::jfloat miterLimit, const FloatArray& dash,
        ::jxx::lang::jfloat dashPhase)
        : width_(width), cap_(cap), join_(join), miterLimit_(miterLimit),
          dashPhase_(dashPhase)
    {
        validate(width, cap, join, miterLimit, dash, dashPhase);
        if (dash != nullptr)
            for (std::uint32_t index = 0; index < dash->length; ++index)
                dash_.push_back((*dash)[index]);
    }

    void BasicStroke::validate(::jxx::lang::jfloat width,
        ::jxx::lang::jint cap, ::jxx::lang::jint join,
        ::jxx::lang::jfloat miterLimit, const FloatArray& dash,
        ::jxx::lang::jfloat dashPhase)
    {
        if (width < 0.0f) throw ::jxx::lang::IllegalArgumentException("width");
        if (cap < CAP_BUTT || cap > CAP_SQUARE)
            throw ::jxx::lang::IllegalArgumentException("cap");
        if (join < JOIN_MITER || join > JOIN_BEVEL)
            throw ::jxx::lang::IllegalArgumentException("join");
        if (join == JOIN_MITER && miterLimit < 1.0f)
            throw ::jxx::lang::IllegalArgumentException("miterLimit");
        if (dashPhase < 0.0f)
            throw ::jxx::lang::IllegalArgumentException("dashPhase");
        if (dash == nullptr) return;
        if (dash->length == 0)
            throw ::jxx::lang::IllegalArgumentException("dash");
        bool positive = false;
        for (std::uint32_t index = 0; index < dash->length; ++index)
        {
            const auto value = (*dash)[index];
            if (value < 0.0f)
                throw ::jxx::lang::IllegalArgumentException("dash");
            positive = positive || value > 0.0f;
        }
        if (!positive) throw ::jxx::lang::IllegalArgumentException("dash");
    }

    ::jxx::Ptr<Shape> BasicStroke::createStrokedShape(
        const ::jxx::Ptr<Shape>& shape) const
    {
        return shape;
    }

    ::jxx::lang::jfloat BasicStroke::getLineWidth() const { return width_; }
    ::jxx::lang::jint BasicStroke::getEndCap() const { return cap_; }
    ::jxx::lang::jint BasicStroke::getLineJoin() const { return join_; }
    ::jxx::lang::jfloat BasicStroke::getMiterLimit() const { return miterLimit_; }
    ::jxx::lang::jfloat BasicStroke::getDashPhase() const { return dashPhase_; }

    BasicStroke::FloatArray BasicStroke::getDashArray() const
    {
        if (dash_.empty()) return nullptr;
        auto result = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jfloat, 1>>(
            static_cast<std::uint32_t>(dash_.size()));
        for (std::uint32_t index = 0; index < result->length; ++index)
            (*result)[index] = dash_[index];
        return result;
    }

    ::jxx::lang::jbool BasicStroke::equals(
        const ::jxx::Ptr<::jxx::lang::Object>& object) const
    {
        const auto other = ::jxx::CAST<BasicStroke>(object);
        return other != nullptr && width_ == other->width_ &&
            cap_ == other->cap_ && join_ == other->join_ &&
            miterLimit_ == other->miterLimit_ && dash_ == other->dash_ &&
            dashPhase_ == other->dashPhase_;
    }

    ::jxx::lang::jint BasicStroke::hashCode() const
    {
        std::uint32_t hash = 1;
        const auto addFloat = [&hash](::jxx::lang::jfloat value)
        {
            std::uint32_t bits = 0;
            std::memcpy(&bits, &value, sizeof(bits));
            hash = hash * 31u + bits;
        };
        addFloat(width_);
        hash = hash * 31u + static_cast<std::uint32_t>(cap_);
        hash = hash * 31u + static_cast<std::uint32_t>(join_);
        addFloat(miterLimit_);
        for (const auto value : dash_) addFloat(value);
        addFloat(dashPhase_);
        return static_cast<::jxx::lang::jint>(hash);
    }
}
