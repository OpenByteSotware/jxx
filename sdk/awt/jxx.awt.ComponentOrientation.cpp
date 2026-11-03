#include "awt/jxx.awt.ComponentOrientation.h"

#include <array>
#include <string>

#include "util/jxx.util.Locale.h"
#include "util/jxx.util.ResourceBundle.h"
#include "lang/jxx.lang.String.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.NullPointerException.h"

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


    namespace component_orientation_detail {
        ::jxx::Ptr<ComponentOrientation> createSerializationInstance()
        {
            return ::jxx::Ptr<ComponentOrientation>(
                new ComponentOrientation(true, true));
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


    void ComponentOrientation::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output)
    {
        if (output == nullptr) throw ::jxx::lang::NullPointerException();
        output->writeBoolean(horizontal_);
        output->writeBoolean(leftToRight_);
    }

    void ComponentOrientation::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input)
    {
        if (input == nullptr) throw ::jxx::lang::NullPointerException();
        horizontal_ = input->readBoolean();
        leftToRight_ = input->readBoolean();
    }

    void ComponentOrientation::readObjectNoData()
    {
        horizontal_ = true;
        leftToRight_ = true;
    }
}
