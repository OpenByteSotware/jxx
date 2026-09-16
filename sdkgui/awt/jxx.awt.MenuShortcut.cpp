#include "awt/jxx.awt.MenuShortcut.h"
#include <sstream>
namespace jxx::awt {
MenuShortcut::MenuShortcut(::jxx::lang::jint key):MenuShortcut(key,false){} MenuShortcut::MenuShortcut(::jxx::lang::jint key,::jxx::lang::jbool shift):key_(key),usesShift_(shift){}
::jxx::lang::jint MenuShortcut::getKey() const{return key_;} ::jxx::lang::jbool MenuShortcut::usesShiftModifier() const{return usesShift_;}
::jxx::lang::jbool MenuShortcut::equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const{auto value=::jxx::CAST<MenuShortcut>(object);return value&&value->key_==key_&&value->usesShift_==usesShift_;}
::jxx::lang::jint MenuShortcut::hashCode() const{return key_+(usesShift_?1:0);} ::jxx::Ptr<::jxx::lang::String> MenuShortcut::toString() const{std::ostringstream out;out<<"jxx.awt.MenuShortcut[key="<<key_<<",usesShift="<<(usesShift_?"true":"false")<<"]";return ::jxx::NEW<::jxx::lang::String>(out.str());}
}
