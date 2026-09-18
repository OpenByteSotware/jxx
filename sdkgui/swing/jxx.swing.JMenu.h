#pragma once
#include <vector>
#include "swing/jxx.swing.JMenuItem.h"
namespace jxx::swing
{
    class JMenu : public ::jxx::lang::ClassBase<JMenu, JMenuItem>
    {
    public:
        using JxxSuper = JMenuItem;
        using Super = ::jxx::lang::ClassBase<JMenu, JxxSuper>;
        JMenu();
        explicit JMenu(const ::jxx::Ptr<::jxx::lang::String>& text);
        ::jxx::Ptr<JMenuItem> add(const ::jxx::Ptr<JMenuItem>& item);
        ::jxx::Ptr<JMenuItem> add(const ::jxx::Ptr<::jxx::lang::String>& text);
        void addSeparator();
        void insert(const ::jxx::Ptr<JMenuItem>& item, ::jxx::lang::jint position);
        void remove(::jxx::lang::jint position);
        void remove(const ::jxx::Ptr<JMenuItem>& item);
        void removeAll();
        ::jxx::lang::jint getItemCount() const;
        ::jxx::Ptr<JMenuItem> getItem(::jxx::lang::jint position) const;
        ::jxx::lang::jbool isTopLevelMenu() const;
        void setPopupMenuVisible(::jxx::lang::jbool visible);
        ::jxx::lang::jbool isPopupMenuVisible() const;
    private:
        std::vector<::jxx::Ptr<JMenuItem>> items_;
        ::jxx::lang::jbool popupVisible_ = false;
    };
}
