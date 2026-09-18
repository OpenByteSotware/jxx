#pragma once
#include "swing/jxx.swing.ComboBoxModel.h"
namespace jxx::swing
{
    template<typename E>
    class MutableComboBoxModel : public ::jxx::lang::InterfaceBase<MutableComboBoxModel<E>,ComboBoxModel<E>>
    {
    public:
        ~MutableComboBoxModel() override=default;
        virtual void addElement(const ::jxx::Ptr<E>& item)=0;
        virtual void removeElement(const ::jxx::Ptr<E>& item)=0;
        virtual void insertElementAt(const ::jxx::Ptr<E>& item,::jxx::lang::jint index)=0;
        virtual void removeElementAt(::jxx::lang::jint index)=0;
    };
}
