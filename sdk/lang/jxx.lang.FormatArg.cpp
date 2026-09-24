#include "lang/jxx.lang.FormatArg.h"

#include <cstdio>
#include <string>

namespace jxx::lang {
FormatArg::FormatArg(const String& s):v_(s){} FormatArg::FormatArg(const char* s):v_(String(s)){} FormatArg::FormatArg(long long i):v_(i){} FormatArg::FormatArg(int i):v_(static_cast<long long>(i)){} FormatArg::FormatArg(double d):v_(d){} FormatArg::FormatArg(float f):v_(static_cast<double>(f)){} FormatArg::FormatArg(bool b):v_(b){}
const FormatArg::Var& FormatArg::value()const{return v_;}
String toString(const FormatArg&a,int precision){const auto&v=a.value();if(std::holds_alternative<String>(v))return std::get<String>(v);if(std::holds_alternative<long long>(v))return String(std::to_string(std::get<long long>(v)));if(std::holds_alternative<double>(v)){const auto d=std::get<double>(v);if(precision>=0){char b[128];std::snprintf(b,sizeof(b),("% ."+std::to_string(precision)+"f").c_str(),d);return String(b);}return String(std::to_string(d));}if(std::holds_alternative<bool>(v))return String(std::get<bool>(v)?"true":"false");return String("");}
} // namespace jxx::lang
