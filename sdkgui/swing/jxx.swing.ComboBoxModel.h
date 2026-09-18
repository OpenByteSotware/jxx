#pragma once
#include "swing/jxx.swing.ListModel.h"
namespace jxx::swing
{
    template<typename E>
    class ComboBoxModel : public ::jxx::lang::InterfaceBase<ComboBoxModel<E>,ListModel<E>>
    {
    public:
        ~ComboBoxModel() override=default;
        virtual void setSelectedItem(const ::jxx::Ptr<E>& item)=0;
        virtual ::jxx::Ptr<E> getSelectedItem() const=0;
    };
}
