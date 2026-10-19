#include "awt/jxx.awt.ComponentOrientation.h"

#include <array>
#include <string>

#include "util/jxx.util.Locale.h"
#include "util/jxx.util.ResourceBundle.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt
{
    namespace
    {
        ::jxx::lang::jbool isRightToLeftLanguage(const std::string& language)
        {
            static constexpr std::array<const char*, 8> languages = {
                "ar", "fa", "he", "iw", "ji", "ps", "ur", "yi"
            };
            for (const auto* candidate : languages)
                if (language == candidate) return true;
            return false;
        }
    }

    ::jxx::Ptr<ComponentOrientation> ComponentOrientation::LEFT_TO_RIGHT =
        create(true, true);

    ::jxx::Ptr<ComponentOrientation> ComponentOrientation::RIGHT_TO_LEFT =
        create(true, false);

    ::jxx::Ptr<ComponentOrientation> ComponentOrientation::UNKNOWN =
        create(true, true);

    ::jxx::Ptr<ComponentOrientation> ComponentOrientation::create(
        ::jxx::lang::jbool horizontal,
        ::jxx::lang::jbool leftToRight)
    {
        return ::jxx::Ptr<ComponentOrientation>(
            new ComponentOrientation(horizontal, leftToRight));
    }

    ComponentOrientation::ComponentOrientation(
        ::jxx::lang::jbool horizontal,
        ::jxx::lang::jbool leftToRight)
        : horizontal_(horizontal), leftToRight_(leftToRight)
    {
    }

    ::jxx::lang::jbool ComponentOrientation::isHorizontal() const
    {
        return horizontal_;
    }

    ::jxx::lang::jbool ComponentOrientation::isLeftToRight() const
    {
        return leftToRight_;
    }

    ::jxx::Ptr<ComponentOrientation> ComponentOrientation::getOrientation(
        const ::jxx::Ptr<::jxx::util::Locale>& locale)
    {
        if (locale == nullptr) return UNKNOWN;
        const auto language = locale->getLanguage();
        if (language != nullptr &&
            isRightToLeftLanguage(language->utf8()))
            return RIGHT_TO_LEFT;
        return LEFT_TO_RIGHT;
    }

    ::jxx::Ptr<ComponentOrientation> ComponentOrientation::getOrientation(
        const ::jxx::Ptr<::jxx::util::ResourceBundle>& bundle)
    {
        return bundle == nullptr
            ? UNKNOWN
            : getOrientation(bundle->getLocale());
    }
}
