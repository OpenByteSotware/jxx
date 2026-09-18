#include "gui/internal/jxx.gui.internal.WxFramePeer.h"
#include <utility>
#include <wx/event.h>
#include <wx/frame.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/menu.h>
#include <wx/string.h>
#include "awt/event/jxx.awt.event.WindowEvent.h"
#include "awt/jxx.awt.CheckboxMenuItem.h"
#include "awt/jxx.awt.Button.h"
#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Label.h"
#include "awt/jxx.awt.TextField.h"
#include "gui/internal/jxx.gui.internal.WxComponentPeer.h"
#include "swing/jxx.swing.AbstractButton.h"
#include "swing/jxx.swing.JLabel.h"
#include "swing/jxx.swing.JTextArea.h"
#include "swing/jxx.swing.JTextField.h"
#include "swing/jxx.swing.JMenu.h"
#include "swing/jxx.swing.JMenuBar.h"
#include "swing/jxx.swing.JMenuItem.h"
#include "swing/jxx.swing.JCheckBoxMenuItem.h"
#include "swing/jxx.swing.JRadioButtonMenuItem.h"
#include "awt/jxx.awt.Menu.h"
#include "awt/jxx.awt.MenuBar.h"
#include "awt/jxx.awt.MenuItem.h"
#include "gui/internal/jxx.gui.internal.GuiRuntime.h"
namespace jxx::gui::internal
{
	namespace
	{
		wxString nativeText(const ::jxx::Ptr<::jxx::lang::String>& v)
		{
			return v ? wxString::FromUTF8(v->utf8().c_str()) : wxString();
		}
	}
	WxFramePeer::WxFramePeer(const ::jxx::Ptr<::jxx::lang::String>& title) :frame_(nullptr), title_(title ? title : ::jxx::NEW<::jxx::lang::String>(""))
	{
		if (GuiRuntime::ensureInitialized()) {
			frame_ = new wxFrame(nullptr, wxID_ANY, nativeText(title_), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);
			frame_->Bind(wxEVT_SHOW, [this](wxShowEvent& e)
		  {
						send(e.IsShown() ? ::jxx::awt::event::WindowEvent::WINDOW_OPENED : ::jxx::awt::event::WindowEvent::WINDOW_CLOSED); e.Skip();
		  });
			frame_->Bind(wxEVT_ACTIVATE, [this](wxActivateEvent& e)
		  {
						send(e.GetActive() ? ::jxx::awt::event::WindowEvent::WINDOW_ACTIVATED : ::jxx::awt::event::WindowEvent::WINDOW_DEACTIVATED); e.Skip();
		  });
			frame_->Bind(wxEVT_ICONIZE, [this](wxIconizeEvent& e)
		  {
						send(e.IsIconized() ? ::jxx::awt::event::WindowEvent::WINDOW_ICONIFIED : ::jxx::awt::event::WindowEvent::WINDOW_DEICONIFIED); e.Skip();
		  });
			frame_->Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent& e)
		  {
						send(::jxx::awt::event::WindowEvent::WINDOW_CLOSING); e.Skip();
		  });
			frame_->Bind(wxEVT_DESTROY, [this](wxWindowDestroyEvent& e)
		  {
						send(::jxx::awt::event::WindowEvent::WINDOW_CLOSED); frame_ = nullptr; e.Skip();
		  });
			frame_->Bind(wxEVT_MENU, [this](wxCommandEvent& e)
		  {
						dispatchMenu(e.GetId());
		  });
		}
	}
	WxFramePeer::~WxFramePeer()
	{
		destroy();
	} void WxFramePeer::send(::jxx::lang::jint id)
	{
		if (callback_)callback_(id);
	} void WxFramePeer::setEventCallback(EventCallback cb)
	{
		callback_ = std::move(cb);
	}
	void WxFramePeer::show()
	{
		if (frame_)frame_->Show(true);
	} void WxFramePeer::hide()
	{
		if (frame_)frame_->Show(false);
	} void WxFramePeer::destroy()
	{
		menuItems_.clear(); swingMenuItems_.clear(); if (frame_) {
			auto* value = frame_; frame_ = nullptr; value->Destroy();
		}
	}
	void WxFramePeer::toFront()
	{
		if (frame_)frame_->Raise();
	} void WxFramePeer::toBack()
	{
		if (frame_)frame_->Lower();
	} void WxFramePeer::setBounds(::jxx::lang::jint x, ::jxx::lang::jint y, ::jxx::lang::jint w, ::jxx::lang::jint h)
	{
		if (frame_)frame_->SetSize(x, y, w, h);
	}
	void WxFramePeer::setTitle(const ::jxx::Ptr<::jxx::lang::String>& title)
	{
		title_ = title ? title : ::jxx::NEW<::jxx::lang::String>(""); if (frame_)frame_->SetTitle(nativeText(title_));
	} ::jxx::Ptr<::jxx::lang::String> WxFramePeer::getTitle() const
	{
		return title_;
	} ::jxx::lang::jbool WxFramePeer::isShown() const
	{
		return frame_ && frame_->IsShown();
	}
	wxMenu* WxFramePeer::buildMenu(const ::jxx::Ptr<::jxx::awt::Menu>& menu)
	{
		auto* nativeMenu = new wxMenu(); if (!menu)return nativeMenu; for (::jxx::lang::jint i = 0; i < menu->getItemCount(); ++i) {
			auto item = menu->getItem(i); if (!item)continue; const auto label = item->getLabel(); if (label && label->utf8() == "-") {
				nativeMenu->AppendSeparator(); continue;
			}if (auto submenu = ::jxx::CAST<::jxx::awt::Menu>(item)) {
				nativeMenu->AppendSubMenu(buildMenu(submenu), nativeText(submenu->getLabel())); continue;
			}const int id = wxWindow::NewControlId(); wxItemKind kind = ::jxx::CAST<::jxx::awt::CheckboxMenuItem>(item) ? wxITEM_CHECK : wxITEM_NORMAL; auto* nativeItem = nativeMenu->Append(id, nativeText(label), wxEmptyString, kind); nativeItem->Enable(item->isEnabled()); if (auto check = ::jxx::CAST<::jxx::awt::CheckboxMenuItem>(item))nativeItem->Check(check->getState()); menuItems_[id] = item;
		}return nativeMenu;
	}
	wxMenu* WxFramePeer::buildSwingMenu(const ::jxx::Ptr<::jxx::swing::JMenu>& menu)
	{
		auto* nativeMenu = new wxMenu();
		if (!menu) return nativeMenu;
		for (::jxx::lang::jint i = 0; i < menu->getItemCount(); ++i)
		{
			auto item = menu->getItem(i);
			if (!item) { nativeMenu->AppendSeparator(); continue; }
			if (auto submenu = ::jxx::CAST<::jxx::swing::JMenu>(item))
			{ nativeMenu->AppendSubMenu(buildSwingMenu(submenu), nativeText(submenu->getText())); continue; }
			const int id = wxWindow::NewControlId();
			wxItemKind kind = ::jxx::CAST<::jxx::swing::JCheckBoxMenuItem>(item) ? wxITEM_CHECK
			    : (::jxx::CAST<::jxx::swing::JRadioButtonMenuItem>(item) ? wxITEM_RADIO : wxITEM_NORMAL);
			auto* nativeItem = nativeMenu->Append(id, nativeText(item->getText()), wxEmptyString, kind);
			nativeItem->Enable(item->isEnabled());
			if (auto check = ::jxx::CAST<::jxx::swing::JCheckBoxMenuItem>(item)) nativeItem->Check(check->isSelected());
			if (auto radio = ::jxx::CAST<::jxx::swing::JRadioButtonMenuItem>(item)) nativeItem->Check(radio->isSelected());
			swingMenuItems_[id] = item;
		}
		return nativeMenu;
	}
	void WxFramePeer::setSwingMenuBar(const ::jxx::Ptr<::jxx::swing::JMenuBar>& menuBar)
	{
		if (!frame_) return;
		swingMenuItems_.clear();
		auto* nativeBar = new wxMenuBar();
		if (menuBar) for (::jxx::lang::jint i = 0; i < menuBar->getMenuCount(); ++i)
		{ auto menu = menuBar->getMenu(i); if (menu) nativeBar->Append(buildSwingMenu(menu), nativeText(menu->getText())); }
		frame_->SetMenuBar(nativeBar);
	}
	void WxFramePeer::setMenuBar(const ::jxx::Ptr<::jxx::awt::MenuBar>& menuBar)
	{
		if (!frame_)return; menuItems_.clear(); auto* nativeBar = new wxMenuBar(); if (menuBar) {
			for (::jxx::lang::jint i = 0; i < menuBar->getMenuCount(); ++i) {
				auto menu = menuBar->getMenu(i); if (menu)nativeBar->Append(buildMenu(menu), nativeText(menu->getLabel()));
			}
		}frame_->SetMenuBar(nativeBar);
	}
	void WxFramePeer::dispatchMenu(::jxx::lang::jint id)
	{
		auto sit = swingMenuItems_.find(id); if (sit != swingMenuItems_.end()) { if (auto item = sit->second.lock()) { if (auto check = ::jxx::CAST<::jxx::swing::JCheckBoxMenuItem>(item)) check->setSelected(!check->isSelected()); if (auto radio = ::jxx::CAST<::jxx::swing::JRadioButtonMenuItem>(item)) radio->setSelected(true); item->doClick(); } return; } auto it = menuItems_.find(id); if (it == menuItems_.end())return; if (auto item = it->second.lock()) {
			if (auto check = ::jxx::CAST<::jxx::awt::CheckboxMenuItem>(item))check->setState(!check->getState()); item->fireActionPerformed();
		}
	}

	void WxFramePeer::installComponents(const ::jxx::Ptr<::jxx::awt::Container>& container)
	{
		if (!frame_ || !container)return; buildChildren(frame_, container);
	}
	void WxFramePeer::buildChildren(wxWindow* parent, const ::jxx::Ptr<::jxx::awt::Container>& container)
	{
		for (::jxx::lang::jint i = 0; i < container->getComponentCount(); ++i) {
			auto component = container->getComponent(i); if (!component || component->nativeComponent_)continue; wxWindow* native = nullptr; if (auto button = ::jxx::CAST<::jxx::swing::AbstractButton>(component)) {
                auto* control = new wxButton(parent, wxID_ANY, nativeText(button->getText())); auto peer = ::jxx::NEW<WxComponentPeer>(control); std::weak_ptr<::jxx::swing::AbstractButton> weak = button; peer->setActionCallback([weak]() { if (auto owner = weak.lock()) owner->fireActionPerformed(); }); component->setNativeComponentInternal(peer); native = control;
            }
            else if (auto label = ::jxx::CAST<::jxx::swing::JLabel>(component)) {
                auto* control = new wxStaticText(parent, wxID_ANY, nativeText(label->getText())); component->setNativeComponentInternal(::jxx::NEW<WxComponentPeer>(control)); native = control;
            }
            else if (auto field = ::jxx::CAST<::jxx::swing::JTextField>(component))
            {
                auto* control = new wxTextCtrl(parent, wxID_ANY, nativeText(field->getText()), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
                control->SetEditable(field->isEditable());
                auto peer = ::jxx::NEW<WxComponentPeer>(control);
                std::weak_ptr<::jxx::swing::JTextField> weak = field;
                peer->setActionCallback([weak] { if (auto owner = weak.lock()) owner->fireActionPerformed(); });
                component->setNativeComponentInternal(peer); native = control;
            }
            else if (auto area = ::jxx::CAST<::jxx::swing::JTextArea>(component))
            {
                long style = wxTE_MULTILINE;
                if (!area->getLineWrap()) style |= wxTE_DONTWRAP;
                auto* control = new wxTextCtrl(parent, wxID_ANY, nativeText(area->getText()), wxDefaultPosition, wxDefaultSize, style);
                control->SetEditable(area->isEditable());
                component->setNativeComponentInternal(::jxx::NEW<WxComponentPeer>(control)); native = control;
            }
            else if (auto button = ::jxx::CAST<::jxx::awt::Button>(component)) {
				auto* control = new wxButton(parent, wxID_ANY, nativeText(button->getLabel())); auto peer = ::jxx::NEW<WxComponentPeer>(control); std::weak_ptr<::jxx::awt::Button> weak = button; peer->setActionCallback([weak]()
{
	if (auto owner = weak.lock())owner->fireActionPerformed();
}); component->setNativeComponentInternal(peer); native = control;
			}
			else if (auto label = ::jxx::CAST<::jxx::awt::Label>(component)) {
				long style = label->getAlignment() == ::jxx::awt::Label::RIGHT ? wxALIGN_RIGHT : (label->getAlignment() == ::jxx::awt::Label::CENTER ? wxALIGN_CENTRE_HORIZONTAL : wxALIGN_LEFT); auto* control = new wxStaticText(parent, wxID_ANY, nativeText(label->getText()), wxDefaultPosition, wxDefaultSize, style | wxST_NO_AUTORESIZE); component->setNativeComponentInternal(::jxx::NEW<WxComponentPeer>(control)); native = control;
			}
			else if (auto field = ::jxx::CAST<::jxx::awt::TextField>(component)) {
				long style = wxTE_PROCESS_ENTER; if (field->echoCharIsSet())style |= wxTE_PASSWORD; auto* control = new wxTextCtrl(parent, wxID_ANY, nativeText(field->getText()), wxDefaultPosition, wxDefaultSize, style); control->SetEditable(field->isEditable()); auto peer = ::jxx::NEW<WxComponentPeer>(control); std::weak_ptr<::jxx::awt::TextField> weak = field; peer->setActionCallback([weak, control]()
{
	if (auto owner = weak.lock()) {
		owner->setText(::jxx::NEW<::jxx::lang::String>(control->GetValue().ToUTF8().data())); owner->fireActionPerformed();
	}
}); component->setNativeComponentInternal(peer); native = control;
			}
			else if (auto child = ::jxx::CAST<::jxx::awt::Container>(component)) {
				auto* panel = new wxPanel(parent, wxID_ANY); component->setNativeComponentInternal(::jxx::NEW<WxComponentPeer>(panel)); native = panel; buildChildren(panel, child);
			}if (native)native->Show(component->isVisible());
		}
	}
}
