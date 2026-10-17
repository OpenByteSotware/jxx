#include "util/jxx.util.GregorianCalendar.h"
#include "io/jxx.io.InvalidObjectException.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"

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
GregorianCalendar::GregorianCalendar(const ::jxx::Ptr<TimeZone>&z,const ::jxx::Ptr<Locale>&l):Super(),locale_(l),gregorianChange_(::jxx::NEW<Date>(-12219292800000LL)){if(!z||!l)throw ::jxx::lang::NullPointerException();setTimeZone(z);setTimeInMillis(nowMillis());}
GregorianCalendar::GregorianCalendar(::jxx::lang::jint y,::jxx::lang::jint m,::jxx::lang::jint d):GregorianCalendar(){setLocal_(y,m,d,0,0,0,0);}
GregorianCalendar::GregorianCalendar(::jxx::lang::jint y,::jxx::lang::jint m,::jxx::lang::jint d,::jxx::lang::jint h,::jxx::lang::jint min):GregorianCalendar(){setLocal_(y,m,d,h,min,0,0);}
GregorianCalendar::GregorianCalendar(::jxx::lang::jint y,::jxx::lang::jint m,::jxx::lang::jint d,::jxx::lang::jint h,::jxx::lang::jint min,::jxx::lang::jint s):GregorianCalendar(){setLocal_(y,m,d,h,min,s,0);}
::jxx::lang::jbool GregorianCalendar::isLeapYear(::jxx::lang::jint y)const{return y%4==0&&(y%100!=0||y%400==0);}
void GregorianCalendar::setGregorianChange(
    const ::jxx::Ptr<Date>& date) {
    if (date == nullptr) throw ::jxx::lang::NullPointerException();
    gregorianChange_ = ::jxx::NEW<Date>(date->getTime());
}

::jxx::Ptr<Date> GregorianCalendar::getGregorianChange() const {
    return gregorianChange_ == nullptr
        ? nullptr
        : ::jxx::NEW<Date>(gregorianChange_->getTime());
}
::jxx::lang::jint GregorianCalendar::daysInMonth_(::jxx::lang::jint y,::jxx::lang::jint m){static const int days[]={31,28,31,30,31,30,31,31,30,31,30,31};if(m<0||m>11)throw ::jxx::lang::IllegalArgumentException();return days[m]+(m==1&&y%4==0&&(y%100!=0||y%400==0));}
::jxx::lang::jlong GregorianCalendar::localToEpoch_(::jxx::lang::jint y,::jxx::lang::jint m,::jxx::lang::jint d,::jxx::lang::jint h,::jxx::lang::jint min,::jxx::lang::jint s,::jxx::lang::jint ms,const ::jxx::Ptr<TimeZone>&z){auto local=daysFromCivil(y,m+1,d)*86400000LL+h*3600000LL+min*60000LL+s*1000LL+ms;auto candidate=local-z->getRawOffset();for(int i=0;i<3;++i)candidate=local-z->getOffset(candidate);return candidate;}
void GregorianCalendar::setLocal_(
    ::jxx::lang::jint year,
    ::jxx::lang::jint month,
    ::jxx::lang::jint day,
    ::jxx::lang::jint hour,
    ::jxx::lang::jint minute,
    ::jxx::lang::jint second,
    ::jxx::lang::jint millisecond) {

    if (!isLenient()) {
        if (month < JANUARY || month > DECEMBER ||
            day < 1 || day > daysInMonth_(year, month) ||
            hour < 0 || hour > 23 || minute < 0 || minute > 59 ||
            second < 0 || second > 59 ||
            millisecond < 0 || millisecond > 999) {
            throw ::jxx::lang::IllegalArgumentException();
        }
        setTimeInMillis(localToEpoch_(
            year, month, day, hour, minute, second, millisecond,
            getTimeZone()));
        return;
    }

    const auto floorDivide = [](::jxx::lang::jlong value,
                                ::jxx::lang::jlong divisor) {
        auto quotient = value / divisor;
        const auto remainder = value % divisor;
        if (remainder != 0 && ((remainder < 0) != (divisor < 0))) {
            --quotient;
        }
        return quotient;
    };

    const auto totalMonths =
        static_cast<::jxx::lang::jlong>(year) * 12LL + month;
    const auto normalizedYear = floorDivide(totalMonths, 12LL);
    const auto normalizedMonth = totalMonths - normalizedYear * 12LL;

    const auto timeOfDay =
        static_cast<::jxx::lang::jlong>(hour) * 3600000LL +
        static_cast<::jxx::lang::jlong>(minute) * 60000LL +
        static_cast<::jxx::lang::jlong>(second) * 1000LL + millisecond;
    const auto dayCarry = floorDivide(timeOfDay, 86400000LL);
    const auto normalizedTime = timeOfDay - dayCarry * 86400000LL;

    const auto baseDay = daysFromCivil(
        static_cast<int>(normalizedYear),
        static_cast<unsigned>(normalizedMonth + 1), 1U);
    const auto localMillis =
        (baseDay + static_cast<::jxx::lang::jlong>(day) - 1LL + dayCarry) *
            86400000LL +
        normalizedTime;

    const auto zone = getTimeZone();
    auto epochMillis = localMillis - zone->getRawOffset();
    for (int iteration = 0; iteration < 4; ++iteration) {
        const auto adjusted = localMillis - zone->getOffset(epochMillis);
        if (adjusted == epochMillis) break;
        epochMillis = adjusted;
    }
    setTimeInMillis(epochMillis);
}
void GregorianCalendar::set(::jxx::lang::jint y,::jxx::lang::jint m,::jxx::lang::jint d){setLocal_(y,m,d,get(HOUR_OF_DAY),get(MINUTE),get(SECOND),get(MILLISECOND));}
void GregorianCalendar::set(::jxx::lang::jint y,::jxx::lang::jint m,::jxx::lang::jint d,::jxx::lang::jint h,::jxx::lang::jint min,::jxx::lang::jint s){setLocal_(y,m,d,h,min,s,0);}
void GregorianCalendar::set(::jxx::lang::jint field,::jxx::lang::jint value){auto y=get(YEAR),m=get(MONTH),d=get(DAY_OF_MONTH),h=get(HOUR_OF_DAY),min=get(MINUTE),s=get(SECOND),ms=get(MILLISECOND);switch(field){case YEAR:y=value;break;case MONTH:m=value;break;case DAY_OF_MONTH:d=value;break;case HOUR_OF_DAY:h=value;break;case MINUTE:min=value;break;case SECOND:s=value;break;case MILLISECOND:ms=value;break;default:throw ::jxx::lang::IllegalArgumentException();}setLocal_(y,m,d,h,min,s,ms);}
void GregorianCalendar::add(
    ::jxx::lang::jint field,
    ::jxx::lang::jint amount) {

    if (amount == 0) return;

    switch (field) {
        case MILLISECOND:
            setTimeInMillis(getTimeInMillis() + amount);
            return;
        case SECOND:
            setTimeInMillis(getTimeInMillis() +
                            static_cast<::jxx::lang::jlong>(amount) * 1000LL);
            return;
        case MINUTE:
            setTimeInMillis(getTimeInMillis() +
                            static_cast<::jxx::lang::jlong>(amount) * 60000LL);
            return;
        case HOUR:
        case HOUR_OF_DAY:
            setTimeInMillis(getTimeInMillis() +
                            static_cast<::jxx::lang::jlong>(amount) * 3600000LL);
            return;
        case AM_PM:
            setTimeInMillis(getTimeInMillis() +
                            static_cast<::jxx::lang::jlong>(amount) * 43200000LL);
            return;
        default:
            break;
    }

    auto year = get(YEAR);
    auto month = get(MONTH);
    auto day = get(DAY_OF_MONTH);

    switch (field) {
        case YEAR:
            year += amount;
            day = std::min(day, daysInMonth_(year, month));
            break;
        case MONTH: {
            const auto totalMonths =
                static_cast<::jxx::lang::jlong>(year) * 12LL + month + amount;
            auto quotient = totalMonths / 12LL;
            auto remainder = totalMonths % 12LL;
            if (remainder < 0) {
                remainder += 12LL;
                --quotient;
            }
            year = static_cast<::jxx::lang::jint>(quotient);
            month = static_cast<::jxx::lang::jint>(remainder);
            day = std::min(day, daysInMonth_(year, month));
            break;
        }
        case WEEK_OF_YEAR:
        case WEEK_OF_MONTH:
        case DAY_OF_WEEK_IN_MONTH:
            day += amount * 7;
            break;
        case DAY_OF_MONTH:
        case DAY_OF_YEAR:
        case DAY_OF_WEEK:
            day += amount;
            break;
        default:
            throw ::jxx::lang::IllegalArgumentException();
    }

    setLocal_(year, month, day,
              get(HOUR_OF_DAY), get(MINUTE), get(SECOND), get(MILLISECOND));
}
void GregorianCalendar::roll(
    ::jxx::lang::jint field,
    ::jxx::lang::jint amount) {

    if (amount == 0) return;

    const auto wrap = [amount](::jxx::lang::jint value,
                               ::jxx::lang::jint minimum,
                               ::jxx::lang::jint maximum) {
        const auto range = maximum - minimum + 1;
        auto normalized = (value - minimum + amount) % range;
        if (normalized < 0) normalized += range;
        return minimum + normalized;
    };

    const auto year = get(YEAR);
    const auto month = get(MONTH);
    const auto day = get(DAY_OF_MONTH);
    const auto hour = get(HOUR_OF_DAY);
    const auto minute = get(MINUTE);
    const auto second = get(SECOND);
    const auto millisecond = get(MILLISECOND);

    switch (field) {
        case YEAR: {
            const auto rolledYear = wrap(
                year, getActualMinimum(YEAR), getActualMaximum(YEAR));
            setLocal_(rolledYear, month,
                      std::min(day, daysInMonth_(rolledYear, month)),
                      hour, minute, second, millisecond);
            return;
        }
        case MONTH: {
            const auto rolledMonth = wrap(month, JANUARY, DECEMBER);
            setLocal_(year, rolledMonth,
                      std::min(day, daysInMonth_(year, rolledMonth)),
                      hour, minute, second, millisecond);
            return;
        }
        case DAY_OF_MONTH: {
            const auto rolledDay = wrap(
                day, 1, daysInMonth_(year, month));
            setLocal_(year, month, rolledDay,
                      hour, minute, second, millisecond);
            return;
        }
        case HOUR:
        case HOUR_OF_DAY: {
            const auto rolledHour = wrap(hour, 0, 23);
            setLocal_(year, month, day,
                      rolledHour, minute, second, millisecond);
            return;
        }
        case AM_PM: {
            const auto half = get(AM_PM);
            const auto rolledHalf = wrap(half, AM, PM);
            const auto rolledHour = (hour % 12) + rolledHalf * 12;
            setLocal_(year, month, day,
                      rolledHour, minute, second, millisecond);
            return;
        }
        case MINUTE:
            setLocal_(year, month, day, hour,
                      wrap(minute, 0, 59), second, millisecond);
            return;
        case SECOND:
            setLocal_(year, month, day, hour,
                      minute, wrap(second, 0, 59), millisecond);
            return;
        case MILLISECOND:
            setLocal_(year, month, day, hour,
                      minute, second, wrap(millisecond, 0, 999));
            return;
        default:
            throw ::jxx::lang::IllegalArgumentException();
    }
}

void GregorianCalendar::roll(
    ::jxx::lang::jint field,
    ::jxx::lang::jbool up) {
    roll(field, up ? 1 : -1);
}
::jxx::lang::jint GregorianCalendar::getActualMaximum(
    ::jxx::lang::jint field) const {
    switch (field) {
        case DAY_OF_MONTH:
            return daysInMonth_(get(YEAR), get(MONTH));
        case DAY_OF_YEAR:
            return isLeapYear(get(YEAR)) ? 366 : 365;
        case WEEK_OF_MONTH: {
            const auto lastDay = daysInMonth_(get(YEAR), get(MONTH));
            const auto saved = get(DAY_OF_MONTH);
            const_cast<GregorianCalendar*>(this)->set(DAY_OF_MONTH, lastDay);
            const auto result = get(WEEK_OF_MONTH);
            const_cast<GregorianCalendar*>(this)->set(DAY_OF_MONTH, saved);
            return result;
        }
        case WEEK_OF_YEAR: {
            const auto year = get(YEAR);
            const auto savedMonth = get(MONTH);
            const auto savedDay = get(DAY_OF_MONTH);
            const_cast<GregorianCalendar*>(this)->set(year, DECEMBER, 31);
            auto result = get(WEEK_OF_YEAR);
            if (result == 0) result = 52;
            const_cast<GregorianCalendar*>(this)->set(year, savedMonth, savedDay);
            return result;
        }
        case DAY_OF_WEEK_IN_MONTH:
            return (daysInMonth_(get(YEAR), get(MONTH)) + 6) / 7;
        default:
            return Calendar::getMaximum(field);
    }
}

::jxx::lang::jint GregorianCalendar::getActualMinimum(
    ::jxx::lang::jint field) const {
    return Calendar::getMinimum(field);
}
::jxx::Ptr<::jxx::lang::String> GregorianCalendar::getCalendarType()const{return ::jxx::NEW<::jxx::lang::String>("gregory");}

void GregorianCalendar::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) {
    if (out == nullptr) throw ::jxx::lang::NullPointerException();
    Calendar::writeObject(out);
    if (locale_ == nullptr || gregorianChange_ == nullptr) {
        throw ::jxx::io::InvalidObjectException(
            ::jxx::NEW<::jxx::lang::String>(
                "GregorianCalendar locale or change date is null"));
    }
    out->writeObject(::jxx::CAST<::jxx::lang::Object>(
        locale_->toLanguageTag()));
    out->writeLong(gregorianChange_->getTime());
}

void GregorianCalendar::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) {
    if (in == nullptr) throw ::jxx::lang::NullPointerException();
    Calendar::readObject(in);
    const auto languageTag =
        ::jxx::CAST<::jxx::lang::String>(in->readObject());
    if (languageTag == nullptr) {
        throw ::jxx::io::InvalidObjectException(
            ::jxx::NEW<::jxx::lang::String>(
                "GregorianCalendar locale tag is null"));
    }
    locale_ = Locale::forLanguageTag(languageTag);
    if (locale_ == nullptr) {
        throw ::jxx::io::InvalidObjectException(
            ::jxx::NEW<::jxx::lang::String>(
                "GregorianCalendar locale is invalid"));
    }
    gregorianChange_ = ::jxx::NEW<Date>(in->readLong());
}
void GregorianCalendar::readObjectNoData() { Calendar::readObjectNoData(); locale_=Locale::getDefault(); gregorianChange_=::jxx::NEW<Date>(-12219292800000LL); }

} // namespace jxx::util
