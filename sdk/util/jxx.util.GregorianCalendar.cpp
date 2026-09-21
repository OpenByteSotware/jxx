#include "util/jxx.util.GregorianCalendar.h"

#include <algorithm>
#include <chrono>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::util {
namespace {
std::int64_t daysFromCivil(int y,unsigned m,unsigned d){y-=m<=2;const int era=(y>=0?y:y-399)/400;const unsigned yoe=static_cast<unsigned>(y-era*400);const unsigned doy=(153*(m+(m>2?-3:9))+2)/5+d-1;const unsigned doe=yoe*365+yoe/4-yoe/100+doy;return era*146097+static_cast<int>(doe)-719468;}
::jxx::lang::jlong nowMillis(){return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();}
}
GregorianCalendar::GregorianCalendar():GregorianCalendar(TimeZone::getDefault(),Locale::getDefault()){}
GregorianCalendar::GregorianCalendar(const ::jxx::Ptr<TimeZone>&z):GregorianCalendar(z,Locale::getDefault()){}
GregorianCalendar::GregorianCalendar(const ::jxx::Ptr<Locale>&l):GregorianCalendar(TimeZone::getDefault(),l){}
GregorianCalendar::GregorianCalendar(const ::jxx::Ptr<TimeZone>&z,const ::jxx::Ptr<Locale>&l):Calendar(),locale_(l),gregorianChange_(::jxx::NEW<Date>(-12219292800000LL)){if(!z||!l)throw ::jxx::lang::NullPointerException();setTimeZone(z);setTimeInMillis(nowMillis());}
GregorianCalendar::GregorianCalendar(::jxx::lang::jint y,::jxx::lang::jint m,::jxx::lang::jint d):GregorianCalendar(){setLocal_(y,m,d,0,0,0,0);}
GregorianCalendar::GregorianCalendar(::jxx::lang::jint y,::jxx::lang::jint m,::jxx::lang::jint d,::jxx::lang::jint h,::jxx::lang::jint min):GregorianCalendar(){setLocal_(y,m,d,h,min,0,0);}
GregorianCalendar::GregorianCalendar(::jxx::lang::jint y,::jxx::lang::jint m,::jxx::lang::jint d,::jxx::lang::jint h,::jxx::lang::jint min,::jxx::lang::jint s):GregorianCalendar(){setLocal_(y,m,d,h,min,s,0);}
::jxx::lang::jbool GregorianCalendar::isLeapYear(::jxx::lang::jint y)const{return y%4==0&&(y%100!=0||y%400==0);}
void GregorianCalendar::setGregorianChange(const ::jxx::Ptr<Date>&d){if(!d)throw ::jxx::lang::NullPointerException();gregorianChange_=::jxx::NEW<Date>(d->getTime());}
::jxx::Ptr<Date> GregorianCalendar::getGregorianChange()const{return ::jxx::NEW<Date>(gregorianChange_->getTime());}
::jxx::lang::jint GregorianCalendar::daysInMonth_(::jxx::lang::jint y,::jxx::lang::jint m){static const int days[]={31,28,31,30,31,30,31,31,30,31,30,31};if(m<0||m>11)throw ::jxx::lang::IllegalArgumentException();return days[m]+(m==1&&y%4==0&&(y%100!=0||y%400==0));}
::jxx::lang::jlong GregorianCalendar::localToEpoch_(::jxx::lang::jint y,::jxx::lang::jint m,::jxx::lang::jint d,::jxx::lang::jint h,::jxx::lang::jint min,::jxx::lang::jint s,::jxx::lang::jint ms,const ::jxx::Ptr<TimeZone>&z){auto local=daysFromCivil(y,m+1,d)*86400000LL+h*3600000LL+min*60000LL+s*1000LL+ms;auto candidate=local-z->getRawOffset();for(int i=0;i<3;++i)candidate=local-z->getOffset(candidate);return candidate;}
void GregorianCalendar::setLocal_(::jxx::lang::jint y,::jxx::lang::jint m,::jxx::lang::jint d,::jxx::lang::jint h,::jxx::lang::jint min,::jxx::lang::jint s,::jxx::lang::jint ms){if(m<0||m>11||d<1||d>daysInMonth_(y,m)||h<0||h>23||min<0||min>59||s<0||s>59||ms<0||ms>999)throw ::jxx::lang::IllegalArgumentException();setTimeInMillis(localToEpoch_(y,m,d,h,min,s,ms,getTimeZone()));}
void GregorianCalendar::set(::jxx::lang::jint y,::jxx::lang::jint m,::jxx::lang::jint d){setLocal_(y,m,d,get(HOUR_OF_DAY),get(MINUTE),get(SECOND),get(MILLISECOND));}
void GregorianCalendar::set(::jxx::lang::jint y,::jxx::lang::jint m,::jxx::lang::jint d,::jxx::lang::jint h,::jxx::lang::jint min,::jxx::lang::jint s){setLocal_(y,m,d,h,min,s,0);}
void GregorianCalendar::set(::jxx::lang::jint field,::jxx::lang::jint value){auto y=get(YEAR),m=get(MONTH),d=get(DAY_OF_MONTH),h=get(HOUR_OF_DAY),min=get(MINUTE),s=get(SECOND),ms=get(MILLISECOND);switch(field){case YEAR:y=value;break;case MONTH:m=value;break;case DAY_OF_MONTH:d=value;break;case HOUR_OF_DAY:h=value;break;case MINUTE:min=value;break;case SECOND:s=value;break;case MILLISECOND:ms=value;break;default:throw ::jxx::lang::IllegalArgumentException();}setLocal_(y,m,d,h,min,s,ms);}
void GregorianCalendar::add(::jxx::lang::jint field,::jxx::lang::jint amount){if(amount==0)return;if(field==MILLISECOND){setTimeInMillis(getTimeInMillis()+amount);return;}if(field==SECOND){setTimeInMillis(getTimeInMillis()+amount*1000LL);return;}if(field==MINUTE){setTimeInMillis(getTimeInMillis()+amount*60000LL);return;}if(field==HOUR_OF_DAY){setTimeInMillis(getTimeInMillis()+amount*3600000LL);return;}if(field==DAY_OF_MONTH){setTimeInMillis(getTimeInMillis()+amount*86400000LL);return;}auto y=get(YEAR),m=get(MONTH),d=get(DAY_OF_MONTH);if(field==YEAR)y+=amount;else if(field==MONTH){auto total=y*12+m+amount;y=total/12;m=total%12;if(m<0){m+=12;--y;}}else throw ::jxx::lang::IllegalArgumentException();d=std::min(d,daysInMonth_(y,m));setLocal_(y,m,d,get(HOUR_OF_DAY),get(MINUTE),get(SECOND),get(MILLISECOND));}
void GregorianCalendar::roll(::jxx::lang::jint field,::jxx::lang::jint amount){if(amount==0)return;auto min=getActualMinimum(field),max=getActualMaximum(field),value=get(field),range=max-min+1;value=min+((value-min+amount)%range+range)%range;set(field,value);}
::jxx::lang::jint GregorianCalendar::getActualMaximum(::jxx::lang::jint field)const{switch(field){case MONTH:return 11;case DAY_OF_MONTH:return daysInMonth_(get(YEAR),get(MONTH));case HOUR_OF_DAY:return 23;case MINUTE:case SECOND:return 59;case MILLISECOND:return 999;default:throw ::jxx::lang::IllegalArgumentException();}}
::jxx::lang::jint GregorianCalendar::getActualMinimum(::jxx::lang::jint field)const{switch(field){case MONTH:case HOUR_OF_DAY:case MINUTE:case SECOND:case MILLISECOND:return 0;case DAY_OF_MONTH:return 1;default:throw ::jxx::lang::IllegalArgumentException();}}
::jxx::Ptr<::jxx::lang::String> GregorianCalendar::getCalendarType()const{return ::jxx::NEW<::jxx::lang::String>("gregory");}
} // namespace jxx::util
