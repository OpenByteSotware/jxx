#include "awt/jxx.awt.AlphaComposite.h"

#include <cstdint>
#include <cstring>

#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    ::jxx::Ptr<AlphaComposite> AlphaComposite::create(
        ::jxx::lang::jint rule, ::jxx::lang::jfloat alpha)
    {
        return ::jxx::Ptr<AlphaComposite>(
            new AlphaComposite(rule, alpha));
    }

    ::jxx::Ptr<AlphaComposite> AlphaComposite::Clear =
        create(CLEAR, 1.0f);
    ::jxx::Ptr<AlphaComposite> AlphaComposite::Src =
        create(SRC, 1.0f);
    ::jxx::Ptr<AlphaComposite> AlphaComposite::Dst =
        create(DST, 1.0f);
    ::jxx::Ptr<AlphaComposite> AlphaComposite::SrcOver =
        create(SRC_OVER, 1.0f);
    ::jxx::Ptr<AlphaComposite> AlphaComposite::DstOver =
        create(DST_OVER, 1.0f);
    ::jxx::Ptr<AlphaComposite> AlphaComposite::SrcIn =
        create(SRC_IN, 1.0f);
    ::jxx::Ptr<AlphaComposite> AlphaComposite::DstIn =
        create(DST_IN, 1.0f);
    ::jxx::Ptr<AlphaComposite> AlphaComposite::SrcOut =
        create(SRC_OUT, 1.0f);
    ::jxx::Ptr<AlphaComposite> AlphaComposite::DstOut =
        create(DST_OUT, 1.0f);
    ::jxx::Ptr<AlphaComposite> AlphaComposite::SrcAtop =
        create(SRC_ATOP, 1.0f);
    ::jxx::Ptr<AlphaComposite> AlphaComposite::DstAtop =
        create(DST_ATOP, 1.0f);
    ::jxx::Ptr<AlphaComposite> AlphaComposite::Xor =
        create(XOR, 1.0f);

    AlphaComposite::AlphaComposite(::jxx::lang::jint rule,
        ::jxx::lang::jfloat alpha)
        : rule_(rule), alpha_(alpha)
    {
        validate(rule, alpha);
    }

    void AlphaComposite::validate(::jxx::lang::jint rule,
        ::jxx::lang::jfloat alpha)
    {
        if (rule < CLEAR || rule > XOR)
            throw ::jxx::lang::IllegalArgumentException("rule");
        if (alpha < 0.0f || alpha > 1.0f)
            throw ::jxx::lang::IllegalArgumentException("alpha");
    }

    ::jxx::Ptr<AlphaComposite> AlphaComposite::getInstance(
        ::jxx::lang::jint rule)
    {
        return getInstance(rule, 1.0f);
    }

    ::jxx::Ptr<AlphaComposite> AlphaComposite::getInstance(
        ::jxx::lang::jint rule,
        ::jxx::lang::jfloat alpha)
    {
        validate(rule, alpha);
        if (alpha != 1.0f) return create(rule, alpha);
        switch (rule)
        {
            case CLEAR: return Clear;
            case SRC: return Src;
            case DST: return Dst;
            case SRC_OVER: return SrcOver;
            case DST_OVER: return DstOver;
            case SRC_IN: return SrcIn;
            case DST_IN: return DstIn;
            case SRC_OUT: return SrcOut;
            case DST_OUT: return DstOut;
            case SRC_ATOP: return SrcAtop;
            case DST_ATOP: return DstAtop;
            case XOR: return Xor;
            default: break;
        }
        return nullptr;
    }

    ::jxx::Ptr<AlphaComposite> AlphaComposite::derive(
        ::jxx::lang::jint rule) const
    {
        return rule == rule_
            ? ::jxx::CAST<AlphaComposite>(thisPtr())
            : getInstance(rule, alpha_);
    }

    ::jxx::Ptr<AlphaComposite> AlphaComposite::derive(
        ::jxx::lang::jfloat alpha) const
    {
        return alpha == alpha_
            ? ::jxx::CAST<AlphaComposite>(thisPtr())
            : getInstance(rule_, alpha);
    }

    ::jxx::lang::jfloat AlphaComposite::getAlpha() const { return alpha_; }
    ::jxx::lang::jint AlphaComposite::getRule() const { return rule_; }

    ::jxx::Ptr<CompositeContext> AlphaComposite::createContext(
        const ::jxx::Ptr<::jxx::awt::image::ColorModel>&,
        const ::jxx::Ptr<::jxx::awt::image::ColorModel>&,
        const ::jxx::Ptr<RenderingHints>&) const
    {
        return nullptr;
    }

    ::jxx::lang::jbool AlphaComposite::equals(
        const ::jxx::Ptr<::jxx::lang::Object>& object) const
    {
        const auto other = ::jxx::CAST<AlphaComposite>(object);
        return other != nullptr && rule_ == other->rule_ &&
            alpha_ == other->alpha_;
    }

    ::jxx::lang::jint AlphaComposite::hashCode() const
    {
        std::uint32_t bits = 0;
        std::memcpy(&bits, &alpha_, sizeof(bits));
        return static_cast<::jxx::lang::jint>(bits ^
            static_cast<std::uint32_t>(rule_));
    }
}
