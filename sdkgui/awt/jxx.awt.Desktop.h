#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::io { class File; }
namespace jxx::net { class URI; }
namespace jxx::awt {
class Desktop : public ::jxx::lang::ClassBase<Desktop,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<Desktop,JxxSuper>;
 enum class Action { OPEN, EDIT, PRINT, MAIL, BROWSE };
 static ::jxx::lang::jbool isDesktopSupported(); static ::jxx::Ptr<Desktop> getDesktop();
 ::jxx::lang::jbool isSupported(Action action) const;
 void open(const ::jxx::Ptr<::jxx::io::File>& file); void edit(const ::jxx::Ptr<::jxx::io::File>& file); void print(const ::jxx::Ptr<::jxx::io::File>& file); void browse(const ::jxx::Ptr<::jxx::net::URI>& uri); void mail(); void mail(const ::jxx::Ptr<::jxx::net::URI>& mailtoURI);
private: Desktop();
}; }
