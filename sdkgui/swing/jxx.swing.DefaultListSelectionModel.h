#pragma once
#include <set>
#include <vector>
#include "lang/jxx.lang.Object.h"
#include "swing/jxx.swing.ListSelectionModel.h"
namespace jxx::swing
{
    class DefaultListSelectionModel : public ::jxx::lang::ClassBase<
        DefaultListSelectionModel, ::jxx::lang::Object, ListSelectionModel>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<DefaultListSelectionModel, JxxSuper, ListSelectionModel>;
        DefaultListSelectionModel();
        void setSelectionInterval(::jxx::lang::jint index0, ::jxx::lang::jint index1) override;
        void addSelectionInterval(::jxx::lang::jint index0, ::jxx::lang::jint index1) override;
        void removeSelectionInterval(::jxx::lang::jint index0, ::jxx::lang::jint index1) override;
        ::jxx::lang::jint getMinSelectionIndex() const override;
        ::jxx::lang::jint getMaxSelectionIndex() const override;
        ::jxx::lang::jbool isSelectedIndex(::jxx::lang::jint index) const override;
        ::jxx::lang::jint getAnchorSelectionIndex() const override;
        void setAnchorSelectionIndex(::jxx::lang::jint index) override;
        ::jxx::lang::jint getLeadSelectionIndex() const override;
        void setLeadSelectionIndex(::jxx::lang::jint index) override;
        void clearSelection() override;
        ::jxx::lang::jbool isSelectionEmpty() const override;
        void insertIndexInterval(::jxx::lang::jint index, ::jxx::lang::jint length, ::jxx::lang::jbool before) override;
        void removeIndexInterval(::jxx::lang::jint index0, ::jxx::lang::jint index1) override;
        void setValueIsAdjusting(::jxx::lang::jbool value) override;
        ::jxx::lang::jbool getValueIsAdjusting() const override;
        void setSelectionMode(::jxx::lang::jint mode) override;
        ::jxx::lang::jint getSelectionMode() const override;
        void addListSelectionListener(const ::jxx::Ptr<::jxx::swing::event::ListSelectionListener>& listener) override;
        void removeListSelectionListener(const ::jxx::Ptr<::jxx::swing::event::ListSelectionListener>& listener) override;
    private:
        void fireValueChanged(::jxx::lang::jint first, ::jxx::lang::jint last);
        std::set<::jxx::lang::jint> selected_;
        std::vector<::jxx::Ptr<::jxx::swing::event::ListSelectionListener>> listeners_;
        ::jxx::lang::jint anchor_;
        ::jxx::lang::jint lead_;
        ::jxx::lang::jint mode_;
        ::jxx::lang::jbool adjusting_;
    };
}
