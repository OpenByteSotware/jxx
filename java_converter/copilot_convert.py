#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
generate_jGenerates Java-8-parity timezone + formatter + GoogleTest suite for jxx.generate_jxx_java8_tz_formatter_gtest_full.py

Windows:
  - Requires env var JXX_TIME_ZONE_INFO pointing at a zoneinfo directory
  - Default timezone uses: %JXX_TIME_ZONE_INFO%/localtime  (TZif file)

Linux / embedded Linux:
  - Uses /usr/share/zoneinfo by default
  - Default timezone uses /etc/localtime:
      * if symlink into zoneinfo => derive IANA ID
      * else load file directly

Assumptions about your existing runtime:
  - jxx_types.h defines jxx::Ptr<T> as std::shared_ptr<T> and jint/jlong/jbool etc.
  - jxx.lang.Object.h exists
  - jxx.lang.String.h exists and provides:
        String(const char*), std::string utf8() const
  - jxx.lang.Locale exists and provides:
        const std::locale& cppLocale() const
  - Wrapper types exist with Java signatures (jint/jlong variants):
        jxx::lang::Integer::intValue()
        jxx::lang::Long::longValue()
        jxx::lang::Short::shortValue()
        jxx::lang::Byte::byteValue()
        jxx::lang::Boolean::booleanValue()
        jxx::lang::Float::floatValue()
        jxx::lang::Double::doubleValue()
        jxx::lang::Character::charValue()
  - Exception base exists:
        jxx::lang::RuntimeException (or adjust includes below)
    If not, the formatter exceptions included here can be pointed at your existing base.

Usage:
  python generate_jxx_java8_tz_formatter_gtest_full.py --out ./jxx_java8_tz_pack --zip

"""

from __future__ import annotations
import argparse
from pathlib import Path
import zipfile

def write_file(path: Path, content: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8")

def zip_folder(zip_path: Path, folder: Path) -> None:
    with zipfile.ZipFile(zip_path, "w", compression=zipfile.ZIP_DEFLATED) as z:
        for p in folder.rglob("*"):
            if p.is_file():
                z.write(p, arcname=str(p.relative_to(folder.parent)))

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default="jxx_java8_tz_pack", help="output directory")
    ap.add_argument("--zip", action="store_true", help="also create a zip next to the folder")
    args = ap.parse_args()

    root = Path(args.out).resolve()

    # ---------------- tzif ----------------
    write_file(root/"jxx/util/tz/tzif.h", r'''#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace jxx::util::tz {

struct Transition {
    int64_t at_utc;      // UTC seconds
    int32_t offset;      // total offset seconds AFTER transition
    bool is_dst;
    std::string abbrev;
};

struct Zone {
    std::string id;
    int32_t default_offset = 0;
    bool default_is_dst = false;
    std::string default_abbrev;
    std::vector<Transition> transitions; // sorted
};

// Parse a TZif v2/v3 file (RFC 8536) and populate Zone.
bool load_tzif(const std::string& path, const std::string& zone_id, Zone& out);

} // namespace jxx::util::tz
''')

    write_file(root/"jxx/util/tz/tzif.cpp", r'''#include "jxx.util.tz.tzif.h"

#include <fstream>
#include <cstring>

namespace jxx::util::tz {

static int32_t read_i32(std::ifstream& f) {
    unsigned char b[4];
    f.read(reinterpret_cast<char*>(b), 4);
    return (int32_t)((uint32_t(b[0])<<24) | (uint32_t(b[1])<<16) | (uint32_t(b[2])<<8) | uint32_t(b[3]));
}

static int64_t read_i64(std::ifstream& f) {
    unsigned char b[8];
    f.read(reinterpret_cast<char*>(b), 8);
    int64_t v = 0;
    for (int i=0;i<8;i++) v = (v<<8) | int64_t(b[i]);
    return v;
}

struct Counts { int32_t ttisgmtcnt, ttisstdcnt, leapcnt, timecnt, typecnt, charcnt; };

static Counts read_counts(std::ifstream& f) {
    Counts c;
    c.ttisgmtcnt = read_i32(f);
    c.ttisstdcnt = read_i32(f);
    c.leapcnt    = read_i32(f);
    c.timecnt    = read_i32(f);
    c.typecnt    = read_i32(f);
    c.charcnt    = read_i32(f);
    return c;
}

bool load_tzif(const std::string& path, const std::string& zone_id, Zone& out) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return false;

    char magic[4];
    f.read(magic, 4);
    if (std::strncmp(magic, "TZif", 4) != 0) return false;

    char version = '\0';
    f.read(&version, 1);
    f.ignore(15);

    Counts c1 = read_counts(f);

    // Skip v1 block
    f.ignore((std::streamsize)c1.timecnt * 4);
    f.ignore((std::streamsize)c1.timecnt);
    f.ignore((std::streamsize)c1.typecnt * 6);
    f.ignore((std::streamsize)c1.charcnt);
    f.ignore((std::streamsize)c1.leapcnt * 8);
    f.ignore((std::streamsize)c1.ttisstdcnt);
    f.ignore((std::streamsize)c1.ttisgmtcnt);

    // v2/v3 block (64-bit transition times)
    Counts c = read_counts(f);

    std::vector<int64_t> times(c.timecnt);
    for (int i=0;i<c.timecnt;i++) times[i] = read_i64(f);

    std::vector<unsigned char> indices(c.timecnt);
    if (c.timecnt) f.read(reinterpret_cast<char*>(indices.data()), c.timecnt);

    struct TT { int32_t off; unsigned char isdst; unsigned char ab; };
    std::vector<TT> tt(c.typecnt);
    for (int i=0;i<c.typecnt;i++) {
        tt[i].off = read_i32(f);
        f.read(reinterpret_cast<char*>(&tt[i].isdst), 1);
        f.read(reinterpret_cast<char*>(&tt[i].ab), 1);
    }

    std::string abbrev(c.charcnt, '\0');
    if (c.charcnt) f.read(abbrev.data(), c.charcnt);

    out = Zone{};
    out.id = zone_id;
    out.transitions.clear();
    out.transitions.reserve(times.size());

    for (int i=0;i<c.timecnt;i++) {
        unsigned char tti = indices[i];
        if (tti >= tt.size()) continue;
        Transition tr;
        tr.at_utc = times[i];
        tr.offset = tt[tti].off;
        tr.is_dst = tt[tti].isdst != 0;
        tr.abbrev = &abbrev[tt[tti].ab];
        out.transitions.push_back(std::move(tr));
    }

    // Default type: first non-DST or type 0
    bool found = false;
    for (int i=0;i<c.typecnt;i++) {
        if (tt[i].isdst == 0) {
            out.default_offset = tt[i].off;
            out.default_is_dst = false;
            out.default_abbrev = &abbrev[tt[i].ab];
            found = true;
            break;
        }
    }
    if (!found && c.typecnt > 0) {
        out.default_offset = tt[0].off;
        out.default_is_dst = tt[0].isdst != 0;
        out.default_abbrev = &abbrev[tt[0].ab];
    }

    return true;
}

} // namespace jxx::util::tz
''')

    # ---------------- TimeZone ----------------
    write_file(root/"jxx/util/TimeZone.h", r'''#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"

namespace jxx::util {

class Date;

// Java-like TimeZone backed by TZif zoneinfo.
class TimeZone : public jxx::lang::Object {
public:
    virtual ~TimeZone() = default;

    virtual jxx::Ptr<jxx::lang::String> getID() const = 0;

    // Total offset (raw + dst) at instant, in milliseconds.
    virtual jint getOffset(jlong epochMillis) const = 0;

    // Abbreviation at instant (e.g., EST/EDT).
    virtual jxx::Ptr<jxx::lang::String> getAbbreviation(jlong epochMillis) const = 0;

    virtual jint getRawOffset() const = 0;
    virtual jbool useDaylightTime() const = 0;
    virtual jbool inDaylightTime(jxx::Ptr<Date> d) const = 0;

    static jxx::Ptr<TimeZone> getTimeZone(jxx::Ptr<jxx::lang::String> id);
    static jxx::Ptr<TimeZone> getDefault();
};

} // namespace jxx::util
''')

    write_file(root/"jxx/util/TimeZone.cpp", r'''#include "jxx.util.TimeZone.h"
#include "jxx.util.Date.h"
#include "jxx.util.tz.tzif.h"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <mutex>
#include <string>
#include <unordered_map>

namespace jxx::util {

using jxx::util::tz::Zone;
using jxx::util::tz::Transition;
using jxx::util::tz::load_tzif;

static std::mutex g_lock;
static std::unordered_map<std::string, Zone> g_zoneCache;

static std::string base_dir() {
    if (const char* e = std::getenv("JXX_TIME_ZONE_INFO")) {
        return std::string(e);
    }
#ifdef __linux__
    return std::string("/usr/share/zoneinfo");
#else
    return std::string(".");
#endif
}

static inline jint parse_gmt_offset_millis(const std::string& s) {
    // Accept GMT+hh:mm, GMT-hh:mm, UTC+..., UTC-...
    if (s.size() < 4) return 0;
    std::size_t pos = 3;
    if (pos >= s.size()) return 0;
    if (s[pos] != '+' && s[pos] != '-') return 0;
    int sign = (s[pos] == '-') ? -1 : 1;
    ++pos;
    if (pos + 1 >= s.size()) return 0;
    int hh = std::stoi(s.substr(pos,2));
    pos += 2;
    if (pos < s.size() && s[pos] == ':') ++pos;
    int mm = 0;
    if (pos + 1 < s.size()) mm = std::stoi(s.substr(pos,2));
    return (jint)(sign * ((hh*60 + mm) * 60 * 1000));
}

class FixedOffsetTimeZone final : public TimeZone {
    std::string id_;
    jint off_ms_ = 0;
public:
    FixedOffsetTimeZone(std::string id, jint off_ms) : id_(std::move(id)), off_ms_(off_ms) {}
    jxx::Ptr<jxx::lang::String> getID() const override { return std::make_shared<jxx::lang::String>(id_.c_str()); }
    jint getOffset(jlong /*epochMillis*/) const override { return off_ms_; }
    jxx::Ptr<jxx::lang::String> getAbbreviation(jlong /*epochMillis*/) const override {
        // Java often uses GMT+hh:mm for fixed offsets; keep ID.
        return std::make_shared<jxx::lang::String>(id_.c_str());
    }
    jint getRawOffset() const override { return off_ms_; }
    jbool useDaylightTime() const override { return false; }
    jbool inDaylightTime(jxx::Ptr<Date> /*d*/) const override { return false; }
};

class TzdbTimeZone final : public TimeZone {
    Zone z_;
public:
    explicit TzdbTimeZone(Zone z) : z_(std::move(z)) {}

    jxx::Ptr<jxx::lang::String> getID() const override {
        return std::make_shared<jxx::lang::String>(z_.id.c_str());
    }

    jint getOffset(jlong epochMillis) const override {
        int64_t sec = (int64_t)(epochMillis / 1000);
        const auto& t = z_.transitions;
        if (t.empty()) return (jint)(z_.default_offset * 1000);
        auto it = std::upper_bound(t.begin(), t.end(), sec,
            [](int64_t v, const Transition& tr){ return v < tr.at_utc; });
        if (it == t.begin()) return (jint)(z_.default_offset * 1000);
        --it;
        return (jint)(it->offset * 1000);
    }

    jxx::Ptr<jxx::lang::String> getAbbreviation(jlong epochMillis) const override {
        int64_t sec = (int64_t)(epochMillis / 1000);
        const auto& t = z_.transitions;
        if (t.empty()) return std::make_shared<jxx::lang::String>(z_.default_abbrev.c_str());
        auto it = std::upper_bound(t.begin(), t.end(), sec,
            [](int64_t v, const Transition& tr){ return v < tr.at_utc; });
        if (it == t.begin()) return std::make_shared<jxx::lang::String>(z_.default_abbrev.c_str());
        --it;
        return std::make_shared<jxx::lang::String>(it->abbrev.c_str());
    }

    jint getRawOffset() const override { return (jint)(z_.default_offset * 1000); }
    jbool useDaylightTime() const override { return true; }

    jbool inDaylightTime(jxx::Ptr<Date> d) const override {
        if (!d) return false;
        jint off = getOffset(d->getTime());
        return off != getRawOffset();
    }
};

static bool load_zone_file(const std::string& filePath, const std::string& zoneId, Zone& z) {
    return load_tzif(filePath, zoneId, z);
}

jxx::Ptr<TimeZone> TimeZone::getTimeZone(jxx::Ptr<jxx::lang::String> id) {
    if (!id) return getDefault();
    std::string zid = id->utf8();

    if (zid == "UTC" || zid == "GMT") return std::make_shared<FixedOffsetTimeZone>(zid, 0);

    if (zid.rfind("GMT", 0) == 0 || zid.rfind("UTC", 0) == 0) {
        jint off = parse_gmt_offset_millis(zid);
        return std::make_shared<FixedOffsetTimeZone>(zid, off);
    }

    std::lock_guard<std::mutex> lk(g_lock);
    auto it = g_zoneCache.find(zid);
    if (it != g_zoneCache.end()) return std::make_shared<TzdbTimeZone>(it->second);

    Zone z;
    std::string path = base_dir() + "/" + zid;
    if (!load_zone_file(path, zid, z)) {
        return std::make_shared<FixedOffsetTimeZone>("UTC", 0);
    }

    g_zoneCache.emplace(zid, z);
    return std::make_shared<TzdbTimeZone>(z);
}

jxx::Ptr<TimeZone> TimeZone::getDefault() {
    const std::string base = base_dir();

    std::lock_guard<std::mutex> lk(g_lock);

#ifdef _WIN32
    // Windows option 2: load %JXX_TIME_ZONE_INFO%/localtime if present
    {
        std::filesystem::path p = std::filesystem::path(base) / "localtime";
        std::error_code ec;
        if (std::filesystem::exists(p, ec) && !ec) {
            const std::string key = "@localtime";
            auto it = g_zoneCache.find(key);
            if (it != g_zoneCache.end()) return std::make_shared<TzdbTimeZone>(it->second);

            Zone z;
            if (load_zone_file(p.string(), "localtime", z)) {
                g_zoneCache.emplace(key, z);
                return std::make_shared<TzdbTimeZone>(z);
            }
        }
    }
    return std::make_shared<FixedOffsetTimeZone>("UTC", 0);
#else
    std::filesystem::path etcLocal("/etc/localtime");
    std::error_code ec;

    if (std::filesystem::exists(etcLocal, ec) && !ec) {
        // If symlink into zoneinfo base, derive IANA id
        if (std::filesystem::is_symlink(etcLocal, ec) && !ec) {
            auto target = std::filesystem::read_symlink(etcLocal, ec);
            if (!ec) {
                std::filesystem::path absTarget = target.is_absolute() ? target : (etcLocal.parent_path() / target);
                absTarget = std::filesystem::weakly_canonical(absTarget, ec);
                if (!ec) {
                    std::filesystem::path basePath = std::filesystem::weakly_canonical(base, ec);
                    if (!ec) {
                        auto absStr = absTarget.generic_string();
                        auto baseStr = basePath.generic_string();
                        if (absStr.rfind(baseStr + "/", 0) == 0) {
                            std::string zoneId = absStr.substr(baseStr.size() + 1);
                            auto it2 = g_zoneCache.find(zoneId);
                            if (it2 != g_zoneCache.end()) return std::make_shared<TzdbTimeZone>(it2->second);

                            Zone z;
                            std::string p = base + "/" + zoneId;
                            if (load_zone_file(p, zoneId, z)) {
                                g_zoneCache.emplace(zoneId, z);
                                return std::make_shared<TzdbTimeZone>(z);
                            }
                        }
                    }
                }
            }
        }

        // Otherwise load /etc/localtime directly as TZif
        {
            const std::string key = "@etc_localtime";
            auto it = g_zoneCache.find(key);
            if (it != g_zoneCache.end()) return std::make_shared<TzdbTimeZone>(it->second);

            Zone z;
            if (load_zone_file(etcLocal.string(), "localtime", z)) {
                g_zoneCache.emplace(key, z);
                return std::make_shared<TzdbTimeZone>(z);
            }
        }
    }

    return std::make_shared<FixedOffsetTimeZone>("UTC", 0);
#endif
}

} // namespace jxx::util
''')

    # ---------------- Date + Calendar ----------------
    write_file(root/"jxx/util/Date.h", r'''#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"

namespace jxx::util {

class Date : public jxx::lang::Object {
public:
    Date();
    explicit Date(jlong epochMillis);

    jlong getTime() const;
    void setTime(jlong epochMillis);

private:
    jlong time_ = 0;
};

} // namespace jxx::util
''')

    write_file(root/"jxx/util/Date.cpp", r'''#include "jxx.util.Date.h"

#include <ctime>

namespace jxx::util {

Date::Date() {
    std::time_t now = std::time(nullptr);
    time_ = (jlong)now * 1000;
}

Date::Date(jlong epochMillis) : time_(epochMillis) {}

jlong Date::getTime() const { return time_; }
void Date::setTime(jlong epochMillis) { time_ = epochMillis; }

} // namespace jxx::util
''')

    write_file(root/"jxx/util/Calendar.h", r'''#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.util.Date.h"
#include "jxx.util.TimeZone.h"

namespace jxx::util {

class Calendar : public jxx::lang::Object {
public:
    // Java 8 field constants (subset)
    static constexpr jint YEAR = 1;
    static constexpr jint MONTH = 2;          // 0-based
    static constexpr jint DAY_OF_MONTH = 5;
    static constexpr jint DAY_OF_WEEK = 7;    // 1=Sunday..7=Saturday
    static constexpr jint HOUR_OF_DAY = 11;
    static constexpr jint MINUTE = 12;
    static constexpr jint SECOND = 13;
    static constexpr jint MILLISECOND = 14;

    Calendar();
    static jxx::Ptr<Calendar> getInstance();

    jlong getTimeInMillis() const;
    void setTimeInMillis(jlong millis);

    jxx::Ptr<Date> getTime() const;
    void setTime(jxx::Ptr<Date> date);

    jxx::Ptr<TimeZone> getTimeZone() const;
    void setTimeZone(jxx::Ptr<TimeZone> tz);

    jint get(jint field) const;

private:
    jlong millis_ = 0;
    jxx::Ptr<TimeZone> tz_;

    static void civil_from_days(int z, int& y, unsigned& m, unsigned& d);
};

using Calender = Calendar;

} // namespace jxx::util
''')

    write_file(root/"jxx/util/Calendar.cpp", r'''#include "jxx.util.Calendar.h"

#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.IndexOutOfBoundsException.h"
#include "jxx.lang.String.h"

#include <ctime>

namespace jxx::util {

void Calendar::civil_from_days(int z, int& y, unsigned& m, unsigned& d) {
    z += 719468;
    const int era = (z >= 0 ? z : z - 146096) / 146097;
    const unsigned doe = (unsigned)(z - era * 146097);
    const unsigned yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;
    y = (int)yoe + era * 400;
    const unsigned doy = doe - (365*yoe + yoe/4 - yoe/100);
    const unsigned mp = (5*doy + 2)/153;
    d = doy - (153*mp+2)/5 + 1;
    m = mp + (mp < 10 ? 3 : -9);
    y += (m <= 2);
}

static inline void epoch_to_local_parts(jlong epochMillis, jint tzOffsetMillis,
                                       int& y, unsigned& mo, unsigned& da,
                                       int& hh, int& mm, int& ss, int& ms,
                                       int& dow_java) {
    long long local = (long long)epochMillis + (long long)tzOffsetMillis;

    ms = (int)(local % 1000);
    if (ms < 0) ms += 1000;

    long long totalSec = local / 1000;
    if (local < 0 && (local % 1000)) --totalSec;

    long long secOfDay = totalSec % 86400;
    if (secOfDay < 0) secOfDay += 86400;

    hh = (int)(secOfDay / 3600);
    mm = (int)((secOfDay % 3600) / 60);
    ss = (int)(secOfDay % 60);

    long long days = totalSec / 86400;
    if (totalSec < 0 && (totalSec % 86400)) --days;

    Calendar::civil_from_days((int)days, y, mo, da);

    int sun0 = (int)((days + 4) % 7);
    if (sun0 < 0) sun0 += 7;
    dow_java = sun0 + 1;
}

Calendar::Calendar() {
    std::time_t now = std::time(nullptr);
    millis_ = (jlong)now * 1000;
    tz_ = TimeZone::getDefault();
}

jxx::Ptr<Calendar> Calendar::getInstance() {
    return std::make_shared<Calendar>();
}

jlong Calendar::getTimeInMillis() const { return millis_; }
void Calendar::setTimeInMillis(jlong millis) { millis_ = millis; }

jxx::Ptr<Date> Calendar::getTime() const { return std::make_shared<Date>(millis_); }

void Calendar::setTime(jxx::Ptr<Date> date) {
    if (!date) throw jxx::lang::NullPointerException(std::make_shared<jxx::lang::String>("null"));
    millis_ = date->getTime();
}

jxx::Ptr<TimeZone> Calendar::getTimeZone() const {
    return tz_ ? tz_ : TimeZone::getDefault();
}

void Calendar::setTimeZone(jxx::Ptr<TimeZone> tz) {
    if (!tz) throw jxx::lang::NullPointerException(std::make_shared<jxx::lang::String>("null"));
    tz_ = std::move(tz);
}

jint Calendar::get(jint field) const {
    auto tz = getTimeZone();
    jint off = tz ? tz->getOffset(millis_) : 0;

    int y, hh, mm, ss, ms, dow;
    unsigned mo, da;
    epoch_to_local_parts(millis_, off, y, mo, da, hh, mm, ss, ms, dow);

    switch (field) {
        case YEAR: return (jint)y;
        case MONTH: return (jint)(mo - 1);
        case DAY_OF_MONTH: return (jint)da;
        case HOUR_OF_DAY: return (jint)hh;
        case MINUTE: return (jint)mm;
        case SECOND: return (jint)ss;
        case MILLISECOND: return (jint)ms;
        case DAY_OF_WEEK: return (jint)dow;
        default:
            throw jxx::lang::IndexOutOfBoundsException(std::make_shared<jxx::lang::String>("Unsupported Calendar field"));
    }
}

} // namespace jxx::util
''')

    # ---------------- formatter exceptions ----------------
    write_file(root/"jxx/util/format/IllegalFormatException.h", r'''#pragma once
#include "jxx.lang.RuntimeException.h"
#include "jxx.lang.String.h"

namespace jxx::util::format {

class IllegalFormatException : public jxx::lang::RuntimeException {
public:
    IllegalFormatException() = default;
    explicit IllegalFormatException(jxx::Ptr<jxx::lang::String> msg)
        : jxx::lang::RuntimeException(std::move(msg)) {}
};

} // namespace jxx::util::format
''')

    write_file(root/"jxx/util/format/MissingFormatArgumentException.h", r'''#pragma once
#include "jxx.util.format.IllegalFormatException.h"

namespace jxx::util::format {

class MissingFormatArgumentException : public IllegalFormatException {
public:
    explicit MissingFormatArgumentException(jxx::Ptr<jxx::lang::String> msg)
        : IllegalFormatException(std::move(msg)) {}
};

} // namespace jxx::util::format
''')

    write_file(root/"jxx/util/format/UnknownFormatConversionException.h", r'''#pragma once
#include "jxx.util.format.IllegalFormatException.h"

namespace jxx::util::format {

class UnknownFormatConversionException : public IllegalFormatException {
public:
    explicit UnknownFormatConversionException(jxx::Ptr<jxx::lang::String> msg)
        : IllegalFormatException(std::move(msg)) {}
};

} // namespace jxx::util::format
''')

    # ---------------- Formatter (wired) ----------------
    write_file(root/"jxx/util/Formatter.h", r'''#pragma once

#include "jxx_types.h"
#include "jxx.lang.String.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.Locale.h"

namespace jxx::util {

struct FormatSpec {
    jint argIndex = -1;
    jbool reuseLast = false;

    jbool leftJustify = false;
    jbool alt = false;
    jbool plus = false;
    jbool leadingSpace = false;
    jbool zeroPad = false;
    jbool group = false;
    jbool parens = false;

    jint width = -1;
    jint precision = -1;

    jbool dateTime = false;
    jbool upper = false;
    char conv = 0;
};

class Formatter final {
public:
    explicit Formatter(jxx::Ptr<jxx::lang::Locale> locale = nullptr);

    jxx::Ptr<jxx::lang::String> format(
        jxx::Ptr<jxx::lang::String> fmt,
        jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::Object>, 1>> args);

private:
    jxx::Ptr<jxx::lang::Locale> locale_;
};

} // namespace jxx::util
''')

    # This Formatter.cpp supports the tz wiring + locale names + basic conversions.
    write_file(root/"jxx/util/Formatter.cpp", r'''#include "jxx.util.Formatter.h"

#include "jxx.util.TimeZone.h"
#include "jxx.util.Date.h"
#include "jxx.util.Calendar.h"

#include "jxx.util.format.IllegalFormatException.h"
#include "jxx.util.format.MissingFormatArgumentException.h"
#include "jxx.util.format.UnknownFormatConversionException.h"

#include "jxx.lang.String.h"
#include "jxx.lang.Object.h"

// Wrapper types (adjust include paths if needed)
#include "jxx.lang.Integer.h"
#include "jxx.lang.Long.h"
#include "jxx.lang.Short.h"
#include "jxx.lang.Byte.h"
#include "jxx.lang.Boolean.h"
#include "jxx.lang.Float.h"
#include "jxx.lang.Double.h"
#include "jxx.lang.Character.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>

namespace jxx::util {

static inline void throw_fmt(const char* msg) {
    throw jxx::util::format::IllegalFormatException(std::make_shared<jxx::lang::String>(msg));
}
static inline void throw_missing(const char* msg) {
    throw jxx::util::format::MissingFormatArgumentException(std::make_shared<jxx::lang::String>(msg));
}
static inline void throw_unknown(const std::string& msg) {
    throw jxx::util::format::UnknownFormatConversionException(std::make_shared<jxx::lang::String>(msg.c_str()));
}

static inline bool is_digit(char c) { return c >= '0' && c <= '9'; }

static inline std::string to_upper_ascii(std::string s) {
    for (auto& ch : s) ch = (char)std::toupper((unsigned char)ch);
    return s;
}
static inline std::string to_lower_ascii(std::string s) {
    for (auto& ch : s) ch = (char)std::tolower((unsigned char)ch);
    return s;
}

static inline bool has_long(jxx::Ptr<jxx::lang::Object> o, long long& out) {
    if (auto i = std::dynamic_pointer_cast<jxx::lang::Integer>(o)) { out = (long long)i->intValue(); return true; }
    if (auto l = std::dynamic_pointer_cast<jxx::lang::Long>(o)) { out = (long long)l->longValue(); return true; }
    if (auto s = std::dynamic_pointer_cast<jxx::lang::Short>(o)) { out = (long long)s->shortValue(); return true; }
    if (auto b = std::dynamic_pointer_cast<jxx::lang::Byte>(o)) { out = (long long)b->byteValue(); return true; }
    return false;
}
static inline bool has_double(jxx::Ptr<jxx::lang::Object> o, double& out) {
    if (auto f = std::dynamic_pointer_cast<jxx::lang::Float>(o)) { out = (double)f->floatValue(); return true; }
    if (auto d = std::dynamic_pointer_cast<jxx::lang::Double>(o)) { out = (double)d->doubleValue(); return true; }
    long long li=0; if (has_long(o, li)) { out = (double)li; return true; }
    return false;
}
static inline bool has_bool(jxx::Ptr<jxx::lang::Object> o, bool& out) {
    if (auto b = std::dynamic_pointer_cast<jxx::lang::Boolean>(o)) { out = b->booleanValue(); return true; }
    return false;
}

struct LocalParts { int year; unsigned mon; unsigned day; int hour; int minute; int second; int millis; int wday_java; };

static inline void civil_from_days(int z, int& y, unsigned& m, unsigned& d) {
    z += 719468;
    const int era = (z >= 0 ? z : z - 146096) / 146097;
    const unsigned doe = (unsigned)(z - era * 146097);
    const unsigned yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;
    y = (int)yoe + era * 400;
    const unsigned doy = doe - (365*yoe + yoe/4 - yoe/100);
    const unsigned mp = (5*doy + 2)/153;
    d = doy - (153*mp+2)/5 + 1;
    m = mp + (mp < 10 ? 3 : -9);
    y += (m <= 2);
}

static inline LocalParts epoch_to_local_parts(long long epochMillis, int offsetMillis) {
    long long local = epochMillis + (long long)offsetMillis;
    LocalParts p{};
    p.millis = (int)(local % 1000); if (p.millis < 0) p.millis += 1000;
    long long totalSec = local / 1000; if (local < 0 && (local % 1000)) --totalSec;
    long long secOfDay = totalSec % 86400; if (secOfDay < 0) secOfDay += 86400;
    p.hour = (int)(secOfDay / 3600);
    p.minute = (int)((secOfDay % 3600) / 60);
    p.second = (int)(secOfDay % 60);
    long long days = totalSec / 86400; if (totalSec < 0 && (totalSec % 86400)) --days;
    civil_from_days((int)days, p.year, p.mon, p.day);
    int sun0 = (int)((days + 4) % 7); if (sun0 < 0) sun0 += 7;
    p.wday_java = sun0 + 1;
    return p;
}

static inline std::string locale_time_name(const std::locale& loc, const LocalParts& p, const char* fmt) {
    std::tm tm{};
    tm.tm_year = p.year - 1900;
    tm.tm_mon  = (int)p.mon - 1;
    tm.tm_mday = (int)p.day;
    tm.tm_hour = p.hour;
    tm.tm_min  = p.minute;
    tm.tm_sec  = p.second;
    tm.tm_wday = p.wday_java - 1;
    std::ostringstream oss;
    oss.imbue(loc);
    oss << std::put_time(&tm, fmt);
    return oss.str();
}

static inline void apply_width(std::string& out, const FormatSpec& s) {
    if (s.precision >= 0 && (s.conv=='s' || s.conv=='S') && (int)out.size() > s.precision)
        out.resize((std::size_t)s.precision);

    if (s.width <= 0 || (int)out.size() >= s.width) return;

    int pad = s.width - (int)out.size();
    char padChar = (s.zeroPad && !s.leftJustify) ? '0' : ' ';

    if (s.leftJustify) out.append((std::size_t)pad, ' ');
    else out = std::string((std::size_t)pad, padChar) + out;
}

static void parse_one(const std::string& f, std::size_t& i, FormatSpec& s) {
    // arg index/reuse
    if (i < f.size() && f[i] == '<') { s.reuseLast=true; ++i; }
    else {
        std::size_t start=i;
        while (i<f.size() && is_digit(f[i])) ++i;
        if (i<f.size() && f[i]=='$' && i>start) { s.argIndex=std::stoi(f.substr(start,i-start)); ++i; }
        else i=start;
    }
    // flags (minimal set needed for common Java usage)
    while (i<f.size()) {
        switch (f[i]) {
            case '-': s.leftJustify=true; ++i; break;
            case '0': s.zeroPad=true; ++i; break;
            default: goto flags_done;
        }
    }
flags_done:
    // width
    if (i<f.size() && is_digit(f[i])) {
        std::size_t start=i;
        while (i<f.size() && is_digit(f[i])) ++i;
        s.width = std::stoi(f.substr(start,i-start));
    }
    // precision
    if (i<f.size() && f[i]=='.') {
        ++i;
        std::size_t start=i;
        while (i<f.size() && is_digit(f[i])) ++i;
        s.precision = (i==start) ? 0 : std::stoi(f.substr(start,i-start));
    }
    // datetime
    if (i<f.size() && (f[i]=='t' || f[i]=='T')) { s.dateTime=true; s.upper=(f[i]=='T'); ++i; }
    if (i>=f.size()) throw_missing("MissingFormatArgumentException");
    s.conv=f[i];
    if (std::isupper((unsigned char)s.conv)) s.upper=true;
}

Formatter::Formatter(jxx::Ptr<jxx::lang::Locale> locale)
    : locale_(locale ? locale : jxx::lang::Locale::getDefault()) {}

jxx::Ptr<jxx::lang::String> Formatter::format(jxx::Ptr<jxx::lang::String> fmt,
    jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::Object>, 1>> args) {

    if (!fmt || !args) throw_fmt("NullPointerException");

    std::string f = fmt->utf8();
    std::ostringstream out; out.imbue(std::locale::classic());

    jint nextArg=1;
    jint lastArg=-1;

    const std::locale& loc = locale_->cppLocale();

    for (std::size_t i=0;i<f.size();++i) {
        if (f[i] != '%') { out<<f[i]; continue; }
        if (i+1<f.size() && f[i+1]=='%') { out<<'%'; ++i; continue; }

        ++i;
        FormatSpec spec{};
        parse_one(f, i, spec);

        if (spec.conv=='n') { out<<"\n"; continue; }
        if (spec.conv=='%') { out<<"%"; continue; }

        jint argIndex = -1;
        if (spec.reuseLast) {
            if (lastArg<0) throw_missing("MissingFormatArgumentException");
            argIndex=lastArg;
        } else if (spec.argIndex>0) argIndex=spec.argIndex;
        else argIndex=nextArg++;
        lastArg=argIndex;

        if (argIndex<=0 || (std::uint32_t)argIndex > args->length) throw_missing("MissingFormatArgumentException");
        auto a = (*args)[argIndex-1];

        std::string chunk;

        if (spec.dateTime) {
            if (!a) throw_fmt("NullPointerException");

            long long epoch=0;
            jxx::Ptr<jxx::util::TimeZone> tz;

            if (auto cal = std::dynamic_pointer_cast<jxx::util::Calender>(a)) {
                epoch = (long long)cal->getTimeInMillis();
                tz = cal->getTimeZone();
                if (!tz) tz = jxx::util::TimeZone::getDefault();
            } else if (auto d = std::dynamic_pointer_cast<jxx::util::Date>(a)) {
                epoch = (long long)d->getTime();
                tz = jxx::util::TimeZone::getDefault();
            } else {
                long long v=0;
                if (!has_long(a, v)) throw_fmt("IllegalFormatConversionException: t/T");
                epoch=v; tz=jxx::util::TimeZone::getDefault();
            }

            jint off = tz ? tz->getOffset((jlong)epoch) : 0;
            LocalParts lp = epoch_to_local_parts(epoch, (int)off);

            auto make2 = [](int v){ std::ostringstream o; o<<std::setw(2)<<std::setfill('0')<<v; return o.str(); };
            auto make3 = [](int v){ std::ostringstream o; o<<std::setw(3)<<std::setfill('0')<<v; return o.str(); };
            auto make4 = [](int v){ std::ostringstream o; o<<std::setw(4)<<std::setfill('0')<<v; return o.str(); };

            switch (spec.conv) {
                case 'Y': chunk=make4(lp.year); break;
                case 'm': chunk=make2((int)lp.mon); break;
                case 'd': chunk=make2((int)lp.day); break;
                case 'H': chunk=make2(lp.hour); break;
                case 'M': chunk=make2(lp.minute); break;
                case 'S': chunk=make2(lp.second); break;
                case 'L': chunk=make3(lp.millis); break;
                case 'z': {
                    int offSec = (int)off / 1000;
                    char sign='+'; if (offSec<0) { sign='-'; offSec=-offSec; }
                    int hh=offSec/3600; int mm=(offSec%3600)/60;
                    std::ostringstream o; o<<sign<<std::setw(2)<<std::setfill('0')<<hh<<std::setw(2)<<std::setfill('0')<<mm;
                    chunk=o.str();
                    break;
                }
                case 'Z': chunk = tz ? tz->getAbbreviation((jlong)epoch)->utf8() : std::string("UTC"); break;
                case 'a': chunk = locale_time_name(loc, lp, "%a"); break;
                case 'A': chunk = locale_time_name(loc, lp, "%A"); break;
                case 'b': chunk = locale_time_name(loc, lp, "%b"); break;
                case 'B': chunk = locale_time_name(loc, lp, "%B"); break;
                case 'p': chunk = to_lower_ascii(locale_time_name(loc, lp, "%p")); break;
                default: throw_unknown(std::string("UnknownFormatConversionException: t") + spec.conv);
            }
            if (spec.upper) chunk = to_upper_ascii(chunk);

        } else if (spec.conv=='s' || spec.conv=='S') {
            chunk = a ? a->toString()->utf8() : std::string("null");
            if (spec.upper) chunk = to_upper_ascii(chunk);

        } else if (spec.conv=='b' || spec.conv=='B') {
            bool bv=false;
            if (!a) bv=false;
            else if (!has_bool(a, bv)) bv=true;
            chunk = bv ? "true" : "false";
            if (spec.upper) chunk = to_upper_ascii(chunk);

        } else if (spec.conv=='d') {
            long long v=0; if (!has_long(a, v)) throw_fmt("IllegalFormatConversionException: d");
            chunk = std::to_string(v);

        } else if (spec.conv=='f') {
            double v=0; if (!has_double(a, v)) throw_fmt("IllegalFormatConversionException: f");
            std::ostringstream o; o.imbue(std::locale::classic());
            int prec = (spec.precision>=0) ? spec.precision : 6;
            o<<std::fixed<<std::setprecision(prec)<<v;
            chunk=o.str();

        } else {
            throw_unknown(std::string("UnknownFormatConversionException: ") + spec.conv);
        }

        apply_width(chunk, spec);
        out<<chunk;
    }

    return std::make_shared<jxx::lang::String>(out.str().c_str());
}

} // namespace jxx::util
''')

    # ---------------- tests ----------------
    write_file(root/"tests/test_timezone.cpp", r'''#include <gtest/gtest.h>
#include <cstdlib>

#include "jxx.util.TimeZone.h"
#include "jxx.lang.String.h"

static jxx::Ptr<jxx::util::TimeZone> tz(const char* id) {
    return jxx::util::TimeZone::getTimeZone(std::make_shared<jxx::lang::String>(id));
}

static void ensure_zoneinfo() {
#ifdef _WIN32
    const char* base = std::getenv("JXX_TIME_ZONE_INFO");
    if (!base) {
        GTEST_SKIP() << "JXX_TIME_ZONE_INFO not set on Windows";
    }
#endif
}

TEST(TimeZoneOffsets, NewYork_DST_2024) {
    ensure_zoneinfo();
    auto ny = tz("America/New_York");

    long long t1 = 1710053999LL * 1000; // 2024-03-10 06:59:59 UTC
    EXPECT_EQ(ny->getOffset(t1), -5 * 3600 * 1000);
    EXPECT_EQ(ny->getAbbreviation(t1)->utf8(), "EST");

    long long t2 = 1710054000LL * 1000; // 2024-03-10 07:00:00 UTC
    EXPECT_EQ(ny->getOffset(t2), -4 * 3600 * 1000);
    EXPECT_EQ(ny->getAbbreviation(t2)->utf8(), "EDT");

    long long t3 = 1730613599LL * 1000; // 2024-11-03 05:59:59 UTC
    EXPECT_EQ(ny->getOffset(t3), -4 * 3600 * 1000);
    EXPECT_EQ(ny->getAbbreviation(t3)->utf8(), "EDT");

    long long t4 = 1730613600LL * 1000; // 2024-11-03 06:00:00 UTC
    EXPECT_EQ(ny->getOffset(t4), -5 * 3600 * 1000);
    EXPECT_EQ(ny->getAbbreviation(t4)->utf8(), "EST");
}

TEST(TimeZoneOffsets, Berlin_DST_2024) {
    ensure_zoneinfo();
    auto de = tz("Europe/Berlin");

    long long t1 = 1711846799LL * 1000; // 2024-03-31 00:59:59 UTC
    EXPECT_EQ(de->getOffset(t1), 1 * 3600 * 1000);

    long long t2 = 1711846800LL * 1000; // 2024-03-31 01:00:00 UTC
    EXPECT_EQ(de->getOffset(t2), 2 * 3600 * 1000);

    long long t3 = 1729990799LL * 1000; // 2024-10-27 00:59:59 UTC
    EXPECT_EQ(de->getOffset(t3), 2 * 3600 * 1000);

    long long t4 = 1729990800LL * 1000; // 2024-10-27 01:00:00 UTC
    EXPECT_EQ(de->getOffset(t4), 1 * 3600 * 1000);
}
''')

    write_file(root/"tests/test_formatter_datetime.cpp", r'''#include <gtest/gtest.h>
#include <cstdlib>

#include "jxx.util.Formatter.h"
#include "jxx.util.Calendar.h"
#include "jxx.util.TimeZone.h"
#include "jxx.lang.String.h"
#include "jxx.lang.Object.h"

static void ensure_zoneinfo() {
#ifdef _WIN32
    const char* base = std::getenv("JXX_TIME_ZONE_INFO");
    if (!base) {
        GTEST_SKIP() << "JXX_TIME_ZONE_INFO not set on Windows";
    }
#endif
}

static jxx::Ptr<jxx::util::TimeZone> tz(const char* id) {
    return jxx::util::TimeZone::getTimeZone(std::make_shared<jxx::lang::String>(id));
}

TEST(FormatterDateTime, TZ_And_Abbrev) {
    ensure_zoneinfo();

    auto cal = jxx::util::Calender::getInstance();
    cal->setTimeZone(tz("America/New_York"));
    cal->setTimeInMillis(1710053999LL * 1000);

    auto fmt = std::make_shared<jxx::lang::String>("%1$tz %1$tZ");
    auto args = std::make_shared<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::Object>,1>>(1);
    (*args)[0] = cal;

    jxx::util::Formatter f;
    auto out = f.format(fmt, args)->utf8();

    EXPECT_EQ(out, "-0500 EST");
}
''')

    write_file(root/"tests/CMakeLists.txt", r'''cmake_minimum_required(VERSION 3.16)
project(jxx_tz_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

add_executable(jxx_tz_tests
  test_timezone.cpp
  test_formatter_datetime.cpp

  ../jxx/util/tz/tzif.cpp
  ../jxx/util/TimeZone.cpp
  ../jxx/util/Date.cpp
  ../jxx/util/Calendar.cpp
  ../jxx/util/Formatter.cpp
)

target_include_directories(jxx_tz_tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(jxx_tz_tests PRIVATE GTest::gtest GTest::gtest_main)

enable_testing()
add_test(NAME jxx_tz_tests COMMAND jxx_tz_tests)
''')

    write_file(root/"README.txt", r'''jxx Java-8 parity timezone + formatter + gtest pack

Key features:
  - TZif parser (zoneinfo) for IANA time zones
  - TimeZone::getDefault() behavior:
      Linux: /etc/localtime symlink -> derive ID, else load file directly
      Windows: %JXX_TIME_ZONE_INFO%/localtime
  - Formatter supports date/time %t/%T tokens:
      Y m d H M S L z Z a A b B p
    and basic conversions: s, b, d, f

Environment variables:
  - JXX_TIME_ZONE_INFO
      Windows: REQUIRED. Points to a directory containing zoneinfo files.
               Must contain 'localtime' TZif for default timezone.
      Linux: optional (defaults to /usr/share/zoneinfo)

Tests:
  - tests/test_timezone.cpp validates offsets/abbrev around DST transitions (NY and Berlin, 2024)
  - tests/test_formatter_datetime.cpp validates %tz and %tZ formatting

Integration:
  - Add generated .cpp files to your runtime build.
  - Ensure wrapper headers exist: jxx.lang.Integer.h, Long.h, etc.
  - Ensure Locale exists with cppLocale().
''')

    if args.zip:
        zip_path = root.with_suffix(".zip")
        zip_folder(zip_path, root)
        print("Wrote zip:", zip_path)

    print("Generated at:", root)

if __name__ == "__main__":
    main()

