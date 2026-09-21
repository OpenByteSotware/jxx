#include "swing/jxx.swing.LayoutStyle.h"

#include <mutex>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "swing/jxx.swing.JComponent.h"
#include "swing/jxx.swing.SwingConstants.h"

namespace jxx::swing
{
    namespace
    {
        class DefaultLayoutStyle final : public ::jxx::lang::ClassBase<
            DefaultLayoutStyle, LayoutStyle>
        {
        public:
            using JxxSuper = LayoutStyle;
            using Super = ::jxx::lang::ClassBase<DefaultLayoutStyle, JxxSuper>;

            DefaultLayoutStyle() : Super()
            {
            }

            ::jxx::lang::jint getPreferredGap(
                const ::jxx::Ptr<JComponent>& component1,
                const ::jxx::Ptr<JComponent>& component2,
                ComponentPlacement type,
                ::jxx::lang::jint position,
                const ::jxx::Ptr<::jxx::awt::Container>&) override
            {
                if (component1 == nullptr || component2 == nullptr)
                    throw ::jxx::lang::IllegalArgumentException("component");
                if (position != SwingConstants::NORTH
                    && position != SwingConstants::SOUTH
                    && position != SwingConstants::EAST
                    && position != SwingConstants::WEST)
                    throw ::jxx::lang::IllegalArgumentException("position");
                if (type == ComponentPlacement::UNRELATED) return 12;
                if (type == ComponentPlacement::INDENT) return 12;
                return 6;
            }

            ::jxx::lang::jint getContainerGap(
                const ::jxx::Ptr<JComponent>& component,
                ::jxx::lang::jint position,
                const ::jxx::Ptr<::jxx::awt::Container>&) override
            {
                if (component == nullptr)
                    throw ::jxx::lang::IllegalArgumentException("component");
                if (position != SwingConstants::NORTH
                    && position != SwingConstants::SOUTH
                    && position != SwingConstants::EAST
                    && position != SwingConstants::WEST)
                    throw ::jxx::lang::IllegalArgumentException("position");
                return 6;
            }
        };

        std::mutex& instanceMutex()
        {
            static std::mutex value;
            return value;
        }

        ::jxx::Ptr<LayoutStyle>& instanceValue()
        {
            static ::jxx::Ptr<LayoutStyle> value;
            return value;
        }
    }

    LayoutStyle::LayoutStyle() : Super()
    {
    }

    void LayoutStyle::setInstance(const ::jxx::Ptr<LayoutStyle>& style)
    {
        std::lock_guard<std::mutex> lock(instanceMutex());
        instanceValue() = style;
    }

    ::jxx::Ptr<LayoutStyle> LayoutStyle::getInstance()
    {
        std::lock_guard<std::mutex> lock(instanceMutex());
        auto& value = instanceValue();
        if (value == nullptr) value = ::jxx::NEW<DefaultLayoutStyle>();
        return value;
    }
}
