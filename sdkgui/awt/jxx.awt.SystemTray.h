#pragma once
#include <vector>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::awt { class Dimension; class TrayIcon; class SystemTray : public ::jxx::lang::ClassBase<SystemTray,::jxx::lang::Object> { public: using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<SystemTray,JxxSuper>;static ::jxx::lang::jbool isSupported();static ::jxx::Ptr<SystemTray> getSystemTray();void add(const ::jxx::Ptr<TrayIcon>& icon);void remove(const ::jxx::Ptr<TrayIcon>& icon);::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<TrayIcon>,1>> getTrayIcons()const;::jxx::Ptr<Dimension> getTrayIconSize()const;private:SystemTray();std::vector<::jxx::Ptr<TrayIcon>> icons_;}; }
