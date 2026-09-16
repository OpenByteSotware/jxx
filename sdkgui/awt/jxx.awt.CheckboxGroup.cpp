#include "awt/jxx.awt.CheckboxGroup.h"

#include <sstream>

#include "awt/jxx.awt.Checkbox.h"

namespace jxx::awt
{
    ::jxx::Ptr<Checkbox> CheckboxGroup::getSelectedCheckbox() const
    {
        return synchronized([this] { return selectedCheckbox_; });
    }

    void CheckboxGroup::setSelectedCheckbox(
        const ::jxx::Ptr<Checkbox>& checkbox)
    {
        synchronized([this, &checkbox]
        {
            if (checkbox != nullptr &&
                checkbox->getCheckboxGroup().get() != this)
                return;

            const auto previous = selectedCheckbox_;
            if (previous == checkbox) return;

            selectedCheckbox_ = checkbox;
            if (previous != nullptr) previous->setStateFromGroup(false);
            if (checkbox != nullptr) checkbox->setStateFromGroup(true);
        });
    }

    ::jxx::Ptr<Checkbox> CheckboxGroup::getCurrent() const
    {
        return getSelectedCheckbox();
    }

    void CheckboxGroup::setCurrent(const ::jxx::Ptr<Checkbox>& checkbox)
    {
        setSelectedCheckbox(checkbox);
    }

    ::jxx::Ptr<::jxx::lang::String> CheckboxGroup::toString() const
    {
        const auto selected = getSelectedCheckbox();
        std::ostringstream out;
        out << "jxx.awt.CheckboxGroup[selectedCheckbox=";
        if (selected != nullptr) out << selected->toString()->utf8();
        else out << "null";
        out << "]";
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
