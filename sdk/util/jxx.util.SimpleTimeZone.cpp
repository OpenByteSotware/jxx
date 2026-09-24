#include "util/jxx.util.SimpleTimeZone.h"

#include <algorithm>
#include <cstdint>

#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.Date.h"

namespace jxx::util
{
	namespace
	{
		bool leap(int y)
		{
			return y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
		}
		int monthDays(int y, int m)
		{
			static const int d[] = { 31,28,31,30,31,30,31,31,30,31,30,31 }; return d[m] + (m == 1 && leap(y));
		}
		std::int64_t daysFromCivil(int y, unsigned m, unsigned d)
		{
			y -= m <= 2; const int era = (y >= 0 ? y : y - 399) / 400; const unsigned yoe = static_cast<unsigned>(y - era * 400); const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1; const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy; return era * 146097 + static_cast<int>(doe) - 719468;
		}
		void civilFromDays(std::int64_t z, int& y, unsigned& m, unsigned& d)
		{
			z += 719468; 
			const auto era = (z >= 0 ? z : z - 146096) / 146097; 
			const auto doe = static_cast<unsigned>(z - era * 146097);
			const auto yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
			y = static_cast<int>(yoe) + static_cast<int>(era) * 400;
			const auto doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
			
			const auto mp = (5 * doy + 2) / 153; d = doy - (153 * mp + 2) / 5 + 1;
			m = mp + (mp < 10 ? 3 : -9); y += m <= 2;
		}
		int dayOfWeek(int y, int m, int d)
		{
			auto days = daysFromCivil(y, m + 1, d); int value = static_cast<int>((days + 4) % 7); if (value < 0)value += 7; return value + 1;
		}
		template <typename RuleType> int resolvedDay(int y, const RuleType& r)
		{
			const int dim = monthDays(y, r.month); if (r.dayOfWeek == 0)return std::min(std::abs(r.day), dim); if (r.day < 0) {
				int base = std::min(-r.day, dim); int dow = dayOfWeek(y, r.month, base); int wanted = std::abs(r.dayOfWeek); return base - ((dow - wanted + 7) % 7);
			}int base = std::min(r.day, dim); int dow = dayOfWeek(y, r.month, base); int wanted = std::abs(r.dayOfWeek); return base + ((wanted - dow + 7) % 7);
		}
	}
	SimpleTimeZone::SimpleTimeZone(::jxx::lang::jint raw, const ::jxx::Ptr<::jxx::lang::String>& id) :id_(id), rawOffset_(raw)
	{
		if (!id_)throw ::jxx::lang::NullPointerException();
	}
	SimpleTimeZone::SimpleTimeZone(::jxx::lang::jint raw, const ::jxx::Ptr<::jxx::lang::String>& id, ::jxx::lang::jint sm, ::jxx::lang::jint sd, ::jxx::lang::jint sw, ::jxx::lang::jint st, ::jxx::lang::jint em, ::jxx::lang::jint ed, ::jxx::lang::jint ew, ::jxx::lang::jint et) :SimpleTimeZone(raw, id, sm, sd, sw, st, WALL_TIME, em, ed, ew, et, WALL_TIME, 3600000)
	{
	}
	SimpleTimeZone::SimpleTimeZone(::jxx::lang::jint raw, const ::jxx::Ptr<::jxx::lang::String>& id, ::jxx::lang::jint sm, ::jxx::lang::jint sd, ::jxx::lang::jint sw, ::jxx::lang::jint st, ::jxx::lang::jint stm, ::jxx::lang::jint em, ::jxx::lang::jint ed, ::jxx::lang::jint ew, ::jxx::lang::jint et, ::jxx::lang::jint etm, ::jxx::lang::jint dst) :SimpleTimeZone(raw, id)
	{
		start_ = { sm,sd,sw,st,stm }; end_ = { em,ed,ew,et,etm }; validateRule_(start_); validateRule_(end_); setDSTSavings(dst); usesDaylight_ = true;
	}
	void SimpleTimeZone::validateRule_(const Rule& r)
	{
		if (r.month < 0 || r.month>11 || r.day == 0 || std::abs(r.day) > 31 || std::abs(r.dayOfWeek) > 7 || r.time < 0 || r.time >= 86400000 || r.mode < 0 || r.mode>2)throw ::jxx::lang::IllegalArgumentException();
	}
	::jxx::lang::jlong SimpleTimeZone::transition_(::jxx::lang::jint y, const Rule& r, ::jxx::lang::jint raw, ::jxx::lang::jint daylightBefore)
	{
		auto local = daysFromCivil(y, r.month + 1, resolvedDay(y, r)) * 86400000LL + r.time; auto correction = r.mode == UTC_TIME ? 0 : raw + (r.mode == WALL_TIME ? daylightBefore : 0); return local - correction;
	}
	::jxx::Ptr<::jxx::lang::String> SimpleTimeZone::getID()const
	{
		return id_;
	}
	::jxx::lang::jint SimpleTimeZone::getOffset(::jxx::lang::jlong epoch)const
	{
		if (!usesDaylight_)return rawOffset_; auto localDays = (epoch + rawOffset_) / 86400000LL; int y; unsigned m, d; civilFromDays(localDays, y, m, d); if (y < startYear_)return rawOffset_; auto start = transition_(y, start_, rawOffset_, 0); auto end = transition_(y, end_, rawOffset_, dstSavings_); bool active = start < end ? (epoch >= start && epoch < end) : (epoch >= start || epoch < end); return rawOffset_ + (active ? dstSavings_ : 0);
	}
	::jxx::Ptr<::jxx::lang::String> SimpleTimeZone::getAbbreviation(::jxx::lang::jlong)const
	{
		return id_;
	}
	::jxx::lang::jint SimpleTimeZone::getRawOffset()const
	{
		return rawOffset_;
	}void SimpleTimeZone::setRawOffset(::jxx::lang::jint v)
	{
		rawOffset_ = v;
	}
	::jxx::lang::jbool SimpleTimeZone::useDaylightTime()const
	{
		return usesDaylight_;
	}
	::jxx::lang::jbool SimpleTimeZone::inDaylightTime(const ::jxx::Ptr<Date>& d)const
	{
		if (!d)throw ::jxx::lang::NullPointerException(); return getOffset(d->getTime()) != rawOffset_;
	}
	::jxx::lang::jint SimpleTimeZone::getDSTSavings()const
	{
		return usesDaylight_ ? dstSavings_ : 0;
	}void SimpleTimeZone::setDSTSavings(::jxx::lang::jint v)
	{
		if (v <= 0)throw ::jxx::lang::IllegalArgumentException(); dstSavings_ = v;
	}
	void SimpleTimeZone::setStartYear(::jxx::lang::jint y)
	{
		startYear_ = y;
	}
	void SimpleTimeZone::setStartRule(::jxx::lang::jint m, ::jxx::lang::jint d, ::jxx::lang::jint w, ::jxx::lang::jint t)
	{
		Rule r{ m,d,w,t,WALL_TIME }; validateRule_(r); start_ = r; usesDaylight_ = true;
	}
	void SimpleTimeZone::setEndRule(::jxx::lang::jint m, ::jxx::lang::jint d, ::jxx::lang::jint w, ::jxx::lang::jint t)
	{
		Rule r{ m,d,w,t,WALL_TIME }; validateRule_(r); end_ = r; usesDaylight_ = true;
	}
	::jxx::lang::jbool SimpleTimeZone::hasSameRules(const ::jxx::Ptr<TimeZone>& o)const
	{
		auto v = ::jxx::CAST<SimpleTimeZone>(o); return v && rawOffset_ == v->rawOffset_ && dstSavings_ == v->dstSavings_ && usesDaylight_ == v->usesDaylight_ && start_.month == v->start_.month && start_.day == v->start_.day && start_.dayOfWeek == v->start_.dayOfWeek && start_.time == v->start_.time && end_.month == v->end_.month && end_.day == v->end_.day && end_.dayOfWeek == v->end_.dayOfWeek && end_.time == v->end_.time;
	}
	::jxx::lang::jbool SimpleTimeZone::equals(const ::jxx::Ptr<::jxx::lang::Object>& o)const
	{
		auto v = ::jxx::CAST<SimpleTimeZone>(o); return v && id_->equals(v->id_) && hasSameRules(v);
	}
	::jxx::lang::jint SimpleTimeZone::hashCode()const
	{
		return id_->hashCode() ^ rawOffset_ ^ dstSavings_;
	}
} // namespace jxx::util
