#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::swing::event { class ListSelectionListener; }
namespace jxx::swing
{
    class ListSelectionModel : public ::jxx::lang::InterfaceBase<ListSelectionModel>
    {
    public:
        static constexpr ::jxx::lang::jint SINGLE_SELECTION = 0;
        static constexpr ::jxx::lang::jint SINGLE_INTERVAL_SELECTION = 1;
        static constexpr ::jxx::lang::jint MULTIPLE_INTERVAL_SELECTION = 2;
        ~ListSelectionModel() override = default;
        virtual void setSelectionInterval(::jxx::lang::jint index0, ::jxx::lang::jint index1) = 0;
        virtual void addSelectionInterval(::jxx::lang::jint index0, ::jxx::lang::jint index1) = 0;
        virtual void removeSelectionInterval(::jxx::lang::jint index0, ::jxx::lang::jint index1) = 0;
        virtual ::jxx::lang::jint getMinSelectionIndex() const = 0;
        virtual ::jxx::lang::jint getMaxSelectionIndex() const = 0;
        virtual ::jxx::lang::jbool isSelectedIndex(::jxx::lang::jint index) const = 0;
        virtual ::jxx::lang::jint getAnchorSelectionIndex() const = 0;
        virtual void setAnchorSelectionIndex(::jxx::lang::jint index) = 0;
        virtual ::jxx::lang::jint getLeadSelectionIndex() const = 0;
        virtual void setLeadSelectionIndex(::jxx::lang::jint index) = 0;
        virtual void clearSelection() = 0;
        virtual ::jxx::lang::jbool isSelectionEmpty() const = 0;
        virtual void insertIndexInterval(::jxx::lang::jint index, ::jxx::lang::jint length, ::jxx::lang::jbool before) = 0;
        virtual void removeIndexInterval(::jxx::lang::jint index0, ::jxx::lang::jint index1) = 0;
        virtual void setValueIsAdjusting(::jxx::lang::jbool value) = 0;
        virtual ::jxx::lang::jbool getValueIsAdjusting() const = 0;
        virtual void setSelectionMode(::jxx::lang::jint mode) = 0;
        virtual ::jxx::lang::jint getSelectionMode() const = 0;
        virtual void addListSelectionListener(const ::jxx::Ptr<::jxx::swing::event::ListSelectionListener>& listener) = 0;
        virtual void removeListSelectionListener(const ::jxx::Ptr<::jxx::swing::event::ListSelectionListener>& listener) = 0;
    };
}
