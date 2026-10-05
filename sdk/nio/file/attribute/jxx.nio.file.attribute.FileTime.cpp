#include "nio/file/attribute/jxx.nio.file.attribute.FileTime.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::nio::file::attribute {
FileTime::FileTime(::jxx::lang::jlong v)noexcept:millis_(v){}
::jxx::Ptr<FileTime> FileTime::fromMillis(::jxx::lang::jlong v){return ::jxx::NEW<FileTime>(v);}
::jxx::lang::jlong FileTime::toMillis()const noexcept{return millis_;}
::jxx::lang::jint FileTime::compareTo(const ::jxx::Ptr<FileTime>&o)const{if(!o)throw ::jxx::lang::NullPointerException();return millis_<o->millis_?-1:(millis_>o->millis_?1:0);}
::jxx::lang::jbool FileTime::equals(const ::jxx::Ptr<::jxx::lang::Object>&o)const{auto f=::jxx::CAST<FileTime>(o);return f&&f->millis_==millis_;}
::jxx::lang::jint FileTime::hashCode()const{return static_cast<::jxx::lang::jint>(millis_^(millis_>>32));}
::jxx::Ptr<::jxx::lang::String> FileTime::toString()const{std::time_t t=static_cast<std::time_t>(millis_/1000);std::tm tm{};
#ifdef _WIN32
 gmtime_s(&tm,&t);
#else
 gmtime_r(&t,&tm);
#endif
 std::ostringstream s;s<<std::put_time(&tm,"%Y-%m-%dT%H:%M:%S");auto fraction=millis_%1000;if(fraction<0)fraction=-fraction;if(fraction!=0)s<<'.'<<std::setw(3)<<std::setfill('0')<<fraction;s<<'Z';return ::jxx::NEW<::jxx::lang::String>(s.str());}
}
